#ifndef __CONST_H
#define __CONST_H

#define MathZer 1e-35
#define MathInf 1e35
#define MIN_EPS 1e-12
#define MIN_NUMBER 1e-300
//#define StndErr 1e-6
extern double StndErr;

#define FM  "  "<<setw(13)<<setprecision(6)
#define FMT "  "<<setw(13)<<setprecision(6)

//#define WCPP_NET
//#define WCPP
//#define WINDOWS_HEAD
//#define MSDOS
//#define LINUX
//#define DEBUG_MODE
//#define MAC

#include "F2C/libf2c/math.h"
#ifndef MAC
#define M_PI        3.14159265358979323846
#define M_PI_2      1.57079632679489661923
#define M_PI_4      0.785398163397448309616
#define M_SQRTPI   1.772453850905515881919
#define M_SQRT2     1.414213562373095145474
#define M_SQRT1_2     0.707106781186547524401
#define M_E         2.71828182845904523536
#endif

#define M_SQRT2PI   2.506628274631000241612
#define M_1_SQRTPI  0.56418958354775627928
#define MAXPATHLEN  1024

#define M_Eps  2.2204460492503131e-016
#define M_Eps2 1.49011611938477e-08
#define M_MinDouble 2.2250738585072014e-308
#define M_MinDouble2 1.49166814624004e-154






//  ====================    Physical constants - old one  =====================

//#define M_Na        6.02214199e23
//#define M_Bor       0.5291772083              // in Angstrem
//#define M_Rconst    8.314472e-3               // kj/mol
//#define M_Rydberg_K 1.578885e5
//#define M_C         2.99792458e10              // cm/c
//
//#define M_eV_K      1.1604505949e4
//#define M_eV_kJ     1.60217653e-22      //Mul on Na...
//#define M_PlankCros_K      7.628382e-12  // This an old var with error...
//#define M_NuclearWeight_K	1.080961e13
//#define M_Mass_Electron_kg	9.1093897e-31
//#define M_Mass_Electron_eV  5.10998902e5
//#define M_eV_Joule	1.602176462e-19
//// Joule = kg*(m/c)^2
//#define M_h_Plank	6.62606876e-34	// joule*cek
//
//#define M_PartDistInBor_DencIn_g_cm3 2.696  // Denc=M_../r_s^3
//
//
//#define M_ElectronCharge 1.602176462e-19 // kulon
//#define M_Epsilon0       8.854187817e-12   // Kulon^2/(H*m^2)  F=1/(4*Pi*Epsilon0)q^2/r^2
//#define M_ElectrInter_Div_MassElectron  2.81794094e-13 // e^2/r/mc^2 ElectronDistance - 1 cm
//



//  ====================    Physical constants - new one  =====================
//#define M_PlankCros_K      7.628382e-12  // This an old var with error...
#define M_PlankCros_K        7.63822378788303945929e-12  // New one... Units - [Kelvin*Sec] - changed!!!
#define M_eV_kJ     1.60217733e-22      //Mul on Na...  //same  
#define M_eV_Joule	1.60217733e-19         //same
#define M_eV_K      11604.512236009364642  //same
// Joule = kg*(m/c)^2
#define M_h_Plank	6.62606876e-34	// joule*cek   //same
#define M_C         2.99792458e10              // cm/c  same

#define M_Mass_Electron_kg	9.10938188e-31         //same
#define M_Mass_Electron_eV   510998.6258350895368494     // same
#define M_ProtonMass_kg  1.67262158e-27   
#define M_ProtonMass_eV  9.3827148777e8           
#define M_ProtonMass_K   1.088818296e13         //same
//#define M_NuclearWeight_K	1.080961e13         - depricated - bad one



#define M_Na        6.02214199e23             //same
#define M_Rconst    8.314472e-3               // kj/(mol K)  same
//#define M_Rydberg_K 1.578885e5                // ? do not know
#define M_Rydberg   1.0973731568549e7         //1/meter

#define M_ElectronCharge    1.602176462e-19        // kulon   same F=1/(4PiEps0)*e^2/r^2
#define M_ElectricEpsilon0  8.854187817e-12              // farad/meter
#define M_ElectronCGSCharge 4.806529386e-10        // cgs charge F=e^2/r^2 Dyne = (cgs_charge/centimeter)^2
#define M_CGSCharge         1e-5                    // cgs_charge in (kJoule*Centimeter)^0.5


#define M_Bor       0.5291772083              // in Angstrem = 10^-10 m = 4Pie0h_p^2/me/e^2
#define M_Rau_cm    0.5291772083e-10          // m
#define M_Eau_J     4.35974394e-18            //Joule
#define M_Eau_eV    27.21136954297            //eV
#define M_Pau_GPa   2.9421010852e4            //GPa

    // DistInBor=(1/(Denc(mol/cm3)*M_PartDist1Bor_is_VolIn_mole_cm3))^(1./3)
    // Denc(mol/cm3)=1/(DistInBor^3*M_PartDist1Bor_is_VolIn_mole_cm3)
#define M_PartDist1Bor_is_VolIn_cm3_mol 0.373803181029  
#define N_Vau_Vcm3 8.92389360063671354269829801183e-2

// Unchecked values




//#define M_PartDistInBor_DencIn_g_cm3 2.696  // Denc=M_../r_s^3


#define M_Epsilon0       8.854187817e-12   // Kulon^2/(H*m^2)  F=1/(4*Pi*Epsilon0)q^2/r^2
#define M_ElectrInter_Div_MassElectron  2.81794094e-13 // e^2/r/mc^2 ElectronDistance - 1 cm




#endif
