#include <lib\precompiled\adi_dou.h>

#include "adi_dou.h"

int OnlyData;

extern int GetInitialPressure;

ostream& operator<<(ostream &output,Sl_Par &Par)
    {
	 if (!OnlyData)
	 {
//       if ((Par.M!=NULL) && (!MatterStore(output,Par.M)))  { cout<<"Could not store. Exit.\n";exit(0);}
       output<<" Vel "<<Par.P.Velo<<" Dens "<<Par.P.Dens<<" Pres "<<Par.P.Pres<<" Ener "<<Par.P.Ener;
       output<<" Length "<<Par.L<<" Time "<<Par.T<<" ShockV "<<Par.D;
	   if (Par.M!=NULL) output<<" Temperature "<<Par.M->Temperature(Par.P.Dens,Par.P.Ener)<<"\n";
	   else output<<"\n";
	 }
     else 
	 {
	   output<<Par.P.Velo<<" "<<Par.P.Dens<<" "<<Par.P.Pres<<" "<<Par.P.Ener<<" "<<Par.L<<" "<<Par.T<<" "<<Par.D;
	   if (Par.M!=NULL) output<<" "<<Par.M->Temperature(Par.P.Dens,Par.P.Ener)<<"\n";
	   else output<<"\n";
	 };
       return output;    
    };

istream& operator>>(istream &input,Sl_Par &Par)
    {
     char tmp[256];
     //delete Par.M;
	 if (NULL==(Par.M=dynamic_cast<MatterIO*>(SavableClass::Read(input)))) 
        throw info_except("Could not read. Exit.\n");
     input>>tmp>>Par.P.Velo>>tmp>>Par.P.Dens>>tmp>>Par.P.Pres>>tmp>>Par.P.Ener;
     input>>tmp>>Par.L>>tmp>>Par.T>>tmp>>Par.D;
     return input;
    };



// =======================================================================
// =======================================================================
// =======================================================================

void ReadArr(istream &in,Sl_Par *Sl,int &N)
    {
     char tmp[256];
     in>>tmp>>N;
     for (int k=1;k<=N;k++) in>>Sl[k];
    };
void StoreArr(ostream &out,Sl_Par *Sl,Sl_Par *Res,int N,int StoreOnlyData)
    {
	    OnlyData=StoreOnlyData;
//     Sl_Par tmp;
     if (!OnlyData) 
      {  
    		 out<<" NumLayers "<<N<<"\n";
       for (int k=1;k<=N;k++) 
        {
            if (!SavableClass::Save(out,Sl[k].M))  { cout<<"Could not store. Exit.\n";exit(0);}
			out<<"\n";
          out<<Sl[k]<<Res[k]<<"\n";
//        {tmp=Res[k];//tmp.M=NULL;
//         out<<Sl[k]<<tmp<<"\n";}
        }
      }
	    else {for (int k=1;k<=N;k++) out<<Sl[k]<<Res[k]<<"\n";}
	    out.flush();
    };
void HugLay(Sl_Par &P1,Sl_Par &P2,Sl_Par &R1,Sl_Par &R2)
    {
//     Sl_Par R1,R2;
     R1=P1;R2=P2;
     Hug2(P1.P,P2.P,R1.P,R1.D,R2.P,R2.D,P1.M,P2.M);
    };
void HugArr(Sl_Par *Ar,Sl_Par *Res,int N,int Iter=0)
    {
     Sl_Par *Tmp;Tmp=new Sl_Par[N+1];
     Res[1]=Ar[1];
     for (int k=1;k<N;k++) 
         {HugLay(Res[k],Ar[k+1],Tmp[k],Tmp[k+1]);
          Tmp[k+1].T=Tmp[k+1].L/Tmp[k+1].D;Tmp[k+1].L=Ar[k+1].L*Ar[k+1].P.Dens/Tmp[k+1].P.Dens;
          Res[k+1]=Tmp[k+1];
//cout<<Tmp[k];
         }
     Tmp[1].T=Tmp[1].L/Tmp[1].D;Tmp[1].L=Ar[1].L*Ar[1].P.Dens/Tmp[1].P.Dens;
     Res[1]=Tmp[1];
     if (Iter) 
         {Tmp[2].T=Tmp[2].L/Tmp[2].D;Tmp[2].L=Res[2].L*Res[2].P.Dens/Tmp[2].P.Dens;
          Ar[2]=Tmp[2];}
     delete Tmp;
    };
