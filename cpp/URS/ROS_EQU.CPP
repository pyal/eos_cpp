#include <lib\precompiled\eos.h>

#include "ros_equ.h"

#include "mat\Quad.h"





double Exp6Param::Pow6Clc(double r, const Exp6Param &par){
    if (r<par.RadiusExp) return par.MulExp*exp(par.ArgExp*r);
    double Epsilon=par.Epsilon;
    double Alpha  =par.Alpha;
    double Radius =par.Radius0;
    return Epsilon/(Alpha-6)*(6*exp(Alpha*(1-r/Radius))-Alpha*pow(Radius/r,6));
}
double Exp6Param::Pow6ClcDeriv(double r, const Exp6Param &par){
    double Epsilon=Current->Epsilon;
    double Alpha  =Current->Alpha;
    double Radius =Current->Radius0;
    //return Epsilon/(Alpha-6)*( -6*Alpha/Radius*exp(Alpha*(1-r/Radius))+
    //    6*Alpha/Radius*pow(Radius/r,7) );
    return Epsilon/(Alpha-6)*6*Alpha/Radius*( -exp(Alpha*(1-r/Radius))+
        pow(Radius/r,7) );

}
double Exp6Param::Pow6ClcDeriv2(double r){
    double Epsilon=Current->Epsilon;
    double Alpha  =Current->Alpha;
    double Radius =Current->Radius0;
// This is simplified deriv only
    double ret = exp( Alpha*(1-r/Radius) )*Alpha - 7*pow(fabs(Radius/r),8);
//cout<<" r "<<r<<" D2 "<<ret<<"\n";
//    return ret*1e5;
    return ret;

}
Exp6Param* Exp6Param::Current=NULL;
int Exp6Param::FindRexp(Exp6Param &Rexp){
    Rexp.Alpha=fabs(Rexp.Alpha);
    Rexp.Radius0 = fabs(Rexp.Radius0);
    Current = &Rexp;
    double minR, maxR, Cent, Al = Rexp.Alpha, Ra = Rexp.Radius0;
    Cent = Ra*pow(7/Al,1./8)*exp(-Al/8);
    minR = Cent;maxR = Cent*2.5;

    Rexp.RadiusExp = (minR+maxR)*0.5;
    Fzero(Pow6ClcDeriv2,minR,maxR,Rexp.RadiusExp,1e-8,1e-8,10000);
    if (Pow6ClcDeriv2(Rexp.RadiusExp)>1e-4){
//        throw info_except(" Could not find RadExp for Alpha=%f Ra=%f\n",Al,Ra);
        Fzero(Pow6ClcDeriv2,minR,maxR,Rexp.RadiusExp,1e-8,1e-8,10000);
        cout<<" Could not find RadExp for Alpha= "<<Al<<" Ra= "<<Ra<<" zer?= "<<Pow6ClcDeriv2(Rexp.RadiusExp)<<"\n";
    }
    Clc_ArgExpMulExp(Rexp);
    return 1;
}
void Exp6Param::Clc_ArgExpMulExp(Exp6Param &Rexp){
    Rexp.Alpha=fabs(Rexp.Alpha);
    Rexp.Radius0 = fabs(Rexp.Radius0);
    Current = &Rexp;
    Rexp.ArgExp = Pow6ClcDeriv(Rexp.RadiusExp,Rexp)/Pow6Clc(Rexp.RadiusExp,Rexp);
    Rexp.MulExp = Pow6Clc(Rexp.RadiusExp,Rexp)/exp(Rexp.ArgExp*Rexp.RadiusExp);
}


// ==========================================================
// ============ IntExp6Correlation   ========================
// ==========================================================
//#define MAX_NIU 0.68
//#define MAX_NIU M_PI/(3*sqrt(2))





#define CE CalculateExp6Struct
Exp6Param *CalculateExp6Struct;
double DisDegree;
double IntegralInfinity=200;
// ==========================================================
// ============ Integals for Exp6    ========================
// ==========================================================


//  Int(sin(qx)*x*f(d*x)*dx,x0,inf)
//  f(x)=exp(-bx)
double IntExpInf(double b,double q,double x0)
{
	double abd=b,qx0=q*x0,abd2=sqr(abd),q2=sqr(q);
	return exp(-abd*x0)/sqr(abd2+q2)*
		(q*cos(qx0)*(x0*abd2+2*abd+q2*x0)+sin(qx0)*(q2*(abd*x0-1)+abd2*(abd*x0+1)));
}
//  Int(x^2*f(d*x)*dx,x0,inf)
//  f(x)=exp(-bx)
double IntExpInf1(double b,double x0)
{
//	double abd=b,abdx=abd*x0,abdx2=sqr(abdx),abd3=abd*abd*abd;
//	return exp(-abdx)*(abdx2+2+2*abdx)/abd3;
	double bx=b*x0,bx2=sqr(bx),b3=b*b*b;
	return exp(-bx)*(bx2+2+2*bx)/b3;
}
//
//double IntPow6Inf(double q,double x0)
//{
//	double qx0=q*x0,qx2=sqr(qx0),q4=pow(q,4),x4=pow(x0,4);
//	double SiQ=MTSinIntegral(qx0);
//	return ((cos(qx0)*qx0*(2-qx2)+sin(qx0)*(6-qx2))/x4+(0.5*M_PI-SiQ)*q4)/24;
//}
// ===================Strict ==================================
// Integral[-1/x^6*x*sin(qx),x]+Pi/2
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
    return (x*cos(x)+5*sin(x))/(sqr(x)*pow(x0,4));//-M_PI*q4/48
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
// ===================Strict ==================================

// ==========================================================
// ============ Integals for Exp6    ========================
// ==========================================================


