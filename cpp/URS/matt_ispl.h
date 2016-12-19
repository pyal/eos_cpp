#pragma once

#include "matt_spl.h"
#include <mat/spline/spl_inc.h>
//#include <lib/ref/refmanip.h>



struct TMatterSpl : MatterIO {
private:
    Stroka SplFile, SplineDesciption;

    double DencCold,DencMax;
    Ref<ISplineCalculator> Pspl, Tspl;
    Ref<IFunctionConverter> E2FT_Converter;
    double SoundStep, E2FTmin, E2FTmax, E2FTzero, E2FTeps;

    double St_Denc,St_Temp;
    static TMatterSpl* Current;


   static double ClcSpl(ISplineCalculator *spl, double x, double y, const char *err_mes = NULL) {
        vector<double> xV(2);
        double z;
        xV[0] = x;
        xV[1] = y;
        int err;
        if (!(err = spl->Evaluate(xV, z)) && err_mes )
            throw info_except("Could not evaluate spline for x = %f y = %f res = %i\nDescription : %s", x, y, err, err_mes);
        return z;
    }
   static void ClcSplV(ISplineCalculator *spl, double* x, double* y, double *res, int num, const char *err_mes = NULL) {
        vector<vector<double> > dat(2);
        DataManip::SetVector(dat[0], x, num);
        DataManip::SetVector(dat[1], y, num);
        vector<double> resV;
        int err;
        if (!(err = spl->Evaluate(dat, resV)) && err_mes )
            throw info_except("Could not evaluate spline for x = %s\n y = %s\n res = %i\nDescription : %s", ~DataManip::SaveTextVector(dat[0]), ~DataManip::SaveTextVector(dat[1]), err, err_mes);
        DataManip::SetData(res, resV);
    }

public:

    //Stroka SplFile, SplineDesciption;

    //double DencCold,DencMax;
    //Ref<ISplineCalculator> Pspl, Tspl;
    //Ref<IFunctionConverter> E2FT_Converter;
    //double SoundStep, E2FTmin, E2FTmax, E2FTzero, E2FTeps;
    TMatterSpl();
    //    :SplFile("h2e_ros1.ispl")
    //    , SplineDesciption("sample spline")
    //    , DencCold(1e-6)
    //    , DencMax(100)
    //    , E2FT_Converter(new TExpConverter(1, 1, 150)
    //    , SoundStep(StndErr * 10)
    //    , E2FTmin(0)
    //    , E2FTmax(100)
    //    , E2FTzero(5)
    //    , E2FTeps(StndErr * 0.01) {
    //}
    double DencityCold() {return DencCold;}
    double DencityMax()  {return DencMax;}

    void Temperature(double *T,double *Dencity,double *Energy,int Num) {
        if (Tspl != NULL)
            ClcSplV(Tspl, &Energy[1], &Dencity[1], &T[1], Num, "Temperature vector spl");
    }
    void Pressure(double *P,double *Dencity,double *Energy,int Num) {
        ClcSplV(Pspl, &Energy[1], &Dencity[1], &P[1], Num, "Pressure vector spl");
    }
    double Pressure(double Dencity,double Energy) {
        return ClcSpl(Pspl, Energy, Dencity, "Pressure spl");
    }
    double Temperature(double Dencity,double Energy) { 
        if (Tspl == NULL)
            return 0;
        return ClcSpl(Tspl, Energy, Dencity, "Temperature spl");
    }
    double Sound(double Dencity,double Energy) {
        double dr = SoundStep * Dencity, r = Dencity;
        if (r<MathZer) { 
            cout<<"MatterIO::Sound; Dencity<MathZer :"<<Dencity<<"\n";
            return 0.1;
        }
        double P = Pressure(r,Energy);
        double Pp = Pressure(r+dr,Energy+P*dr/(r*r));Pp-=P;
        if (Pp<0)
            Pp=0.1;
        double S=sqrt( max<double>(0,Pp)/dr );
        return S;
    };
      
