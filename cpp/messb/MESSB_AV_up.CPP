#include <lib\precompiled\Messb.h>
#include "messb_av.h"












SortClass *SortClass::Cur;
#include <algorithm>

void SortClass::MakeCode(VecCl &vec)
{
  code.clear();Ptr=&vec.Ptr[1];Cur=this;
  for (int k=0;k<vec[0];k++) code.push_back(k);
  sort( code.begin( ), code.end( ), greater );
//cout<<" Coding vector: ";for (int k=0;k<vec[0];k++) cout<<code[k]<<" ";cout<<"\n";
}
void SortClass::Code(VecCl &vec)
{
  CheckVector(vec);VecCl ret(vec.Dim());
  for (int k=1;k<=vec.Dim();k++) ret[k]=vec[code[k-1]+1];
//cout<<" Coding vec "<<vec;
  vec=ret;
//cout<<" Coded vec "<<vec;
}
void SortClass::Decode(VecCl &vec)
{
  CheckVector(vec);VecCl ret(vec.Dim());
  for (int k=1;k<=vec.Dim();k++) ret[code[k-1]+1]=vec[k];
//cout<<" DeCoding vec "<<vec;
  vec=ret;
//cout<<" DeCoded vec "<<vec;
}
void SortClass::CheckVector(VecCl &vec)
{ if (vec.Dim()!=code.size()) {cout<<"vector size "<<vec[0]<<" differs from code size "<<(int)code.size()<<"\n";cout.flush();exit(0);}}



//  int SphereNum,RotNum,SphereQuater;
//  double RotAngle;

void SphereAnglesGenerator::InitSphereSerie()
{
  int nT,nP;
  double lowT,upT,lowP,upP;
  if (SphereQuater)  { nP=SphereNum;nT=SphereNum;lowT=0;upT=90;lowP=0;upP=90; }
  else             { nP=4*SphereNum;nT=SphereNum*2;lowT=0;upT=180;lowP=0;upP=360; }

  VecCl Teta=VecCl::MakeStepVector(nT,lowT,upT,1);
  int N=Teta.Dim(),k,s=0,k1=0;
  vector<VecCl> Phi_arr(N+1);
  VecCl Prob(N);
  double ProbNorm=0;
  for (k=1;k<=N;k++)
  {
    Phi_arr[k]=VecCl::MakeStepVector(max(nP*fabs(sin(Teta[k]*M_PI/180)),1),lowP,upP,1);
    s+=Phi_arr[k].Dim();
    Prob[k]=fabs(sin(Teta[k]*M_PI/180))/Phi_arr[k].Dim();
    ProbNorm+=fabs(sin(Teta[k]*M_PI/180));
  }
  if (ProbNorm>0) Prob=Prob/ProbNorm;
  Angles.clear();//Angles.resize(s);
  for (k=1;k<=N;k++) {for (k1=1;k1<=Phi_arr[k].Dim();k1++) Angles.push_back(V3D_Angles(Prob[k],Teta[k],Phi_arr[k][k1]));}
  RestartSphereSerie();
}

void SphereAnglesGenerator::MakeRotAngles(V3D_Angles &rot_ax,V3D_Angles &view_ax,vector<V3D_Angles> &rot_ang)
{
  int nP;
  double lowP,upP;
  if (SphereQuater)  { nP=RotNum;lowP=0;upP=90; }
  else             { nP=4*RotNum;lowP=0;upP=360; }
  VecCl Phi=VecCl::MakeStepVector(max(nP*fabs(sin(RotAngle*M_PI/180)),1),lowP,upP,1);

  int k,N=Phi.Dim();
  rot_ang.clear();
  for (k=1;k<=N;k++) 
  {
    rot_ang.push_back(Turn3D::TurnAngles
                      (V3D_Angles(1,RotAngle,Phi[k]),
                      Turn3D::ITurnMatr(rot_ax)
                      *Turn3D::TurnMatr(view_ax)
                      ));
//    cout<<" View "<<V3D_Angles(1,RotAngle,Phi[k])<<" rot_ax "<<rot_ax<<" view_ax "<<view_ax<<" res "<<*(rot_ang.end()-1);
  }
}

  //VecCl A;
  //double Hc;
