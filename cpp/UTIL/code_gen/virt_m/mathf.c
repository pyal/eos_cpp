#include <math.h>

/**************************************************************
  MATHF.C
 **************************************************************/



/*  mathematical functions  */

double ZAcos ( double x ) { return( acos( x ) ); }
double ZAsin ( double x ) { return( asin( x ) ); }

double ZAtan ( double x ) { return( atan( x ) ); }
double ZAtan2( double x, double y )
{
   return( atan2( x, y ) );
}

double ZCos  ( double x ) { return( cos( x ) ); }
double ZSin  ( double x ) { return( sin( x ) ); }
double ZTan  ( double x ) { return( tan( x ) );   }

double ZCosh ( double x ) { return( cosh( x ) ); }
double ZSinh ( double x ) { return( sinh( x ) ); }
double ZTanh ( double x ) { return( tanh( x ) );  }

double ZSqrt ( double x ) { return( sqrt( x ) ); }

double ZExp  ( double x ) { return( exp( x ) ); }
double ZLog  ( double x ) { return( log( x ) ); }
double ZLog10( double x ) { return( log10( x ) ); }

double ZCeil ( double x ) { return( ceil( x ) );  }
double ZFabs ( double x ) { return( fabs( x ) );  }
double ZFloor( double x ) { return( floor( x ) ); }

double ZPow  ( double x, double y ) { return( pow( x, y ) ); }



/*******************  END OF FILE MATHF.Y  ********************/

