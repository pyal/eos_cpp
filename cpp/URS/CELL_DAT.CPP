
//#include "util.h"
//#include "matt_fac.h"
//#include "matt_reg.h"
#include "cell_fac.h"
//#define STP stp
//#define STP stp/ro
// ============= inputing region ===============
//NumPnt   0  LftPnt  1   RgtPnt  10 Viscouse 0.05    comments; MatterRegister
//NumMatters 2  Matter_Boundaries  50
//MatterABu   Matter_File material.cfg    Substance  Tmp
//MatterABu   Matter_File material.cfg    Substance  Tmp
//
// Number_Pnt  100    NumMatter=2
// Pressure[1]  100  Dencity[1] 1    Energy[1] 0  Velocity[1]  0  Pos[2] 5
// Pressure[2]  100  Dencity[2] 7.1  Energy[1] 0  Velocity[1]  -5  Pos[3] 10
//double RegionIO::Pres(MatterIO *Mat,double Denc,double En,double Vel,double dV)
//  {}; 
//double RegionIO::Sound(MatterIO *Mat,double Denc,double En,double Vel)
//  {};
         
void SetParam(double *A,double &stp,double *U,double *P,double *E,
                   double *R,double *Stp,int BndNum)
  {
   A[2]=P[BndNum];A[3]=R[BndNum];
   A[4]=E[BndNum];A[5]=U[BndNum];stp=Stp[BndNum];
  };
void GetParam(fstream &input,double *U,double *P,double *E,
        double *R,double *Stp,RegionIO &Reg,int &Num,int BegReg,int GivenL)
  {
   char tmp[256];
   double Pos,Lft=Reg.LftPnt;
//   input>>tmp>>Num;input.getline(tmp,255);
	  Num=Reg.NumPnt;
cout<<" Num "<<Num<<" NumMatter "<<Reg.NumMatter<<"\n";
//input>>tmp;cout<<"\nFst "<<tmp<<"\n";
   for (int k=BegReg;k<=Reg.NumMatter;k++)
     {
      input>>tmp>>P[k]>>tmp>>R[k]>>tmp>>E[k]>>tmp>>U[k]>>tmp>>Pos;
						if (R[k]==0) R[k]=Reg.Matter[k]->DencityCold();
cout<<" P[k] "<<P[k]<<" R[k] "<<R[k]<<" E[k] "<<E[k]<<" U[k] "<<U[k]
    <<" Pos "<<Pos;
      input.getline(tmp,255);
      if (GivenL) Stp[k]=Pos;else  Stp[k]=(Pos-Lft);
      Lft=Pos;
cout<<" Stp[k] "<<Stp[k]<<"\n";
      if (Stp[k]<=0) { cout<<" Error Stp<0. Exiting.\n";exit(0);}
     }
cout<<"\n";
   for (k=2;k<Reg.NumMatter;k++)  Stp[k]/=(Reg.BndMatter[k]-Reg.BndMatter[k-1]);
   if (Reg.NumMatter>=1)
     Stp[1]/=(Reg.BndMatter[1]); 
   if (Reg.NumMatter>=2)
     Stp[Reg.NumMatter]/=(Num-Reg.BndMatter[Reg.NumMatter-1]);
cout<<" Bnd k-1 "<<Reg.BndMatter[Reg.NumMatter-1]<<" Num  "<<Num<<" Stp "<<Stp[Reg.NumMatter]<<"\n";
  };


