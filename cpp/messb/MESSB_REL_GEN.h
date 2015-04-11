#ifndef __MESSB_REL_GEN_H
#define __MESSB_REL_GEN_H

#include "mat\mtrnd.h"
#include "mat\matrics.h"





// Model H=Asin(Teta)^2
// Probability  of state exp(-H/T)=exp(-AnisT*sin(Teta)^2-AnisP*sin(Phi)^2)

// AnisotropeTeta        AnisTeta
// AnisotropePhi         AnisPhi
// NoRelaxPhi            NoPhi

// TimeRelaxTetaValue    TimeTeta 
// TimeRelaxTetaDispers  SigTimeTeta
// AngleRelaxTetaValue   AngTeta
// AngleRelaxTetaDispers SigAngTeta

// TimeRelaxPhiValue     TimePhi 
// TimeRelaxPhiDispers   SigTimePhi
// AngleRelaxPhiValue    AngPhi
// AngleRelaxPhiDispers  SigAngPhi

// Model of meanSpeed relaxation in 2D

void MC_Anis2DAnglesWalk(VecCl &phi,VecCl &teta,VecCl &time,double TimeClc,
					 double TimeTeta,double SigTimeTeta,double AngTeta,double SigAngTeta,double AnisTeta,
					 double TimePhi,double SigTimePhi,double AngPhi,double SigAngPhi,double AnisPhi,
					 int NoPhi,double AnisPower);

// Model H=Asin(Teta)^2
// Probability  of state exp(-H/T)=exp(-AnisT*sin(Teta)^2-AnisP*sin(Phi)^2)

// AnisotropeTeta        AnisTeta
// AnisotropePhi         AnisPhi
// NoRelaxPhi            NoPhi

// TimeRelaxTetaValue    TimeTeta 
// TimeRelaxTetaDispers  SigTimeTeta

// TimeRelaxPhiValue     TimePhi 
// TimeRelaxPhiDispers   SigTimePhi

// Model of Jump relaxation in 2D ( no speed Limit )
void MC_AnisJump(VecCl &phi,VecCl &teta,VecCl &time,double TimeClc,
					 double TimeTeta,double SigTimeTeta,double AnisTeta,
					 double TimePhi,double SigTimePhi,double AnisPhi,
					 int NoPhi,double AnisPower);


// Model of haotic jumps between different axis directions
// axProb - Blumes treatment - prob[k] prob to leave [k] axis

void MC_AxisJump(VecCl &resPhi,VecCl &resTeta,VecCl &resT,VecCl &axPhi,VecCl &axTeta,
				 VecCl &axProb,double fullTime,double timeDisp);


// Sum of Models:	 MC_AxisJump + MC_AnisJump
void MC_Anis2DAnglesJump(VecCl &resPhi,VecCl &resTeta,VecCl &resTime,double TimeClc,
					 double TimeTeta,double SigTimeTeta,double AnisTeta,
					 double TimePhi,double SigTimePhi,double AnisPhi,
					 int NoPhi,double AnisPower,
					 VecCl &axPhi,VecCl &axTeta,VecCl &axProb,double axTimeDisp);




#endif



/*
void MC_AxisStep(VecCl &ph,VecCl &time,double Phi0,int N,double MainHarm,
                 double FurieDiscr,double FullTime,double StrongShort,double p1,double p2,
                 double AnisotrpCoef);

void MC_AxisStepShorten(double SameAng,VecCl &time,VecCl &phi);
//void SetGausDistrib();

void MC_BlumeTest()


*/