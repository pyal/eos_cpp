#include "fre_ros.h"
//#include "const.h"

RossParam *CalculateRossParam;
#define RP CalculateRossParam

//  Int(sin(qx)*x*f(d*x)*dx,x0,inf)

//  f(x)=exp(-bx)
double IntExpInf(double b,double q,double x0)
{
	double abd=b,qx0=q*x0,abd2=sqr(abd),q2=sqr(q);
	return exp(-abd*x0)/sqr(abd2+q2)*
		(q*cos(qx0)*(x0*abd2+2*abd+q2*x0)+sin(qx0)*(q2*(abd*x0-1)+abd2*(abd*x0+1)));
}
// The same, without sin(qx)
double IntExpInf1(double b,double x0)
{
	double abd=b,abdx=abd*x0,abdx2=sqr(abdx),abd3=abd*abd*abd;
	return exp(-abdx)*(abdx2+2+2*abdx)/abd3;
}

//  f(x)=exp(-bx); Int(,1,x0)
//double IntExpLow(double b,double q,double x0)
//{
//	return IntExpInf(b,q,1)-IntExpInf(b,q,x0);
//}
//  f(x)=x^-6; Int(,x0,inf)
double IntPow6Inf(double q,double x0)
{
	double qx0=q*x0,qx2=sqr(qx0),q4=pow(q,4),x4=pow(x0,4);
	double SiQ=MTSinIntegral(qx0);
	return ((cos(qx0)*qx0*(2-qx2)+sin(qx0)*(6-qx2))/x4+(0.5*M_PI-SiQ)*q4)/24;
}
// ============================================================
// ===================Strict ==================================
// ============================================================

double IntPow6Inf_Strict(double q,double x0)
{
	double x=q*x0,x2=sqr(x),q4=pow(q,4),x4=pow(x,4);
	double SiQ;
     double f, g, t, y;
     if (x > 16.) goto L10;
     t = x2 * .0078125 - 1.;
    y = t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (
     t * (t * (t * (t * (t * (t * (t * (t * 
     1.13364013523612467e-12-1.73295770867796541e-11)+2.34653547501289734e-10)-
	 2.9387085702927766e-9) +3.30969553150126728e-8) -3.32784295002924013e-7)+ 
     2.96606267125684584e-6)-2.32323765441590923e-5) +1.58305204989030543e-4)-
	 9.27069661302843286e-4)+0.00459705459778527538) -0.0189410064147072063) +
     0.0632412080713534156) -0.165103602060387617) +  0.31828183819586503) - 
	 0.40419356081893966) +  0.229433945244285525) +  0.161990304098876091) - 
     0.334271212963463397)+  0.0886834755381626329)+  0.132473585260859799)-
	 0.11050677668138556) +  0.137060472876350089;
    SiQ=x*y;return ((cos(x)*x*(2-x2)+sin(x)*(6-x2))/x4+0.5*M_PI-SiQ)*q4/24;
L10:
    y = x2;
    t = 512. / y - 1.;
    f = t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (
     t * (t * 4.8699066372784128e-14 - 
     1.09176921093439488e-13)+6.3498988357705728e-14) -2.39368626222505984e-13)+
	 1.15517351776520397e-12)-3.55518881942203699e-12)+1.17023188129910753e-11)- 
	 4.39951217228505001e-11)+1.84001853732121956e-10)-8.7433384997443117e-10) +
	 4.87245424704650801e-9) -3.32013637685389532e-8) +2.93937999539516224e-7) - 
	 3.71218824655850721e-6) +7.82215041547366874e-5) -0.00373729126869038934) + 
     .99618044190788651;
    g = t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (
     t * (t * (t * (t * 2.43669269913534464e-13 - 
     4.81013570050588672e-13)-1.06265802930487296e-13)-9.021839656198144e-14)+
	 2.93520978929726259e-12)-7.18307892366012006e-12)+1.70687678062319268e-11)-
	 5.22266851938134641e-11)+1.71515979253039184e-10)-6.02557598370451512e-10)+ 
     2.33450259515872927e-9) -1.01708486037094415e-8) +5.11015920578187682e-8) - 
	 3.06745558439750417e-7) +2.31342831851492447e-6) -2.36338137299290861e-5) + 
     3.68834391295988884e-4) -0.0108989877894521955)  +0.98870585937050572;
	return (cos(x)*(x*(2-x2)/x4+f/x)+sin(x)*((6-x2)/x4+g/y))*q4/24;
}
// ============================================================
// ===================Strict ==================================
// ============================================================

