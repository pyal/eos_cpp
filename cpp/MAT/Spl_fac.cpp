#include <lib\precompiled\math.h>
#include "spl_fac.h"
//#include <bios.h>
//#include <alloc.h>

static ClassDesc RegridSpline_ClassDesc(typeid(RegridSpline),"RegridSpline",1,"Spline_category",create<RegridSpline>);
static ClassDesc CurveSpline_ClassDesc(typeid(CurveSpline),"CurveSpline",1,"Spline_category",create<CurveSpline>);
static ClassDesc ExtendedRegridSpline_ClassDesc(typeid(ExtendedRegridSpline),"ExtendedRegridSpline",1,"Spline_category",create<ExtendedRegridSpline>);
static ClassDesc CurveSpline_Noise_ClassDesc(typeid(CurveSpline_Noise),"CurveSpline_Noise",1,"Spline_category",create<CurveSpline_Noise>);

#ifdef __cplusplus
extern "C" {
#endif
extern int regrid_(integer *, integer *, real *, integer
 *, real *, real *, real *, real *, real *, real *, integer *,
 integer *, real *, integer *, integer *, integer *, real *,
 integer *, real *, real *, real *, real *, integer *, integer *,
 integer *, integer *);
extern int bispev_(real *, integer *, real *, integer
 *, real *, integer *, integer *, real *, integer *, real *,
 integer *, real *, real *, integer *, integer *, integer *, 
 integer *);

extern int splev_(real *, integer *,real *,integer *,real *,real *,
                 integer *,integer *);
extern int curfit_(integer *,integer *,real *,real *,real *,real *,real *,
                  integer *,real *,integer *,integer *,real *,real *,real *,
                  real *,integer *,integer *,integer *);

extern int gcvspl_(real *x, real *y, integer *ny, 
	real *wx, real *wy, integer *m, integer *n, integer *k, 
	integer *md, real *val, real *c__, integer *nc, 
	real *wk, integer *ier);

extern real splder_(integer *ider, integer *m, integer *n, real *t, 
	real *x, real *c__, integer *l, real *q);

#ifdef __cplusplus
}
#endif

SplineXIO *SplineXIO::StaticSpl = NULL;


