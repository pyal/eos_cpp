OLD TO REMOVE

#include <lib/precompiled/madiex.h>

#include "matt_adi.h"
#include "matt_h2.h"
//#include <alloc.h>





double StndErr;
MatterIO *Matter;
extern unsigned _stklen=64000;
#ifdef WCPP
//#include <afx.h>
#endif
//extern int NumHugIteration=5;
extern void OutputExamp();
void main(int argc,char *argv[])
  {
	  try{
   if (argc<2) {OutputExamp();exit(0);}
  SetLeakTest();

   Time_struct Time;
   fstream in(argv[1],ios::in);if ((!in) || (seek_eof(in))) { cout<<" no such cfg file "<<argv[1]<<"\n";exit(0);}
   char tmp[512];
   double u2=0,r2,p2=0,e2=0,D=0,porous;
   double u1,r1,p1,e1;
   StndErr=1e-6;
   if (NULL==(Matter=dynamic_cast<MatterIO*>(SavableClass::Read(in))))
       throw info_except("Could not register matter.\n");
cout<<" Coreleft() "<<Coreleft()<<" Stack "<<StackAvail()<<"\n";
   int GetBegPres;
//   in>>tmp>>u1>>tmp>>r1>>tmp>>p1>>tmp>>e1;in.getline(tmp,511);
   in>>tmp>>u1>>tmp>>r1>>tmp>>p1>>tmp>>e1>>tmp>>porous>>tmp>>StndErr>>tmp>>GetBegPres;
   r2=r1;
//>>tmp;r1=r2;
   double Rcritical=Matter->DencityMax();
   MatterAdiabat=Matter;
   SetIntPar(r1,e1);
//   p1=Matter->Pressure(r1,e1);
   if (!GetBegPres)   p1=PressurePorous(r1,e1);
cout<<"ShowHugPres  beg    Coreleft() "<<Coreleft()<<Time<<" p0 "<<p1<<"\n";
//NumPres=0;NumPresClc=0;NumDenc=0;NumDencClc=0;
   ShowHugPres(in,u1,r1,p1,e1,u2,r2,p2,e2,D);
cout<<"ShowHugPres  Coreleft() "<<Coreleft()<<Time<<"\n";
//cout<<" NumCalcPnt "<<NumPres<<" NumPressCalc "<<NumPresClc<<"\n";
   ShowHugDenc(in,u1,r1,p1,e1,u2,r2,p2,e2,D);
cout<<"ShowHugDenc    beg    Coreleft() "<<Coreleft()<<Time<<"\n";
//cout<<" NumCalcPnt "<<NumDenc<<" NumDencCalc "<<NumDencClc<<"\n";
cout<<"ShowEnergy   beg Coreleft() "<<Coreleft()<<Time<<"\n";
   ShowEnergy(in); // H2
cout<<"ShowTemperature   beg Coreleft() "<<Coreleft()<<Time<<"\n";
   ShowTemperature(in); // H2
cout<<"ShowIsoTherm beg   Coreleft() "<<Coreleft()<<Time<<"\n";
   ShowIsoTherm(in); // H2
cout<<"ShowIsoEntrope beg Coreleft() "<<Coreleft()<<Time<<"\n";
   ShowIsoEntrope(in); // H2
cout<<"ShowIsoEntrope end Coreleft() "<<Coreleft()<<Time<<"\n";
//#ifdef  MatterSpline_Register
cout<<"FormSpline beg Coreleft() "<<Coreleft()<<Time<<"\n";
   FormSplineDat(in);
cout<<"FormTwoPhaseBnd beg Coreleft() "<<Coreleft()<<Time<<"\n";
   FormTwoPhaseBnd(in);
cout<<"FindBushCoefABC_Mat beg Coreleft() "<<Coreleft()<<Time<<"\n";
   FindBushCoefABC_Mat(in);
cout<<"FormSolLiqBnd beg Coreleft() "<<Coreleft()<<Time<<"\n";
   FormSolLiqBnd(in);

//#endif
cout<<"FormSolidLiquidDat  beg    Coreleft() "<<Coreleft()<<Time<<"\n";
   FormSolidLiquidDat(in);   // H2
/*
#ifdef MatterH2Liq_Register
cout<<"ShowColdH2  beg    Coreleft() "<<Coreleft()<<Time<<"\n";
   ShowColdH2(in);   // H2
cout<<"ShowColdEmpi  beg    Coreleft() "<<Coreleft()<<Time<<"\n";
   ShowColdEmpi(in);   // H2
#endif
#ifdef  RossDiameter
cout<<"RossDiameter beg Coreleft() "<<Coreleft()<<Time<<"\n";
   ShowRossDiameter(in);
#endif
*/
   delete Matter;
   cout<<" Leak Test Now\n";
  LeakTest();
    } catch(stdexception &ex){ 
        cout<<" Exeption caught:\n"<<ex.what()<<"\n";
        return ;
    }

  };

