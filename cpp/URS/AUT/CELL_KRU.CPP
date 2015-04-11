
  
//#include <alloc.h>
#include "cell_fac.h"
//#include "spline.h"
//#include "h2_liqui.h"

//
//  units for pressure : (gram*km)/(cm^2*c^2)  
//  units for energy   : (km/c)^2
//  units for dencity  :  gram/cm^3
//  units for velocity :  km/c   
//
#define U_  Reg.Velocity
#define P_  Reg.Pressure
#define Ro_ Reg.Dencity 
#define M_  Reg.Mass    
#define E_  Reg.Energy  
#define R_  Reg.Pos  
#define F_  Reg.Fixed

#define Num Reg.NumPnt

#ifdef WCPP
#define ESC 27
#else
#define ESC 283
#endif


//#define U_h  Half.Velocity
//#define P_h  Half.Pressure
//#define Ro_h Half.Dencity 
//#define E_h  Half.Energy  
//#define R_h  Half.Pos  
extern unsigned _stklen=64000;

//double OldU[10][5];
int FstU=1;

fstream *my_file;
double CoefUp;

void CellRecalc(RegionIO &Reg){};
void CellFixed(RegionIO &Reg,double &PresDeriv)
  {
			int k;
			double *M_l=M_,*P_l=P_,*U_l=U_;
			int *F_l=F_;
   P_l[0]=0;P_l[Num]=10;
   for (k=1;k<=Num;k++) F_l[k]=1;
   F_l[1]=0;F_l[Num]=0;
   for (k=1;k<Num;k++) 
     { if ((fabs(U_l[k]-U_l[k+1])>1e-4) ||
           (fabs(P_l[k+1]-P_l[k])>11)) { F_l[k]=0;F_l[k+1]=0;F_l[k-1]=0;}}
			Reg.Pres();
			PresDeriv=0;
   for (k=1;k<Reg.NumMatter;k++)
      PresDeriv+=fabs(P_l[Reg.BndMatter[k]-2]-P_l[Reg.BndMatter[k]+1]);
			PresDeriv=max(log(max(1,PresDeriv))-8,0);
  };
void ClcMass(RegionIO &Reg)
  {
   double *M_l=M_,*Ro_l=Ro_,*R_l=R_,*U_l=U_;
   for(int i=1;i<Num;i++)  M_l[i]=Ro_l[i]*(R_l[i+1]-R_l[i]);
   M_l[Num]=0;M_l[0]=0;Reg.ExtendBnd();
   M_l[Num]=0;M_l[0]=0;
   U_l[0]=U_l[1];U_l[Num]=U_l[Num-1];
  }; 
void ClcAddU(RegionIO &Reg,double *AddU)
  {
//cout<<" ClcAddU(RegionIO &Reg,double TimeStp,double *AddU)\n";
			double *M_l=M_,*P_l=P_,*U_l=U_,*Ro_l=Ro_,*R_l=R_;
			int *F_l=F_;
			int k;
   for (k=1;k<=Num;k++) 
      if (!F_l[k])
         AddU[k]=2*(P_l[k]-P_l[k-1])/(M_l[k]+M_l[k-1]);
      else  AddU[k]=0;
   AddU[1]*=2;
   AddU[Num]*=2;
  }; 

void CalcPos(RegionIO &Reg,double TimeStp,double &MinTimeStp)
  {
//cout<<" CalcPos \n";
   double *M_l=M_,*P_l=P_,*U_l=U_,*Ro_l=Ro_,*R_l=R_,*E_l=E_;
			int *F_l=F_;
			int k;
   double Coef;
   MinTimeStp=0;
   M_l[Num]=0;M_l[0]=0;
			double *tmp=new double[Num+3];
			Reg.Sound(tmp);
   for (k=1;k<Num;k++) 
      if (!F_l[k])
        { Coef=tmp[k]/(R_l[k+1]-R_l[k]);
          if (Coef>MinTimeStp ) MinTimeStp=Coef;
        } 
   Reg.MullVisc(tmp);
   double V2=Reg.Viscouse2,du;
   for (k=1;k<Num;k++) 
      if (!F_l[k])
        { 
         du=U_l[k+1]-U_l[k];
         P_l[k]-=Ro_l[k]*du*tmp[k]*1e5;
//         P_l[k+1]-=Ro_l[k+1]*du*tmp[k+1]*1e5;
        } 
   P_l[0]=10;P_l[Num]=10;
//cout<<" MinTimeStp=  \n";
   MinTimeStp=Reg.TimeStability/MinTimeStp*1e-5;
//   delete tmp;
   double *AddU=tmp;
   ClcAddU(Reg,AddU);
   double c=TimeStp*1e5;
   for (k=1;k<=Num;k++) 
     {
      AddU[k]=TimeStp*AddU[k];
      R_l[k]+=c*(U_l[k]-AddU[k]*0.5);
      U_l[k]-=AddU[k];
     }
   for (k=1;k<Num;k++) 
     {
      if (!F_l[k])
        {
         double Del=M_l[k];
         double AddL=TimeStp*(U_l[k+1]-U_l[k]);
         E_l[k]-=P_l[k]*AddL/Del;
         Ro_l[k]=1/(1/Ro_l[k]+AddL*1e5/Del);
        }    
     }
/*   
   for (k=1;k<Num;k++) 
     {
      if (!F_l[k])
        {
         double Del=M_l[k];
         double AddL=TimeStp*(U_l[k+1]-U_l[k]);
         Ro_l[k]=1/(1/Ro_l[k]+AddL*1e5/Del);
         E_l[k]-=P_l[k]*AddL/Del;
        }    
     }
*/
   delete AddU;
		};			


