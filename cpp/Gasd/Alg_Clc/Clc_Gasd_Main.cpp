

#include "clc_Gasd_Main.h"


DescribedClass *create_GasdCalculator_cd() {
    return new GasdCalculator;
}
DescribedClass *create_Config_Gasd_ReadPar_cd() {
    return new Config_Gasd_ReadPar;
}

DescribedClass *create_GasdCalculator_Simple1D_cd() {
    return new GasdCalculator_Simple1D;
}

static ClassDesc GasdCalculator_cd(
    typeid(GasdCalculator),
    "GasdCalculator",
    1,
    "GasdCalculator_Category",
    &create_GasdCalculator_cd);
static ClassDesc Config_Gasd_ReadPar_cd(
    typeid(Config_Gasd_ReadPar),
    "Config_Gasd_ReadPar",
    1,
    "Config_Gasd_Category",
    &create_Config_Gasd_ReadPar_cd);


static ClassDesc GasdCalculator_Simple1D_cd(
    typeid(GasdCalculator_Simple1D),
    "GasdCalculator_Simple1D",
    1,
    "GasdCalculator_Category",
    &create_GasdCalculator_Simple1D_cd);

//=======================================================================
//======================       GasdCalculator        ====================
//=======================================================================
#include "Gasd/Topology/grid_util.h"
FieldInterface *GasdCalculator::CreateGrid() {
    //  Ref<GridCenter> stor;//=new GridCenter;//stor.unmanage();
    // AccuracySpace;
    double add = (double)AccuracySpace * Length / (NumPnt), Shift = 0;
    SpacePoint low(WorkDim), up(WorkDim);
    low = Shift - add;
    up = Length + Shift + add;
    SpaceBound bound(low, up);
    IndexPoint lowi(WorkDim), upi(WorkDim);
    lowi = (int)0;
    upi = (int)NumPnt + 2 * AccuracySpace;
    IndBound index(lowi, upi);

    area = new AreaStoreInterface;
    area->CreateArea(new TriangulatorSingleSquare(bound, index));
    grid_manip = area->GetGridManip();
    grid_field = area->GetGridField();


    GridField<int> *tmp = new GridField<int>(grid_manip, reader->MaskStoredPoints);
    mask_manip = area->GetMaskStr();
    MaskManipulatorGeneral *mask_gen = area->GetMaskGen();
    MaskAll = mask_gen->MaskAll();
    MaskMain = mask_gen->MaskBoundary(AccuracySpace - 1);
    MaskMain = mask_gen->RemoveMask(MaskAll, MaskMain);
    Mask2Dat(MaskMain, tmp);
    // AccuracySpace;
    return grid_field;
};


//=======================================================================
//======================   Config_Gasd_ReadPar       ====================
//=======================================================================


int Config_Gasd_ReadPar::ReadData(FilterIn &in, FilterOut &out) {
    if(NotToReadPar) {
        ReadIsGood = 1;
        calc->StartCalculation();
        WriteRes(0, out);
        return 1;
    }
    ReadIsGood = 0;
    GridManipulator *grid = stor->GetGridManip();
    if(!stor) {
        fcout << " Config_Gasd_ReadPar::ReadData, stor is null\n";
        fcout.flush();
        return 0;
    }
    DataVector<int> mask = Dat2Mask(grid->GetVar(MaskStoredPoints));
    int Npnt = mask.SetDim(), Nvar = data_names.SetNumEl(), k, k1;
    DataArray<double> arr(Nvar, Npnt);
    DataVector<SpacePoint, CopyStructSlow<SpacePoint>> coord(Npnt);
    char tmp[256];
    in >> tmp >> ReadTime;
    for(k = 0; k < Npnt; k++) {
        if(!ReadSpaceAsDouble)
            in >> coord[k];
        else {
            coord[k].SetDim(SpaceDim);
            for(k1 = 0; k1 < SpaceDim; k1++)
                in >> coord[k][k1];
        }
        for(k1 = 0; k1 < Nvar; k1++)
            in >> arr(k, k1);
    }
    if(in.fail())
        return 0;
    grid->SetGridMaskedPnt(coord, mask);
    for(k = 0; k < Nvar; k++)
        ((GridField<double, CopyStructFast<double>> *)(grid->GetVar(data_names[k].name)))
            ->SetMaskedPnt(arr.SetColumn<CopyStructFast<double>>(k), mask);
    in >> tmp;
    if(Stricmp(tmp, "EndIter") == 0)
        ReadIsGood = 1;
    calc->StartCalculation();
    return 1;
};

void Config_Gasd_ReadPar::WriteRes(double TimeNew, FilterOut &out) {
    out << "CurTime" << TimeNew << "\n";

    GridManipulator *grid = stor->GetGridManip();
    //fcout<<HEAD("Writing Begin")<<*grid->GetGridField()<<ENDL;
    if(!stor) {
        fcout << " Config_Gasd_ReadPar::WriteRes, stor is null\n";
        fcout.flush();
        return;
    }
    DataVector<GridIndex> mask = Dat2Mask(grid->GetVar("StoredPoints"));
    int Npnt = mask.SetDim(), Nvar = data_names.SetNumEl(), k, k1;
    DataArray<double> arr;
    DataVector<SpacePoint, CopyStructSlow<SpacePoint>> coord;
    coord = grid->GetGridMaskedPnt(mask);
    for(k = 0; k < Nvar; k++)
        arr.SetColumn(
            k,
            ((GridField<double> *)(grid->GetVar(data_names[k].name)))
                ->GetMaskedPnt(mask));
    for(k = 0; k < Npnt; k++) {
        if(!ReadSpaceAsDouble)
            out << coord[k];
        else {
            for(k1 = 0; k1 < SpaceDim; k1++)
                out << coord[k][k1];
        }
        for(k1 = 0; k1 < Nvar; k1++)
            out << arr(k, k1);
        out << "\n";
    }
    out << "EndIter"
        << "\n";
    //fcout<<HEAD("Writing End")<<*grid->GetGridField()<<ENDL;
};


int Config_Gasd_ReadPar::save_data_state(FilterOut &so) {
    int NumVar = data_names.SetNumEl();
    so << "NumStoredVars" << NumVar << "There_Names";
    for(int k = 0; k < NumVar; k++)
        so << data_names[k];
    so << KeyDesc("\nSpaceDim") << SpaceDim << KeyDesc("ReadSpaceAsDouble")
       << ReadSpaceAsDouble;
    so << KeyDesc("NotToReadPar") << NotToReadPar;
    so << KeyDesc("\nGasdCalculator") << (SavableClass *)calc;
    return !so.fail();
};

int Config_Gasd_ReadPar::read_data_state(FilterIn &si) {
    int NumVar, k;
    si >> StdKey >> NumVar >> StdKey;
    for(k = 0; k < NumVar; k++)
        si >> data_names[k];
    si >> StdKey >> SpaceDim >> StdKey >> ReadSpaceAsDouble >> StdKey >> NotToReadPar >>
        StdKey;
    GetHuman(si, calc);
    if(calc != NULL) {
        calc->reader = this;
        stor = calc->CreateGrid();
        GridManipulator *grid = stor->GetGridManip();
        GridField<double> *tmp;
        for(k = 0; k < NumVar; k++)
            tmp = new GridField<double>(grid, data_names[k].name);
    }
    return stor != NULL;
}
