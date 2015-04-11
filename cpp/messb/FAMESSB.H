
#ifndef __FAMessb_H
#define __FAMessb_H

#include "messb.h"
#include "faobj.h"
//#define __FAMessbRegist_H
#include "faregist.h"
#include "mat\quad.h"
#include "mat\mtfunc.h"

int FAMessbRegister(fstream &in,FAIO *&Func,char *name);
extern int FARegister(istream &in,FAIO *&Func);

struct StandardParams
{
  static int H_Qs_eiler_short(char **namepar,double *valpar,char **helppar,int StartNum);
  static int H_Qs_eiler(char **namepar,double *valpar,char **helppar,int StartNum);
//  static void H_Qs_free(char &*namepar,double *valpar,char &*helppar,int StartNum);
  static int H_Qs_gamma(char **namepar,double *valpar,char **helppar,int StartNum);
};

//  =======================================================
//  ================ Polycristall case  to be deleted =====
//  =======================================================
struct FAZerHQs:FAZerBase
  {
   FAZerHQs();
   void   ClcBase(double *par,double *Xval,double *Res);
  };

//  =======================================================
//  ================ Monocristall case  ===================
//  =======================================================
struct FAZerHQsM:FAZerBase
  {
   FAZerHQsM();
   void   ClcBase(double *par,double *Xval,double *Res);
  };

//  ================================================
//  ================ Relax 2 case  =================
//  ================  Free case     ================
//  ================================================

struct FAZerFreeR:FAZerBase
  {
   FAZerFreeR();
   void   ClcBase(double *par,double *Xval,double *Res);
  };


//  ================================================
//  ================ Average HQs  case    ==========
//  ================================================
//  Monocristal in the external field rotated base
struct FAZerHQsExternalField:FAZerBase
  {
   FAZerHQsExternalField();
   void   ClcBase(double *par,double *Xval,double *Res);
  };


//  ================================================
//  ======== Relaxation n _H angle Function  =======
//  ================================================

struct FAZerQsH_R_Hn:FAZerBase
  {
   FAZerQsH_R_Hn();
   void   ClcBase(double *par,double *Xval,double *Res);
  };

//  ================================================
//  ================ Relaxation Random Walk  =======
//  ================================================
struct FAZerQsHRelaxRandWalk:FAZerBase
  {
   FAZerQsHRelaxRandWalk();
   void   ClcBase(double *par,double *Xval,double *Res);
  };
//  ================================================
//  ================ Relaxation Random Jump  =======
//  ================================================
struct FAZerQsHRelaxRandJump:FAZerBase
  {
   FAZerQsHRelaxRandJump();
   void   ClcBase(double *par,double *Xval,double *Res);
  };

//  ================================================
//  ================ Relaxation Mean Field   =======
//  ================================================
struct FAZerQsHRelaxMeanField:FAZerBase
  {
   FAZerQsHRelaxMeanField();
   void   ClcBase(double *par,double *Xval,double *Res);
  };

//  =======================================================
//  ================    Magnon Mean Field  Std      =======
//  =======================================================
struct FAZerHQsMagnonMeanFieldStd:FAZerBase
  {
   FAZerHQsMagnonMeanFieldStd();
   void   ClcBase(double *par,double *Xval,double *Res);
  };
//  =======================================================
//  ================    Magnon Mean Field  Optic      =======
//  =======================================================
struct FAZerHQsMagnonMeanFieldOptic:FAZerBase
  {
   FAZerHQsMagnonMeanFieldOptic();
   void   ClcBase(double *par,double *Xval,double *Res);
  };
//  =======================================================
//  ================    Magnon Mean Field  Accust       ===
//  =======================================================
struct FAZerHQsMagnonMeanFieldAccus:FAZerBase
  {
   FAZerHQsMagnonMeanFieldAccus();
   void   ClcBase(double *par,double *Xval,double *Res);
  };
//  =======================================================
//  ================    Magnon Mean Field  Paper        ===
//  =======================================================
struct FAZerHQsMagnonMeanFieldPaper:FAZerBase
  {
   FAZerHQsMagnonMeanFieldPaper();
   void   ClcBase(double *par,double *Xval,double *Res);
  };
//  =======================================================
//  ================    Magnon Mean Field  PaperPlus    ===
//  =======================================================
struct FAZerHQsMagnonMeanFieldPaperPlus:FAZerBase
  {
   FAZerHQsMagnonMeanFieldPaperPlus();
   void   ClcBase(double *par,double *Xval,double *Res);
  };
//  =======================================================
//  ================    Magnon Mean Field  Paper9       ===
//  =======================================================
struct FAZerHQsMagnonMeanFieldPaper9:FAZerBase
  {
   FAZerHQsMagnonMeanFieldPaper9();
   void   ClcBase(double *par,double *Xval,double *Res);
  };
//  ================================================
//  ================ QS(t) approcsimaton  ==========
//  ===== sum of two probable values for QS ========
//  ======       T=0 Qs=max;  T=inf QS=0;   ========
//  ================================================
struct FAZerQs_T:FAZer
  {
   FAZerQs_T();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
  };

//  ================================================
//  ================ Int(t) in the  ================
//  ==============  deby approcsimaton  ============
//  ================================================
struct FAZerInt_T:FAZer
  {
   FAZerInt_T();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
  };

//  ================================================
//  ================ Is(t) in the  ================
//  ==============  deby approcsimaton  ============
//  ================================================
struct FAZerIs_T:FAZer
  {
   FAZerIs_T();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
  };

/*
//      Do not work for Phi
//  ================================================
//  ================ QH poly     ===================
//  ================================================
struct FAZerQsHR0:FAZer
  {
   FAZerQsHR0();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
//   ~FAZerExp();
  };
//  ================================================
//  ================ No Relax case  ================
//  ================ calculated as relax ===========
//  ================================================
struct FAZerHQsR0:FAZer
  {
   FAZerHQsR0();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
//   ~FAZerExp();
  };

*/

#endif 




/*
//  ================================================
//  ================ Relax 2 case  =================
//  ================================================
struct FAZerHQsR:FAZer
  {
   FAZerHQsR();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
//   ~FAZerExp();
  };

//  ================================================
//  ================ Relax 2 case  =================
//  ================ Field no field ================
//  ================================================
struct FAZerHQs0QsR:FAZer
  {
   FAZerHQs0QsR();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
//   ~FAZerExp();
  };


//  ================================================
//  ================ Relax 3 case  =================
//  ================================================
struct FAZerHQsR3:FAZer
  {
   FAZerHQsR3();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
//   ~FAZerExp();
  };
//  ================================================
//  ================ Relax 6 case  =================
//  ================================================
struct FAZerQsHR6:FAZer
  {
   FAZerQsHR6();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
//   ~FAZerExp();
  };
/ *
//  ================================================
//  ================ Relax circular case  ==========
//  ================================================
struct FAZerHQsRC:FAZer
  {
   FAZerHQsRC();
   double Clc(double *par,double Xval);
   void   Clc(double *par,double *Xval,double *Res);
   void ParStp(double *par,double *stp);
//   ~FAZerExp();
  };
* /
*/

