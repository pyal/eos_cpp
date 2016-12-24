#ifndef __HISCH_H
#define __HISCH_H

#include "matt_cfg.h"
#include "lib/std/deffunc.h"
//#include "lib/std/interfac.h"
//#include <alloc.h>

struct HischConst{  
	HischConst(){};
	  HischConst(double b0c,double v0c,double m,double n,double ed,double gi,double v0,double g0c,double sn,double sm,double ea)
		  :B0c(b0c),V0c(v0c),M(m),N(n),Ed(ed),Gi(gi),V0(v0),G0c(g0c),Sn(sn),Sm(sm),Ea(ea){}
	  double B0c,V0c,M,N,Ed,Gi,V0,G0c,Sn,Sm,Ea; 
  };

double Pres(double Dencity,double Energy,HischConst &Hisch);
void Pres(double *Dencity,double *Energy,double *Pressure,HischConst &Hisch,int Num);

double Grun(double Sigma,double Energy,HischConst &Hisch);

struct IvlConst{  
	IvlConst(){};
	IvlConst(double dx, double cmn, double m, double n, double ed, double gi, double v0,
		double gc, double ggi, double gf, double qs, double sm, double ea, double e0)
		:DX(dx),CMN(cmn),M(m),N(n),Ed(ed),Gi(gi),V0(v0),GC(gc),GGI(ggi),GF(gf),QS(qs),Sm(sm),Ea(ea),E0(e0){};
	double DX,CMN,M,N,Ed,Gi,V0,GC,GGI,GF,QS,Sm,Ea,E0; 
};
double Pres(double Dencity,double Energy,IvlConst &Ivl);
void Pres(double *Dencity,double *Energy,double *Pressure,IvlConst &Ivl,int Num);



#endif
