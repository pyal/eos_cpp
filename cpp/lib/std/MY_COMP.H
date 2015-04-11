
#ifndef __cplusplus
#error Must use C++ for the type complex.
#endif

#if !defined(__MY_COMP_H)
#define __MY_COMP_H

//#if !defined(___DEFS_H)
//#include <_defs.h>
//#endif

//#if !defined(__IOSTREAM_H)
#include "util.h"
//#include <iostream.h>
//#endif

//#if !defined(__MATH_H)
#include <math.h>
//#endif

//#pragma option -Vo-
//#if defined(__BCOPT__) && !defined(_RTL_ALLOW_po)
//#pragma option -po-
//#endif

//_CLASSDEF(complex)

class //_CLASSTYPE 
     my_comp {

public:
    // constructors
    my_comp(double __re_val, double __im_val=0);
    my_comp();

    // complex manipulations
    friend double     my_real(my_comp   &);   // the real part
    friend double     my_imag(my_comp   &);   // the imaginary part
    friend my_comp     conj(my_comp   &);  // the complex conjugate
    friend double     norm(my_comp   &);   // the square of the magnitude
    friend double     arg(my_comp   &);    // the angle in the plane

    // Create a complex object given polar coordinates
    friend my_comp   polar(double __mag, double __angle=0);

    // Overloaded ANSI C math functions
    friend my_comp     cos(my_comp   &);
    friend my_comp     cosh(my_comp   &);
    friend my_comp     exp(my_comp   &);
    friend my_comp     log(my_comp   &);
    friend my_comp     pow(my_comp   & __base, my_comp   & __expon);
    friend my_comp     pow(my_comp   & __base, double __expon);
    friend my_comp     sin(my_comp   &);
    friend my_comp     sinh(my_comp   &);
    friend my_comp     sqrt(my_comp   &);
//    friend double      abs(my_comp   &);
    friend double      fabs(my_comp   &);
/*
    friend my_comp     pow(double __base, my_comp   & __expon);
    friend my_comp     acos(my_comp   &);
    friend my_comp     asin(my_comp   &);
    friend my_comp     atan(my_comp   &);
    friend my_comp     log10(my_comp   &);
    friend my_comp     tan(my_comp   &);
    friend my_comp     tanh(my_comp   &);
*/
    // Binary Operator Functions
    friend my_comp     operator+(my_comp   &, my_comp   &);
    friend my_comp     operator+(double, my_comp   &);
    friend my_comp     operator+(my_comp   &, double);
    friend my_comp     operator-(my_comp   &, my_comp   &);
    friend my_comp     operator-(double, my_comp   &);
    friend my_comp     operator-(my_comp   &, double);
    friend my_comp     operator*(my_comp   &, my_comp   &);
    friend my_comp     operator*(my_comp   &, double);
    friend my_comp     operator*(double, my_comp   &);
    friend my_comp     operator/(my_comp   &, my_comp   &);
    friend my_comp     operator/(my_comp   &, double);
    friend my_comp     operator/(double, my_comp   &);
    friend int     operator==(my_comp   &, my_comp   &);
    friend int     operator!=(my_comp   &, my_comp   &);
    my_comp   &   operator+=(my_comp   &);
    my_comp   &   operator+=(double);
    my_comp   &   operator-=(my_comp   &);
    my_comp   &   operator-=(double);
    my_comp   &   operator*=(my_comp   &);
    my_comp   &   operator*=(double);
    my_comp   &   operator/=(my_comp   &);
    my_comp   &   operator/=(double);
    my_comp   operator+();
    my_comp   operator-();
    double &real(){return re;}
    double &image(){return im;}

// Implementation
private:
        double re, im;
};

void errorFP(char *FuncName,char *Err) ;
// Inline complex functions

inline my_comp::my_comp(double __re_val, double __im_val)
{
    re = __re_val;
    im = __im_val;
}

inline my_comp::my_comp()
{
/* if you want your complex numbers initialized ...
    re = im = 0;
*/
}

inline my_comp   my_comp::operator+()
{
    return *this;
}

inline my_comp   my_comp::operator-()
{
    return my_comp(-re, -im);
}


// Definitions of compound-assignment operator member functions

inline my_comp   &   my_comp::operator+=(my_comp   & __z2)
{
    re += __z2.re;
    im += __z2.im;
    return *this;
}

inline my_comp   &   my_comp::operator+=(double __re_val2)
{
    re += __re_val2;
    return *this;
}

inline my_comp   &   my_comp::operator-=(my_comp   & __z2)
{
    re -= __z2.re;
    im -= __z2.im;
    return *this;
}

inline my_comp   &   my_comp::operator-=(double __re_val2)
{
    re -= __re_val2;
    return *this;
}

inline my_comp   &   my_comp::operator*=(double __re_val2)
{
    re *= __re_val2;
    im *= __re_val2;
    return *this;
}

inline my_comp   &   my_comp::operator/=(double __re_val2)
{
    re /= __re_val2;
    im /= __re_val2;
    return *this;
}


// Definitions of non-member complex functions

inline double   my_real(my_comp   & __z)
{
    return __z.re;
}

inline double   my_imag(my_comp   & __z)
{
    return __z.im;
}

inline my_comp   conj(my_comp   & __z)
{
    return my_comp(__z.re, -__z.im);
}

inline my_comp   polar(double __mag, double __angle)
{
    return my_comp(__mag*cos(__angle), __mag*sin(__angle));
}


// Definitions of non-member binary operator functions

inline my_comp   operator+(my_comp   & __z1, my_comp   & __z2)
{
    return my_comp(__z1.re + __z2.re, __z1.im + __z2.im);
}