Sl_Par *S_VSl,*S_VRes;
double S_VValFind;
int S_VParFind,S_VNSl;
double MisVel(double U)
    {
     S_VSl[1].P.Velo=U;
     HugArr(S_VSl,S_VRes,S_VNSl);
//cout<<"==================\n Found mis "<<S_VRes[S_VNSl].P.Velo-S_VValFind
//    <<"\n==================\n";
     if (S_VParFind==1) return S_VRes[S_VNSl].T-S_VValFind;
     if (S_VParFind==2) return S_VRes[S_VNSl].D-S_VValFind;
     if (S_VParFind==3) return S_VRes[S_VNSl].P.Pres-S_VValFind;
     if (S_VParFind==4) return S_VRes[S_VNSl].P.Velo-S_VValFind;
     return 0;
    };
double Uplus_minus;
double FindVel(int ParFind,double ValFind,Sl_Par *Sl,int N)
    {
     double U=Sl[1].P.Velo;
//     double U=1;
     Sl_Par *tmp;tmp=new Sl_Par[N+1];
     S_VValFind=ValFind;S_VParFind=ParFind;S_VSl=Sl;S_VRes=tmp;S_VNSl=N;
//     if (!ZeroNewtonIt(MisVel,U,StndErr,30,1e6,-1e6,5))
//      cout <<" Error, could not find FindVel";
//int Fzero(X_func f,double From,double To,double &Guess,double ErrorAbs,double ErrorRel,
//                  int MaxIter);
//     if (3<Fzero(MisVel,1e6,-1e6,U,StndErr,StndErr,100))
     if (3<Fzero(MisVel,U+Uplus_minus,U-Uplus_minus,U,StndErr,StndErr,100))
        { cout <<" Error, could not find FindVel ZeroFzeroIt\n";}
//int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
//                             double Up,double Down,double MaxStp,double BegStp) 
     delete tmp;
cout<<" FoundVel "<<U<<"\n";
     return U;
    };
void DoIterations(ostream &out,Sl_Par *Sl,int N_Sl,int N_Iter,int StoreOnlyData)
    {
     if (N_Sl!=3) {cout<<" Iter work only for 3\n";exit(0);}
     Sl_Par *Res;Res=new Sl_Par[N_Sl+1];
     for (int k=1;k<=N_Iter;k++)
         {
          if (!StoreOnlyData) out<<" Iteration Number "<<k<<"\n";
          HugArr(Sl,Res,N_Sl,1);StoreArr(out,Sl,Res,N_Sl,StoreOnlyData); 
          Sl[1]=Res[1];Sl[3]=Res[3];
         };
     delete Res;
    };
