#ifndef __ADI_DOU_H
#define __ADI_DOU_H

#include "urs_forcelnk.h"
//#include "matt_reg.h"
//#include "zero.h"
//#include "matt_adi.h"
#include "aut_hug.h"

struct Sl_Par
    {
     Int_Par P;
     MatterIO *M;
     double L,D,T;
     friend ostream& operator<<(ostream &output,Sl_Par &Par);
     friend istream& operator>>(istream &input,Sl_Par &Par);

    };

#endif
