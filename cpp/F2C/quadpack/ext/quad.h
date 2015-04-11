#ifndef QUAD_H
#define QUAD_H


#include <stdlib.h>
#include <float.h>
#include <ostream.h>
#include <math.h>
#include "f2c.h"

#ifdef __cplusplus
extern "C" {
#endif

doublereal _r1mach_(integer *i);

int	_xerror_(char *msg, integer *len, integer *ier,integer *lvl, ftnlen len1);

#ifdef __cplusplus
	}
#endif


typedef double (*X_func)     (double par);


double Qag_OscilInt(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err);

double Qage_OscilInt(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err);

double Qagi_InfinInt(X_func f,double From,int Inf,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err);

double Qagie_InfinInt(X_func f,double From,int Inf,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err);

double Qags_IntSubInt(X_func f,double From,double To,double ErrorAbs,double ErrorRel,
                  int NumSubInt,double &ResErr,int &Err);

int Fzero(X_func f,double From,double To,double &Guess,double ErrorAbs,double ErrorRel,
                  int MaxIter);

double Fmin(X_func f,double From,double To,double Guess,double AbsErr,double &FuncXVal);


extern int NumberEvaluations;

#endif