double MainIntegral(double q)
{
 double X=CE->X,Cut=0,Ec0=CE->Ec0,Ec1=CE->Ec1,P6=CE->P6,E0=CE->E0,E1=CE->E1;
	if (X>1) Cut=(IntExpInf(Ec1,q,1)-IntExpInf(Ec1,q,X))*Ec0;
	else X=1;
	double ExpI=IntExpInf(E1,q,X)*E0;
	double PowI=0;if (P6!=0) PowI=IntPow6Inf_Strict(q,X)*P6;
	return (ExpI-PowI+Cut);
};
double IntExpInf_Cutted(double b,double q,double x0)
 {
	double qx0=q*x0,b2=sqr(b),q2=sqr(q);
	double ret=exp(-b*x0)*(1/sqr(b2+q2)*
		(q*cos(qx0)*(x0*b2+2*b+q2*x0)+sin(qx0)*(q2*(b*x0-1)+b2*(b*x0+1)))-cos(qx0)*x0/q);
// double sub=exp(-b*x0)*cos(qx0)*x0/q;
//cout<<ret<<" "<<sub<<"...........\n";
 return ret;
}
double IntPow6Inf_Strict_Cutted(double q,double x0)
 {
  return 5*sin(q*x0)/(sqr(q*x0)*pow(x0,4));
 }
double MainIntegral_Cutted(double q)
 {
 double X=CE->X,Cut=0,Ec0=CE->Ec0,Ec1=CE->Ec1,P6=CE->P6,E0=CE->E0,E1=CE->E1;
	if (X>1) Cut=(IntExpInf_Cutted(Ec1,q,1)-IntExpInf_Cutted(Ec1,q,X))*Ec0;
	else X=1;
	double ExpI=IntExpInf_Cutted(E1,q,X)*E0;
	double PowI=0;if (P6!=0) PowI=IntPow6Inf_Strict_Cutted(q,X)*P6;
	return (ExpI-PowI+Cut);
 }
// double d1=-(CE->Amp[0]*cos(CE->Freq[0]*q)+CE->Amp[1]*cos(CE->Freq[1]*q))/q;
double IntExpInf_Cut(double b,double x0){return -exp(-b*x0)*x0;}
double IntPow6Inf_Cut(double x0){return 1/pow(x0,5);}
double IntNC_Cut(){double n=CE->Niu;return 24*n*(1-1.5*n+0.5*pow(n,3))/pow(n-1,4);}
void MainIntegral_Cut()
{
 double X=CE->X,Cut=0,Ec0=CE->Ec0,Ec1=CE->Ec1,P6=CE->P6,E0=CE->E0,E1=CE->E1;
 X=(X<1)?1:X;
 CE->Amp[0]=0;CE->Freq[0]=1;CE->Amp[1]=0;CE->Freq[1]=X;//CE->Freq[3]=1;
	if (X>1) {CE->Amp[0]=IntExpInf_Cut(Ec1,1)*Ec0;CE->Amp[1]=-IntExpInf_Cut(Ec1,X)*Ec0;}
//	else X=1;
	CE->Amp[1]+=IntExpInf_Cut(E1,X)*E0;
	if (P6!=0) CE->Amp[1]+=IntPow6Inf_Cut(X)*P6;
 CE->Amp[2]=IntNC_Cut();
};

