#pragma once

#include "fun_fac.h"
#include "specfunc.h"
#include "lib/ref/str_manip.h"
#include "lib/ref/file_manip.h"
#include "lib/ref/data_manip.h"
#include "curveanalyzer.h"

struct Cold_Vinet:NamedOneVarFunction{
    //Cold_VinetP():B0(6.36), B0prime(7.61), V0(102.128), MolVeight(10){};
    Cold_Vinet();

    virtual double Calculate(double Denc, const Stroka &What){
        if (Denc<MathZer){
            cout<<"Bad denc in Cold_Vinet "<<Denc<<"\n";
            return 0;
        }
        KnownClcName(What,1);
        if (What=="Pressure")
            return Pressure(Denc);
        return Energy(Denc);
    }
    double Pressure(double Denc){
        double V = MolVeight/Denc;
        double x = pow(V/V0,1./3);
        double niu = 1.5*(B0prime-1);
        double Pc = 3*B0/sqr(x)*(1-x)*exp(niu*(1-x));
        return Pc;
    }
    double Energy(double Denc){
        double V = MolVeight/Denc;
        double x = pow(V/V0,1./3);
        double niu = 1.5*(B0prime-1);
//        double Mul = exp(-x*niu);
        //double Ec = -9*B0*Mul*V0*(-1/Mul+exp(niu)*(1+niu*(x-1)))/sqr(niu);
        double Arg = niu*(1-x);
        double Ec = 9*B0*V0/sqr(niu)*(1-(1-Arg)*exp(Arg));
        return Ec;
    }
    virtual int save_data_state(FilterOut&so){
        so<<" B0 "<<B0<<" B0prime "<<B0prime<<" V0_cm3/mol "<<V0<<" MolVeight "<<MolVeight<<" ";
        return !(!so);
    }
    virtual int read_data_state(FilterIn&si){
        Stroka tmp;
        si>>tmp>>B0>>tmp>>B0prime>>tmp>>V0>>tmp>>MolVeight;
        return !(!si);
    }
    Stroka MakeHelp(){
        Stroka res = "Class for calculation of cold pressure and energy in the Vinet appr \nJ.Phys Cond Mat 8 (1996) p.67-81 paper\\eos_Make Hama_Suito_EOS_HighP_caloric.pdf  \n";
        res += NamedOneVarFunction::MakeHelp();
        return res;
    }
//protected:
    double B0, B0prime, V0, MolVeight;

};


struct Cold_TF_Kalitkin_P:NamedOneVarFunction{
    Cold_TF_Kalitkin_P():NamedOneVarFunction(Stroka("KalitkinP FermiP")){}
    virtual double Calculate(double Denc, const Stroka &What){
        if (Denc<MathZer){
            cout<<"Bad denc in Cold_VinetP "<<Denc<<"\n";
            return 0;
        }
        KnownClcName(What,1);
        if ( What==Stroka("KalitkinP") )
            return KalitkinP(Denc);
        return FermiP(Denc);
    }
    double KalitkinP(double Denc){
        double LgZ = log10(Z);
        double alpha = 0.1935*pow(Z, 0.495-0.039*LgZ);
        double beta = 0.068+(0.078-0.086*LgZ)*LgZ;
        double r = pow(MolVeight/(Denc*M_PartDist1Bor_is_VolIn_cm3_mol),1./3);
        double V = 4./3*M_PI*pow(r,3);
        double ro_r = Z/V*exp(-alpha*r-beta*sqr(r));
        double Pc = 1./5*pow(3*sqr(M_PI),2./3)*pow(ro_r,5./3)-13./36*pow(3/M_PI,1./3)*pow(ro_r,4./3);
        return Pc*M_Pau_GPa;
    }
    double FermiP(double Denc){
//        double LgZ = log10(Z);
        double r = pow(MolVeight/(Denc*M_PartDist1Bor_is_VolIn_cm3_mol),1./3);
        double V = 4./3*M_PI*pow(r,3);
        double ro_r = Z/V;
        double Pc = 1./5*pow(3*sqr(M_PI),2./3)*pow(ro_r,5./3);
        return Pc*M_Pau_GPa;
    }
    virtual int save_data_state(FilterOut&so){
        so<<" Z "<<Z<<" MolVeight "<<MolVeight<<" ";
        return !(!so);
    }
    virtual int read_data_state(FilterIn&si){
        Stroka tmp;
        si>>tmp>>Z>>tmp>>MolVeight;
        return !(!si);
    }
    Stroka MakeHelp(){
        Stroka res = "Class for calculation of cold pressure in Kalitkin appr \nFizika Tverdogo Tela t13, v8 (1971) p.2314-2318 paper\\eos_Make Hama_Suito_EOS_HighP_caloric.pd\nOr Fermi appr\n"+
            NamedOneVarFunction::MakeHelp();
        return res;
    }
protected:
    double Z, MolVeight;
};


