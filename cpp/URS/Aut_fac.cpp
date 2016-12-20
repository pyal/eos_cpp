#include <lib/precompiled/adi_dou.h>

#include "aut_fac.h"

const char * const AutIO::name="AutIO";
void SetCurTim(One_Cell &Par,double Tim){ Par.CurTime=Tim;};

void ClcPos(One_Cell &Par,double &Pos0,double &Pos1,double &Pos2,double &L)
// Taking Sh0.Time - as last calculated parameter for shock times
// And Par.CurrentTime - as new time, we calculate the result positions
{
   double Ulast=Par.Sh0.Med.Velo+Par.Sh1.Med.Velo+Par.Sh2.Med.Velo;
   double dU=Par.Sh1.Med.Velo+Par.Sh2.Med.Velo,Ct=Par.CurTime;
   Pos1=0;Pos2=0;
   if (Par.Sh1.Dvel!=0) Pos1=(Par.Sh1.Time-Ct)*(Par.Sh1.Dvel-Par.Sh2.Med.Velo);
   if (Par.Sh2.Dvel!=0) Pos2=(Par.Sh2.Time-Ct)*(Par.Sh2.Dvel+Par.Sh1.Med.Velo);
   L=Par.Length+(Ct-Par.Sh0.Time)*dU;
   Pos1=L-Pos1;
   Pos2=max<double>(0,Pos2);Pos1=min<double>(L,Pos1);
   Pos0=Par.Position+(Ct-Par.Sh0.Time)*Par.Sh0.Med.Velo;
};
void ControlBound(One_Cell &Par)
{
// Form Positions, TimeStp
	double Pos1,Pos2,Pos0,L;ClcPos(Par,Pos0,Pos1,Pos2,L);
	Par.Sh0.Time=Par.CurTime;Par.Position=Pos0;
	Par.Sh1.Pos=Pos1;Par.Sh2.Pos=Pos2;Par.Sh0.Pos=L;
// Now we extact waves wich have gone from the cell
	double Tim=Par.CurTime;
	if (Tim>=Par.Sh1.Time) 
	{Par.Sh1.Dvel=0;Par.Sh1.Time=MathInf;Par.Sh1.Med=Par.Sh1.Med*0;}
	if (Tim>=Par.Sh2.Time) 
	{Par.Sh2.Dvel=0;Par.Sh2.Time=MathInf;
	 Par.Sh0.Med=Par.Sh0.Med+Par.Sh2.Med;
	 Par.Sh2.Med=Par.Sh2.Med*0;}
};
void ExchangeBounds(One_Cell &Par)
  {
	One_Cell tmp=Par;
	tmp.Sh1=Par.Sh2;tmp.Sh2=Par.Sh1;
	tmp.Sh0.Med=Par.Sh0.Med+Par.Sh1.Med+Par.Sh2.Med;
	tmp.Sh1.Med=tmp.Sh1.Med*0-Par.Sh2.Med;
	tmp.Sh2.Med=tmp.Sh1.Med*0-Par.Sh1.Med;
	tmp.Sh1.Pos=tmp.Sh0.Pos-Par.Sh2.Pos;
	tmp.Sh2.Pos=tmp.Sh0.Pos-Par.Sh1.Pos;
	Par=tmp;
  };