double PYfunc(double q)
{
   double niu=CE->Niu;
   double q2=sqr(q),sq=sin(q),cq=cos(q),niu2=sqr(niu-1);
   double nc=-1/(pow(q,6)*sqr(niu2))*24*niu*
             (sqr(2*niu+1)*(sq-q*cq)*q2*q+
              1.5*niu*sqr(niu+2)*((q2-2)*cq-2*q*sq+2)*q2-
              0.5*niu*sqr(2*niu+1)*((sqr(q2)-12*q2+24)*cos(q)-
              4*(q*(q2-6)*sq+6)));
   return (1/(1-nc)-1)*q;
};
//X_func DoubleIntegralFunc;
double DoubleIntegral(double q)
{
	double d=MainIntegral(q),r=PYfunc(q);
//	double d=DoubleIntegralFunc(q),r=PYfunc(q);
	double ret=d*r;
//ret=r;
// if (q>IntegralInfinity) ret*=sqr(IntegralInfinity/q);
// if (q>0.7*IntegralInfinity) ret*=exp(30*(0.7*IntegralInfinity/q-1));
//cout<<q<<" "<<ret<<"\n";
 return ret;
};
double DoubleIntegral_CutSum(double q)
{
	double d=MainIntegral(q),d_=MainIntegral_Cutted(q),r=PYfunc(q);//,r=NCfunc(q),c=CosIntegralVal(q);
 double r1=CE->Amp[2]/q*cos(q);
	double ret;//=d*r;
 ret=d_*r+d*(r-r1)+d_*(r-r1);
 return ret;
};
double EXP_SMOOTH_FACTOR=0.2;
double EXP_SMOOTH_FACTOR_START=20;
double DoubleIntegral_CutSumSmooth(double q)
{
	double d=MainIntegral(q),d_=MainIntegral_Cutted(q),r=PYfunc(q);//,r=NCfunc(q),c=CosIntegralVal(q);
 double r1=CE->Amp[2]/q*cos(q);
	double ret;//=d*r;
 ret=d_*r+d*(r-r1)+d_*(r-r1);
 ret*=exp(-EXP_SMOOTH_FACTOR*(q-EXP_SMOOTH_FACTOR_START));
 return ret;
};
// ===================================================================
// ================  New Block, added to devide integration  =========
// ===================================================================
// PY func, without up nc
double NCfunc(double q)
{
   double niu=CE->Niu;
   double q2=sqr(q),sq=sin(q),cq=cos(q),niu2=sqr(niu-1);
   double nc=-1/(pow(q,6)*sqr(niu2))*24*niu*
             (sqr(2*niu+1)*(sq-q*cq)*q2*q+
              1.5*niu*sqr(niu+2)*((q2-2)*cq-2*q*sq+2)*q2-
              0.5*niu*sqr(2*niu+1)*((sqr(q2)-12*q2+24)*cq-
              4*(q*(q2-6)*sq+6)));
   return q/(1-nc);
};
/*
#define EXP_PRE_FACTOR 1e-5
double SinIntegralVal(double q)
{
   double niu=CE->Niu;
   double q2=sqr(q),q5=sqr(q2)*q,niu3=sqr(niu)*niu,sq=1;//sin(q),;
//   sq=exp(-EXP_PRE_FACTOR*q);//*sin(q);
   double ret=sq*niu*(1152*niu*sqr(0.49999999999999967+niu)+q2*(-24+144*niu-120*niu3))/
              (q5*sqr(sqr(niu-1)));
   return ret;
}
double CosIntegralVal(double q)
{
   double niu=CE->Niu;
   double q2=sqr(q),q4=sqr(q2),cq=1;//cos(q)
//   cq=exp(-EXP_PRE_FACTOR*q);//*cos(q);
//   cq=1/q;
   double ret=cq*niu*(1152*niu*sqr(0.49999999999999967+niu)-
              504*q2*niu*(-0.32037724101704074+niu)*(0.89180581244561208+niu)+
              12*q4*sqr(-1+niu)*(2+niu))/(q4*q2*sqr(sqr(niu-1)));
//cout<<q<<" "<<ret<<"\n";
   return ret;
}
double ConIntegralVal(double q)
{
   double niu=CE->Niu;
   double q2=sqr(q),q6=sqr(q2)*q2,niu2=sqr(niu);
   double ret=niu2*(-1152*sqr(0.49999999999999967+niu)-
              72*q2*sqr(1.9999999999999987+niu))/(q6*sqr(sqr(niu-1)));
   return ret;
}

double DoubleSinIntegral(double q)
{
	double d=MainIntegral(q),r=NCfunc(q),s=SinIntegralVal(q);
	double ret=d*r*s;
//ret=r*s;
 return ret;
};
double DoubleCosIntegral(double q)
{
	double d=MainIntegral(q),r=NCfunc(q),c=CosIntegralVal(q);
	double ret=d*r*c;
//ret=r*c;
//cout<<q<<" "<<d<<" "<<r<<" "<<c<<" "<<ret<<"\n";
 return ret;
};
double DoubleCosIntegral_Cut(double q)
{
	double d=MainIntegral(q),r=NCfunc(q),c=CosIntegralVal(q);
 double d1=-(CE->Amp[0]*cos(CE->Freq[0]*q)+CE->Amp[1]*cos(CE->Freq[1]*q))/q;
 double d_=MainIntegral_Cutted(q);
 double r1=CE->Amp[2]/q;
	double ret=d*r*c;
 ret=(d-d1)*(r*c-r1);
//cout<<d-d1<<d_<<"\n";
//cout<<q<<" "<<d<<" "<<d1<<" "<<r*c<<" "<<r1<<" "<<c<<" "<<ret<<"\n";
 return ret;
};
double DoubleCosIntegral_CutSum(double q)
{
	double d=MainIntegral(q),r=NCfunc(q),c=CosIntegralVal(q);
 double d1=-(CE->Amp[0]*cos(CE->Freq[0]*q)+CE->Amp[1]*cos(CE->Freq[1]*q))/q;
 double r1=CE->Amp[2]/q;
	double ret=d*r*c;
// ret=(d-d1)*(r*c-r1);
//cout<<q<<" "<<d<<" "<<d1<<" "<<r*c<<" "<<r1<<" "<<c<<" "<<ret<<"\n";
// double deltaD=d-d1,deltaR=r*c-r1;
 ret=(d-d1)*r*c+d*(r*c-r1)+(d-d1)*(r*c-r1);
//ret*=exp(-q*EXP_PRE_FACTOR);
 return ret;
};
double DoubleCosIntegral_CutSum_Plus(double q)
 { return DoubleCosIntegral_CutSum(q)*cos(q);}

double DoubleConIntegral(double q)
{
	double d=MainIntegral(q),r=NCfunc(q),c=ConIntegralVal(q);
 double ret=d*r*c;
//ret=r*c;
 return ret;
};
*/
double IntCosQCosQX(double st,double x)
 {
  double xm=x-1,xp=x+1;
  return -M_PI*(sqr(xp)+fabs(sqr(x)-1))/(4*xp)+
          (cos(st*xm)+cos(st*xp)+st*(xm*MTSinIntegral(st*xm)+xp*MTSinIntegral(st*xp)))/(2*st);
 }
double AprocsIntegral(double From)
 {
  return -(IntCosQCosQX(From,CE->Freq[0])*CE->Amp[0]+IntCosQCosQX(From,CE->Freq[1])*CE->Amp[1])*CE->Amp[2];
 }
//double Qawf_FourierIntInf(X_func f,double From,double ErrorAbs,
//                  int NumSubInt,double &ResErr,int &Err,double Omega,int ToMulOnCos)

//#define ROS_INTEGRAL_STRICT
//#define ROS_INTEGRAL_SMOOTH
//#define ROS_INTEGRAL_NOTSMOOTH

double SmoothLimitUp=1e-6,SmoothLimitLo=1e-7;


