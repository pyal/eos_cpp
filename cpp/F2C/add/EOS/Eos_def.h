#ifndef __EOS_DEF_H
#define __EOS_DEF_H


#define MathInf 1e35
#define MathZer 1e-35
extern double StndErr;


typedef double (*PointFunc)(double x, double y, void *IntPar);
typedef int (*ReadFunc)(FILE *hand, void **IntPar);
typedef void (*VoidFunc)(void **IntPar);


#endif