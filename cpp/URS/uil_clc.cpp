#include <lib/precompiled/cell_kru.h>
#include <lib/precompiled/cell_dat.h>


#include "uil_clc.h"

#define U_  WorkBody.Velocity
#define P_  WorkBody.Pressure
#define Ro_ WorkBody.Dencity 
#define M_  WorkBody.Mass    
#define E_  WorkBody.Energy  
#define R_  WorkBody.Pos  
#define F_  WorkBody.Fixed
#define V_  WorkBody.Volume

#define Num WorkBody.NumPnt

#define UilWorkDim WorkBody.UilWorkDim







void ClcMassInBody(Body &WorkBody)
  {
   if (UilWorkDim==1)
     {for(int i=1;i<Num-1;i++)  M_[i]=Ro_[i]*(R_[i+1]-R_[i]);}
   else 
     {for(int i=1;i<Num-1;i++)  
        M_[i]=Ro_[i]*fabs(pow(R_[i+1],UilWorkDim)-pow(R_[i],UilWorkDim))/UilWorkDim;}

   M_[Num-1]=M_[Num-2];

//   R_[Num]=M_[Num-1]/Ro_[Num-1]+R_[Num-1];
//   WorkBody.BoundMU();						            // M_[Num]=0;M_[0]=0;U_[0]=U_[1];U_[Num]=U_[Num-1];
  }; 

void CellFixed(Body &WorkBody)
  {
   int k;
//   WorkBody.BoundP();                 //   P_[0]=10;P_[Num]=10;
   for (k=1;k<=Num;k++) F_[k]=1;
//   if ((fabs(U_[0]-U_[1])>1e-4) || (fabs(P_[0]-P_[1])>11)) {F_[1]=0;F_[2]=0;}
//   if ((fabs(U_[Num]-U_[Num-1])>1e-4) || (fabs(P_[Num]-P_[Num-1])>11)) F_[Num-1]=0;
   F_[Num]=0;
   F_[2]=0;
   F_[1]=0;
   if (UilWorkDim==1)
    {
     for (k=2;k<Num;k++) 
     { if (WorkBody.UnfixAll || (fabs(U_[k]-U_[k+1])>1e-4) || (fabs(P_[k+1]-P_[k])>11))
        { F_[k]=0;F_[k+1]=0;F_[k-1]=0;}}
    }
   else 
    {
     for (k=2;k<Num;k++) 
       { if (WorkBody.UnfixAll || ((fabs(U_[k])+fabs(U_[k+1])>1e-4) || (fabs(P_[k+1]-P_[k])>11)))
        { F_[k]=0;F_[k+1]=0;F_[k-1]=0;}}
    }

   WorkBody.Pres();
//   P_[Num]=P_[Num-1];
  };