#include "mat/spl_fac.h"

struct Spl2DGenerator:ManyVarFunction{
    virtual double Calculate(const map<Stroka, double> &x, const Stroka &name){
        if (GenerateSpline!=1 && name != Spl->ResultName()){
            map<Stroka, double> bnd = Spl->GetBoundary();
            return bnd[name];
        }
        map<Stroka, vector<double> > clc;
        for(map<Stroka, double>::const_iterator it = x.begin(); it != x.end(); it++)
            if (GenerateSpline!=1)
                clc[it->first].push_back(it->second);
            else
                Val[it->first].push_back(it->second);
        if (GenerateSpline!=1){
            clc[Spl->ResultName()].push_back(0);
            Spl->Evaluate(clc);
            return clc[Spl->ResultName()][0];
        }
        return -1;
   }

   Stroka SplFileName, SplineDescr, SplName;
   int GenerateSpline;
   Ref<ISpline> Spl;
   map<Stroka, vector<double> > Val;
   virtual int save_data_state(FilterOut&out){
       out<<" SplineFile "<<SplFileName<<" GenerateSpline "<<GenerateSpline<<" SplineDescr "<<SplineDescr<<" SplineClass "<<SplName;
       Spl->save_generation_par(out);
       return 1;
   };
   virtual int read_data_state(FilterIn&in) {
        char tmp[256];
        in>>tmp>>SplFileName>>tmp>>GenerateSpline>>tmp>>SplineDescr>>tmp>>SplName;
        Spl << SavableClass::name2object(SplName.c_str());
        if (!Spl)
            throw info_except(" Object %s is not of SplinXIO type\n", SplName.c_str());
        Spl->read_generation_par(in);
        if (GenerateSpline==0){
		    FilterTextIn in_(SplFileName.c_str());
		    in_>>tmp;
		    while (Stricmp(tmp,"GeneratedSpline")!=0 && in_)
			    in_>>tmp;
		    if (!in_)
			    throw info_except("Wrong spline format. Spline file: %s\n",SplFileName.c_str());
		    in_.SetRefMask(SavableClass::SimpleEdit);
		    in_>>Spl;
		    if (!in_ || !Spl)
			    throw info_except("Could not read spline???\n");
       };
        if (GenerateSpline==2){
            vector<vector<double> > dat;
            File::ReadFile(SplFileName.c_str(), dat, "Could not read data points for spline generation");
            int ret = Spl->Generate(0, dat);
            if (ret != 0)
                cout<<" Spl generation returned "<<ret<<"\n";
       };
	    if (!in)
		    throw info_except("Could not read spline???\n");

       return 1;
   }
   Spl2DGenerator():SplFileName("splFile.spl"), SplineDescr("Spline"), SplName("CurveSpline"), GenerateSpline(0), Spl(new CurveSpline){
       //ErrorDescr = Str::MakeErrDescr(Stroka("0:OK mis is in range;-1:OK interpolation;-2:OK;1:Error. The required storage space exceeds the available storage space, as specified by the parameter nest probably causes nest too small. if nest is already large (say nest > m/2), it may also indicate that s is too small;2:error. a theoretically impossible result was found during the iteration proces for finding a smoothing spline with fp = s. probably causes s too small.;3:error. the maximal number of iterations maxit (set to 20  by the program) allowed for finding a smoothing spline with fp=s has been reached. probably causes  s too small;10:error. on entry, the input data are controlled on validity the following restrictions must be satisfied.  -1<=iopt<=1, 1<=k<=5, m>k, nest>2*k+2, w(i)>0,i=1,2,...,m  xb<=x(1)<x(2)<...<x(m)<=xe, lwrk>=(k+1)*m+nest*(7+3*k)  if iopt=-1 2*k+2<=n<=min(nest,m+k+1)  xb<t(k+2)<t(k+3)<...<t(n-k-1)<xe  the schoenberg-whitney conditions, i.e. there  must be a subset of data points xx(j) such that  t(j) < xx(j) < t(j+k+1), j=1,2,...,n-k-1  if iopt>=0 s>=0  if s=0  nest >= m+k+1  if one of these conditions is found to be violated,control  is immediately repassed to the calling program. in that case there is no approximation returned."));
       Spl << SavableClass::name2object(SplName.c_str());
   }


