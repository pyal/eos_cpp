
#pragma once

#include "matt_2phase.h"
#include "fre_fac.h"

int GenerateTwoPhaseBnd(istream &row,MatterIO *Mat);
void FindBushCoefABC(istream &row,MatterIO  *Mat_In,
                     double dPdT,double SeekFrom,double SeekTo,double StpShow);

int GenerateSolLiqPhaseBnd(istream &row,MatterIO *MatSol);