double IntegralPYFunction(double From,double Err,int NumInt,double niu,int IntMethod)
 {
  double ResErr;
  int error;
  if (IntMethod==Exp6Param::ROS_INTEGRAL_STRICT)
   {
    double ret,Assim,start,oscil,aprocs,nevs,nevo;//,test
//    int nev;
    MainIntegral_Cut();
    double Assim0=7,Assim1=20,Assim_=60,Assim2=150,start1,start2;
    Assim=150;
//cout<<" start1\n";
//    start1=Qags_IntSubInt(DoubleIntegral,From,Assim0,Err,Err,NumInt,ResErr,error);
//    nevs=NumberEvaluations;
//    start1+=Qags_IntSubInt(DoubleIntegral,Assim0,Assim1,Err,Err,NumInt,ResErr,error);
    start1=Qags_IntSubInt(DoubleIntegral,From,Assim0,Err,Err,NumInt,ResErr,error);
    nevs=NumberEvaluations;
    start1+=Qags_IntSubInt(DoubleIntegral,Assim0,Assim1,Err,Err,NumInt,ResErr,error);
    nevs+=NumberEvaluations;
//cout<<" start2\n";
    start2=Qags_IntSubInt(DoubleIntegral_CutSum,Assim1,Assim_,Err,Err,NumInt,ResErr,error);
    nevo=NumberEvaluations;
    start2+=Qags_IntSubInt(DoubleIntegral_CutSum,Assim_,Assim2,Err,Err,NumInt,ResErr,error);
    start=start1+start2;
    nevo+=NumberEvaluations;
//    cout<<" NumberEvaluations "<<NumberEvaluations<<"\n";
//cout<<" oscil\n";
    oscil=Qainf_OscilIntInf(DoubleIntegral_CutSum,Assim2,250, Err,Err,8*NumInt,ResErr,error,2*M_PI,3,OscillMethod_Overholt);
//    nevo=NumberEvaluations;
    aprocs=AprocsIntegral(Assim2);
    ret=start+oscil+aprocs;
//cout<<" start1 "<<start1<<" start2 "<<start2<<" starttot "<<start+AprocsIntegral(Assim1)-AprocsIntegral(Assim2)<<" n "<<nevo<<" ntst "<<nevs<<"\n";
//cout<<" oscil "<<oscil<<" nev "<<NumberEvaluations<<" ret "<<ret<<"\n";
//cout<<" Assim "<<k<<" Nev "<<nev<<" res "<<ret<<"\n";
cout<<" nevs "<<NumberEvaluations+nevo+nevs<<"\n";
    return ret;
   }
  if (IntMethod==Exp6Param::ROS_INTEGRAL_SMOOTH)
   {
    double ret,start,aprocs;
    double Assim0=7,Assim1=20,Assim2=60,Assim3=150, Assim4=220, Assim5=350;
    EXP_SMOOTH_FACTOR=6/(Assim5-Assim3);EXP_SMOOTH_FACTOR_START=Assim3;
    int nevs;
    MainIntegral_Cut();
    start=Qags_IntSubInt(DoubleIntegral,From,Assim0,Err,Err,NumInt,ResErr,error); nevs=NumberEvaluations;
    start+=Qags_IntSubInt(DoubleIntegral,Assim0,Assim1,Err,Err,NumInt,ResErr,error); nevs+=NumberEvaluations;
    start+=Qags_IntSubInt(DoubleIntegral,Assim1,Assim2,Err,Err,NumInt,ResErr,error); nevs+=NumberEvaluations;
    start+=Qags_IntSubInt(DoubleIntegral,Assim2,Assim3,Err,Err,NumInt,ResErr,error); nevs+=NumberEvaluations;
    start+=Qags_IntSubInt(DoubleIntegral_CutSumSmooth,Assim3,Assim4,Err,Err,NumInt,ResErr,error); nevs+=NumberEvaluations;
    start+=Qags_IntSubInt(DoubleIntegral_CutSumSmooth,Assim4,Assim5,Err,Err,NumInt,ResErr,error); nevs+=NumberEvaluations;
    aprocs=AprocsIntegral(Assim3);
    ret=start+aprocs;
//cout<<" nevs "<<nevs<<"\n";
    return ret;
   }
//  if (IntMethod==Exp6Param::ROS_INTEGRAL_SMOOTH)
//   {
//    double ret,start,aprocs;
//    double Assim0=7,Assim1=20,Assim2=60,Assim3=150;
//    EXP_SMOOTH_FACTOR=6/(Assim3-Assim1);EXP_SMOOTH_FACTOR_START=Assim1;
//    int nevs;
//    MainIntegral_Cut();
//    start=Qags_IntSubInt(DoubleIntegral,From,Assim0,Err,Err,NumInt,ResErr,error); nevs=NumberEvaluations;
//    start+=Qags_IntSubInt(DoubleIntegral,Assim0,Assim1,Err,Err,NumInt,ResErr,error); nevs+=NumberEvaluations;
//    start+=Qags_IntSubInt(DoubleIntegral_CutSumSmooth,Assim1,Assim2,Err,Err,NumInt,ResErr,error); nevs+=NumberEvaluations;
//    start+=Qags_IntSubInt(DoubleIntegral_CutSumSmooth,Assim2,Assim3,Err,Err,NumInt,ResErr,error); nevs+=NumberEvaluations;
//    aprocs=AprocsIntegral(Assim1);
//    ret=start+aprocs;
////cout<<" nevs "<<nevs<<"\n";
//    return ret;
//   }
  if (IntMethod==Exp6Param::ROS_INTEGRAL_SMOOTH_PLUSNIU)
   {
//    if (CE->Niu<=SmoothLimitLo) return 0;
    double ret,start,aprocs;
    double Assim0=7,Assim1=20,Assim2=60,Assim3=150;
    EXP_SMOOTH_FACTOR=6/(Assim3-Assim1);EXP_SMOOTH_FACTOR_START=Assim1;
    int nevs;
    MainIntegral_Cut();
    start=Qags_IntSubInt(DoubleIntegral,From,Assim0,Err,Err,NumInt,ResErr,error); nevs=NumberEvaluations;
    start+=Qags_IntSubInt(DoubleIntegral,Assim0,Assim1,Err,Err,NumInt,ResErr,error); nevs+=NumberEvaluations;
    start+=Qags_IntSubInt(DoubleIntegral_CutSumSmooth,Assim1,Assim2,Err,Err,NumInt,ResErr,error); nevs+=NumberEvaluations;
    start+=Qags_IntSubInt(DoubleIntegral_CutSumSmooth,Assim2,Assim3,Err,Err,NumInt,ResErr,error); nevs+=NumberEvaluations;
    aprocs=AprocsIntegral(Assim1);
    ret=start+aprocs;
    if (CE->Niu<SmoothLimitUp) 
     {
//      double add=pow((SmoothLimitUp-CE->Niu)/(SmoothLimitUp-SmoothLimitLo),4)/pow(SmoothLimitUp,1);
      double add=10*pow(fabs((log(SmoothLimitUp)-log(CE->Niu))/(log(SmoothLimitUp)-log(SmoothLimitLo))),4)/SmoothLimitUp;
      ret+=add;
     }
//cout<<" nevs "<<nevs<<"\n";
    return ret;
   }
  if (IntMethod==Exp6Param::ROS_INTEGRAL_NOTSMOOTH)
   {
    double ret,start,aprocs;
    int nevs;
    MainIntegral_Cut();
    double Assim0=7,Assim1=20;
    start=Qags_IntSubInt(DoubleIntegral,From,Assim0,Err,Err,NumInt,ResErr,error); nevs=NumberEvaluations;
    start+=Qags_IntSubInt(DoubleIntegral,Assim0,Assim1,Err,Err,NumInt,ResErr,error); nevs+=NumberEvaluations;
    aprocs=AprocsIntegral(Assim1);
    ret=start+aprocs;
cout<<" nevs "<<nevs<<"\n";
    return ret;
//#endif
   }
   cout<<" IntegralPYFunction do not know how to integrate\n."<<flush;exit(0);
   return 0;

 }