void AddNewWave(One_Cell &Par,Shock_Par Res,int Fst)
{
   if (!Fst) ExchangeBounds(Par);
   Res.Med=Res.Med-Par.Sh0.Med-Par.Sh1.Med-Par.Sh2.Med;
   if (Norm(Res.Med)<StndErr) {if (!Fst) ExchangeBounds(Par);return ;}
   double Pos1=Par.Sh1.Pos,Pos2=Par.Sh2.Pos,L=Par.Sh0.Pos;
//   double Told=Pos2/max<double>(StndErr,(Par.Sh2.Dvel+Par.Sh0.Med.Velo));
//   double Tnew=L/max<double>(StndErr,(Res.Dvel+Par.Sh0.Med.Velo));
//   if ((Tnew<=Told) || (Norm(Par.Sh2.Med)<sqrt(MathZer)))
	{
     Par.Sh2.Med=Par.Sh2.Med+Res.Med;Par.Sh2.Pos=L;
     Par.Sh2.Dvel=Res.Dvel;
	 Par.Sh2.Time=L/max<double>(StndErr,(Par.Sh2.Dvel+Par.Sh1.Med.Velo))+Par.CurTime;
	 Par.Sh1.Time=(L-Pos2)/max<double>(StndErr,(Par.Sh1.Dvel-Par.Sh2.Med.Velo))+Par.CurTime;
	 if (Norm(Par.Sh1.Med)<sqrt(MathZer)) Par.Sh1.Time=MathInf;
	 if (!Fst) ExchangeBounds(Par);return ;
	}// Just sum
/*   double MisL=(Tnew-Told)*(Res.Dvel+Par.Sh0.Med.Velo);
   double Lnew=L-Tnew*(Res.Med.Velo-Par.Sh0.Med.Velo);
   VecCl Vo=Par2Vec(Par.Sh2.Med),Vn=Par2Vec(Res.Med+Par.Sh2.Med);
   double Ko=sqrt(Vo*Vo),Kn=sqrt(Vn*Vn);
   if ((Ko<MathZer) || (Kn<MathZer)) 
	{ cout<<" Very small add, do not work now. \n";exit(0);}
   VecCl Eo=Vo/Ko,En=Vn/Kn;
   double Alpha=Eo*En;
   VecCl Eort=Eo-En*Alpha;
   double Ltobe=Alpha*Ko/Kn*MisL;
   if ((Ltobe>=L) || (Ltobe<0)) {Ltobe=0;Eort=En;Lnew=L;cout<<" Bad AddNewWave\n";Alpha=0;}
   Int_Par Mean=Vec2Par(Eort*Ko*MisL/Lnew);
   Par.Sh0.Med=Par.Sh0.Med+Mean;
   Par.Sh2.Med=Par.Sh2.Med+Res.Med;
   Par.Sh2.Dvel=Res.Dvel;Par.Sh2.Pos=L-Ltobe;
   Par.Sh2.Time=(L-Ltobe)/(Res.Dvel+Par.Sh0.Med.Velo);
   if (!Fst) ExchangeBounds(Par); 
*/
};
/*
Int_Par Dyn_Hug(Int_Par &Par)
{ Int_Par ret=Par;ret.Pres*=1e-5;return ret;}
Int_Par Hug_Dyn(Int_Par &Par)
{ Int_Par ret=Par;ret.Pres*=1e5;return ret;}
*/
int Interact(One_Cell &Fst,One_Cell &Sec,
             MatterIO *MFst,MatterIO *MSec,double Tim)
{
	Int_Par Par1=Par1*0,Par2=Par2*0;
	Fst.CurTime=Tim;Sec.CurTime=Tim;
	if (MFst!=NULL) {ControlBound(Fst);Par1=Fst.Sh0.Med+Fst.Sh2.Med+Fst.Sh1.Med;}
	if (MSec!=NULL) {ControlBound(Sec);Par2=Sec.Sh0.Med;}
	Shock_Par Res1,Res2;
	if ((MFst!=NULL) && (MSec!=NULL))
		{
//		 Hug2(Dyn_Hug(Par1),Dyn_Hug(Par2),Res1.Med,Res1.Dvel,Res2.Med,Res2.Dvel,MFst,MSec);
//		 Res1.Med=Hug_Dyn(Res1.Med);Res2.Med=Hug_Dyn(Res2.Med);
		 Hug2(Par1,Par2,Res1.Med,Res1.Dvel,Res2.Med,Res2.Dvel,MFst,MSec);
		 AddNewWave(Fst,Res1,1);AddNewWave(Sec,Res2,0);
		 return 1;
		}
	if (MSec==NULL)
		{ 
//		 Hug1(Dyn_Hug(Par1),Res1.Med,Res1.Dvel,MFst,BndPressure);
//		 Res1.Med=Hug_Dyn(Res1.Med);
		 Hug1(Par1,Res1.Med,Res1.Dvel,MFst,BndPressure);
		 AddNewWave(Fst,Res1,1);return 1;
		}
	if (MFst==NULL)
		{
//		 Hug1(Dyn_Hug(Par2),Res2.Med,Res2.Dvel,MSec,BndPressure);
//		 Res2.Med=Hug_Dyn(Res2.Med);
		 Hug1(Par2,Res2.Med,Res2.Dvel,MSec,BndPressure);
		 AddNewWave(Sec,Res2,0);return 1;
		}
		 
	return 0;
};

One_Cell FormNormal(One_Cell &Par) 
  {
   One_Cell ret; 
   Shock_Par tmp;
   double Pos1,Pos2,L,Pos0;ClcPos(Par,Pos0,Pos1,Pos2,L);

   ret=Par;ret.Sh1.Pos=Pos1;ret.Sh2.Pos=Pos2;ret.Sh0.Pos=L;
   if (Pos1>Pos2) {tmp=ret.Sh1;ret.Sh1=ret.Sh2;ret.Sh2=tmp;}
   ret.Position=Pos0;
   return ret;
	};

