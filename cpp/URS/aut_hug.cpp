#include <lib/precompiled/adi_dou.h>

#include "aut_hug.h"

int OnlyHugoniotCalculation=0;

int GetInitialPressure=0;

double Norm(Int_Par &Par)
{return fabs(Par.Dens)+fabs(Par.Ener)+fabs(Par.Pres)+fabs(Par.Velo);};

Int_Par operator*(Int_Par &Par,double coef)
{
	Int_Par ret=Par;
	if (coef!=0)
	{ret.Dens*=coef;ret.Ener*=coef;ret.Pres*=coef;ret.Velo*=coef;}
	else 
	{ret.Dens=0;ret.Ener=0;ret.Pres=0;ret.Velo=0;}
	return ret;
};
//Int_Par operator/(Int_Par &Par,double coef);

Int_Par operator+(Int_Par &Par,double coef)
{	Int_Par ret=Par;ret.Velo+=coef;return ret;};

Int_Par operator+(Int_Par &Par1,Int_Par &Par2)
{
	Int_Par ret=Par1;
	ret.Dens+=Par2.Dens;ret.Ener+=Par2.Ener;
	ret.Pres+=Par2.Pres;ret.Velo+=Par2.Velo;
	return ret;
};

Int_Par operator-(Int_Par &Par1,Int_Par &Par2)
{
	Int_Par ret=Par1;
	ret.Dens-=Par2.Dens;ret.Ener-=Par2.Ener;
	ret.Pres-=Par2.Pres;ret.Velo-=Par2.Velo;
	return ret;
};



VecCl Par2Vec(Int_Par &Par)
  {
	VecCl ret(3);
	ret[1]=Par.Dens;ret[2]=Par.Ener;
	ret[3]=Par.Dens*Par.Velo;
	return ret;
  };
Int_Par Vec2Par(VecCl &Par)
  {
	Int_Par ret;
	ret.Dens=Par[1];ret.Ener=Par[2];
	ret.Velo=Par[3]/Par[1];ret.Pres=0;
	return ret;
  };



//===============================================================
//===============================================================
//===============================================================

Int_Par S_Par1,S_Par2,S_Res1,S_Res2;
double S_ResD1,S_ResD2;
MatterIO *S_Mat1,*S_Mat2;

double MisU(double P)
    {
    MatterAdiabat=S_Mat1;
    if ((S_Par1.Pres<=P) || (OnlyHugoniotCalculation))
     HugPres(S_Par1.Velo,S_Par1.Dens,S_Par1.Pres,S_Par1.Ener,
             S_Res1.Velo,S_Res1.Dens,P,S_Res1.Ener,S_ResD1);
    else
      IsentropePres(S_Par1.Velo,S_Par1.Dens,S_Par1.Pres,S_Par1.Ener,
             S_Res1.Velo,S_Res1.Dens,P,S_Res1.Ener,S_ResD1);
     MatterAdiabat=S_Mat2;
    if ((S_Par2.Pres<=P) || (OnlyHugoniotCalculation))
     HugPres(S_Par2.Velo,S_Par2.Dens,S_Par2.Pres,S_Par2.Ener,
             S_Res2.Velo,S_Res2.Dens,P,S_Res2.Ener,S_ResD2);
    else
     IsentropePres(S_Par2.Velo,S_Par2.Dens,S_Par2.Pres,S_Par2.Ener,
             S_Res2.Velo,S_Res2.Dens,P,S_Res2.Ener,S_ResD2);
	 double ret=S_Res2.Velo+S_Res1.Velo-2*S_Par2.Velo;
cout<<" MisU "<<ret<<" P "<<P<<"\n";cout.flush();
     return ret;
    }

void Hug2(Int_Par &P1,Int_Par &P2,Int_Par &Pr1,double &Dr1,Int_Par &Pr2,
          double &Dr2,MatterIO *Mat1,MatterIO *Mat2)
  {
   double Pest;//,Delta;
   MatterAdiabat=Mat1;P1.Pres=PressurePorous(P1.Dens,P1.Ener);
   MatterAdiabat=Mat2;
   if (!GetInitialPressure) P2.Pres=PressurePorous(P2.Dens,P2.Ener);
   else P2.Pres=0.0001;
   if (Norm(P1-P2)<StndErr) 
   {Pr1=P1;Pr2=P2;Dr1=Mat1->Sound(P1.Dens,P1.Ener)+P1.Velo;Dr2=Dr1;return ;}
   double A1=max<double>(0.1,P1.Dens*Mat1->Sound(P1.Dens,P1.Ener));
   double A2=max<double>(0.1,P2.Dens*Mat2->Sound(P2.Dens,P2.Ener));
   Pest=(P2.Pres*A1+P1.Pres*A2+A1*A2*(P1.Velo-P2.Velo))/(A1+A2);

   S_Par1=P1;S_Par2=P2;S_Mat1=Mat1;S_Mat2=Mat2;
#ifdef FZERO_HUGONIO_SEARCH
//int Fzero(X_func f,double From,double To,double &Guess,double ErrorAbs,double ErrorRel,
//                  int MaxIter);
    Fzero(MisU,1e6,M_Eps2,Pest,StndErr,StndErr,200);
//   if (3<Fzero(MisU,-1e6,1e6,Pest,StndErr,StndErr,200));
    //{ cout <<" Error, could not find Hug2 ZeroFzeroIt\n";}
//int ZeroNewtonIt(X_func Func,double &CurPnt,double Tol,int MaxIter,
//                             double Up,double Down,double MaxStp,double BegStp) 
#else
#ifdef NEWTON_HUGONIO_SEARCH
   if (!ZeroNewtonIt(MisU,Pest,StndErr,100,1e6,-1e6,5*fabs(Pest)))
   { double Ctrl=MisU(Pest);cout <<" Error, could not find Hug2";}
#else
   if (!FindZeroPos(MisU,Pest,Delta,StndErr,0.5*Pest))
     cout <<" Error, could not find Hug2";
#endif
#endif
   Pr1=S_Res1;Pr2=S_Res2;Dr1=-S_ResD1+S_Par1.Velo;Dr2=-S_ResD2+S_Par2.Velo;
   Pr2.Velo=-Pr2.Velo+2*P2.Velo;
   Pr1.Pres=Pest;Pr2.Pres=Pest;
  }; 

void Hug1(Int_Par &P1,Int_Par &Pr1,double &Dr1,
		  MatterIO *Mat1,double Pres)
  {
   MatterAdiabat=Mat1;
   Pr1.Pres=Pres;
   HugPres(P1.Velo,P1.Dens,P1.Pres,P1.Ener,
             Pr1.Velo,Pr1.Dens,Pres,Pr1.Ener,Dr1);
   Dr1=-Dr1+P1.Velo;
  }; 