// ==========================  End   =================================
// ================  New Block, added to devide integration  =========
// ===================================================================


double UnityIntegral()
{
 double X=CE->X,Cut=0,Ec0=CE->Ec0,Ec1=CE->Ec1,P6=CE->P6,E0=CE->E0,E1=CE->E1;
	if (X>1) Cut=(IntExpInf1(Ec1,1)-IntExpInf1(Ec1,X))*Ec0;
	else X=1;
	double ExpI=IntExpInf1(E1,X)*E0;
	double PowI=P6/(3*pow(X,3));
	return (ExpI-PowI+Cut);
};

// ==========================================================
// ============ Working Functions    ========================
// ==========================================================
#define OUTVAR(name) " "#name" "<<CE->name
void WriteCe(double niu,double Diameter)
 {
  cout<<"\n Exp6Par Pow6 "<<CE->IncludePow6<<" Diameter "<<Diameter<<" Niu "<<niu<<"\n";
//  cout<<OUTVAR(MulExp)<<OUTVAR(ArgExp)<<OUTVAR(P6)<<OUTVAR(Epsilon)<<OUTVAR(Alpha)<<OUTVAR(Radius0)<<"\n";
//  cout<<OUTVAR(Niu)<<OUTVAR(Ec0)<<OUTVAR(Ec1)<<OUTVAR(E0)<<OUTVAR(E1)<<OUTVAR(P6)<<OUTVAR(X)<<"\n";
 }

int IntExp6Correlation_IntMethod=Exp6Param::ROS_INTEGRAL_SMOOTH;
double IntExp6Correlation(Exp6Param &Par,double niu,double Diameter)
{
 CE=&Par;
 double d=Diameter,Epsilon=CE->Epsilon,Alpha=CE->Alpha,Radius=CE->Radius0;
 CE->Ec0=CE->MulExp;
 CE->Ec1=-CE->ArgExp*d;
 CE->E0=Epsilon/(Alpha-6)*6*exp(Alpha);
 CE->E1=d*Alpha/Radius;
// CE->P6 definition
 if (fabs(CE->IncludePow6)>MathZer)                                   // Addition for binary He-H2
   CE->P6=Epsilon/(Alpha-6)*Alpha*pow(Radius/d,6)*CE->IncludePow6;
 else CE->P6=0;
 CE->X=CE->RadiusExp/d;
 CE->Niu=niu;
// int Flag=0;
	double UInt=UnityIntegral(),DInt=0;
//	if (RP->X>1) 
//		{DoubleIntegralFunc=MainIntegralCut;
//		 DInt=RecInt(DoubleIntegral,0.1*StndErr,25,StndErr);}
//	else RP->X=1;
//	DoubleIntegralFunc=MainIntegralNoCut;
//double InFinQunc10(X_func Func,double A,double AbsErr,double InFinZero,
//                   int NumZero,long MaxFunc,int &Flag)
//	double ResErr;//int Err;
double Fr=0.1*StndErr;
Fr=1e-6;
Fr=1e-9;
//WriteCe(niu,Diameter);
 DInt=
//		InFinQulb9(DoubleIntegral,0.1*StndErr,StndErr,10*StndErr,10,10000,Flag);
//		RecInt(DoubleIntegral,0.1*StndErr,25,0.0001);  // Was 25 - upper limit. Too small.
//		RecInt(DoubleIntegral,0.1*StndErr,400,0.0001);
//    Qags_IntSubInt(DoubleIntegral,0.1*StndErr,2*IntegralInfinity,1e-6,1e-6,1000,ResErr,Err);

//    Qags_IntSubInt(DoubleIntegral,0.1*StndErr,IntegralInfinity,1e-6,1e-6,2000,ResErr,Err);
//    IntegralPYFunction(Fr,1e-6,2000,niu,ROS_INTEGRAL_STRICT);
//    IntegralPYFunction(Fr,1e-6,10000,niu,IntExp6Correlation_IntMethod);
    IntegralPYFunction(Fr,0.1*StndErr,10000,niu,IntExp6Correlation_IntMethod);
//double Test=Qags_IntSubInt(DoubleIntegral,0.1*StndErr,IntegralInfinity,1e-6,1e-6,2000,ResErr,Err);
//double Test=IntegralPYFunction(Fr,1e-6,2000,niu,ROS_INTEGRAL_NOTSMOOTH);
//cout<<" Test "<<Test<<" DInt "<<DInt<<"\n";
// if (Flag!=0) cout<<" Flag "<<Flag<<"\n";
	return UInt*12*niu+DInt/M_PI;
};


double HS_Correction(double niu)
{
   double niu2=sqr(niu);
//   double F_HS=(4*niu-3*niu2)/(1-niu2);
   double F_HS=(4*niu-3*niu2)/sqr(1-niu);
   double F_SS=-(sqr(niu2)*0.5+niu2+niu*0.5);
   return (F_HS+F_SS);
};
MinConfigParam *Ross6Min;
double Ross6MinFunc(double niu)
  {
   double F_Corr=HS_Correction(niu);
   double C=M_Na/1e24;
   double Diameter=pow(6*niu/(Ross6Min->S_DensDia*M_PI*C),1./3);//*2.238515;// To obtain S_Diameter in Bor radius
   double Int=IntExp6Correlation(*CE,niu,Diameter);
   double ret=(Ross6Min->S_Temp*F_Corr+Ross6Min->S_Rconst*Int);
//cout<<" niu "<<niu<<" ret "<<ret<<"\n";cout.flush();
   return ret;
  };