Shock_Par SetMeanPar(One_Cell &Par,Int_Par *Set)
  {
   Shock_Par ret;
   if (Set==NULL) 
	{
     One_Cell tmp=FormNormal(Par);
	 ret.Med=(tmp.Sh0.Med*tmp.Sh0.Pos+tmp.Sh1.Med*(tmp.Sh0.Pos-tmp.Sh1.Pos)
		  +tmp.Sh2.Med*(tmp.Sh0.Pos-tmp.Sh2.Pos))*(1/tmp.Sh0.Pos);
	 ret.Pos=tmp.Position;
	}
   else 
   {ret.Med=ret.Med*0;Par.Sh0.Med=(*Set);
    Par.Sh1.Med=ret.Med;Par.Sh2.Med=ret.Med;
    Par.Sh0.Dvel=0;Par.Sh1.Dvel=0;Par.Sh2.Dvel=0;
	Par.Sh0.Time=0;Par.Sh1.Time=0;Par.Sh2.Time=0;
	Par.Sh0.Pos=0;Par.Sh1.Pos=0;Par.Sh2.Pos=0;
   }
   return ret;
  };
ostream& operator<<(ostream &output,One_Cell &Par)
  {
//   if ((Par.M!=NULL) && (!MatterStore(output,Par.M)))  { cout<<"Could not store. Exit.\n";exit(0);}
   Shock_Par ret=SetMeanPar(Par);
   output<<FM<<ret.Pos<<FMT<<ret.Med.Pres<<FMT<<ret.Med.Dens
	     <<FMT<<ret.Med.Ener<<FMT<<ret.Med.Velo;
//      d[1]=Data[i].PosM;d[2]=Data[i].IntM.Pres*1e-5;d[3]=Data[i].IntM.Dens;
//      d[4]=Data[i].IntM.Ener;d[5]=Data[i].IntM.Velo;WriteStr(output,&d[0],5);
   return output;    
  };

istream& operator>>(istream &input,One_Cell &Par)
  {
   //char tmp[256];
   Int_Par ret;
   input>>Par.Position>>ret.Pres>>ret.Dens>>ret.Ener>>ret.Velo;
   SetMeanPar(Par,&ret);
//         Data[i].PosM=d[1];Data[i].IntM.Pres=d[2]*1e5;Data[i].IntM.Dens=d[3];
//         Data[i].IntM.Ener=d[4];Data[i].IntM.Velo=d[5];
   return input;
  };

//===============================================================
//===============================================================
//===============================================================



void OneInteract(AutIO &dat,int k)
  {
   int MFst,MSec;
   MFst=GetNumMatter(k,dat);MSec=GetNumMatter(k+1,dat);
   if (!Interact(dat.Data[k],dat.Data[k+1],dat.Matter[MFst],
             dat.Matter[MSec],dat.CurTime)) {cout<<" Bad interaction in form\n";exit(0);}
  };

   
void FormClc(AutIO &dat)
  {
   int k,k1,cur,Tim=dat.CurTime;
   for (k=0;k<dat.NumMatter;k++) 
     {for (k1=dat.BndMatter[k]+1;k1<dat.BndMatter[k+1];k1++) 
       {
        dat.Data[k1].Length=dat.Data[k1+1].Position-dat.Data[k1].Position;
        dat.Data[k1].Mass=dat.Data[k1].Length*dat.Data[k1].Sh0.Med.Dens;
		dat.Data[k1].Sh0.Time=Tim;
       }
	  cur=dat.BndMatter[k+1];
      dat.Data[cur].Mass=dat.Data[cur-1].Mass;
	  dat.Data[cur].Length=dat.Data[cur].Mass/dat.Data[cur].Sh0.Med.Dens;
	 };
   for (k=0;k<=dat.NumPnt;k++) OneInteract(dat,k);
  };
       
void ClcTim(double TimeClc,AutIO &dat)
  {
//   double TimeStp=TimeClc-dat.CurTime;
   do 
     {
      double MinT=MathInf,tmp;
      int MinK=1,k;
      for (k=1;k<=dat.NumPnt;k++)
	    {if ((tmp=min(dat.Data[k].Sh1.Time,dat.Data[k].Sh2.Time))<MinT) 
	        {MinT=tmp;MinK=k;}}
      if (MinT>=TimeClc) {dat.CurTime=TimeClc;return ;}
      else 
        {
         int Fst=MinK;
         if (MinT==dat.Data[MinK].Sh2.Time) Fst--;
		 dat.CurTime=MinT;
         OneInteract(dat,Fst);
         dat.CurTime=MinT;
        };
     }
   while (1);
  };       

