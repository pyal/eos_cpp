
#include "eno_reg.h"

double StndErr=8e-4;


int Region::ReadMatter(istream &input)
  {
   DeleteRegion();
   char tmp[256],tmp1[256];
   int k;NumIntPar=0;
   input>>tmp>>NumPnt>>tmp>>LftPnt>>tmp>>RgtPnt;
#ifdef InternalPointsUrs
input>>tmp>>NumIntPar;
#endif
   input.getline(tmp,256);
   input>>tmp>>LftFree>>tmp>>RgtFree>>tmp>>TimeStability;input.getline(tmp1,256);
   input>>tmp>>Viscouse>>tmp>>Viscouse2;
input>>tmp>>Order;    
input.getline(tmp1,256);
   input>>tmp>>NumMatter>>tmp;
cout<<" NumMatter "<<NumMatter<<"\n";
   for (k=1;k<NumMatter;k++) input>>BndMatter[k];
   BndMatter[NumMatter]=NumPnt;
   BndMatter[0]=0;
   input.getline(tmp,256);
   for (k=1;k<=NumMatter;k++) 
      if (!MatterRegister(input,Matter[k]) ) 
        { cout<<"RegionIO::ReadIni;  Could not register matter. Exiting.\n";
          for (int i=1;i<k;i++) delete Matter[i];
          exit(1);
          return 0;
         }//exit(1);

   for (k=1;k<=NumMatter;k++) B[k]=new Body(BndMatter[k]-BndMatter[k-1]+1,NumIntPar,Matter[k],
                                             Viscouse,Viscouse2);
   B[0]=NULL;B[NumMatter+1]=NULL;
   SetSkipBody();
   return 1;
  }; 



int Region::ReadData(istream &input)
 {
   char tmp[256];
cout<<" Reading Param RegionIO::ReadData Num "<<NumPnt<<" Coreleft "<<Coreleft()<<"\n";
   input.getline(tmp,255);// EndIter
   input.getline(tmp,255);// Pos Pres...
   for (int k=1;k<=NumMatter;k++) 
     if (!B[k]->ReadData(input))
      {cout<<"Region::ReadData; Entered only part of points NumMatter i "
          <<k<<"\n";
       DeleteRegion();
       return 0;
      }
   return 1;
 };

void Region::SaveData(ostream &output)
  {
   output<<"   Pos         Pressure   Dencity    Energy     Velocity   \n";
   int k;
   if (RegionShow) for ( k=1;k<=NumMatterV;k++) BV[k]->SaveData(output);
   else            for ( k=1;k<=NumMatter;k++) B[k]->SaveData(output);
   
   output<<"EndIter \n";
  }; 


void Region::SaveMatter(ostream &output)
  {
//   char tmp[256];
   int k;
   output<<"NumPnt   "<<NumPnt<<"  LftPnt   "<<LftPnt<<"  RgtPnt   "<<RgtPnt;
#ifdef InternalPointsUrs
   output<<" NumIntPar "<<NumIntPar;
#endif
			output<<"\n";
   output<<"LftBnd_Free "<<LftFree<<" RgtBnd_Free "<<RgtFree
         <<" TimeStability "<<TimeStability<<"\n";
   output<<" Viscouse "<<Viscouse<<" Viscouse2 "<<Viscouse2;//<<"\n";
output<<" OrderOfSheme "<<Order<<"\n";
   output<<"NumMatters "<<NumMatter<<" Matter_Boundaries ";
   for (k=1;k<NumMatter;k++) output<<BndMatter[k]<<"  ";
   output<<"\n";
   for (k=1;k<=NumMatter;k++) 
      if (!MatterStore(output,Matter[k]) ) 
        { cout<<"Region::SaveIni;  Could not save     matter. Exiting.\n";exit(1);}
  }; 

    
void Region::TimeCh(double TimeStp) 
 { for (int k=1;k<=NumMatter;k++) B[k]->TimeCh(TimeStp);};

int Region::SetWorkDim(int n)
 {
  if (n==0) return UilWorkDim;
  UilWorkDim=n;
  int N=NumMatterV;
  for (int k=1;k<=N;k++) BV[k]->UilWorkDim=n;
  return 1;
 };

void Region::SetBodySkipS(int NumSkipS)
 { for (int k=1;k<=NumMatter;k++) B[k]->NumSkipS=NumSkipS;};


void Region::DeleteRegion()
 {
  if (NumMatter!=0)
   {
    for (int k=1;k<=NumMatter;k++) 
     {
      delete B[k];B[k]=NULL;
      delete Matter[k];Matter[k]=NULL;
     }
    NumMatter=0;
   }
 };
void Region::SetSkipBody()
 {
  for (int k=0;k<=NumMatter+1;k++) BV[k]=B[k];
  NumMatterV=NumMatter;
  MayBeSkippedNum=0;
  for (k=2;k<NumMatter;k++) 
     if (B[k]->Matter->DencityCold()==0) 
      {
       MayBeSkippedNum++;
       MayBeSkipped[0][MayBeSkippedNum]=k;
       MayBeSkipped[1][MayBeSkippedNum]=0;
      }
 };

