#include <lib/precompiled/eos.h>
#include "matt_spl.h"   
#include "empi_equ.h"
#include "matt_fac.h"


static ClassDesc MatterSpl_ClassDesc(typeid(MatterSpl),"MatterSpl",1,"Matter_category",create<MatterSpl>);

fstream StdOut;

extern EmpiParamStruct *EmpiColdParam;
//double MatterSpl::St_Denc,MatterSpl::St_Temp;
MatterSpl* MatterSpl::Current;

void MatterSpl::Pressure(double *P,double *Dencity,double *Energy,int Num)
  {
   double *E=new double [Num+1],e,d,*D=new double [Num+1];
   for (int k=1;k<=Num;k++) if ((e=Energy[k]+AddE)>StndErr) E[k]=log(e); else E[k]=-6;
   for (int k=1;k<=Num;k++) if ((d=Dencity[k])>StndErr) D[k]=log(d); else D[k]=-9;
   SPre->Evaluate(&E[1],&D[1],&P[1],Num);
   for (int k=1;k<=Num;k++) P[k]=exp(P[k])-AddP;
   delete []E;delete []D;
  }; 
void MatterSpl::Temperature(double *T,double *Dencity,double *Energy,int Num)
  {
   int k;
   if (SThe!=NULL) 
     {
      double *E=new double [Num+1],e,d,*D=new double [Num+1];
      for (k=1;k<=Num;k++) if ((e=Energy[k]+AddE)>StndErr) E[k]=log(e); else E[k]=-6;
      for (k=1;k<=Num;k++) if ((d=Dencity[k])>StndErr) D[k]=log(d); else D[k]=-9;
      SThe->Evaluate(&E[1],&D[1],&T[1],Num);
      for (k=1;k<=Num;k++) T[k]=exp(T[k]);
      delete []E;delete []D;
     } 
   else {for (k=1;k<=Num;k++) T[k]=0;}
  }; 

// ==============================================================================
// ======================== Spline Generation ===================================
// ==============================================================================

void ReadData(const char *name,TData<real> *&dat)
  {
   fstream row(name,ios::in);
   if (!DataRegister(row,dat)) { cout<<" Could not read param.";exit(0);}
   row.close();
  }
void SetIntParInit(double &IntPar,MatterIO *Mat)
  {
//#ifdef InternalPointsUrs  
//   Mat->SetIntPar(&IntPar); 
//#endif
  };
void SetIntPar(double &IntPar,MatterIO *Mat,double Denc,double Energy)
  {
//#ifdef InternalPointsUrs
////   IntPar=0;Mat->ChangeIntPar(dat->D[1][k],dat->D[0][k]-AddE,1);
//   IntPar=0;Mat->ChangeIntPar(Denc,Energy,1);//Time);
//#endif
  };
double SetIntFuncTemp(MatterIO *Mat,double Denc,double Temp)
  {
   double ret=Temp;
//#ifdef INTERNAL_FUNC
//   double inp[3],outp[2];
//   inp[1]=Denc;inp[2]=Temp;
//   Mat->Int_Par(inp,outp);
//   ret=outp[1];
//#endif
   return ret;
  }
//CurveSpline Boundary_ZeroTemp;

