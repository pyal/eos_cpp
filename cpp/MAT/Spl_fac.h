#ifndef __SPL_FAC_H
#define __SPL_FAC_H

#include "lib\std\deffunc.h"
//#include "lib\std\interfac.h"
#include "mat\dat_fac.h"
#include "mat\dat_reg.h"
#include "lib\std\f2c.h"
#include "lib\ref\class_sav.h"
#include "lib\data\DataVec.h"
#include "lib\ref\file_manip.h"

#include "mat\matrics.h"
#include "mat\quad.h"

struct ISpline:SavableClass{
    ISpline():SavableClass(){}
    virtual map<Stroka, double> GetBoundary() = 0;
    virtual void SetBoundary(const map<Stroka, double> &bnd) = 0;
    virtual void SetBoundary(TData<real> &dat) = 0;

    virtual int  Generate(map<Stroka, int> Num, double &Misf, int Cont, TData<real> &dat)=0;
    virtual int  Generate(int Cont, TData<real> &dat)=0;
    virtual int  Evaluate (TData<real> &dat)=0;

    virtual int  Generate(int Cont, vector<vector<double> > &vec){
        TData<real> dat;
        dat.Delete();
        for(size_t i = 0; i < vec.size(); i++)
            dat.AddColumn(&vec[i][0], (int)vec[i].size());
        return Generate(Cont, dat);
    }
    virtual int  Evaluate (vector<vector<double> > &vec)=0;

    virtual int  Generate(int Cont, map<Stroka, vector<double> > &vec) = 0;
    virtual int  Evaluate (map<Stroka, vector<double> > &vec)=0;
    virtual Stroka ResultName() = 0;

  	virtual int read_generation_par(FilterIn &in){return 1;}
  	virtual int save_generation_par(FilterOut &out){return 1;}

};

struct SplineXYIO:ISpline {
    virtual map<Stroka, double> GetBoundary(){
        map<Stroka, double> ret;
        ret["Xmin"] = Xlow;
        ret["Xmax"] = Xup;
        ret["Ymin"] = Ylow;
        ret["Ymax"] = Yup;
        return ret;
    }
    virtual double GetMisfit() {
        return Delta;
    }
    virtual void SetBoundary(const map<Stroka, double> &bnd){
        for(map<Stroka, double>::const_iterator it = bnd.begin(); it != bnd.end(); it++){
            if (it->first == "Xmin")
                Xlow = it->second;
            if (it->first == "Xmax")
                Xup = it->second;
            if (it->first == "Ymax")
                Ylow = it->second;
            if (it->first == "Ymax")
                Yup = it->second;
        }
    }
    virtual void SetBoundary(TData<real> &dat){
        SetLimits(dat);
    }

    virtual int  Generate(map<Stroka, int> Num, double &Misf, int Cont, TData<real> &dat){
        int NumX = Num["X"];
        int NumY = Num["Y"];
        int ret = Generate(NumX, NumY, Misf, Cont, dat);
        Num["X"] = NumX;
        Num["Y"] = NumY;
        return ret;
    }
    virtual int  Evaluate (vector<vector<double> > &vec){
        return Evaluate (&vec[0][0], &vec[1][0], &vec[2][0], (int)vec[0].size());
    }

    virtual int  Generate(int Cont, map<Stroka, vector<double> > &vec){
        //TData<real> dat;
        TColumn<real> dat;
        dat.Delete();
        if (vec.find("X") == vec.end() || vec.find("Y") == vec.end() || vec.find("Z") == vec.end())
            throw info_except("Have to define X Y Z variables to generate 3D spline\n");
        dat.AddColumn(&vec["X"][0], (int)vec["X"].size());
        dat.AddColumn(&vec["Y"][0], (int)vec["Y"].size());
        dat.AddColumn(&vec["Z"][0], (int)vec["Z"].size());
        return Generate(Cont, dat);
        
    }
    virtual int  Evaluate (map<Stroka, vector<double> > &vec){
        if (vec.find("X") == vec.end() || vec.find("Y") == vec.end() || vec.find("Z") == vec.end())
            throw info_except("Have to define X Y Z variables to evaluate 3D spline\n");
        return Evaluate (&vec["X"][0], &vec["Y"][0], &vec["Z"][0], (int)vec["X"].size());
    }
    virtual Stroka ResultName(){return "Z";};


// End of ISpline interface



