#ifndef __AUT_FAC_H
#define __AUT_FAC_H

#include "matt_reg.h"
//#include <malloc.h>
#define MaxNumMatter 10

struct Int_Par
  {
   double Pres,Dens,Ener,Velo;
  }; 
struct One_Cell
  {
   double PosU,PosD,PosCU,PosCD,PosM;
   Int_Par IntU,IntD,IntC,IntM;
   double TimU,TimD,TimB;
   double DvelU,DvelD;
   friend int Interact(One_Cell &Fst,One_Cell &Sec,
         MatterIO *MFst,MatterIO *MSec,double Tim);
   friend int MeanPar(One_Cell Fst,Int_Par &Res,double &Pos,double Tim);
//   friend void FormCell(One_Cell &Fst,Int_Par Cur,double Pos,double Tim);
  }; 

struct AutIO:InterfaceIO
  {
   static const char * const name;
   virtual const char *InterfaceIOName() {return name;}

   int NumPnt,NumMatter,BndMatter[MaxNumMatter+1];
   double CurTime;
   MatterIO *Matter[MaxNumMatter+1];
   One_Cell *Data;

   AutIO(){One_Cell=NULL;NumPnt=0;Matter=NULL;}
   friend int ReadIni(fstream &input);
   friend void SaveIni(fstream &output);
   friend void FormClc(AutIO &dat);
   friend void FormSav(AutIO &dat);
   friend void ClcTim(double TimeStp,AutIO &dat);

//   ReadPar(fstream &input){};
//   SavePar(fstream &output){};
//   int GetNumMatter(int k);    
//   RegionIO& operator=(RegionData& Dat);

   void InterfaceIODelete();
   ~AutIO(){InterfaceIODelete();};
  };

void FormClc(AutIO &dat)
  {
   int k,MFst,MSec;
   ConstructPos();
   for (k=0;k<=NumPnt;k++)
     {
      MFst=GetNumMatter(k,dat);MSec=GetNumMatter(k+1,dat);
      if (!Interact(dat.Data[k],dat.Data[k+1],dat.Data.Matter[MFst],
             dat.Data.Matter[MSec],dat.CurTime) {cout<<" Bad interaction in form\n";exit(0);}
     }
  };
       
void ClcTim(double TimeStp,AutIO &dat)
  {
   do 
     {
      double MinT=MathInf;
      int MinK=1,k;
      for (k=1;k<=NumPnt;k++)
         if (dat.Data[k].TimB>MinT) {MinT=dat.Data[k].TimB;MinK=k;}
      if (MinT-dat.CurTime>=TimeStp) {dat.CurTime+=TimeStp;return ;}
      else 
        {
         int Fst,Sec,MFst,MSec;
         if (MinT==dat.Data[MinK].TimU) {Fst=MinK;Sec=MinK+1;}
         else {Fst=MinK-1;Sec=MinK;}
         MFst=GetNumMatter(Fst,dat);MSec=GetNumMatter(Sec,dat);
         if (!Interact(dat.Data[Fst],dat.Data[Sec],Matter[MFst],
                       Matter[MSec],Tim) {cout<<" Bad interaction\n";exit(0);}
         dat.CurTime=Tim;
        };
     }
   while (1);
  };       

int ReadIni(fstream &input,AutIO &dat);
void SaveIni(fstream &output,AutIO &dat);

#endif


int AutIO::ReadIni(fstream &input)
  {
   InterfaceIODelete();
   char tmp[256],tmp1[256];
   int k;NumIntPar=0;
   input>>tmp>>NumPnt>>tmp>>CurTime;input.getline(tmp,256);
   input>>tmp>>NumMatter>>tmp;
//cout<<" NumMatter "<<NumMatter<<"\n";
   for (k=1;k<NumMatter;k++) input>>BndMatter[k];
   BndMatter[NumMatter]=NumPnt;
   BndMatter[0]=1;
   input.getline(tmp,256);
   for (k=1;k<=NumMatter;k++) 
      if (!MatterRegister(input,Matter[k]) ) 
        { cout<<"RegionIO::ReadIni;  Could not register matter. Exiting.\n";
          for (int i=1;i<k;i++) delete Matter[i];
exit(1);
          return 0;
         }//exit(1);
   int i=0,NumR=5;
   double *d=new double [7];
   Data=new One_Cell[NumPnt+1];
			input.getline(tmp,255);
			input.getline(tmp,255);
cout<<" Reading Param RegionIO::ReadIni Num "<<NumPnt<<" Coreleft "<<Coreleft()<<"\n";
   while ( (i<NumPnt) && (!seek_eof(input)) && 
			                      (EnterStr(input,&d[0],NumR+1,1)==NumR) )
//      if ( (EnterStr(input,&d[0],6)==5) ) 
        {                    
         i++;
         Data[i].PosM=d[1];
         Data[i].IntM.Pres=d[2]*1e5;
         Data[i].IntM.Dens=d[3];
         Data[i].IntM.Ener=d[4];
         Data[i].IntM.Velo=d[5];
        }
			delete d;
   if ((i<NumPnt) || (NumPnt==0))
     {cout<<"RegionIO::ReadIni; Entered only part of points NumPnt i "
          <<NumPnt<<"   "<<i<<"\n";
      if (i!=0) InterfaceIODelete();
						return 0;
      }
   return 1;
  }; 

void RegionIO::SaveIni(fstream &output)
  {
   char tmp[256];
   int k;
   output<<"NumPnt   "<<NumPnt<<"  CurTime   "<<CurTime<<"\n";
   output<<"NumMatters "<<NumMatter<<" Matter_Boundaries ";
   for (k=1;k<NumMatter;k++) output<<BndMatter[k]<<"  ";
   output<<"\n";
   for (k=1;k<=NumMatter;k++) 
      if (!MatterStore(output,Matter[k]) ) 
        { cout<<"RegionIO::SaveIni;  Could not save     matter. Exiting.\n";exit(1);}
   int i=0;
   double *d=new double [7];

   output<<"   Pos         Pressure   Dencity    Energy     Velocity   \n";
   while (i<NumPnt)
     {
      i++;
      d[1]=Data[i].PosM;
      d[2]=Data[i].IntM.Pres*1e-5;
      d[3]=Data[i].IntM.Dens;
      d[4]=Data[i].IntM.Ener;
      d[5]=Data[i].IntM.Velo;

      WriteStr(output,&d[0],5);
     }
			delete d;

  }; 