AnisotropeGenerator *AnisotropeGenerator::CurPtr;
#include "mat\quad.h"
double AnisotropeGenerator::FindAnisC(double c)
{
  double a12=2*(CurPtr->A.x()-CurPtr->A.y()),a13=2*(CurPtr->A.x()-CurPtr->A.z());
  V3D_XYZ H=CurPtr->CurrentH;
  H.x()/=c;
  H.y()/=(a12+c);
  H.z()/=(a13+c);
//cout<<" c "<<c<<" res "<<H.vec*H.vec-1<<"\n";
  return H.vec*H.vec-1;
}
void AnisotropeGenerator::GetResH(V3D_Angles &Hrnd,V3D_Angles &Hres)
{
  V3D_XYZ H=V3D_XYZ(Hrnd)+V3D_XYZ(Hint);
//cout<<" A "<<A<<"\n";
  SortClass srt(A.vec,1);srt.Code(A.vec);srt.Code(H.vec);
//cout<<" coded A "<<A<<"\n";
  double a12=2*(A.x()-A.y()),a13=2*(A.x()-A.z()),c=sqrt(H.vec*H.vec);
  CurrentH=H;CurPtr=this;
//cout<<" H "<<H<<"\n";
    Fzero(FindAnisC,M_Eps*c,c,c,1e-8,1e-8,100);

  H.x()/=c;
  H.y()/=(a12+c);
  H.z()/=(a13+c);
  srt.Decode(H.vec);srt.Decode(A.vec);
if (fabs(sqrt(H.vec*H.vec)-1)>1e-3) cout<<"EEEEEEEEEEEEEEEEEEEEEERROR!!!!!!!Norm for Anisotropy is not found\n";
  H.vec=H.vec*Hc/sqrt(H.vec*H.vec)+V3D_XYZ(Hrnd).vec;//Hext.vec;
//cout<<" Found vec "<<Turn3D::XYZ2Angles(H);
  Hres=H;
}


//vector<V3D_Angles> View;

void Solver_H_Z_mono_multiView::Solve(double *x,double *Res)
{
  VecCl sum(x[0]),cur(x[0]);sum=sum*0;
  for (int k=0;k<(int)View.size();k++)
  {
    Solver_H_Z_mono::PhiH_View=View[k].phi();
    Solver_H_Z_mono::TetaH_View=View[k].teta();
    Solver_H_Z_mono::Solve(x,cur.Ptr);
    sum=sum+cur;
  }
  sum=sum/View.size();sum.Copy2Double(Res);
};

//Ref<SphereAnglesGenerator> Angles;
//Ref<AnisotropeGenerator> Anis;
//Ref<Ham_Qs_Gamma> Ham;
//double Hext;

void PolyCrystallAverage::Solve(double *x,double *y)
{
  VecCl SumI(x[0]),I(x[0]);SumI=SumI*0;y[0]=x[0];

  Angles->InitSphereSerie();
  V3D_Angles Hrnd,Hres;
  while (Angles->NextSphereAngle(Hrnd))
  {
//  Hrnd.r - probability of this direction, sum of probs is 1
//  For clc of result field - need Hext value;
//    cout<<" Hrnd_prob: "<<Hrnd;
    double p=Hrnd.r();Hrnd.r()=Hext;
//cout<<" Hrnd_int: "<<Hrnd;
    Anis->GetResH(Hrnd,Hres);
    Ham->H=Hres.r();
//cout<<" Hres: "<<Hres;
    Ref<Ham_Z_Turned_Qs_Gamma> Cur=new Ham_Z_Turned_Qs_Gamma(*Ham,Hres.teta(),Hres.phi());
    vector<V3D_Angles> View;Angles->MakeRotAngles(Hrnd,Hres,View);
//cout<<" View "<<View[0];
    Solver_H_Z_mono_multiView(Cur,View).Solve(x,I.Ptr);
    SumI=SumI+I*p;
  }
  SumI.Copy2Double(y);
};












