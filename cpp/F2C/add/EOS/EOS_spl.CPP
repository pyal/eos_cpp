#include "eos_spl.h"

#ifdef __cplusplus
extern "C" {
#endif
extern int bispev_(real *, integer *, real *, integer
 *, real *, integer *, integer *, real *, integer *, real *,
 integer *, real *, real *, integer *, integer *, integer *, 
 integer *);
#ifdef __cplusplus
}
#endif
/*
// ========================================================
// ================= Spl  EOS   ===========================
// ========================================================
*/
int EvalSpl(double *x,double *y,double *z,int nx,DatSpline *DatSpl)
  {
   real rx,ry,rz;
   integer lwk=8,liw=2,ier,n1=1,ny=1,SX=DatSpl->Num_SX,SY=DatSpl->Num_SY;
   real *wk=new real[lwk];
   integer *iw=new integer[liw],px=3,py=3;
   for (int k=0;k<nx;k++)
     { 
      rx=x[k];ry=y[k];
      bispev_(DatSpl->D_SX,&SX,DatSpl->D_SY,&SY,DatSpl->D_SZ,&px,&py,&rx,&n1,&ry,&ny,&rz,
              wk,&lwk,iw,&liw,&ier);
if (ier!=0) {cout<<" Bad Evaluate Spl "<<ier<<"\n";}
      z[k]=rz;
     }  

   delete wk;delete iw;
   return ier;
  };
int ReadSplStruct(FILE *in,DatSpline *DatSpl)
  {
   char tmp[256];double dtmp;int N;
   fscanf(in,"%s ",&tmp);// in>>tmp;
   if (Stricmp(tmp,"RegridSpline")!=0) return 0;
   fscanf(in,"%s %lf %s %lf %s %lf %s %lf %s %lf %s %s %i",
      &tmp,&dtmp,&tmp,&dtmp,&tmp,&dtmp,&tmp,&dtmp,&tmp,&dtmp,&tmp,&tmp,&N);
/* in>>tmp>>Delta>>tmp>>Xup>>tmp>>Xlow >>tmp>>Yup>>tmp>>Ylow >>tmp>>tmp>>N;*/
   if (N!=3) return 0;
   fscanf(in,"%s %i %s %i %s %i",
      &tmp,&DatSpl->Num_SX,&tmp,&DatSpl->Num_SY,&tmp,&DatSpl->Num_SZ);
/* in>>tmp>>DatSpl.Num_SX>>tmp>>DatSpl.Num_SY>>tmp>>DatSpl.Num_SZ;*/
   DatSpl->D_SX=new real[DatSpl->Num_SX+1];
   DatSpl->D_SY=new real[DatSpl->Num_SY+1];
   DatSpl->D_SZ=new real[DatSpl->Num_SZ+1];
   for (int p=0;p<DatSpl->Num_SX;p++) fscanf(in,"%f ",&DatSpl->D_SX[p]);
   for (    p=0;p<DatSpl->Num_SY;p++) fscanf(in,"%f ",&DatSpl->D_SY[p]);
   for (    p=0;p<DatSpl->Num_SZ;p++) fscanf(in,"%f ",&DatSpl->D_SZ[p]);
   return 1;
  };
void DeleteSplStruct(DatSpline *DatSpl) 
  { delete DatSpl->D_SX;delete DatSpl->D_SY;delete DatSpl->D_SZ; 
    DatSpl->D_SX=NULL;DatSpl->D_SY=NULL;DatSpl->D_SZ=NULL;};
void DeleteSpl(void **IntPar) 
  { 
   EOSSpline_struct *dat=(EOSSpline_struct *)(*IntPar);
   DeleteSplStruct(&dat->PSpl);if (dat->TempSpl) DeleteSplStruct(&dat->TSpl);
  };
int ReadSpl(FILE *hand,void **IntPar)
  {
/*   DeleteVar();*/
   *IntPar=new EOSSpline_struct;
   EOSSpline_struct *dat=(EOSSpline_struct *)(*IntPar);
   char tmp[256];
   fscanf(hand,"%s ",tmp);//in>>tmp;
   FILE *hand1;
   if( (hand1  = fopen( tmp, "r" )) == NULL ) {printf( "The file %s was not opened\n",tmp );return 0;}
   strcpy(dat->spl_name,tmp);
   fscanf(hand1,"%s %lf %s %lf %s %lf %s %lf ",&tmp,&(dat->AddE),&tmp,&(dat->AddP),&tmp,&(dat->DencCold),&tmp,&(dat->DencMax));
/*   sp>>tmp>>dat->AddE>>tmp>>dat->AddP>>tmp>>dat->DencCold>>tmp>>dat->DencMax;*/
   int ret=ReadSplStruct(hand1,&dat->PSpl);
   if (ret) 
     if (!ReadSplStruct(hand1,&dat->TSpl) ) dat->TempSpl=0;else dat->TempSpl=1;
   return ret;
  };

double PressureSpl(double Dencity,double Energy,void *IntPar) 
  {
   EOSSpline_struct *dat=(EOSSpline_struct *)IntPar;
   double  res,E=-6,D=-9;
   double AddE=dat->AddE,AddP=dat->AddP;
   if (Energy+AddE>StndErr) E=log(Energy+AddE);
   if (Dencity>StndErr) D=log(Dencity);
   int er=EvalSpl(&E,&D,&res,1,&dat->PSpl);
   if (er!=0) {res=1;cout<<" Very Bad!!\n";}
   else res=exp(res)-AddP;
   return res;
  };
double SoundSpl(double Dencity,double Energy,void *IntPar)
  {
   double dr=50*Dencity*StndErr,r=Dencity;
   if (r<MathZer) { /*cout<<"MatterSpl::Sound; Dencity<MathZer :"<<Dencity<<"\n";*/
                    return 0.1;}
   double P=PressureSpl(r,Energy,IntPar);
   double Pp=PressureSpl(r+dr,Energy+P*dr/(r*r),IntPar);Pp-=P;
   if (Pp<0) {/*cout<<" MatterIO::Sound;  deriv is negative: " <<Pp<<" Sqrt from negative value          \n";*/
      Pp=0.1;}
   double S=sqrt( max<double>(0,Pp)/dr );
   return S;
  };
      
double TemperatureSpl(double Dencity,double Energy,void *IntPar)
  { 
   double res;
   EOSSpline_struct *dat=(EOSSpline_struct *)IntPar;
   if (dat->TempSpl)
     {
      double E=-6,D=-9;
      double AddE=dat->AddE;
      if (Energy+AddE>StndErr) E=log(Energy+AddE);
      if (Dencity>StndErr) D=log(Dencity);
      int er=EvalSpl(&E,&D,&res,1,&dat->TSpl);
      if (er!=0) {res=1;}/*cout<<" Very Bad Temp!!\n";}*/
      else res=exp(res);
     } 
   else res=0;
   return res;
  };
/*
//=====================================================
//=============== RegridSpl ===========================
//=====================================================
*/