void OutputExamp()
 {
  cout<<" usage: \nmadi_ex file_cfg \n"<<
         "Current version - \n"<<
//#ifdef INTERNAL_FUNC 
//         "INTERNAL_FUNC - with dissosiation\n"<<
//#else
//         "NOT INTERNAL_FUNC - without dissosiation\n"<<
//#endif

#ifdef RossDiameter 
         "RossDiameter - can show rossdiameter\n"<<
#else
         "NOT RossDiameter - can NOT show rossdiameter\n"<<
#endif

//#ifdef InternalPointsUrs 
//         "InternalPointsUrs - EOS with internal points - MatterSum\n"<<
//#else
//         "NOT InternalPointsUrs - without MatterSum\n"<<
//#endif
         "\n";
   fstream examp("madi_ex",ios::out);
   examp<<
//#ifndef InternalPointsUrs 
"MatterABu   Material_File material.cfg Substance Steel-Tern          \n"<<
"                                                                     \n"<<
"u1 0 r1 7.9 p1 0      e1  0.86  porous 1. StndErr 1e-5 SetBegPres 1  \n"<<
"u_beg 0.  min_p 1.001 max_u1 100 stp 3                               \n"<<
"ShowHugPres      0   !h.dat                                          \n"<<
"                                                                     \n"<<
"not_used 0.1 min_D 0.14 max_D 0.35 stp .02                           \n"<<
"ShowHugDenc     0   !dhug5.dat                                       \n"<<
"                                                                     \n"<<
"No   0.1     No 5         No 1         No 1000                       \n"<<
"ShowEnergyTabl  0   tab1                                             \n"<<
"                                                                     \n"<<
"No   0.1     No 5         No 1         No 1000                       \n"<<
"ShowTempTabl  0   data.txt                                           \n"<<
"                                                                     \n"<<
"initial_t    5000 min_r1 0.08    max_r1 2.5     stp .01              \n"<<
"ShowIsotherm  1   tr_.dat                                            \n"<<
"                                                                     \n"<<
"init_e 62.2368 min_d 0.46 max_d 1.1 stp 0.01                         \n"<<
"ShowIsoentr 0   !grig1.dat                                           \n"<<
"                                                                     \n"<<
"No   0.1     No 5         No 1         No 1000                       \n"<<
"ConstructSpl  0   !beg.cfg                                           \n"<<
"                                                                     \n"<<
"FileName h2_param.cfg  NameSubstance H2_My                           \n"<<
"initial_t  10 min_r1 0.029 max_r1 2   stp .01                        \n"<<
"ShowCold     0   !01.dat                                             \n"<<
"                                                                     \n"<<
"MatterFreeE FreeEEmpiLiquid  FileParam h2_param.cfg  NameSubstance Empi_My3 \n"<<
"initial_t  10 min_r1 0.029 max_r1 2   stp .01                        \n"<<
"ShowCold     0   !01.dat                                             \n";
//#else
//#endif
   examp.close();
  }
  