/*


void PolyCrystallAverage::GenerateVec(VecCl &vec,int Num,int low,int up,int centered)
{
  int N=Num ;
  double stp=up-low,start=low;
  if (centered) { stp/=N;start+=0.5*stp; }
  else stp/=max(N-1,1);
  vec.SetDim(N);
  for (int k=1;k<=N;k++) {vec[k]=start;start+=stp;}
}
void PolyCrystallAverage::FormHextAngle_ViewVec(HQsData &HQs,AverageData &aver,
                     double H_TetVec,double H_PhiVec,
                     double &H,double &Hteta,double &Hphi,VecCl &V_Teta,VecCl &V_Phi)
{
  VecCl Hres(3),Hext(3);
  Hext=aver.V+aver.Hext*TetaPhi2Vec(1,H_TetVec,H_PhiVec);
  Hres[1]=Hext[1]/(2*(aver.A[2]-aver.A[1])+1);
  Hres[2]=Hext[2];
  Hres[3]=Hext[3]/(2*(aver.A[2]-aver.A[3])+1);
  Hres=aver.Hc*Hres/sqrt(Hres*Hres)+Hext;
  Vec2TetaPhi(Hres,H,Hteta,Hphi);

  double up=90;
  int n=aver.PhiAverNum;
  if (aver.Sum8) {up*=4;n*=4;};
  VecCl tmpPhi,tmpTeta(n);
  GenerateVec(tmpPhi,n,0,up,1);tmpTeta=tmpPhi*0+1;
  for (int k=1;k<=tmpTeta.Dim();k++)
  {
//    Vec2TetaPhi(Rot3D(TetaPhi2Vec(1,tmpTeta[k],tmpPhi[k]),H_TetVec,H_PhiVec),up,V_Teta[k],V_Phi[k]);
    Vec2TetaPhi(Rot3D(Rot3D(TetaPhi2Vec(1,tmpTeta[k],tmpPhi[k]),H_TetVec,H_PhiVec),
      Hteta,Hphi),up,V_Teta[k],V_Phi[k]);
  }
}

// Turn Matr?
void PolyCrystallAverage::GenerateHam(CMatrCl &Ham,HQsData &HQs,double &H,double &Hteta,double &Hphi)
{
  
}
//void MulDimsSame(VecCl &vec,int dim1)
//{
//  VecCl res(vec[0]*dim1);
//  int N=1;
//  for (int k=1;k<=vec[0];k++)
//  { for (int k1=1;k1<=dim1;k1++) res[N++]=vec[k]; }
//  vec=res;
//}
//void MulDimsDiff(VecCl &vec,int dim1)
//{
//  VecCl res(vec[0]*dim1);
//  int N=1;
//  for (int k=1;k<=dim1;k++)
//  { for (int k1=1;k1<=vec[0];k1++) res[N++]=vec[k]; }
//  vec=res;
//}

void PolyCrystallAverage::GenerateAngles(AverageData &aver,VecCl &IntVec,VecCl &TetVec,VecCl &PhiVec)
{
   GeneratePhiTeta(aver.FineAver,aver.Sum8,IntVec,TetVec,PhiVec);
   //GenerateVec(GammaVec,aver.QsRotNum,0,180,1);
   //MulDimsSame(IntVec,GammaVec[0]);
   //MulDimsSame(TetVec,GammaVec[0]);
   //MulDimsSame(PhiVec,GammaVec[0]);
   //MulDimsDiff(GammaVec,PhiVec[0]);
}
void PolyCrystallAverage::HQsAvIntGam(HQsData &HQs,AverageData &aver,
                                      double *x,double *y)
  {
   if (aver.FineAver<1) return ;
   VecCl SumI(x[0]),I(x[0]);SumI=SumI*0;
// random Hext angles, probability
   VecCl H_IntVec,H_TetVec,H_PhiVec;//,H_GammaVec;
   PolyCrystallAverage work;

   work.GenerateAngles(aver,H_IntVec,H_TetVec,H_PhiVec);
   for (int k=1;k<=H_IntVec[0];k++)
    {
     CMatrCl Ham(4);
     VecCl V_Teta,V_Phi;
     double H,Hteta,Hphi;
     work.FormHextAngle_ViewVec(HQs,aver,H_TetVec[k],H_PhiVec[k],
                    H,Hteta,Hphi,V_Teta,V_Phi);
     work.GenerateHam(Ham,HQs,H,Hteta,Hphi);//Turn Matr?
     work.GenerateInt(Ham,aver,V_Teta,V_Phi,x,I.Ptr);
     SumI=SumI+I*H_IntVec[k];
     }   
   MemoryMove(SumI.Ptr,y,sizeof(double)*(x[0]+1));
  };  



*/




