inline my_comp   operator+(double __re_val1, my_comp   & __z2)
{
    return my_comp(__re_val1 + __z2.re, __z2.im);
}

inline my_comp   operator+(my_comp   & __z1, double __re_val2)
{
    return my_comp(__z1.re + __re_val2, __z1.im);
}

inline my_comp   operator-(my_comp   & __z1, my_comp   & __z2)
{
    return my_comp(__z1.re - __z2.re, __z1.im - __z2.im);
}

inline my_comp   operator-(double __re_val1, my_comp   & __z2)
{
    return my_comp(__re_val1 - __z2.re, -__z2.im);
}

inline my_comp   operator-(my_comp   & __z1, double __re_val2)
{
    return my_comp(__z1.re - __re_val2, __z1.im);
}

inline my_comp   operator*(my_comp   & __z1, double __re_val2)
{
    return my_comp(__z1.re*__re_val2, __z1.im*__re_val2);
}

inline my_comp   operator*(double __re_val1, my_comp   & __z2)
{
    return my_comp(__z2.re*__re_val1, __z2.im*__re_val1);
}

inline my_comp   operator/(my_comp   & __z1, double __re_val2)
{
    return my_comp(__z1.re/__re_val2, __z1.im/__re_val2);
}

inline int   operator==(my_comp   & __z1, my_comp   & __z2)
{
    return __z1.re == __z2.re && __z1.im == __z2.im;
}

inline int   operator!=(my_comp   & __z1, my_comp   & __z2)
{
    return __z1.re != __z2.re || __z1.im != __z2.im;
}

inline double     arg(my_comp   &x)
{
   return atan2(x.im, x.re);
}

inline double  fabs(my_comp &x)
 {
   return hypot(x.re, x.im);
 }
 
inline double  norm(my_comp &x)      
{
  return (x.re * x.re + x.im * x.im);
}
 

// complex stream I/O

ostream   &     operator<<(ostream   &out, my_comp   &c);
istream   &     operator>>(istream   &in , my_comp   &c);

//#pragma option -Vo.
//#if defined(__BCOPT__)
//#pragma option -po.
//#endif

#endif  // __MY_COMP_H

/*


 inline Complex sqr     (Complex a)         { return a*a; }
 inline Complex pow2    (Complex a)         { return a*a; }
 inline Complex pow3    (Complex a)         { return a*a*a; }
 inline Complex pow4    (Complex a)         { Complex b = a*a; return b*b; }
 
 extern Complex pow_int  (Complex a, int i);  // a to power i
 extern BooLean eq (Complex a, Complex b, real tol = comparison_tolerance);
 extern BooLean lt (Complex a, Complex b, real tol = comparison_tolerance);
 extern BooLean le (Complex a, Complex b, real tol = comparison_tolerance);
 
 inline Complex min(Complex arg1, Complex arg2)
 { return  arg1 < arg2 ? arg1 : arg2; }
 
 inline Complex max(Complex arg1, Complex arg2)
 { return  arg1 > arg2 ? arg1 : arg2; }
 

 
 // non-inline functions
 
 char*  oform2 (char* s, Complex c);
 real   getReal (const Complex& c);
 
 Complex   operator /  (const Complex& x, const Complex& y);
 Complex   operator /  (const Complex& x, real   y);
 Complex   operator /  (real     x, const Complex& y);
 
 Complex   cos(const Complex& x);
 Complex   sin(const Complex& x);
 
 Complex   cosh(const Complex& x);
 Complex   sinh(const Complex& x);
 Complex   exp(const Complex& x);
 //int   exp(const int& i);
 Complex   log(const Complex& x);
 
 Complex   pow(const Complex& x, int p);
 Complex   pow(const Complex& x, const Complex& p);
 Complex   pow(const Complex& x, real   y);
 Complex   sqrt(const Complex& x);
 
 
// /* use Is/Os instead (automatic type conversion from iostream is available!)
 istream&  operator >> (istream& s, Complex& x);
 ostream&  operator << (ostream& s, const Complex& x);
//* / 
 
 Is&  operator >> (Is& s, Complex& x);
 Os&  operator << (Os& s, const Complex& x);
 



 inline int  operator == (const Complex& x, const Complex& y);
 inline int  operator == (const Complex& x, real   y);
 inline int  operator != (const Complex& x, const Complex& y);
 inline int  operator != (const Complex& x, real   y);
 inline int  operator >  (const Complex& x, const Complex& y);
 inline int  operator >= (const Complex& x, const Complex& y);
 inline int  operator <  (const Complex& x, const Complex& y);
 inline int  operator <= (const Complex& x, const Complex& y);
 
 inline Complex  operator - (const Complex& x);
 inline Complex  conj(const Complex& x);
 inline Complex  operator + (const Complex& x, const Complex& y);
 inline Complex  operator + (const Complex& x, real   y);
 inline Complex  operator + (real   x, const Complex& y);
 inline Complex  operator - (const Complex& x, const Complex& y);
 inline Complex  operator - (const Complex& x, real   y);
 inline Complex  operator - (real   x, const Complex& y);
        Complex  operator * (const Complex& x, const Complex& y);
 inline Complex  operator * (const Complex& x, real   y);
 inline Complex  operator * (real   x, const Complex& y);
 
 inline real    Re(const Complex& x);
 inline real    Im(const Complex& x);
 inline real    abs(const Complex& x);
 inline real    norm(const Complex& x);
 inline real    arg(const Complex& x);
 
 inline Complex  polar(real   r, real   t = 0.0);
*/