   TDataF<real> BSpl;
   virtual void GetBoundary(double &Xl, double &Xu, double &Yl, double &Yu){
       Xl = Xlow; Xu = Xup; Yu = Yup; Yl = Ylow;
   }
   SplineXYIO(){};
   virtual void SetLimits(double xu,double xl,double yu,double yl) {Xup=xu;Xlow=xl;Yup=yu;Ylow=yl;};
   virtual void SetLimits(TData<real> &dat) 
     {if (stricmp(dat.InterfaceIOName(),"TTable")!=0) {cout<<"SetLimits wrong data.\n";exit(0);}
      SetLimits(dat.D[0][dat.I[0]-1],dat.D[0][0],dat.D[1][dat.I[1]-1],dat.D[1][0]);};
   virtual int  Generate(int &NumX,int &NumY,double &Misf,int Cont,TData<real> &dat)=0;
   virtual int  Generate(int Cont,TData<real> &dat)=0;
   virtual int  Evaluate (double *x,double *y,double *z,int nx)=0;
   virtual int  Evaluate (TData<real> &dat)=0;

   int save_data_state(FilterOut& out){
      out.precision(12);
      out<<"\nMisfit "<<Delta<<" Xup "<<Xup<<" Xlow "<<Xlow<<" Yup "<<Yup<<"  Ylow "<<Ylow<<"\n";
      out<<" BSpline_XY_Coef \n";
      BSpl.SaveIni(out);
	  return 1;
      };
   int read_data_state(FilterIn &in){
		char tmp[256];
		in>>tmp>>Delta>>tmp>>Xup>>tmp>>Xlow>>tmp>>Yup>>tmp>>Ylow>>tmp;
		BSpl.ReadIni(in);
		if (!in)
			throw info_except("Could not read spline.\n");

		return 1;
   };

  	virtual int read_generation_par(FilterIn &in){return 1;}
  	virtual int save_generation_par(FilterOut &out){return 1;}

   virtual ~SplineXYIO(){};
  protected:
   real Xup,Xlow,Yup,Ylow,Delta;

  };

struct SplineXIO:ISpline {
    virtual map<Stroka, double> GetBoundary(){
        map<Stroka, double> ret;
        ret["Xmin"] = Xlow;
        ret["Xmax"] = Xup;
        return ret;
    }
    virtual void SetBoundary(const map<Stroka, double> &bnd){
        for(map<Stroka, double>::const_iterator it = bnd.begin(); it != bnd.end(); it++){
            if (it->first == "Xmin")
                Xlow = it->second;
            if (it->first == "Xmax")
                Xup = it->second;
        }
    }
    virtual void SetBoundary(TData<real> &dat){
        SetLimits(dat);
    }

    virtual int  Generate(map<Stroka, int> Num, double &Misf, int Cont, TData<real> &dat){
        int NumX = Num["X"];
        int ret = Generate(NumX, Misf, Cont, dat);
        Num["X"] = NumX;
        return ret;
    }
    virtual int  Evaluate (vector<vector<double> > &vec){
        return Evaluate (&vec[0][0], &vec[1][0], (int)vec[0].size());
    }

    virtual int  Generate(int Cont, map<Stroka, vector<double> > &vec){
        TData<real> dat;
        dat.Delete();
        if (vec.find("X") == vec.end() || vec.find("Y") == vec.end())
            throw info_except("Have to define X Y (and can define W ) variables to generate 2D spline\n");
        dat.AddColumn(&vec["X"][0], (int)vec["X"].size());
        dat.AddColumn(&vec["Y"][0], (int)vec["Y"].size());
        if (vec.find("W") != vec.end())
            dat.AddColumn(&vec["W"][0], (int)vec["W"].size());
        return Generate(Cont, dat);
        
    }
    virtual int  Evaluate (map<Stroka, vector<double> > &vec){
        if (vec.find("X") == vec.end() || vec.find("Y") == vec.end())
            throw info_except("Have to define X Y (and can define W ) variables to evaluate 2D spline\n");
        return Evaluate (&vec["X"][0], &vec["Y"][0], (int)vec["X"].size());
    }
    virtual Stroka ResultName(){return "Y";};

// End of ISpline interface


