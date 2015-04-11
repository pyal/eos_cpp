//#ifndef __MATT_REG_H
//#define __MATT_REG_H
//
//
//
//#include "matt_cfg.h"
//
//#ifdef Matter_Register
//#include "matt_fac.h"
//
//#ifdef  MatterH2Liq_Register
//#include "matt_h2.h"
//#endif
//
//#ifdef  MatterSpline_Register
//#include "matt_spl.h"
//#endif
//
//#ifdef  MatterABuExp_Register
//#include "matt_exp.h"
//#endif
//
//#ifdef  MatterHisch_Register
//#include "matt_his.h"
//#endif
//
//#endif // Matter_Register
//
//
//#ifdef FreeE_Register
//
//#include "fre_fac.h"
//
//#ifdef  FreeE_RossRegister
//#include "fre_ros.h"
//#endif   
//#ifdef  MatterUrl_Register
//#include "fre_url.h"
//#endif
//#ifdef  FreeE_BushmanRegister
//#include "fre_bus.h"
//#endif
//#endif   // FreeE_Register
//
//#endif
//



//  Definitions for registering of various functions
//#define  Matter_Register
//#define  FreeE_Register
//#define  InternalPointsUrs  // -> in define "matt_fac.h"

/*
#define  MatterABu_Register
#define  MatterH2Liq_Register
#define  MatterSpline_Register

#ifdef InternalPointsUrs
#define MatterSum_Register
#endif


#include "matt_h2.h"
#include "matt_spl.h"

int MatterRegister(fstream &in,MatterIO *&Func);
int MatterStore(fstream &out,MatterIO *Func);

#endif   // Matter_Register




#ifdef FreeE_Register
//#define  FreeEVanDerVals_Register
//#define  FreeEH2Liquid_Register
//#define  FreeEH2Solid_Register
#include "fre_fac.h"

#define  MatterFreeE_Register

int FreeERegister(fstream &in,InterfaceFreeEIO *&Func);
int FreeEStore(fstream &out,InterfaceFreeEIO *&Func);

  
#endif   // FreeE_Register

#endif
*/
