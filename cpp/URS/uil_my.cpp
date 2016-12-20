#include <lib/precompiled/cell_kru.h>

  
//#include <alloc.h>
#include "uil_reg.h"
#include "lib/std/ex_out.h"
#include "lib\ref\class_sav.h"
#include "uil_e.h"

//
//  units for pressure : (gram*km)/(cm^2*c^2)  
//  units for energy   : (km/c)^2
//  units for dencity  :  gram/cm^3
//  units for velocity :  km/c   
//


#ifdef WCPP
#define ESC 27
#else
#define ESC 283
#endif


extern unsigned _stklen=64000;


fstream *my_file;



void PreClc(Region &Reg) { Reg.ClcMass();};


void StepRegion(Region &Reg,double TimeStp,double &MinTimeStp,double &PresDeriv)
  {
   Reg.StepRegion(TimeStp,MinTimeStp,PresDeriv);
  };
void Reconstruct(Region &Reg){};
void OutHead(fstream &out,Region &Reg,double CurTime,double TimeStp,
             double TimeWrite,double PresDerivCoef,double EndTime, Ref<ExternalEnergyInput> &en, int DataOnly=0)
  {
   out<<" CurTime "<<CurTime<<" TimeStp "<<TimeStp<<" TimeWrite "<<TimeWrite
							       <<" PresDerivCoef "<<PresDerivCoef<<" EndTime "<<EndTime;
   if (en) {
       out<<" { ExternalEnergy ";
       SavableClass::Save(out, en);
       out<<" } ";
   }
   out<<"\n";
   if (DataOnly) 
       Reg.SaveMatter(out);
   Reg.SaveData(out);
  };	           
int InHead(fstream &in,double &CurTime,double &TimeStp,double &TimeWrite,
           double &PresDerivCoef,double &EndTime, Ref<ExternalEnergyInput> &energyAdd) {
	char tmp[256];
    in>>tmp>>CurTime>>tmp>>TimeStp>>tmp>>TimeWrite>>tmp>>PresDerivCoef>>tmp>>EndTime;
    if ( (!in)  && (seek_eof(in)) ) 
        return 0;
    
    if (SavableClass::TestNextChar(in, '{', 1)) {
        in>>tmp;
        energyAdd << SavableClass::Read(in);
        TestNextWord(in, "}", "ExternalEnergyInput : no closing braces\n");
    }
    return 1;
};	         
		  