   ~Spl2DGenerator(){
       if (GenerateSpline!=1 ||  !Spl)
           return ;
       if (Val.empty() || Val.begin()->second.size() == 0)
           return;
       int ret = Spl->Generate(0, Val);
	    if (ret>0)
            throw info_except("Could not generate spline!!!. Return code is %i reason : \n",ret);
	    FilterTextOut out(SplFileName.c_str());
        out<<"Raw_SplineDescription_Str"<<SplineDescr.c_str()<<"\n";
	    out<<"GeneratedSpline\n";
	    out.SetRefMask(SavableClass::SimpleEdit);
	    out<<Spl;
   }
    Stroka MakeHelp(){
        Stroka res = "Class for generating and calculating splines. To make spline define <X> <Y> <Z>? <W>?. Work in 3 modes GenerateSpline: 0 - read spline, calculate points according to it (define x y?- get y z?). 1 - call with defined (x,y,z?) - will memorise data, create spline in the destructor. 2 - in the SplFileName file - will be points (x,y,z?) - calculate spline accordingly, run with (x,y?) - clc y Spline will not be saved.\n Currently present Splines:"+
            SavableClass::list_all_classes("Spline_category") + "\n";
        Stroka add;
        const int BufSize = 100000;
        char buf[BufSize+1];

        for (ClassDesc *desc = ClassDesc::list_category_start("Spline_category");desc;
            desc = ClassDesc::list_category_next(desc, "Spline_category") ){
                Ref<DescribedClass> cl = desc->create();
                Ref<ISpline> spl; spl << cl;
                if (!spl){
                    add += Stroka("Class ") + desc->name() + " is not ISpline type\n";
                    continue;
                }
                    //throw info_except("Class %s is not SplineXIO type...\n", cl->class_name());
                    //continue;
                add += Stroka("For class <") + spl->class_name() + "> input internal params are:[\n";
                std::strstream out(buf,BufSize);
                FilterTextOut output(out.rdbuf(),0);
                spl->save_generation_par(output);
                add += Stroka(buf,0,out.pcount()) + "\n]:\n";
				add += spl->MakeHelp();
        }
        res += add +
            ManyVarFunction::MakeHelp();
        return res;
    }

};


struct NoiseRemoval:ManyVarFunction{
    virtual double Calculate(const map<Stroka, double> &x, const Stroka &name){
        map<Stroka, double>::const_iterator it = x.find("X");
        if ( it==x.end() )
            throw info_except("Have to define <X> val to calculate data\n");
        Xval.push_back(it->second);
        it = x.find("Y");
        if ( it==x.end() )
            throw info_except("Have to define <Y> val to generate spline\n");
        Yval.push_back(it->second);
        it = x.find("W");
        if ( it!=x.end() )
            Wval.push_back(it->second);
        return Xval.size();
   }

   Stroka ResultFileName;
   vector<double> Xval, Yval, Wval;
   int StartNumDiv, MaxNumPnt, WriteDescr;
   double MaxMisfitVal, SameCoef;
   virtual int save_data_state(FilterOut&out){
       out<<" ResultFileName "<<ResultFileName<<" MaxMisfitVal "<<MaxMisfitVal<<" StartNumDiv "<<StartNumDiv<<" MaxNumPnt "<<MaxNumPnt<<" SameCoef "<<SameCoef<<" WriteDescr "<<WriteDescr;
       return 1;
   };
   virtual int read_data_state(FilterIn&in) {
       char tmp[256];
       in>>tmp>>ResultFileName>>tmp>>MaxMisfitVal>>tmp>>StartNumDiv>>tmp>>MaxNumPnt>>tmp>>SameCoef>>tmp>>WriteDescr;
	    if (!in)
		    throw info_except("Could not read spline???\n");

       return 1;
   }
   NoiseRemoval():ResultFileName("exp.dat"), StartNumDiv(20), MaxNumPnt(5000), WriteDescr(1), MaxMisfitVal(1), SameCoef(3) { }

    
   ~NoiseRemoval(){
       if (Xval.size()==0 || StartNumDiv==0)
           return;
       Stroka descr;
       vector< vector <double> > model = CurveAnalyzer().InitDat(Xval, Yval, StartNumDiv, MaxMisfitVal, MaxNumPnt, SameCoef).
           FormResult(Xval, Yval, descr);
       const char *head = descr.c_str();
       if (!WriteDescr)
           head = NULL;
       File::WriteFile(ResultFileName.c_str(), model, head, NULL);
   }
    Stroka MakeHelp(){
        Stroka res = "Class for noise removing from experiment signal. MaxMisfitVal - not used. Set initial intervals with StartNumDiv points, clc mean, deriv for them, using SameCoef - joins them, if interval is more then MaxNumPnt - split interval. Write mY - intervals with mean, deriv, mM - intervals with seond order curve, mD1 - deriv for the point, Mis - found misfits.\n"+
            ManyVarFunction::MakeHelp();
        return res;
    }

};


