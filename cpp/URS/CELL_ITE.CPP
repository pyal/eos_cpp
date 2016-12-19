//  Iterations
#include <alloc.h>
#include "cell_fac.h"
#include "spline.h"
#include "h2_liqui.h"
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
#define ESC 283

//#define U_h  Half.Velocity
//#define P_h  Half.Pressure
//#define Ro_h Half.Dencity 
//#define E_h  Half.Energy  
//#define R_h  Half.Pos  
extern unsigned _stklen=64000;

double CoefUp;
fstream *my_file;
fstream delta_file("Delta.dat",ios::out);
double Mis_E,Mis_P,Mis_Ev,Mis_DeltaE,Mis_DeltaP,Mis_DeltaEv,Mis_DeltaEF;
/*
double RegionIO::Pres(MatterIO *Mat,double D,double E,double V,
                                                      double dV)
//  {return Mat->Pressure(D,E-sqr(V)*0.5)*1e5;}
  {  return max<double>(Mat->Pressure(D,E)*1e5,10);}
double RegionIO::Sound(MatterIO *Mat,double D,double E,double V)
//  {  return Mat->Sound(D,E-sqr(V)*0.5); }
  {  return Mat->Sound(D,E); }
*/
//  {  
//cout<<" Sound ";
//			double ret=Mat->Sound(D,E);
//cout<<ret;return ret; }

void CellRecalc(RegionIO &Reg){};
void CellFixed(RegionIO &Reg,double &PresDeriv)
  {
			int k;
   double *P_l=P_,*U_l=U_;
			int *F_l=F_;
   for (k=1;k<Num;k++) 
			  {	F_l[k]=1;}
			if (fabs(P_l[1]-P_l[0])>10*StndErr) F_l[1]=0;
   for (k=1;k<Num;k++) 
     { if (fabs(U_l[k]-U_l[k+1])+fabs(P_l[k+1]-P_l[k]) >100*StndErr) { F_l[k]=0;F_l[k+1]=0;F_l[k-1]=0;}}
			PresDeriv=0;
   for (k=1;k<Reg.NumMatter;k++)
      PresDeriv+=fabs(P_l[Reg.BndMatter[k]-2]-P_l[Reg.BndMatter[k]+1]);
			PresDeriv=max<double>(log(max<double>(1,PresDeriv))-8,0);
  };

void ClcMass(RegionIO &Reg)
  {
//  Right - Calculating masses between points
			double *M_l=M_,*Ro_l=Ro_,*R_l=R_;
   for(int i=1;i<Num;i++)  M_l[i]=Ro_l[i]*(R_l[i+1]-R_l[i]);
   M_l[Num]=0;M_l[0]=0;Reg.ExtendBnd();
   M_l[Num]=0;M_l[0]=0;
  }; 
void CalcPos(RegionIO &Reg,double TimeStp)//,double &MinTimeStp)
  {
//cout<<" CalcPos \n";
   double *M_l=M_,*P_l=P_,*U_l=U_,*R_l=R_;
			int *F_l=F_;
			int k;
   M_l[Num]=0;M_l[0]=0;
   for (k=1;k<=Num;k++) 
     {
      if (!F_l[k])
        {
         double Del=0.5*(M_l[k]+M_l[k-1]);
         double AddU=TimeStp*(P_l[k]-P_l[k-1])/Del;
         R_l[k]+=TimeStp*1e5*(U_l[k]-AddU*0.5);
         U_l[k]-=AddU;
        }
      else  R_l[k]+=TimeStp*1e5*U_l[k];
					}
//			delete tmp;
		};			

void CalcMain(RegionIO &Reg,double TimeStp)
  {
//cout<<" ClcMain \n";
   double *M_l=M_,*P_l=P_,*U_l=U_,*Ro_l=Ro_,*E_l=E_;
			int *F_l=F_;
			int k;
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
  };

struct HidData {double *P,*R,*Ro,*E,*U;};
void CalcPres(RegionIO &Reg,double &MinTimeStp)
	 {
   double *P_l=P_,*U_l=U_,*Ro_l=Ro_,*R_l=R_;
			int *F_l=F_;
			Reg.Pres();
   double *tmp=new double[Num+3],Coef;
			Reg.Sound(tmp);
   int k;
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
//         if  (P_l[k]>-10) 
//           {if (du>0) 
                P_l[k]+=Ro_l[k]*V2*sqr(du)*1e5;
//            }
//         else P_l[k]=-10;
//         P_l[k]=max<double>(-10,P_l[k]);
        } 
   P_l[0]=10;P_l[Num]=10;
   MinTimeStp=Reg.TimeStability/MinTimeStp*1e-5;
			delete tmp;
		};	
