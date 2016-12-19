
//#define MaxD2Points 256
//#include "spline.h"
#include "urs_for.h"
//#include "urmatter.h"
//#include "uradia.h"
//#include "ur_obj.h"
//#include "fstream.h"
//#include "process.h"
//#include <math.h>

#define MaximalPressure 1e8
double MassCoef= 1e-4;

double PressureZer;
double EnergyZer=1e-4;
/*
struct MatterArrStruct
  {
   double Length;
   double LftCoord;
   int  Num;
   double XStp;
   
   double Viscouse1,Viscouse2;
   double CellM;//CellM=Dencity*XStp  CellM=const for all mediu (Bnd?)
   Thermo *Arr;
   double *CellPos;
   friend ostream& operator<<(ostream &output,MatterArrStruct &Pnt);
   friend istream& operator>>(istream &input,MatterArrStruct &Pnt);
  };
*/
ostream& operator<<(ostream &output,MatterArrStruct &Pnt)
  {
   double cur=Pnt.LftCoord+Pnt.XStp/2;
//   output.x_width=10;
   for (int k=1;k<=Pnt.Num;k++)
     { output
//<<setw(13)<<Pnt.CellPos[k]<<"   "<<setw(13)<<Pnt.CellPos[k+1]-Pnt.CellPos[k]
       <<setw(13)<<cur<<"   "
//<<Pnt.Arr[k];
       <<setw(13)<<Pnt.Arr[k].P<<"   "<<setw(13)<<Pnt.Arr[k].E<<"   "
       <<setw(13)<<Pnt.Arr[k].X<<"   "<<setw(13)<<Pnt.Arr[k].U<<"   "
//<<setw(13)<<Pnt.Arr[k].l<<"   "<<setw(13)<<Pnt.Arr[k].dl
       <<"\n";
       cur+=Pnt.XStp; }
   return output;
  };

istream& operator>>(istream &input,MatterArrStruct &Pnt)
  {
   char tmp[256],subst[256];
   Thermo P;
                        
   input>>tmp;
   P.Func=Input(input);seek_eoln(input);
   P.E=EnergyZer;P.X=0;P.U=0;P.P=P.Func->Pressure(P);
   PressureZer=P.P;

   input>>tmp>>Pnt.Length;
   input>>tmp>>Pnt.Num;
   input>>tmp>>P.U;
   input>>tmp>>Pnt.Viscouse1;
   input>>tmp>>Pnt.Viscouse2;

   Pnt.Arr=new Thermo [Pnt.Num+2];
   Pnt.CellPos=new double [Pnt.Num+2];
   Pnt.CellM=P.Func->RConst/Pnt.Num*MassCoef;
   Pnt.XStp=Pnt.Length/Pnt.Num;
   Pnt.LftCoord=0;
   double CurPos=0;
   for (int k=1;k<=Pnt.Num+1;k++)  
     {
      Pnt.Arr[k]=P;
      Pnt.CellPos[k]=CurPos;
      CurPos+=Pnt.XStp;
     }
   return input;
  };
/*
struct MatterSumStruct
  {
   int Num;
   double Time;
   double TimeStab;
   double EndTime;
   double WriteTime;

   MatterArrStruct *Matter;
   friend ostream& operator<<(ostream &output,MatterSumStruct &Sum);
   friend istream& operator>>(istream &input,MatterSumStruct &Sum);
  };
*/
ostream& operator<<(ostream &output,MatterSumStruct &Sum)
  {
   double tmp=fmod(Sum.Time,Sum.WriteTime)/Sum.WriteTime;
   if ( (tmp<1e-10) || (1-tmp<1e-10) )
     { output<<" Time : "<<Sum.Time<<"\n";
       for (int k=1;k<=Sum.Num;k++) output<<Sum.Matter[k]; }
   return output;
  };