void Region::ControlSkipBody()
 {
  int ResetAll=0;
  for (int k=1;k<=MayBeSkippedNum;k++) 
     if (!MayBeSkipped[1][k])
      {
       int b=MayBeSkipped[0][k],N=B[b-1]->NumPnt;
       if  ( ( 2*B[b-1]->Pos[N-1]-B[b-1]->Pos[N-2]>=B[b+1]->Pos[1]) ||
             ( 2*B[b+1]->Pos[1]-B[b+1]->Pos[2]<=B[b-1]->Pos[N-1]) )
         {
          ResetAll=1;MayBeSkipped[1][k]=1;
         };
      }
  if (ResetAll)
   {
    int i=1,Shift=0;
    for (k=0;k<=NumMatter+1;k++)
     {
      if ( (i>MayBeSkippedNum) || (k!=MayBeSkipped[0][i]) ) BV[k-Shift]=B[k];
      else 
       {
        if (!MayBeSkipped[1][i]) {BV[k-Shift]=B[k];}
        else                    {Shift++;}
        i++;
       }
     }
    NumMatterV=NumMatter-Shift;
    for (k=1;k<=NumMatterV;k++) BV[k]->SetBoundMU(BV[k-1],BV[k+1]);
    for (k=1;k<=NumMatterV;k++) BV[k]->SetBoundU(BV[k-1],BV[k+1]);
    for (k=1;k<=NumMatterV;k++) BV[k]->SetBoundPv(BV[k-1],BV[k+1]);
       
   }
 };






void Region::StepRegion(double TimeStp,double &MinTimeStp,double &PresDeriv)
 {
   ControlSkipBody();
   MinTimeStp=1e10;

  int N=NumMatterV;
  for (int k=1;k<=N;k++) CellFixed(*(BV[k]));
  for (k=1;k<=N;k++) BV[k]->SetBoundU(BV[k-1],BV[k+1]);
  for (k=1;k<=N;k++) CalcTStpVisc(*(BV[k]),TimeStp,MinTimeStp,TimeStability);
  for (k=1;k<=N;k++) BV[k]->SetBoundPv(BV[k-1],BV[k+1]);
  for (k=1;k<=N;k++) 
   {
    CalcPosE(*(BV[k]),TimeStp);
    BV[k]->TimeCh(TimeStp);
   };
   PresDeriv=0;
   for (k=1;k<N;k++)
      PresDeriv+=fabs(BV[k]->Pressure[BV[k]->NumPnt-2]-BV[k+1]->Pressure[1]);
	  PresDeriv=max(log(max(1,PresDeriv))-8,0);
 };

void Region::ClcMass() 
 {
  double *S=new double[NumPnt];
  for (int k=1;k<=NumMatter;k++) ClcMassInBody(*B[k]);
  for (k=1;k<=NumMatter;k++) B[k]->SetBoundMU(BV[k-1],B[k+1]);
  for (k=1;k<=NumMatter;k++) B[k]->SetBoundU(BV[k-1],B[k+1]);
  for (k=1;k<=NumMatter;k++) B[k]->SetBoundPv(BV[k-1],B[k+1]);
 };

/*       UIL   VAR
void Region::StepRegion(double TimeStp,double &MinTimeStp,double &PresDeriv)
 {
   ControlSkipBody();
   MinTimeStp=1e10;

  int N=NumMatterV;
  for (int k=1;k<=N;k++) CellFixed(*(BV[k]));
  for (k=1;k<=N;k++) BV[k]->SetBoundU(BV[k-1],BV[k+1]);
  for (k=1;k<=N;k++) CalcTStpVisc(*(BV[k]),TimeStp,MinTimeStp,TimeStability);
  for (k=1;k<=N;k++) BV[k]->SetBoundPv(BV[k-1],BV[k+1]);
  for (k=1;k<=N;k++) 
   {
    CalcPosE(*(BV[k]),TimeStp);
    BV[k]->TimeCh(TimeStp);
   };
   PresDeriv=0;
   for (k=1;k<N;k++)
      PresDeriv+=fabs(BV[k]->Pressure[BV[k]->NumPnt-2]-BV[k+1]->Pressure[1]);
	  PresDeriv=max(log(max(1,PresDeriv))-8,0);
 };

void Region::ClcMass() 
 {
  double *S=new double[NumPnt];
  for (int k=1;k<=NumMatter;k++) ClcMassInBody(*B[k]);
  for (k=1;k<=NumMatter;k++) B[k]->SetBoundMU(BV[k-1],B[k+1]);
  for (k=1;k<=NumMatter;k++) B[k]->SetBoundU(BV[k-1],B[k+1]);
  for (k=1;k<=NumMatter;k++) B[k]->SetBoundPv(BV[k-1],B[k+1]);
 };

*/