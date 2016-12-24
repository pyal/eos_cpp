
#ifndef FFT_H
#define FFT_H

#include "common.h"


#define CRRMULRE(z, yre, yim) ((z).Re * (yre) - (z).Im * (yim))
#define CRRMULIM(z, yre, yim) ((z).Re * (yim) + (z).Im * (yre))
#define CCMULRE(z1, z2) ((z1).Re * (z2).Re - (z1).Im * (z2).Im)
#define CCMULIM(z1, z2) ((z1).Re * (z2).Im + (z1).Im * (z2).Re)

typedef struct {
    real Re;
    real Im;
} complexWavelet;

#ifdef __cplusplus
extern "C" {
#endif
int br(
    int n,        /* Nonnegative integer to bit-reverse. */
    int log2len); /* Reverse this many bits.  */


//extern "C" void
void bitrevd(
    void *out,      /* Pointer to base of the output array. */
    const void *in, /* Pointer to base of the input array. */
    int q,          /* # of index bits, or elements of `in[]'. */
    int size);      /* Number of bytes in an `in[]' element.  */

//extern "C"
void bitrevi(
    void *x,   /* Pointer to the input/output array. */
    int q,     /* # of index bits, or elements of `x[]'. */
    int size); /* Number of bytes in an `x[]' element.  */

//extern "C"
void fftproduct(                          /* Apply sparse matrix product. */
                complexWavelet *f,        /* Input and output vector.     */
                int q,                    /* Length of `f[]' is N=1<<q.   */
                const complexWavelet *W); /* Exponentials: `Omega(N/2)' */

//extern "C"
complexWavelet *fftomega(        /* Return exp(-PI*i*n/M), */
                         int M); /* for n=0,1,2,...,|M|-1. */

//extern "C"
void fftnormal(                   /* Multiply `f[n].Re' and     */
               complexWavelet *f, /* `f[n].Im' by `1.0/sqrt(N), */
               int N);            /* for n=0,1,2,...,N.         */

//extern "C"
complexWavelet *dft(                         /* Allocate, assign and return  */
                    const complexWavelet *f, /* a complexWavelet vector, the (1<<q) */
                    int q);                  /* point DFT of the input `f[]' */

#ifdef __cplusplus
};
#endif

#endif
