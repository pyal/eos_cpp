// Configuration dat
// Pos Denc temp Opac_Coef Shock_Pos
// Cfg
// NumFrom NumTo NumPnt
// Entered struct

#include "util.h"
#include "matrics.h"
#include "anal.h"

double EmisTemp,NormDens;
int Sub;
int  NumCall,InvertDirection;
//EmisTemp=1e5;
/*        Old Var
double DoIt(MatrCl &GotData,istream &shock)
  {
//   MatrCl GotData;
   double ShockPos;char tmp[50];shock>>tmp;ShockPos=atoi(&tmp[3])-Sub;
//cout<<ShockPos<<"\n";
   for (int k=1;k<GotData.DimX;k++)
      GotData(k,2)=fabs((GotData(k+1,1)-GotData(k,1)))*pow(GotData(k,2),1./3);
   k=GotData.DimX;
   GotData(k,2)=fabs((GotData(k,1)-GotData(k-1,1)))*pow(GotData(k,2),1./3);
// tab(k,2) = L*Dens^1/3
   NumCall++;
   double Sum=0;
   for (k=1;((k<=GotData.DimX) && (k<=ShockPos) );k++)
     {
      GotData(k,2)=GotData(k,4)*GotData(k,2)/NormDens;
      Sum=Sum*exp(-GotData(k,2))+
           (1-exp(-GotData(k,2)))/(exp(EmisTemp/GotData(k,3))-1);
// Int(T,l)=k/l^5/(exp(C/lT)-1)
// C =1.436 cm*K
	  }
   return Sum;
  }; 
*/

double OptLengthCoef,CrossSection;
double EmisLength;
double ZeroElectron;
double DoIt(MatrCl &GotData,istream &shock)
  {
   double ShockPos;char tmp[50];shock>>tmp;ShockPos=atoi(&tmp[3])-Sub;
   double niu=M_C/(EmisLength*1e-7);
   double MainCoef=M_Na/(28*M_PI)*M_C*M_ElectrInter_Div_MassElectron*0.1;
   // 0.1 - to obtain Opasity in 1/mm
//   double vel_coef=M_C*0.1*M_Na/28*1e-16;// in cm/c
   double vel_coef=sqrt(2/(M_eV_K*M_Mass_Electron_eV))*M_C*M_Na/28*1e-16;// in cm/c
//cout<<ShockPos<<"\n";
// tab1 - Pos in mm  Dens  Temp  Diss 
// res     -        OptDens Temp Diss
cout<<"\n"<<" MainCoef "<<MainCoef<<" vel_coef "<<vel_coef<<"\n";
   for (int k=1;k<GotData.DimX;k++)
    {
     double Dens=GotData(k,2),El=GotData(k,4);
//     double q=CrossSection*vel_coef*GotData(k,2)*M_Na/28*1e-16;
     double q=CrossSection*vel_coef*sqrt(GotData(k,3))*GotData(k,2);

     GotData(k,2)*=q*(fabs(GotData(k,4))+ZeroElectron)*MainCoef/sqr(niu);//+sqr(q));
if (int((k-5)/30)==(k-5)/30.) {cout<<q<<"   "<<niu<<"   "<<GotData(k,2)<<"  "<<Dens<<"  "<<El<<"\n";}
    }
   for ( k=1;k<GotData.DimX;k++)
      GotData(k,2)*=fabs((GotData(k+1,1)-GotData(k,1)));
   k=GotData.DimX;
   GotData(k,2)*=fabs((GotData(k,1)-GotData(k-1,1)));
// tab res 
   NumCall++;
   double Sum=0;
   for (k=1;((k<=GotData.DimX) && (k<=ShockPos) );k++)
     {
      Sum=Sum*exp(-GotData(k,2))+
           (1-exp(-GotData(k,2)))/(exp(EmisTemp/GotData(k,3))-1);
// Int(T,l)=k/l^5/(exp(C/lT)-1)
// C =1.436 cm*K
	  }
   return Sum;
  }; 