double Ross6Config(Exp6Param &Par6,double MolVeight,double Denc,double T)
{
   MinConfigParam MinPar;Ross6Min=&MinPar;CE=&Par6;
   MinPar.S_Temp=M_Rconst/MolVeight*T;
   MinPar.S_Rconst=M_Rconst/MolVeight;
   MinPar.S_DensDia=Denc/MolVeight;
   double Min_niu,Min_E;
   if ( (fabs(CE->L_Denc-Denc)<0.001*StndErr*Denc) && (fabs(CE->L_T-T)<0.5) )
	{return Ross6Min->S_Temp*HS_Correction(CE->L_Niu)+
//				Ross6Min->S_DensDia*M_Rconst*CE->L_Int;}
				Ross6Min->S_Rconst*CE->L_Int;}
   Min_niu=Fmin(Ross6MinFunc,StndErr,CE->CutDiameter_Niu,CE->L_Niu,StndErr,Min_E);
//   Min_niu=Fmin(Ross6MinFunc,StndErr,CE->CutDiameter_Niu,CE->L_Niu,StndErr,Min_E);
//   if (!NAGMin(Ross6MinFunc,StndErr,StndErr,StndErr,CE->CutDiameter_Niu,30,Min_niu,Min_E))
//      cout<<" Did not find Ross6MinFunc.\n";
//cout<<" ========================================================================\n";
//cout<<" Niu "<<Min_niu<<" E "<<Min_E<<"\n";
//cout<<" ========================================================================\n";
//int NAGMin(X_func F,double relat_err,double abs_err,double MinB,
//              double MaxB,int MaxCal,double &X_res,double &F_res)
//double Fmin(X_func f,double From,double To,double Guess,double AbsErr,double &FuncXVal)
   CE->L_Denc=Denc;CE->L_Niu=Min_niu;CE->L_T=T;
   CE->L_Int=IntExp6Correlation(*CE,Min_niu,
	   pow(6*Min_niu/(Ross6Min->S_DensDia*M_PI*M_Na/1e24),1./3));
//cout<<" Denc "<<Denc<<" T "<<T<<" D "<<pow(6*Min_niu/(Ross6Min->S_DensDia*M_PI*M_Na/1e24),1./3)<<"\n";
//cout<<" Denc "<<Denc<<" T "<<T<<" MinE "<<Min_E<<"\n";
//cout<<" Found Min "<<Min_E<<" niu "<<Min_niu<<" Denc "<<Denc<<" T "<<T<<"\n";cout.flush();
   return Min_E;
};
double DissEnergy(double E1,double E2,double MolVeight,double T)
{
 double kt=M_Rconst/MolVeight*T;
 double Q=(E2-E1)/kt,X;
 if (Q<-25) { DisDegree=0;return E2;}
 if (Q>25)  { DisDegree=1;return E1;}
 Q=exp(Q);X=sqrt(Q/(4+Q));
DisDegree=X;
//cout<<"\n T "<<T<<" X "<<X<<" E1 "<<E1<<" E2 "<<E2;
 double Xm=1-X,Xp=1+X;
//cout<<" DisE "<< X*E1+Xm*E2+kt*(Xm*log(Xm/Xp)+2*X*log(2*X/Xp))<<"\n";
 return X*E1+Xm*E2+kt*(Xm*log(Xm/Xp)+2*X*log(2*X/Xp));
};
double IonizeEnergy(double E1,double E2,double MolVeight,double T,
					double Mul_Degeneration,double V)
{
 double kt=M_Rconst*T/MolVeight;
 double Q=(E2-E1)/(2*kt),X;
 if (Q<-25) return E2;if (Q>25) return -2*kt+E2;
 Q=min(exp(Q),1);
 X=2*4.0097117e-9*sqrt(Mul_Degeneration)*Q*V*MolVeight*pow(T,1.5);
//cout<<" T "<<T<<" V "<<MolVeight*V<<" X "<<X
//    <<" Q "<<Q<<"\n";
// return (E2-E1)*X*kt*2+E2;
// return (E2-E1)*X*kt+E2;
//   return (E1-E2)*X+E2;
 return -X*2*kt+E2;
};
//=================================================================
// ========= Fermi ionize =========================================
//=================================================================

