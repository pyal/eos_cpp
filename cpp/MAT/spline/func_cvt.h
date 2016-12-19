
#pragma once

#include <mat/fun_fac.h>
#include <mat/quad.h>
//#include <mat\spl_fac.h>
#include <lib/ref/data_manip.h>


struct TExpConverter : IFunctionConverter {
    TExpConverter()
        : IFunctionConverter()
        , LogX(1)
        , MulX(1)
        , AddX(0) {
    }
    TExpConverter(int logX, double mulX, double addX)
        : IFunctionConverter()
        , LogX(logX)
        , MulX(mulX)
        , AddX(addX) {
    }
    ~TExpConverter();//{}

    virtual double Encode(double x, const char *errMes = NULL) {
        double arg = x * MulX + AddX;
        if (!LogX)
            return arg;
        if (arg < MathZer) {
            if (errMes)
                throw info_except("Encoding x %f with MulX %f AddX %f Log arg is %f\n", x, MulX, AddX, arg);
            else
                return 0;
        }
        return log(arg);
    }
    virtual double Decode(double x, const char *errMes = NULL) {
		if (LogX){
			if (x<-100)
				x=0;
			else
                if (x>100)
                    x=1/MathZer;
                else
				    x = exp(x);
		}
		return (x-AddX)/MulX;
    }

    virtual void GetBoundary(double &minX, double &minY, double &maxX, double &maxY) {
        IFunctionConverter::GetBoundary(minX, minY, maxX, maxY);
        if (!LogX) 
            return ;
        if (MulX > 0)
            minX = (MathZer - AddX) / MulX;
        else
            maxX = (MathZer - AddX) / MulX;
    }
	int CheckVal(double x){
        if (!LogX)
            return 1;
	    x = x * MulX + AddX;
		if (x < MathZer)
			return 0;
		return 1;
	}
   virtual int save_data_state(FilterOut&so) {
       so << "MakeLog " << LogX << " MulX "<< MulX << " AddX " << AddX;
       return 1; 
   }
   virtual int read_data_state(FilterIn&si) {
       Stroka tmp;
       si >> tmp >> LogX >> tmp >> MulX >> tmp >> AddX;
       return 1;
   }
   Stroka MakeHelp() {
       return "LogX == 1 -> y = log(MulX *x + AddX) \nelse -> y = MulX * x + AddX ";
   }
protected:
    int LogX;
    double MulX, AddX;
 };

struct TAutoExpConverter : TExpConverter {
    TAutoExpConverter()
        : TExpConverter()
        , AutoMin(1)
        , AutoVal(1e-3) {
//        , AutoSearchMin(0) , AutoSearchMax(0)
    }
   virtual int save_data_state(FilterOut&so) {
       TExpConverter::save_data_state(so);
       so << "AutoMin_yesno " << AutoMin << " AutoVal " << AutoVal;// << " AutoSearchMin " << AutoSearchMin << " AutoSearchMax " << AutoSearchMax;
       return 1; 
   }
   virtual int read_data_state(FilterIn&si) {
       TExpConverter::read_data_state(si);
       Stroka tmp;
       si >> tmp >> AutoMin >> tmp >> AutoVal;// >> tmp >> AutoSearchMin >> tmp >> AutoSearchMax;
       return 1;
   }
    virtual void SetAutoEncode(const vector<double> &xVec) {
        double val;
        VecCl vec(xVec);
        if (AutoMin==2) {
            AddX = FindBestMin(xVec);
            return;
        }
        if (AutoMin)
            val = VecCl::Min(vec);
        else
            val = VecCl::Max(VecCl(xVec));
        AddX = AutoVal - val * MulX;
    }
    double FindBestMin(const vector<double> &xVec) {
        MinimXvector = xVec;
//cout<<" given " << DataManip::SaveTextVector(xVec)<<"\n";
        VecCl vec(xVec);
        double min = VecCl::Min(vec), max = VecCl::Max(vec);
        double fmin =  - min, fmax = fmin + (max - min) * AutoVal;
        if (min > 0 ) 
            fmin = - min * (1 - M_Eps);
        else
            fmin = - min * (1 + M_Eps);
        double guess = fmax, resX, resY;
        TAutoExpConverter *ptr = CurPtr;
        CurPtr = this;
//cout<< " min " << min << " max " << max << " from " << fmin << " to " << fmax << " guess " << guess << "\n";
        resX = Fmin(Quality, fmin, fmax, guess, StndErr, resY);
//cout << " found X " << resX << " Y " << resY << "\n";
        CurPtr = ptr;
        return resX;
    }
    static TAutoExpConverter *CurPtr;
    static double Quality(double x) {
        CurPtr->AddX = x;
        vector<double> xEnc;
        //((IFunctionConverter*)CurPtr)->Encode(CurPtr->MinimXvector, xEnc, ~(Stroka("Bad !!!") + Stroka(x) + "\nGiven vector \n" + DataManip::SaveTextVector(CurPtr->MinimXvector)));
        ((IFunctionConverter*)CurPtr)->Encode(CurPtr->MinimXvector, xEnc, NULL);
        double sum = 0, step = CurPtr->MinimXvector[CurPtr->MinimXvector.size() -1] - CurPtr->MinimXvector[0] / (CurPtr->MinimXvector.size() - 1);
        step = fabs(step);
        for(size_t i = 0; i < xEnc.size() - 1; i++)
            sum += sqr(log(max<double>(fabs(xEnc[i+1] - xEnc[i]), MathZer) / step));
//cout << " x " << x << " res " << sum << "\n";
        return sum;
    }

private:
    int AutoMin;
    double AutoVal;
//            AutoSearchMin, ,  AutoSearchMax
    vector<double> MinimXvector;
};