void StepRegion(RegionIO &Reg,double TimeStp,double &MinTimeStp,double &PresDeriv)
  {
   CellFixed(Reg,PresDeriv);
//cout<<"StepRegion  beg    Coreleft() "<<coreleft()<<"\n";

   CalcPos(Reg,TimeStp,MinTimeStp);
			Reg.TimeCh(TimeStp);                
  };
void Reconstruct(RegionIO &Reg){};
void OutHead(fstream &out,RegionIO &Reg,double CurTime,double TimeStp,
             double TimeWrite,double PresDerivCoef,double EndTime)
  {
   out<<" CurTime "<<CurTime<<" TimeStp "<<TimeStp<<" TimeWrite "<<TimeWrite
							       <<" PresDerivCoef "<<PresDerivCoef<<" EndTime "<<EndTime<<"\n";
   Reg.SaveIni(out);out<<" EndIter\n";
		};	           
int InHead(fstream &in,double &CurTime,double &TimeStp,double &TimeWrite,
                                    double &PresDerivCoef,double &EndTime)
  {
			char tmp[256];
   in>>tmp>>CurTime;
   in>>tmp>>TimeStp;
   in>>tmp>>TimeWrite;
   in>>tmp>>PresDerivCoef;
   in>>tmp>>EndTime;
			if ( (!in)  && (seek_eof(in)) ) return 0;
			return 1;
		};	         
		  
RegionIO* GetRegion(char *inname,fstream &output,int &FstIter,
                        double &CurTime,double &TimeStp,double &TimeWrite,
                                    double &PresDerivCoef,double &EndTime)
  {
   int Cont=1,HeadRead=1;FstIter=1;
			char tmp[150];
			double CurTime1,Buf;
   fstream input=fstream(inname,ios::in);
   RegionIO *Reg=NULL,*Reg1=NULL,*Reg0=NULL;
   Reg=new RegionIO;Reg1=new RegionIO;
   do
     {
      if (!FstIter) {input>>tmp;HeadRead=InHead(input,CurTime,Buf,Buf,Buf,Buf);}
						else InHead(input,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime);
      Cont=0;
      if ((HeadRead) && (Reg->ReadIni(input)))
        {
         if (!FstIter)
           {
            OutHead(output,*Reg1,CurTime1,TimeStp,TimeWrite,PresDerivCoef,EndTime);
            Reg1->InterfaceIODelete();
           }  
         FstIter=0;
//cout<<" Read Reg1 \n";
         input>>tmp;
         if (InHead(input,CurTime1,Buf,Buf,Buf,Buf) && (Reg1->ReadIni(input)))
           {
            OutHead(output,*Reg,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime);
            Reg->InterfaceIODelete();
            Cont=1;
cout<<" Cont "<<Cont<<"\n";
           } 
         else Reg1->InterfaceIODelete();
        }   
      else 
        {
         if (FstIter) { cout<<" There is no points. Exiting\n";exit (1);}
         Reg0=Reg;Reg=Reg1;Reg1=Reg0;CurTime=CurTime1;
        }
     }
   while (Cont);
cout<<" Bef del Coreleft "<<coreleft()<<"\n";
   delete Reg1;
cout<<" Aft del Coreleft "<<coreleft()<<"\n";
   OutHead(output,*Reg,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime);
//ChRead();
   return Reg;
		};	                  
