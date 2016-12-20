#include <lib/precompiled/cell_kru.h>
#include <lib/precompiled/cell_dat.h>

#include "uil_reg.h"

//extern unsigned _stklen=60000;
//double StndErr=1e-6;
double StndErr=8e-4;


int Region::ReadMatter(istream &input)
  {
   DeleteRegion();
   char tmp[2256],tmp1[2256];
   input>>tmp>>NumPnt>>tmp>>LftPnt>>tmp>>RgtPnt;
//#ifdef InternalPointsUrs
   int k;NumIntPar=0;
   if (!seek_eoln(input))
        input>>tmp>>NumIntPar;


//#endif
   input.getline(tmp,2256);
   input>>tmp>>LftFree>>tmp>>RgtFree>>tmp>>TimeStability;input.getline(tmp1,2256);
   input>>tmp>>Viscouse>>tmp>>Viscouse2;input.getline(tmp1,2256);
   input>>tmp>>NumMatter>>tmp;
cout<<" NumMatter "<<NumMatter<<"\n";
   for (k=1;k<NumMatter;k++) input>>BndMatter[k];
   BndMatter[NumMatter]=NumPnt;
   BndMatter[0]=0;
   input.getline(tmp,2256);
   IntParNames.Resize(NumMatter+1);
   //Matter.Resize(NumMatter+1);
   //B.Resize(NumMatter+1);
   Matter.SetDim(NumMatter+2);
   B.SetDim(NumMatter+2);
   for (k=1;k<=NumMatter;k++){ 
       //delete Matter[k];
       if ( !(Matter[k] << SavableClass::Read(input)) ) 
            throw info_except("Could not read. Exit.\n");
       cout << " Read Matter " << Matter[k] << "\n";
        int N = Matter[k]->NumInputVal()-2;
        assert(N <= NumIntPar);
        map<Stroka, void*> names = Matter[k]->GetInputNamesMap();
        //NumIntPar = max<double>(NumIntPar, N);
        IntParNames[k].SetDim(NumIntPar);
       for(int i=0;i<N;i++){
           input>>tmp;
           if (names.find(tmp) == names.end()) {
               Stroka descr = Stroka("This matter have internal names to be settled.") +
                 "Read bad internal name for the matter: " + tmp + " have to be one of the: " + Str::Vec2Str(Str::Map2Vec(names));
               throw info_except(~descr);
           }
           IntParNames[k][i] = tmp;
       }
       for(int i=N;i<NumIntPar;i++)
           IntParNames[k][i] = Itoa(i,tmp,10);
   }

   for (k=1;k<=NumMatter;k++) B[k]=new Body(BndMatter[k]-BndMatter[k-1]+1,Matter[k],
       Viscouse,Viscouse2,IntParNames[k]);
   B[0]=NULL;B[NumMatter+1]=NULL;
   SetSkipBody();
   return 1;
  }; 



int Region::ReadData(istream &input)
 {
//   int i=0,NumR=5+NumIntPar;
//   double *d=new double [7+NumIntPar];
   char tmp[2256];
cout<<" Reading Param RegionIO::ReadData Num "<<NumPnt<<" Coreleft "<<Coreleft()<<"\n";
   input.getline(tmp,2255);// EndIter
   input.getline(tmp,2255);// Pos Pres...
   for (int k=1;k<=NumMatter;k++) 
     if (!B[k]->ReadData(input))
      {cout<<"Region::ReadData; Entered only part of points NumMatter i "
          <<k<<"\n";
       DeleteRegion();
       return 0;
      }
   return 1;
 };

void Region::SaveData(ostream &output){
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
//#ifdef InternalPointsUrs
   output<<" NumIntPar "<<NumIntPar;
//#endif
			output<<"\n";
   output<<"LftBnd_Free "<<LftFree<<" RgtBnd_Free "<<RgtFree
         <<" TimeStability "<<TimeStability<<"\n";
   output<<" Viscouse "<<Viscouse<<" Viscouse2 "<<Viscouse2<<"\n";
   output<<"NumMatters "<<NumMatter<<" Matter_Boundaries ";
   for (k=1;k<NumMatter;k++) output<<BndMatter[k]<<"  ";
   output<<"\n";
//   output<<" Viscouse ";
//   for (k=1;k<=NumMatter;k++) output<<Viscouse[k]<<"  ";
//   output<<"\n";
   for (k=1;k<=NumMatter;k++){ 
       if (!SavableClass::Save(output,Matter[k]) ) 
            throw info_except("Could not save matter");
       int N = Matter[k]->NumInputVal()-2;
       for(int i=0;i<N;i++)
           output<<" "<<IntParNames[k][i].c_str()<<" ";
       output<<"\n";
   }
}; 

