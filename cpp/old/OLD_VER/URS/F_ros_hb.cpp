#include "fre_ros.h"
//#include "const.h"

RossParam *CalculateRossParam;
#define RP CalculateRossParam

//  Int(sin(qx)*x*f(d*x)*dx,x0,inf)

//  f(x)=exp(a-bx)
double IntExpInf(double a,double b,double d,double q,double x0)
{
	double abd=b*d,qx0=q*x0,abd2=sqr(abd),q2=sqr(q);
	return exp(a-abd*x0)/sqr(abd2+q2)*
		(q*cos(qx0)*(x0*abd2+2*abd+q2*x0)+sin(qx0)*(q2*(abd*x0-1)+abd2*(abd*x0+1)));
}
// The same, without sin(qx)
double IntExpInf1(double a,double b,double d,double x0)
{
	double abd=b*d,abdx=abd*x0,abdx2=sqr(abdx),abd3=abd*abd*abd;
	return exp(a-abdx)*(abdx2+2+2*abdx)/abd3;
}

//  f(x)=a*exp(bx); Int(,1,x0)
double IntExpLow(double a,double b,double d,double q,double x0,int One=0)
{
	if (!One) return -a*(IntExpInf(0,-b,d,q,x0)-IntExpInf(0,-b,d,q,1));
}
double IntExpLow1(double a,double b,double d,double x0,int One=0)
{
	if (!One) return -a*(IntExpInf1(0,-b,d,x0)-IntExpInf1(0,-b,d,1));
}
//  f(x)=x^-6*a; Int(,x0,inf)
double IntPow6Inf(double a,double b,double d,double q,double x0)
{
	double qx0=q*x0,qx2=sqr(qx0),q4=pow(q,4),x4=pow(x0,4),d6=pow(d,6);
	double SiQ=MTSinIntegral(qx0);
	return a/(d6*24)*((cos(qx0)*qx0*(2-qx2)+sin(qx0)*(6-qx2))/x4+(0.5*M_PI-SiQ)*q4);
}
double IntPow6Inf1(double a,double b,double d,double x0)
{
	double x3=pow(x0,3),d6=pow(d,6);
	return a/(d6*3*x3);
}

//void FindExp6Cut(RossParam *par){};

// IntCorrellation=Int((S(q,niu)-1)/PI*Int1*dq,0,inf)+12*niu*Int(x^2*Exp6(d*x)*dx,1,inf)
// Int1=Int(x*Exp6(d*x)*sin(q*x)*dx,1,inf)
double CutIntegral(double q)
{
	double d=RP->S_Diameter;
    double Radius =RP->ParInteract[3];
	double x0=RP->ParInteract[0];//,PowIa=pow(Radius,6);
	double LowInt=IntExpLow(1,RP->ParInteract[5],d,q,x0/d);
//cout<<q<<" "<<LowInt<<"\n";
	return LowInt;
};
double MainIntegral(double q)
{
    double Epsilon=RP->ParInteract[1];
    double Alpha  =RP->ParInteract[2];
    double Radius =RP->ParInteract[3];
	double d=RP->S_Diameter;
	double x0=RP->ParInteract[0];
	double ExpIa=Alpha,ExpIb=Alpha/Radius,PowIa=pow(Radius,6);
	if (d>x0) x0=d;
	double ExpI=IntExpInf(ExpIa,ExpIb,d,q,x0/d);
	double PowI=IntPow6Inf(PowIa,1,d,q,x0/d);
//cout<<q<<" "<<ExpI<<" "<<PowI<<" "<<(ExpI*6-PowI*Alpha)<<"\n";
//cout<<" q "<<q<<" LowInt "<<LowInt<<" ExpI "<<ExpI<<" PowI "<<PowI<<"\n";
//cout<<" LowIa "<<LowIa<<" LowIb "<<LowIb<<" d "<<d<<" q "<<q<<" x "<<x0/d<<"\n";
	return (ExpI*6-PowI*Alpha);
};

