
#pragma once

//#include "urs\matt_fac.h"
//#include "mat\mtrung.h"
#include "urs\test\urs_curve.h"
#include "lib\ref\file_manip.h"


//==============================================================================================
//============================    UrsCurve_BrightTempClc     ===================================
//==============================================================================================

struct UrsCurve_BrightTempClc:URS_Curve::ClcVar{
    UrsCurve_BrightTempClc();//:OpacityDifCalib2Measure(1), WaveLength(500), CalibTemp(2000), CalibSignal(0.5), 
        //IntegrateSpectr(0), DetectorEmissivityName("SensName"), CalibEmissivityName("WolfEmissivity"), VarSignalName("NameSignalVar"){};
    static double PlancEmisE(double Temp, double Lambda);
    //    double tl = Temp*Lambda*1e-9;
    //    return 1.19104/pow(Lambda, 5)/(exp(0.0143878/tl)-1);//*1e-34 Joule/Second/nMeter^3
    //}

	virtual void ClcStart(URS_Curve* Data){
		URS_Curve::ClcVar::ClcStart(Data);
        InitSens();
    };
    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
        CheckStp(SubName, this);
        map<Stroka, double>::iterator it = Results.find(SubName);
        if (it==Results.end())
            throw info_except("Variable <%s> for some reason was not calculated\n", SubName.c_str());
        return new SavableDouble(it->second);
    }


//    virtual Ref<SavableClass> GetRes(){return new SavableDouble(Res);}
    virtual int save_data_state( FilterOut&so) { 
        so<<" VarSignalName "<<VarSignalName<<" OpacityDifCalib2Measure "<<OpacityDifCalib2Measure<<
            " WaveLength "<<WaveLength<<" InterFilterFormName "<<InterFilterFormName <<
            " DetectorEmissivityName "<<DetectorEmissivityName<<
            " CalibTemp "<<CalibTemp<<" CalibSignal "<<CalibSignal<<" CalibEmissivityName "<<CalibEmissivityName<<
            " UseFixedCalibEmissivity "<<UseFixedCalibEmissivity<<
            " ExtraFiberNorm "<<ExtraFiberNorm<<" ExtraSignalNorm "<<ExtraSignalNorm;
        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
        si>>tmp>>VarSignalName>>tmp>>OpacityDifCalib2Measure>>tmp>>WaveLength>>tmp>>InterFilterFormName>>tmp>>DetectorEmissivityName
            >>tmp>>CalibTemp>>tmp>>CalibSignal>>tmp>>CalibEmissivityName>>tmp>>UseFixedCalibEmissivity>>tmp>>ExtraFiberNorm>>tmp>>ExtraSignalNorm;
        return 1; 
    };
	virtual Stroka MakeHelp(){
        Stroka res = "Class for calculation brightness temperautre using calibration data.\nKnown SubNames are: Temperature, DetectorSensitivity, Plank, IntegTemp\n";
        res += "Physical model: calibration <CalibSignal> done: using the light with temperature <CalibTemp> \n        with fixed emissivity <UseFixedEmissivity> or emissivity is taken from <CalibEmissivityName> 2D spline \n           (line,temp -> emissivity) calculated from 2D data by urs_curve <UrsCurve_SplConstr> method\n";
        res += "Measurements done for the line <WaveLength> or \n";
        res += "variable <IntegTemp> - will calculate integral over detector sensibility (during calibration and measurement)\n";
        res += "During integration will use <DetectorEmissivityName> 2 column file with [line emissivity] format\n";
        res += "Experimental signal is multiplied by <OpacityDifCalib2Measure> variable\n";
        res += "<InterFilterName> - currently is not working ( models non zero length filtering of the signal... )\n";

        return res;
	}