double S_EInterval,S_ClcT,S_WorkMiu,S_DegenGr,S_DegenUp,S_IntLim;
double IntFuncPartElectron(double e)
{
	double N=S_DegenGr*sqrt(max(0,-e))+
		     S_DegenUp*sqrt(max(0,e));
    if (e>=0) return N/(exp((e+S_EInterval-S_WorkMiu)/S_ClcT)+1);
	return N/(exp((fabs(e)+S_WorkMiu)/S_ClcT)+1);
};
double IntFuncPartElectron1(double e)
{
  double N=S_DegenGr*sqrt(max(0,-e))+S_DegenUp*sqrt(max(0,e));
  double E;
  if (e>0) E=((-e-S_EInterval+S_WorkMiu)/S_ClcT);
  else     E=((fabs(e)+S_WorkMiu)/S_ClcT);
  if (fabs(E)>25) return 0;E=exp(E);
  double res=(-S_ClcT*log(E+1)+S_WorkMiu*E/(E+1));
  if (e<0) res+=fabs(e);
  return res*N;
};
double FindFuncMiu(double miu)
{
	S_WorkMiu=miu;
	double ResErr;int Err;
 double res1=Qags_IntSubInt(IntFuncPartElectron,-S_IntLim,-MathZer,
                            StndErr*0.001,StndErr*0.001,1000,ResErr,Err);
 double res2=Qags_IntSubInt(IntFuncPartElectron,MathZer,S_IntLim,
                            StndErr*0.001,StndErr*0.001,1000,ResErr,Err);

//   	double res1= RecInt(IntFuncPartElectron,-S_IntLim,-MathZer,StndErr*0.001);
//    double res2= RecInt(IntFuncPartElectron,MathZer,S_IntLim,StndErr*0.001);
//cout<<" miu "<<miu<<" int "<<res1-res2<<" res1 "<<res1<<" res2 "<<res2<<"\n";
    return res1-res2;
};
// return DensOfFreeE to add
#include "mat\mtquad.h"
double FreeESemiConductor(double ZoneWidth,double LowDegener,double UpDegener, // in eV
						  double MolVeight,double Dens,double T)
{
//    double kt=M_Rconst/MolVeight*T;
	double V=MolVeight/Dens;
	S_EInterval=ZoneWidth;S_ClcT=T/M_eV_K;S_IntLim=fabs(T/M_eV_K*10)+fabs(ZoneWidth*10)+1;
	S_DegenGr=LowDegener;S_DegenUp=UpDegener;
	double Coef=1.13118829e-2*V,MiuEst;
	MiuEst=ZoneWidth/2;
    if (3<Fzero(FindFuncMiu,-500,500,MiuEst,StndErr*0.1,StndErr*0.1,300) )
//    if (!ZeroNewtonIt(FindFuncMiu,MiuEst,StndErr*0.1,300,500,-500,MiuEst*0.5,0.1))
     { cout <<" Error, could not find Miu in FreeESemiConductor miu="<<MiuEst<<" beg "<<ZoneWidth/2
	        <<" res "<<FindFuncMiu(MiuEst)<<"\n";}
	S_WorkMiu=MiuEst;
//  Quazi-Boltsman estimation
//  double res2= RecInt(IntFuncPartElectron,MathZer,S_IntLim,StndErr*0.01);
// 	return Coef*res2*T*2*M_Rconst/MolVeight;

	double res1=0,res2=0;//,res0,resa,resb;
	double ResErr;int Err;
// resa=Qags_IntSubInt(IntFuncPartElectron,-S_IntLim,-MathZer,
//                     StndErr*0.001,StndErr*0.001,1000,ResErr,Err);
// resb=Qags_IntSubInt(IntFuncPartElectron,MathZer,S_IntLim,
//                     StndErr*0.001,StndErr*0.001,1000,ResErr,Err);
 res1=Qagie_InfinInt(IntFuncPartElectron1,0,2,StndErr*0.01,StndErr*0.01,
                  1000,ResErr,Err);
// 	res1= RecInt(IntFuncPartElectron1,-S_IntLim,-MathZer,StndErr*0.01);
//  res2= RecInt(IntFuncPartElectron1,MathZer,S_IntLim,StndErr*0.01);
//cout<<" miu "<<MiuEst<<" int "<<res1+res2<<" res1 "<<res1<<" res2 "<<res2<<"\n";
//cout<<" miu "<<MiuEst<<" res "<<Coef*(res1+res2)*M_eV_K*M_Rconst/MolVeight<<"\n";

//  if (fabs(res0)>1e-3) cout<<res0<<" "<<(res1+res2)<<"\n";

	return Coef*(res1+res2)*M_eV_K*M_Rconst/MolVeight;

};
//int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
//                             double Up,double Down,double MaxStp,double BegStp)

//=================================================================
// ========= Fermi ionize =========================================
//=================================================================

double InteractFunc(double r)
  {
   if (r<CE->RadiusExp) return CE->MulExp*exp(CE->ArgExp*r);
   double Epsilon=CE->Epsilon;
   double Alpha  =CE->Alpha;
   double Radius =CE->Radius0;
//cout<<"  "<<r<<" "<<Epsilon/(Alpha-6)*(6*exp(Alpha*(1-r/Radius))-Alpha*pow(Radius/r,6))<<"\n";
   return Epsilon/(Alpha-6)*(6*exp(Alpha*(1-r/Radius))-Alpha*pow(Radius/r,6));
  }; 

int Exp6Read(istream &in,Exp6Param *&par)
{
 if (par==NULL)
     par=new Exp6Param;
 return Exp6Read(in,*par);
};
int Exp6Read(istream &in,Exp6Param &par){
    CE=&par;
    if (seek_eof(in)) return 0;
    char tmp[256];
    in>>tmp>>par.Epsilon>>tmp>>par.Alpha>>tmp>>par.Radius0
    >>tmp>>par.RadiusExp>>tmp>>par.CutDiameter_Niu
    >>tmp>>par.IncludePow6;
    if (par.RadiusExp<MathZer)
        Exp6Param::FindRexp(par);
    else
        Exp6Param::Clc_ArgExpMulExp(par);
    return 1;
};
void Exp6Save(ostream &out,Exp6Param &par){
 CE=&par;
 if (!out) return ;
 out<<" Epsilon "<<par.Epsilon<<" par.Alpha "<<par.Alpha <<" par.Radius0 "<<par.Radius0<<" par.RadiusExp "<<par.RadiusExp
	 <<" par.CutDiameter_Niu "<<par.CutDiameter_Niu <<" par.IncludePow6 "<<par.IncludePow6<<"\n";
};


//double M_Ev_K=10000;
double RossElectronN2_E(double T)
{
	double T0=T;
	T/=M_eV_K;
//	return 0;
	return -log(1+3*exp(-6.167/T)+3*exp(-7.351/T)+exp(-8.546/T))*T0*M_Rconst/28;
};
double RossElectronN_E(double T,int MetalE)
{
	double T0=T;
	T/=M_eV_K;
 if (MetalE) return -log(3.)*T0*M_Rconst/14;
	else return -log(4+10*exp(-2.38/T)+6*exp(-3.58/T))*T0*M_Rconst/14;
//	

};

double FreeRCheck(Exp6Param *par,double Denc,double T,double niu,double MolVeight,double &Niu_Res,double &F_Res)
  {
   MinConfigParam MinPar;Ross6Min=&MinPar;CE=par;
   MinPar.S_Temp=M_Rconst/MolVeight*T;
   MinPar.S_DensDia=Denc/MolVeight;
   MinPar.S_Rconst=M_Rconst/MolVeight;
   double C=M_Na/1e24;
   double Diameter=pow(6*niu/(Ross6Min->S_DensDia*M_PI*C),1./3);
   double Int=IntExp6Correlation(*CE,niu,Diameter);
//   double Min_niu,Min_E;
   Niu_Res=Fmin(Ross6MinFunc,0.1*StndErr,CE->CutDiameter_Niu,CE->L_Niu,0.1*StndErr,F_Res);
//   if (!NAGMin(Ross6MinFunc,0.1*StndErr,0.1*StndErr,0.1*StndErr,CE->CutDiameter_Niu,30,Niu_Res,F_Res))
//      cout<<" Did not find Ross6MinFunc.\n";
   Diameter=pow(6*Niu_Res/(Ross6Min->S_DensDia*M_PI*C),1./3);
   F_Res=IntExp6Correlation(*CE,Niu_Res,Diameter);
   return Int;
  }; 