   real Xup,Xlow,Delta;
   TDataF<real> BSpl;

   int GenNumX;
   double GenMisf;

   SplineXIO():ISpline(), Xup(0), Xlow(0), GenNumX(0), GenMisf(0){};
   virtual void SetLimits(double xu,double xl) {Xup=xu;Xlow=xl;};
   virtual void SetLimits(TData<real> &dat) 
     {if ( (stricmp(dat.InterfaceIOName(),"TDataF")!=0) || 
           ( (dat.N!=2) && (dat.N!=3) ) || (dat.I[0]!=dat.I[1]) )
     {cout<<"SetLimits SplineXIO wrong data.\n";exit(0);}
      SetLimits(dat.D[0][dat.I[0]-1],dat.D[0][0]);};
   virtual int  Generate(int &NumX,double &Misf,int Cont,TData<real> &dat,
                         int Xcol=-1,int Ycol=-1,int Wcol=-1)=0;
   virtual int  Generate(int Cont, TData<real> &dat){
       return Generate(Cont, dat, -1);
   }
   virtual int  Generate(int Cont, TData<real> &dat,
       int Xcol, int Ycol=-1, int Wcol=-1){
       int nX(GenNumX);
       double mis(GenMisf);
       return Generate(nX, mis, Cont, dat, Xcol, Ycol, Wcol);
   }
   virtual int  Evaluate (double *x,double *y,int nx)=0;
   virtual int  Evaluate (TData<real> &dat){
       return Evaluate(dat, -1);
   };
   virtual int  Evaluate (TData<real> &dat,int Xcol, int Ycol=-1)=0;

   int save_data_state(FilterOut &out){
	  out<<"\nMisfit "<<Delta<<" Xup "<<Xup<<" Xlow "<<Xlow<<"\n";
      out<<" BSpline_X_Coef \n";
      BSpl.SaveIni(out);
	  return 1;
   };
   int read_data_state(FilterIn &in){
	  char tmp[256];
      in>>tmp>>Delta>>tmp>>Xup>>tmp>>Xlow>>tmp;
      BSpl.ReadIni(in);
      return 1;
   };

   //static VecCl LimitBounds(double *x,int nx, double Min, double Max){
   //     VecCl res(nx);
   //     for(int k=1;k<=nx;k++){
   //         double t = x[k-1];
   //         res[k]=t>Min?t:Min;
   //         t=res[k];
   //         res[k]=t<Max?t:Max;
   //     }
   //     return res;
   // }

  	virtual int read_generation_par(FilterIn &in){
        char tmp[256];
        in>>tmp>>GenNumX>>tmp>>GenMisf;
        return 1;
    }
  	virtual int save_generation_par(FilterOut &out){
        out<<" GenNumX "<<GenNumX<<" GenMisf "<<GenMisf;
        return 1;
    }

   virtual ~SplineXIO(){};



   static double Integrate(double xFrom, double xTo, double misf, SplineXIO *spl) {
       double ResErr;
       int Err;
       SplineXIO *oldSpl = StaticSpl;
       StaticSpl = spl;
       double ret = Qags_IntSubInt(IntegSplClcFunc, xFrom, xTo, misf, misf, 10000, ResErr, Err);
       StaticSpl = oldSpl;
       return  ret;
   }
   static SplineXIO *StaticSpl;
   static double IntegSplClcFunc(double x) {
       double y;
       StaticSpl->Evaluate(&x, &y, 1);
       return y;
   }

  };

struct RegridSpline:SplineXYIO{
	RegridSpline():GenNumX(205), GenNumY(205), GenMisfit(1e-6){};
   int  Generate(int &NumX,int &NumY,double &Misf,int Cont,TData<real> &dat);
   int  Generate(int Cont,TData<real> &dat){
		int x = GenNumX, y = GenNumY;
		double m = GenMisfit;
		return Generate(x, y, m, Cont, dat);
   }
   virtual int  Evaluate (double *x,double *y,double *z,int nx);
   virtual int  Evaluate (TData<real> &dat);

