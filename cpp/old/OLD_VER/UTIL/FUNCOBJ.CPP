#include "funcobj.h"
   
   
void FAZer::DeleteFAZer()
  {
   if (name!=NULL) 
     {
      for (int k=1;k<=NumPar;k++) delete namepar[k];
      delete name;name=NULL;NumPar=0;
     }
  };

//=====================================================================
//=========================    FAZerIO ===============================
//=====================================================================

FAZerIO::FAZerIO(){FuncOne=NULL;Par=NULL;Stp=NULL;
//cout<<" Mem after FAZerIO() "<<coreleft()<<"\n";
 };
FAZerIO::~FAZerIO(){DeleteFAZerIO(); };
void FAZerIO::DeleteFAZerIO()
  {delete Par;Par=NULL;delete Stp;Stp=NULL;
   DeleteFAZer();
//cout<<" Mem after FAZerIODelete "<<coreleft()<<"\n";
  };

double FAZerIO::Clc(double *par,double Xval)
  {SetPar(par);FAZer::Clc(Par,Xval);};
double FAZerIO::Clc(double Xval){return FAZer::Clc(Par,Xval);};
void   FAZerIO::Clc(double *Xval,double *Res) { FAZer::Clc(Par,Xval,Res);};
 
void FAZerIO::GetPar(double *par)
  {movmem(Par,par,sizeof(double)*(Par[0]+1));};
void FAZerIO::SetPar(double *par)
  {movmem(&par[1],&Par[1],sizeof(double)*Par[0]);};
void FAZerIO::GetParStp(double *par,double *stp)
  {
   movmem(Par,par,sizeof(double)*(Par[0]+1));
   movmem(Stp,stp,sizeof(double)*(Par[0]+1));
   };
void FAZerIO::SetParStp(double *par,double *stp)
  {movmem(&par[1],&Par[1],sizeof(double)*Par[0]);
   movmem(&stp[1],&Stp[1],sizeof(double)*Par[0]);
  };

void FAZerIO::Output(ostream &output)
  {
   char tmp[256];
   output<<name<<"\n";
   for (int k=1;k<=NumPar;k++)
     {
      strcpy(tmp,namepar[k]);SetLength(tmp,15);
      output<<tmp<<"  ";
      if (Stp[k]==0) output<<" F     ";
      else           output     <<" V     ";
      output<<Par[k]<<"\n";
     }
   return output;
  };

void FAZerIO::Input(istream &input);
  {
   char tmp[256];
//   input>>tmp;  it was name for registration
   if (name!=NULL) DeleteFAZerIO();
   Stp=new double [NumPar+1];Stp[0]=NumPar;
   Par=new double [NumPar+1];Par[0]=NumPar;
   for (int k=1;k<=NumPar;k++)
     {
      input>>tmp>>tmp;
      if (!Stricmp(tmp,"f")) Stp[k]=0;
      else                   Stp[k]=1;
      input>>Par[k];
     }
   ParStp(Par,Stp);
   return input;
  };


int Register(char *name,FAZerIO &Func)
  {
   FAZerIO *tmp;
   int ret=1;
   if (!Stricmp(name,"exponent")) {tmp=new FAZerIOExp;}
   else
     if (!Stricmp(name,"lorents")) {tmp=new FAZerIOLor;}
     else if (!Stricmp(name,"polynom")) {tmp=new FAZerIOPoly;}
          else if (!Stricmp(name,"sum_func"))
                 {tmp=new FAArrAdd;}
               else
                 if (!Stricmp(name,"set_func")) {tmp=new FAArrSet;}
                       else ret=0;
   return ret;
   if (!ret)
     {
      cout<<" There is no such function "<<tmp<<"\n";
//      exit(0);
     }
   else
      {
       if (Func!=NULL) delete(Func);
       Func=tmp;
      }
//cout<<" Mem after FAZerIORegister  "<<coreleft()<<"\n";
   return ret;
  };




//=====================================================================
//=========================    FAZerArrIO =========================
//=====================================================================


FAArr::FAArr(){NumFunc=0;
//cout<<" Mem after FAZerArrIO() "<<coreleft()<<"\n";
 };
FAArrIO::~FAZerArrIO()
  {DeleteFAArr();};