TDataF<double> Boundary_ZeroEnergy;
double Boundary_ZeroTemperature=50;
void InitPresTemp(int GetPnt,TData<real> &datP,MatterIO *Mat)
{
   if (GetPnt==2)
   {
    int N=datP.I[1],tmp[2]={N,N},k;
    Boundary_ZeroEnergy.SetDim(2,tmp);
    for (k=0;k<N;k++) 
    {
     Boundary_ZeroEnergy.D[0][k]=Mat->Energy(datP.D[1][k],Boundary_ZeroTemperature);
     Boundary_ZeroEnergy.D[1][k]=Mat->Pressure(datP.D[1][k],Boundary_ZeroEnergy.D[0][k]);
    }
fstream out("###tmp_zer",ios::out);DataStore(out,&Boundary_ZeroEnergy);out.close();

   }
}
void GetPresTemp(int GetPnt,double IntPar,MatterIO *Mat,
                 double curD,double curE,int NumD,double &curP,double &curT)
{
   double ZerE;
//cout<<" curD "<<curD<<" numD "<<NumD<<" curE "<<curE<<" zerE "<<Boundary_ZeroEnergy.D[0][NumD]<<"\n";
   if ( (GetPnt==2) && ( curE<(ZerE=Boundary_ZeroEnergy.D[0][NumD]) ) )
    {
     SetIntPar(IntPar,Mat,curD,curE);
     curT=Boundary_ZeroTemperature+curE-ZerE;
     curP=Boundary_ZeroEnergy.D[1][NumD]+(curE-ZerE)*0.1;
     curT=SetIntFuncTemp(Mat,curD,curT);
    }
   else
    {
     SetIntPar(IntPar,Mat,curD,curE);
     curT=Mat->Temperature(curD,curE);
     curP=Mat->Pressure(curD,curE);
     curT=SetIntFuncTemp(Mat,curD,curT);
    }
//cout<<" curD "<<curD<<" numD "<<NumD<<" curE "<<curE<<" zerE "<<Boundary_ZeroEnergy.D[0][NumD]<<"\n\n";
}
//struct ClcPnt
//{
//  MatterIO *Mat;
//  VecCl Energy;
//  double ZeroTemp,MinE,MaxE;
//  double MisfTspl,MisfPspl;
//  void ClcDencity(VecCl &Pres,VecCl &Temp,double Denc)
//  {
//    //double ZeroE=Mat->Energy(Denc,ZeroTemp);
//    //double UpT=Mat->Temperature(Denc,MaxE);
//    VecCl FindT=GetTVector(Denc);//(UpT,ZeroE);
//    VecCl FoundE,FoundP;
//    GeneratePE(FindT,FoundE,FoundP);
//    CurveSpline Tspl,Pspl;
//    Tspl.Generate(NumX,Misf,0,&FoundE[1],&FindT[1],FindT.Dim());
//    Pspl.Generate(NumX,Misf,0,&FindT[1],&FoundP[1],FindT.Dim());
//    Tspl.Evaluate(&Energy[1],&Temp[1],Energy.Dim());
//    Pspl.Evaluate(&Temp[1],&Pres[1],Energy.Dim());
//  }
//
//};
//void ClcData(MatterIO *Mat,TData<real> &datP,TData<real> &datT)
//{
//  TTable<real> dP,dT;dP=datP;dT=datT;
//
//  double IntPar,curP,curT,curD,curE;
//  SetIntParInit(IntPar,Mat);    // for eos with internal par
//  int k,Num=dP->I[Row],NumIt=datP.I[1],It=0,NumD=0;
//  NumD=dP.I[1];NumE=dP.I[0];
//  for (int k=0;k<NumD;k++)
//
//	InitPresTemp(GetPnt,datP,Mat);//Make zero temp e
//
//  for (k=0,NumD=0;k<Num;k++,NumD++) 
//    {
//      if (NumD>=NumIt) NumD=0;
//
//      curD=dP->D[1][k];curE=dP->D[0][k]-AddE;
//      GetPresTemp(GetPnt,IntPar,Mat,curD,curE,NumD,curP,curT);
//      dP->D[Row][k]=curP+AddP;
//      dT->D[Row][k]=max<double>(curT,StndErr);
//
//cout<<" n1 "<<k<<" l "<<Num<<" ";
//    }
//
//}
void SetDatFunc(TData<real> &datP,TData<real> &datT,int Row,MatterIO *Mat,
                double AddE,double AddP,int GetPnt)
  {
   char NamePo[256],NameTo[256];
   strcpy(NamePo,"tmp_pres.dat");strcpy(NameTo,"tmp_temp.dat");
   TData<real> *dP=NULL,*dT=NULL;
   if ((GetPnt!=0) && (GetPnt!=2)) 
     {
      ReadData("tmp_pres.dat",dP);ReadData("tmp_temp.dat",dT);
      strcpy(NamePo,"tmp_pre1.dat");strcpy(NameTo,"tmp_tem1.dat");
     }
   else 
     {
      double IntPar,curP,curT,curD,curE;
      SetIntParInit(IntPar,Mat);
      DataRegister("TColumn",dP);DataRegister("TColumn",dT); 
      *dT=datP; *dP=datP;
      int k,Num=dP->I[Row],NumIt=datP.I[1],
//              It=0,
              NumD=0;

	     InitPresTemp(GetPnt,datP,Mat);

      for (k=0,NumD=0;k<Num;k++,NumD++) 
        {
          if (NumD>=NumIt) {NumD=0;cout<<" E++\n";}

         curD=dP->D[1][k];curE=dP->D[0][k]-AddE;
         GetPresTemp(GetPnt,IntPar,Mat,curD,curE,NumD,curP,curT);
         dP->D[Row][k]=curP+AddP;
         dT->D[Row][k]=max<double>(curT,StndErr);

cout<<" n1 "<<k<<" l "<<Num<<" ";
//cout<<" nD "<<NumD;cout.flush();
        }
     }
   datP=*dP;datT=*dT; 
   {
fstream tmpP("##tmpP",ios::out),tmpT("##tmpT",ios::out);
TColumn<real> tmp_p,tmp_t;tmp_p=*dP;tmp_t=*dT;
DataStore(tmpP,&tmp_p);DataStore(tmpT,&tmp_t);
tmpP.close();tmpT.close();
   }
   delete dP;delete dT;
   fstream outP(NamePo,ios::out),outT(NameTo,ios::out);
   DataStore(outP,&datP);DataStore(outT,&datT);
   outP.close();outT.close();
cout.flush();
  };

