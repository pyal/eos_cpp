


//#include "matt_reg.h"
//
//#ifdef Matter_Register
//
////#ifdef MatterABu_Register
//const char * const MatterABu::name="MatterABu";
//const char * const MatterGas::name="MatterGas";
//const char * const MatterBornMaier::name="MatterBornMaier"; // Do not correct
//const char * const MatterMurnaganBerch::name=  "MatterMurnaganBerch";
//const char * const MatterZubarev::name=  "MatterZubarev";
//const char * const MatterVacuum::name="MatterVacuum";
//const char * const MatterStable::name="MatterStable";
//
////#endif
//#ifdef InternalPointsUrs  
//const char * const MatterSum::name="MatterSum";
//const char * const MatterBinodalSpinodal::name="MatterBinodalSpinodal";
//#endif
//
//
//#ifdef MatterH2Liq_Register
//const char * const MatterH2Liq::name="MatterH2Liq";
//const char * const MatterH2Sol::name="MatterH2Sol";
//#endif
//
//
//#ifdef MatterSpline_Register
//const char * const MatterSpl::name="MatterSpl";
//#endif
//
//#ifdef MatterHisch_Register
//const char * const MatterHisch::name="MatterHisch";
//const char * const MatterIVL::name="MatterIVL";
//const char * const MatterABuExpFit::name="MatterABuExpFit";
//#endif
//
//#ifdef FreeE_Register
//const char * const MatterFreeE::name="MatterFreeE";
//MatterFreeE* MatterFreeE::Current=NULL;
//#endif
//
//#ifdef MatterABuExp_Register
//const char * const MatterABuAdd::name="MatterABuAdd";
//const char * const MatterABuExp::name="MatterABuExp";
//#endif
//
//
//int MatterRegister(istream &in,MatterIO *&Func)
//  {
//   char name[256];in>>name;
//
////#ifdef MatterABu_Register
//   if (Stricmp(name,"MatterABu")==0) Func=new MatterABu;
//   else
//		   if (Stricmp(name,"MatterGas")==0) Func=new MatterGas;
//		   else
//		   if (Stricmp(name,"MatterBornMaier")==0) Func=new MatterBornMaier;
//		   else
//		   if (Stricmp(name,"MatterZubarev")==0) Func=new MatterZubarev;
//		   else
//		   if (Stricmp(name,"MatterMurnaganBerch")==0) Func=new MatterMurnaganBerch;
//		   else
//		   if (Stricmp(name,"MatterVacuum")==0) Func=new MatterVacuum;
//		   else
//		   if (Stricmp(name,"MatterStable")==0) Func=new MatterStable;
//		   else
//
////#endif
//#ifdef MatterH2Liq_Register
//   if (Stricmp(name,"MatterH2Liq")==0) Func=new MatterH2Liq;
//   else
//   if (Stricmp(name,"MatterH2Sol")==0) Func=new MatterH2Sol;
//   else
//#endif
//#ifdef MatterHisch_Register
//   if (Stricmp(name,"MatterHisch")==0) Func=new MatterHisch;
//		   else
//     if (Stricmp(name,"MatterIVL")==0) Func=new MatterIVL;
//		   else
//     if (Stricmp(name,"MatterABuExpFit")==0) Func=new MatterABuExpFit;
//   else
//#endif
//#ifdef MatterSpline_Register
//   if (Stricmp(name,"MatterSpl")==0) Func=new MatterSpl;
//   else
//#endif
//#ifdef FreeE_Register
//   if (Stricmp(name,"MatterFreeE")==0) Func=new MatterFreeE;
//   else
//#endif
//#ifdef InternalPointsUrs 
//   if (Stricmp(name,"MatterSum")==0) Func=new MatterSum;
//   else
//   if (Stricmp(name,"MatterBinodalSpinodal")==0) Func=new MatterBinodalSpinodal;
//   else
//
//#endif
//#ifdef MatterABuExp_Register
//     if (Stricmp(name,"MatterABuAdd")==0) Func=new MatterABuAdd;
//		   else
//     if (Stricmp(name,"MatterABuExp")==0) Func=new MatterABuExp;
//		   else
//#endif
//    { cout<<" MatterRegister; There is no such function \n"<<name<<"\n";return 0; }
//return Func->ReadIni(in);
////   Func->ReadIni(in);
////   return 1;
//  };
//
//int MatterStore(ostream &out,MatterIO *Func)
//  {
////   InterfaceIO *F=(InterfaceIO*)&Func;
////   out<<Func->InterfaceIOName()<<"  ";
////cout<<" SaveIni core "<<coreleft()<<"  "<<Func<<" \n";
////cout<<Func->InterfaceIOName()<<" Presskey \n ";
////ChRead();
//  if (!Func)
//      return 0;
//   out<<Func->InterfaceIOName()<<"  ";
////cout<<Func->InterfaceIOName()<<" \n ";
//   Func->SaveIni(out);
//   return 1;
//  };
//
//#endif    //Matter_Register
//
//#ifdef FreeE_Register
//
//const char * const FreeESumMatter::name="FreeESumMatter";
//
//
//const char * const FreeEElectron::name="FreeEElectron";
//const char * const FreeEIdeal::name="FreeEIdeal";
//const char * const FreeEVanDerVals::name="FreeEVanDerVals";
//const char * const FreeEVirial::name="FreeEVirial";
//const char * const FreeEH2Liquid::name="FreeEH2Liquid";
//const char * const FreeEH2Solid::name="FreeEH2Solid";
//const char * const FreeEMetHCold::name="FreeEMetHCold";
//const char * const FreeEEmpiSolid::name="FreeEEmpiSolid";
//const char * const FreeEEmpiLiquid::name="FreeEEmpiLiquid";
//const char * const FreeEEmpiLiquidPres::name="FreeEEmpiLiquidPres";
//
//
//#ifdef MatterUrl_Register
//const char * const FreeEUrlLiquid::name="FreeEUrlLiquid";
//const char * const FreeEUrlSolid::name="FreeEUrlSolid";
//#endif
//
//
//
//int FreeERegister(istream &in,InterfaceFreeEIO *&Func)
//  {
//   char name[256];in>>name;
//
//   if (Stricmp(name,"FreeEVanDerVals")==0) Func=new FreeEVanDerVals;
//   else
//   if (Stricmp(name,"FreeEIdeal")==0) Func=new FreeEIdeal;
//   else
//   if (Stricmp(name,"FreeEVirial")==0) Func=new FreeEVirial;
//   else
//   if (Stricmp(name,"FreeESumMatter")==0) Func=new FreeESumMatter;
//   else
//   if (Stricmp(name,"FreeEElectron")==0) Func=new FreeEElectron;
//   else
//		   if (Stricmp(name,"FreeEH2Liquid")==0) Func=new FreeEH2Liquid;
//		   else
//		   if (Stricmp(name,"FreeEH2Solid")==0) Func=new FreeEH2Solid;
//		   else
//#ifdef MatterUrl_Register  
//     if (Stricmp(name,"FreeEUrlLiquid")==0) Func=new FreeEUrlLiquid;
//		   else
//     if (Stricmp(name,"FreeEUrlSolid")==0) Func=new FreeEUrlSolid;
//		   else
//#endif
//     if (Stricmp(name,"FreeEMetHCold")==0) Func=new FreeEMetHCold;
//		   else
//     if (Stricmp(name,"FreeEEmpiLiquid")==0) Func=new FreeEEmpiLiquid;
//		   else
//     if (Stricmp(name,"FreeEEmpiSolid")==0) Func=new FreeEEmpiSolid;
//		   else
//     if (Stricmp(name,"FreeEEmpiLiquidPres")==0) Func=new FreeEEmpiLiquidPres;
//		   else
//#ifdef FreeE_BushmanRegister
//		   if (Stricmp(name,"FreeEBushman")==0) Func=new FreeEBushman;
//		   else
//#endif
//
//    { cout<<" FreeERegister; There is no such function \n"<<name<<"\n";exit(0);}//return 0; }
//   Func->ReadIni(in);
//   return 1;
//  };
//
//int FreeEStore(ostream &out,InterfaceFreeEIO *&Func)
//  {
//   out<<Func->InterfaceIOName()<<"  ";
//   Func->SaveIni(out);
//   return 1;
//  };
//
//#endif    //FreeE_Register
//  