   static double MisE(double E) { 
        if (Current->Tspl == NULL) 
            return 0;
        double res = ClcSpl(Current->Tspl, Current->E2FT_Converter->Decode(E), Current->St_Denc);
        //res = log(res/Current->St_Temp);
        res = (res - Current->St_Temp);
        return res;
    }
    double Energy(double Dencity,double Temperature) {
        St_Denc = Dencity;
        St_Temp = Temperature;
        double E0 = E2FTzero;
        TMatterSpl *Old = Current;
        Current=this;
        if (!Fzero(MisE, E2FTmin, E2FTmax, E0, E2FTeps, E2FTeps, 1000)) {
            cout<<" Bad Fzero    in MatterSpl::Energy Not Zero\nfound Encode(E) " << E0 << " E0 " << E2FT_Converter->Decode(E0) << 
               " Mis " << MisE(E0) << "\n";
        }
        Current = Old;
        return E2FT_Converter->Decode(E0);

    }	                                         

    int read_data_state(FilterIn &in) {
        in >> SplFile;
        ReadSplFile();
        return 1;
    };
   int save_data_state(FilterOut &out){ 
	   out << "  " << SplFile << " ";
	   return 1;
   };
   void ReadSplFile() {
	  FilterTextIn sp(SplFile.c_str());
      if ( (!sp) || (seek_eof(sp)) )
          throw info_except(" There is no such MatterSpline file named %s\n", SplFile.c_str());
      sp.SetRefMask(SavableClass::SimpleEdit);
      vector<char> tmp_str(10256);
      char *tmp = &tmp_str[0];
      //char tmp[10256];
      sp.getline(tmp, 10256);
      SplineDesciption = tmp;
      sp >> tmp >> DencCold >> tmp >> DencMax;// >> SavableClass::EOLN();
      sp >> tmp >> SoundStep >> tmp >> E2FTmin >> tmp >> E2FTmax >> tmp >> E2FTzero >> tmp >> E2FTeps;
      sp >> tmp >> E2FT_Converter >> tmp >> Pspl >> tmp >> Tspl;
      if (!E2FT_Converter)
          throw info_except("Cannot read E2FT_Converter\n");
      if (!Pspl)
          throw info_except("Cannot read Pressure\n");
   }
   void SaveSplFile() {
	  FilterTextOut out(~SplFile);
      out.SetRefMask(SavableClass::SimpleEdit);
      if (!out)
          throw info_except(" Cannot create MatterSpline file named %s\n", ~SplFile);
      out << SplineDesciption << SavableClass::EOLN();
      out << " DencCold " << DencCold << " DencMax " << DencMax << SavableClass::EOLN();
      out << " SoundStep " << SoundStep << " E2FTmin " << E2FTmin << " E2FTmax " << E2FTmax << " E2FTzero " << E2FTzero << " E2FTeps " << E2FTeps << SavableClass::EOLN();
      out << " E2FT_Converter " << E2FT_Converter << SavableClass::EOLN();
      out << " Pspl " << Pspl << SavableClass::EOLN();
      //if (Tspl) 
      out << " Tspl " << Tspl << SavableClass::EOLN();
   }
   TMatterSpl &SetParams(const Stroka &splFile, const Stroka &splineDesciption, 
       ISplineCalculator *pspl, ISplineCalculator *tspl,
       double dencCold = 1e-6, double dencMax = 100 , IFunctionConverter *e2FT_Converter = new TExpConverter(1, 1, 150), 
       double soundStep = StndErr * 10, double e2FTmin = 0, double e2FTmax = 100, double e2FTzero = 5, double e2FTeps = StndErr * 0.01) {

        SplFile = splFile;
        Pspl = pspl;
        Tspl = tspl;
        SplineDesciption = splineDesciption;
        DencCold = dencCold;
        DencMax = dencMax;
        E2FT_Converter = e2FT_Converter;
        SoundStep = soundStep;
        E2FTmin = e2FTmin;
        E2FTmax = e2FTmax;
        E2FTzero = e2FTzero;
        E2FTeps = e2FTeps;
        return *this;
    }

    virtual Stroka MakeHelp(){
        Stroka res = MatterIO::MakeHelp();
		res += Stroka("spline file - is a binary file... Help for it is not available....\n");
        return res;
	}

};
