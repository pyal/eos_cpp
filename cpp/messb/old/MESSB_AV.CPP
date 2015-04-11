#include "messb_av.h"
//#include <malloc.h>

//#define Pi M_PI
//3.14159265358979

VecCl FormHVec(double H,double H_zx,double H_xy)
  {
   VecCl Ret(3);
   double Teta=H_zx*M_PI/180,Phi=H_xy*M_PI/180;
   double M=H*sin(Teta);
   Ret[1]=H*cos(Teta);
   Ret[2]=M*cos(Phi);
   Ret[3]=M*sin(Phi);
   return Ret;
  }; 
void FormHPhi(VecCl &Hi,double &Hs,double &Hs_zx,double &Hs_xy)
  {
   Hs=sqrt(Hi*Hi);      if (Hs<StndErr)    {Hs_zx=0;Hs_xy=0;return ;}
   Hs_zx=acos(Hi[1]/Hs);                                    
   double H_xy=sqrt(sqr(Hi[2])+sqr(Hi[3]));
   if (H_xy<StndErr) {Hs_xy=0;Hs_zx*=180/M_PI; return ;}
   Hs_xy=acos(Hi[2]/H_xy);
//   if (Hi[2]<0) Hs_xy=M_PI-Hs_xy; 
   if (Hi[3]<0) { Hs_zx=2*M_PI-Hs_zx;Hs_xy=M_PI-Hs_xy; }
   Hs_xy*=180/M_PI;Hs_zx*=180/M_PI;
//   VecCl Res=FormHVec(Hs,Hs_zx,Hs_xy);
//cout<<" HI "<<Hi<<" Res "<<Res-Hi;
  }; 
VecCl MulVec(VecCl &X,VecCl &Y)
  {
   VecCl Res(3);                           
   Res[1]=X[2]*Y[3]-X[3]*Y[2];
   Res[2]=X[3]*Y[1]-X[1]*Y[3];
   Res[3]=X[1]*Y[2]-X[2]*Y[1];
   return Res;
  }; 