void main( int argc, char *argv[] )
  {
argc--;
cout<<" Coreleft "<<Coreleft()<<"\n";
   char tmp[50];
   if ((argc<2) || (GetCmd("/h",argc,argv)!=NULL))
     { cout<<"usage:\n"<<argv[0]<<"  in_lmethod  output_file\n"<<
           "      /h - display help\n"; exit(1);  }
         my_file=new fstream(argv[2],ios::out);


   int FstIter=0;
   double TimeStp,EndTime,CurTime=0,TimeWrite,PresDerivCoef;
   RegionIO *Reg=GetRegion(argv[1],*my_file,FstIter,
			               CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime); 
//	my_file->close();};/*

   if (argc>2) CoefUp=max(1,atoi(argv[3]));else CoefUp=1;
//cout<<" Coreleft "<<coreleft()<<"\n";
//			OutHead(*my_file,*Reg,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime);
   ClcMass(*Reg);
   int x=1;
//			int i=1;
//cout<<" TW     "<<TimeWrite<<" CurTime "<<CurTime<<" TimeStp "<<TimeStp<<"\n";
//ChRead();

   double CurStp,OldTimeWrite,NewTimeWrite,PresDeriv=1,OldStp=1e5;
   int BreakSignal=0;
//#ifdef WCPP
   double elapsed_time;
   clock_t start, finish;start=clock();
//#endif

   while ((!(CurTime>=EndTime)) && (!BreakSignal))
     {
  	  OldTimeWrite=CurTime;
	  NewTimeWrite=min(OldTimeWrite+
	                   TimeWrite/(1+PresDeriv*PresDerivCoef),EndTime);
//cout<<" Before While NewTime "<<NewTimeWrite<<"\n";
        while ((!(CurTime>=NewTimeWrite)) && (!BreakSignal))
        {
 	     NewTimeWrite=min(OldTimeWrite+
		              TimeWrite/(1+PresDeriv*PresDerivCoef),EndTime);
		 if (x<100) {x++;CurStp=0.5*log10(x)*TimeStp;}
		 else CurStp=TimeStp;
		 if (CurStp+CurTime>NewTimeWrite) CurStp=NewTimeWrite-CurTime;
		 if (CurStp<MathZer) break;
         if (CurStp>OldStp) CurStp=OldStp*(CoefUp*log(CurStp/OldStp)+1);
         OldStp=CurStp;
//cout<<" CurStp "<<CurStp<<" CurTime "<<CurTime<<"\n";
//ChRead();
         if ((x==2) && (FstIter)) CalcPos(*Reg,-0.5*CurStp,TimeStp);
//cout<<" StepRegion \n";
         CurTime+=CurStp;StepRegion(*Reg,CurStp,TimeStp,PresDeriv);
//         if (i++==10) {i=0;Reconstruct(*Reg);}
//      Reg.SaveIni(output);

				    }
					OutHead(*my_file,*Reg,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime);
cout<<" Write !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n";
cout<<" CurStp "<<CurStp<<" CurTime "<<CurTime<<"\n";     
#ifdef WCPP
		 if (_kbhit()) { BreakSignal=_getch();if (BreakSignal==ESC) break;}
#else
         while ((BreakSignal=bioskey(1))!=0) if (BreakSignal==ESC) break;else {cout<<" "<<BreakSignal;bioskey(0);}
#endif
     }
//#ifdef WCPP
  finish=clock();elapsed_time=(finish-start);//CLK_TCK;
cout<<" RunTime "<<elapsed_time<<"\n";
//#endif
//cout<<" Reg.InterfaceIODelete();  \n";
cout<<" Coreleft "<<Coreleft()<<"\n";
    Reg->InterfaceIODelete();
	my_file->close();
cout<<" Coreleft "<<Coreleft()<<"\n";
  };
//*/



/*
double Mis_E,Mis_P,Mis_Ev,Mis_DeltaE,Mis_DeltaP,Mis_DeltaEv,Mis_DeltaEF;
cout<<" Miss_P "<<Mis_DeltaP<<" Miss_Einter "<<Mis_DeltaE
    <<" Miss_Ekin "<<Mis_DeltaEv<<" Miss_Efull "<<Mis_DeltaEF<<"\n";


void ClcMis(RegionIO &Reg,int Fst=0)
  {
			double P=0,E=0,Ev=0;
			for (int k=1;k<Num;k++)
			  {
			   P+=(U_[k-1]+U_[k])*M_[k]*0.5;
			   E+=E_[k]*M_[k];
			   Ev+=sqr(U_[k-1]+U_[k])*M_[k]*0.125;
					}
			if (!Fst)
			  {
						Mis_DeltaP=Mis_P-P;Mis_DeltaE=Mis_E-E;Mis_DeltaEv=Mis_Ev-Ev;
						Mis_DeltaEF=Mis_E+Mis_Ev-Ev-E;
			  }
			else 
			  {
						Mis_P=P;Mis_E=E;Mis_Ev=Ev;
			  }
		};			
*/
