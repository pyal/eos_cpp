//#include <alloc.h>

#include "lib/precompiled/ivl_cvt.h"

#include "mat/dat_fac.h"
#include "mat/spl_fac.h"
//#include "mat/spl_reg.h"
//#include "mat/dat_reg.h"

//#include "matlib.h"
#include "mat/mtmin.h"
#include "mat/zero.h"


double StndErr=1e-6;

//extern unsigned _stklen=60000;


void Tab_cvt(fstream &scfg,const char *tab_file,const char *res_file)
  {
   char tmp[256];
   TTable<real> dat;
   int DoIt;
   double AddZ;
   scfg>>tmp>>DoIt>>tmp>>AddZ;
//cout<<tmp;ChRead();
   if (!ReadTableRow(scfg,(TData<real>*)&dat)) { cout<<" Could not read param.";exit(0);}
   if (!DoIt) return ;
//   FormDat(&dat);
//   TColumn<real> res;(TData<real>&)res=(TData<real>&)dat;
   fstream in1(tab_file,ios::in);
   fstream out(res_file,ios::out);
   int k,N1=dat.I[1],N0=dat.I[0],k0,k1;
   real *D1=new real[N1];
//      LOG scale
//   for (k=0;k<N1;k++) D1[N1-k-1]=1/dat.D[1][k];
   for (k=0;k<N0;k++) dat.D[0][k]=log(dat.D[0][k]);
   for (k=0;k<N1;k++) D1[N1-k-1]=-log(dat.D[1][k]);
   for (k=0;k<N1;k++) dat.D[1][k]=D1[k];
   for (k0=0;k0<N0;k0++) 
     {
      for (k1=0;k1<N1;k1++) {in1>>D1[k1];}
//cout<<D1[k1]<<"  ";}
//cout<<"\n";
      for (k1=0;k1<N1;k1++) {dat.D[2][N1*k0+k1]=log(D1[N1-k1-1]+AddZ);}
     } 
   delete []D1;
   out<<" AddZ "<<AddZ<<"\n";
   DataStore(out,(TData<real>*)&dat);out.close();in1.close();
//   DataStore(out,(TData<real>*)&res);out.close();
  };


void Const_spl(fstream &scfg,const char *dat_file,const char *spl_file,const char *cmp_file,int Two=0)
  {
   char tmp[256];
cout<<" coreleft "<<Coreleft()<<"\n";
   int NumX,NumY,Meth,LogZ;
   double Misf;
   int DoIt;
   scfg>>tmp>>DoIt;cout<<tmp<<" "<<DoIt<<" ";
   scfg>>tmp>>NumX>>tmp>>NumY>>tmp>>Meth>>tmp>>Misf>>tmp>>LogZ;
   if (!DoIt) return ;

   fstream sdat(dat_file,ios::in);
   fstream sspl(spl_file,ios::out);
   fstream scmp(cmp_file,ios::out);
   Ref<SplineXYIO> sp=new RegridSpline;
   TData<real> *d=NULL;
   TData<real> *d1=NULL;DataRegister("TColumn",d1);
double AddE,AddP;sdat>>tmp>>AddE;if (Two) sdat>>tmp>>AddP;
   if ( !(DataRegister(sdat,d)) ) 
                  {cout<<"Const_spl::could not register data. File \""<<dat_file<<"\"\n";exit(0);}
cout<<" Const_spl   \n";

   if (LogZ)  for (int k=0;k<d->I[2];k++) d->D[2][k]=log(d->D[2][k]);
   *d1=*d;

   //SurfRegister("RegridSpline",sp);
   sp->SetLimits(d1->D[0][d1->I[0]-1], d1->D[0][0], d1->D[1][d1->I[1]-1], d1->D[1][0]);
   //sp->Xup=d1->D[0][d1->I[0]-1];sp->Xlow=d1->D[0][0];
   //sp->Yup=d1->D[1][d1->I[1]-1];sp->Ylow=d1->D[1][0];

   //sp->Delta=0;

   DataStore(scmp,d1);
   sp->Generate(NumX,NumY,Misf,Meth,*d);
if (!Two) sspl<<" AddZ "<<AddE<<"\n";
else {
    double Xl, Xu, Yl, Yu;
    sp->GetBoundary(Xl, Xu, Yl, Yu);
    sspl<<" AddE "<<AddE<<" AddP "<<AddP<<"\n"<<" MinDenc "
         <<exp(Yl)<<" MaxDenc "<<exp(Yu)<<"\n";
}
SavableClass::Save(sspl, sp);
   //SurfStore(sspl,sp);
   sp->Evaluate(*d1);
   DataStore(scmp,d1);
   sdat.close();sspl.close();scmp.close();
   delete d;delete d1;
   //delete sp;
cout<<" coreleft "<<Coreleft()<<"\n";
  };