double PYfunc(double q)
{
   double niu=RP->S_Niu;
   double q2=sqr(q),sq=sin(q),cq=cos(q),niu2=sqr(niu-1);
   double nc=-1/(pow(q,6)*sqr(niu2))*24*niu*
             (sqr(2*niu+1)*(sq-q*cq)*q2*q+
              1.5*niu*sqr(niu+2)*((q2-2)*cq-2*q*sq+2)*q2-
              0.5*niu*sqr(2*niu+1)*((sqr(q2)-12*q2+24)*cos(q)-
              4*(q*(q2-6)*sq+6)));
//cout<<" py "<<(1/(1-nc)-1)*q<<"\n";
   return (1/(1-nc)-1)*q;
};
X_func DoubleIntegralArg;
double DoubleIntegral(double q)
{
//   double Int=DoubleInt1(q);
//cout<<q<<" "<<(1/(1-nc)-1)*Int*q<<" "<<Int<<"\n";
	double d=DoubleIntegralArg(q),r=PYfunc(q);
	double ret=d*r;
//cout<<"  "<<q<<" "<<d<<" "<<r<<" "<<ret<<"\n";
   return ret;
};
double UnityIntegral()
{
   double Epsilon=RP->ParInteract[1];
   double Alpha  =RP->ParInteract[2];
   double Radius =RP->ParInteract[3];
//   return Epsilon/(Alpha-6)*(6*exp(Alpha*(1-r/Radius))-Alpha*pow(Radius/r,6));

	double d=RP->S_Diameter;
	double x0=RP->ParInteract[0];
	double ExpIa=Alpha,ExpIb=Alpha/Radius;
	double PowIa=pow(Radius,6),PowIb=1;
	if (d>=x0) { return (IntExpInf1(ExpIa,ExpIb,d,1)*6-
						IntPow6Inf1(PowIa,PowIb,d,1)*Alpha)*Epsilon/(Alpha-6);}
	double LowIa=1,LowIb=RP->ParInteract[5];
	double LowInt=IntExpLow1(LowIa,LowIb,d,x0/d)*RP->ParInteract[4];
	return (IntExpInf1(ExpIa,ExpIb,d,x0/d)*6-
						IntPow6Inf1(PowIa,PowIb,d,x0/d)*Alpha)*Epsilon/(Alpha-6)+LowInt;
};

double IntExp6Correlation(double niu)
{
    double Epsilon=RP->ParInteract[1];
    double Alpha  =RP->ParInteract[2];
	double UInt=UnityIntegral();
//double InFinQunc10(X_func Func,double A,double AbsErr,double InFinZero,
//                   int NumZero,long MaxFunc,int &Flag);
	int Flag=0;
	double d=RP->S_Diameter,x0=RP->ParInteract[0],DInt=0;
	if (d<x0) 
	{ 
		DoubleIntegralArg=CutIntegral;
		DInt=
//			InFinQunc10(DoubleIntegral,0.1*StndErr,StndErr,100,10,1000,Flag);
			RecInt(DoubleIntegral,0.1*StndErr,25,StndErr);
		if (Flag!=0) cout<<" Flag "<<Flag<<"\n";
		DInt=DInt*RP->ParInteract[4];
	}
	DoubleIntegralArg=MainIntegral;
	DInt+=
//		InFinQunc10(DoubleIntegral,0.1*StndErr,StndErr,100,10,1000,Flag)
		RecInt(DoubleIntegral,0.1*StndErr,25,StndErr)
			*Epsilon/(Alpha-6);
	if (Flag!=0) cout<<" Flag "<<Flag<<"\n";
	return UInt*12*niu+DInt/M_PI;
};

double PY_Min(double niu)
{
   RP->S_Niu=niu;
//   S_Temp=Rconst/MolVeight*T; S_DensDia=Denc/MolVeight;
// To obtain S_Diameter in Bor radius
   double C=M_Na/1e24;
   RP->S_Diameter=pow(6*niu/(RP->S_DensDia*M_PI*C),1./3);
//*2.238515;
   double niu2=sqr(niu);
   double F_HS=(4*niu-3*niu2)/(1-niu2);
   double F_SS=-(sqr(niu2)*0.5+niu2+niu*0.5);
//   int Err;
//   double MaxR=RP->S_MaxR_PY*RP->S_Diameter;
   double Int=IntExp6Correlation(niu);
//   return RP->S_Temp*(F_HS+F_SS)+
	   //RP->S_DensDia*RP->Rconst*
//	   Int;//*0.5;
   return Int;
}; 

