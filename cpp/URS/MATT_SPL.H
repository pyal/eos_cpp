#ifndef __MATT_SPL_H
#define __MATT_SPL_H

#include "lib\std\deffunc.h"
#include "lib\std\const.h"
#include "matt_fac.h"
#include "fre_fac.h"

#include "mat\dat_fac.h"
#include "mat\dat_reg.h"
#include "mat\spl_fac.h"
#include "fre_bus.h"

//#include "h2_liqui.h"
//#include "matt_adi.h"


struct MatterSpl:MatterIO
  {
   Stroka spl_name;
   //static const char * const name;
   //virtual const char *InterfaceIOName()  {return name;}
   double DencCold,DencMax;
   Ref<SplineXYIO> SPre,SThe;
   double AddE,AddP;
//   SplineXYIO *STre;

   MatterSpl():spl_name("h2e_ros1.spl"){}
   double DencityCold() {return DencCold;}
   double DencityMax()  {return DencMax;}
   //void DeleteVar(){ delete SPre;SPre=NULL;delete spl_name;spl_name=NULL;
   //                  delete SThe;SThe=NULL;}
   //~MatterSpl() {DeleteVar();}

   double Pressure(double Dencity,double Energy)
     { double res,E=-6,D=-9;
       if (Energy+AddE>StndErr) E=log(Energy+AddE);
       if (Dencity>StndErr) D=log(Dencity);
       int er=SPre->Evaluate(&E,&D,&res,1);
       if (er!=0) {res=1;cout<<" Very Bad!!\n";}
       else res=exp(res)-AddP;
//cout<<" D "<<Dencity<<" E "<<Energy<<" LogE "<<E<<" P "<<res<<"\n";
       return res;}
   double Sound(double Dencity,double Energy)
     {
//cout<<" MatterIO::Sound "<<" Dencity "<<Dencity<<" Energy  "<<Energy<<"\n";
      double dr=50*Dencity*StndErr,r=Dencity;
      if (r<MathZer) { 
cout<<"MatterIO::Sound; Dencity<MathZer :"<<Dencity<<"\n";
                      return 0.1;}
      double P=Pressure(r,Energy);
      double Pp=Pressure(r+dr,Energy+P*dr/(r*r));Pp-=P;
//double Pp1=Pressure(r+dr,Energy);Pp1-=P;
//cout<<" Pp "<<Pp<<" Pp1 "<<Pp1<<" S "<<sqrt( max(0,Pp)/dr )<<" S1 "<<sqrt( max(0,Pp1)/dr )<<"\n";
      if (Pp<0)
        {
//cout<<" MatterIO::Sound;  deriv is negative: " <<Pp<<" Sqrt from negative value          \n";
         Pp=0.1;
        }
      double S=sqrt( max(0,Pp)/dr );
      return S;
     };
      
   double Temperature(double Dencity,double Energy)
     { double res;
       if (SThe!=NULL)
         {
          double E=-6,D=-9;
          if (Energy+AddE>StndErr) E=log(Energy+AddE);
          if (Dencity>StndErr) D=log(Dencity);
          int er=SThe->Evaluate(&E,&D,&res,1);
          if (er!=0) {res=1;cout<<" Very Bad Temp!!\n";}
          else res=exp(res);
//cout<<" D "<<Dencity<<" E "<<Energy<<" T "<<res<<"\n";
         } 
       else res=0;
       return res;}
//   void Sound(double *S,double *Dencity,double *Energy,int Num);
   double St_Denc,St_Temp;
   static MatterSpl* Current;
   static double MisE(double E)
     { double res;
       if (Current->SThe!=NULL)
         {
          double Ec=E,D=Current->St_Denc;
          int er=Current->SThe->Evaluate(&Ec,&D,&res,1);
          if (er!=0) {res=0;cout<<" Very Bad Temp!!\n";}
          else res=log(res/Current->St_Temp);
          //else res-=St_Temp;
//          else res=St_Temp-exp(res);
         } 
       else res=0;
       return res;
      }
   double Energy(double Dencity,double Temperature)
     {
      St_Denc=log(Dencity);St_Temp=log(Temperature);
      double E0=log(150+AddE);
      MatterSpl *Old = Current;
      Current=this;
      //if (!ZeroNewtonIt(MisE,E0,StndErr,1000,100,-100,0.1*E0,0.5))
      //  {cout<<" Bad ZeroNewtonIt    in MatterSpl::Energy Not Zero\nfound E "
      //       <<E0<<" Mis "<<MisE(E0)<<"\n";}
      if (!Fzero(MisE,-100, 100,E0,M_Eps2*10,M_Eps2*10,1000))
        {cout<<" Bad Fzero    in MatterSpl::Energy Not Zero\nfound E "
             <<E0<<" Mis "<<MisE(E0)<<"\n";}
      Current = Old;
      return exp(E0)-AddE;
    }	                                         

   void Temperature(double *T,double *Dencity,double *Energy,int Num);
   void Pressure(double *P,double *Dencity,double *Energy,int Num);
   int read_data_state(FilterIn &in)
     {
	  in>>spl_name;
      ReadSplFile();
      return 1;
     };
   void ReadSplFile() {
	  FilterTextIn sp(spl_name.c_str());
      if ( (!sp) || (seek_eof(sp)) )
          throw info_except(" There is no such MatterSpline file named %s\n", spl_name.c_str());
      char tmp[256];
      sp>>tmp>>AddE>>tmp>>AddP>>tmp>>DencCold>>tmp>>DencMax;
	  ReadSpline(SPre, "PresSpline", sp);
	  if (seek_eof(sp))
		  SThe = NULL;
	  else 
		  ReadSpline(SThe, "TempSpline", sp);

   }
   void SaveSplFile() {
	  FilterTextOut sp(spl_name.c_str());
      if (!sp)
          throw info_except(" There is no such MatterSpline file named %s\n", spl_name.c_str());
      sp<<" AddE "<<AddE<<" AddP "<<AddP<<" DencCold "<<DencCold<<" DencMax "<<DencMax<<"\n";
	  SaveSpline(SPre, sp);
      if (SThe != NULL) {
          sp<<"\n\n";
	      SaveSpline(SThe, sp);
      }
   }
// Old spline format compatibility....
   static void ReadSpline(Ref<SplineXYIO> &Spl, const char *SplType, FilterIn &in){
      Stroka SplObj;
	  in>>SplObj;
	  ClassDesc *cd = ClassDesc::name_to_class_desc(SplObj.c_str());
      if (cd) 
        Spl << cd->create();
      else 
		  throw info_except(" Class %s unknown \n", SplObj.c_str() );
      if (!Spl) 
		  throw info_except(" Read class <%s> for %s spline is not of SplineXYIO typeClass or CreateMethod is not registered.\n",SplObj.c_str(), SplType );
      if (!Spl->read_data_state(in)) 
        throw info_except("Error while reading by state savable class functions.\n "); 

   }
   static void SaveSpline(Ref<SplineXYIO> &Spl, ostream &out){
		if  (!out) 
			throw info_except(" bad out stream\n ");
		FilterTextOut output(out.rdbuf(),0);output.SetRefMask(SavableClass::SimpleEdit);//SingleFileStorage);
		output<<Spl->class_name()<<" ";
		Spl->save_data_state(output);
		if  (!out) 
			throw info_except(" after readind spline - bad out stream\n ");
   }
   int save_data_state(FilterOut &out){ 
	   out<<"  "<<spl_name<<" ";
	   return 1;
   };

    virtual Stroka MakeHelp(){
        Stroka res = MatterIO::MakeHelp();
		res += Stroka("Depricated format. Use TMatterSpl instead!!!!\nspline file - is a binary file... Help for it is not available....\n");
        return res;
	}
  };

int GenerateMatterSpl(istream &row,MatterIO *Mat);

//int GenerateSumRegionDat(istream &row,MatterIO *Mat);

// Exported for spl_2phase
namespace Matt_spl {
	void ReadData(char *name,TData<real> *&dat);
	void SetIntPar(double &IntPar,MatterIO *Mat,double Denc,double Energy);
	void SetIntParInit(double &IntPar,MatterIO *Mat);
	fstream& StdOut();
}



#endif