void CalcTStpVisc(Body &WorkBody,double TimeStp,double &MinTimeStp,double TimeStability)
  {
   if (Ro_[1]==0) return ;
   int k;
   double Coef,*Sound=new double[Num+3],*L=new double[Num+3],*tmp=new double[Num+3];
   double MinTimeCoef=0;
//   WorkBody.BoundM();						                //M_[Num]=0;M_[0]=0;
   WorkBody.Sound(Sound);
   int BadSize = 0;
//  Next TimeStep Clc
   for (k=1;k<Num-1;k++) if (!F_[k]) 
    {
     tmp[k]=max<double>(fabs(Sound[k]),fabs(U_[k]-U_[k+1]));
     tmp[k]=max<double>(tmp[k],fabs(U_[k-1]-U_[k]));
     if (k>1)
         L[k]=min(R_[k]-R_[k-1],R_[k+1]-R_[k]);
     else
         L[k]=R_[k+1]-R_[k];                    // Chech it may be BUG !!!!!!!!!!!!!
     //if ( (L[k]<sqrt(MathZer)) || (R_[k]<0) ){ 
     if ( (L[k]<sqrt(MathZer)) ){ 
        cout<<" Wrong cell size, negative.\n"<<" k "<<k<<" L[k] "<<L[k]<<" R[k] "<<R_[k]<<"\n";
        BadSize = 1;
         }//exit(0);}
    }
   for (k=1;k<Num-1;k++) 
        if (!F_[k]) { 
//         Coef=Sound[k]/(R_[k+1]-R_[k]);
            Coef=fabs(tmp[k]/L[k]);
            //if ( (1/Coef<sqrt(MathZer)) ){ 
            if ( (Coef>1e7) ){ 
                cout<<" Wrong cell size, negative.\n"<<" k "<<k<<" L[k] "<<L[k]<<" tmp[k] "<<tmp[k]<<" MinTimeCoef "<<Coef<<"\n";
                BadSize = 1;
            }
            if (Coef>MinTimeCoef ) MinTimeCoef=Coef;
        }
    if (BadSize)
        WorkBody.SaveData(cout);
   MinTimeStp=min(MinTimeStp,TimeStability/MinTimeCoef*1e-5);
//  Viscouse addition to pressure
   WorkBody.MullVisc(Sound);
   for (k=1;k<Num;k++) 
      if (!F_[k])
        { 
         double du=U_[k+1]-U_[k];
//if (du<0) du=0;
double r0=R_[Num-1]-R_[Num-2],rc=R_[k]-R_[k-1];
if ((k<2) || (k>Num-1)) rc=r0;
//tmp[k]=Sound[k]*(1+WorkBody.Viscouse2*log(max<double>(1,fabs(du))));
tmp[k]=Sound[k]*(1+WorkBody.Viscouse2*max<double>(1,fabs(du)));
         P_[k]-=Ro_[k]*du*tmp[k]*1e5;
        } 
   delete []Sound;delete []L;delete []tmp;
  };

void ClcAddU(Body &WorkBody,double *AddU)
  {
   int k;
   if (UilWorkDim==1)
    {
     for (k=1;k<=Num;k++) 
        if ( (!F_[k]) && (M_[k]+M_[k-1]>MathZer) ) AddU[k]=2*(P_[k]-P_[k-1])/(M_[k]+M_[k-1]);
        else  AddU[k]=0;
     WorkBody.BoundV(AddU[1],AddU[Num]);		  // AddU[1]*=2;AddU[Num]*=2;
    }

   else 
    {
     double R;
     for (k=1;k<=Num;k++) 
        if ( (!F_[k]) && (M_[k]+M_[k-1]>MathZer) ) 
          {
           if (UilWorkDim==2) R=fabs(R_[k]+R_[k-1])*0.5;
           else R=sqr(R_[k]+R_[k-1])*0.25; // - not so, rough approximation
           AddU[k]=2*(P_[k]-P_[k-1]) *R/
                    (M_[k]+M_[k-1]);}
        else  AddU[k]=0;
     WorkBody.BoundV(AddU[1],AddU[Num]);
    }     

  }; 

void CalcPosE(Body &WorkBody,double TimeStp)
  {
//   WorkBody.BoundPV();						               //P_[0]=10;P_[Num]=10;
// Norm of velocity recalculation
   double *AddU=new double[Num+3];
   ClcAddU(WorkBody,AddU);
// velocity + pos  calculation
   double c=TimeStp*1e5;
   for (int k=1;k<=Num;k++) 
     {
      AddU[k]=TimeStp*AddU[k];
      R_[k]+=c*(U_[k]-AddU[k]*0.5);
      U_[k]-=AddU[k];
     }
//  density + energy calculation
   if (UilWorkDim==1)
    {
     for (int k=1;k<Num;k++) 
       {
        if ((!F_[k]) && (M_[k]>MathZer))
          {
           double Del=1/M_[k];
           double AddL=TimeStp*(U_[k+1]-U_[k]);
           E_[k]-=P_[k]*AddL*Del;
		         Ro_[k]=1/(1/Ro_[k]+AddL*1e5*Del);
          }    
       }
    }
   else 
    {
     for (int k=1;k<Num;k++) 
       {
        if ((!F_[k]) && (M_[k]>MathZer))
          {
           double Del=1/M_[k];
           double AddL=TimeStp*(U_[k+1]  *pow(R_[k+1],UilWorkDim-1)-
                                U_[k]  *pow(R_[k],UilWorkDim-1));
//           double AddL=TimeStp*(U_[k+1]-U_[k]);
           E_[k]-=P_[k]*AddL*Del;
		         Ro_[k]=1/(1/Ro_[k]+AddL*1e5*Del);
          }    
       }
    }
   delete []AddU;
};			