void FAArr::DeleteFAArr()
  {
   for (int k=1;k<=NumFunc;k++) delete FuncArr[k];
   NumFunc=0;
//cout<<" Mem after FAZerArrIODelete "<<coreleft()<<"\n";
  };

double FAArr::Clc(double *par,double Xval)
  {
   SetPar(par);return Clc(Xval);
  };
void   FAArr::Clc(double *Xval,double *Res) { Clc(Xval,Res);};
void FAArr::GetPar(double *par)
  {
   int Cur=0;
   double tmp=NumParArr[0];
   for (int k=1;k<=NumFunc;k++)
     { FuncArr[k]->GetPar(&par[Cur]);par[Cur]=tmp;
       Cur+=NumParArr[k];tmp=par[Cur];}
  };
void FAArr::SetPar(double *par)
  {
   int Cur=0;
   for (int k=1;k<=NumFunc;k++)
     { FuncArr[k]->SetPar(&par[Cur]);Cur+=NumParArr[k];}
//   par[0]=Cur;
  };
void FAArr::GetParStp(double *par,double *stp)
  {
   int Cur=0;
   double tmppar=NumParArr[0],tmpstp=NumParArr[0];
   for (int k=1;k<=NumFunc;k++)
     { FuncArr[k]->GetParStp(&par[Cur],&stp[Cur]);par[Cur]=tmppar;stp[Cur]=tmpstp;
       Cur+=NumParArr[k];tmppar=par[Cur];tmpstp=stp[Cur];}
  };
void FAArr::SetParStp(double *par,double *stp)
  {
   int Cur=0;
   for (int k=1;k<=NumFunc;k++)
     { FuncArr[k]->SetParStp(&par[Cur],&stp[Cur]);Cur+=NumParArr[k];}
  };
//ostream& operator<<(ostream &output,FAArr &Func)
void FAArr::Output(ostream &output)
  {
//   output<<name<<"    "<<NumFunc<<"\n";
   for (int k=1;k<=NumFunc;k++) output<<(*FuncArr[k]);
//   return output;
  };
//istream& operator>>(istream &input,FAArr &Func)
void FAArr::Input(istream &input)
  {
cout<<" Begin FAArr Input "<<"\n";
   int Cur=0;
//   if (NumFunc!=0) DeleteFAArr();
//   input>>NumFunc;             // Dont no why but it was a remark
   for (int k=1;k<=NumFunc;k++)
     {
      FuncArr[k]=new FAZerIO;
      input>>(*FuncArr[k]);NumParArr[k]=FuncArr[k]->Par[0];
      Cur+=NumParArr[k];
     }
    NumParArr[0]=Cur;
cout<<" End FuncSumZerInput\n";
//    return input;
  };


//=====================================================================
//=========================    FAZerExp =========================
//=====================================================================

FAZerExp::FAZerExp()
  {
   name=strdup("exponent");
   NumPar=4;
   namepar[1]=strdup("Ground   ");
   namepar[2]=strdup("Intencity");
   namepar[3]=strdup("Position ");
   namepar[4]=strdup("1/alpha_in_exp");
  };
double FAZerExp::Clc(double *par,double Xval)
  {
   if (Xval<par[3]) return par[1];
   return par[1]+par[2]*(1-exp((par[3]-Xval)/par[4]));
  }
void   FAZerExp::Clc(double *par,double *Xval,double *Res)
  {
   double gr=par[1],in=par[2],pos=par[3],al=par[4];
   for (int k=1;k<=Xval[0];k++)
     if (Xval[k]<pos) Res[k]=gr;
     else Res[k]=gr+in*(1-exp((pos-Xval[k])/al));
  };
void FAZerExp::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };
FAZerExp::~FAZerExp(){DeleteFAZer();};

//=====================================================================
//=========================    FAZerLor =========================
//=====================================================================

FAZerLor::FAZerLor()
  {
   name=strdup("lorents");
   NumPar=5;
   namepar[1]=strdup("Ground   ");
   namepar[2]=strdup("Intencity");
   namepar[3]=strdup("Position ");
   namepar[4]=strdup("Width    ");
   namepar[5]=strdup("4-ord    ");
//cout<<" Mem after FAZerLor() "<<coreleft()<<"\n";
  };