extern unsigned _stklen=64000;
double StndErr;
int main(int argc,char *argv[])
  {
      try {
       StndErr=5e-7;
       OnlyData=0;
    //cout<<" argc "<<argc<<"\n";
       if (argc<=2)
         {
          cout<<" usage \n"<<
                "      adi_dou.exe conf out_file [/Key] \n"<<
                "      Key: \n"<<
                "           t23.2 : Given Last Time, determine first velocity  \n"<<
                "           d23.2 : Given Last Shock, determine first velocity  \n"<<
                "           p23.2 : Given Last Pres, determine first velocity  \n"<<
                "           s23.2 : Given Last Mass U, determine first velocity  \n"<<
                "           v5.4  : In calculation use first velocity          \n"<<
                "           i10   : Make number iterations                     \n"<<
                "           f     : Calculate only hugoniots( not isoent)      \n"<<
                "           u     : To search Velocity in limits u0+-u         \n"<<
                "           z     : For all take zero initial pressure        \n"<<
                "           oRes  : Output only raw data in the file Res       \n"<<
                " conf   structure:                                            \n"<<
                "                                                              \n"<<
                " NumMatter 2                                                  \n"<<
                "                                                              \n"<<
                "MatterABu   Material_File material.cfg Substance Steel-Tern   \n"<<
                "u  5.2  r  7.89  p  0  e  0  l  1  t  1  d  1                 \n"<<
                "                                                              \n"<<
                "MatterABu   Material_File material.cfg Substance Z-Al203_     \n"<<
                "u  0  r  3.985  p  0  e  0  l  1  t  1  d  1                  \n";
          exit(0);
         }
       double BegVel=0,FindD=0,FindP=0,FindT=0,FindU=0;int DoIter=0;
       char *tmpptr,*OutData=NULL;
       Uplus_minus=300;
       if ( (tmpptr=GetCmd("/t",argc-1,argv))!=NULL) FindT=atof(tmpptr);
       if ( (tmpptr=GetCmd("/p",argc-1,argv))!=NULL) FindP=atof(tmpptr);
       if ( (tmpptr=GetCmd("/d",argc-1,argv))!=NULL) FindD=atof(tmpptr);
       if ( (tmpptr=GetCmd("/s",argc-1,argv))!=NULL) FindU=atof(tmpptr);
       if ( (tmpptr=GetCmd("/v",argc-1,argv))!=NULL) BegVel=atof(tmpptr);
       if ( (tmpptr=GetCmd("/i",argc-1,argv))!=NULL) DoIter=atoi(tmpptr);
       if ( (tmpptr=GetCmd("/o",argc-1,argv))!=NULL) OutData=tmpptr;
       if ( (tmpptr=GetCmd("/f",argc-1,argv))!=NULL) OnlyHugoniotCalculation=1;
       if ( (tmpptr=GetCmd("/u",argc-1,argv))!=NULL) Uplus_minus=atof(tmpptr);
       if ( (tmpptr=GetCmd("/z",argc-1,argv))!=NULL) GetInitialPressure=1;

    //OnlyHugoniotCalculation
       fstream in(argv[1],ios::in),out(argv[2],ios::out);
       Sl_Par Sl_Arr[30],Sl_Res[30];
       int Sl_N;
       ReadArr(in,Sl_Arr,Sl_N);in.close();
       if (BegVel) { Sl_Arr[1].P.Velo=BegVel;}
       else if (FindT){Sl_Arr[1].P.Velo=FindVel(1,FindT,Sl_Arr,Sl_N);}
       else if (FindD){Sl_Arr[1].P.Velo=FindVel(2,FindD,Sl_Arr,Sl_N);}
       else if (FindP){Sl_Arr[1].P.Velo=FindVel(3,FindP,Sl_Arr,Sl_N);}
       else if (FindU){Sl_Arr[1].P.Velo=FindVel(4,FindU,Sl_Arr,Sl_N);}
       else if (DoIter) {DoIterations(out,Sl_Arr,Sl_N,DoIter,OutData!=NULL);out.close();exit(0);}
       HugArr(Sl_Arr,Sl_Res,Sl_N);
       StoreArr(out,Sl_Arr,Sl_Res,Sl_N,0);
       if (OutData!=NULL)
       {
         fstream dat(OutData,ios::out);
         if ( (FindT) || (FindD) || (FindP) ) dat<<Sl_Arr[1].P.Velo<<"\n";
	     StoreArr(dat,Sl_Arr,Sl_Res,Sl_N,1);
	     dat.close();
       }
       out.close();
      } CATCHMAINEXCEPTION(" Adi_dou failed.\n ");
      return 0;
  };