//#ifdef FreeE_BushmanRegister
//const char * const FreeEBushman::name="FreeEBushman";
//#endif
//#ifdef FreeE_RossRegister
//const char * const FreeERossH2::name="FreeERossH2";
//const char * const FreeERossN2::name="FreeERossN2";
//const char * const FreeERossXe::name="FreeERossXe";
//const char * const FreeERossBin::name="FreeERossBin";
//const char * const FreeERossH2_DisLim0::name="FreeERossH2_DisLim0";
//const char * const FreeERossH2_Minim::name1="FreeERossH2_Minim";
//
//
//#endif
//#ifdef FreeE_RossRegister
//   if (Stricmp(name,"FreeERossH2")==0) Func=new FreeERossH2;
//   else
//   if (Stricmp(name,"FreeERossN2")==0) Func=new FreeERossN2;
//   else
//   if (Stricmp(name,"FreeERossXe")==0) Func=new FreeERossXe;
//   else
//   if (Stricmp(name,"FreeERossBin")==0) Func=new FreeERossBin;
//   else
//   if (Stricmp(name,"FreeERossH2_DisLim0")==0) Func=new FreeERossH2_DisLim0;
//   else
//   if (Stricmp(name,"FreeERossH2_Minim")==0) Func=new FreeERossH2_Minim;
//   else
//
//#endif