double FreeSSVT(RossParam *par,double Denc,double T)
  {
   RP=par;
   if ((T<MathZer) || (Denc<MathZer)) 
     { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
   RP->S_Temp=RP->Rconst/RP->MolVeight*T;RP->S_DensDia=Denc/RP->MolVeight;
//   RP->S_MaxR_PY=6;
   double PY_add;
   if ((fabs(RP->DL-Denc)<1e-2) && (fabs(RP->TL-T)<1)) ;
   else { RP->DL=Denc;RP->TL=T;
//          RP->NiuL=MinFunc(0.0001,0.7,PY_Min,1e-4);}
   if (!NAGMin(PY_Min,1e-4,1e-4,0.0001,0.7,1000,RP->NiuL,PY_add))
      cout<<" Did not find RP->NiuL. Try old version\n";}

   PY_add=PY_Min(RP->NiuL);
   double F=RP->Zero+PY_add-RP->S_Temp*
            (log(max(1/Denc,MathZer))+RP->CvId*log(T)+RP->HiT+
                 log(1-exp(-RP->Tvib/T)));
   return F;
  }; 
double FreeRCheck(RossParam *par,double Denc,double T,double niu)
  {
   RP=par;
   RP->S_Temp=RP->Rconst/RP->MolVeight*T;RP->S_DensDia=Denc/RP->MolVeight;
   RP->NiuL=niu;
   double PY_add;
//   RP->DL=Denc;RP->TL=T;RP->NiuL=MinFunc(0.0001,0.7,PY_Min,1e-4);}
   PY_add=PY_Min(RP->NiuL);
   double F=RP->Zero+PY_add-RP->S_Temp*
            (log(max(1/Denc,MathZer))+RP->CvId*log(T)+RP->HiT+
                 log(1-exp(-RP->Tvib/T)));
   return PY_add;
  }; 

double InteractFunc(double r)
  {
   if (r<RP->ParInteract[0]) return RP->ParInteract[4]*exp(RP->ParInteract[5]*r);
   double Epsilon=RP->ParInteract[1];
   double Alpha  =RP->ParInteract[2];
   double Radius =RP->ParInteract[3];
//cout<<"  "<<r<<" "<<Epsilon/(Alpha-6)*(6*exp(Alpha*(1-r/Radius))-Alpha*pow(Radius/r,6))<<"\n";
   return Epsilon/(Alpha-6)*(6*exp(Alpha*(1-r/Radius))-Alpha*pow(Radius/r,6));
  }
int FreeERossRead(fstream &in,RossParam *&par)
  {
//cout<<" FreeERoss reading\n";
   par=new RossParam;RP=par;
   char tmp[256];
   in>>tmp>>RP->CvId>>tmp>>RP->MolVeight>>tmp>>RP->Zero>>tmp>>RP->HiT>>tmp>>RP->Tvib;
   in>>tmp>>RP->ParInteract[1]>>tmp>>RP->ParInteract[2]>>tmp>>RP->ParInteract[3];
   in>>tmp>>RP->ParInteract[0];
   in>>tmp>>tmp;//spl_name=strdup(tmp);
//   fstream sp(tmp,ios::in);
   int ret=1;
//cout<<" FreeERoss spline registering. Name: "<<tmp<<"\n";
//   if (!(ret=SurfRegister(sp,RP->PercYerv))) 
//       {cout<<" Could not register Percus. file: "<<tmp
//            <<" :\nNow exititng\n";exit(0);}
//cout<<" FreeERoss Entered. Coreleft "<<Coreleft()<<"\n";
//   S_ParInteract=&ParInteract[0];
   double x=RP->ParInteract[0]*(1+StndErr);
   double c=InteractFunc(x);
   double c1=InteractFunc(x*(1+StndErr));
   c1=(c1-c)/(x*StndErr);
   RP->ParInteract[4]=c/(exp(c1/c*x));
   RP->ParInteract[5]=c1/c;
cout<<" FreeERoss Entered. Coreleft "<<Coreleft()<<" Cut A "<<RP->ParInteract[4]<<" Cut B "<<RP->ParInteract[5]<<"\n";
//exit(0);
   return 1;
  };
	
/*   
double DoubleInt1(double q)
{
//   if (r<RP->ParInteract[0]) return RP->ParInteract[4]*exp(RP->ParInteract[5]*r);
   double Epsilon=RP->ParInteract[1];
   double Alpha  =RP->ParInteract[2];
   double Radius =RP->ParInteract[3];
//   return Epsilon/(Alpha-6)*(6*exp(Alpha*(1-r/Radius))-Alpha*pow(Radius/r,6));

	double d=RP->S_Diameter;
	double x0=RP->ParInteract[0];
	double ExpIa=Alpha,ExpIb=Alpha/Radius;
	double PowIa=1,PowIb=1;
	if (d>=x0) { return (IntExpInf(ExpIa,ExpIb,d,q,1)*6-
						IntPow6Inf(PowIa,PowIb,d,q,1)*Alpha)*Epsilon/(Alpha-6);}
	double LowIa=1,LowIb=RP->ParInteract[5];
	double LowInt=IntExpLow(LowIa,LowIb,d,q,x0/d);
	double ExpI=IntExpInf(ExpIa,ExpIb,d,q,x0/d);
	double PowI=IntPow6Inf(PowIa,PowIb,d,q,x0/d);
//cout<<" q "<<q<<" LowInt "<<LowInt<<" ExpI "<<ExpI<<" PowI "<<PowI<<"\n";
//cout<<" LowIa "<<LowIa<<" LowIb "<<LowIb<<" d "<<d<<" q "<<q<<" x "<<x0/d<<"\n";
	return (ExpI*6-PowI*Alpha)*Epsilon/(Alpha-6)+LowInt*RP->ParInteract[4];
};
*/