extern void OutputExamp();
void main( int argc, char *argv[] )
  {
   char tmp[256];
   argc--;
   double U[MaxNumMatter],P[MaxNumMatter],D[MaxNumMatter],R[MaxNumMatter],
          E[MaxNumMatter],Stp[MaxNumMatter];
   double A[7],stp;
   int Num,k,BndNum=1;
   if (argc<=1) { OutputExamp();exit(0);}
   fstream input=fstream(argv[1],ios::in);
   fstream output=fstream(argv[2],ios::out);
   input.getline(tmp,255);	output<<tmp<<"\n";
//cout<<tmp<<"\n";
   RegionIO Reg;Reg.ReadMatter(input);
	if (seek_eof(input)) { cout<<"Reg ate all;Break;\n";exit(0);};
input.getline(tmp,255);input.getline(tmp,255);cout<<tmp<<"\n";


	double *d=new double [Reg.NumIntPar];
   for (k=0;k<Reg.NumIntPar;k++) d[k]=0;
   A[1]=Reg.LftPnt;
   char *AddFile;
   int NumCellFirst=1;
   if ((AddFile=GetCmd("/b",argc,argv))!=NULL)
     {
cout<<" Adding file "<<AddFile<<"\n";
      fstream Add=fstream(AddFile,ios::in);
      RegionIO Reg1;Add.getline(tmp,255);
      if (!Reg1.ReadIni(Add)) {cout<<" Could not Add. Exiting\n";exit(0);}
//      for (k=2;k<=Reg1.NumMatter;k++) {BndNum++;SetParam(&A[0],stp,&U[0],&P[0],&E[0],&R[0],&Stp[0],BndNum);}
      BndNum=Reg1.NumMatter+1;
//      NumCellFirst=Reg1.NumPnt+1;
      NumCellFirst=Reg1.NumPnt;
      MemoryMove(&Reg1.Pos[1],&Reg.Pos[1],sizeof(double)*Reg1.NumPnt);
      MemoryMove(&Reg1.Pressure[1],&Reg.Pressure[1],sizeof(double)*Reg1.NumPnt);
      MemoryMove(&Reg1.Dencity [1],&Reg.Dencity [1],sizeof(double)*Reg1.NumPnt);
      MemoryMove(&Reg1.Velocity[1],&Reg.Velocity[1],sizeof(double)*Reg1.NumPnt);
      MemoryMove(&Reg1.Energy  [1],&Reg.Energy  [1],sizeof(double)*Reg1.NumPnt);
      if (Reg.NumIntPar>0) 
        {for (k=1;k<=Reg1.NumPnt;k++) 
          MemoryMove(Reg1.IntPar[k],Reg.IntPar[k],sizeof(double)*Reg.NumIntPar);
        }
      Add.close();
      A[1]=Reg1.Pos[Reg1.NumPnt];
cout<<" A[1] "<<A[1]<<" BndNum "<<BndNum<<" NumCellFirst "<<NumCellFirst<<"\n";
     };   
   GetParam(input,&U[0],&P[0],&E[0],&R[0],&Stp[0],Reg,Num,BndNum,(GetCmd("/l",argc,argv)!=NULL));
   SetParam(&A[0],stp,&U[0],&P[0],&E[0],&R[0],&Stp[0],BndNum);
   if (NumCellFirst==1) A[1]-=stp;

   for (k=NumCellFirst;k<=Num;k++)
     {
						A[1]+=stp;
      if ( (k>=Reg.BndMatter[BndNum]) && (BndNum!=Reg.NumMatter) )
        {
         BndNum++;          
         SetParam(&A[0],stp,&U[0],&P[0],&E[0],&R[0],&Stp[0],BndNum);
cout<<" BndNum "<<BndNum;
cout<<" Pos "<<A[1]<<" stp "<<stp<<" U "<<A[5]<<" P "<<A[2]<<" E "<<A[4]<<"\n";
        }
						Reg.Pos     [k]=A[1];
						Reg.Pressure[k]=A[2]*1e5;
						Reg.Dencity [k]=A[3];
						Reg.Velocity[k]=A[5];
						Reg.Energy  [k]=A[4];
						if (Reg.NumIntPar>0) MemoryMove(d,Reg.IntPar[k],sizeof(double)*Reg.NumIntPar);

//        WriteStr(output,A,5);
     } 
   			delete d;
			Reg.Pres();
			Reg.SaveIni(output);
			output<<"EndIter\n";	output.close();	input.close();
  };



void OutputExamp()
  {cout<<" usage: \ncell_dat file_cfg file_calc [options]\n"<<
         "options: /bnam_ - at the begining put data from nam_ calculation file\n"<<
         "         /l     - set length of matters, not there positions\n";
   fstream examp("cell_dat",ios::out);
   examp<<
"BegTime 0   TimeStp 1e-10 TimeWrite   1.5e-7 PresDerivCoef 0.7 EndTime   1e-5  \n"<<
"NumPnt   400  LftPnt   0  RgtPnt   10  NumPar 0                                \n"<<
"LftBnd_Free 1 RgtBnd_Free 0 TimeStability 0.02                                 \n"<< 
"Viscouse 0.7  SecondVisc 0                                                     \n"<<
"NumMatters 3 Matter_Boundaries 100  200                                        \n"<<
"MatterABu   Material_File material.cfg Substance Steel-Tern                    \n"<<
"MatterABu   Material_File material.cfg Substance Pleks-Tern                    \n"<<
"MatterABu   Material_File material.cfg Substance Cu-Tern                       \n"<<
"bad                                                                            \n"<<
"Parameters                                                                     \n"<<
" P[1]   00  D[1] 0       E[1] 0    V[1]  5.2    Pos[2] 1.52                    \n"<<   
" P[1]   00  D[1] 0       E[1] 0    V[1]  0  Pos[2] 4.09                        \n"<<  
" P[1]   00  D[1] 0       E[1] 0    V[1]   0  Pos[2] 4.596                      \n";
   examp.close();
  }