Region* GetRegion(char *inname,fstream &output,int &FstIter,
                        double &CurTime,double &TimeStp,double &TimeWrite,
                                    double &PresDerivCoef,double &EndTime, Ref<ExternalEnergyInput> &energyAdd)
  {
   int Cont=1,HeadRead=1;FstIter=1;
	  char tmp[150];
  	double CurTime1,Buf;
   fstream input(inname,ios::in);
   Region *Reg=NULL,*Reg1=NULL,*Reg0=NULL;
   Reg=new Region;Reg1=new Region;
   Ref<ExternalEnergyInput> enBad;

   InHead(input,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime, energyAdd);

   if ( (!Reg->ReadMatter(input)) || (!Reg->ReadData(input)) )
      { cout<<" Could not read ReadIni. exiting.\n";exit(0);}
   input>>tmp;
   if (!seek_eof(input)) {
        input.close();
        input.open(inname,ios::in);
        InHead(input,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime, enBad);
        Reg1->ReadMatter(input);Reg1->ReadData(input);
   }
   input>>tmp;
   OutHead(output,*Reg,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime, energyAdd, 1);
   if (InHead(input,CurTime1,Buf,Buf,Buf,Buf, enBad)) 
    {
	    if (!Reg1->ReadData(input)) goto exit_loop;
	    input>>tmp;OutHead(output,*Reg1,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime, (enBad = NULL));
     Reg0=Reg;Reg=Reg1;Reg1=Reg0;CurTime=CurTime1;
     while (InHead(input,CurTime1,Buf,Buf,Buf,Buf, enBad)) 
       {
//      if (!Reg1->ReadData(input)) goto exit_loop;
        if (!Reg1->ReadData(input)) goto exit_loop;
	       input>>tmp;OutHead(output,*Reg1,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime, (enBad = NULL));
        Reg0=Reg;Reg=Reg1;Reg1=Reg0;CurTime=CurTime1;
       }
    };
exit_loop:
cout<<" Bef del Coreleft "<<Coreleft()<<"\n";
   delete Reg1;
cout<<" Aft del Coreleft "<<Coreleft()<<"\n";
   return Reg;
};
int main( int argc, char *argv[] ){
	SetLeakTest();
	Time_struct time;
    try {

        argc--;
        cout<<" uil_my - version\n";   
        if ((argc<2) || (GetCmd("/h",argc,argv)!=NULL)) { 
            cout<<"Program build time "<< __TIMESTAMP__<<"\n";
            cout<<Stroka(" Registered curves:\n\n\n")<<SavableClass::HelpForCategory("ExternalEnergy_category");

            cout<<"usage:\n"<<argv[0]<<"  in_lmethod  output_file\n"<<
            "      /h - display help\n"<<
            "      /dN - set dimension N\n"<<
            "      /s - Skip vacuum for better show\n"<<
            "      /oT - output in T times more frequently when something happens on the bound\n"<<
            "      /eN - Skip calculation of sound N times ( 10 by default )\n";//<<
        //#ifndef InternalPointsUrs
        //       "  current version - no internal points \n";
        //#else
        //       "  current version - with internal points \n";
        //#endif

        
            exit(1);
        }

        my_file=new fstream(argv[2],ios::out);

        int FstIter=0;
        double TimeStp,EndTime,CurTime=0,TimeWrite,PresDerivCoef;
        Ref<ExternalEnergyInput> energyAddition;
        Region *Reg=GetRegion(argv[1],*my_file,FstIter,
			                    CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime, energyAddition); 

        double CoefUp=1,NumSkipS=10;
        char *tmp;
        if ( (tmp=GetCmd("/e",argc,argv))!=NULL )  NumSkipS=atoi(tmp);
        if ( (tmp=GetCmd("/o",argc,argv))!=NULL )  CoefUp=min(atof(tmp),1);
        if ( (tmp=GetCmd("/s",argc,argv))!=NULL )  Reg->RegionShow=1;
        int d=1;
        if ( (tmp=GetCmd("/d",argc,argv))!=NULL )  d=atoi(tmp);
        cout<<d<<"\n";

        Reg->SetBodySkipS(NumSkipS);

        Reg->SetWorkDim(d);
        Reg->ClcMass();
        int x=1;

        double CurStp,OldTimeWrite,NewTimeWrite,PresDeriv=1,OldStp=1e5;
        int BreakSignal=0;
        Time_struct Time;
        double CurStpSum=0;int CurStpNum=1;
        Ref<ExternalEnergyInput> enBad = NULL;
        cout<<" Before clc Coreleft "<<Coreleft()<<"\n";
        while ((!(CurTime>=EndTime)) && (!BreakSignal)){
   	        OldTimeWrite=CurTime;
  	        NewTimeWrite=min(OldTimeWrite+
	                            TimeWrite/(1+PresDeriv*PresDerivCoef),EndTime);
        //cout<<" Before While NewTime "<<NewTimeWrite<<"\n";
            while ((!(CurTime>=NewTimeWrite)) && (!BreakSignal)){
 	            NewTimeWrite=min(OldTimeWrite+
		                                TimeWrite/(1+PresDeriv*PresDerivCoef),EndTime);
      		    if (x<100) {x++;CurStp=0.5*log10((double)x)*TimeStp;}
      		    else CurStp=TimeStp;
      		    if (CurStp+CurTime>NewTimeWrite) CurStp=NewTimeWrite-CurTime;
		        if (CurStp<MathZer) break;
                if (CurStp>OldStp) CurStp=OldStp*(CoefUp*log(CurStp/OldStp)+1);
                OldStp=CurStp;
                CurStpSum+=CurStp;CurStpNum++;
                if (energyAddition) 
                    energyAddition->ClcAddition(Reg, CurTime, CurStp);

                CurTime+=CurStp;
                StepRegion(*Reg,CurStp,TimeStp,PresDeriv);

			}
			OutHead(*my_file,*Reg,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime, enBad);
            cout<<" Write !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n";
            cout<<" CurStp "<<CurStpSum/CurStpNum<<" CurTime "<<CurTime<<"\n";     
            CurStpNum=1;CurStpSum=0;
#if defined WCPP || defined WCPP_NET
		    if (_kbhit()) { BreakSignal=_getch();if (BreakSignal==ESC) break;}
#else
            while ((BreakSignal=bioskey(1))!=0) if (BreakSignal==ESC) break;else {cout<<" "<<BreakSignal;bioskey(0);}
#endif
        }
        cout<<Time<<"\n"<<" Coreleft "<<Coreleft()<<"\n";
        Reg->DeleteRegion();
   	    my_file->close();
   	    delete my_file;
   	    delete Reg;
    } catch(exception &ex){ 
        cout<<" Exeption caught:\n"<<ex.what()<<"\n";
        return 1;
    }
    LeakTest();
    return 0;
};




/*
   int FstIter=0;
   double TimeStp,EndTime,CurTime=0,TimeWrite,PresDerivCoef;
   Region *Reg=GetRegion(argv[1],*my_file,FstIter,
			               CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime); 
//	my_file->close();};/*

   if (argc>2) CoefUp=max<double>(1,atoi(argv[3]));else CoefUp=1;
//cout<<" Coreleft "<<coreleft()<<"\n";
//			OutHead(*my_file,*Reg,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime);
   PreClc(*Reg);
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
//         if ((x==2) && (FstIter)) CalcPos(*Reg,-0.5*CurStp,TimeStp); // Exracted? 
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
    delete Reg;
	   my_file->close();
cout<<" Coreleft "<<Coreleft()<<"\n";
  };
*/
//*/