int  RegridSpline::Generate(int &NumX,int &NumY,
                 double &Misf,int Cont,TData<real> &dat)
  {
   
//cout<<" RegridSpilne::Generate \n";
   TTable<real> RD;//SetDifTData((TData&)RD,dat);
   (TData<real>&)RD=dat;
   SetLimits(RD);
//cout<<" RegridSpline::Generate equal\n";
//cout<<RD.I[0]<<"  "<<RD.I[1]<<" Misf "<<Misf<<" Cont "<<Cont<<"\n";
   int i[3]={NumX,NumY,NumX*NumY};BSpl.SetDim(3,i);
   if ( RD.I[2]!=RD.I[1]*RD.I[0] || RD.I[1]<2 || RD.I[0]<2 )
	   throw info_except("Bad dim of the input data DimZ(%i)!=DimX(%i)*DimY(%i) or DimX,DimY<2. Hint if DimY=1 then take into account that X=const each NumY times\n",RD.I[2],RD.I[0],RD.I[1]);
   real MisfR=Misf;
   integer NumXi=NumX,NumYi=NumY,Conti=Cont,px=3,py=3,DX=RD.I[0],DY=RD.I[1];
   integer SX,SY;

   integer NRWork=4+NumX*(DY+px*2+5)+NumY*(2*px+5)+(px+1)*(DX+DY)+NumX+DY,
           NIWork=3+DX+DY+NumX+NumY,Result,*IWork;
//NRWork=64000/sizeof(real);
//NIWork*=2;NRWork*=2;
   IWork=new integer[NIWork];
   real SumSquareResiduals,*RWork=new real[NRWork];
//cout<<sizeof(real)*NRWork<<"\n";

//cout<<" Generate begin Good\n";cout<<" NumX "<<NumXi<<" NumY "<<NumYi<<" NumIWork "<<NIWork<<" NRWork "<<NRWork<<"\n";bioskey(0);
//cout<<coreleft()<<"\n";

/*
    if (px <= 0 || px > 5) {	goto L70;}
    kx1 = *kx + 1;
    kx2 = kx1 + 1;
    if (*ky <= 0 || *ky > 5) {goto L70;}
    ky1 = *ky + 1;
    ky2 = ky1 + 1;
    if (*iopt < -1 || *iopt > 1) {goto L70; }
    nminx = kx1 << 1;
    if (*mx < kx1 || *nxest < nminx) {goto L70;}
    nminy = ky1 << 1;
    if (*my < ky1 || *nyest < nminy) {goto L70; }
    mz = *mx * *my;
    nc = (*nxest - kx1) * (*nyest - ky1);
    lwest = *nxest * (*my + (kx2 << 1) + 1) + 4 + *nyest * ((ky2 << 1) + 1) + 
	    *mx * kx1 + *my * ky1 + max(*nxest,*my);
    kwest = *mx + 3 + *my + *nxest + *nyest;
    if (*lwrk < lwest || *kwrk < kwest) {	goto L70; }
    if (*xb > x[1] || *xe < x[*mx]) {	goto L70;   }
    i__1 = *mx;
    for (i__ = 2; i__ <= i__1; ++i__) if (x[i__ - 1] >= x[i__]) { goto L70;}
    if (*yb > y[1] || *ye < y[*my]) { 	goto L70;   }
    i__1 = *my;
    for (i__ = 2; i__ <= i__1; ++i__)	if (y[i__ - 1] >= y[i__]) { goto L70;	}
    if (*iopt ==-1) 
      if (*nx < nminx || *nx > *nxest) {goto L70;}

// int regrid_(integer *iopt, integer *mx, real *x, integer *my,
//	 real *y, real *z__, real *xb, real *xe, real *yb, real *ye, integer *
//  kx, integer *ky, real *s, integer *nxest, integer *nyest, integer *nx,
//	 real *tx, integer *ny, real *ty, real *c__, real *fp, real *wrk, 
// 	integer *lwrk, integer *iwrk, integer *kwrk, integer *ier)

*/

   regrid_(&Conti,&DX,RD.D[0],&DY,RD.D[1],RD.D[2],
           &Xlow,&Xup,&Ylow,&Yup,&px,&py,
             &MisfR,&NumXi,&NumYi,&SX,BSpl.D[0],&SY,BSpl.D[1],BSpl.D[2],
              &SumSquareResiduals,RWork,&NRWork,IWork,&NIWork,&Result);
//cout<<" Generate Good "<<Result<<" SX "<<SX<<" SY "<<SY<<" MisfR "<<SumSquareResiduals<<"\n";
//cout<<" NumXi "<<NumXi<<" NumYi "<<NumYi<<" DX "<<DX<<" DY "<<DY;
   if (SumSquareResiduals == 0 || Misf < SumSquareResiduals * 0.5 || _isnan(SumSquareResiduals))
       throw info_except("Could not make spline. Error %g have to be %g . Result is %i \n", SumSquareResiduals, Misf, Result);
       //Result *= 100;
   Delta=Misf=SumSquareResiduals;NumX=SX;NumY=SY;
   if (Result != 10 && Result != 1000 )
     {BSpl.I[0]=SX;BSpl.I[1]=SY; BSpl.I[2]=(SX-px-1)*(SY-py-1);}
//   Bspl=Bspl;
   delete RWork;delete IWork;
   return Result;
  }; 

int RegridSpline::Evaluate (double *x,double *y,double *z,int nx)
  {
  if (nx==0)
    return 0;
   real rx,ry,rz;

   integer lwk=8,liw=2,ier,n1=1,ny=1,SX=BSpl.I[0],SY=BSpl.I[1];
   real *wk=new real[lwk];
   integer *iw=new integer[liw],px=3,py=3;
   //VecCl X = SplineXIO::LimitBounds(x,nx,BSpl.D[0][0], BSpl.D[0][BSpl.I[0]-1]);
   //VecCl Y = SplineXIO::LimitBounds(y,nx,BSpl.D[1][0], BSpl.D[1][BSpl.I[1]-1]);
//        call bispev(tx,nx,ty,ny,c,kx,ky,x,mx,y,my,f,
//     *   wk,132,iw,22,ier)
//cout<<" Evaluate "<<" SX "<<SX<<" SY "<<SY<<"\n";cout<<" nx "<<nx<<"\n";
   for (int k=0;k<nx;k++)
     { 
      rx=x[k];ry=y[k];
      //rx=X[k+1];ry=Y[k+1];
      bispev_(BSpl.D[0],&SX,BSpl.D[1],&SY,BSpl.D[2],&px,&py,&rx,&n1,&ry,&ny,&rz,
              wk,&lwk,iw,&liw,&ier);
if (ier!=0) {cout<<" Bad Evaluate Spl "<<ier<<"\n";}
      z[k]=rz;
     }  

   delete wk;delete iw;
   return ier;
  };

