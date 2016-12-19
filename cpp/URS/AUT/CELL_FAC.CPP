
#include "cell_fac.h"

//extern unsigned _stklen=60000;
//double StndErr=1e-6;
double StndErr=8e-4;

const char * const RegionIO::name="RegionIO";

   
// NumPnt 100 Lft 0 Rgt 10  Viscouse 1
// Pos         Pressure   Dencity    Energy     Velocity  

int RegionIO::ReadIni(fstream &input)
  {
//cout<<"RegionIO::ReadIni\n";
   InterfaceIODelete();
//cout<<"RegionIO::ReadIni   Deleted\n";
   char tmp[256],tmp1[256];
   int k;NumIntPar=0;
   input>>tmp>>NumPnt>>tmp>>LftPnt>>tmp>>RgtPnt;
#ifdef InternalPointsUrs
input>>tmp>>NumIntPar;
#endif
   input.getline(tmp,256);
   input>>tmp>>LftFree>>tmp>>RgtFree>>tmp>>TimeStability;input.getline(tmp1,256);
cout<<" TimeStability "<<TimeStability<<"\n";
   input>>tmp>>Viscouse>>tmp>>Viscouse2;input.getline(tmp1,256);
//cout<<" Viscouse2 "<<tmp<<"\n";
   input>>tmp>>NumMatter>>tmp;
cout<<" NumMatter "<<NumMatter<<"\n";
   for (k=1;k<NumMatter;k++) input>>BndMatter[k];
   BndMatter[NumMatter]=NumPnt;
   BndMatter[0]=1;
//   input>>tmp;
//   for (k=1;k<=NumMatter;k++) input>>Viscouse[k];
   input.getline(tmp,256);
   for (k=1;k<=NumMatter;k++) 
      if (!MatterRegister(input,Matter[k]) ) 
        { cout<<"RegionIO::ReadIni;  Could not register matter. Exiting.\n";
          for (int i=1;i<k;i++) delete Matter[i];
exit(1);
          return 0;
         }//exit(1);
   int i=0,NumR=5+NumIntPar;
			if (NumIntPar>0) //IntPar=new double[NumPnt+2][NumIntPar];
			  {
						IntPar=new double*[NumPnt+2];
			   for (k=0;k<=NumPnt+1;k++) IntPar[k]=new double[NumIntPar];
					}	
//   double d[7];//,Stp=(RgtPnt-LftPnt)/(NumPnt-1);
			double *d=new double [7+NumIntPar];

   Pos     =new double[NumPnt+2];
   Pressure=new double[NumPnt+2];
   Dencity =new double[NumPnt+2];
   Energy  =new double[NumPnt+2];
   Velocity=new double[NumPnt+2];
   Mass    =new double[NumPnt+2];

   PresL =new double[NumPnt+2];
   SounL =new double[NumPnt+2];
   int NumL=0;for (k=1;k<=NumMatter;k++) NumL=max<double>(NumL,BndMatter[k]-BndMatter[k-1]);
//cout<<" NumL "<<NumL<<"\n";ChRead();
   TmpE  =new double[NumL+2];
   TmpP  =new double[NumL+2];
   TmpD  =new double[NumL+2];
   FstCalcP=1;FstCalcS=1;
   Fixed   =new int[NumPnt+2];
			input.getline(tmp,255);
			input.getline(tmp,255);
cout<<" Reading Param RegionIO::ReadIni Num "<<NumPnt<<" Coreleft "<<Coreleft()<<"\n";
   while ( (i<NumPnt) && (!seek_eof(input)) && 
			                      (EnterStr(input,&d[0],NumR+1,1)==NumR) )
//      if ( (EnterStr(input,&d[0],6)==5) ) 
        {                    
         i++;
         Pos     [i]=d[1];
         Pressure[i]=d[2]*1e5;
         Dencity [i]=d[3];
         Velocity[i]=d[5];
//         Energy  [i]=d[4]+sqr(Velocity[i])*0.5;
         Energy  [i]=d[4];
									if (NumIntPar>0) MemoryMove(&d[6],IntPar[i],sizeof(double)*NumIntPar);
//         Length  [i]=d[6];
//cout<<" Entering ?\n";
        }
			delete d;
   if ((i<NumPnt) || (NumPnt==0))
     {cout<<"RegionIO::ReadIni; Entered only part of points NumPnt i "
          <<NumPnt<<"   "<<i<<"\n";
      if (i!=0) InterfaceIODelete();
						return 0;
//      exit(1);
      }
// Now it is not used
//  Right - Calculating masses between points
//   double Cur=0.5*(2*Pos[1]-Pos[2]);
//   for(i=1;i<NumPnt;i++)
//     {
//      Cur=2*Pos[i]-Cur;
//      Mass[i]=Dencity[i]*(Pos[i]-Pos[i-1]);
//      Mass[i]=Dencity[i+1]*(Pos[i+1]-Cur)+Dencity[i]*(Cur-Pos[i]);
//     }
//   Mass[NumPnt]=Mass[NumPnt-1];
//   Mass[1]=Mass[2];
//cout<<" Extend bounds RegioIO.Read\n";
   ExtendBnd();
//cout<<" Exit RegioIO.Read\n";
   return 1;
  }; 