/*
RossParam *CalculateRossParam;
#define RP CalculateRossParam

double InteractFunc(double r)
  {
   if (r<RP->ParInteract[0]) return RP->ParInteract[4]*exp(RP->ParInteract[5]*r);
   double Epsilon=RP->ParInteract[1];
   double Alpha  =RP->ParInteract[2];
   double Radius =RP->ParInteract[3];
//cout<<"  "<<r<<" "<<Epsilon/(Alpha-6)*(6*exp(Alpha*(1-r/Radius))-Alpha*pow(Radius/r,6))<<"\n";
   return Epsilon/(Alpha-6)*(6*exp(Alpha*(1-r/Radius))-Alpha*pow(Radius/r,6));
  }; 
double IntegPY(double r)
  {
   double niu=RP->S_Niu,rad=r/RP->S_Diameter,res;
   RP->PercYerv->Evaluate(&rad,&niu,&res,1);
//cout<<"  "<<r<<" "<<res<<"\n";res=0;
   return sqr(r)*InteractFunc(r)*(res+1);
  }; 

double PY_Min(double niu)
  {
   RP->S_Niu=niu;
//   S_Temp=Rconst/MolVeight*T; S_DensDia=Denc/MolVeight;
// To obtain S_Diameter in Bor radius
   RP->S_Diameter=pow(6*niu/(RP->S_DensDia*M_PI),1./3)*2.238515;
   double niu2=sqr(niu);
   double F_HS=(4*niu-3*niu2)/(1-niu2);
   double F_SS=-(sqr(niu2)*0.5+niu2+niu*0.5);
   int Err;
   double MaxR=RP->S_MaxR_PY*RP->S_Diameter;
   double Int=RecInt(IntegPY,RP->S_Diameter*(1+StndErr),MaxR,1e-3);
   double Epsilon=RP->ParInteract[1],Alpha=RP->ParInteract[2],Radius=RP->ParInteract[3];
   double X0=Alpha*(1-MaxR/Radius),C=Epsilon/(Alpha-6),B=pow(Radius,6)*Alpha;
//cout<<" PY_Min Int Mis "<<Int-InFinQulb9(IntegPY,S_Diameter*(1+StndErr),
//                              3e-4,1e-3,10,3000,Err);
      Int-=C*(6*Radius/pow(Alpha,3)*(sqr(Alpha*MaxR)+2*Alpha*Radius*MaxR+2*sqr(Radius))*exp(X0)
           +B/(3*pow(MaxR,3)));
// Addition to the integral - integral from MaxR to the infinity with PY=1
//cout<<" PY_Min Int Mis+ "<<Int-InFinQulb9(IntegPY,S_Diameter*(1+StndErr),
//                              3e-4,1e-3,10,3000,Err)<<"\n";
//cout<<" PY_Min Int "<<S_Temp*(F_HS+F_SS)+S_DensDia*Rconst*Int*0.5<<" Err "<<Err<<"\n";
      return RP->S_Temp*(F_HS+F_SS)+RP->S_DensDia*RP->Rconst*Int*0.5;
     }; 
   
double FreeSSVT(RossParam *par,double Denc,double T)
  {
   RP=par;
   if ((T<MathZer) || (Denc<MathZer)) 
     { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
   RP->S_Temp=RP->Rconst/RP->MolVeight*T;RP->S_DensDia=Denc/RP->MolVeight;
   RP->S_MaxR_PY=6;
   double PY_add;
   if ((fabs(RP->DL-Denc)<1e-2) && (fabs(RP->TL-T)<1)) ;
   else { RP->DL=Denc;RP->TL=T;
//    RP->NiuL=MinFunc(0.035,0.62,PY_Min,2e-3);}
   if (!NAGMin(PY_Min,2e-3,2e-3,0.035,0.7,1000,RP->NiuL,PY_add))
      cout<<" Did not find RP->NiuL. Try old version\n";}
   PY_add=PY_Min(RP->NiuL);
   double F=RP->Zero+PY_add-RP->S_Temp*
            (log(max(1/Denc,MathZer))+RP->CvId*log(T)+RP->HiT+
                 log(1-exp(-RP->Tvib/T)));
   return F;
  }; 

int FreeERossRead(fstream &in,RossParam *&par)
  {
//cout<<" FreeERoss reading\n";
   par=new RossParam;RP=par;
   char tmp[256];
   in>>tmp>>RP->CvId>>tmp>>RP->MolVeight>>tmp>>RP->Zero>>tmp>>RP->HiT>>tmp>>RP->Tvib;
   in>>tmp>>RP->ParInteract[1]>>tmp>>RP->ParInteract[2]>>tmp>>RP->ParInteract[3];
   in>>tmp>>RP->ParInteract[0];
   in>>tmp>>tmp;//spl_name=strdup(tmp);
   fstream sp(tmp,ios::in);
   int ret=1;
//cout<<" FreeERoss spline registering. Name: "<<tmp<<"\n";
   if (!(ret=SurfRegister(sp,RP->PercYerv))) 
       {cout<<" Could not register Percus. file: "<<tmp
            <<" :\nNow exititng\n";exit(0);}
cout<<" FreeERoss Entered. Coreleft "<<Coreleft()<<"\n";
//   S_ParInteract=&ParInteract[0];
   double x=RP->ParInteract[0]*(1+StndErr);
   double c=InteractFunc(x);
   double c1=InteractFunc(x*(1+StndErr));
   c1=(c1-c)/(x*StndErr);
   RP->ParInteract[4]=c/(exp(c1/c*x));
   RP->ParInteract[5]=c1/c;
   return 1;
  };
*/
