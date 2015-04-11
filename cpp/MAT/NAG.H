#ifndef __NAG_H
#define __NAG_H 

#include "util.h"
#ifdef __cplusplus
extern "C" {
#endif

// Integration 1D over a fixed number of points
double d01gae_(double *x, double *y, int n, double &er,int &ifail);

#ifdef __cplusplus
	}
#endif

int ErrNag(int &Err,int Flag,char *Name);

#endif