void RegionIO::SaveIni(fstream &output)
  {
   char tmp[256];
   int k;
   output<<"NumPnt   "<<NumPnt<<"  LftPnt   "<<LftPnt<<"  RgtPnt   "<<RgtPnt;
#ifdef InternalPointsUrs
   output<<" NumIntPar "<<NumIntPar;
#endif
			output<<"\n";
   output<<"LftBnd_Free "<<LftFree<<" RgtBnd_Free "<<RgtFree
         <<" TimeStability "<<TimeStability<<"\n";
   output<<" Viscouse "<<Viscouse<<" Viscouse2 "<<Viscouse2<<"\n";
   output<<"NumMatters "<<NumMatter<<" Matter_Boundaries ";
   for (k=1;k<NumMatter;k++) output<<BndMatter[k]<<"  ";
   output<<"\n";
//   output<<" Viscouse ";
//   for (k=1;k<=NumMatter;k++) output<<Viscouse[k]<<"  ";
//   output<<"\n";
   for (k=1;k<=NumMatter;k++) 
      if (!MatterStore(output,Matter[k]) ) 
        { cout<<"RegionIO::SaveIni;  Could not save     matter. Exiting.\n";exit(1);}
   int i=0;
//   double d[17];//,Stp=(RgtPnt-LftPnt)/(NumPnt-1);
			double *d=new double [7+NumIntPar];

   output<<"   Pos         Pressure   Dencity    Energy     Velocity   \n";
   while (i<NumPnt)
     {
      i++;
      d[1]=Pos     [i];
      d[2]=Pressure[i]*1e-5;
      d[3]=Dencity [i];
      d[5]=Velocity[i];
      d[4]=Energy  [i];
//      d[4]=Energy  [i]-sqr(Velocity[i])*0.5;
						if (NumIntPar>0) MemoryMove(IntPar[i],&d[6],sizeof(double)*NumIntPar);

      WriteStr(output,&d[0],5+NumIntPar);
//      d[6]=GetNumMatter(i);
//      d[7]=BndMatter[d[6]];
//      WriteStr(output,&d[0],7);
     }
			delete d;

  }; 


int RegionIO::GetNumMatter(int k)
  {
   for (int i=1;i<NumMatter;i++)
      if (k<BndMatter[i]) return i;
   return NumMatter;
  };   
void RegionIO::MullVisc(double *tmp)
  {
   int i,k,b;
   double c=Viscouse;
   for ( i=1;i<=NumPnt;i++) tmp[i]*=c;
//   for ( i=1;i<=NumMatter;i++)
//     {
//      for (k=BndMatter[i-1];k<BndMatter[i];k++) tmp[k]*=c;
//     }

//   for ( k=1;k<NumMatter;k++)
//     {
//      b=BndMatter[k];
//      if (Mass[b-1]>100*Mass[b]) tmp[b]*=2;
//     }
		};			
   
void RegionIO::InterfaceIODelete()
 { 
  if (Pressure!=NULL)
    {
cout<<" RegionIO::InterfaceIODelete(); delete Matter Pressure\n"<<Pressure<<"\n";
//ChRead();
					int k;
     delete Pressure;Pressure=NULL;delete Energy  ;Energy  =NULL;
     delete Dencity ;Dencity =NULL;delete Velocity;Velocity=NULL;
     delete Pos     ;Pos     =NULL;
     delete Mass    ;Mass    =NULL;
     
     delete PresL   ;PresL   =NULL;delete SounL   ;SounL   =NULL;
     delete Fixed   ;Fixed   =NULL;
     delete TmpE;TmpE=NULL;delete TmpP;TmpP=NULL;delete TmpD;TmpD=NULL;
	 if (NumIntPar>0) //IntPar=new double[NumPnt+2][NumIntPar];
	   {for (k=0;k<=NumPnt+1;k++) delete IntPar[k];delete IntPar;}
//cout<<"     RegionIO::InterfaceIODelete();Matter \n";ChRead();
     for (k=1;k<=NumMatter;k++) {delete Matter[k];Matter[k]=NULL;}
     NumIntPar=0;
    }
//cout<<"     RegionIO::InterfaceIODelete(); \n";ChRead();
 };

