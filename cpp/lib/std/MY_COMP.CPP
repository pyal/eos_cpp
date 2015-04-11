#include <lib\precompiled\Std.h>


#include "my_comp.h"


my_comp  operator * (my_comp & x, my_comp & y)
{
  return my_comp(my_real(x) * my_real(y) - my_imag(x) * my_imag(y),
                 my_real(x) * my_imag(y) + my_imag(x) * my_real(y));
}
my_comp &  my_comp::operator*=(my_comp &y)
{
    double re1 = re*my_real(y)-im*my_imag(y);
    im = re*my_imag(y)+im*my_real(y);
    re=re1;
    return *this;
}

my_comp /* const */ operator / (my_comp& x, my_comp& y)
{
//cout<<" x "<<x<<" y "<<y<<"\n";
  double den = fabs(my_real(y)); den += fabs(my_imag(y));
  if (den == 0.0) errorFP("my_comp::operator/","Attempted division by zero.");
  double xrden = my_real(x) / den;
  double xiden = my_imag(x) / den;
  double yrden = my_real(y) / den;
  double yiden = my_imag(y) / den;
  double nrm   = yrden * yrden + yiden * yiden;
  return my_comp((xrden * yrden + xiden * yiden) / nrm,
                 (xiden * yrden - xrden * yiden) / nrm);
}

my_comp& my_comp::operator /= (my_comp& y)
{
//cout<<" x0 "<<*this<<" y "<<y<<"\n";
  double den = fabs(my_real(y)); den += fabs(my_imag(y));
  if (den == 0.0) errorFP("my_comp::operator/","Attempted division by zero.");
  double xrden = re / den;
  double xiden = im / den;
  double yrden = my_real(y) / den;
  double yiden = my_imag(y) / den;
  double nrm   = yrden * yrden + yiden * yiden;
  re = (xrden * yrden + xiden * yiden) / nrm;
  im = (xiden * yrden - xrden * yiden) / nrm;
  return *this;
}

my_comp /* const */ operator / (double x, my_comp& y)
{
  double den = norm(y);
  if (den == 0.0) errorFP("my_comp::operator/","Attempted division by zero.");
  return my_comp((x * my_real(y)) / den, -(x * my_imag(y)) / den);
}

/*
my_comp  operator / (my_comp& x, double y)
{
  if (y == 0.0) errorFP("my_comp::operator/","Attempted division by zero.");
  return my_comp(my_real(x) / y, my_imag(x) / y);
}

my_comp& my_comp::operator /= (double y)
{
  if (y == 0.0) errorFP("my_comp::operator/","Attempted division by zero.");
  re /= y;  im /= y;
  return *this;
}
*/

my_comp /* const */ exp( my_comp& x)
{
  double r = exp(my_real(x));
  return my_comp(r * cos(my_imag(x)),
                 r * sin(my_imag(x)));
}

my_comp /* const */ cosh( my_comp& x)
{
  return my_comp(cos(my_imag(x)) * cosh(my_real(x)),
                 sin(my_imag(x)) * sinh(my_real(x)));
}

my_comp /* const */ sinh( my_comp& x)
{
  return my_comp(cos(my_imag(x)) * sinh(my_real(x)),
                 sin(my_imag(x)) * cosh(my_real(x)));
}

my_comp /* const */ cos( my_comp& x)
{
  return my_comp(cos(my_real(x)) * cosh(my_imag(x)),
                 -sin(my_real(x)) * sinh(my_imag(x)));
}

my_comp /* const */ sin( my_comp& x)
{
  return my_comp(sin(my_real(x)) * cosh(my_imag(x)),
                 cos(my_real(x)) * sinh(my_imag(x)));
}

my_comp /* const */ log( my_comp& x)
{
  double h = hypot(my_real(x), my_imag(x));
  if (h <= 0.0) errorFP("my_comp::log",
			"attempted log of zero magnitude number.");
  return my_comp(log(h), atan2(my_imag(x), my_real(x)));
}