	int read_generation_par(FilterIn &in){
	    char tmp[256];
		in>>tmp>>GenMisfit>>tmp>>GenNumX>>tmp>>GenNumY;
		return !(!in);
	}
	int save_generation_par(FilterOut &out){
		out<<"GenerationMisfit"<<GenMisfit<<"GenerationNumX"<<GenNumX<<"GenerationNumY"<<GenNumY<<SavableClass::EOLN();//<<"\n";
		return !(!out);
	}

  protected:
	int GenNumX, GenNumY;
	double GenMisfit;

  };
struct ExtendedRegridSpline:RegridSpline {
	ExtendedRegridSpline():RegridSpline(), LogX(1), LogY(1), LogZ(1),
		AddBeforeLogX(0), AddBeforeLogY(0), AddBeforeLogZ(0), MulX(1), MulY(1), MulZ(1){};
    //double ResMisfit;

    virtual void GetBoundary(double &Xl, double &Xu, double &Yl, double &Yu){
        Xl = DecodeVal(Xlow,LogX,AddBeforeLogX,MulX); 
        Xu = DecodeVal(Xup,LogX,AddBeforeLogX,MulX); 
        Yl = DecodeVal(Ylow,LogY,AddBeforeLogY,MulY); 
        Yu = DecodeVal(Yup,LogY,AddBeforeLogY,MulY);
    }

    void SetGrid(int numX, double minX, double maxX, int numY, double minY, double maxY, TTable<real> &dat) {
        double eMinX = EncodeVal(minX, LogX, AddBeforeLogX, MulX);
        double eMaxX = EncodeVal(maxX, LogX, AddBeforeLogX, MulX);
        double eMinY = EncodeVal(minY, LogY, AddBeforeLogY, MulY);
        double eMaxY = EncodeVal(maxY, LogY, AddBeforeLogY, MulY);
        VecCl eX = VecCl::MakeStepVector(numX, eMinX, eMaxX, 0);
        VecCl eY = VecCl::MakeStepVector(numY, eMinY, eMaxY, 0);
        VecCl eZ(numX * numY); eZ = eZ * 0 + 1.;
        //dat.SetDim(0);
        dat.SetColumn(&eX[1], numX, 0);
        dat.SetColumn(&eY[1], numY, 1);
        dat.SetColumn(&eZ[1], numX * numY, 2);
        //TTable<real> da;da = dat
        EncodeData(dat, 1);

    }

