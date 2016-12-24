

#include "std_alg.h"

int GetHuman(FilterIn &in, RefBase &ref) {
    SavableClass *tmp;
    in >> tmp;
    if(ref.GetSavableBase(tmp) == NULL) {
        delete tmp;
        return 0;
    }
    return 1;
};


DescribedClass *create_Algorithm_TimeWrite_Gasd_cd() {
    return new Algorithm_TimeWrite_Gasd;
}
DescribedClass *create_Algorithm_TimeWrite_cd() {
    return new Algorithm_TimeWrite;
}

static ClassDesc Algorithm_TimeWrite_Gasd_cd(
    typeid(Algorithm_TimeWrite_Gasd),
    "Algorithm_TimeWrite_Gasd",
    1,
    "Algorithm_TimeWrite_Category",
    &create_Algorithm_TimeWrite_Gasd_cd);
static ClassDesc Algorithm_TimeWrite_cd(
    typeid(Algorithm_TimeWrite),
    "Algorithm_TimeWrite",
    1,
    "Algorithm_TimeWrite_Category",
    &create_Algorithm_TimeWrite_cd);


DescribedClass *create_Algorithm_MakeStp_cd() {
    return new Algorithm_MakeStp;
}
DescribedClass *create_Algorithm_MakeStp_Gasd_cd() {
    return new Algorithm_MakeStp_Gasd;
}

static ClassDesc Algorithm_MakeStp_cd(
    typeid(Algorithm_MakeStp),
    "Algorithm_MakeStp",
    1,
    "Algorithm_MakeStp_Category",
    &create_Algorithm_MakeStp_cd);
static ClassDesc Algorithm_MakeStp_Gasd_cd(
    typeid(Algorithm_MakeStp_Gasd),
    "Algorithm_MakeStp_Gasd",
    1,
    "Algorithm_MakeStp_Category",
    &create_Algorithm_MakeStp_Gasd_cd);

DescribedClass *create_Config_Gasd_cd() {
    return new Config_Gasd;
}

static ClassDesc Config_Gasd_cd(
    typeid(Config_Gasd),
    "Config_Gasd",
    1,
    "Config_Gasd_Category",
    &create_Config_Gasd_cd);

//=======================================================================
//======================   Algorithm_MainAction     =====================
//=======================================================================

int Algorithm_MainAction::SetParam(int argc, char **argv) {
    NumArg = argc;
    CharArg = argv;
    if(GetCmd("/h", NumArg - 1, CharArg) != NULL) {
        PrintHelp();
        return 0;
    }
    return 1;
}
void Algorithm_MainAction::PrintHelp() {
    fcout << " usage " << CharArg[0] << "\n keys :\n";
    fcout << "       /h - print help\n";
};


//=======================================================================
//======================   Algorithm_MainAction_Gasd     ================
//=======================================================================


int Algorithm_MainAction_Gasd::SetParam(int argc, char **argv) {
    if(!Algorithm_MainAction::SetParam(argc, argv))
        return 0;
    if(NumArg < 2) {
        PrintHelp();
        return 0;
    }
    char *ReadFrom = CharArg[1], *WriteTo = CharArg[2];
    Delete();
    in_ptr = new FilterTextIn(ReadFrom);
    in_ptr->SetRefMask(SavableClass::UnlinkedObjects);
    out_ptr = new FilterTextOut(WriteTo);
    out_ptr->SetRefMask(SavableClass::UnlinkedObjects);
    //  SavableClass *stp,*clc;
    //  (*in_ptr)>>StdKey>>stp>>StdKey>>clc;
    //  clc_ptr<<clc;time_ptr<<stp;
    //  if (clc_ptr==NULL) delete clc;if (time_ptr==NULL) delete stp;
    *in_ptr >> StdKey;
    GetHuman(*in_ptr, time_ptr);
    *in_ptr >> StdKey;
    GetHuman(*in_ptr, clc_ptr);
    if((in_ptr->fail()) || (time_ptr == NULL) || (clc_ptr == NULL))
        return 0;
    //  Delete();

    *out_ptr << KeyDesc("Algorithm_TimeWrite\n") << (SavableClass *)time_ptr
             << KeyDesc("Algorithm_MakeStp\n") << (SavableClass *)clc_ptr;
    if(!clc_ptr->SetParam(this, time_ptr, in_ptr, out_ptr))
        return 0;
    return 1;
}
void Algorithm_MainAction_Gasd::WriteRes() {
    double TimeWas = clc_ptr->GetStartTime(time_ptr->GetStartTime());
    time_ptr->GetStartTime(TimeWas);
    double TimeNew, MaxTimeStp, BoundWaveAmp;
    int WriteRes = 0;
    TimeNew = TimeWas;
    do {
        clc_ptr->MakeTimeStp(TimeWas, TimeNew, MaxTimeStp, BoundWaveAmp);
        if(WriteRes == 2)
            clc_ptr->WriteRes(TimeNew, *out_ptr);
        TimeWas = TimeNew;
    } while(WriteRes = time_ptr->GetNextTime(TimeNew, MaxTimeStp, BoundWaveAmp));
};

void Algorithm_MainAction_Gasd::PrintHelp() {
    fcout.SetRefMask(SavableClass::UnlinkedObjects);
    fcout << " usage " << CharArg[0] << "File_cfg File_res /keys\n keys :\n";
    fcout << "       /h - print help\n";
    fcout
        << " File_cfg struct \n Algorithm_TimeWrite Algorithm_TimeWrite_Object Algorithm_MakeStp Algorithm_MakeStp_Object\n";
    fcout << " Currently present Algorithm_TimeWrite classes\n";
    fcout << SavableClass::HelpForCategory("Algorithm_TimeWrite_Category", 1);
    fcout << " Currently present Algorithm_MakeStp classes\n";
    fcout << SavableClass::HelpForCategory("Algorithm_MakeStp_Category", 1);
    fcout.flush();
};


//=======================================================================
//======================   Algorithm_TimeWrite_Gasd  ====================
//=======================================================================


int Algorithm_TimeWrite_Gasd::GetNextTime(
    double &Time,
    double MaxTimeStp,
    double BoundWaveAmp) {

    if(fabs(Time - EndTime) < MathZer)
        return 0;
    if(fabs(Time - NewTimeWrite) < MathZer) {
        fcout << " CurStp " << (NewTimeWrite - OldTimeWrite) / NumWriteSteps
              << " CurTime " << Time << "\n";
        NewTimeWrite =
            min(Time + TimeWriteStp / (1 + BoundWaveAmp * BoundWaveCoef), EndTime);
        OldTimeWrite = Time;
        NumWriteSteps = 0;

        int BreakSignal = 0, ESC = 27;
        if(_kbhit()) {
            BreakSignal = _getch();   //fcout<<BreakSignal<<"\n";
            if(BreakSignal == ESC)
                return 0;
        }
        //    while ((BreakSignal=bioskey(1))!=0) if (BreakSignal==ESC) return 0;
        //    else {fcout<<" "<<BreakSignal;bioskey(0);}
    }

    double CurStp = MaxTimeStp * StabilityCoef;
    if(NumIter < 100) {
        NumIter++;
        CurStp *= 0.5 * log10((double)NumIter);
    }
    if(CurStp > OldStp)
        CurStp = OldStp * (IncreaseStepCoef * log(CurStp / OldStp) + 1);
    int ret = 1;
    if(CurStp + Time >= NewTimeWrite) {
        CurStp = NewTimeWrite - Time;
        ret = 2;
    }

    OldStp = CurStp;
    NumWriteSteps++;
    Time += CurStp;
    return ret;
};