void FormHSum(double H,double H_zx,double H_xy,double I_Hx,double I_Hy,
              double I_Hz,double Hc,double Q_teta,double Q_phi,double Teta_V,
              int Add,int FineAver,
              double &Hs,double &Q_zx,double &Q_xy,VecCl &Teta_Varr,
              VecCl &Phi_Varr)
  {
//  1  - z; 2 - x; 3 - y
//  Qs - bazis for I_H; find Hi - vector of internal Hc + Hext
//cout<<" FormHSum ";
   double Hs_zx,Hs_xy;
   VecCl Hext(3);Hext=FormHVec(H,H_zx,H_xy);
   VecCl Hi(3);
   if (Add=1) 
	{Hi[1]=I_Hz;Hi[2]=I_Hx;Hi[3]=I_Hy;Hi=Hi+Hext;}
   else {Hi[1]=I_Hz*Hext[1];Hi[2]=I_Hx*Hext[2];Hi[3]=I_Hy*Hext[3];}
//   for (int k=1;k<=3;k++) if (Hext[k]<0) Hi[k]=-Hi[k];
   Hs=sqrt(Hi*Hi);Hi=Hi*(Hc/Hs)+Hext;
//cout<<" Teta "<<H_zx<<" Phi "<<H_xy<<"\n";
//cout<<" Hi "<<Hi<<" Hext "<<Hext;
// Find View in Hi basis. X axis in the plane z & Hi;
   VecCl Z(3),HZ(3),HX(3),HY(3);Z=FormHVec(1,0,0);Hs=sqrt(Hi*Hi);
if (Hs<StndErr) {Q_zx=0;Q_xy=0;cout<<" very bad. Hi==0;\n";return ;}
   HZ=Hi*(1/Hs);HX=MulVec(HZ,Z);Hs=sqrt(HX*HX);
if (Hs<StndErr) {Q_zx=0;Q_xy=0;cout<<" very bad. HX==0;\n";return ;}
   HX=HX*(1/Hs);HY=MulVec(HZ,HX);Hs=sqrt(HY*HY);

//   HZ=Hi*(1/Hs);HY=MulVec(HZ,Z);Hs=sqrt(HY*HY);
//if (Hs<StndErr) {Hs_zx=0;Hs_xy=0;cout<<" very bad. HX==0;\n";return ;}
//   HY=HY*(1/Hs);HX=MulVec(HY,HZ);Hs=sqrt(HX*HX);

if (fabs(Hs-1)>StndErr) {cout<<" very bad. HY!=1;\n";return ;}
// Form Teta_Varr, First for Phi=0; now for Teta_V=0
//   double Phi_Arr[5]={4,0,90,180,270};
   VecCl V0(3);V0=FormHVec(1,H_zx-Teta_V,H_xy);
//cout<<" V0 "<<V0;
   VecCl Res(3);Res[1]=V0*HZ;Res[2]=V0*HX;Res[3]=V0*HY; // V0 - in Hi coord
//   double Teta,Phi;
//cout<<" Res "<<Res;
   FormHPhi(Res,Hs,Hs_zx,Hs_xy); // In Hi coordinates - V0 angles
//cout<<" Mod "<<Hs<<" Teta "<<Hs_zx<<" Phi "<<Hs_xy<<"\n";
//Res=FormHVec(Hs,Hs_zx,Hs_xy);cout<<" Res "<<Res<<"\n";

   Teta_Varr.SetDim(1);Teta_Varr[1]=Hs_zx;
   Phi_Varr.SetDim(1); Phi_Varr[1]=Hs_xy;
//   Teta_Varr.SetDim(2);Teta_Varr[1]=0;Teta_Varr[2]=180;
//   Phi_Varr.SetDim(2); Phi_Varr[1]=0;Phi_Varr[2]=0;

//   Res[1]=Z*HX;Res[2]=Z*HY;Res[3]=Z*HZ; // Z_Qs - in Hi coord

   FormHPhi(HZ,Hs,Hs_zx,Hs_xy);Q_zx=-Hs_zx;
   FormHPhi(HX,Hs,Hs_zx,Hs_xy);Q_xy=-Hs_xy;
//cout<<" Contr Teta=90 ? "<<Hs_zx<<" Phi "<<Hs_xy<<"\n";
//cout<<Hi;
//   FormHPhi(Hi,Hs,Hs_zx,Hs_xy); // In Hi coordinates - angles
//   Q_zx=-Hs_zx;Q_xy=-Hs_xy;

//while (Q_zx>90) {Q_zx-=180;Q_xy=90-Q_xy;cout<<"I";};
//while (Q_zx<-90)  {Q_zx+=180;Q_xy=90-Q_xy;cout<<"I";};
//while (Q_zx>90) {Q_zx-=180;cout<<"I";};while (Q_zx<-90)  {Q_zx+=180;cout<<"I";};
//while (Q_xy>90) Q_xy-=180;while (Q_xy<-90) Q_xy+=180;
//while (Q_xy>90) Q_xy-=90;while (Q_xy<0) Q_xy+=90;

//cout<<" H_zx "<<H_zx<<" H_xy "<<H_xy<<" Q_zx "<<Q_zx<<" Q_xy "<<Q_xy<<" Teta_Varr[1] "<<Teta_Varr[1]<<" Phi_Varr[1] "<<Phi_Varr[1]<<"\n";

/*    Tryed to include Qs_Teta - Qs_Phi turn. Bad Attempt
   V0=FormHVec(1,Q_teta,Q_phi);Res[1]=V0*HZ;Res[2]=V0*HX;Res[3]=V0*HY; //Z_Qs
   FormHPhi(Res,Hs,Hs_zx,Hs_xy); // In Hi coordinates - angles 
   Q_zx=-Hs_zx;
   V0=FormHVec(1,Q_teta+90,Q_phi);Res[1]=V0*HZ;Res[2]=V0*HX;Res[3]=V0*HY;//X_Qs
   FormHPhi(Res,Hs,Hs_zx,Hs_xy); // In Hi coordinates - angles
   Q_xy=Hs_xy;
FormHPhi(Hi,Hs,Hs_zx,Hs_xy);
cout<<" Q_zx "<<Q_zx<<" Q_xy "<<Q_xy<<" Hs_zx "<<Hs_zx<<" Hs_xy "<<Hs_xy<<"\n";
*/
   Hs=sqrt(Hi*Hi);
  }; 