void SetNewVel(int MatterNum,double VelBefore,double VelAfter,
                                                   MatterSumStruct &Sum)
  {
   double CurVel=Sum.Matter[MatterNum].Arr[3].U;
   Sum.Matter[MatterNum].Arr[1].U=(VelBefore+0.5*(CurVel-VelBefore));
   Sum.Matter[MatterNum].Arr[2].U=(VelBefore+0.75*(CurVel-VelBefore));
   Sum.Matter[MatterNum].Arr[Sum.Matter[MatterNum].Num].U=
                                  (CurVel+0.25*(VelAfter-CurVel));
   Sum.Matter[MatterNum].Arr[Sum.Matter[MatterNum].Num+1].U=
                                  (CurVel+0.5*(VelAfter-CurVel));
  };
void FormBoundaries(MatterSumStruct &Sum)
  {
   SetNewVel(1,Sum.Matter[1].Arr[3].U,Sum.Matter[2].Arr[3].U,Sum);
   for (int k=2;k<=Sum.Num-1;k++) 
     SetNewVel(k,Sum.Matter[k-1].Arr[3].U,Sum.Matter[k+1].Arr[3].U,Sum);
   SetNewVel(Sum.Num,Sum.Matter[Sum.Num-1].Arr[3].U,
                                   Sum.Matter[Sum.Num].Arr[3].U,Sum);
  };
istream& operator>>(istream &input,MatterSumStruct &Sum)
  {
   double CurCoord=0;
   char tmp[256];
   input>>tmp>>Sum.Num;
   Sum.Matter=new MatterArrStruct[Sum.Num+1];
   input>>tmp>>Sum.EndTime;            
   input>>tmp>>Sum.TimeStab;//Coef stab
   input>>tmp>>Sum.WriteTime;
   input>>tmp>>EnergyZer;
   input>>tmp>>MassCoef;
   Sum.Time=0;
   for (int k=1;k<=Sum.Num;k++)
     {
      input>>Sum.Matter[k];
      Sum.Matter[k].LftCoord=CurCoord;
      CurCoord+=Sum.Matter[k].Length;
      if (!input) {cout<<"Cannot read input Sum parameters !!!!/n";exit(0);}
     }
   FormBoundaries(Sum);
   return input;
  };
/*
struct IntegralConst
  {
   int First;
   Thermo *LastValues;
   Thermo *DeltaValues;
   IntegralConst(){First=1;};
   ~IntegralConst(){if (!First) {delete LastValues;delete NewValues;};
   void Control(MatterSumStruct &Sum);
   void Write(MatterSumStruct &Sum,ostream &out);
  };
*/
void IntegralConst::Write(MatterSumStruct &Sum,ostream &out)
  {
   Thermo LastFull(0,0,0,0,NULL);
   for (int k=1;k<=Sum.Num;k++)
      out<<" Matter ["<<k<<"]  Impuls = "<<setw(13)<<LastValues[k].U
                         <<"  DImpuls = "<<setw(13)<<DeltaValues[k].U
                         <<"   Energy = "<<setw(13)<<LastValues[k].E
                         <<"  DEnergy = "<<setw(13)<<DeltaValues[k].E<<"\n";
   out<<"\n Full E=  "<<setw(13)<<LastValues[0].E
        <<" Full dE= "<<setw(13)<<DeltaValues[0].E
        <<" Full I = "<<setw(13)<<LastValues[0].U
        <<" Full dI= "<<setw(13)<<DeltaValues[0].U<<"\n";
  };

void IntegralConst::Control(MatterSumStruct &Sum)
  {
   if (First) { LastValues=new Thermo[Sum.Num+1];
                DeltaValues=new Thermo[Sum.Num+1];}
   Thermo *NewValues=new Thermo[Sum.Num+1];
   NewValues[0]=SetThermo(0,0,0,0,NULL);
   MatterArrStruct *Pnt;
   for (int k=1;k<=Sum.Num;k++)
     {
      Pnt=&Sum.Matter[k];
      NewValues[k]=SetThermo(0,0,0,0,NULL);
      double FullEnergy=0,FullImpuls=0;
      for (int k1=1;k1<=Pnt->Num;k1++)
        {
         FullEnergy+=Pnt->Arr[k1].E+sqr(Pnt->Arr[k1].U)/2;
         FullImpuls+=Pnt->Arr[k1].U;
//cout<<k1;
        };
cout<<"\n"<<FullEnergy<<"  "<<FullImpuls<<"\n";
      NewValues[k].E=FullEnergy;//*Pnt->CellM;
      NewValues[k].U=FullImpuls;//*Pnt->CellM;
      if (First) DeltaValues[k]=SetThermo(0,0,0,0,NULL);
      else 
        {
         DeltaValues[k].E=NewValues[k].E-LastValues[k].E;
         DeltaValues[k].U=NewValues[k].U-LastValues[k].U;
        }
       NewValues[0].E+=NewValues[k].E;
       NewValues[0].U+=NewValues[k].U;
     }
   if (First) DeltaValues[0]=SetThermo(0,0,0,0,NULL);
   else
     {
      DeltaValues[0].E=NewValues[0].E-LastValues[0].E;
      DeltaValues[0].U=NewValues[0].U-LastValues[0].U;
     }
   delete LastValues;
   LastValues=NewValues;
   First=0;
  };