void RegionIO::Pres()
  {  
   int l,b,k,i;
   for (i=1;i<=NumMatter;i++)
			  {
						b=BndMatter[i-1]-1;l=BndMatter[i]-BndMatter[i-1];
#ifdef InternalPointsUrs
						if (Matter[i]->NumIntPar()!=0) Matter[i]->SetIntParArr(IntPar,b);
			   Matter[i]->Pressure(&Pressure[b],&Dencity[b],&Energy[b],l);
#else
//      FstCalcP=1;
      if (FstCalcP)
			   Matter[i]->Pressure(&Pressure[b],&Dencity[b],&Energy[b],l);
      else 
        {
//         double  *TmpP=new double [l],*TmpD=new double [l],*TmpE=new double [l];
         double  *TP=TmpP,*TD=TmpD,*TE=TmpE;
         int n=0,o;
         for (o=b+1;o<=b+l;o++) 
             {
//cout<<o<<" ";
              if (!Fixed[o]) {TD[n]=Dencity[o];TE[n]=Energy[o];n++;}}
         Matter[i]->Pressure(&TP[-1],&TD[-1],&TE[-1],n);
//cout<<" PT "<<TmpP[int(n/2)]<<" n "<<n<<"\n";
//for (k=0;k<n;k++) cout<<" PT "<<TmpP[k]<<" k "<<k<<"\n";
         n=0;
         for (o=b+1;o<=b+l;o++) if (!Fixed[o]) {Pressure[o]=TP[n];n++;} else Pressure[o]=PresL[o];
//         delete TmpP;delete TmpE; delete TmpD;
        }
#endif
     }
     if (LftFree) { Pressure[0]=0;Energy[0]=0;}
     if (RgtFree) { Pressure[NumPnt]=0;Energy[NumPnt]=0;}
   MemoryMove(Pressure,PresL,sizeof(double)*(NumPnt+2));FstCalcP=0;
   for ( k=1;k<NumPnt;k++) Pressure[k]=max<double>(Pressure[k]*1e5,10);
//   for ( k=1;k<NumPnt;k++) cout<<" P "<<Pressure[k]<<" k "<<k<<"\n";
//   for ( k=1;k<NumPnt;k++) cout<<" PL "<<PresL[k]<<" k "<<k<<"\n";
//   ChRead();
  };
void RegionIO::Sound(double *S)
{  
   int l,b,k;
   for (int i=1;i<=NumMatter;i++)
   {
	b=BndMatter[i-1]-1;l=BndMatter[i]-BndMatter[i-1];
#ifdef InternalPointsUrs
	if (Matter[i]->NumIntPar()!=0) Matter[i]->SetIntParArr(IntPar,b);
	Matter[i]->Sound(&S[b],&Dencity[b],&Energy[b],l);
#else
//      FstCalcS=1;
      if (FstCalcS)
	Matter[i]->Sound(&S[b],&Dencity[b],&Energy[b],l);
      else 
        {
         double  *TS=TmpP,*TD=TmpD,*TE=TmpE;
         int n=0,o;
         for (o=b+1;o<=b+l;o++) 
             if (!Fixed[o]) {TD[n]=Dencity[o];TE[n]=Energy[o];n++;}
         Matter[i]->Sound(&TS[-1],&TD[-1],&TE[-1],n);
//for (k=0;k<n;k++) cout<<" ST "<<TmpS[k]<<"\n";
         n=0;
         for (o=b+1;o<=b+l;o++) if (!Fixed[o]) {S[o]=TS[n];n++;} else S[o]=SounL[o];
//         delete TmpS;delete TmpE; delete TmpD;
        }
#endif
	}

  MemoryMove(S,SounL,sizeof(double)*(NumPnt+2));FstCalcS=0;
//   for (k=1;k<NumPnt;k++) cout<<" S "<<S[k]<<" k "<<k<<"\n";
//   for ( k=1;k<NumPnt;k++) cout<<" SL "<<SounL[k]<<" k "<<k<<"\n";
//   ChRead();
};			

void RegionIO::TimeCh(double TimeStp)
  {  
#ifdef InternalPointsUrs
//cout<<"RegionIO::TimeCh(double TimeStp)/n";
   int l,b;
   for (int i=1;i<=NumMatter;i++)
   {
 	b=BndMatter[i-1]-1;l=BndMatter[i]-BndMatter[i-1];
	if (Matter[i]->NumIntPar()!=0) 
	  {
	   Matter[i]->SetIntParArr(IntPar,b);
	   Matter[i]->ChangeIntParArr(&Dencity[b],&Energy[b],l,TimeStp);
	   }
	}					
#endif
	};			
/*
double RegionIO::Pres(int k,double dV)
  {  return Pres(Matter[GetNumMatter(k)],Dencity[k],Energy[k],Velocity[k],dV);}
double RegionIO::Sound(int k)
  {  return Sound(Matter[GetNumMatter(k)],Dencity[k],Energy[k],Velocity[k]);}
*/

