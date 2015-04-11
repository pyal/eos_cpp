#ifndef __ANALGLB_H
#define __ANALGLB_H

#include "anal.h"
#include <string.h>

#include <math.h>
//#include "mem.h"
//#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
//#include <ctype.h>


#define MAXFILEWORK 5
#define MAXROWWORK 20


struct WorkDo
  {
     int Type,p1,p2,p3,p4; 
     double d1,d2;
     int *arf,*arn;
     double *sum;
  };
int FormWork(char *s,WorkDo *wdo);
int DoMes(WorkDo *wdo,double (&ar)[MAXFILEWORK][MAXROWWORK],int col);



#endif