void HQsAmpVecMonoAver(CMatrCl &Hamilt,VecCl &Teta_Varr,VecCl &Phi_Varr,
                       double H,double IS,double Wid,int Add,
                       double *x,double *y)
  {
   CMatrCl EigenVect(4),EigenVect1(4);
   double EigenVal[5],Intencity[9],Position[9];
   CVecCl EVal;
   int Ret=1,k,i;
   EigenVal[0]=4;Intencity[0]=8;Position[9]=0;
   URComeig(Hamilt,EigenVect1,EigenVect);          
   for (i=1;i<=4;i++) EigenVal[i]=my_real(Hamilt(i,i));EigenVect=Transpon(EigenVect);
//   int Ret=CReduceLQ(Hamilt,EigenVect,EigenVal,StndErr);
   double SumI[9];for (i=1;i<=8;i++) SumI[i]=0;
   VecCl Amp2(4);
   
//   double Az[5]={4,0,90,180,270};
//   double Ax[3]={2,0,90};
//if (Add!=3) {Ax[0]=1;Az[0]=1; }
//   for (int az=1;az<=Az[0];az++)
//     {
//      for (int ax=1;ax<=Ax[0];ax++)
//        {

   for (i=1;i<=Teta_Varr.Dim();i++) 
     {
      double TetaH_View=Teta_Varr[i]//+Az[az]
         ,PhiH_View=Phi_Varr[i]//+Ax[ax]
                               ;
      double Cos=cos(TetaH_View*M_PI/180),Sin=sin(TetaH_View*M_PI/180);
      double Cos2=sqr(Cos);
      Amp2[1]=(1+Cos2)*0.75;Amp2[2]=1.-Cos2;Amp2[3]=(1+Cos2)*0.25;Amp2[4]=0;
      IntPos(EigenVect,EigenVal,Intencity,Position,H,IS,Amp2);
//  Add interference 

      my_comp e1=my_comp(cos(PhiH_View*M_PI/180),sin(PhiH_View*M_PI/180)),e2=e1*e1;
      my_comp C1=sqrt(3.)*Sin*Cos,C2=0.5*sqrt(3.)*sqr(Sin),C3=C1/sqrt(3.);
      for (k=1;k<=4;k++)
        {
      Intencity[k]+=my_real(   C1*e1*conj(EigenVect(1,k))*EigenVect(2,k)+
                            C2*e2*conj(EigenVect(1,k))*EigenVect(3,k)-
                            C3*e1*conj(EigenVect(2,k))*EigenVect(3,k));
      Intencity[k+4]+=my_real(-C1*conj(e1*EigenVect(4,k))*EigenVect(3,k)+
                            C2*conj(e2*EigenVect(4,k))*EigenVect(2,k)+
                            C3*conj(e1*EigenVect(3,k))*EigenVect(2,k));
        }
      for (k=1;k<=8;k++) 
        {if (Intencity[k]<0) cout<<" Monocryst. Negative.\n";
         SumI[k]+=Intencity[k];}
     }; 
//        }};

//   delete EigenVal;
   for (k=1;k<=8;k++) Intencity[k]=SumI[k];
   HQsInt(Intencity,Position,Wid,x,y);
//   return Ret;
  };
   
void FormOrt(double Q_zx,double Q_xy,double &O_zx,double &O_xy)
  {
   VecCl Q(3);Q=FormHVec(1,Q_zx,Q_xy);
   VecCl X(3);X[1]=0;X[2]=1;X[3]=0;
   VecCl A(3);A=MulVec(X,Q);
   double Norm=sqrt(A*A);A=A*(1/Norm);
   double Phi=M_PI/180*Q_xy;
   VecCl X1(3);X1=X*cos(Phi)+A*sin(Phi);  // Now X1 - new Z was X after turn
   Norm=sqrt(X1*X1);
//cout<<" Norm unit ? "<<Norm<<"\n";
   X[1]=1;X[2]=0;X[3]=0;O_zx=acos(X*X1)*180/M_PI;
   X=MulVec(X1,X);O_xy=acos(X*Q)*180/M_PI;
cout<<" Q_zx "<<Q_zx<<" Q_xy "<<Q_xy<<" O_zx "<<O_zx<<" O_xy "<<O_xy<<"\n";
  }; 


