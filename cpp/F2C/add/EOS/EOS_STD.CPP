#include "eos_std.h"
double StndErr=1e-5;

double max(double a,double b) {if (a>b) return a;else return b;};
double min(double a,double b) {if (a<b) return a;else return b;};
double sqr(double a){return a*=a;};

int ReadStd(FILE *hand,void **IntPar,int NumPar,int Skip)
  {
   char tmp[256],NameSubst[256],NameFile[256],tmp1[256];
   fscanf(hand,"%s %s %s %s ",&tmp,&NameSubst,&tmp1,&NameFile);//   hand>>tmp>>NameSubst>>tmp>>NameFile;
   FILE *hand1;
   if( (hand1  = fopen( NameFile, "r" )) == NULL ) {printf( "The file %s was not opened\n",NameFile );return 0;}
   int ret=0;
   while ((!feof(hand1)) && (!ret))
     {
      fscanf(hand1,"%s ",&tmp);
      if (stricmp(tmp,NameSubst)!=0) fgets(tmp,256,hand1);
      else 
		{
		 double *d=new double[NumPar+1];
		 for (int k=1;k<=Skip;k++) fscanf(hand1,"%s ",&tmp);
		 for ( k=1;k<=NumPar;k++) fscanf(hand1,"%s %lf ",&tmp,&d[k]);
		 *IntPar=d;
		 ret=1;}
     }
   fclose(hand1);
   return ret;
  };

double SoundStd(double Dencity,double Energy,void *IntPar,PointFunc Pressure)
  {
   double dr=Dencity*StndErr,r=Dencity;
   if (r<MathZer) return 0;/*{ cout<<"MatterIO::Sound; Dencity<MathZer :"<<Dencity<<"\n";return 0;}*/
   double P=Pressure(r,Energy,IntPar);
   double Pp=Pressure(r+dr,Energy+P*dr/(r*r),IntPar);Pp-=P;
   if (Pp<0) {     }
   double S=sqrt( max(0,Pp)/dr );
   return S;
  };
/*
// ========================================================
// ================= ABu  EOS   ===========================
// ========================================================
*/
double TemperatureABu(double Dencity,double Energy,void *IntPar)
  {
/*
//   double P=PressureABu(Dencity,Energy,IntPar),*d=(double *)IntPar;
//   double ret=0.5*P*(1/d[1]-1/Dencity);
//   return ret;
*/
	return Energy;
  };

double PressureABu(double Dencity,double Energy,void *IntPar) // P(x,e)
  {
   if (Dencity<MathZer) return 0;
/*			  {cout<<" Dencity is negative. double MatterABu::Pressure.";return 0;}*/
   double *d=(double *)IntPar;
   double Denc=d[1],A=d[2],B=d[3],Grun=d[4];
   double X=1-Denc/Dencity;
/*   if (1-B*X<MathZer) return MathInf;*/
   double Ph=Denc*sqr(A)*X/sqr(1-B*X);
   double Eh=0.5*Ph*X/Denc;
/* Grun0*Ro0= Grun*Ro; (dP/dE)v=Grun*Ro*/
   return Ph+Grun*Denc*(Energy-Eh);  /* Grun0*Ro0= Grun*Ro; (dP/dE)v=Grun*Ro*/
  };
 
double SoundABu(double Dencity,double Energy,void *IntPar)
{return SoundStd(Dencity,Energy,IntPar,PressureABu);};
void DeleteABu(void **IntPar) { delete *IntPar;*IntPar=NULL;};

int ReadABu(FILE *hand,void **IntPar)
  {
/*                   Dens0 A B Grun*/
   if (!ReadStd(hand,IntPar,4,1)) return 0;
   else return 1;
  }; 

/*
// ========================================================
// ================= Gas  EOS   ===========================
// ========================================================
*/
double TemperatureGas(double Dencity,double Energy,void *IntPar){	return Energy;  };
double PressureGas(double Dencity,double Energy,void *IntPar) 
			  {return ( ((double*)IntPar)[1]-1)*Dencity*Energy;}
double SoundGas(double Dencity,double Energy,void *IntPar)
{return SoundStd(Dencity,Energy,IntPar,PressureGas);};
void DeleteGas(void **IntPar) { delete *IntPar;*IntPar=NULL;};
int ReadGas(FILE *hand,void **IntPar)
  {
/*                   Gamma*/
   if (!ReadStd(hand,IntPar,1,1)) return 0;
   else return 1;
  }; 