	int  Generate(int &NumX,int &NumY,double &Misf,int Cont,TData<real> &dat){
		TColumn<real> da;
        da = dat;
		EncodeData(da);
		int ret = RegridSpline::Generate(NumX,NumY,Misf,Cont,da);
        SetBoundary();
        return ret;
	}
	int  Generate(int Cont,TData<real> &dat, const char *mes){
		int x = GenNumX, y = GenNumY;
		double m = GenMisfit;
        int ret;
        try {
		    ret = Generate(x, y, m, Cont, dat);
        } catch(stdexception &ex) {
            if (mes) {
                throw info_except("Caught exception:\n%s\n%s", ex.what(), mes);
            }
        }
        if (abs(ret) > 3 && mes)
            throw info_except("Bad spline obtained: error %i\n%s\n", ret, mes);
        return ret;
	}
    double SetInLimit(double x, double low, double up) {
        if (x < low)
            return low;
        if (x > up)
            return up;
        return x;
    }
	virtual int  Evaluate (double *x,double *y,double *z,int nx){
		DataVector<double, CopyStructFast<double> > X(nx),Y(nx),Z(nx);
		int i;
		for (i=0;i<nx;i++){
            X[i] = EncodeVal(SetInLimit(x[i], BndXlow, BndXup), LogX, AddBeforeLogX, MulX);
			Y[i] = EncodeVal(SetInLimit(y[i], BndYlow, BndYup), LogY, AddBeforeLogY, MulY);
		}
		int ret = RegridSpline::Evaluate((double*)X,(double*)Y,(double*)Z,nx);
		for (i=0;i<nx;i++)
			z[i] = DecodeVal(Z[i],LogZ,AddBeforeLogZ,MulZ);
        for (i=0;i<nx;i++) {
			z[i] = DecodeVal(Z[i],LogZ,AddBeforeLogZ,MulZ);
            if (!(!In_Lim(x[i], BndXlow, BndXup, 1) || !In_Lim(y[i], BndYlow, BndYup, 1)))
                continue;
            double x0 = x[i], y0 = y[i], x1 = x[i], y1 = y[i], xStp = 0, yStp = 0;
            if (x0 < BndXlow) {
                xStp = x0 - BndXlow;
                x0 = BndXlow;
                x1 = BndXlow + Xstp;
            }
            if (x0 > BndXup) {
                xStp = x0 - BndXup;
                x0 = BndXup;
                x1 = BndXup - Xstp;
            }
            if (y0 < BndYlow) {
                yStp = y0 - BndYlow;
                y0 = BndYlow;
                y1 = BndYlow + Ystp;
            }
            if (x0 > BndXup) {
                yStp = y0 - BndYup;
                y0 = BndYup;
                y1 = BndYup - Ystp;
            }
            double z0, zx1, zy1;
            z0 = EvaluateSimple(x0, y0);
            if (xStp != 0)
                zx1 = EvaluateSimple(x1, y0);
            if (yStp != 0)
                zy1 = EvaluateSimple(x0, y1);
            double res = z0;
            if (xStp != 0)
                res += (zx1 - z0) / (x1 - x0) * xStp;
            if (yStp != 0)
                res += (zy1 - z0) / (y1 - y0) * yStp;
            z[i] = res;
        }
		return ret;
	}
    double EvaluateSimple(double x, double y) {
        double ex = EncodeVal(x, LogX, AddBeforeLogX, MulX);
        double ey = EncodeVal(y, LogY, AddBeforeLogY, MulY);
        double ez;
        RegridSpline::Evaluate(&ex, &ey, &ez, 1);
        return DecodeVal(ez, LogZ, AddBeforeLogZ, MulZ);
    }
	virtual int  Evaluate (TData<real> &dat){
		return Evaluate(dat.D[0],dat.D[1],dat.D[2],dat.I[0]);
	}
    int save_data_state(FilterOut &out){
		out<<"LogX"<<LogX<<"LogY"<<LogY<<"LogZ"<<LogZ<<"AddBeforeLogX"<<AddBeforeLogX
            <<"AddBeforeLogY"<<AddBeforeLogY<<"AddBeforeLogZ"<<AddBeforeLogZ<<SavableClass::EOLN();
		out<<"MulX"<<MulX<<"MulY"<<MulY<<"MulZ"<<MulZ<<SavableClass::EOLN();
		out<<"MinX"<<DecodeVal(Xlow,LogX,AddBeforeLogX,MulX)<<"MaxX"<<DecodeVal(Xup,LogX,AddBeforeLogX,MulX)
			<<"MinY"<<DecodeVal(Ylow,LogY,AddBeforeLogY,MulY)<<"MaxY"<<DecodeVal(Yup,LogY,AddBeforeLogY,MulY)<<SavableClass::EOLN();
		return SplineXYIO::save_data_state(out);
    };
	int read_data_state(FilterIn &in){
	    char tmp[256];
	    in>>tmp>>LogX>>tmp>>LogY>>tmp>>LogZ>>tmp>>AddBeforeLogX>>tmp>>AddBeforeLogY>>tmp>>AddBeforeLogZ;
	    in>>tmp>>MulX>>tmp>>MulY>>tmp>>MulZ;
	    in>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp;
		if (!in)
			throw info_except("Could not read spline...Last read word %s\n",tmp);
   	    int ret =  RegridSpline::read_data_state(in);
        SetBoundary();
        return ret;
	};
	int read_generation_par(FilterIn &in){
	    char tmp[256];
	    in>>tmp>>LogX>>tmp>>LogY>>tmp>>LogZ>>tmp>>AddBeforeLogX>>tmp>>AddBeforeLogY>>tmp>>AddBeforeLogZ;
	    in>>tmp>>MulX>>tmp>>MulY>>tmp>>MulZ;
		return RegridSpline::read_generation_par(in);
	}
	int save_generation_par(FilterOut &out){
		out<<"LogX"<<LogX<<"LogY"<<LogY<<"LogZ"<<LogZ<<"AddBeforeLogX"<<AddBeforeLogX
			<<"AddBeforeLogY"<<AddBeforeLogY<<"AddBeforeLogZ"<<AddBeforeLogZ<<SavableClass::EOLN();//<<"\n";
		out<<"MulX"<<MulX<<"MulY"<<MulY<<"MulZ"<<MulZ<<SavableClass::EOLN();
		return RegridSpline::save_generation_par(out);
	}
    void SetBoundary() {
        GetBoundary(BndXlow, BndXup, BndYlow, BndYup);
        Xstp = (BndXup - BndXlow) / 100;
        Ystp = (BndYup - BndYlow) / 100;
    }
    double BndXlow, BndXup, BndYlow, BndYup;
    double Xstp, Ystp;
private:
	int LogX, LogY, LogZ;
	double AddBeforeLogX,AddBeforeLogY,AddBeforeLogZ;
	double MulX, MulY,MulZ;