// Corrections based on reports from: thc@cs.brown.edu & saito@sdr.slb.com
my_comp /* const */ pow( my_comp& x,  my_comp& p)
{
  double h = hypot(my_real(x), my_imag(x));
  if (h <= 0.0) errorFP("my_comp::pow",
			"attempted power of zero magnitude number.");

  double a = atan2(my_imag(x), my_real(x));
  double lr = pow(h, my_real(p));
  double li = my_real(p) * a;
  if (my_imag(p) != 0.0)
  {
    lr /= exp(my_imag(p)  * a);
    li += my_imag(p)  * log(h);
  }
  return my_comp(lr * cos(li), lr * sin(li));
}

my_comp /* const */ pow( my_comp& x, double p)
{
  double h = hypot(my_real(x), my_imag(x));
  if (h <= 0.0) errorFP("my_comp::pow",
			"attempted power of zero magnitude number.");
  double lr = pow(h, p);
  double a = atan2(my_imag(x), my_real(x));
  double li = p * a;
  return my_comp(lr * cos(li), lr * sin(li));
}


my_comp /* const */ sqrt( my_comp& x)
{
  if (my_real(x) == 0.0 && my_imag(x) == 0.0)
    return my_comp(0.0, 0.0);
  else
    {
      double s = sqrt((fabs(my_real(x)) + hypot(my_real(x), my_imag(x))) * 0.5);
      double d = (my_imag(x) / s) * 0.5;
      if (my_real(x) > 0.0)
 return my_comp(s, d);
      else if (my_imag(x) >= 0.0)
 return my_comp(d, s);
      else
 return my_comp(-d, -s);
    }
}

ostream   &     operator<<(ostream   &out, my_comp   &c)
  {out<<"("<<my_real(c)<<","<<my_imag(c)<<")";return out;};
istream   &     operator>>(istream   &in , my_comp   &c)
  {double re,im;char tmp[256];in>>tmp>>re>>tmp>>im>>tmp;c=my_comp(re,im);return in;};
void errorFP(char *FuncName,char *Err) 
{ cout<<" errorFR. Error in function "<<FuncName<<" \ncaused by "<<Err<<"\n";}

/*
Os& operator << (Os& s, const my_comp& x)
{
  if (s->getFormat() == ASCII)
    s << '(' << my_real(x) << ',' << my_imag(x) << ')' ;
  else // binary format:
    {
      s->put(my_real(x));
      s->put(my_imag(x));
    }
  return s;
}



Is& operator >> (Is& s, my_comp& x)
{
  double r, i;
  if (s->getFormat() == ASCII)
    {
      s->ignore('(');
      s >> r;
      s->ignore(',');
      s >> i;
    }
  else
    {
      s->get(r);
      s->get(i);
    }
  x = my_comp(r, i);
  return s;
}


/*
my_comp  pow(const my_comp& x, int p)
{
  if (p == 0)  return my_comp(1.0, 0.0);
  else if (x == 0.0) return my_comp(0.0, 0.0);
  else
  {
    my_comp res(1.0, 0.0);
    my_comp b = x;
    if (p < 0)
    {
      p = -p;
      b = 1.0 / b;
    }
    for(;;)
    {
      if (p & 1)
      {
        res *= b;
	b *= b;
      }
      if ((p >>= 1) == 0)
        return res;
    }
   return my_comp(0,0); // job: should newer be called, just to avoid warning
  }
}

Complex pow_int  (Complex a, int i)
{
  // computes a**i (Fortran-syntax)


  if (i == 0)
    return 1;
  else if (i == 1)
    return a;
  else if (i == 2)
    return a*a;
  else if (i == 3)
    return a*a*a;
  else
    {
      int  k;
      Complex h = 1;
      Complex h1 = 0;
      for (k=1; k<=fabs(i); k++)
	h *= a;
      if (i > 0)
	 h1 = h;
      else
	h1 = 1.0/h;
      return h1;
    }

}

BooLean eq (Complex a, Complex b, real tol)
{
  // checks whether a==b to some tolerance.
  // note that a!=b is checked by !eq(.,.)
  // tol is comparison_tolerance (global variable) per default

  real diff = fabs(a-b);

  return getBooLean(diff < tol);
}


BooLean lt (Complex a, Complex b, real tol)
{
  if (a + Complex(tol, tol) < b)
    return dpTRUE;
  else
    return dpFALSE;
}

BooLean le (Complex a, Complex b, real tol)
{
  if (a - Complex(tol, tol) < b)
    return dpTRUE;
  else
    return dpFALSE;
}


*/
