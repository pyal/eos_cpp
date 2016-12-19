#include <lib/precompiled/math.h>
#include "mtfunc.h"
#include "quad.h"

double MTSign(double x)
{
     if (x>0) return 1;
     if (x<0) return -1;
     return 0;
}
double MTSinIntegral(double x)
{
     double pib2 =1.57079632679489662;
     double xhi = 1e19;
     double r__1;
     double f, g, t, u, y;

     u = fabs(x);
     if (u > 16.) goto L10;

/*     SMALL X - ARGUMENT EVALUATION */
     t = x * x * .0078125 - 1.;

/*     EXPANSION (0017) EVALUATED AS Y(T)  --PRECISION 18E */
    y = t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (
     t * (t * (t * (t * (t * (t * (t * (t * 
     1.13364013523612467e-12 - 1.73295770867796541e-11) + 
     2.34653547501289734e-10) - 2.9387085702927766e-9) + 
     3.30969553150126728e-8) - 3.32784295002924013e-7) + 
     2.96606267125684584e-6) - 2.32323765441590923e-5) + 
     1.58305204989030543e-4) - 9.27069661302843286e-4) + 
     .00459705459778527538) - .0189410064147072063) +
     .0632412080713534156) - .165103602060387617) + 
     .31828183819586503) - .40419356081893966) + 
     .229433945244285525) + .161990304098876091) - 
     .334271212963463397) + .0886834755381626329) + 
     .132473585260859799) - .11050677668138556) + 
	    .137060472876350089;
    return x*y;

/*     LARGE X - ARGUMENT EVALUATION */
L10:
    if (u >= xhi) goto L20;
    y = x * x;
    t = 512. / y - 1.;
/*     EXPANSION (0019) EVALUATED AS F(T)  --PRECISION 18E */
    f = t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (
     t * (t * 4.8699066372784128e-14 - 
     1.09176921093439488e-13) + 6.3498988357705728e-14) - 
     2.39368626222505984e-13) + 1.15517351776520397e-12) 
     - 3.55518881942203699e-12) + 
     1.17023188129910753e-11) - 4.39951217228505001e-11) + 
     1.84001853732121956e-10) - 8.7433384997443117e-10) + 
     4.87245424704650801e-9) - 3.32013637685389532e-8) + 
     2.93937999539516224e-7) - 3.71218824655850721e-6) + 
     7.82215041547366874e-5) - .00373729126869038934) + 
     .99618044190788651;
/*     EXPANSION (0020) EVALUATED AS G(T)  --PRECISION 18E */
    g = t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (t * (
     t * (t * (t * (t * 2.43669269913534464e-13 - 
     4.81013570050588672e-13) - 1.06265802930487296e-13) - 
     9.021839656198144e-14) + 2.93520978929726259e-12) - 
     7.18307892366012006e-12) + 1.70687678062319268e-11) 
     - 5.22266851938134641e-11) + 
     1.71515979253039184e-10) - 6.02557598370451512e-10) + 
     2.33450259515872927e-9) - 1.01708486037094415e-8) + 
     5.11015920578187682e-8) - 3.06745558439750417e-7) + 
     2.31342831851492447e-6) - 2.36338137299290861e-5) + 
     3.68834391295988884e-4) - .0108989877894521955) + 
     .98870585937050572;
    r__1 = pib2 * .5 - f * cos(u) / u - g * sin(u) / y + pib2 * .5;
    return MTSign(x)*r__1;
L20:
    return MTSign(x)*pib2;

} /* s13ade_ */

double DebyFuncInt(double x) {if (x==0) return 0;return x*x*x/(exp(x)-1);};
double DebyFunc(double x) 
  {
   double ResMis;int Err;
   return 3/(x*x*x)*Qags_IntSubInt(DebyFuncInt,0,x,StndErr,StndErr,1000,ResMis,Err);};
//  RecInt(DebyFuncInt,0,x,StndErr);};

/*
double IntPow6Inf(double a,double b,double d,double q,double x0)
{
	double qx0=q*x0,qx2=qx0*qx0,q4=pow(q,4),x4=pow(x0,4),d6=pow(d,6);
 double SiQ=MTSinIntegral(x0*q);
	return a/(d6*24)*((cos(qx0)*qx0*(2-qx2)+sin(qx0)*(6-qx2))/x4+(0.5*M_PI-SiQ)*q4);
}

void main()
{
 for (int k=1;k<=10000;k++)
   cout<<k<<" "<<IntPow6Inf(1,1,1,k*0.1,0.5)<<"\n";
};   
*/