	double EncodeVal(double x,int Log,double AddBefore,double Mul){
	    x*=Mul;
		x+=AddBefore;
		if (!Log)
			return x;
		if (x<MathZer)
			throw info_except("Cannot make log of %f\n",x);
		return log(x);
	}
	double DecodeVal(double x,int Log,double AddBefore, double Mul){
		if (Log){
			if (x<-100)
				x=0;
			else
                if (x>100)
                    x=1/MathZer;
                else
				    x = exp(x);
		}
		return (x-AddBefore)/Mul;
	}
	int CheckVal(double x,int Log,double AddBefore,double Mul){
	    x*=Mul;
		x+=AddBefore;
		if (x<MathZer && Log)
			return 0;
		return 1;
	}
	void EncodeData(TData<real> &dat, int decodeIt = 0) {
		int l[] = {LogX,LogY,LogZ};
		double a[] = {AddBeforeLogX,AddBeforeLogY,AddBeforeLogZ};
		double m[] = {MulX,MulY,MulZ};
		for(int i=0;i<3;i++){
			for(int i1=0;i1<dat.I[i];i1++){
                if (!decodeIt) {
				    if ( !CheckVal(dat.D[i][i1],l[i],a[i],m[i]) ){
		                double Min,Max;
                        TData<real>::FindMinMax(dat,i,Min,Max);
					    throw info_except("Row %i min val is %f max val is %f AddVal is %f MulVal is %f.Cannot make log...\n",i,Min,Max,a[i],m[i]);
				    }
				    dat.D[i][i1] = EncodeVal(dat.D[i][i1],l[i],a[i],m[i]);
                } else 
				    dat.D[i][i1] = DecodeVal(dat.D[i][i1],l[i],a[i],m[i]);
			}
		}
	}
  };

