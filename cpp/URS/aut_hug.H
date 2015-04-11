#ifndef __AUT_HUG_H
#define __AUT_HUG_H

#include "matt_reg.h"
#include "mat\zero.h"
#include "matt_adi.h"
#include "mat\matrics.h"



struct Int_Par  
	{ double Pres,Dens,Ener,Velo;
      friend Int_Par operator*(Int_Par &Par,double coef);
//      friend Int_Par operator/(Int_Par &Par,double coef);
      friend Int_Par operator+(Int_Par &Par,double coef);
      friend Int_Par operator+(Int_Par &Par1,Int_Par &Par2); 
      friend Int_Par operator-(Int_Par &Par1,Int_Par &Par2); 
      friend double Norm(Int_Par &Par); 
	}; 

VecCl Par2Vec(Int_Par &Par);
Int_Par Vec2Par(VecCl &Par);

void Hug2(Int_Par &P1,Int_Par &P2,Int_Par &Pr1,double &Dr1,Int_Par &Pr2,
          double &Dr2,MatterIO *Mat1,MatterIO *Mat2);

void Hug1(Int_Par &P1,Int_Par &Pr1,double &Dr1,
		  MatterIO *Mat1,double Pres);

extern int OnlyHugoniotCalculation;

#endif