/*
void SetDatFunc(TData<real> &datP,TData<real> &datT,int Row,MatterIO *Mat,
                double AddE,double AddP,int GetPnt)
  {
   char NamePo[256],NameTo[256];
   strcpy(NamePo,"tmp_pres.dat");strcpy(NameTo,"tmp_temp.dat");
   TData<real> *dP=NULL,*dT=NULL;
   if (GetPnt) 
     {
      ReadData("tmp_pres.dat",dP);ReadData("tmp_temp.dat",dT);
      strcpy(NamePo,"tmp_pre1.dat");strcpy(NameTo,"tmp_tem1.dat");
     }
   else 
     {
      double tmp,IntPar;
      SetIntParInit(IntPar,Mat);
      DataRegister("TColumn",dP);DataRegister("TColumn",dT); 
      *dT=datP; *dP=datP;
      int k,Num=dP->I[Row];
      for (k=0;k<Num;k++) 
        {
         double curD=dP->D[1][k],curE=dP->D[0][k]-AddE;
         SetIntPar(IntPar,Mat,curD,curE);
         tmp=Mat->Temperature(curD,curE);
         dP->D[Row][k]=Mat->Pressure(curD,curE)+AddP;
         tmp=SetIntFuncTemp(Mat,curD,tmp);
         dT->D[Row][k]=max<double>(tmp,StndErr);
cout<<" n1 "<<k<<" l "<<Num<<" ";
        }
     }
   datP=*dP;datT=*dT; 
   delete dP;delete dT;
   fstream outP(NamePo,ios::out),outT(NameTo,ios::out);
   DataStore(outP,&datP);DataStore(outT,&datT);
   outP.close();outT.close();
  };

*/
 
void SetDatLog(TData<real> &dat,int Row,double  lm)
  {
     for (int k=0;k<dat.I[Row];k++) 
      { if (dat.D[Row][k]<0) cout<<" Log neg !!\n";
        dat.D[Row][k]=log(max<double>(lm,dat.D[Row][k]));  }
  };
      
