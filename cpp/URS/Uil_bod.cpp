#include <lib\precompiled\cell_kru.h>
#include <lib\precompiled\cell_dat.h>

#include "uil_bod.h"

int Body::ReadData(istream &input)
{
   if (NumPnt==0) return 0;
   int i=0,NumR=5+NumIntPar;
   double *d=new double [7+NumIntPar];
   while ((!seek_eof(input)) && (EnterStr(input,&d[0],NumR+1,1)==0)) ;
   if (seek_eof(input))
       throw info_except("No points\n");
   do
        {                    
         i++;
         Pos     [i]=d[1];
         Pressure[i]=d[2]*1e5;
         Dencity [i]=d[3];
         Velocity[i]=d[5];
         Energy  [i]=d[4];
		 if (NumIntPar>0)
             for(int k=0;k<NumIntPar;k++)
                 IntPar[k][i] = d[6+k];
//             MemoryMove(&d[6],IntPar[i],sizeof(double)*NumIntPar);
        }
   while ( (i<NumPnt-1) && (!seek_eof(input)) && 
			                      (EnterStr(input,&d[0],NumR+1,1)==NumR) );
   delete d;
   if ((i<NumPnt-1) || (NumPnt==0))
     {cout<<"Body::ReadData; Entered only part of points NumPnt i "
          <<NumPnt<<"   "<<i<<"\n";
//      if (i!=0) DeleteBody();
      return 0;
      }
//   ExtendBnd();
   return 1;
};
void Body::SaveData(ostream &output)
{
   int i=0;
   double *d=new double [7+NumIntPar];
//   output<<"   Pos         Pressure   Dencity    Energy     Velocity   \n";
   while (i<NumPnt-1){
        i++;
        d[1]=Pos     [i];
        d[2]=Pressure[i]*1e-5;
        d[3]=Dencity [i];
        d[5]=Velocity[i];
        d[4]=Energy  [i];
	    if (NumIntPar>0)
            for(int k=0;k<NumIntPar;k++)
                d[6+k] = IntPar[k][i];
          
//        if (NumIntPar>0) MemoryMove(IntPar[i],&d[6],sizeof(double)*NumIntPar);

        WriteStr(output,&d[0],5+NumIntPar);
     }
   delete d;
  }; 





void Body::Pres()
  {  
   int l,b,k;
   b=0;l=NumPnt;
//#ifdef InternalPointsUrs
   if (Matter->NumInputVal()!=2) {
       Matter->StoreInputVal(InputPar);
	   Matter->Pressure(Pressure,Dencity,Energy,NumPnt-1);
   }else {
//#else
        if (FstCalcP)  Matter->Pressure(Pressure,Dencity,Energy,NumPnt-1);
        else {
            double  *TP=TmpP,*TD=TmpD,*TE=TmpE;
            int n=0,o;
            for (o=1;o<NumPnt;o++) 
            {if (!Fixed[o]) {TD[n]=Dencity[o];TE[n]=Energy[o];n++;}}
            Matter->Pressure(&TP[-1],&TD[-1],&TE[-1],n);
            n=0;
            for (o=b+1;o<b+l;o++) 
                if (!Fixed[o]) {Pressure[o]=TP[n];n++;} 
			    else Pressure[o]=PresL[o];
        }
   }
//#endif
//     if (LftFree) { Pressure[0]=0;Energy[0]=0;}
//     if (RgtFree) { Pressure[NumPnt]=0;Energy[NumPnt]=0;}
   MemoryMove(Pressure,PresL,sizeof(double)*(NumPnt+2));FstCalcP=0;
   for ( k=1;k<NumPnt;k++) Pressure[k]=max(Pressure[k]*1e5,10);

  };