double FAZerLor::Clc(double *par,double Xval)
  {
   double misfit=par[3]-Xval ;
   return par[1]+ par[2]*sqr(par[4]/2)/
                (sqr(par[4]/2)+sqr(misfit)+par[5]*sqr(sqr(misfit)));
  }
void   FAZerLor::Clc(double *par,double *Xval,double *Res) 
  {
   double gr=par[1],in=par[2]*sqr(par[4]/2),
          wi=sqr(par[4]/2),pos=par[3],al=par[5];
   double misfit;
   for (int k=1;k<=Xval[0];k++)
     {
      misfit=sqr(pos-Xval[k]);
      Res[k]=gr+in/(wi+misfit+al*sqr(misfit));
     }
  };
void FAZerLor::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
 };
FAZerLor::~FAZerLor() {DeleteFAZer();};
//cout<<" Mem after ~FAZerLor() "<<coreleft()<<"\n";
   };

//=====================================================================
//=========================    FAZerPoly ========================
//=====================================================================


FAZerPoly::FAZerPoly()
  {
   name=strdup("polynom");
   NumPar=8;
   namepar[1]=strdup("Ground   ");
   namepar[2]=strdup("Intencity");
   namepar[3]=strdup("Position ");
   namepar[4]=strdup("second   ");
   namepar[5]=strdup("third    ");
   namepar[6]=strdup("fourth   ");
   namepar[7]=strdup("fifth    ");
   namepar[8]=strdup("sixth    ");
  };
double FAZerPoly::Clc(double *par,double Xval)
  {
   double curx=Xval-par[3],sum=0,x0;x0=curx;
   for (int k=1;k<=5;k++) { sum+=curx*par[3+k];curx*=x0;}
   return par[1]+par[2]*sum;
  };
void   FAZerPoly::Clc(double *par,double *Xval,double *Res)
  {
   double gr=par[1],in=par[2],pos=par[3],
          c1=par[4],c2=par[5],c3=par[6],c4=par[7],c5=par[8];
   double misfit,sum,x1,x2,x3,x4,x5;
   for (int k=1;k<=Xval[0];k++)
     {
      x1=Xval[k]-pos;x2=x1*x1;x3=x2*x1;x4=x3*x1;x5=x4*x1;
      Res[k]=gr+in*(x1*c1+x2*c2+x3*c3+x4*c4+x5*c5);
     }
  };
void FAZerPoly::ParStp(double *par,double *stp)
 {
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                   else stp[k]=par[k]*0.01;
  stp[3]*=0.1;
 };
FAZerPoly::~FAZerPoly() {DeleteFAZer();};
   



//=====================================================================
//=========================    FAArrAdd       =========================
//=====================================================================

FAArrAdd::FAArrAdd(){name=strdup("sum_func");
//cout<<" Mem after FAArrAdd() "<<coreleft()<<"\n";
 };
FAArrAdd::~FAArrAdd(){DeleteFAZer();
//cout<<" Mem after ~FAArrAdd() "<<coreleft()<<"\n";
 };
double FAArrAdd::Clc(double Xval)
  {
   double result=0;
   for (int k=1;k<=NumFunc;k++) 
     {
      result+=FuncArr[k]->Clc(Xval);
     }
   return result;
  };
void   FAArrAdd::Clc(double *Xval,double *Res)
  {
   double *tmp=new double[Xval[0]+1];
   FuncArr[1]->Clc(Xval,tmp);movmem(&tmp[1],&Res[1],sizeof(double )*Xval[0]);
   for (int k=2;k<=NumFunc;k++)
     {
      FuncArr[k]->Clc(Xval,tmp);
      for (int k1=2;k1<=Xval[0];k1++) Res[k1]+=tmp[k1];
     }
   delete tmp;
  };


//ostream& operator<<(ostream &output,FAArrAdd &Func)
//  {output<<(FAArr&)(Func);return output;};
//istream& operator>>(istream &input,FAArrAdd &Func)
//  {input>>(FAArr&)(Func);return input;};

//=====================================================================
//=========================    FAArrSet       =========================
//=====================================================================


FuncSumCellSet::FuncSumCellSet()
  {
   name=strdup("set_func");
//   namepar[1]=strdup("first_bnd");
//   namepar[2]=strdup("second_bnd");
  };