IntegralConst Integral;

// ===================== Main Body Begin ==================

double TetaFunction(double x) { if (x<0) return 0;return 1; };

void UBnd(MatterArrStruct &Pnt1,MatterArrStruct &Pnt2,int First,double TStp)
  {

   if (First)
     {
      Pnt1.Arr[1].U-=2*(Pnt1.Arr[1].P-PressureZer)*TStp/Pnt1.CellM;
      Pnt1.Arr[Pnt1.Num+1].U-=(Pnt2.Arr[1].P-Pnt1.Arr[Pnt1.Num].P)*TStp/
                                              (0.5*(Pnt2.CellM+Pnt1.CellM));
     }
   else
     {
      Pnt2.Arr[Pnt2.Num+1].U+=2*(Pnt2.Arr[Pnt2.Num].P-PressureZer)*TStp/Pnt2.CellM;
      Pnt2.Arr[1].U=Pnt1.Arr[Pnt1.Num+1].U;
     }
  };
void UBnd(MatterArrStruct &Pnt1,MatterArrStruct &Pnt2,
                                   MatterArrStruct &Pnt3,double TStp)
  {
   Pnt2.Arr[1].U=Pnt1.Arr[Pnt1.Num+1].U;
   Pnt2.Arr[Pnt2.Num+1].U-=(Pnt3.Arr[1].P-Pnt2.Arr[Pnt2.Num].P)*TStp/
                                              (0.5*(Pnt2.CellM+Pnt1.CellM));
  };

void UArray(MatterArrStruct &Pnt,double TStp)
  {
   int k;
   double Coef=TStp/Pnt.CellM;
   for (k=2;k<=Pnt.Num;k++)
      Pnt.Arr[k].U-=(Pnt.Arr[k].P-Pnt.Arr[k-1].P)*Coef;

//   for (k=1;k<=Pnt.Num+1;k++)  Pnt.CellPos[k]+=Pnt.TStp*Pnt.Arr[k].U;
  };
double Volume(Thermo Pnt,double CellM)
  {
   return (1-Pnt.X)/Pnt.Func->RConst;
  };