double WorkSub;
double WorkDencity;
SplineXYIO *WorkSpline;
int WorkCol;

#define MinPhys -10

double GetWork(double T)
  {
   double x,y,z;
   if (WorkCol) {x=T;y=WorkDencity;}
   else {x=WorkDencity;y=T;}
   WorkSpline->Evaluate (&x,&y,&z,1);
   return z-WorkSub;
  };
   
real FindTemp(SplineXYIO *se,real Denc,real EFind,real Tmin,real Tmax,int Dcol)
  {
   WorkSpline=se;WorkCol=Dcol;WorkDencity=Denc;WorkSub=EFind;
   real ret=ZeroIn(Tmin,Tmax,GetWork,StndErr);
//cout<<" FindTemp "<<exp(ret)<<" Delta "<<GetWork(ret)<<" Tmin "<<exp(Tmin)<<
//    " Tmax "<<exp(Tmax)<<"\n";
   return ret;
  }; 

void FindMinMaxE(SplineXYIO *se,real Denc,real &Emin,real &Emax,
                              real &Tmin,real Tl,real Tu,int Dcol)
  {
   WorkSpline=se;WorkCol=Dcol;WorkDencity=Denc;WorkSub=0;
   Emax=GetWork(Tu);

//   Tmin=MinFunc(Tl,Tu,GetWork,StndErr);
//   Emin=GetWork(Tmin);
   if (!NAGMin(GetWork,StndErr,StndErr,Tl,Tu,1000,Tmin,Emin))
      cout<<" Did not find Tmin. Try old version\n";
cout<<"FindMinMaxE Emax "<<Emax<<" Tmin "<<Tmin<<" Emin "<<Emin <<"\n";
cout<<" Tl "<<Tl<<" Tu "<<Tu<<" Denc "<<Denc<<"\n";
  } 
void Construct(TData<real> *datP,TData<real> *datT,SplineXYIO *se,SplineXYIO *sp,real *Tmin,real Tmax,int Dcol)
  {
   int Ecol=1-Dcol;
//cout<<" Construct\n";ChRead();
//   *datT=*datP;
//cout<<" Construct\n";ChRead();
   int ND=datP->I[Dcol],NE=datP->I[Ecol];
   int kd,ke,n=-1;
   real T;
   for (ke=0;ke<NE;ke++)
     {
      for (kd=0;kd<ND;kd++)
        {
         n++;
         if (datP->D[2][n]==MinPhys) continue;
         if ( (T=FindTemp(se,datP->D[Dcol][kd],datP->D[Ecol][ke],Tmin[kd],Tmax,Dcol))==0 )
           cout<<" BAD in FindTemp Denc "<<datP->D[Dcol][kd]
               <<" E "<<datP->D[Ecol][ke]<<"\n";
         WorkSpline=sp;WorkCol=Dcol;WorkDencity=datP->D[Dcol][kd];WorkSub=0;
         datP->D[2][n]=GetWork(T);
         datT->D[2][n]=T;
//cout<<" Denc "<<exp(datP->D[Dcol][kd])<<" E "<<exp(datP->D[Ecol][ke])<<
//     " P "<<exp(datP->D[2][n])<<" T "<<exp(T)<<"\n";
        };
     };
  };                                                     
   
void FormLimits(TData<real> *dat,SplineXYIO *se,real *&Tmin,real &Tu,int Dcol)
  {
   int Ecol=1-Dcol, k;
   int ND=dat->I[Dcol],NE=dat->I[Ecol];
   real *Emin=new real [ND], *Emax=new real [ND];Tmin=new real [ND];
   Tu=dat->D[Ecol][NE-1];
   real Tl=dat->D[Ecol][0];
   for (k = 0; k < ND; k++)
      FindMinMaxE(se,dat->D[Dcol][k],Emin[k],Emax[k],Tmin[k],Tl,Tu,Dcol);
   double El=Emin[0],Eu=Emax[ND-1];
   double Estp=(Eu-El)/(NE-1),Ecur=El;
   for (k=0;k<NE;k++) { dat->D[Ecol][k]=Ecur;Ecur+=Estp;}
   int n=0,kd,ke;
   for (kd=0;kd<ND;kd++)
     {
      for (ke=0;ke<NE;ke++)
        {
//         if (dat->D[Ecol][ke]<Emin[kd]) dat->D[2][n]=MinPhys;
//         else 
           dat->D[2][n]=0;
         n++;
        }
     }
   delete []Emin;delete []Emax;
  }; 

