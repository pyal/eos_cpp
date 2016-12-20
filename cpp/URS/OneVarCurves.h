#pragma once

#include "mat\fun_fac.h"
#include "mat\specfunc.h"
#include "lib/ref/str_manip.h"
#include "lib/ref/file_manip.h"
#include "lib/ref/data_manip.h"
#include "mat\curveanalyzer.h"

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
        double Mul = exp(-x*niu);
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
        double LgZ = log10(Z);
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

struct ManyVarSplGenerator:ManyVarFunction{
    virtual double Calculate(const map<Stroka, double> &x, const Stroka &name){
        if (!GenerateSpline){
            if (name=="Xmin")
                return Spl->Xlow;
            if (name=="Xmax")
                return Spl->Xup;
        }
        map<Stroka, double>::const_iterator it = x.find("X");
        if ( it==x.end() )
            throw info_except("Have to define <X> val to calculate data\n");
        if (!GenerateSpline){
            double ret;
            double v = it->second;
            Spl->Evaluate(&v, &ret, 1);
            return ret;
        }
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

   Stroka SplFileName, SplineDescr, SplName;
   int GenerateSpline, SplineOrder;
   Ref<SplineXIO> Spl;
   vector<real> Xval, Yval, Wval;
   map<int, Stroka> ErrorDescr;
   virtual int save_data_state(FilterOut&out){
       out<<" SplineFile "<<SplFileName<<" GenerateSpline "<<GenerateSpline<<" SplineDescr "<<SplineDescr<<" SplineOrder "<<SplineOrder<<" SplineClass "<<SplName;
       Spl->save_generation_par(out);
       return 1;
   };
   virtual int read_data_state(FilterIn&in) {
        char tmp[256];
        in>>tmp>>SplFileName>>tmp>>GenerateSpline>>tmp>>SplineDescr>>tmp>>SplineOrder>>tmp>>SplName;
        Spl << SavableClass::name2object(SplName.c_str());
        if (!Spl)
            throw info_except(" Object %s is not of SplinXIO type\n", SplName.c_str());
        Spl->read_generation_par(in);
        if (!GenerateSpline){
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
            Xval.clear();Yval.clear();Wval.clear();
       };
	    if (!in)
		    throw info_except("Could not read spline???\n");

       return 1;
   }
   ManyVarSplGenerator():GenerateSpline(0), SplFileName("splFile.spl"), Spl(new CurveSpline), SplineOrder(3), SplineDescr("Simple_xy_spline"), SplName("CurveSpline"){
       ErrorDescr = Str::MakeErrDescr(Stroka("0:OK mis is in range;-1:OK interpolation;-2:OK;1:Error. The required storage space exceeds the available storage space, as specified by the parameter nest probably causes nest too small. if nest is already large (say nest > m/2), it may also indicate that s is too small;2:error. a theoretically impossible result was found during the iteration proces for finding a smoothing spline with fp = s. probably causes s too small.;3:error. the maximal number of iterations maxit (set to 20  by the program) allowed for finding a smoothing spline with fp=s has been reached. probably causes  s too small;10:error. on entry, the input data are controlled on validity the following restrictions must be satisfied.  -1<=iopt<=1, 1<=k<=5, m>k, nest>2*k+2, w(i)>0,i=1,2,...,m  xb<=x(1)<x(2)<...<x(m)<=xe, lwrk>=(k+1)*m+nest*(7+3*k)  if iopt=-1 2*k+2<=n<=min(nest,m+k+1)  xb<t(k+2)<t(k+3)<...<t(n-k-1)<xe  the schoenberg-whitney conditions, i.e. there  must be a subset of data points xx(j) such that  t(j) < xx(j) < t(j+k+1), j=1,2,...,n-k-1  if iopt>=0 s>=0  if s=0  nest >= m+k+1  if one of these conditions is found to be violated,control  is immediately repassed to the calling program. in that case there is no approximation returned."));
       Spl << SavableClass::name2object(SplName.c_str());
   }


   ~ManyVarSplGenerator(){
       if (!GenerateSpline ||  !Spl)
           return ;
       if (Xval.size()==0)
           return;
       //assert(Xval.size()!=0);
        TColumn<real> dat;dat.Delete();
        dat.AddColumn(&Xval[0], (int)Xval.size());
        dat.AddColumn(&Yval[0], (int)Yval.size());
        int ret;
        if (Wval.size()>0){
            dat.AddColumn(&Wval[0], (int)Wval.size());
            ret = Spl->Generate(0, dat, 0, 1, 2);
        } else
	        ret = Spl->Generate(0,dat);
	    if (ret>0)
            throw info_except("Could not generate spline!!!. Return code is %i reason : \n%s\n",ret, ErrorDescr[ret].c_str());
	    FilterTextOut out(SplFileName.c_str());
        out<<"Raw_SplineDescription_Str"<<SplineDescr.c_str()<<"\n";
	    //out<<"SplMisfit"<<SplMisfit<<"NumSplX"<<NumSplX<<"NumSplY"<<NumSplY<<"\n";
	    //out<<"LogX"<<LogX<<"LogY"<<LogY<<"LogZ"<<LogZ<<"AddBeforeLogX"<<AddBeforeLogX<<"AddBeforeLogY"<<AddBeforeLogY<<"AddBeforeLogZ"<<AddBeforeLogZ<<"\n";
	    //out<<"MinX"<<DecodeVal(X[0],LogX,AddBeforeLogX)<<"MaxX"<<DecodeVal(X[X.SetNumEl()-1],LogX,AddBeforeLogX)
	    //   <<"MinY"<<DecodeVal(Y[0],LogY,AddBeforeLogY)<<"MaxY"<<DecodeVal(Y[Y.SetNumEl()-1],LogY,AddBeforeLogY)<<"\n";
	    out<<"GeneratedSpline\n";
	    //out.SetRefMask(SavableClass::UnlinkedObjects);
	    out.SetRefMask(SavableClass::SimpleEdit);
	    out<<Spl;
   }
    Stroka MakeHelp(){
        Stroka res = "Class for generating and calculating splines -> y(x). Do make spline define <X> <Y> and may <W>\n Currently present Splines:"+
            SavableClass::list_all_classes("Spline_category") + "\n";
         //spl = ClassDesc::list_category_obj_start("Spline_category");
        Stroka add;
        const int BufSize = 100000;
        char buf[BufSize+1];

        for (ClassDesc *desc = ClassDesc::list_category_start("Spline_category");desc;
            desc = ClassDesc::list_category_next(desc, "Spline_category") ){
                Ref<DescribedClass> cl = desc->create();
                Ref<SplineXIO> spl; spl << cl;
                if (!spl){
                    add += Stroka("Class ") + desc->name() + " is not SplineXIO type\n";
                    continue;
                }
                    //throw info_except("Class %s is not SplineXIO type...\n", cl->class_name());
                    //continue;
                add += Stroka("For class <") + spl->class_name() + "> input internal params are:[\n";
                std::strstream out(buf,BufSize);
                FilterTextOut output(out.rdbuf(),0);
                spl->save_generation_par(output);
                add += Stroka(buf,0,out.pcount()) + "\n]:\n";
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
   int StartNumDiv, MinNumDiv;
   double MaxMisfitVal;
   virtual int save_data_state(FilterOut&out){
       out<<" ResultFileName "<<ResultFileName<<" MaxMisfitVal "<<MaxMisfitVal<<" StartNumDiv "<<StartNumDiv<<" MinNumDiv "<<MinNumDiv;
       return 1;
   };
   virtual int read_data_state(FilterIn&in) {
       char tmp[256];
       in>>tmp>>ResultFileName>>tmp>>MaxMisfitVal>>tmp>>StartNumDiv>>tmp>>MinNumDiv;
	    if (!in)
		    throw info_except("Could not read spline???\n");

       return 1;
   }
   NoiseRemoval():ResultFileName("exp.dat"), StartNumDiv(100), MaxMisfitVal(1), MinNumDiv(5) { }

   //struct SolDat{
   //    double mean, sigma, meand1;
   //    double x_center;
   //    double curveMis;
   //    void Set(double m, double d1, double s, double x, int ib, int ie){
   //        mean = m;
   //        sigma = s;
   //        meand1 = d1;
   //        x_center = x;
   //        iBeg = ib;
   //        iEnd = ie;
   //        SortY.clear();
   //    };
   //    int iBeg, iEnd;

   //    vector<int> SortY;
   //    vector<double> val, Noise;
   //};
   //struct Sol{
   //    Sol(const SolDat &d):x(d.x_center), y(d.mean), d1(d.meand1), sig(d.sigma), nPnt(d.iEnd-d.iBeg), curveMis(d.curveMis),
   //        iBeg(d.iBeg), iEnd(d.iEnd), Noise(d.Noise){};
   //    static vector<vector<double > > FormResult(const vector<Sol> &sol){
   //        vector<vector<double > > vec(6);
   //        for(size_t i = 0;i<sol.size();i++){
   //            vec[0].push_back(sol[i].x);
   //            vec[1].push_back(sol[i].y);
   //            vec[2].push_back(sol[i].d1);
   //            vec[3].push_back(sol[i].sig);
   //            vec[4].push_back(sol[i].curveMis);
   //            vec[5].push_back(sol[i].nPnt);
   //        }
   //        return vec;
   //    }
   //    static vector<vector<double > > FormResult(const vector<Sol> &sol, const vector<double> &Xval, const vector<double> &Yval){
   //        vector<vector<double > > vec(6);
   //        //vector<vector<double > > vec(5);
   //        for(size_t i = 0;i<sol.size();i++){
   //            Sol tmp = sol[i];
   //            for (int i1 = sol[i].iBeg;i1<sol[i].iEnd;i1++){
   //                vec[0].push_back(Xval[i1]);
   //                vec[1].push_back(Yval[i1] - sol[i].sig*sol[i].Noise[i1-sol[i].iBeg]);
   //                vec[2].push_back(sol[i].d1);
   //                vec[3].push_back(sol[i].sig);
   //                vec[4].push_back(sol[i].curveMis);
   //                vec[5].push_back(sol[i].nPnt);
   //                //vec[2].push_back(Yval[i1]);
   //                //vec[3].push_back(sol[i].sig*sol[i].Noise[i1-sol[i].iBeg]);
   //                //vec[4].push_back(Yval[i1] + sol[i].sig*sol[i].Noise[i1-sol[i].iBeg]);
   //            }
   //        }
   //        return vec;
   //    }
   //    double x, y, d1, sig, curveMis;
   //    int nPnt, iBeg, iEnd;
   //    vector<double> Noise;
   //};
    
   ~NoiseRemoval(){
       if (Xval.size()==0 || StartNumDiv==0)
           return;
       //vector<Sol> Result;
       //Splitter(Result, StartNumDiv, 0, (int)Yval.size()-1);
       double SameCoef = 1;
       Stroka descr;
       CurveAnalyzer anal;anal.InitDat1(Xval, Yval, StartNumDiv, MaxMisfitVal, MinNumDiv, SameCoef);
       vector< vector <double> > model = anal.FormResult(Xval, Yval, descr);
       File::WriteFile(ResultFileName.c_str(), model, descr.c_str(), NULL);
   }
//   void Splitter(vector<Sol> &LeftPnt, int NumInt, int iBeg, int iEnd){
//       VecCl Pnt = VecCl::MakeStepVector(NumInt+1, iBeg, iEnd, 0);
////cout<<"beg "<<iBeg<<" end "<<iEnd<<" n "<<NumInt<<" vec "<<Pnt;
//       SolDat dat;
//       for(int i = 1;i<Pnt.Dim();i++){
//           double PntI = (int)Pnt[i];
//           if (i!=1)
//               PntI = LeftPnt[LeftPnt.size()-1].iEnd;
//           MakeSolDat(PntI, (int)Pnt[i+1], dat, LeftPnt, i==Pnt.Dim()-1);
//           if (!GoodDat(dat))
//               Splitter(LeftPnt, 2, PntI, Pnt[i+1]);
//           else{
//               LeftPnt.push_back(Sol(dat));
////cout<<dat.x_center<<" "<<dat.mean<<"\n";
//           }
//       }
//   }
//   void MakeSolDat(int iBeg, int iEnd, SolDat &dat, vector<Sol> &LeftPnt, bool LastPnt){
//       if (iBeg==iEnd-1){
//           dat.Set(Yval[iBeg], 0, 0, Xval[iBeg], iBeg, iEnd);
//           return;
//       }
//       double s = 0, x = (Xval[iBeg] + Xval[iEnd])*0.5;
//       int N = iEnd - iBeg;
//       double a0, a1;
//       vector<double> vec = ClcDerivConst(a0,a1,iBeg, iEnd, LeftPnt);
//       for(size_t i = 0;i<vec.size();i++){
//           s += sqr(vec[i]);
//       }
//       int i;
//       if (!LastPnt){
//           while(1){
//               double s0 = s;
//               for(i = iEnd;i>iBeg+6;i--)
//                   if (fabs(vec[i-1-iBeg])<sqrt(s0/N))
//                       break;
//                   else
//                       s0 -= sqr(vec[i-1-iBeg]);
//               if (i==iEnd)
//                   break;
//               iEnd = i;
//               //vec.resize(iEnd-iBeg);
//               //s = s0;
//               vec = ClcDerivConst(a0,a1,iBeg, iEnd, LeftPnt);
//               for(size_t i = 0;i<vec.size();i++){
//                   s += sqr(vec[i]);
//               }
//           }
//       }
//
//       dat.Set(a0, a1, sqrt( s/N ), x, iBeg, iEnd);
//       dat.val = vec;
//       dat.SortY = DataManip::Sort(vec, 1);
//   }
//   vector<double> ClcDerivConst(double &a0, double &a1, int iBeg, int iEnd, vector<Sol> &LeftPnt){
//       int N = iEnd - iBeg;
//       double Mean = (N-1)*0.5;
//       double x0 = iBeg+Mean, sXY = 0, sX2 = 0, sY = 0;
//       for(int i = iBeg;i<iEnd;i++){
//           double x = i - x0, y = Yval[i];
//           sY += y;
//           sXY += (x*y);
//           sX2 += x*x;
//       }
//       //a0 = sY/(N-1);
//       a0 = sY/N;
//       a1 = sXY/sX2;
//       //if (LeftPnt.size() > 0){
//       //    Sol &sol = LeftPnt[LeftPnt.size()-1];
//       //    double dxm = sol.iEnd - sol.iBeg, dxp = iEnd - iBeg;
//       //    a1 = -(2*(sol.y - a0)+sol.d1*dxm)/dxp;
//       //} else {
//       //    a1 = 0;
//       //}
//       //a1 = 0;
//       vector<double> vec;
//       DataManip::CopyVector(vec, Yval, iBeg, iEnd);
//       //x0 = (vec.size()-1)*0.5;
//       x0 = Mean;
//       for(size_t i = 0;i<vec.size();i++)
//           vec[i] -= (a0 + a1 *(i-x0)); ;
//       return vec;
//   }
//   int GoodDat(SolDat &dat){
//       int N = (int)dat.SortY.size();
//       dat.curveMis = 0;
//       dat.Noise.resize(N);
//       DataManip::SetVector(dat.Noise, 0);
//       if (N<3 )
//           return 1;
//       double s = 0, s0 = 0;
//       if (dat.sigma<MathZer)
//           return 1;
//       //vector<double> vec;DataManip::CopyVector(vec, Yval, dat.iBeg, dat.iEnd);
//       dat.Noise.resize(N);
//       int np = 0, nm = 0, cent = (N-1)*0.5;
//       for(int i = 0;i<N;i++){
//           double noise = ClcDistribInt(i, dat);
//           s += sqr( (dat.val[dat.SortY[i]])/dat.sigma - noise );
//           s0 += sqr( (dat.val[dat.SortY[i]])/dat.sigma );
//           if (i<cent)
//               if (dat.SortY[i]>cent)
//                   np++;
//               else
//                   nm++;
//           dat.Noise[dat.SortY[i]] = noise;
//           //cout<<i<<" "<<(vec[dat.SortY[i]]-dat.mean)/dat.sigma <<" "<<(dat.val[dat.SortY[i]] - dat.mean)/dat.sigma <<" "<<ClcDistribInt(i, dat)<<"\n";
//           //cout<<i<<" "<<dat.val[i] <<" "<<(dat.val[dat.SortY[i]] - dat.mean)/dat.sigma <<" "<<ClcDistribInt(i, dat)<<"\n";
//           //cout<<i<<" "<<(dat.val[dat.SortY[i]]-dat.mean)/dat.sigma<<" "<<ClcDistribInt(i, dat)<<" "<<dat.sigma<<"\n";
//           //cout<<i<<" "<<(dat.val[dat.SortY[i]])/dat.sigma<<" "<<ClcDistribInt(i, dat)<<" "<<dat.SortY[i]<<"\n";
//       }
////exit(1);
//       //s = sqrt(s/N);
//       s += sqr(np-nm);
//       s = sqr(N)*sqrt(s);
//       //cout<<N<<" "<<s<<"\n";
//       dat.curveMis = s;
//       if (N<MinNumDiv)
//            return 1;
//       return (s<MaxMisfitVal && s0<0.3*s);
//   }
//   double ClcDistribInt(int i, const SolDat &dat){
//       double x = (i+0.5)/(dat.iEnd - dat.iBeg);
//       double Est = SpecFunc::iErf(x);
//       return Est;
//   }
    Stroka MakeHelp(){
        Stroka res = "Class for noise removing from experiment signal. Define <X> <Y> and may <W>\n"+
            ManyVarFunction::MakeHelp();
        return res;
    }

};