int RegridSpline::Evaluate (TData<real> &dat)
  {
   real rx,ry,rz;
   TColumn<real> RD;//SetDifTData((TData&)RD,dat);
   (TData<real>&)RD=dat;
   int nx=RD.I[0];

   integer lwk=8,liw=2,ier,n1=1,ny=1,SX=BSpl.I[0],SY=BSpl.I[1];
   real *wk=new real[lwk];
   integer *iw=new integer[liw],px=3,py=3;
//        call bispev(tx,nx,ty,ny,c,kx,ky,x,mx,y,my,f,
//     *   wk,132,iw,22,ier)
//cout<<" Evaluate "<<" SX "<<SX<<" SY "<<SY<<"\n";
//cout<<" nx "<<nx<<"\n";
   for (int k=0;k<nx;k++)
     { 
      rx=RD.D[0][k];ry=RD.D[1][k];
      bispev_(BSpl.D[0],&SX,BSpl.D[1],&SY,BSpl.D[2],&px,&py,&rx,&n1,&ry,&ny,&rz,
              wk,&lwk,iw,&liw,&ier);
      if (ier!=0) cout<<"Very Bad ier in spl "<<ier<<"\n";
//cout<<" "<<ier;
      RD.D[2][k]=rz;
//cout<<RD.D[0][k]<<"  "<<RD.D[1][k]<<"  " <<RD.D[2][k]<<"\n";
     }  
   dat=(TData<real>&)RD;
   delete wk;delete iw;
   return ier;
  };



int CurveSpline::Generate(int &NumX,double &Misf,int Cont,TData<real> &dat,
              int Xcol,int Ycol,int Wcol)
  {
   
//cout<<" CurveSpline::Generate \n";
   real *Weight;
   int DelWeight=0;
   if ( ((Xcol==-1) && (dat.N!=3)) || (Wcol==-1))
     {
      if (Xcol==-1) {Xcol=0;Ycol=1;Wcol=2;}
      Weight=new real[dat.I[Xcol]];
      for (int k=0;k<dat.I[Xcol];k++) Weight[k]=1;
      DelWeight=1;
     }
   else Weight=dat.D[Wcol];
   if (Xcol==-1) {Xcol=0;Ycol=1;Wcol=2;}
   SetLimits(dat.D[Xcol][dat.I[Xcol]-1],dat.D[Xcol][0]);
//cout<<" CurveSpline::Generate equal\n";
//cout<<RD.I[0]<<"  "<<RD.I[1]<<" Misf "<<Misf<<" Cont "<<Cont<<"\n";
   int i[2]={NumX,NumX};BSpl.SetDim(2,i);

   integer NumXs=NumX,Conti=Cont,NumXd=dat.I[Xcol],polynom_degree=SplineOrder,Result,NumXs_r;
   integer NRWork=NumXd*(polynom_degree+1)+NumX*(3*polynom_degree+7),NIWork=NumX,*IWork;
   IWork=new integer[NIWork];
   real *RWork=new real[NRWork],MisfR=Misf,Mis_ret;
//cout<<" Generate begin Good\n";cout<<" NumXd "<<NumXd<<" NumIWork "<<NIWork<<" NRWork "<<NRWork<<"\n";bioskey(0);
//cout<<" Xup "<<Xup<<" Xlow "<<Xlow<<" NumXs "<<NumXs<<"\n";
//cout<<coreleft()<<"\n";

//      subroutine curfit(iopt,m,x,y,w,xb,xe,k,s,nest,n,t,c,fp,
//     * wrk,lwrk,iwrk,ier)
//   curfit_(&Conti,&NumXd,&dat.D[Xcol][-1],&dat.D[Ycol][-1],&Weight[-1],&Xlow,&Xup,&px,
//          &MisfR,&NumXs,&NumXs_r,BSpl.D[0],BSpl.D[1],&Mis_ret,RWork,&NRWork,
//          IWork,&Result);
   curfit_(&Conti,&NumXd,dat.D[Xcol],dat.D[Ycol],Weight,&Xlow,&Xup,&polynom_degree,
          &MisfR,&NumXs,&NumXs_r,BSpl.D[0],BSpl.D[1],&Mis_ret,RWork,&NRWork,
          IWork,&Result);
//cout<<" Generate Good "<<Result<<" SX "<<NumXs_r<<" Misf_ret "<<Mis_ret<<"\n";
//cout<<" NumXi "<<NumXi<<" NumYi "<<NumYi<<" DX "<<DX<<" DY "<<DY;
   NumX=NumXs_r;

   if (Mis_ret == 0 || Misf < Mis_ret * 0.5 || _isnan(Mis_ret))
       throw info_except("Could not make spline. Error %g have to be %g\n", Mis_ret, Misf);
       //Result *= 100;

   if (Result!=10 && Result != 1000)
     {BSpl.I[0]=NumX;BSpl.I[1]=NumX-polynom_degree-1;Delta=Mis_ret; }
else //if (Result!=0) 
 { cout<<" problems in CurveSpline::Generate; Result: "<<Result<<"\n";}

//   Bspl=Bspl;
   delete RWork;delete IWork;
   if (DelWeight) delete Weight;
//   Misf=Mis_ret;
   

   return Result;
  }; 