struct CurveSpline : SplineXIO {
   int SplineOrder;
   CurveSpline(int SplOrder=3){SplineOrder=SplOrder;}
//   real *Weight;
   virtual int  Generate(int &NumX,double &Misf,int Cont,TData<real> &dat,
                         int Xcol=-1,int Ycol=-1,int Wcol=-1);
   virtual int  Generate(int &NumX,double &Misf,int Cont,double *x,double *y,int n,
                         double *weight=NULL);
   virtual int  Evaluate (TData<real> &dat,int Xcol=-1,int Ycol=-1);
   virtual int  Evaluate (double *x,double *y,int nx);
   virtual double Evaluate (double x);
   void ReadIni(istream &in){
		FilterTextIn input(in.rdbuf(),0);
		read_data_state(input);
		if (!in) throw info_except(" Could not read class %s\n ", class_name());
   }
   void SaveIni(ostream &out){
		FilterTextOut output(out.rdbuf(),0);
		save_data_state(output);
		if (!out) throw info_except(" Could not save class %s\n ", class_name());
   }
   static int SplineFromData(const char *file, CurveSpline &spl, double misf, const char *mes) {
        vector<vector<double > > vec;
        File::ReadFile(file, vec, mes);
        int nR = 0;
	    if ( mes != NULL && (vec.size()!=2 || (nR = (int)vec[0].size())<3) )
            throw info_except("Got file <%s> with NumCol %i NumRow %i\nErrorMes:<%s>", file, vec.size(), nR, mes);
        if ( vec.size() == 2 && (nR = (int)vec[0].size())>=3 ) {
            int NumInt = 100;
            //double misf = 1e-2;
            int Gen = spl.Generate(NumInt, misf, 0, &vec[0][0], &vec[1][0], (int)vec[0].size());
            if (!Gen && mes)
                throw info_except("File <%s> Could not generate spline\nErrorMes:<%s>", file, mes);
            if (Gen) 
                return 1;
        }
        return 0;
    }
	Stroka MakeHelp() {
		Stroka ret = "ier=0  : normal return. the spline returned has a residual sum of squares fp such that abs(fp-s)/s <= tol with tol a relative tolerance set to 0.001 by the program. \n";
		ret += "ier=-1 : normal return. the spline returned is an interpolating spline (fp=0). \n";
		ret += "ier=-2 : normal return. the spline returned is the weighted least squares polynomial of degree k. in this extreme case fp gives the upper bound fp0 for the smoothing factor s. \n";
		ret += "ier=1  : error. the required storage space exceeds the available storage space, as specified by the parameter nest.\n";
		ret += "ier=2  : error. a theoretically impossible result was found during  the iteration proces for finding a smoothing spline with fp = s. probably causes : s too small. \n";
		ret += "ier=3  : error. the maximal number of iterations maxit (set to 20  by the program) allowed for finding a smoothing spline  with fp=s has been reached. probably causes : s too small \n";
		ret += "ier=10 : error. on entry, the input data are controlled on validity the following restrictions must be satisfied.  -1<=iopt<=1, 1<=k<=5, m>k, nest>2*k+2, w(i)>0,i=1,2,...,m  xb<=x(1)<x(2)<...<x(m)<=xe, lwrk>=(k+1)*m+nest*(7+3*k) \n";
		ret += "\n\n";
		return ret;
	}

  };
struct CurveSplineEncode : CurveSpline {
    CurveSplineEncode(int splOrder=3)
        : CurveSpline(splOrder) 
        , LogX(0)
        , LogY(0)
        , AddBeforeLogX(0)
        , AddBeforeLogY(0)
        , MulX(1)
        , MulY(1){
    }
    void SetGrid(int numX, double minX, double maxX, TTable<real> &dat) {
        double eMinX = EncodeVal(minX, LogX, AddBeforeLogX, MulX);
        double eMaxX = EncodeVal(maxX, LogX, AddBeforeLogX, MulX);
        VecCl eX = VecCl::MakeStepVector(numX, eMinX, eMaxX, 0);
        VecCl eY = eX * 0 + 1.;
        dat.SetColumn(&eX[1], numX, 0);
        dat.SetColumn(&eY[1], numX, 1);
        EncodeData(dat, 1);
    }
    void GetGenerationPar(CurveSplineEncode &spl) {
        ostrstream out;
	    FilterTextOut output(out.rdbuf(), 0);
        spl.save_generation_par(output);
        istrstream in(out.str(), out.pcount());
        FilterTextIn input(in.rdbuf(), 0);
        read_generation_par(input);
    }
   //virtual int  Generate(int &NumX,double &Misf,int Cont,TData<real> &dat,
   //                      int Xcol=-1,int Ycol=-1,int Wcol=-1);
   virtual int  Generate(int &NumX,double &Misf,int Cont,double *x,double *y,int n,
                         double *weight=NULL);
   //virtual int  Evaluate (TData<real> &dat,int Xcol=-1,int Ycol=-1);
   virtual int  Evaluate (double *x,double *y,int nx);
   //virtual double Evaluate (double x);
  // void ReadIni(istream &in){
		//FilterTextIn input(in.rdbuf(),0);
		//read_data_state(input);
		//if (!in) throw info_except(" Could not read class %s\n ", class_name());
  // }
  // void SaveIni(ostream &out){
		//FilterTextOut output(out.rdbuf(),0);
		//save_data_state(output);
		//if (!out) throw info_except(" Could not save class %s\n ", class_name());
  // }
	int read_generation_par(FilterIn &in){
        CurveSpline::read_generation_par(in);
	    char tmp[256];
        in>>tmp>>LogX>>tmp>>AddBeforeLogX>>tmp>>MulX;
        in>>tmp>>LogY>>tmp>>AddBeforeLogY>>tmp>>MulY;
		return !(!in);
	}
	int save_generation_par(FilterOut &out){
        CurveSpline::save_generation_par(out);
		out<<"LogX"<<LogX<<"AddBeforeLogX"<<AddBeforeLogX<<"MulX"<<MulX<<SavableClass::EOLN();
		out<<"LogY"<<LogY<<"AddBeforeLogY"<<AddBeforeLogY<<"MulY"<<MulY<<SavableClass::EOLN();
		return !(!out);
	}

private:
   int LogX, LogY;
   double AddBeforeLogX, AddBeforeLogY, MulX, MulY;