struct PolynomFunc:OneVarFunction{
    PolynomFunc():X0(0){
        Coef.push_back(1);
        Power.push_back(0);
    }
    vector<double> Coef, Power;
    double X0;
    virtual double Calculate(double x){
        double res = 0;
        x -= X0;
        for(size_t i = 0; i < Coef.size(); i++)
            res += Coef[i]*pow(x, Power[i]);
        return res;
    }
    virtual int read_data_state(FilterIn& input){
        char tmp[256];
        input>>tmp>>X0>>tmp>>tmp>>tmp;
        Coef.clear();
        while (Stricmp(tmp,"}")!=0 && !(!input) ){
            vector<Stroka> par = Str::SplitLine(tmp, 1, ':');
            Coef.push_back(atof(par[0].c_str()));
            Power.push_back(atof(par[1].c_str()));
            input>>tmp;
        }
        return !(!input);
    }
    virtual int save_data_state(FilterOut&output){
        output<<" X0 "<<X0<<" Coef:Power { ";
        for(size_t i = 0; i < Coef.size(); i++){
            char tmp[256];
            sprintf(tmp," %g:%g ",Coef[i], Power[i]);
            output<<tmp;
        }
        output<<" } ";
        return 1;
    }
    Stroka MakeHelp(){
        Stroka res = "Clc polynom function y = Sum(Coef[i]*(x-x0)^Power[i])\n";
        return res;
    }

};

#include "ext/wave++/source/wavelet.h"

struct NoiseWavelet:OneVarFunction {
    virtual double Calculate(double x){
        if (X2Y.find(x)==X2Y.end())
            return -1;
        return X2Y[x];
    }
    map<double, double> X2Y;
    int NoiseXcol, NoiseYcol, WaveletType, UsePlusMethod;
    Stroka NoiseFile;
    double Signal2NoiseRatio;
    virtual int read_data_state(FilterIn& input){
        char tmp[256];
        input>>tmp>>NoiseFile>>tmp>>NoiseXcol>>tmp>>NoiseYcol>>tmp>>WaveletType>>tmp>>Signal2NoiseRatio>>tmp>>UsePlusMethod;
        std::vector< std::vector<double> > vec;
        File::ReadFile(NoiseFile.c_str(), vec, "NoiseWavelet");
        if (vec.size() == 0)
            throw info_except("Could not read data NumRow==0\n");
        if (vec.size() <= (size_t)max<size_t>(NoiseYcol, NoiseXcol))
            throw info_except("Bad input file Ncol <%i> Nrow<%i> but NoiseXcol <%i> NoiseYcol <%i>\n", vec.size(), vec[0].size(), NoiseXcol, NoiseYcol);
        Wavelet wave;
        std::vector<double> Y;
        if (!UsePlusMethod)
            Y = wave.Construct(&vec[NoiseYcol][0], int(vec[NoiseYcol].size()), WaveletType, Signal2NoiseRatio);
        else
            Y = wave.ConstructPlus(&vec[NoiseYcol][0], int(vec[NoiseYcol].size()), WaveletType, Signal2NoiseRatio);
        for(size_t i = 0; i < Y.size(); i++)
            X2Y[vec[NoiseXcol][i]] = Y[i];
        return !(!input);
    }
    virtual int save_data_state(FilterOut&output){
        output<<" NoiseFile "<<NoiseFile.c_str()<<" NoiseXcol "<<NoiseXcol<<" NoiseYcol "<<NoiseYcol<<" WaveletType "<<WaveletType;
        output<<" Signal2NoiseRatio "<<Signal2NoiseRatio<<" UsePlusMethod "<<UsePlusMethod;
        return 1;
    }
    Stroka MakeHelp(){
        Stroka res = "Remove noise for given points";
        return res;
    }
};





