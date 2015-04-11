
#pragma once

#include "gasd/polygon/lib/PolyMarchBase.h"
#include "lib/std/ex_out.h"
#include "FluxPrepare.h"


namespace NPolygon {

    struct TPolyMarchTestU2 : TPolyMarchRegionBase {
        TRegBoundaryCircle Bnd;
        TRegBoundaryCircleX BndX;
        TPolyMarchTestU2();
        virtual double GetMaxTimeStp(TPolyRegion *reg) {
            return 1;
        };
        virtual void MakeTimeStep(TPolyRegion *reg, double curTime, double timeStp) {

            //TGridMaskedData cur(this, curMask);
            //TGridMaskedData::TIterator dencIter(cur), posIter(mdata);
            //TGridVar *ptr = SavableClass::TestType<TGridVar>(mdata.Data);
            //vector<T> &tmpStor = ptr->VectorT;
            //for( ; curIter.IsOk() && rgtIter.IsOk(); curIter.Next(), rgtIter.Next()) 
            //    VectorT[curIter.GetElementInd()] = tmpStor[rgtIter.GetElementInd()];


            MakeTimeStepTestTime(reg, timeStp);
            MakeTimeStepTestVector(reg, timeStp);

        }
        void MakeTimeStepTestVector(TPolyRegion *reg, double timeStp) {
            if (! reg->Grid.VarExists("XY"))
                TFluxPrepare::SetVectorVar(reg, "X Y", "XY");

            TGridMaskedData src = reg->Grid.GetMaskedData(TRegionBounds(0,0), "XY");
            src += 10;
        }
        void MakeTimeStepTestTime(TPolyRegion *reg, double timeStp) {
            TGridMaskedData src = reg->Grid.GetMaskedData(TRegionBounds(0,0), "Y");
            TGridVar<double> *srcData = SavableClass::TestType<TGridVar<double> >(src.Data);
            if (! reg->Grid.VarExists("Tmp"))
                reg->Grid.AddVar("Tmp", new TGridVar<double>(*srcData));

            TGridMaskedData dst = reg->Grid.GetMaskedData(TRegionBounds(0,0), "Tmp");
            TGridVar<double> *dstData = SavableClass::TestType<TGridVar<double> >(dst.Data);

            size_t numIter = (size_t)reg->GetConstantDouble("CopyNumber");
	        Time_struct Time;
            switch ((int)reg->GetConstantDouble("CopyType")) {
                case 1 :
                    cout<<" SetFast NumberTimes "<<numIter<<" NumberElements "<<dst.Mask->NumPoints()<<"\n";
                    for(size_t i = 0; i < numIter; i++)
                        dst = src;
                        //dstData->Set(src.Mask, src);
                    break;
                case 2 :
                    cout<<" Adding NumberTimes "<<numIter<<" NumberElements "<<dst.Mask->NumPoints()<<"\n";
                    for(size_t i = 0; i < numIter; i++)
                        dst += src;
                        //dstData->Add(src.Mask, src, 1);
                    break;
                case 3 :
                    cout<<" RawSet NumberTimes "<<numIter<<" NumberElements "<<dst.Mask->NumPoints()<<"\n";
                    for(size_t i = 0; i < numIter; i++)
                        dstData->Set(srcData);
                    break;
                case 4 :
                    cout<<" SetByInternalIteration NumberTimes "<<numIter<<" NumberElements "<<dst.Mask->NumPoints()<<"\n";
                    for(size_t i = 0; i < numIter; i++)
                        dstData->SetByInternalIteration(src.Mask, src);
                    break;
                case 5 :
                    cout<<" SetByIterator NumberTimes "<<numIter<<" NumberElements "<<dst.Mask->NumPoints()<<"\n";
                    for(size_t i = 0; i < numIter; i++)
                        dstData->SetByIterator(src.Mask, src);
                    break;
                default :
                    throw info_except("CopyType have to be in 1..3 and is - %i\n", (int)reg->GetConstantDouble("CopyType"));
            }
	        cout<<"Done in "<<Time<<"\n";


        };
        virtual void RebuildBounds(TPolyRegion *reg) {
            vector<Stroka> names = reg->ShallowStart().CurRegion()->Grid.GetVarNames();
            for(size_t i = 0; i < names.size(); i++) {
                if (names[i] == "X")
                    reg->FillChildBounds(&BndX, names[i]);
                if (names[i] == "Y")
                    reg->FillChildBounds(&Bnd, names[i]);
            }
        };
		virtual void InitBeforeBounds(TPolyRegion *reg){};
		virtual void InitAfterBounds(TPolyRegion *reg){};
		virtual void SetNewTimeStp(double curTime, double timeStp){};
    };

}; //namespace NPolygon {
