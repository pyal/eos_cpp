/* complex.h

    Complex Number Library - Include File
    class complex:  declarations for complex numbers.

    Copyright (c) 1990, 1992 by Borland International
    All Rights Reserved.

All function names, member names, and operators have been borrowed
from AT&T C++, except for the addition of:

    friend complex   acos(complex   &);
    friend complex   asin(complex   &);
    friend complex   atan(complex   &);
    friend complex   log10(complex   &);
    friend complex   tan(complex   &);
    friend complex   tanh(complex   &);
    complex   operator+();
    complex   operator-();
*/

#ifndef __cplusplus
#error Must use C++ for the type complex.
#endif

#if !defined(__COMPLEX_H)
#define __COMPLEX_H

#if !defined(___DEFS_H)
//#include <_defs.h>
#endif

#if !defined(__IOSTREAM_H)
#include <iostream.h>
#endif

#if !defined(__MATH_H)
#include <math.h>
#endif

//#pragma option -Vo-
//#if defined(__BCOPT__) && !defined(_RTL_ALLOW_po)
//#pragma option -po-
//#endif

//_CLASSDEF(complex)

class //_CLASSTYPE 
     complex {

public:
    // constructors
    complex(double __re_val, double __im_val=0);
    complex();

    // complex manipulations
    friend double     real(complex   &);   // the real part
    friend double     imag(complex   &);   // the imaginary part
    friend complex     conj(complex   &);  // the complex conjugate
    friend double     norm(complex   &);   // the square of the magnitude
    friend double     arg(complex   &);    // the angle in the plane

    // Create a complex object given polar coordinates
    friend complex   polar(double __mag, double __angle=0);

    // Overloaded ANSI C math functions
    friend double      abs(complex   &);
    friend complex     acos(complex   &);
    friend complex     asin(complex   &);
    friend complex     atan(complex   &);
    friend complex     cos(complex   &);
    friend complex     cosh(complex   &);
    friend complex     exp(complex   &);
    friend complex     log(complex   &);
    friend complex     log10(complex   &);
    friend complex     pow(complex   & __base, double __expon);
    friend complex     pow(double __base, complex   & __expon);
    friend complex     pow(complex   & __base, complex   & __expon);
    friend complex     sin(complex   &);
    friend complex     sinh(complex   &);
    friend complex     sqrt(complex   &);
    friend complex     tan(complex   &);
    friend complex     tanh(complex   &);

    // Binary Operator Functions
    friend complex     operator+(complex   &, complex   &);
    friend complex     operator+(double, complex   &);
    friend complex     operator+(complex   &, double);
    friend complex     operator-(complex   &, complex   &);
    friend complex     operator-(double, complex   &);
    friend complex     operator-(complex   &, double);
    friend complex     operator*(complex   &, complex   &);
    friend complex     operator*(complex   &, double);
    friend complex     operator*(double, complex   &);
    friend complex     operator/(complex   &, complex   &);
    friend complex     operator/(complex   &, double);
    friend complex     operator/(double, complex   &);
    friend int     operator==(complex   &, complex   &);
    friend int     operator!=(complex   &, complex   &);
    complex   &   operator+=(complex   &);
    complex   &   operator+=(double);
    complex   &   operator-=(complex   &);
    complex   &   operator-=(double);
    complex   &   operator*=(complex   &);
    complex   &   operator*=(double);
    complex   &   operator/=(complex   &);
    complex   &   operator/=(double);
    complex   operator+();
    complex   operator-();

// Implementation
private:
        double re, im;
};


// Inline complex functions

inline complex::complex(double __re_val, double __im_val)
{
    re = __re_val;
    im = __im_val;
}

inline complex::complex()
{
/* if you want your complex numbers initialized ...
    re = im = 0;
*/
}

inline complex   complex::operator+()
{
    return *this;
}

inline complex   complex::operator-()
{
    return complex(-re, -im);
}


// Definitions of compound-assignment operator member functions

inline complex   &   complex::operator+=(complex   & __z2)
{
    re += __z2.re;
    im += __z2.im;
    return *this;
}

inline complex   &   complex::operator+=(double __re_val2)
{
    re += __re_val2;
    return *this;
}

inline complex   &   complex::operator-=(complex   & __z2)
{
    re -= __z2.re;
    im -= __z2.im;
    return *this;
}

inline complex   &   complex::operator-=(double __re_val2)
{
    re -= __re_val2;
    return *this;
}

inline complex   &   complex::operator*=(double __re_val2)
{
    re *= __re_val2;
    im *= __re_val2;
    return *this;
}

inline complex   &   complex::operator/=(double __re_val2)
{
    re /= __re_val2;
    im /= __re_val2;
    return *this;
}


// Definitions of non-member complex functions

inline double   real(complex   & __z)
{
    return __z.re;
}

inline double   imag(complex   & __z)
{
    return __z.im;
}

inline complex   conj(complex   & __z)
{
    return complex(__z.re, -__z.im);
}

inline complex   polar(double __mag, double __angle)
{
    return complex(__mag*cos(__angle), __mag*sin(__angle));
}


// Definitions of non-member binary operator functions

inline complex   operator+(complex   & __z1, complex   & __z2)
{
    return complex(__z1.re + __z2.re, __z1.im + __z2.im);
}

inline complex   operator+(double __re_val1, complex   & __z2)
{
    return complex(__re_val1 + __z2.re, __z2.im);
}

inline complex   operator+(complex   & __z1, double __re_val2)
{
    return complex(__z1.re + __re_val2, __z1.im);
}

inline complex   operator-(complex   & __z1, complex   & __z2)
{
    return complex(__z1.re - __z2.re, __z1.im - __z2.im);
}

inline complex   operator-(double __re_val1, complex   & __z2)
{
    return complex(__re_val1 - __z2.re, -__z2.im);
}

inline complex   operator-(complex   & __z1, double __re_val2)
{
    return complex(__z1.re - __re_val2, __z1.im);
}

inline complex   operator*(complex   & __z1, double __re_val2)
{
    return complex(__z1.re*__re_val2, __z1.im*__re_val2);
}

inline complex   operator*(double __re_val1, complex   & __z2)
{
    return complex(__z2.re*__re_val1, __z2.im*__re_val1);
}

inline complex   operator/(complex   & __z1, double __re_val2)
{
    return complex(__z1.re/__re_val2, __z1.im/__re_val2);
}

inline int   operator==(complex   & __z1, complex   & __z2)
{
    return __z1.re == __z2.re && __z1.im == __z2.im;
}

inline int   operator!=(complex   & __z1, complex   & __z2)
{
    return __z1.re != __z2.re || __z1.im != __z2.im;
}


// Complex stream I/O

ostream   &     operator<<(ostream   &, complex   &);
istream   &     operator>>(istream   &, complex   &);

//#pragma option -Vo.
//#if defined(__BCOPT__)
//#pragma option -po.
//#endif

#endif  // __COMPLEX_H