//void Region::SetBound() { for (int k=1;k<=NumMatter;k++) B[k]->SetBound(B[k-1],B[k+1]);};
    
void Region::TimeCh(double TimeStp) { for (int k=1;k<=NumMatter;k++) B[k]->TimeCh(TimeStp);};

void Region::StepRegion(double TimeStp,double &MinTimeStp,double &PresDeriv){
    ControlSkipBody();
    MinTimeStp=1e10;

    int N=NumMatterV;
    for (int k=1;k<=N;k++) 
        CellFixed(*(BV[k]));
    for (int k=1;k<=N;k++) 
        BV[k]->SetBoundU(BV[k-1],BV[k+1]);
    for (int k=1;k<=N;k++) 
        CalcTStpVisc(*(BV[k]),TimeStp,MinTimeStp,TimeStability);
    for (int k=1;k<=N;k++) 
        BV[k]->SetBoundPv(BV[k-1],BV[k+1]);
    for (int k=1;k<=N;k++) {
        CalcPosE(*(BV[k]),TimeStp);
        BV[k]->TimeCh(TimeStp);
    };
    PresDeriv=0;
    for (int k=1;k<N;k++)
        PresDeriv+=fabs(BV[k]->Pressure[BV[k]->NumPnt-2]-BV[k+1]->Pressure[1]);
    PresDeriv=max<double>(log(max<double>(1,PresDeriv))-8,0);
 };

int Region::SetWorkDim(int n)
 {
  if (n==0) return UilWorkDim;
  UilWorkDim=n;
  int N=NumMatterV;
  for (int k=1;k<=N;k++) BV[k]->UilWorkDim=n;
  return 1;
 };

void Region::SetBodySkipS(int NumSkipS)
 {
  for (int k=1;k<=NumMatter;k++) B[k]->NumSkipS=NumSkipS;
 };

void Region::ClcMass() 
 {
  //double *S=new double[NumPnt];
  for (int k=1;k<=NumMatter;k++) ClcMassInBody(*B[k]);
  for (int k=1;k<=NumMatter;k++) B[k]->SetBoundMU(BV[k-1],B[k+1]);
  for (int k=1;k<=NumMatter;k++) B[k]->SetBoundU(BV[k-1],B[k+1]);
  for (int k=1;k<=NumMatter;k++) B[k]->SetBoundPv(BV[k-1],B[k+1]);
 };

void Region::DeleteRegion()
 {
  //if (NumMatter!=0)
  // {
  //  for (int k=1;k<=NumMatter;k++) 
  //   {
  //    delete B[k];B[k]=NULL;
  //    delete Matter[k];Matter[k]=NULL;
  //   }
  //  NumMatter=0;
  // }
 };
void Region::SetSkipBody()
 {
  for (int k=0;k<=NumMatter+1;k++) BV[k]=B[k];
  NumMatterV=NumMatter;
  MayBeSkippedNum=0;
  for (int k=2;k<NumMatter;k++) 
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
//       if ( ( B[b-1]->Pos[N-1]+(B[b-1]->Pos[N-1]-B[b-1]->Pos[N-2])*1.5>=B[b+1]->Pos[1]) ||
//            ( B[b+1]->Pos[1]+(B[b+1]->Pos[1]-B[b+1]->Pos[2])*1.5<=B[b-1]->Pos[N-1]) )
       if  ( ( 2*B[b-1]->Pos[N-1]-B[b-1]->Pos[N-2]>=B[b+1]->Pos[1]) ||
             ( 2*B[b+1]->Pos[1]-B[b+1]->Pos[2]<=B[b-1]->Pos[N-1]) )
         {
          ResetAll=1;MayBeSkipped[1][k]=1;
         };
      }
  if (ResetAll)
   {
    int i=1,Shift=0;
    for (int k=0;k<=NumMatter+1;k++)
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
    for (int k=1;k<=NumMatterV;k++) BV[k]->SetBoundMU(BV[k-1],BV[k+1]);
    for (int k=1;k<=NumMatterV;k++) BV[k]->SetBoundU(BV[k-1],BV[k+1]);
    for (int k=1;k<=NumMatterV;k++) BV[k]->SetBoundPv(BV[k-1],BV[k+1]);
       
   }
 };







