#include <lib\precompiled\Messb.h>

#include "fastd.h"

//=====================================================================
//=========================    Just Known Functions ===================
//=====================================================================

//=====================================================================
//=========================    FAZerExp ===============================
//=====================================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerExp(){return new FAZerExp();}
static ClassDesc FAZerExp_ClassDesc(typeid(FAZerExp),"exponent",1,"FAZer",Dummy_FAZerExp);
#else
static ClassDesc FAZerExp_ClassDesc(typeid(FAZerExp),"exponent",1,"FAZer",create<FAZerExp>);
#endif

FAZerExp::FAZerExp()
  {
   name=strdup("exponent");
   helppar[0]=strdup(" Calculated function is:\n Pos-x>0? F[x] = Gr+Int*(1-exp[(Pos-x)/Alpha]);\nPos-x<0? F[x]=0\nSo F[x<=0]=Gr;F[x->inf]=Gr+Int\nAl>0");
   int i=SetStdPar_GrInt(namepar,valpar,helppar,1);
   namepar[i+1]=strdup("  Position    ");valpar[i+1]=0;	helppar[i+1]=strdup("Pos value");
   namepar[i+2]=strdup("1/alpha_in_exp");valpar[i+2]=1;	helppar[i+2]=strdup("Alpha value");
   NumPar=i+2;
  };
void   FAZerExp::ClcBase(double *par,double *Xval,double *Res)
  {
   double pos=par[3],al=par[4],tmp;
   for (int k=1;k<=Xval[0];k++)
      if ( (Xval[k]<pos) || (par[4]<MathZer) ) Res[k]=0;
      else 
        {tmp=(pos-Xval[k])/al;
         if ((fabs(tmp)>100) || (fabs(tmp)<1e-10)) Res[k]=0;
// Std makes Gr-Int*F
         else Res[k]=-(1-exp(tmp));
        }
  };

//=====================================================================
//=========================    FAZerLor =========================
//=====================================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerLor(){return new FAZerLor();}
static ClassDesc FAZerLor_ClassDesc(typeid(FAZerLor),"lorents",1,"FAZer",Dummy_FAZerLor);
#else
static ClassDesc FAZerLor_ClassDesc(typeid(FAZerLor),"lorents",1,"FAZer",create<FAZerLor>);
#endif

FAZerLor::FAZerLor()
  {
   name=strdup("lorents");
   helppar[0]=strdup(" Calculated function is:\n F[x] = Gr+Int*(W/2)^2/[(W/2)^2+(P-x)^2+A*(P-x)^4]");
   int i=SetStdPar_GrInt(namepar,valpar,helppar,1);
   namepar[i+1]=strdup("Position ");valpar[i+1]=0;	helppar[i+1]=strdup("Pos value   -P");
   namepar[i+2]=strdup("  Width  ");valpar[i+2]=1;	helppar[i+2]=strdup("Width value -W");
   namepar[i+3]=strdup("  4-ord  ");valpar[i+3]=0;	helppar[i+3]=strdup("Alpha value -A");
   NumPar=i+3;
  };
void FAZerLor::ClcBase(double *par,double *Xval,double *Res)
  {
   double Pos=par[3], Al=par[5], W2=sqr(par[4]/2);
   for (int k=1;k<=Xval[0];k++){
		double misfit = Pos-Xval[k];
// Std makes Gr-Int*F
		Res[k] = -W2 / (W2 + sqr(misfit) + Al*sqr( sqr(misfit) ));
   }
  }

//=====================================================================
//=========================    FAZerPoly ========================
//=====================================================================

#ifndef WCPP_NET
DescribedClass * Dummy_FAZerPoly(){return new FAZerPoly();}
static ClassDesc FAZerPoly_ClassDesc(typeid(FAZerPoly),"polynom",1,"FAZer",Dummy_FAZerPoly);
#else
static ClassDesc FAZerPoly_ClassDesc(typeid(FAZerPoly),"polynom",1,"FAZer",create<FAZerPoly>);
#endif