//=========================================================================
//===============   Old Var  ==============================================
//=========================================================================


/*



void ClcMassInBody(Body &WorkBody)
  {
   for(int i=1;i<Num-1;i++)  M_[i]=Ro_[i]*(R_[i+1]-R_[i]);
   M_[Num-1]=M_[Num-2];
//   R_[Num]=M_[Num-1]/Ro_[Num-1]+R_[Num-1];
//   WorkBody.BoundMU();						            // M_[Num]=0;M_[0]=0;U_[0]=U_[1];U_[Num]=U_[Num-1];
  }; 

void CellFixed(Body &WorkBody)
  {
   int k;
//   WorkBody.BoundP();                 //   P_[0]=10;P_[Num]=10;
   for (k=1;k<=Num;k++) F_[k]=1;
//   if ((fabs(U_[0]-U_[1])>1e-4) || (fabs(P_[0]-P_[1])>11)) {F_[1]=0;F_[2]=0;}
//   if ((fabs(U_[Num]-U_[Num-1])>1e-4) || (fabs(P_[Num]-P_[Num-1])>11)) F_[Num-1]=0;
   F_[Num]=0;
   F_[2]=0;
   F_[1]=0;
   for (k=2;k<Num;k++) 
     { if ((fabs(U_[k]-U_[k+1])>1e-4) || (fabs(P_[k+1]-P_[k])>11))
        { F_[k]=0;F_[k+1]=0;F_[k-1]=0;}}
   WorkBody.Pres();
  };


void CalcTStpVisc(Body &WorkBody,double TimeStp,double &MinTimeStp,double TimeStability)
  {
   int k;
   double Coef,*tmp=new double[Num+3];
   MinTimeStp=0;
//   WorkBody.BoundM();						                //M_[Num]=0;M_[0]=0;
   WorkBody.Sound(tmp);
//  Next TimeStep Clc
   for (k=1;k<Num-1;k++) 
      if (!F_[k])
        { Coef=tmp[k]/(R_[k+1]-R_[k]);
          if (Coef>MinTimeStp ) MinTimeStp=Coef;
        } 
   MinTimeStp=TimeStability/MinTimeStp*1e-5;
//  Viscouse addition to pressure
   WorkBody.MullVisc(tmp);
   for (k=1;k<Num;k++) 
      if (!F_[k])
        { 
         double du=U_[k+1]-U_[k];
         P_[k]-=Ro_[k]*du*tmp[k]*1e5;
        } 
   delete tmp;
  };

void ClcAddU(Body &WorkBody,double *AddU)
  {
   int k;
   for (k=1;k<=Num;k++) 
      if ( (!F_[k]) && (M_[k]+M_[k-1]>0) ) AddU[k]=2*(P_[k]-P_[k-1])/(M_[k]+M_[k-1]);
      else  AddU[k]=0;
   WorkBody.BoundV(AddU[1],AddU[Num]);		  // AddU[1]*=2;AddU[Num]*=2;
  }; 

void CalcPosE(Body &WorkBody,double TimeStp)
  {
//   WorkBody.BoundPV();						               //P_[0]=10;P_[Num]=10;
// Norm of velocity recalculation
   double *AddU=new double[Num+3];
   ClcAddU(WorkBody,AddU);
// velocity + pos  calculation
   double c=TimeStp*1e5;
   for (int k=1;k<=Num;k++) 
     {
      AddU[k]=TimeStp*AddU[k];
      R_[k]+=c*(U_[k]-AddU[k]*0.5);
      U_[k]-=AddU[k];
     }
//  density + energy calculation
   for (k=1;k<Num;k++) 
     {
      if ((!F_[k]) && (M_[k]>0))
        {
         double Del=1/M_[k];
         double AddL=TimeStp*(U_[k+1]-U_[k]);
         E_[k]-=P_[k]*AddL*Del;
		       Ro_[k]=1/(1/Ro_[k]+AddL*1e5*Del);
        }    
     }
   delete AddU;
};			
*/