double MainIntegral(double q)
{
	double X=RP->X,Cut=0,Ec0=RP->Ec0,Ec1=RP->Ec1,P6=RP->P6,E0=RP->E0,E1=RP->E1;
	if (X>1) Cut=(IntExpInf(Ec1,q,1)-IntExpInf(Ec1,q,X))*Ec0;
	else X=1;
	double ExpI=IntExpInf(E1,q,X)*E0;
	double PowI=IntPow6Inf_Strict(q,X)*P6;
//cout<<q<<" "<<ExpI<<" "<<PowI<<" "<<(ExpI*6-PowI*Alpha)<<"\n";
//cout<<" q "<<q<<" LowInt "<<LowInt<<" ExpI "<<ExpI<<" PowI "<<PowI<<"\n";
//cout<<" LowIa "<<LowIa<<" LowIb "<<LowIb<<" d "<<d<<" q "<<q<<" x "<<x0/d<<"\n";
	return (ExpI-PowI+Cut);
};
double MainIntegralCut(double q)
{
	double X=RP->X,Cut=0,Ec0=RP->Ec0,Ec1=RP->Ec1;
	if (X>1) Cut=(IntExpInf(Ec1,q,1)-IntExpInf(Ec1,q,X))*Ec0;
	return Cut;
};
double MainIntegralNoCut(double q)
{
	double X=RP->X,P6=RP->P6,E0=RP->E0,E1=RP->E1;
	double ExpI=IntExpInf(E1,q,X)*E0;
	double PowI=IntPow6Inf_Strict(q,X)*P6;
	return (ExpI-PowI);
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
   return (1/(1-nc)-1)*q;
};
X_func DoubleIntegralFunc;
double DoubleIntegral(double q)
{
	double d=MainIntegral(q),r=PYfunc(q);
//	double d=DoubleIntegralFunc(q),r=PYfunc(q);
	double ret=d*r;
    return ret;
};
double UnityIntegral()
{
	double X=RP->X,Cut=0,Ec0=RP->Ec0,Ec1=RP->Ec1,P6=RP->P6,E0=RP->E0,E1=RP->E1;
	if (X>1) Cut=(IntExpInf1(Ec1,1)-IntExpInf1(Ec1,X))*Ec0;
	else X=1;
	double ExpI=IntExpInf1(E1,X)*E0;
	double PowI=P6/(3*pow(X,3));
//cout<<q<<" "<<ExpI<<" "<<PowI<<" "<<(ExpI*6-PowI*Alpha)<<"\n";
//cout<<" q "<<q<<" LowInt "<<LowInt<<" ExpI "<<ExpI<<" PowI "<<PowI<<"\n";
//cout<<" LowIa "<<LowIa<<" LowIb "<<LowIb<<" d "<<d<<" q "<<q<<" x "<<x0/d<<"\n";
	return (ExpI-PowI+Cut);
};

double IntExp6Correlation(double niu)
{
	double d=RP->S_Diameter;
    double Epsilon=RP->ParInteract[1];
    double Alpha  =RP->ParInteract[2];
    double Radius =RP->ParInteract[3];

	RP->Ec0=RP->ParInteract[4];
	RP->Ec1=-RP->ParInteract[5]*d;
	RP->E0=Epsilon/(Alpha-6)*6*exp(Alpha);
	RP->E1=d*Alpha/Radius;
	RP->P6=Epsilon/(Alpha-6)*Alpha*pow(Radius/d,6);
	RP->X=RP->ParInteract[0]/d;
	int Flag=0;
	double UInt=UnityIntegral(),DInt=0;
//	if (RP->X>1) 
//		{DoubleIntegralFunc=MainIntegralCut;
//		 DInt=RecInt(DoubleIntegral,0.1*StndErr,25,StndErr);}
//	else RP->X=1;
//	DoubleIntegralFunc=MainIntegralNoCut;
//double InFinQunc10(X_func Func,double A,double AbsErr,double InFinZero,
//                   int NumZero,long MaxFunc,int &Flag)
	DInt+=
//		InFinQulb9(DoubleIntegral,0.1*StndErr,StndErr,10*StndErr,10,10000,Flag);
		RecInt(DoubleIntegral,0.1*StndErr,25,0.001);
	if (Flag!=0) cout<<" Flag "<<Flag<<"\n";
	return UInt*12*niu+DInt/M_PI;
};

double PY_Min(double niu)
{
   RP->S_Niu=niu;
//   S_Temp=Rconst/MolVeight*T; S_DensDia=Denc/MolVeight;
   double C=M_Na/1e24;
   RP->S_Diameter=pow(6*niu/(RP->S_DensDia*M_PI*C),1./3);
// To obtain S_Diameter in Bor radius
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
            (log(max<double>(1/Denc,MathZer))+RP->CvId*log(T)+RP->HiT+
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
            (log(max<double>(1/Denc,MathZer))+RP->CvId*log(T)+RP->HiT+
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
	