/*          // Old Var II
double DoIt(MatrCl &GotData,istream &shock)
  {
//   MatrCl GotData;
   double ShockPos;char tmp[50];shock>>tmp;ShockPos=atoi(&tmp[3])-Sub;
   double MainCoef=2*sqr(1.6)*1e-11/(9.11*3)*M_Na/28/10*9e9*OptLengthCoef;
   double vel_coef=sqrt(1/(M_eV_K*5.11e5))*3e10;// in cm/c
   double niu=3e8/(EmisLength*1e-9);
//cout<<ShockPos<<"\n";
// tab1 - Pos in mm  Dens  Temp  Diss 
// res     -        OptDens Temp Diss
cout<<"\n";
   for (int k=1;k<GotData.DimX;k++)
    {
//     double q=CrossSection*sqrt(GotData(k,3))*vel_coef*pow(GotData(k,2)*M_Na/28,1./3);
//     double q=CrossSection*sqrt(3e4)*vel_coef*pow(GotData(k,2)*M_Na/28,1./3);
     double Dens=GotData(k,2),El=GotData(k,4);
     double q=sqr(CrossSection)*sqrt(GotData(k,3))*vel_coef*GotData(k,2)*M_Na/28*M_PI*1e-16;
//     double q=sqr(CrossSection)*sqrt(GotData(k,3))*vel_coef*pow(GotData(k,2),ZeroElectron)*M_Na/28*M_PI*1e-16;

     GotData(k,2)*=q/(sqr(niu)+sqr(q))*(fabs(GotData(k,4))+ZeroElectron)*MainCoef;
//     GotData(k,2)*=q/(sqr(niu)+sqr(q))*fabs(GotData(k,4))*MainCoef*pow(GotData(k,2),ZeroElectron);
if (int((k-5)/30)==(k-5)/30.) {cout<<q<<"   "<<niu<<"   "<<GotData(k,2)<<"  "<<Dens<<"  "<<El<<"\n";}
//     GotData(k,2)=ZeroElectron;
    }
   for ( k=1;k<GotData.DimX;k++)
      GotData(k,2)*=fabs((GotData(k+1,1)-GotData(k,1)));
   k=GotData.DimX;
   GotData(k,2)*=fabs((GotData(k,1)-GotData(k-1,1)));
// tab res 
   NumCall++;
   double Sum=0;
   for (k=1;((k<=GotData.DimX) && (k<=ShockPos) );k++)
     {
      Sum=Sum*exp(-GotData(k,2))+
           (1-exp(-GotData(k,2)))/(exp(EmisTemp/GotData(k,3))-1);
// Int(T,l)=k/l^5/(exp(C/lT)-1)
// C =1.436 cm*K
	  }
   return Sum;
  }; 
*/


double FindTemp(double Int)
  { if (Int<MathZer) return 0;return EmisTemp/(log(1/Int+1));};

int ClcOneIter(ifstream &in,ofstream &out,int NumFrom,int NumTo,int NumPnt,istream &shock)
  {
   MatrCl dat(NumTo-NumFrom+1,4);
   VecCl vec(4);
   int GetCol;
   char tmp[256];
   for (int k=1;k<=NumPnt;k++)
     {
      if (seek_eof(in)) return 0;
//      if ((GetCol=EnterStr(in,vec.Ptr,4))==0) {k--;continue;}
      if ((k<=NumTo) && (k>=NumFrom)) 
		{
		  if ((GetCol=EnterStr(in,vec.Ptr,4))==0) {k--;continue;}
		  if (!InvertDirection ) MatrCl::SetRow(dat,k-NumFrom+1,vec);
	      else MatrCl::SetRow(dat,NumTo+1-k,vec);
		}
	  else in.getline(tmp,256);
     }; 
//cout<<dat<<"\n";
   double Int=DoIt(dat,shock);
   out<<FindTemp(Int)<<"\n";
   return 1;
  }; 
void main( int argc, char *argv[] )
  {
   if (argc<=2) 
     {cout<<" use :  \nopacity dat_file out_file icfg_file N_shock_file [options]\n"<<
//            "icfg :   EmisLength_nm 700 NormDens 0.9 NumPnt 600 NumFrom 250 NumTo 500 Sub 0 InvertDir 0\n"<<
            "icfg :   EmisLength_nm 700 OptLengthCoef 0.01 CrossSection 1  ZeroElectron  0  NumPnt 600 NumFrom 250 NumTo 500 Sub 0 InvertDir 0\n"<<
            "dat  :      Pos Denc temp Opac_Coef \n"<<
		      "       (use for calculation in the input only first row)\n"<<
                      "   /n - no header information\n"<<
                      "   /c - number of columns to output\n";exit(1);}
      ifstream in(argv[1],ios::in);
      ifstream icfg(argv[3],ios::in);
      ifstream ish(argv[4],ios::in);
	  char tmp[256];
	  int NumFrom,NumTo,NumPnt;
//               nm
//	  icfg>>tmp>>EmisLength>>tmp>>NormDens>>tmp>>NumPnt>>tmp>>NumFrom>>tmp>>NumTo>>tmp>>Sub>>tmp>>InvertDirection;
	  icfg>>tmp>>EmisLength>>tmp>>OptLengthCoef>>tmp>>CrossSection>>tmp>>ZeroElectron>>tmp>>NumPnt>>tmp>>NumFrom>>tmp>>NumTo>>tmp>>Sub>>tmp>>InvertDirection;
// Int(T,l)=k/l^5/(exp(C/lT)-1)
// C =1.436 cm*K
   EmisTemp=1.436e7/EmisLength;
	  Sub+=NumFrom;
	  NumCall=0;
//	  EmisTemp=1e5;NormalDens=5;
      ofstream out(argv[2],ios::out);
      do
        ClcOneIter(in,out,NumFrom,NumTo,NumPnt,ish);
      while ( !seek_eof(in) );
      out.close();
  };