/*
double Pres(Thermo Pnt)
  {
   Matter m=MatTab.GetMatter(Pnt.Type);
   return Pcold(Pnt)*(1-0.5*m.Grun(Pnt.X)*Pnt.X/(1-Pnt.X));
  };
/*
Thermo NewThermo(Thermo Pnt,double Length,double DeltaLength,double CellM)
  {
/*
   Pnt.Arr[k].X=1-(Length+DeltaLength  )/Length*(1-Pnt.Arr[k].X);
   double OldE=Pnt.Arr[k].E;
   Pnt.Arr[k].E=(Pnt.Arr[k].E*Length-Pnt.Arr[k].P*DeltaLength)
                                            /(Length+DeltaLength);
   double BadPressure=Pressure(Pnt.Arr[k]);
   Pnt.Arr[k].E=(OldE*Length-(Pnt.Arr[k].P+BadPressure)/2*DeltaLength)
                                            /(Length+DeltaLength);
   Pnt.Arr[k].P=Pressure(Pnt.Arr[k]);
/
   
   Matter m=MatTab.GetMatter(Pnt.Type);
   Thermo ret=Pnt;
   ret.X=1-(Length+DeltaLength*0.5)/Length*(1-Pnt.X);
   double Pmean=Pres(ret);
   ret.X=1-(1+DeltaLength/Length)*(1-Pnt.X);
   double V0=Volume(Pnt,CellM),V1=Volume(ret,CellM);
   double s=0.5*m.Grun(Pnt.X)*(V1-V0)/V0;
   ret.E=(Pnt.E*(1-s)-Pmean*(V1-V0))/(1+s);
   ret.P=Pres(ret)+m.Grun(Pnt.X)*ret.E/V1;
   return ret;
  }
*/
Thermo NewThermo(Thermo Pnt,double Length,double DeltaLength,double CellM)
  {
   
   Thermo ret=Pnt;
   ret.X=1-(Length+DeltaLength*0.5)/Length*(1-Pnt.X);
   double Pmean=ret.Func->Pressure(ret);
   ret.X=1-(Length+DeltaLength)/Length*(1-Pnt.X);
/*
   double Dv=(Pnt.X-ret.X)/Pnt.Func->RConst;
   ret.E-=Pmean*Dv;
   ret.P=ret.Func->Pressure(ret);
*/
   double Dv=(Pnt.X-ret.X)/Pnt.Func->RConst;
   UrMatGas *Gas=(UrMatGas *)Pnt.Func;
   double Gamma=Gas->Gamma;
   ret.E*=pow((Length+DeltaLength)/Length,1-Gamma);
   ret.P=ret.Func->Pressure(ret);
cout<<"G "<<Gamma<<" DE= "<<ret.E-Pnt.E<<"  DDE  "
    <<ret.E-Pnt.E+0.5*(ret.P+Pnt.P)*Dv<<"\n";
   return ret;
  };
void PEArray(MatterArrStruct &Pnt,double TStp)
  {
   int k;
   double Length,DeltaLength;
   double AConst=2;
   for (k=1;k<=Pnt.Num;k++)
     {
      Length=Pnt.CellPos[k+1]-Pnt.CellPos[k];
      Pnt.CellPos[k]+=TStp*Pnt.Arr[k].U;
      DeltaLength=(Pnt.Arr[k+1].U-Pnt.Arr[k].U)*TStp;
//      Pnt.Arr[k].l=Length;Pnt.Arr[k].dl=DeltaLength;
      Pnt.Arr[k]=NewThermo(Pnt.Arr[k],Length,DeltaLength,Pnt.CellM);
      double PViscous=Pnt.Viscouse1*TetaFunction(-DeltaLength)
                                   *sqr(DeltaLength)/Length+
                      Pnt.Viscouse2*TetaFunction(DeltaLength)
                                   *sqr(DeltaLength)*AConst
                                   *fabs(Pnt.Arr[k].P/MaximalPressure)
                                   /Length;
      Pnt.Arr[k].P+=PViscous;
     }
   Pnt.CellPos[Pnt.Num+1]+=TStp*Pnt.Arr[Pnt.Num+1].U;
  };

double ClcTimeStp(MatterSumStruct &Sum)
  {
   double stp=0,getstp;
   MatterArrStruct *Pnt;
//   stp=Sum.Matter[1].XStp*Sum.Matter[1].XStp;
   for (int k0=1;k0<=Sum.Num;k0++) 
     {
      Pnt=&Sum.Matter[k0];
      for (int k=1;k<=Pnt->Num;k++)
         stp=max<double>(stp,fabs(Pnt->Arr[k+1].U-Pnt->Arr[k].U)/
                     (Pnt->CellPos[k+1]-Pnt->CellPos[k]));
     }
   stp=1/(Sum.TimeStab*stp);getstp=stp;
//   stp=Sum.TimeStab;
   double tmp=Sum.WriteTime-fmod(Sum.Time,Sum.WriteTime);
   if (tmp<stp)
     {     
      stp=(floor(Sum.Time/Sum.WriteTime)+1)*Sum.WriteTime-Sum.Time;
     }
   if (stp/Sum.WriteTime<1e-10) stp=Sum.WriteTime;
   stp=min(stp,getstp);
   Sum.Time+=stp;
cout<<" Time "<<Sum.Time<<" Stp "<<stp<<" tmp "<<tmp<<"\n";
   
   return stp;
  };