/*
   
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

*/






//#include <malloc.h>

//#define Pi M_PI
//3.14159265358979
//
//VecCl FormHVec(double H,double H_zx,double H_xy)
//  {
//   VecCl Ret(3);
//   double Teta=H_zx*M_PI/180,Phi=H_xy*M_PI/180;
//   double M=H*sin(Teta);
//   Ret[1]=H*cos(Teta);
//   Ret[2]=M*cos(Phi);
//   Ret[3]=M*sin(Phi);
//   return Ret;
//  }; 
//void FormHPhi(VecCl &Hi,double &Hs,double &Hs_zx,double &Hs_xy)
//  {
//   Hs=sqrt(Hi*Hi);      if (Hs<StndErr)    {Hs_zx=0;Hs_xy=0;return ;}
//   Hs_zx=acos(Hi[1]/Hs);                                    
//   double H_xy=sqrt(sqr(Hi[2])+sqr(Hi[3]));
//   if (H_xy<StndErr) {Hs_xy=0;Hs_zx*=180/M_PI; return ;}
//   Hs_xy=acos(Hi[2]/H_xy);
////   if (Hi[2]<0) Hs_xy=M_PI-Hs_xy; 
//   if (Hi[3]<0) { Hs_zx=2*M_PI-Hs_zx;Hs_xy=M_PI-Hs_xy; }
//   Hs_xy*=180/M_PI;Hs_zx*=180/M_PI;
////   VecCl Res=FormHVec(Hs,Hs_zx,Hs_xy);
////cout<<" HI "<<Hi<<" Res "<<Res-Hi;
//  }; 
//VecCl MulVec(VecCl &X,VecCl &Y)
//  {
//   VecCl Res(3);                           
//   Res[1]=X[2]*Y[3]-X[3]*Y[2];
//   Res[2]=X[3]*Y[1]-X[1]*Y[3];
//   Res[3]=X[1]*Y[2]-X[2]*Y[1];
//   return Res;
//  }; 
//VecCl FormViewVec(double H_zx,double H_xy,double Teta_V,double ViewAngle)
// {
//  VecCl ret=FormHVec(1,H_zx,H_xy);
//  if (fabs(Teta_V)<StndErr) return ret;
//  VecCl est=FormHVec(1,H_zx-Teta_V,H_xy);
//  VecCl ort=ret*cos(Teta_V*M_PI/180);
//  VecCl hx=ort-est;hx=hx/(sqrt(hx*hx));
//  VecCl hy=MulVec(hx,ret);
//  double l=fabs(sin(Teta_V*M_PI/180));
//  VecCl add=(hx*cos(ViewAngle*M_PI/180)+hy*sin(ViewAngle*M_PI/180))*l;
////  return ort+ret;
//  return add+ort;
// }
//void FormHSum(double H,double H_zx,double H_xy,double I_Hx,double I_Hy,
//              double I_Hz,double Hc,double Q_teta,double Q_phi,double Teta_V,
//              int Add,int FineAver,int Sum8,
//              double &Hs,double &Q_zx,double &Q_xy,VecCl &Teta_Varr,
//              VecCl &Phi_Varr)
//  {
////  1  - z; 2 - x; 3 - y
////  Qs - bazis for I_H; find Hi - vector of internal Hc + Hext
//   double Hs_zx,Hs_xy;
//   VecCl Hext(3);Hext=FormHVec(H,H_zx,H_xy);
//   VecCl Hi(3);
//   if (Add) {Hi[1]=I_Hz;Hi[2]=I_Hx;Hi[3]=I_Hy;}
//   else {Hi[1]=I_Hz*Hext[1];Hi[2]=I_Hx*Hext[2];Hi[3]=I_Hy*Hext[3];}
//   Hi=Hi+Hext;Hs=sqrt(Hi*Hi);Hi=Hi*(Hc/Hs)+Hext;
////cout<<" Teta "<<H_zx<<" Phi "<<H_xy<<"\n";
////cout<<" Hi "<<Hi<<" Hext "<<Hext;
//// Set Hi basis. X axis in the plane z & Hi;
//   VecCl Z(3),HZ(3),HX(3),HY(3);Z=FormHVec(1,0,0);Hs=sqrt(Hi*Hi);
//if (Hs<StndErr) {Q_zx=0;Q_xy=0;cout<<" very bad. Hi==0;\n";return ;}
//   HZ=Hi*(1/Hs);HX=MulVec(HZ,Z);Hs=sqrt(HX*HX);
//if (Hs<StndErr) {Q_zx=0;Q_xy=0;cout<<" very bad. HX==0;\n";return ;}
//   HX=HX*(1/Hs);HY=MulVec(HZ,HX);Hs=sqrt(HY*HY);
//if (fabs(Hs-1)>StndErr) {cout<<" very bad. HY!=1;\n";return ;}
//
//// Find View in Hi basis
//// Form Teta_Varr, First for Phi=0; now for Teta_V=0
////   double Phi_Arr[5]={4,0,90,180,270};
//   int Nview=min(int(fabs(sin(Teta_V*M_PI/180))*FineAver)+1,FineAver);
//   double ViewStp=90./Nview,ViewStrt=ViewStp*0.5,ViewAngle;
//   if (Sum8) Nview*=4;
//   Teta_Varr.SetDim(Nview);Phi_Varr.SetDim(Nview);
//   VecCl V0(3),Res(3);
//   for (int k=0;k<Nview;k++)
//    {
//     ViewAngle=ViewStp*k+ViewStrt;
//     V0=FormViewVec(H_zx,H_xy,Teta_V,ViewAngle);
////cout<<" Teta_V "<<Teta_V<<" ViewAngle "<<ViewAngle<<"\n";
////cout<<" V0 "<<V0<<"\n";
//     Res[1]=V0*HZ;Res[2]=V0*HX;Res[3]=V0*HY; // V0 - in Hi coord
////cout<<" Res "<<Res<<"\n";
//     FormHPhi(Res,Hs,Hs_zx,Hs_xy); // In Hi coordinates - V0 angles
//
//     Teta_Varr[k+1]=Hs_zx;
//     Phi_Varr[k+1]=Hs_xy;
//    }
////cout<<" Teta "<<Teta_Varr<<"\n";
////cout<<" Phi "<<Phi_Varr<<"\n";
//   FormHPhi(HZ,Hs,Hs_zx,Hs_xy);Q_zx=-Hs_zx;
//   FormHPhi(HX,Hs,Hs_zx,Hs_xy);Q_xy=-Hs_xy;
//   Hs=sqrt(Hi*Hi);
//  }; 
//void HQsAmpVecMonoAver(CMatrCl &Hamilt,VecCl &Teta_Varr,VecCl &Phi_Varr,
//                       double H,double IS,double Wid,int Add,
//                       double *x,double *y)
//  {
//   CMatrCl EigenVect(4),EigenVect1(4);
//   double EigenVal[5],Intencity[9],Position[9];
//   CVecCl EVal;
//   int Ret=1,k,i;
//   EigenVal[0]=4;Intencity[0]=8;Position[0]=8;
//   URComeig(Hamilt,EigenVect1,EigenVect);          
//   for (i=1;i<=4;i++) EigenVal[i]=my_real(Hamilt(i,i));EigenVect=Transpon(EigenVect);
////   int Ret=CReduceLQ(Hamilt,EigenVect,EigenVal,StndErr);
//   double SumI[9];for (i=1;i<=8;i++) SumI[i]=0;
//   VecCl Amp2(4);
//   
//   for (i=1;i<=Teta_Varr.Dim();i++) 
//     {
//      double TetaH_View=Teta_Varr[i],PhiH_View=Phi_Varr[i];
//      double Cos=cos(TetaH_View*M_PI/180),Sin=sin(TetaH_View*M_PI/180);
//      double Cos2=sqr(Cos);
//      Amp2[1]=(1+Cos2)*0.75;Amp2[2]=1.-Cos2;Amp2[3]=(1+Cos2)*0.25;Amp2[4]=0;
//      IntPos(EigenVect,EigenVal,Intencity,Position,H,IS,Amp2);
////  Add interference 
//
//      my_comp e1=my_comp(cos(PhiH_View*M_PI/180),sin(PhiH_View*M_PI/180)),e2=e1*e1;
//      my_comp C1=sqrt(3.)*Sin*Cos,C2=0.5*sqrt(3.)*sqr(Sin),C3=C1/sqrt(3.);
//      for (k=1;k<=4;k++)
//        {
//      Intencity[k]+=my_real(   C1*e1*conj(EigenVect(1,k))*EigenVect(2,k)+
//                            C2*e2*conj(EigenVect(1,k))*EigenVect(3,k)-
//                            C3*e1*conj(EigenVect(2,k))*EigenVect(3,k));
//      Intencity[k+4]+=my_real(-C1*conj(e1*EigenVect(4,k))*EigenVect(3,k)+
//                            C2*conj(e2*EigenVect(4,k))*EigenVect(2,k)+
//                            C3*conj(e1*EigenVect(3,k))*EigenVect(2,k));
//        }
//      for (k=1;k<=8;k++) 
//        {if (Intencity[k]<0) cout<<" Monocryst. Negative.\n";
//         SumI[k]+=Intencity[k];}
//     }; 
////        }};
//
////   delete EigenVal;
//   for (k=1;k<=8;k++) Intencity[k]=SumI[k]/Teta_Varr.Dim();
//   HQsInt(Intencity,Position,Wid,x,y);
////   return Ret;
//  };
//
//// Generating random external field angles, and there relative weight IntExt
//// Sum8=0 - make random angles only for one sphere sector
//// Number of angles used in summation FineAver^2/2 ( aproximately ) in one sector
//void GeneratePhiTeta(int FineAver,int Sum8,VecCl &IntExt,VecCl &TetExt,VecCl &PhiExt)
// {
//  int N=0,k,k1,n=0;
//  VecCl Tet(FineAver),PhiNum(FineAver);
//  double TetStp=90./FineAver,TetStrt=TetStp*0.5;
//  for (k=0;k<FineAver;k++) 
//   {
//    Tet[k+1]=TetStp*k+TetStrt;
//    PhiNum[k+1]=min(int(fabs(sin(Tet[k+1]*M_PI/180))*FineAver)+1,FineAver);
//    N+=PhiNum[k+1];
//   }
//  VecCl TetOne(N),PhiOne(N),IntOne(N);
//  for (k=0;k<FineAver;k++) 
//   {
//    int Nphi=PhiNum[k+1];
//    double PhiStp=90./Nphi,PhiStrt=PhiStp*0.5;
//    double Int=fabs(sin(Tet[k+1]*M_PI/180)*M_PI/(FineAver*Nphi*8));
//    for (k1=0;k1<Nphi;k1++)
//     {
//      TetOne[n+1]=Tet[k+1];
//      PhiOne[n+1]=PhiStp*k1+PhiStrt;
//      IntOne[n+1]=Int;
//      n++;
//     }
//   }
//  if (!Sum8) {IntExt=IntOne;TetExt=TetOne;PhiExt=PhiOne;return;}
//  IntExt.SetDim(N*8);TetExt.SetDim(N*8);PhiExt.SetDim(N*8);
//  IntOne=IntOne*(1./8);
//  double Ax[5]={4,0,90,180,270};
//  double Az[3]={2,0,180};  // Was 90 ? WHYYY ?
//  int Ntot=0,None=0;
//  for (k=0;k<Az[0];k++)
//   {
//    for (k1=0;k1<Ax[0];k1++)
//     {
//      for (None=0;None<N;None++)
//       {
//        TetExt[Ntot+1]=TetOne[None+1]+Az[k+1];
//        PhiExt[Ntot+1]=PhiOne[None+1]+Ax[k1+1];
//        IntExt[Ntot+1]=IntOne[None+1];
//        Ntot++;
//       }
//     }
//   }
// };
//    
//void HQsAvInt(double Teta,double Phi,double H,double Qs,double Eta,
//              double Is,double Wid,double I_Hx,double I_Hy,double I_Hz,
//              double Hc,double Teta_V,int Add,int FineAver,int Sum8,double *x,double *y)
//  {
////cout<<" HQsAvInt\n"; 
//   if (FineAver<1) return ;
//   CMatrCl Mat(4);//,AmpR(4),AmpL(4);
//   VecCl SumI(x[0]),I(x[0]);SumI=SumI*0;
//   VecCl Teta_Varr(0),Phi_Varr(0);
//   double Q_zx,Q_xy,Hs;
//   VecCl IntExt,TetExt,PhiExt;
//   GeneratePhiTeta(FineAver,Sum8,IntExt,TetExt,PhiExt);
//   for (int k=1;k<=IntExt[0];k++)
//    {
//     FormHSum(H,TetExt[k],PhiExt[k],I_Hx,I_Hy, I_Hz, Hc ,Teta,Phi,Teta_V,Add, 
//              FineAver,Sum8,
//              Hs,Q_zx,Q_xy,Teta_Varr,Phi_Varr);
//     HQsMat(Mat,Q_zx,Q_xy,Hs,Qs, Eta);//, Is, Wid, Hs_zx, Hs_xy);
//     HQsAmpVecMonoAver(Mat,Teta_Varr,Phi_Varr,Hs,Is,Wid,Add,x,I.Ptr);
//     SumI=SumI+I*IntExt[k];
//     }   
//   MemoryMove(SumI.Ptr,y,sizeof(double)*(x[0]+1));
//  };  
//
//
//double HQsAvInt(double Teta,double Phi,double H,double Qs,double Eta,
//              double Is,double Wid,double I_Hx,double I_Hy,double I_Hz,
//              double Hc,double Teta_V,int Add,int FineAver,int Sum8,double x)
//  {
//   double xi[2]={1,x};
//   double yi[2]={1,0};
//   HQsAvInt(Teta,Phi,H,   Qs,  Eta, Is,  Wid,I_Hx,I_Hy,I_Hz, Hc,Teta_V , Add,FineAver,Sum8,xi,  yi);
//   return yi[1];
//  }; 
//