protected:
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (SubName=="Temperature")
            Results[SubName] = FindExpT2GivenLengthAdd(fabs(GetVar(VarSignalName)));
        if (SubName=="DetectorSensitivity")
            Results[SubName] = DetectorEmissivitySpl.Evaluate(fabs(GetVar(VarSignalName)));
        if (SubName=="Plank")
            Results[SubName] = PlancEmisE(CalibTemp, fabs(GetVar(VarSignalName)));
        if (SubName=="IntegTemp")
            Results[SubName] = FindExpT2DetectorIntegral(fabs(GetVar(VarSignalName)));
	}
    double OpacityDifCalib2Measure, WaveLength, CalibTemp, CalibSignal, ExtraSignalNorm, ExtraFiberNorm; //nm<<"DetectorEmissivityName"<<DetectorEmissivityName<<
    double UseFixedCalibEmissivity;
    Stroka DetectorEmissivityName, VarSignalName, CalibEmissivityName, InterFilterFormName ;
    CurveSpline DetectorEmissivitySpl, CalibEmissSpl, InterFilterSpl;

    double CalibConst, IntegTemp, IntegMinL, IntegMaxL, IntegConst, OriginalAddCalibConst;
    map<Stroka, double> Results;
    int DetectorEmissivitySplPresent;
    int ReadSpline(const char *file, CurveSpline &spl, const char *mes){
        vector<vector<double > > vec;
        File::ReadFile(file, vec, mes);
        int nR = 0;
	    if ( mes!=NULL && (vec.size()!=2 || (nR = (int)vec[0].size())<3) )
            throw info_except("Got file <%s> with NumCol %i NumRow %i\nErrorMes:<%s>", file, vec.size(), nR, mes);
        if ( vec.size()==2 && (nR = (int)vec[0].size())>=3 ) {
            int NumInt = 100;
            double misf = 1e-2;
            int Gen = spl.Generate(NumInt, misf, 0, &vec[0][0], &vec[1][0], (int)vec[0].size());
            if (!Gen && mes)
                throw info_except("File <%s> Could not generate spline\nErrorMes:<%s>", file, mes);
            if (Gen) 
                return 1;
        }
        return 0;
    }
    int ReadSplineXY(const char *file, CurveSpline &spl, double temp, const char *mes){
		FilterTextIn in(file);
		char tmp[256];in>>tmp;
		while (stricmp(tmp,"GeneratedSpline")!=0 && in)
			in>>tmp;
		if (mes && !in )
            throw info_except("In file %s could not read spline\nMes:%s\n",file, mes);
        if (!in)
            return 0;
		in.SetRefMask(SavableClass::SimpleEdit);
        Ref<SplineXYIO> splXY;
        in>>splXY;
		if (!in || !splXY)
			throw info_except("Could not read spline???\n");
        double xu, xl, yu, yl;
        splXY->GetBoundary(xu, xl, yu, yl);
        VecCl length = VecCl::MakeStepVector(200, xl, xu, 0);
        VecCl tem   = VecCl::MakeStepVector(200, temp, temp, 0);
        VecCl opac = temp;
        int Gen = splXY->Evaluate(&length[1], &tem[1], &opac[1], length.Dim());
        if (Gen!=0 && mes)
            throw info_except("Could not calculate opacity for temperature %f\n", temp);
        if (Gen!=0)
            return 0;

        int NumInt = 200;
        double misf = 1e-2;
        Gen = spl.Generate(NumInt, misf, 0, &length[1], &opac[1], length.Dim());
        if (!Gen && mes)
            throw info_except("File <%s> Could not generate spline\nErrorMes:<%s>", file, mes);
        if (Gen) 
            return 1;
        return 0;
    }
    void InitSens(){
        DetectorEmissivitySplPresent = 0;
        if (ReadSpline(DetectorEmissivityName.c_str(), DetectorEmissivitySpl, NULL)){
            IntegMinL = DetectorEmissivitySpl.Xlow;
            IntegMaxL = DetectorEmissivitySpl.Xup;
            GuessMisf = 10000;
            DetectorEmissivitySplPresent = 1;
        }
        CalibConst = 1;
        if (UseFixedCalibEmissivity<0) {
            ReadSplineXY(CalibEmissivityName.c_str(), CalibEmissSpl, CalibTemp, "Init Calib emissivity");
		    IntegConst = IntegrateInt(CalibTemp, &CalibEmissSpl, NULL);
            CalibConst /= CalibEmissSpl.Evaluate(WaveLength);
        } else {
		    IntegConst = IntegrateInt(CalibTemp, NULL, NULL)/UseFixedCalibEmissivity;
            CalibConst /= UseFixedCalibEmissivity;
        }
        if (WaveLength>0) {
            CalibConst *= exp(0.01438775220950165/(WaveLength*1e-9*CalibTemp))-1;
        } else {
            ReadSpline(InterFilterFormName.c_str(), InterFilterSpl, "Init InterFilterSpl");
            if (UseFixedCalibEmissivity<0)
                CalibConst = IntegrateInt(CalibTemp, &CalibEmissSpl, &InterFilterSpl);
            else 
                CalibConst *= IntegrateInt(CalibTemp, NULL, &InterFilterSpl);
            throw info_except("Currently not working with interfilter\n");
        }



        double Fiber = ExtraFiberNorm * PlancEmisE(CalibTemp, WaveLength);
        double ExtraSignal = ExtraSignalNorm;
        if (UseFixedCalibEmissivity<0){
            Fiber *= CalibEmissSpl.Evaluate(WaveLength);
            ExtraSignal *= IntegrateInt(CalibTemp, &CalibEmissSpl, NULL);
        } else {
            Fiber *= UseFixedCalibEmissivity;
            ExtraSignal *= IntegrateInt(CalibTemp, NULL, NULL)*UseFixedCalibEmissivity;
        }
        OriginalAddCalibConst = CalibSignal/ ( Fiber + ExtraSignal );
    }

    static UrsCurve_BrightTempClc *CurPtr;
    static double IntenValStatic(double lambda){
//cout<<" int "<<CurPtr->IntenVal(lambda)<<" lam "<<lambda<<"\n";
        return CurPtr->IntenVal(lambda);
    }
    CurveSpline *SourceEmissivityPtr, *InterFilterPtr;
    double IntenVal(double lambda){
        if (!DetectorEmissivitySplPresent)
            return 0;
            //throw info_except(" DetectorEmissivitySpl is not present. Cannot calculate IntenVal...\n");
        double intence = DetectorEmissivitySpl.Evaluate(lambda)*PlancEmisE(IntegTemp,lambda);
        if (SourceEmissivityPtr)
            intence *= SourceEmissivityPtr->Evaluate(lambda);
        if (InterFilterPtr)
            intence *= InterFilterPtr->Evaluate(lambda);
        return intence;
    }
    double IntegrateInt(double temp, CurveSpline *sourceemis, CurveSpline *interfilter){
        IntegTemp = temp;
        CurPtr = this;
        SourceEmissivityPtr = sourceemis;
        InterFilterPtr = interfilter;
        double ResErr;
        int Err;
        return Qags_IntSubInt(IntenValStatic, IntegMinL, IntegMaxL, 1e-6, 1e-6, 100, ResErr, Err);
    }

    static double MisfValStatic(double lambda){
        return CurPtr->MisfVal(lambda);
    }
    double MisfVal(double temp){
        return CalibSignal/IntegConst*IntegrateInt(temp, NULL, NULL)-ExpSigMisf;
    }
    double GuessMisf, ExpSigMisf;
    double FindExpT2DetectorIntegral(double signal){
        int res;
        ExpSigMisf = signal*OpacityDifCalib2Measure;
        CurPtr = this;
        if ( (res = Fzero(MisfValStatic, 100, 1e6, GuessMisf, 1e-6, 1e-6, 1000))>3)
            cout<<" Bad FindExpT2DetectorIntegral Fzero return "<< res << " found val "<<GuessMisf<<" found mis "<<MisfValStatic(GuessMisf)<<"\n";
        return GuessMisf;
    }
    double FindExpT2GivenLength(double signal){
        if (WaveLength<00)
            throw info_except("Currently is not working with interfilter\n");
        return  0.01438775220950165/(WaveLength*1e-9*log(1+CalibSignal*CalibConst/(signal*OpacityDifCalib2Measure)));
    }
    static double MisfSigAddStatic(double temp){
        return CurPtr->MisfSigAdd(temp);
    }
    double MisfSigAdd(double temp){
        return OriginalSignal - OriginalAddCalibConst * ( PlancEmisE(temp, WaveLength)*ExtraFiberNorm + ExtraSignalNorm*IntegrateInt(temp, NULL, NULL));
    }
    double OriginalSignal;
    double FindExpT2GivenLengthAdd(double signal){
        if (ExtraSignalNorm==0)
            return FindExpT2GivenLength(signal);
        CurPtr = this;
        
        OriginalSignal = signal*OpacityDifCalib2Measure;
        int res;
        if ( (res = Fzero(MisfSigAddStatic, 100, 1e6, GuessMisf, 1e-6, 1e-6, 1000))>3)
            cout<<" Bad FindExpT2GivenLengthAdd Fzero return "<< res << " found val "<<GuessMisf<<" found mis "<<MisfSigAdd(GuessMisf)<<"\n";
        return GuessMisf;
    }

    double FindExpT2SourceSpectr(double signal){
        return 0;
    }
};