int GetNumMatter(int k,AutIO &dat)
  {
   for (int i=0;i<=dat.NumMatter;i++)
//      if (k<dat.BndMatter[i]) return i; - cell_fac var - conversion to new - Bnd[i]-=1
      if (k<=dat.BndMatter[i]) return i;
   return dat.NumMatter+1;
  };   


int AutIO::ReadIni(istream &input)
  {
   InterfaceIODelete();
   char tmp[256];//,tmp1[256];
   int k,NumIntPar=0;
   input>>tmp>>NumPnt;//>>tmp>>CurTime;input.getline(tmp,256);
   input>>tmp>>NumMatter>>tmp;
   if (NumMatter>MaxNumMatter-2) { cout<<" NumMatter "<<NumMatter<<" Is more then MaxNumMat "<<MaxNumMatter<<" exiting\n";exit(0);}
   for (k=1;k<NumMatter;k++) input>>BndMatter[k];
   BndMatter[NumMatter]=NumPnt;
   BndMatter[0]=0;
   input.getline(tmp,256);
   for (k=1;k<=NumMatter;k++) {
       delete Matter[k];
       //if (NULL==(Matter[k]=dynamic_cast<MatterIO*>(SavableClass::SavableClassRegister(input)))) 
	   if (NULL==(Matter[k]=dynamic_cast<MatterIO*>(SavableClass::Read(input)))) 
        { cout<<"RegionIO::ReadIni;  Could not register matter. Exiting.\n";
          for (int i=1;i<k;i++) delete Matter[i];
          exit(1);
         }//exit(1);
   }
   BadPar=1;
   Matter[0]=NULL;Matter[NumMatter+1]=NULL;
   return 1;
  };

void AutIO::SaveIni(ostream &output)
  {
   //char tmp[256];
   int k;
   output<<"NumPnt   "<<NumPnt;//<<"  CurTime   "<<CurTime<<"\n";
   output<<"NumMatters "<<NumMatter<<" Matter_Boundaries ";
   for (k=1;k<NumMatter;k++) output<<BndMatter[k]<<"  ";
   output<<"\n";
   for (k=1;k<=NumMatter;k++) 
       //if (!SavableClass::SavableClassStore(output,Matter[k]) ) 
       if (!SavableClass::Save(output,Matter[k]) ) 
        { cout<<"RegionIO::SaveIni;  Could not save     matter. Exiting.\n";exit(1);}
  }; 

void AutIO::ReadPar(istream &input)
  {
   if ((NumPnt<=1) || (NumPnt>=MaxNumPoints-2)) {cout<<" Bad Number of points in AutIO::ReadPar\n";return ;}
   int i=0,NumR=5;
   char tmp[256];
   Data=new One_Cell[NumPnt+2];
   input>>tmp>>CurTime;seek_eoln(input);
//   input.getline(tmp,255); // "Current Time"
   input.getline(tmp,255); // " Pos Pres...."
cout<<" Reading Param AutIO::ReadPar Num "<<NumPnt<<" Coreleft "<<Coreleft()<<"\n";
   while ( (i<NumPnt) && (!seek_eof(input)) ) { i++;input>>Data[i]; }
   if ((i<NumPnt) || (NumPnt==0))
     {cout<<"RegionIO::ReadIni; Entered only part of points NumPnt i "
          <<NumPnt<<"   "<<i<<"\n";
      if (i!=0) InterfaceIODelete();
	  BadPar=1;
      return ;
      };
   seek_eoln(input);
   input.getline(tmp,255); // " EndIter "
   BadPar=0;
   return ;
  }; 

void AutIO::SavePar(ostream &output)
  {
   //char tmp[256];
   int i=0;
   output<<" CurrentTime "<<CurTime<<"\n";
   output<<"   Pos         Pressure   Dencity    Energy     Velocity   \n";
   while (i<NumPnt)
     {
      i++;
	  SetCurTim(Data[i],CurTime);output<<Data[i]<<"\n";
     }
   output<<"EndIter\n";
  }; 


void AutIO::InterfaceIODelete()
  { 
      delete Data;Data=NULL;BadPar=1;
      for (int k=1;k<=MaxNumMatter;k++) {
          delete Matter[k];Matter[k]=NULL;
      }
  };