void Body::Sound(double *S)
{  
//#ifdef InternalPointsUrs
   if (Matter->NumInputVal()!=2) {
       Matter->StoreInputVal(InputPar);
	   Matter->Sound(S,Dencity,Energy,NumPnt-1);
   }else {
//#else
        if (FstCalcS) Matter->Sound(S,Dencity,Energy,NumPnt-1);
        else {
            if (IterCalcS++<NumSkipS) 
                {MemoryMove(SounL,S,sizeof(double)*(NumPnt+2));return ;}
            IterCalcS=0;

            double  *TS=TmpP,*TD=TmpD,*TE=TmpE;
            int n=0,o;
            for (o=1;o<NumPnt;o++) 
                if (!Fixed[o]) {TD[n]=Dencity[o];TE[n]=Energy[o];n++;}
            Matter->Sound(&TS[-1],&TD[-1],&TE[-1],n);
            n=0;
            for (o=1;o<NumPnt;o++) if (!Fixed[o]) {S[o]=TS[n];n++;} else S[o]=SounL[o];
        }
    }
//#endif
  MemoryMove(S,SounL,sizeof(double)*(NumPnt+2));FstCalcS=0;
};			

void Body::TimeCh(double TimeStp)
  {  
//#ifdef InternalPointsUrs
   if (Matter->NumInputVal()!=2) {
       Matter->StoreInputVal(InputPar);
	   Matter->ChangeInputVal(Dencity,Energy,NumPnt-1,TimeStp);
    }
//#endif
};

void Body::MullVisc(double *tmp)
  {
   double c=Viscouse;
   for (int i=1;i<=NumPnt;i++) if (tmp[i]<0.1) tmp[i]=0.1*c;else 
                                                                 tmp[i]*=c;
  };			

void Body::DeleteBody()
 { 
  if (NumPnt!=0)
    {
     delete Pressure;Pressure=NULL;delete Energy  ;Energy  =NULL;
     delete Dencity ;Dencity =NULL;delete Velocity;Velocity=NULL;
     delete Pos     ;Pos     =NULL;
     delete Mass    ;Mass    =NULL;
     delete PresL   ;PresL   =NULL;delete SounL   ;SounL   =NULL;
     delete Fixed   ;Fixed   =NULL;
//     delete Volume  ;Volume  =NULL;
     delete TmpE;TmpE=NULL;delete TmpP;TmpP=NULL;delete TmpD;TmpD=NULL;
	    //if (NumIntPar>0) //IntPar=new double[NumPnt+2][NumIntPar];
     //  {for (k=0;k<=NumPnt+1;k++) delete IntPar[k];delete IntPar;}
//     delete Matter;Matter=NULL;
     NumIntPar=0;
   	 NumPnt=0;
    }
 };

//void Body::Create(int NPnt,int NIntPar,MatterIO *Mat,double Vis,double Vis2)
void Body::Create(int NPnt,MatterIO *Mat,double Vis,double Vis2,const DataVector<Stroka> &IntParNames){
    DeleteBody();
    if (NPnt==0) return ;
    NumPnt=NPnt;NumIntPar=(int)IntParNames.SetDim();Matter=Mat;
    Viscouse=Vis;Viscouse2=Vis2;
    if (NumIntPar>0) {
  	    //IntPar=new double*[NumPnt+2];
       // for (int k=0;k<=NumPnt+1;k++) IntPar[k]=new double[NumIntPar];
        IntPar.SetDim(NumIntPar);
        for(int k=0;k<NumIntPar;k++){
            IntPar[k].SetDim(NumPnt+2);
            InputPar[IntParNames[k]] = (double*)IntPar[k];
            //InputPar[IntParNames[k]]--;
        }
        Mat->StoreInputVal(InputPar);
	}	

    Pos     =new double[NumPnt+2];
    Pressure=new double[NumPnt+2];
    Dencity =new double[NumPnt+2];
    Energy  =new double[NumPnt+2];
    Velocity=new double[NumPnt+2];
    Mass    =new double[NumPnt+2];
//  Volume  =new double[NumPnt+2];

    PresL =new double[NumPnt+2];
    SounL =new double[NumPnt+2];
    TmpE  =new double[NumPnt+2];
    TmpP  =new double[NumPnt+2];
    TmpD  =new double[NumPnt+2];
    Fixed   =new int[NumPnt+2];
    FstCalcP=1;FstCalcS=1;
 };