FAZerPoly::FAZerPoly()
  {
   name=strdup("polynom");
   helppar[0]=strdup(" Calculated function is:\n dX=Pos - x\nF[x] = Gr+Int*(a1*dX+a2*dX^2+a3*dX^3+a4*dX^4+a5*dX^5)");
   int i=SetStdPar_GrInt(namepar,valpar,helppar,1);
   namepar[i+1]=strdup("Position ");valpar[i+1]=0;	helppar[i+1]=strdup("Pos value");
   namepar[i+2]=strdup(" First   ");valpar[i+2]=0;	helppar[i+2]=strdup("a1  value");
   namepar[i+3]=strdup(" Second  ");valpar[i+3]=0;	helppar[i+3]=strdup("a2  value");
   namepar[i+4]=strdup(" Third   ");valpar[i+4]=0;	helppar[i+4]=strdup("a3  value");
   namepar[i+5]=strdup(" Fourth  ");valpar[i+5]=0;	helppar[i+5]=strdup("a4  value");
   namepar[i+6]=strdup(" Fifth   ");valpar[i+6]=0;	helppar[i+6]=strdup("a5  value");
   NumPar=i+6;
  };
void   FAZerPoly::ClcBase(double *par,double *Xval,double *Res)
  {
   double pos=par[3],
          c1=par[4],c2=par[5],c3=par[6],c4=par[7],c5=par[8];
   double x1,x2,x3,x4,x5;//,sum,misfit
   for (int k=1;k<=Xval[0];k++)
     {
      x1=Xval[k]-pos;x2=x1*x1;x3=x2*x1;x4=x3*x1;x5=x4*x1;
// Std makes Gr-Int*F
      Res[k]=-(x1*c1+x2*c2+x3*c3+x4*c4+x5*c5);
     }
  };

//=====================================================================
//=========================    FAZerGaus      =========================
//=====================================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAZerGaus(){return new FAZerGaus();}
static ClassDesc FAZerGaus_ClassDesc(typeid(FAZerGaus),"gaus",1,"FAZer",Dummy_FAZerGaus);
#else
static ClassDesc FAZerGaus_ClassDesc(typeid(FAZerGaus),"gaus",1,"FAZer",create<FAZerGaus>);
#endif

FAZerGaus::FAZerGaus()
  {
   name=strdup("gaus");
   helppar[0]=strdup(" Calculated function is:\n F[x] = Gr+Int*Exp{-[(P-x)/W]^2}");
   int i=SetStdPar_GrInt(namepar,valpar,helppar,1);
   namepar[i+1]=strdup("Position ");valpar[i+1]=0;	helppar[i+1]=strdup("Pos value   -P");
   namepar[i+2]=strdup("  Width  ");valpar[i+2]=1;	helppar[i+2]=strdup("Width value -W");
   NumPar=i+2;
  };
void   FAZerGaus::ClcBase(double *par,double *Xval,double *Res) 
  {
   double gr=par[1],in=par[2],
          wi=sqr(par[4]),pos=par[3];
   double misfit;
   for (int k=1;k<=Xval[0];k++)
     {
      misfit=sqr(pos-Xval[k]);
// Std makes Gr-Int*F
      Res[k]=-exp(-misfit/wi);
     }
  };

//=====================================================================
//=========================    Just Arrays Functions ==================
//=====================================================================

//=====================================================================
//=========================    FAArrAdd       =========================
//=====================================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FAArrAdd(){return new FAArrAdd();}
static ClassDesc FAArrAdd_ClassDesc(typeid(FAArrAdd),"sum_func",1,"FAIO",Dummy_FAArrAdd);
#else
static ClassDesc FAArrAdd_ClassDesc(typeid(FAArrAdd),"sum_func",1,"FAIO",create<FAArrAdd>);
#endif
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
   FuncArr[1]->Clc(Xval,Res);
//MemoryMove(&tmp[1],&Res[1],sizeof(double )*(Xval[0]));
   for (int k=2;k<=NumFunc;k++)
     {
      FuncArr[k]->Clc(Xval,tmp);
      for (int k1=1;k1<=Xval[0];k1++) Res[k1]+=tmp[k1];
     }
   delete tmp;
  };