void IterCalcVel(RegionIO &Reg,double *VelOld,double *ROld,double TimeStp)
  {
   double *M_l=M_,*P_l=P_,*U_l=U_;
			int *F_l=F_,k;
   M_l[Num]=0;M_l[0]=0;
   P_l[0]=10;P_l[Num]=10;
   for (k=1;k<=Num;k++) 
     { if (!F_l[k])
        {double Del=0.5*(M_l[k]+M_l[k-1]);
         double AddU=TimeStp*(P_l[k]-P_l[k-1])/Del;
//         R_l[k]=ROld[k]+TimeStp*1e5*(U_l[k]-AddU*0.5);
         U_l[k]=VelOld[k]-AddU;}
       else U_l[k]=VelOld[k];
//      else  R_l[k]=ROld[k]+TimeStp*1e5*U_l[k];
					}	
		};			
void EndCalcPos(RegionIO &Reg,double *VelOld,double TimeStp)
  {
			double *U_l=U_,*R_l=R_;
			int k;
   for (k=1;k<=Num;k++) 
         R_l[k]+=TimeStp*5e4*(U_l[k]+VelOld[k]);
		};			
void IterCalcMain(RegionIO &Reg,double *EOld,double *ROld,double TimeStp)
  {
   double *M_l=M_,*P_l=P_,*U_l=U_,*Ro_l=Ro_,*E_l=E_;
			int *F_l=F_,k;
   for (k=1;k<Num;k++) 
			  {
      if (!F_l[k])
        {double Del=M_l[k];
         double AddL=TimeStp*(U_l[k+1]-U_l[k]);
         Ro_l[k]=1/(1/ROld[k]+AddL*1e5/Del);
						   E_l[k]=EOld[k]-P_l[k]*AddL/Del;}
					}				
  };

void OneIter(RegionIO &Reg,HidData &Old,double TimeStp)
  {
//			CalcPres(Reg);
   for (int k=1;k<=Num;k++) 
			  {
//      if (!F_[k])
			     {
         U_[k]=(0.55*U_[k]+0.45*Old.U[k]);
         P_[k]=(0.55*P_[k]+0.45*Old.P[k]);
//									U_[k]=Old.U[k];
//									P_[k]=Old.P[k];
//         E_[k]=(E_[k]+Old.E[k])*0.5;
//         Ro_[k]=(Ro_[k]+Old.Ro[k])*0.5;
								};	
					}			
		 IterCalcVel(Reg,Old.U,Old.R,TimeStp);
		 IterCalcMain(Reg,Old.E,Old.Ro,TimeStp);
		};




void SaveOld(RegionIO &Reg,HidData &Old)
		{
   Old.P =new double[Num+2];Old.R =new double[Num+2];Old.E =new double[Num+2];
   Old.Ro=new double[Num+2];Old.U =new double[Num+2];
			movmem(P_,Old.P,sizeof(double)*(Num+2));
			movmem(E_,Old.E,sizeof(double)*(Num+2));
			movmem(R_,Old.R,sizeof(double)*(Num+2));
			movmem(U_,Old.U,sizeof(double)*(Num+2));
			movmem(Ro_,Old.Ro,sizeof(double)*(Num+2));
		};
void OutDelta(RegionIO &Reg,HidData &Fst,HidData &Old,double TimeStp)
  {
			double dU,dRo;
			delta_file<<"Stp "<<TimeStp<<"\n";
			for (int k=1;k<Num;k++)
			  {
						dU=U_[k]-Fst.U[k];	dRo=Ro_[k]-Fst.Ro[k];
//	   if ( (!F_[k]) && (fabs(dRo)>StndErr) && (fabs(dU)>StndErr) )
			   if ( (!F_[k]) && (fabs(dRo)>1e-6   ) && (fabs(dU)>1e-6   ) )
						  {
			      delta_file<<FM<<R_[k]
//							<<FMT<<P_[k]-Fst.P[k]
							     <<FMT<<dU/(U_[k]-Old.U[k])
//						   <<FMT<<(dU/TimeStp)
							     <<FMT<<dRo/(Ro_[k]-Old.Ro[k])
//												<<FMT<<(dRo/TimeStp)
												<<"\n";
								}				
					}			
			delta_file<<"\n";
		};																	
void GetOld(RegionIO &Reg,HidData &Old) 
  {
			movmem(Old.P ,P_ ,sizeof(double)*(Num+2));
			movmem(Old.E ,E_ ,sizeof(double)*(Num+2));
			movmem(Old.R ,R_ ,sizeof(double)*(Num+2));
			movmem(Old.U ,U_ ,sizeof(double)*(Num+2));
			movmem(Old.Ro,Ro_,sizeof(double)*(Num+2));
		}	;
void DelSave(HidData &Old)
  {	delete Old.P;delete Old.E;delete Old.R;delete Old.U;delete Old.Ro;}
