  
//#include <alloc.h>
//
//  units for pressure : (gram*km)/(cm^2*c^2)  
//  units for energy   : (km/c)^2
//  units for dencity  :  gram/cm^3
//  units for velocity :  km/c   
//
#include "uil_reg.h"

extern unsigned _stklen=64000;

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
 
void OutHead(fstream &out,Region &Reg,double CurTime,double TimeStp,
             double TimeWrite,double PresDerivCoef,double EndTime,int DataOnly=0)
  {
   out<<" CurTime "<<CurTime<<" TimeStp "<<TimeStp<<" TimeWrite "<<TimeWrite
							       <<" PresDerivCoef "<<PresDerivCoef<<" EndTime "<<EndTime<<"\n";
   if (DataOnly) Reg.SaveMatter(out);
   out<<" CurTime "<<CurTime<<" TimeStp "<<TimeStp<<" TimeWrite "<<TimeWrite
							       <<" PresDerivCoef "<<PresDerivCoef<<" EndTime "<<EndTime<<"\n";

//   Reg.SaveData(out);

   for (int k=1;k<=Reg.NumMatter;k++) 
     {
//    B[k]->SaveData(output);
      Body *B=Reg.B[k];
      int i=0;
      double d[17];
      while (i<B->NumPnt-1)
        {
         i++;
         d[1]=B->Pos     [i];
         d[2]=B->Pressure[i]*1e-5;
         d[3]=B->Dencity [i];
         d[4]=B->Energy  [i];
         d[5]=B->Velocity[i];
         d[6]=B->Matter->Temperature(d[3],d[4]);
         WriteStr(out,&d[0],6);
//      d[6]=GetNumMatter(i);
//      d[7]=BndMatter[d[6]];
//      WriteStr(output,&d[0],7);
     }

  }; 

   out<<"EndIter \n";
  };	           
//Region* 
void GetRegion(char *inname,fstream &output)
  {
   int Cont=1,HeadRead=1,FstIter=1;
	  char tmp[150];
  	double Buf;
   double CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime,CurTime1;
   fstream input=fstream(inname,ios::in);
   Region *Reg=new Region;

   InHead(input,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime);
   if ( (!Reg->ReadMatter(input)) || (!Reg->ReadData(input)) )
      { cout<<" Could not read ReadIni. exiting.\n";exit(0);}
   input>>tmp;
   OutHead(output,*Reg,CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime,1);
   while ( (InHead(input,CurTime1,Buf,Buf,Buf,Buf)) && (Reg->ReadData(input)) )
     {
      input>>tmp;
      OutHead(output,*Reg,CurTime1,TimeStp,TimeWrite,PresDerivCoef,EndTime);
cout<<" temp ";
     };
cout<<" Bef del Coreleft "<<Coreleft()<<"\n";
   delete Reg;
cout<<" Aft del Coreleft "<<Coreleft()<<"\n";
};

void main( int argc, char *argv[] )
  {
   argc--;
cout<<" Coreleft "<<Coreleft()<<"\n";
   char tmp[50];
   if ((argc<2) || (GetCmd("/h",argc,argv)!=NULL))
     { cout<<"usage:\n"<<argv[0]<<"  in_data  output_file\n"<<
           "      /h - display help\n"; exit(1);  }
   fstream *my_file=new fstream(argv[2],ios::out);
   GetRegion(argv[1],*my_file);
  };


/*
#include "cell_fac.h"
#include "spline.h"
#include "h2_liqui.h"
void CurSaveReg(fstream &output,RegionIO &Reg)
  {
   char tmp[256];
   int k;
   output<<"NumPnt   "<<Reg.NumPnt<<"  LftPnt   "<<Reg.LftPnt<<"  RgtPnt   "<<Reg.RgtPnt
         <<" Viscouse "<<Reg.Viscouse<<"\n";
   output<<"LftBnd_Free "<<Reg.LftFree<<" RgtBnd_Free "<<Reg.RgtFree
         <<" TimeStability "<<Reg.TimeStability<<"\n";
   output<<"NumMatters "<<Reg.NumMatter<<" Matter_Boundaries ";
   for (k=1;k<Reg.NumMatter;k++) output<<Reg.BndMatter[k]<<"  ";
   output<<"\n";
   for (k=1;k<=Reg.NumMatter;k++) 
      if (!MatterStore(output,Reg.Matter[k]) ) 
        { cout<<"RegionIO::SaveIni;  Could not save     matter. Exiting.\n";exit(1);}
   int i=0;
   double d[17];//,Stp=(RgtPnt-LftPnt)/(NumPnt-1);

   output<<"   Pos         Pressure   Dencity    Energy     Velocity   Temperature\n";
   while (i<Reg.NumPnt)
     {
      i++;
      d[1]=Reg.Pos     [i];
      d[2]=Reg.Pressure[i]*1e-5;
      d[3]=Reg.Dencity [i];
      d[4]=Reg.Energy  [i];
      d[5]=Reg.Velocity[i];
      d[6]=Reg.Matter[Reg.GetNumMatter(i)]->Temperature(d[3],d[4]);
      WriteStr(output,&d[0],6);
//      d[6]=GetNumMatter(i);
//      d[7]=BndMatter[d[6]];
//      WriteStr(output,&d[0],7);
     }

  }; 
   
void OutHead(fstream &out,RegionIO &Reg,double CurTime,double TimeStp,
             double TimeWrite,double PresDerivCoef,double EndTime)
  {
   out<<" CurTime "<<CurTime<<" TimeStp "<<TimeStp<<" TimeWrite "<<TimeWrite
							       <<" PresDerivCoef "<<PresDerivCoef<<" EndTime "<<EndTime<<"\n";
   CurSaveReg(out,Reg);out<<" EndIter\n";
//   Reg.SaveIni(out);out<<" EndIter\n";
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
		  
void GetRegion(char *inname,fstream &output)
  {
   int FstIter;
   double CurTime,TimeStp,TimeWrite,PresDerivCoef,EndTime;
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
		};	                  

*/