void FAArrAdd::Output(ostream &output)
  {
   output<<name<<"    "<<NumFunc<<"\n";
   FAArr::Output(output);
  };

void FAArrAdd::Input(istream &input)
  {
   input>>NumFunc;
   FAArr::Input(input);
  };
void FAArrAdd::DrawHelp(ostream &out)
{
	FAArr::DrawHelp(out);
	out<<" \n"<<name<<" is a simple function working over functions\n";
	out<<" It sums <NumFunc> functions in one\n";
	out<<" The result function is: Res[Xval]=Sum[i](Func[i,Xval])\n\n";

}


//=====================================================================
//=========================    FAArrSet       =========================
//=====================================================================

#ifndef WCPP_NET
DescribedClass * Dummy_FAArrSet(){return new FAArrSet();}
static ClassDesc FAArrSet_ClassDesc(typeid(FAArrSet),"set_func",1,"FAIO",Dummy_FAArrSet);
#else
static ClassDesc FAArrSet_ClassDesc(typeid(FAArrSet),"set_func",1,"FAIO",create<FAArrSet>);
#endif

//FAArrSet::~FAArrSet(){DeleteFAZer(); };
void FAArrSet::DeleteFAArrSet()
  { 
//cout<<"FAArrSet::DeleteFAArrSet; delete; NumFunc "<<NumFunc<<"\n";
   if ((name!=NULL) && (NumFunc>1))
     for (int k=1;k<=MaxIter;k++) delete BndStp[k];
   FAArrSet::DeleteFAArr(); 
   };
double FAArrSet::Clc(double Xval)
  {
   int k=1;
   while ((k<NumFunc) && (Xval>Bnd[k])) k++;
   double coef1=0,coef2=0;
   double *Shf=FormShf();
   if (k>1)       coef1=exp(-fabs(Xval-Bnd[k-1])*CorFuncExpConst-CorFuncConst0);
   if (k<NumFunc) coef2=exp(-fabs(Xval-Bnd[k])*CorFuncExpConst-CorFuncConst0);
   double result=(FuncArr[k]->Clc(Xval)+Shf[k])*(1-coef1-coef2);
   if (coef1!=0) result+=(FuncArr[k-1]->Clc(Xval)+Shf[k-1])*coef1;
   if (coef2!=0) result+=(FuncArr[k+1]->Clc(Xval)+Shf[k+1])*coef2;
   delete Shf;
   return result;
  };
void SetXVal(int r,int l,double *Xval,double *tmpx)
  { MemoryMove(&Xval[r],&tmpx[1],sizeof(double)*(l-r));tmpx[0]=l-r;};
double *FAArrSet::FormShf()
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
   