FuncSumCellSet::~FuncSumCellSet(){DeleteFAZer(); };
double FuncSumCellSet::Clc(double Xval)
  {
   int k=1;
   while ((k<NumFunc) && (Xval>Bnd[k])) k++;
   double coef1=0,coef2=0;
   double *Shf=FormShf();
   if (k>1)       coef1=exp(-fabs(Xval-Bnd[k-1])-3);
   if (k<NumFunc) coef2=exp(-fabs(Xval-Bnd[k])-3);
   double result=(FuncArr[k]->Clc(Xval)+Shf[k])*(1-coef1-coef2);
   if (coef1!=0) result+=(FuncArr[k-1]->Clc(Xval)+Shf[k-1])*coef1;
   if (coef2!=0) result+=(FuncArr[k+1]->Clc(Xval)+Shf[k+1])*coef2;
   delete Shf;
   return result;
  };
void SetXVal(int r,int l,double *Xval,double *tmpx)
  { movmem(&Xval[r],&tmpx[1],sizeof(double)*(l-r));tmpx[0]=l-r;};
double *FuncSumCellSet::FormShf()
  {
   double *ret=new double[NumFunc+1];ret[1]=0;
   double shf=0;
   for (int k=2;k<=NumFunc;k++)
     {
      shf+=FuncArr[k-1]->Clc(Bnd[k-1])-FuncArr[k]->Clc(Bnd[k-1]);
      ret[k]=shf;
     }
   return ret;
  }
   
void   FuncSumCellSet::Clc(double *Xval,double *Res)
  {
//cout<<" Begin Set Clc \n";
   if (NumFunc==1) { FuncArr[1]->Clc(Xval,Res);return;}
   double *tmpy1=new double[Xval[0]+1];
   double *tmpy2=new double[Xval[0]+1];
   double *tmpy3=new double[Xval[0]+1];
   double *tmpx=new double[Xval[0]+1];
   movmem(Xval,tmpx,sizeof(double )*Xval[0]+1);
   movmem(Xval,tmpy1,sizeof(double )*Xval[0]+1);
   movmem(Xval,tmpy2,sizeof(double )*Xval[0]+1);
   movmem(Xval,tmpy3,sizeof(double )*Xval[0]+1);
   int CurBnd=1,k=1;
   double coef1,coef2,sh1,sh2,sh0;
   int *Pos=new int[NumFunc+1];Pos[0]=1;
   double *Shf=FormShf();
//cout<<" Clc Pos Bnd -> 1 ";
   while ((k<Xval[0]) && (CurBnd<NumFunc))
     {
      while ((Bnd[CurBnd]>Xval[k]) && (k<Xval[0])) k++;
      Pos[CurBnd]=k;
      CurBnd++;
//cout<<k<<"   ";
     }
   Pos[CurBnd]=Xval[0]+1;
//cout<<" \n";
//   for (k=CurBnd+1;k<=NumFunc;k++) Pos[k]=Xval[0];
   for (k=1;k<=CurBnd;k++)
     {
      SetXVal(Pos[k-1],Pos[k],Xval,tmpx);
      FuncArr[k]->Clc(tmpx,&tmpy2[Pos[k-1]-1]);
      if (k>1)       FuncArr[k-1]->Clc(tmpx,&tmpy1[Pos[k-1]-1]);
      if (k<NumFunc) FuncArr[k+1]->Clc(tmpx,&tmpy3[Pos[k-1]-1]);
      sh1=0;sh2=0;
      sh0=Shf[k];
      if (k!=1) sh1=Shf[k-1];
      if (k!=NumFunc) sh1=Shf[k+1];
      for (int i=Pos[k-1];i<Pos[k];i++)
        {
         coef1=0;coef2=0;            
         if (k!=1      )  coef1=exp(-fabs(Xval[i]-Bnd[k-1])-3);
         if (k!=NumFunc)  coef2=exp(-fabs(Xval[i]-Bnd[k+1])-3);
         Res[i]=coef1*(tmpy1[i]+sh1)+(1-coef1-coef2)*(tmpy2[i]+sh0)+
                coef2*(tmpy3[i]+sh2);
//cout<<Res[i]<<"    ";
        }
//cout<<"\n  k= "<<k<<"  Pos1 "<<Pos[k-1]<<"  Pos2  "<<Pos[k]
//    <<"  Num  "<<tmpx[0]<<"\n";
//ChRead();
     }
   delete tmpx;
   delete tmpy1;delete tmpy2;delete tmpy3;delete Pos;delete Shf;
//cout<<"End Clc Set\n";
  };