	double EncodeVal(double x,int Log,double AddBefore,double Mul){
	    x*=Mul;
		x+=AddBefore;
		if (!Log)
			return x;
		if (x<MathZer)
			throw info_except("Cannot make log of %f\n",x);
		return log(x);
	}
	double DecodeVal(double x,int Log,double AddBefore, double Mul){
		if (Log){
			if (x<-100)
				x=0;
			else
                if (x>100)
                    x=1/MathZer;
                else
				    x = exp(x);
		}
		return (x-AddBefore)/Mul;
	}
	int CheckVal(double x,int Log,double AddBefore,double Mul){
	    x*=Mul;
		x+=AddBefore;
		if (x<MathZer && Log)
			return 0;
		return 1;
	}
	void EncodeData(TData<real> &dat, int decodeIt = 0) {
		int l[] = {LogX,LogY};
		double a[] = {AddBeforeLogX,AddBeforeLogY};
		double m[] = {MulX,MulY};
		for(int i=0;i<2;i++){
			for(int i1=0;i1<dat.I[i];i1++){
                if (!decodeIt) {
				    if ( !CheckVal(dat.D[i][i1],l[i],a[i],m[i]) ){
		                double Min,Max;
                        TData<real>::FindMinMax(dat,i,Min,Max);
					    throw info_except("Row %i min val is %f max val is %f AddVal is %f MulVal is %f.Cannot make log...\n",i,Min,Max,a[i],m[i]);
				    }
				    dat.D[i][i1] = EncodeVal(dat.D[i][i1],l[i],a[i],m[i]);
                } else 
				    dat.D[i][i1] = DecodeVal(dat.D[i][i1],l[i],a[i],m[i]);
			}
		}
	}
};


struct CurveSpline_Noise:CurveSpline{
    CurveSpline_Noise():CurveSpline(), Mode(2), SplineOrder(2), ModeVal(1){}
    virtual int  Generate(int &NumX,double &Misf,int Cont,TData<real> &dat,
                         int Xcol=-1,int Ycol=-1,int Wcol=-1);
    virtual int  Evaluate (double *x,double *y,int nx);
    virtual double Evaluate (double x);

  	virtual int read_generation_par(FilterIn &in){
        char tmp[256];
		in >> tmp >> Mode >> tmp >> SplineOrder >> tmp >> SplineOrder;
        return 1;
    }
  	virtual int save_generation_par(FilterOut &out){
        out << " Mode " << Mode << " SplineOrder " << SplineOrder << " ModeVal " << ModeVal;
        //out<<" Mode "<<Mode<<" SplineOrder "<<SplineOrder;
        return 1;
    }
    int Mode, SplineOrder;
    double ModeVal;

	Stroka MakeHelp() {
		Stroka ret = "|MD| = 1: Prior given value for p in VAL  (VAL.ge.ZERO). This is the fastest  use of GCVSPL, since no iteration  is performed in p.\n";
		ret += "|MD| = 2: Generalized cross validation. \n";
		ret += "|MD| = 3: True predicted mean-squared error, with prior given variance in VAL.\n";
		ret += "|MD| = 4: Prior given number of degrees of  freedom in VAL (ZERO.le.VAL.le.N-M). \n";
		ret += "Error return:\n";
		ret += "IER = 0:        Normal exit \n";
		ret += "IER = 1:        M.le.0 .or. N.lt.2*M\n";
		ret += "IER = 2:        Knot sequence is not strictly increasing, or some weight factor is not positive. \n";
		ret += "IER = 3:        Wrong mode  parameter or value. \n";
		ret += "\n\n";
		return ret;
	}
};

#endif