void   FAArrSet::Clc(double *Xval,double *Res)
  {
//cout<<" Begin Set Clc \n";
   if (NumFunc==1) { FuncArr[1]->Clc(Xval,Res);return;}
   double *tmpy1=new double[Xval[0]+1];
   double *tmpy2=new double[Xval[0]+1];
   double *tmpy3=new double[Xval[0]+1];
   double *tmpx=new double[Xval[0]+1];
   MemoryMove(Xval,tmpx,sizeof(double )*Xval[0]+1);
   MemoryMove(Xval,tmpy1,sizeof(double )*Xval[0]+1);
   MemoryMove(Xval,tmpy2,sizeof(double )*Xval[0]+1);
   MemoryMove(Xval,tmpy3,sizeof(double )*Xval[0]+1);
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
         if (k!=1      )  coef1=exp(-fabs(Xval[i]-Bnd[k-1])*CorFuncExpConst-CorFuncConst0);
         if (k!=NumFunc)  coef2=exp(-fabs(Xval[i]-Bnd[k+1])*CorFuncExpConst-CorFuncConst0);
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

void FAArrSet::SetParStp(double *par,double *stp)
  {
   int k;
//cout<<"SetParStp\n";
//for (k=1;k<=NumFunc-1;k++) {cout<<par[k]<<"   "<<stp[k]<<"  ";}
//cout<<"\n";
   for (k=1;k<=NumCor;k++) 
      par[correlatedpar[1][k]]=par[correlatedpar[0][k]]*correlatedcoef[k];
   MemoryMove(&par[1],&Bnd[1],sizeof(double)*(NumFunc-1));
   MemoryMove(&stp[1],&BndStp[CurIter][1],sizeof(double)*(NumFunc-1));
   FAArr::SetParStp(&par[NumFunc-1],&stp[NumFunc-1]);
//cout<<"SetParStp\n";
//for (k=1;k<=NumFunc-1;k++) {cout<<par[k]<<"   "<<stp[k]<<"  ";}
//cout<<"\n";
  };
void FAArrSet::GetParStp(double *par,double *stp)
  {
   FAArr::GetParStp(&par[NumFunc-1],&stp[NumFunc-1]);
   MemoryMove(&Bnd[1],&par[1],sizeof(double)*(NumFunc-1));
   MemoryMove(&BndStp[CurIter][1],&stp[1],sizeof(double)*(NumFunc-1));
   par[0]=GetNumPar();stp[0]=GetNumPar();
//cout<<"GetParStp\n";
//for (int k=1;k<=NumFunc-1;k++) {cout<<par[k]<<"   "<<stp[k]<<"  ";}
//cout<<"\n";
  };
void FAArrSet::SetPar(double *par)
  {
   for (int k=1;k<=NumCor;k++)
      par[correlatedpar[1][k]]=par[correlatedpar[0][k]]*correlatedcoef[k];
   MemoryMove(&par[1],&Bnd[1],sizeof(double)*(NumFunc-1));
   FAArr::SetPar(&par[NumFunc-1]);
//cout<<"SetPar\n";
//for (k=1;k<=NumFunc-1;k++) {cout<<par[k]<<"   ";}
//cout<<"\n";
  };
void FAArrSet::GetPar(double *par)
  {
   FAArr::GetPar(&par[NumFunc-1]);
   MemoryMove(&Bnd[1],&par[1],sizeof(double)*(NumFunc-1));
   par[0]=GetNumPar();
//cout<<"GetPar\n";
//for (int k=1;k<=NumFunc-1;k++) {cout<<par[k]<<"   ";}
//cout<<"\n";
  };
//fstream& operator<<(fstream &output,FAArrSet &Func)
void FAArrSet::Output(ostream &output)
  {
   output<<name<<"    "<<NumFunc<<"\n";
   output<<"CorFuncConst0  "<<CorFuncConst0<<"  CorFuncExpConst    "<<CorFuncExpConst<<"\n";
   for (int k=1;k<NumFunc;k++) 
     {
      char tmp[256];
      output<<"Bnd"<<Itoa(k,tmp,10)<<"        ";
      for (int k1=1;k1<=MaxIter;k1++)
        {
         if (BndStp[k1][k]==0)  output<<"   F   ";
         else                   output<<"   V   ";
        }
//      if (BndStp[k]==0) output<<" F     ";
//      else              output<<" V     ";
      output<<Bnd[k]<<"\n";
     }
   output<<"Num_Correlated_Par        "<<NumCor<<"\n";
   for (int k=1;k<=NumCor;k++) 
       output<<GetParName( correlatedpar[0][k] )<<"    "
             <<GetParName( correlatedpar[1][k] )<<"    "
             <<correlatedcoef[k]<<"\n";
     //output<<correlatedpar[0][k]<<"    "<<correlatedpar[1][k]<<"    "<<
     //       correlatedcoef[k]<<"\n";
   FAArr::Output(output);
//   return output;
  };
void FAArrSet::Input(istream &input)
  {
//cout<<" input set_func\n";
   input>>NumFunc;
//cout<<"FAArrSet::Input ; NumFunc = "<<NumFunc<<"\n";
   char tmp[256],*tmp_;tmp_=tmp;
   input>>tmp>>CorFuncConst0>>tmp>>CorFuncExpConst;
   int iter=0;
   if (NumFunc>1)
     {
      input>>tmp>>tmp;iter=0;
      double tmp_d[MaxIterations+1];
      while ( !IsDouble(tmp_,Bnd[1],1) )
        {
         if (iter++>=MaxIterations) 
           {cout<<"FAArrSet::Input;  Number of iterations is more then "
                <<MaxIterations<<" Program halted\n";exit(1);}
         if (seek_eof(input)) {cout<<"FAArrSet::Input Unexpected end of file\n";exit(1);}
         if (!Stricmp(tmp,"f")) tmp_d[iter]=0;
         else                   tmp_d[iter]=1;
         input>>tmp;tmp_=tmp;
//cout<<"FAArrSet::Input ; tmp_ = "<<tmp<<"\n";
        }
      if (iter==0) {cout<<"FAArrSet::Input; MaxIter=0. Halt.\n";exit(1);}
   //   Stp=new double*[MaxIter+1];
      for (int k=1;k<=iter;k++) 
        {BndStp[k]=new double [NumFunc];
         BndStp[k][0]=NumFunc-1;BndStp[k][1]=tmp_d[k];}
      for (int k=2;k<NumFunc;k++)
        {
         input>>tmp;
         for (int k1=1;k1<=iter;k1++)
           {
            input>>tmp;
            if (!Stricmp(tmp,"f")) BndStp[k1][k]=0;
            else                   BndStp[k1][k]=1;
//cout<<"FAArrSet::Input ; tmp = "<<tmp<<"  ";
           }
//cout<<"\n";
         input>>Bnd[k];
        }
     }
        
//   Func->ParStp(Par,Stp[1]);
//   CurIter=1;
   input>>tmp>>NumCor;
//cout<<" input set_func NumCor "<<tmp<<"    "<<NumCor<<"\n";
//   char Num1[256], Num2[256], Num3[256];
   vector<Stroka> Num1(NumCor+1),Num2(NumCor+1);
//   Stroka Num1, Num2;
   for (int k=1;k<=NumCor;k++) {
//     input>>correlatedpar[0][k]>>correlatedpar[1][k]>>correlatedcoef[k];      
       input>>Num1[k]>>Num2[k]>>correlatedcoef[k];
   }
//cout<<" begin internal input\n";
//   input>>tmp;
//cout<<"FAArrSet::Input ; NumFunc = "<<NumFunc<<"\n";
   FAArr::Input(input);
   if (NumFunc>1) if (iter!=MaxIter) 
     {cout<<" FAArrSet::Input; Wrong number of the parameters \n";
//cout<<"iter "<<iter<<"   MaxIter   "<<MaxIter<<"\n";
      exit(1);}
   for (int k=1;k<=NumCor;k++) {
        if (atoi(Num1[k].c_str())!=0){
            correlatedpar[0][k] = atoi(Num1[k].c_str());
            correlatedpar[1][k] = atoi(Num2[k].c_str());
        } else {
            correlatedpar[0][k] = GetParNameNum(Num1[k].c_str());
            correlatedpar[1][k] = GetParNameNum(Num2[k].c_str());
        }
   }
//cout<<"FAArrSet::Input ; SetPar = "<<NumFunc<<"\n";
   double *tmp1=new double[GetNumPar()+1];
   GetPar(tmp1);SetPar(tmp1);        // just to set correlated parameters

   delete tmp1;
  };

   
void FAArrSet::SaveInPar(ostream &output)
  {
   output<<CorFuncConst0<<"    "<<CorFuncExpConst<<"\n";
   for (int k=1;k<=NumFunc-1;k++) output<<Bnd[k]<<"     ";
   FAArr::SaveInPar(output);
  };

void FAArrSet::GetInPar(istream &input)
  {
   input>>CorFuncConst0>>CorFuncExpConst;      
   for (int k=1;k<=NumFunc-1;k++) input>>Bnd[k];
   FAArr::GetInPar(input);
  };

void FAArrSet::DrawHelp(ostream &out)
{
	FAArr::DrawHelp(out);
	out<<"\n "<<name<<" is a function working over functions\n";
	out<<" It is using <NumFunc> functions to construct a result function\n";
	out<<" It devides all region in a [NumFunc-1] subregion, in each of this\n";
	out<<" subregions is used its own function (any used function may be composite,\n";
	out<<" like sum_func or set_func ). Boundaries of subregions are given by\n";
	out<<" <Bnd> parameters ( Num of <Bnd> parameters is NumFunc-1 ). For example\n";
	out<<" given: NumFunc=3 ; Bnd=100 Bnd=200 - so points with X from .. to 100 \n";
	out<<" will be calculated by function 1, X - from 100 to 200 - function 2\n";
	out<<" all the rest - for X from 200 to .. - by function 3\n";
	out<<" Boundaries can be varied, to find the moment one function changes the\n";
	out<<" other ( for example, experimental spectra with begining zero line and \n";
	out<<" in some moment with exponental increase - we  find where it occurs)\n";
	out<<" To find this point, for better convergence, function in subregion [i] is\n";
	out<<" calculated by the formulas: \n";
	out<<" coef1=exp(-fabs(Xval-Bnd[i-1])*CorFuncExpConst-CorFuncConst0)\n";
	out<<" coef2=exp(-fabs(Xval-Bnd[i+1])*CorFuncExpConst-CorFuncConst0)\n";
	out<<" sh[i]=sh[i-1]+Func[i-1,Bnd[i-1]]-Func[i,Bnd[i-1]]\n";
	out<<" ResFunc[i,Xval]=coef1*(Func[i-1,Xval]+sh[i-1])+\n";
	out<<"                 (1-coef1-coef2)*(Func[i,Xval]+sh[i])+\n";
	out<<"				   coef2*(Func[i+1,Xval]+sh[i+1])\n\n";
	out<<" To all functions is added a constant, so that there values do\n";
	out<<" coincide on the boundaries (sh[i] array), neibhour functions\n";
	out<<" exponentially penetrate into the current region function\n";
	out<<" Also calculated a number of correlated parameters <Num_Correlated_Par>-\n";
	out<<" this number deifines number of dependent parameters\n";
	out<<" Dependent parameters are defined by strings like :\n Num_Correlated_Par 1 \n 10 20 3\n";
	out<<" it means that parameter 20 will be: Par[20]=Par[10]*3 \n";
	out<<" ( 3 - is correlated coef )\n";
	out<<" So variing parameter 10 - we will automatically variate parameter 20\n";
	out<<" The parameter's Number - is the number of parameter which can be varied \n";
	out<<" All variing parameters (parameter which are set F or V )are virtually \n";
	out<<" enumerated by 1,2,3...\n";
	out<<" Count begins from the first variing parameter of <"<<name<<">\n\n\n";

//	out<<"<<"<<name<<">>"<<" is IO envelope for FAZer = Clc functions\n";

}
void FAArrSet::SetNewFunc(FAIO *func,int Num)
{
	FAArr::SetNewFunc(func,Num);
	if (Num!=0) return;
	int iter=GetNumIter(),k,k1;
    double *BndStp1[MaxFunctions+1];
    for (k=1;k<=iter;k++) 
        {
		 BndStp1[k]=BndStp[k];
	     BndStp[k]=new double [NumFunc];
         BndStp[k][0]=NumFunc-1;
		}
    for (k=1;k<NumFunc-1;k++)
	{ for (k1=1;k1<=iter;k1++) BndStp[k1][k]=BndStp1[k1][k];}
	if (NumFunc>1) {for (k=1;k<=iter;k++) delete BndStp1[k];Bnd[NumFunc-1]=128;}
	else 
	{
		NumCor=2;
        for (k=1;k<=NumCor;k++) 
		{correlatedpar[0][k]=1;correlatedpar[1][k]=k+1;correlatedcoef[k]=1;}
	}
	for (k1=1;k1<=iter;k1++) BndStp[k1][NumFunc-1]=0;
}
//=====================================================================
//=========================        FARollIO      ======================
//=====================================================================
#ifndef WCPP_NET
DescribedClass * Dummy_FARoll(){return new FARoll();}
static ClassDesc FARoll_ClassDesc(typeid(FARoll),"Roll_func",1,"FAIO",Dummy_FARoll);
#else
static ClassDesc FARoll_ClassDesc(typeid(FARoll),"Roll_func",1,"FAIO",create<FARoll>);
#endif
void   FARoll::InputXYParam()
  {
//cout<<" Roll input \n";
   int NumXPP=0;
   double tmp[3];
   fstream InXPP(NameInt,ios::in);
   while (!seek_eof(InXPP)) if (EnterStr(InXPP,&tmp[0],2)) NumXPP++;
   if (NumXPP<=1) { cout<<"No data points in input file \n";return;}
//cout<<coreleft()<<"\n";
   XParamPnt=new double [NumXPP+1];XParamPnt[0]=NumXPP;
   YParamPnt=new double [NumXPP+1];YParamPnt[0]=NumXPP;
//cout<<coreleft()<<"\n";ChRead();
   InXPP.close();
   InXPP.open(NameInt,ios::in);
   NumXPP=0;
   while (!seek_eof(InXPP))
     {
      if (EnterStr(InXPP,&tmp[0],2)) NumXPP++;
      XParamPnt[NumXPP]=tmp[1];YParamPnt[NumXPP]=tmp[2];
     }
  };

void   FARoll::Clc(double *Xval,double *Res)
  {
   int NumPar=Func->GetNumPar();
   double *Par=new double[NumPar+1],Coef;
   double *OneRes=new double[Xval[0]];
   FASingleIO::GetPar(Par);
   for (int k=1;k<=Xval[0];k++) Res[k]=0;
   for (int k=1;k<=XParamPnt[0];k++)
     {
      Par[NParam]=XParamPnt[k];FASingleIO::SetPar(Par);
      FASingleIO::Clc(Xval,OneRes);
      Coef=YParamPnt[k];
      for (int k1=1;k1<=Xval[0];k1++) Res[k1]+=OneRes[k1]*Coef;
     }
   delete OneRes;delete Par;
  };

double FARoll::Clc(double Xval)
  {
   int NumPar=Func->GetNumPar();
   double *Par=new double[NumPar+1];
   FASingleIO::GetPar(Par);
   double Sum=0;
   for (int k=1;k<=XParamPnt[0];k++)
     {
      Par[NParam]=XParamPnt[k];FASingleIO::SetPar(Par);
      Sum+=FASingleIO::Clc(Xval)*YParamPnt[k];
     }
   delete Par;
   return Sum;
  };

/*
int FAStdRegister(fstream &in,FAIO *&Func,char *name)
  {
   FAZer *SFunc=NULL;
   FAIO *AFunc=NULL;
   int ret=1;
   if (!Stricmp(name,"exponent")) {SFunc=(FAZer*)new FAZerExp;}
   else if (!Stricmp(name,"lorents")) {SFunc=(FAZer*)new FAZerLor;}
     else if (!Stricmp(name,"polynom")) {SFunc=(FAZer*)new FAZerPoly;}
        else if (!Stricmp(name,"sum_func"))  {AFunc=(FAIO*)new FAArrAdd;}
           else if (!Stricmp(name,"set_func")) {AFunc=(FAIO*)new FAArrSet;}
             else if (!Stricmp(name,"roll_func")) {AFunc=(FAIO*)new FARoll;}
               else ret=0;
   if (!ret) return ret;
   if (Func!=NULL) delete(Func);
   if (SFunc!=NULL) 
     { 
      FASingleIO *F=new FASingleIO;
      F->Func=SFunc;
      Func=(FAIO*)F;
     }
   else  Func=AFunc;
   Func->Input(in);
//cout<<" Mem after FAZerIORegister  "<<coreleft()<<"\n";
   return ret;
  };

*/