//
////==============================================================================================
////============================    UrsCurve_BrightTempClc     ===================================
////==============================================================================================
//
//struct UrsCurve_BrightnesTemperature:URS_Curve::ClcVar{
//    UrsCurve_BrightnesTemperature();//:OpacityDifCalib2Measure(1), WaveLength(500), CalibTemp(2000), CalibSignal(0.5), 
//        //IntegrateSpectr(0), DetectorEmissivityName("SensName"), CalibEmissivityName("WolfEmissivity"), VarSignalName("NameSignalVar"){};
//
//	//virtual void ClcStart(URS_Curve* Data){
//	//	URS_Curve::ClcVar::ClcStart(Data);
// //       //InitSens();
// //   };
//    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
//        CheckStp(SubName, this);
//        map<Stroka, double>::iterator it = Results.find(SubName);
//        if (it==Results.end())
//            throw info_except("Variable <%s> for some reason was not calculated\n", SubName.c_str());
//        return new SavableDouble(it->second);
//    }
//
//    Ref<BrigtnesTemp> calculator;
////    virtual Ref<SavableClass> GetRes(){return new SavableDouble(Res);}
//    virtual int save_data_state( FilterOut&so) { 
//        so<<" VarSignalName "<<VarSignalName<<" OpacityDifCalib2Measure "<<OpacityDifCalib2Measure<<
//            " WaveLength "<<WaveLength<<" InterFilterFormName "<<InterFilterFormName <<
//            " DetectorEmissivityName "<<DetectorEmissivityName<<
//            " CalibTemp "<<CalibTemp<<" CalibSignal "<<CalibSignal<<" CalibEmissivityName "<<CalibEmissivityName<<
//            " UseFixedCalibEmissivity "<<UseFixedCalibEmissivity<<
//            " ExtraFiberNorm "<<ExtraFiberNorm<<" ExtraSignalNorm "<<ExtraSignalNorm;
//        return 1; 
//    };
//	virtual int read_data_state(FilterIn&si){ 
//        char tmp[256];
//        si>>tmp>>VarSignalName>>tmp>>OpacityDifCalib2Measure>>tmp>>WaveLength>>tmp>>InterFilterFormName>>tmp>>DetectorEmissivityName
//            >>tmp>>CalibTemp>>tmp>>CalibSignal>>tmp>>CalibEmissivityName>>tmp>>UseFixedCalibEmissivity>>tmp>>ExtraFiberNorm>>tmp>>ExtraSignalNorm;
//        return 1; 
//    };
//	virtual Stroka MakeHelp(){
//        Stroka res = "Class for calculation brightness temperautre using calibration data.\nKnown SubNames are: Temperature, DetectorSensitivity, Plank, IntegTemp\n";
//        res += "Physical model: calibration <CalibSignal> done: using the light with temperature <CalibTemp> \n        with fixed emissivity <UseFixedEmissivity> or emissivity is taken from <CalibEmissivityName> 2D spline \n           (line,temp -> emissivity) calculated from 2D data by urs_curve <UrsCurve_SplConstr> method\n";
//        res += "Measurements done for the line <WaveLength> or \n";
//        res += "variable <IntegTemp> - will calculate integral over detector sensibility (during calibration and measurement)\n";
//        res += "During integration will use <DetectorEmissivityName> 2 column file with [line emissivity] format\n";
//        res += "Experimental signal is multiplied by <OpacityDifCalib2Measure> variable\n";
//        res += "<InterFilterName> - currently is not working ( models non zero length filtering of the signal... )\n";
//
//        return res;
//	}
//protected:
//	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
//        if (SubName=="Temperature")
//            Results[SubName] = FindExpT2GivenLengthAdd(fabs(GetVar(VarSignalName)));
//        if (SubName=="DetectorSensitivity")
//            Results[SubName] = DetectorEmissivitySpl.Evaluate(fabs(GetVar(VarSignalName)));
//        if (SubName=="Plank")
//            Results[SubName] = PlancEmisE(CalibTemp, fabs(GetVar(VarSignalName)));
//        if (SubName=="IntegTemp")
//            Results[SubName] = FindExpT2DetectorIntegral(fabs(GetVar(VarSignalName)));
//	}
//    double OpacityDifCalib2Measure, WaveLength, CalibTemp, CalibSignal, ExtraSignalNorm, ExtraFiberNorm; //nm<<"DetectorEmissivityName"<<DetectorEmissivityName<<
//    double UseFixedCalibEmissivity;
//    Stroka DetectorEmissivityName, VarSignalName, CalibEmissivityName, InterFilterFormName ;
//    CurveSpline DetectorEmissivitySpl, CalibEmissSpl, InterFilterSpl;
//
//};