int CurveSpline::Generate(int &NumX,double &Misf,int Cont,double *x,double *y,int n,double *weight)
  {
   TDataF<double> tmp;tmp.Delete();
   TDataF<real> work;
   tmp.AddColumn(x,n);tmp.AddColumn(y,n);if (weight!=NULL) tmp.AddColumn(weight,n);
   SetDifTData(work,tmp);
//DataStore(cout,&work);
   work.SortData();
//DataStore(cout,&work);

//DataStore(cout,&work);
//DataStore(cout,&tmp);
   return (Generate(NumX,Misf,Cont,work)!=10);
  };


//int CurveSpline::Evaluate (double *x,double *y,int nx)
//  {
//   vector<real> X(nx), Y(nx);
//   for(size_t i = 0;i<X.size();i++){
//       X[i] = x[i];
//       Y[i] = y[i];
//   }
//
//   integer polynom_degree=SplineOrder,Num=nx,SX=BSpl.I[0],ier;
//   splev_(BSpl.D[0],&SX,BSpl.D[1],&polynom_degree,&X[0],&Y[0],&Num,&ier);
//   if (ier!=0) {
//       cout<<" Bad Evaluate Spl "<<ier<<"\n";
//   }
//   for(size_t i = 0;i<X.size();i++)
//       y[i] = Y[i];
//   return ier;
//  };

double CurveSpline::Evaluate (double x)
  {
    real y,x1=x;
    integer polynom_degree=SplineOrder,Num=1,SX=BSpl.I[0],ier;
    splev_(BSpl.D[0],&SX,BSpl.D[1],&polynom_degree,&x1,&y,&Num,&ier);
    if (ier!=0) {cout<<" Bad Evaluate Spl "<<ier<<"\n";}
    return y;
  };

int CurveSpline::Evaluate (TData<real> &dat,int Xcol,int Ycol)
  {
   if (Xcol==-1) {Xcol=0;Ycol=1;}
   return Evaluate(dat.D[Xcol], dat.D[Ycol], (int)dat.I[Xcol]);
  };

#include "lib\ref\data_manip.h"
int CurveSpline::Evaluate (double *x,double *y,int nx)
  {
   vector<real> X(nx), Y(nx);
   DataManip::CopyVector(X, x);
   DataManip::CopyVector(Y, y);

   integer polynom_degree=SplineOrder,Num=nx,SX=BSpl.I[0],ier;
   splev_(BSpl.D[0],&SX,BSpl.D[1],&polynom_degree,&X[0],&Y[0],&Num,&ier);
   if (ier!=0) {
       cout<<" Bad Evaluate Spl "<<ier<<"\n";
   }
   DataManip::CopyVector(y, Y);
   return ier;
  };









int CurveSpline_Noise::Generate(int &NumX,double &Misf,int Cont,TData<real> &dat,
              int Xcol,int Ycol,int Wcol)
  {
    if (Xcol==-1) {
        Xcol=0;Ycol=1;Wcol=2;
    }
    integer IER, M = SplineOrder, MD = Mode, N(dat.I[Xcol]);
    integer NY(N), K(1), NC(N), IWK(6*(N*M+1)+N);
    vector<real> WX(N),WY(K), C(NC), WK(IWK);
    real MDval = ModeVal;
    if (dat.N!=3)
        DataManip::SetVector(WX, 1);
    else
        DataManip::CopyVector(WX, dat.D[Wcol]);
    DataManip::SetVector(WY, 1);

    BSpl.Delete();
    BSpl.AddColumn(dat.D[Xcol], N);
    BSpl.AddColumn(&C[0], (int)C.size());

    gcvspl_(dat.D[Xcol], dat.D[Ycol], &NY, &WX[0], &WY[0], &M, &N, &K, &MD, &MDval, BSpl.D[1], &NC, &WK[0], &IER);

    Delta = WK[1];
    if (IER>2)
         cout<<" problems in CurveSpline_Noise::Generate; Result: "<<IER<<"\n";

   return IER;
  }; 
int CurveSpline_Noise::Evaluate (double *x,double *y,int nx){
   integer IDER = 0, M = SplineOrder, N = BSpl.I[0], L = N/2;
   vector<real> Q(2*M);
   for(size_t i = 0;i<(size_t)nx;i++){
       real X = x[i];
       y[i] = splder_(&IDER, &M, &N, &X, BSpl.D[0], BSpl.D[1], &L, &Q[0]);
   }
   return 1;
};

double CurveSpline_Noise::Evaluate (double x){
    real x1=x;
    integer IDER = 0, M = SplineOrder, N = BSpl.I[0], L = N/2;
    vector<real> Q(2*M);
    real X = x;
    return splder_(&IDER, &M, &N, &X, BSpl.D[0], BSpl.D[1], &L, &Q[0]);
};


