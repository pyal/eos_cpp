#ifndef __FARegist_H
#define __FARegist_H

#include "fastd.h"
#include "faobj.h"

//#define __FAStateRegist_H
#define __FAMessbRegist_H
//#define __TMSTD_H

#ifdef __FAMessbRegist_H
#include "famessb.h"
#endif
#ifdef __FAStateRegist_H
#include "fastate.h"
#endif

int FARegister(istream &in,FAIO *&Func);

#endif 