void FuncSumCellSet::SetPar(double *par)
  {
   for (int k=1;k<=NumCor;k++)
      par[correlatedpar[1][k]]=par[correlatedpar[0][k]];
   movmem(&par[1],&Bnd[1],sizeof(double)*(NumFunc-1));
   FAArr::SetPar(&par[NumFunc-1]);
//cout<<"SetPar\n";
//for (k=1;k<=NumFunc-1;k++) {cout<<par[k]<<"   ";}
//cout<<"\n";
  };
void FuncSumCellSet::SetParStp(double *par,double *stp)
  {
   int k;
//cout<<"SetParStp\n";
//for (k=1;k<=NumFunc-1;k++) {cout<<par[k]<<"   "<<stp[k]<<"  ";}
//cout<<"\n";
   for (k=1;k<=NumCor;k++) 
      par[correlatedpar[1][k]]=par[correlatedpar[0][k]];
   movmem(&par[1],&Bnd[1],sizeof(double)*(NumFunc-1));
   movmem(&stp[1],&BndStp[1],sizeof(double)*(NumFunc-1));
   FAArr::SetParStp(&par[NumFunc-1],&stp[NumFunc-1]);
//cout<<"SetParStp\n";
//for (k=1;k<=NumFunc-1;k++) {cout<<par[k]<<"   "<<stp[k]<<"  ";}
//cout<<"\n";
  };
void FuncSumCellSet::GetParStp(double *par,double *stp)
  {
   FAArr::GetParStp(&par[NumFunc-1],&stp[NumFunc-1]);
   movmem(&Bnd[1],&par[1],sizeof(double)*(NumFunc-1));
   movmem(&BndStp[1],&stp[1],sizeof(double)*(NumFunc-1));
   par[0]=GetNumPar();stp[0]=GetNumPar();
//cout<<"GetParStp\n";
//for (int k=1;k<=NumFunc-1;k++) {cout<<par[k]<<"   "<<stp[k]<<"  ";}
//cout<<"\n";
  };
void FuncSumCellSet::GetPar(double *par)
  {
   FAArr::GetPar(&par[NumFunc-1]);
   movmem(&Bnd[1],&par[1],sizeof(double)*(NumFunc-1));
   par[0]=GetNumPar();
//cout<<"GetPar\n";
//for (int k=1;k<=NumFunc-1;k++) {cout<<par[k]<<"   ";}
//cout<<"\n";
  };
//ostream& operator<<(ostream &output,FuncSumCellSet &Func)
void FuncSumCellSet::Output(ostream &output)
  {
   for (int k=1;k<NumFunc;k++) 
     {
      output<<"Bnd        ";
      if (BndStp[k]==0) output<<" F     ";
      else              output<<" V     ";
      output<<Bnd[k]<<"\n";
     }
   output<<"Num_Correlated_Par        "<<NumCor<<"\n";
   for (k=1;k<=NumCor;k++) 
     output<<correlatedpar[0][k]<<"    "<<correlatedpar[1][k]<<"\n";
   FAArr::Output(output);
//   return output;
  };
void FuncSumCellSet::Input(istream &input)
//istream& operator>>(istream &input,FuncSumCellSet &Func)
  {
//cout<<" input set_func\n";
   char tmp[256];
   for (int k=1;k<NumFunc;k++) 
     { 
      input>>tmp>>tmp>>Bnd[k];
      if (Stricmp(tmp,"f")==0) BndStp[k]=0;
      else                     BndStp[k]=1;
     }
   input>>tmp>>NumCor;
//cout<<" input set_func NumCor "<<tmp<<"    "<<NumCor<<"\n";
   for (k=1;k<=NumCor;k++) 
     input>>correlatedpar[0][k]>>correlatedpar[1][k];      
//cout<<" begin internal input\n";
//   input>>tmp;
   FAArr::Input(input);
   double *tmp1=new double[GetNumPar()+1];
   GetPar(tmp1);SetPar(tmp1);
   delete tmp1;
//   input>>(FAArr&)(Func);
//   return input;
  };