void PutDel(RegionIO &Reg,HidData &Old) 
  {
			movmem(P_,Old.P,sizeof(double)*(Num+2));
			movmem(E_,Old.E,sizeof(double)*(Num+2));
			movmem(R_,Old.R,sizeof(double)*(Num+2));
			movmem(U_,Old.U,sizeof(double)*(Num+2));
			movmem(Ro_,Old.Ro,sizeof(double)*(Num+2));
		};	
void AddDel(RegionIO &Reg,HidData &Fst,HidData &Sec,double coef)
  {
			for (int k=1;k<Num;k++)
			  {
						U_[k]+=coef*(Sec.U[k]-Fst.U[k]);
						E_[k]+=coef*(Sec.E[k]-Fst.E[k]);
						Ro_[k]+=coef*(Sec.Ro[k]-Fst.Ro[k]);
					}
		};		
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
void StepRegion(RegionIO &Reg,double TimeStp,double &MinTimeStp,double &PresDeriv)
  {
//	//		CalcPres(Reg,MinTimeStp);
// //  CellFixed(Reg,PresDeriv);
// //  CalcPos(Reg,TimeStp);CalcMain(Reg,TimeStp);
//cout<<"StepRegion  beg    Coreleft() "<<coreleft()<<"\n";
			  CalcPres(Reg,MinTimeStp);
					CellFixed(Reg,PresDeriv);
     HidData Old;
  			double tmp;
  			SaveOld(Reg,Old);
     for (int i=1;i<=4;i++) {OneIter(Reg,Old,TimeStp);CalcPres(Reg,tmp);}
     if (1==1)
       {
//        HidData Old1;
  //     for (int i=1;i<=4;i++) {OneIter(Reg,Old,TimeStp);CalcPres(Reg,tmp);}
  //      OneIter(Reg,Old,TimeStp);CalcPres(Reg,tmp);
        EndCalcPos(Reg,Old.U,TimeStp);
//        SaveOld(Reg,Old1);
        double *Old1U=new double[Num+2];movmem(U_,Old1U,sizeof(double)*(Num+2));
        IterCalcVel(Reg,U_,R_,TimeStp);
        int t=0;
        for (int k=1;k<=Num;k++) 
          {
  //         U_[k]=(0.3*U_[k]+0.7*Old.U[k])/1.3;
           if ( (U_[k]-Old1U[k])*(Old1U[k]-Old.U[k])<0 )
             {U_[k]=Old.U[k]+(U_[k]-Old.U[k]);t++;}
           else if (fabs(U_[k]-Old1U[k])>1e-10)
                  {
                   tmp=fabs((Old1U[k]-Old.U[k])/(U_[k]-Old1U[k]));
                   if ( (tmp<0.01) || (tmp>100) )
                     {U_[k]=Old.U[k]+(U_[k]-Old.U[k]);t++;}
                   else U_[k]=Old1U[k];
                  }
                else U_[k]=Old1U[k];
           P_[k]=Old.P[k];
           E_[k]=Old.E[k];
           Ro_[k]=Old.Ro[k];
           R_[k]=Old.R[k];
          }   
        cout<<" t "<<t<<"\n";
        IterCalcMain(Reg,Old.E,Old.Ro,TimeStp);
//        DelSave(Old1);
        delete Old1U;
       }
  			EndCalcPos(Reg,Old.U,TimeStp);
  			DelSave(Old);
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
   in>>tmp>>CurTime>>tmp>>TimeStp>>tmp>>TimeWrite>>
								                  tmp>>PresDerivCoef>>tmp>>EndTime;
			if ( (!in)  && (seek_eof(in)) ) return 0;
			return 1;
		};	         
		  