void HQsAvInt(double Teta,double Phi,double H,double Qs,double Eta,
              double Is,double Wid,double I_Hx,double I_Hy,double I_Hz,
              double Hc,double Teta_V,int Add,int FineAver,int Sum8,double *x,double *y)
  {
//cout<<" HQsAvInt\n"; 
   double Coef=1,Hs, Hs_zx,Hs_xy;
   double *H_zx,*H_xy;
   if (FineAver<1) return ;
   H_zx=new double[FineAver+1];H_zx[0]=FineAver;
   H_xy=new double[FineAver+1];H_xy[0]=FineAver;
   double Coe=90./FineAver,AddCoe=Coe*0.5;
   for (int o=0;o<FineAver;o++) H_zx[o+1]=Coe*o+AddCoe;
   double Ax[5]={4,0,90,180,270};
//   double Az[3]={2,0,90};  // - This was
   double Az[3]={2,0,180};   //   New correction ? better ?
   if (Sum8==0)  { Ax[0]=1;Az[0]=1;}
   CMatrCl Mat(4),AmpR(4),AmpL(4);
   VecCl SumI(x[0]),I(x[0]);SumI=SumI*0;
   VecCl Teta_Varr(0),Phi_Varr(0);
   double Q_zx,Q_xy;
   for (int az=1;az<=Az[0];az++)
     {
      for (int ax=1;ax<=Ax[0];ax++)
        {
         for (int k=1;k<=H_zx[0];k++)
           {
            H_xy[0]=min(int(fabs(sin((H_zx[k]+Az[az])*M_PI/180))*FineAver)+1,FineAver);
            Coe=90./H_xy[0];AddCoe=Coe*0.5;
            for (o=0;o<H_xy[0];o++) H_xy[o+1]=Coe*o+AddCoe;
//cout<<" N "<<H_xy[0]<<" H_zx "<<H_zx[k]<<"\n";
            double IntCoef=fabs(sin((H_zx[k]+Az[az])*M_PI/180)*M_PI/(FineAver*H_xy[0]*8));
            for (int kx=1;kx<=H_xy[0];kx++)
              {
               FormHSum(H,H_zx[k]+Az[az],H_xy[kx]+Ax[ax], 
                        I_Hx,I_Hy, I_Hz, Hc ,Teta,Phi,Teta_V,Add, FineAver,Hs,Q_zx,Q_xy,
                        Teta_Varr,Phi_Varr);
               HQsMat(Mat,Q_zx,Q_xy,Hs,Qs, Eta);//, Is, Wid, Hs_zx, Hs_xy);
               HQsAmpVecMonoAver(Mat,Teta_Varr,Phi_Varr,Hs,Is,Wid,Add,x,I.Ptr);
               SumI=SumI+I*IntCoef;
              }
           } 
        }   
     }   
   Coef=1;if (Sum8) Coef/=8.;
   SumI=SumI*Coef;MemoryMove(SumI.Ptr,y,sizeof(double)*(x[0]+1));
   delete H_zx;delete H_xy;
  };  

double HQsAvInt(double Teta,double Phi,double H,double Qs,double Eta,
              double Is,double Wid,double I_Hx,double I_Hy,double I_Hz,
              double Hc,double Teta_V,int Add,int FineAver,int Sum8,double x)
  {
   double xi[2]={1,x};
   double yi[2]={1,0};
   HQsAvInt(Teta,Phi,H,   Qs,  Eta, Is,  Wid,I_Hx,I_Hy,I_Hz, Hc,Teta_V , Add,FineAver,Sum8,xi,  yi);
   return yi[1];
  }; 
   