void Constr_Pre(fstream &scfg,const char *e_spl,const char *p_spl,const char *spl_Pfile,const char *spl_Tfile,TData<real>  *datP)
  {
   char tmp[256];
   int DoIt;
   scfg>>tmp>>DoIt;
   if (!DoIt) return ;
   fstream ine(e_spl,ios::in);
   fstream inp(p_spl,ios::in);
   fstream outP(spl_Pfile,ios::out);
   fstream outT(spl_Tfile,ios::out);
cout<<" coreleft "<<Coreleft()<<"\n";
   Ref<SplineXYIO> se, sp;
double AddP,AddE;ine>>tmp>>AddE;inp>>tmp>>AddP;
if ( ( !(se<<SavableClass::Read(ine))) || ( !(sp<<SavableClass::Read(inp))) )
     { cout<<" Could not register spline.\n";exit(0);}
   real *Tmin,Tmax;
   FormLimits(datP,se,Tmin,Tmax,1);
   TData<real>  *datT=NULL;DataRegister("TTable",datT);
   *datT=*datP;
   Construct(datP,datT,se,sp,Tmin,Tmax,1);
outP<<" AddE "<<AddE<<" AddP "<<AddP<<"\n";
outT<<" AddE "<<AddE<<" AddP "<<AddP<<"\n";
   DataStore(outP,datP);DataStore(outT,datT);
   outP.close();outT.close();
   delete datT;delete Tmin;
   //delete se; delete sp;
//   DelMinFunc();
cout<<" coreleft "<<Coreleft()<<"\n";
  }; 

int main( int argc, char *argv[] )
  {
// 1 - cfg 2 - energy 3 - pressure  4 - res_spl 5 - res_t_spl
// cfg format: for e,p
//  NumX 100 NumY 100 ExpX 1 ExptY 1  Xlow 0.02 Xup  30 Ylow  1e-4  Yup 5
//  NumX 100 NumY 100 ExpX 1 ExptY 1  Xlow 0.02 Xup  30 Ylow  1e-4  Yup 5
// NumX 60  NumY 60 Meth 0 Misf 1.4e-2 LogZ 1
// NumX 60  NumY 60 Meth 0 Misf 1.4e-2 LogZ 1
// 
cout<<" coreleft "<<Coreleft()<<"\n";
   fstream scfg(argv[1],ios::in);
   const char *e_dat="#tmp_e.dat";
   const char *p_dat="#tmp_p.dat";
    const char *e_spl="#tmp_e.spl";
    const char *p_spl="#tmp_p.spl";
    const char *e_cmp="#tmp_e.cmp";
    const char *p_cmp="#tmp_p.cmp";
    const char *res_Pdat="#tmp_rep.dat";
    const char *res_Tdat="#tmp_ret.dat";
    const char *res_Pcmp="#tmp_rep.cmp";
    const char *res_Tcmp="#tmp_ret.cmp";
   Tab_cvt(scfg,argv[2],e_dat);Tab_cvt(scfg,argv[3],p_dat);
   Const_spl(scfg,e_dat,e_spl,e_cmp);Const_spl(scfg,p_dat,p_spl,p_cmp);
   TData<real> *dat=NULL;
   fstream sdat(e_dat,ios::in);
char tmp[256];sdat>>tmp>>tmp;
   if ( !(DataRegister(sdat,dat)) ) 
                  {cout<<"could not register data. \n";exit(0);}
cout<<" Dat Spl Reg \n";
   sdat.close();
   Constr_Pre(scfg,e_spl,p_spl,res_Pdat,res_Tdat,dat);
   delete dat;
   Const_spl(scfg,res_Pdat,argv[4],res_Pcmp,1);
   Const_spl(scfg,res_Tdat,argv[5],res_Tcmp,1);
   scfg.close();
cout<<" coreleft "<<Coreleft()<<"\n";
return 0;
  }; 