RegionIO* GetRegion(char *inname,fstream &output,int &FstIter,
                        double &CurTime,double &TimeStp,double &TimeWrite,
                                    double &PresDerivCoef,double &EndTime)
  {
   int Cont=1;FstIter=1;
			char tmp[150];
			double CurTime1,Buf;
   fstream input=fstream(inname,ios::in);
   RegionIO *Reg=NULL,*Reg1=NULL,*Reg0=NULL;
   Reg=new RegionIO;Reg1=new RegionIO;
   do
     {
      if (!FstIter) {input>>tmp;InHead(input,CurTime,Buf,Buf,Buf,Buf);}
						else InHead(input,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime);
      Cont=0;
      if (Reg->ReadIni(input))
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
//cout<<" Coreleft "<<coreleft()<<"\n";
   delete Reg1;
//cout<<" Coreleft "<<coreleft()<<"\n";
   OutHead(output,*Reg,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime);
//ChRead();
   return Reg;
		};	                  
void main( int argc, char *argv[] )
  {
cout<<" Coreleft "<<coreleft()<<"\n";
   char tmp[50];
   if ((argc<2) || (GetCmd("/h",argc,argv)!=NULL))
     { cout<<"usage:\n"<<argv[0]<<"  in_lmethod  output_file\n"<<
           "      /h - display help\n"; exit(1);  }
         my_file=new fstream(argv[2],ios::out);
   int FstIter=0;
   double TimeStp,EndTime,CurTime=0,TimeWrite,PresDerivCoef;
   RegionIO *Reg=GetRegion(argv[1],*my_file,FstIter,
			               CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime); 
   CoefUp=max(1,atoi(argv[3]));
//cout<<" Coreleft "<<coreleft()<<"\n";
//			OutHead(*my_file,*Reg,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime);
   ClcMass(*Reg);
   int x=1,BreakSignal=0;
//			int i=1;
//cout<<" TW     "<<TimeWrite<<" CurTime "<<CurTime<<" TimeStp "<<TimeStp<<"\n";
//ChRead();

   double CurStp,OldTimeWrite,NewTimeWrite,PresDeriv=1,OldStp=1e5;
   ClcMis(*Reg,1);
   while ((!(CurTime>=EndTime)) && (!BreakSignal))
     {
//						NewTimeWrite=min(CurTime+TimeWrite,EndTime);
						OldTimeWrite=CurTime;
						NewTimeWrite=min(OldTimeWrite+
									              TimeWrite/(1+PresDeriv*PresDerivCoef),EndTime);
//cout<<" Before While NewTime "<<NewTimeWrite<<"\n";
      while ((!(CurTime>=NewTimeWrite)) && (!BreakSignal))
        {
						   NewTimeWrite=min(OldTimeWrite+
									              TimeWrite/(1+PresDeriv*PresDerivCoef),EndTime);
				     if (x<100) {x++;CurStp=0.5*log10(x)*TimeStp;}
				     else 
									  CurStp=TimeStp;
				     if (CurStp+CurTime>NewTimeWrite) CurStp=NewTimeWrite-CurTime;
									if (CurStp<MathZer) break;
         if (CurStp>OldStp) CurStp=OldStp*(CoefUp*log(CurStp/OldStp)+1);
         OldStp=CurStp;
cout<<" CurStp "<<CurStp<<" CurTime "<<CurTime<<" Coreleft "<<coreleft()<<"\n";
//ChRead();
//         if ((x==2) && (FstIter)) CalcPos(*Reg,-0.5*CurStp,TimeStp);
//cout<<" StepRegion \n";
         CurTime+=CurStp;StepRegion(*Reg,CurStp,TimeStp,PresDeriv);
         while ((BreakSignal=bioskey(1))!=0) if (BreakSignal==ESC) break;else {cout<<" "<<BreakSignal;bioskey(0);}
//         if (i++==10) {i=0;Reconstruct(*Reg);}
//      Reg.SaveIni(output);
				    }
					OutHead(*my_file,*Reg,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime);
cout<<" Write !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n";
    ClcMis(*Reg);
cout<<" Miss_P "<<Mis_DeltaP<<" Miss_Einter "<<Mis_DeltaE
    <<" Miss_Ekin "<<Mis_DeltaEv<<" Miss_Efull "<<Mis_DeltaEF<<"\n";
					}
//cout<<" Reg.InterfaceIODelete();  \n";
cout<<" Coreleft "<<coreleft()<<"\n";
    ClcMis(*Reg);
cout<<" Miss_P "<<Mis_DeltaP<<" Miss_Einter "<<Mis_DeltaE
    <<" Miss_Ekin "<<Mis_DeltaEv<<" Miss_Efull "<<Mis_DeltaEF<<"\n";
    Reg->InterfaceIODelete();
				my_file->close();
cout<<" Coreleft "<<coreleft()<<"\n";
  };



/*
void StepRegion(RegionIO &Reg,double TimeStp,double &MinTimeStp,double &PresDeriv)
  {
			CalcPres(Reg,MinTimeStp);
   CellFixed(Reg,PresDeriv);
//cout<<"StepRegion  beg    Coreleft() "<<coreleft()<<"\n";
			HidData Old,Fst,Sec;
			double tmp;
			SaveOld(Reg,Old);
   OneIter(Reg,Old,0.1*TimeStp);                
			CalcPres(Reg,tmp);SaveOld(Reg,Fst);
			for (int i=1;i<=10;i++) {OneIter(Reg,Old,0.1*TimeStp);CalcPres(Reg,tmp);}
			SaveOld(Reg,Sec);
			GetOld(Reg,Old);OneIter(Reg,Old,TimeStp);AddDel(Reg,Fst,Sec,10);
			EndCalcPos(Reg,Old.U,TimeStp);
			DelSave(Old);DelSave(Fst);DelSave(Sec);
//			CalcPres(Reg,MinTimeStp);
//   CellFixed(Reg,PresDeriv);
//   CalcPos(Reg,TimeStp);CalcMain(Reg,TimeStp);
  };
*/

