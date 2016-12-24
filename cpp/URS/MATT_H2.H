#ifndef __MATT_H2_H
#define __MATT_H2_H

#include "h2_liqui.h"
#include "matt_adi.h"

struct MatterH2:MatterIOFind
  {
//   static const char * const name;
//   virtual const char *InterfaceIOName() const {return name;}
   MatterH2 ();// { SetConstants(); };
   double DencityCold()
     {return ColdParam.Denc;}
   double DencityMax()
     {return 5*ColdParam.Denc;}

   void ReadSubstancePar(istream &in){H2ReadPar(in);};
   void SaveSubstancePar(ostream &out){H2SavePar(out);};

  };

struct MatterH2Liq:MatterH2
  {

   double Energy(double Dencity,double Temperature)
     {return Eliquid(Dencity/DencityCold(),Temperature);}
    
   double Pressure(double Dencity,double Energy)
     { return max<double>(1e-4,PressureL(Dencity,Energy));}
   double Temperature(double Dencity,double Energy)
     { return TemperatureL(Dencity,Energy); }
   double Sound(double Dencity,double Energy)
    {
     double dr=sqrt(StndErr),r=Dencity;
     if (r<MathZer) { cout<<"MatterIO::Sound; Dencity<MathZer :"
                          <<Dencity<<"\n";return 0;}
     double P=PressureL(r,Energy),
            Pp=PressureL(r+dr,Energy+P*dr/(r*r))-P;
     if (Pp<0) 
       {
   //       cout<<" MatterIO::Sound;  deriv is negative: " <<Pp<<" Sqrt from negative value          \n";
       }
     return sqrt( max<double>(0,Pp)/dr );
//     return S;
    };
  };
struct MatterH2Sol:MatterH2
  {

   double Pressure(double Dencity,double Energy)
     { 
       return max<double>(1e-4,PressureS(Dencity,Energy));
//       return max<double>(1e-4,PNull(Dencity/ColdParam.Denc));
     }
// do not work
   double Temperature(double Dencity,double Energy)
     { return TemperatureS(Dencity,Energy); }

   double Energy(double Dencity,double Temperature)
     {return Esolid(Dencity/DencityCold(),Temperature);}
  };



#endif
