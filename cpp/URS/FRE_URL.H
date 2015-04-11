#ifndef __FRE_URL_H
#define __FRE_URL_H 
#include "fre_fac.h"
#include "url_equ.h"

struct FreeEUrlLiquid:FreeEIOFind{
   UrlParamStruct Par;
   FreeEUrlLiquid();//{ };
   double FreeE(double Denc,double T) 
     {return FUrlLiquid( Denc, T,&Par);  };
   void ReadSubstancePar(istream &in) {UrlReadPar(in,&Par);} 
   void SaveSubstancePar(ostream &out){UrlSavePar(out,&Par);}
  }; 
struct FreeEUrlSolid:FreeEIOFind{
   UrlParamStruct Par;

   FreeEUrlSolid():FreeEIOFind("h2_param.cfg","no_help"){ };
   double FreeE(double Denc,double T) {return FUrlSolid(Denc, T,&Par); };
   void ReadSubstancePar(istream &in) {UrlReadPar(in,&Par);} 
   void SaveSubstancePar(ostream &out){UrlSavePar(out,&Par);}
  }; 
#endif