/*
double RegionIO::Pres(int k,RegionData &Dat,double dV)
  {
//   int i= GetNumMatter(k+1);
//   if ((i==1) || ((k+1)!=BndMatter[i-1]))
   int i= GetNumMatter(k);
     return Pres(Matter[i],Dat.Dencity[k],Dat.Energy[k],Dat.Velocity[k],dV);
//   else 
//     {
//						double De=Dat.Energy[k]-0.5*(Energy[k]+Energy[k+1]);
//						double Dr=Dat.Dencity[k]-0.5*(Dencity[k]+Dencity[k+1]);
//						double S1=sqr(Sound(k)),S2=sqr(Sound(k+1));
//						if (S1<MathZer) S1=MathZer;
//						if (S2<MathZer) S2=MathZer;
//						double Dr1=Dr/(1+S1/S2),Dr2=Dr/(1+S2/S1);
//						double De1=De/(1+S2/S1),De2=De/(1+S1/S2);
//     return 
//        (  Pres(Matter[i-1],Dat.Dencity[k-1],
//                            Dat.Energy[k-1],Dat.Velocity[k-1])+
//           Pres(Matter[i  ],Dat.Dencity[k+1],
//                            Dat.Energy[k+1],Dat.Velocity[k+1]))*0.5;
//  
//              (  Pres(Matter[i-1],Dencity[k]+Dr1,
//                                  Energy[k]+De1,Dat.Velocity[k],dV)/Dencity[k]+
//                 Pres(Matter[i  ],Dencity[k+1]+Dr2,
//                                  Energy[k+1]+De2,Dat.Velocity[k],dV)/Dencity[k+1])/
//      																												(1/Dencity[k]+1/Dencity[k+1]);
//              (  Pres(Matter[i-1],Dat.Dencity[k-1],
//                                  Dat.Energy[k-1],Dat.Velocity[k-1],dV)/Dat.Dencity[k-1]+
//                 Pres(Matter[i  ],Dat.Dencity[k+1],
//                                  Dat.Energy[k+1],Dat.Velocity[k+1],dV)/Dat.Dencity[k+1])/
//      																												(1/Dat.Dencity[k-1]+1/Dat.Dencity[k+1]);
//            (  Pres(Matter[i-1],Dat.Dencity[k-1],
//                                Dat.Energy[k-1],Dat.Velocity[k-1],dV)*Dat.Dencity[k-1]+
//               Pres(Matter[i  ],Dat.Dencity[k+1],
//                                Dat.Energy[k+1],Dat.Velocity[k+1],dV)*Dat.Dencity[k+1])/
//    																												(Dat.Dencity[k-1]+Dat.Dencity[k+1]);
//     }  
  }
double RegionIO::Sound(int k,RegionData &Dat)
  {
   int i= GetNumMatter(k+1);
   if ((i==1) || ((k+1)!=BndMatter[i-1]))
     return Sound(Matter[i],Dat.Dencity[k],Dat.Energy[k],Dat.Velocity[k]);
   else 
     {
     return 
          (  Sound(Matter[i-1],Dat.Dencity[k-1],
                              Dat.Energy[k-1],Dat.Velocity[k-1])+
             Sound(Matter[i  ],Dat.Dencity[k+1],
                              Dat.Energy[k+1],Dat.Velocity[k+1]))*0.5;
     }  
  };

RegionData& RegionData::operator=(RegionIO& Reg)
  {
			if ((DeletedPnt) || (NumPnt!=Reg.NumPnt)) SetData(Reg.NumPnt);
   movmem(Reg.Pos     ,Pos     ,sizeof(double)*(NumPnt+2));
   movmem(Reg.Pressure,Pressure,sizeof(double)*(NumPnt+2));
   movmem(Reg.Dencity ,Dencity ,sizeof(double)*(NumPnt+2));
   movmem(Reg.Energy  ,Energy  ,sizeof(double)*(NumPnt+2));
   movmem(Reg.Velocity,Velocity,sizeof(double)*(NumPnt+2));
			return *this;
		};

RegionIO& RegionIO::operator=(RegionData& Dat)
  {
			if (NumPnt!=Dat.NumPnt) 
			  { cout<<"Error in: RegionIO& operator=(RegionData& Dat)  "
					      <<" - NumPnt are different\nExiting\n";exit(1);};
   movmem(Dat.Pos     ,Pos     ,sizeof(double)*(NumPnt+2));
   movmem(Dat.Pressure,Pressure,sizeof(double)*(NumPnt+2));
   movmem(Dat.Dencity ,Dencity ,sizeof(double)*(NumPnt+2));
   movmem(Dat.Energy  ,Energy  ,sizeof(double)*(NumPnt+2));
   movmem(Dat.Velocity,Velocity,sizeof(double)*(NumPnt+2));
			return *this;
		};
*/