int GenerateMatterSpl(istream &row,MatterIO *Mat)
   {
//cout<<"GenerateMatterSpl beg    Coreleft() "<<coreleft()<<"\n";
    char tmp[256],res_name[256];
    TTable<real> dat, datT;
    TColumn<real> da;
    int NumX,NumY,Meth,i=0,GetPnt;
    real DC,DM;
    double AddE,AddP,MinT,Misf;
    row>>tmp>>res_name>>tmp>>DC>>tmp>>DM>>
         tmp>>NumX>>tmp>>NumY>>tmp>>Meth>>tmp>>Misf>>tmp>>AddE>>tmp>>AddP>>
         tmp>>MinT>>tmp>>GetPnt;
    MatterFreeE::AbsoluteMinTemperature = MinT;
	Boundary_ZeroTemperature = max<double>(MinT,Boundary_ZeroTemperature);
cout<<" Entered Cfg for GenerateMatterSpl\n";//ChRead();
    //SplineXYIO *sp=NULL;
// First - Energy, Second - Dencity
    if (!ReadTableRow(row,(TData<real>*)&dat)) { cout<<" Could not read param.";exit(0);}
//    SetDatFunc(dat,2,Mat,1,AddE,AddP,GetPnt);datT=dat;
//    SetDatFunc(datT,2,Mat,0,AddE,AddP,GetPnt);
   	datT=dat;SetDatFunc(dat,datT,2,Mat,AddE,AddP,GetPnt);
//    SetDatFunc(datT,2,Mat,0,AddE,AddP,GetPnt);
cout<<" Constructed Temp\n";
    SetDatLog(dat,2,MathZer);SetDatLog(dat,0,StndErr);SetDatLog(dat,1,StndErr);
cout<<" SetDatLog(dat) \n";
	Ref< SplineXYIO > sp; sp << new RegridSpline();
	//DescribedClass *dc = ClassDesc::name_to_class_desc("RegridSpline")->create();
	//if ( !(sp=dynamic_cast<RegridSpline*>(dc)) )
	//	throw info_except(" Created class %s is not RegridSpline class\n",dc->class_name());
    sp->SetLimits(dat);
//cout<<"GenerateMatterSpl spl P  Coreleft() "<<coreleft()<<"\n";

da.SetDim(i,&i);
//fstream cmp("tmp_cmp.dat",ios::out);
//da=dat;DataStore(cmp,(TData<real>*)&da);
//cmp.close();

    int ret=sp->Generate(NumX,NumY,Misf,Meth,dat);
    if (ret<=0)
      {
//       sp->Evaluate(da);DataStore(cmp,(TData<real>*)&da);
cout<<" ret "<<ret<<" Now generate T\n";
//cmp.open("tmp_cmp.dat",ios::out);
//sp->Evaluate(da);DataStore(cmp,(TData<real>*)&da);
//cmp.close();cmp.open("tmp_cmp.res",ios::out);
cout<<" Stored Sp\n";
       
       ret=1;
       fstream out(res_name,ios::out);
       out<<" AddE "<<AddE<<" AddP "<<AddP<<"\n";
       out<<"Dencity_Cold "<<DC<<" DencityMax "<<DM<<"\n";
	   MatterSpl::SaveSpline(sp,out);
	   //SavableClass::Save(out,sp);
//       SurfStore(out,sp);

//#ifndef INTERNAL_FUNC
       SetDatLog(datT,2,MinT);
//#else
//       SetDatLog(datT,2,StndErr);
//#endif
       SetDatLog(datT,0,StndErr);SetDatLog(datT,1,StndErr);
       sp->SetLimits(datT);
cout<<" SetDatLog(datT) \n";

//       da=datT;
//fstream cmp("tmp_cmp.dat",ios::out);
//DataStore(cmp,(TData<real>*)&datT);
//cmp.close();

da.SetDim(i,&i);dat.SetDim(i,&i);
       row>>tmp>>NumX>>tmp>>NumY>>tmp>>Meth>>tmp>>Misf;

cout<<"GenerateMatterSpl spl T  Coreleft() "<<Coreleft()<<"\n"
<<" NumX "<<NumX<<" NumY "<<NumY<<" Meth "<<Meth<<" Misf "<<Misf<<"\n";

       ret=sp->Generate(NumX,NumY,Misf,Meth,datT);
cout<<" ret "<<ret<<"\n";
//      if (ret==1) 
//         { NumX=datT.I[0]+5;NumY=datT.I[1]+5;Meth=-1;
//           ret=sp->Generate(NumX,NumY,Misf,Meth,datT);
//if (ret<=0) { sp->Evaluate(da);DataStore(cmp,(TData<real>*)&da);}
       
       out<<"\n";
       MatterSpl::SaveSpline(sp, out);
       //SavableClass::Save(out,sp);
//SurfStore(out,sp);
       out.close();
      } 
    else {cout<<" Bad. RetSpl "<<ret<<"\n";ret=0;}//da=dat;DataStore(cmp,(TData<real>*)&da);       }

    //delete sp;
//cmp.close();
cout<<"GenerateMatterSpl spl end  Coreleft() "<<Coreleft()<<"\n";
    return ret;
   };          

// ==============================================================================
// ======================== Spline Generation   End =============================
// ==============================================================================

// Name_space Matt_spl
void Matt_spl::ReadData(char *name,TData<real> *&dat){::ReadData(name,dat);};
void Matt_spl::SetIntPar(double &IntPar,MatterIO *Mat,double Denc,double Energy){::SetIntPar(IntPar, Mat, Denc, Energy);};
void Matt_spl::SetIntParInit(double &IntPar,MatterIO *Mat){::SetIntParInit(IntPar, Mat);};
fstream& Matt_spl::StdOut(){return ::StdOut;};