int ClcUrsStp(MatterSumStruct &Sum,fstream &out)
  {
   double TStp=ClcTimeStp(Sum);
//out<<Sum;
   for (int k=1;k<=Sum.Num;k++)
     {
      if (k==1) UBnd(Sum.Matter[k],Sum.Matter[k+1],1,TStp);
      else if (k==Sum.Num) UBnd(Sum.Matter[k-1],Sum.Matter[k],0,TStp);
        else UBnd(Sum.Matter[k-1],Sum.Matter[k],Sum.Matter[k+1],TStp);
      UArray(Sum.Matter[k],TStp);
     }
//out<<Sum;
   for (k=1;k<=Sum.Num;k++) PEArray(Sum.Matter[k],TStp);
//out<<Sum;
   if (Sum.Time>Sum.EndTime) return 0;
   return 1;
  };


// ========================Input Output ===========================

int ControlArgv(int argc,char *argv[])
  {
   if (argc!=3) { cout<<"Wrong Number of Parameters/n";return 0; }
   return 1;
  };
// ===============Main =================
void main(int argc,char *argv[])
  {
   MatterSumStruct Sum;
//   D2Array Midue1, Midue2;
   if ( !ControlArgv(argc,argv) ) exit(0);
   fstream in(argv[1],ios::in);
   in>>Sum;
   fstream out(argv[2],ios::out);
   while ( ClcUrsStp(Sum,out) )  
     {
      Integral.Control(Sum);Integral.Write(Sum,out);
      out<<Sum;
     }
  };

/*
  I1=60
		I11=I1-1
		DM=H1*R1/I11
		TAU1=DM/(R1*D1*5.)
  TAU2=DM/(R2*D2*5.)
  TAU=AMIN1(TAU1,TAU2)
  KEY=0
		JT=0

  IMI=1
		TRH=TAU/DM
        JM=100
        WRITE_I=10
        DO 100 J=1,JM
		T=T+TAU
		IF(P(I11).LT.-5.E+08)IMI=I1
		IMI1=IMI+1      
              DO 99 I=IMI1,I31
 99           U(I)=U(I)-(P(I)-P(I-1))*TRH
C   BOUNDARY CONDITIONS (LEFT)
             U(IMI)=U(IMI)-2.*P(IMI)*TRH
C   BOUNDARY CONDITIONS (RIGHT)
              U(I3)=U(I3)+2.*P(I31)*TRH
C
                IF(KEY.EQ.1)GOTO98
        IF(ABS(U(I3)).LT.0.1)GO TO 201
		IF(JT.EQ.0) JT=J-1
  98           	UF(J-JT)=U(I3)
		TF(J-JT)=TAU*(J-JT-1)*1.E+09
		NF=NF+1
                IF(WMAX.LT.U(I3))WMAX=U(I3)
                KEY=1
  201 	CONTINUE 
				DO 400 I=IMI,I31
                                G=G1
                                IF(I.GE.I1)G=G2
                                IF(I.GE.I2)G=G3
				DU=U(I+1)-U(I)
				DV=DU*TRH
				V(I)=V(I)+DV
				VS=V(I)-0.5*DV
				AS=A(VS,I)
				S=0.5*G*DV/VS
				E(I)=(E(I)*(1.-S)-AS*DV)/(1.+S)
				P(I)=A(V(I),I)+G*E(I)/V(I)
				PVIS=VIS*(DU-ABS(DU))*DU/V(I)
     ++VIS1*(ABS(DU)-DU)*C1*ABS(P(I)/PMAX)/V(I)
      write(31,1) X(I),P(I),E(I)
  400           P(I)=P(I)+PVIS
                if(J-J/ICOUNT*ICOUNT.ne.0)GOTO 100
c                CALL lineo(X,P,i31)
  100  CONTINUE                          


      FUNCTION A(X,I)
3  R=R3
	  C=C3
	  B=B3
   G=G3
1	  Y=1.-R*X
	  A=R*C**2*Y*(1.-0.5*G*Y/(R*X))/(1.-B*Y)**2
	  RETURN
	  END	  
*/
