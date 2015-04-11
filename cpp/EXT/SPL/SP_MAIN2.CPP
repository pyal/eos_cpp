
/* ---INCLUDES--- */
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <string.h> 

#ifdef __cplusplus
extern "C" {
#endif

/* ---DEFINES (NUMBERS)--- */
#define NMAX 10000      /* Maximum number of data points */
#define LMAX 2001       /* Maximum number of intervals */
#define KMAX 24          /* Maximum spline order */
#define RMAX 500       /* Maximum number of intermediate results */  
#define REJLEV 0.05     /* Rejection level of statistical tests (5%) */
#define UPFACTOR  1.1   /* Increment factor for l */
#define DOWNFACTOR 0.95 /* Decrement factor for l */
#define UPMARGIN 1e-7   /* Calculated (double) values of l this close 
                           under an integer are rounded upward instead
                           of truncated downward (v. 5.02) */

/* ---DEFINES (MACROS)--- */
#define TRUNC_SPEC(i,d) (((double)((i)+1)-(d)) < UPMARGIN ? ((i)+1) : (i))


void bchfac(double dara[][LMAX+KMAX],int *iptr,int *jptr,double ara[]);
void bchslv(double dara[][LMAX+KMAX],int *iptr,int *jptr,double ara[]);
double betacf(double a,double b,double x);
double betai(double a,double b,double x);
void bsplpp(double ara[],double arb[],int *iptr,int *jptr,
           double dara[][KMAX+LMAX],double arc[],double darb[][LMAX],int *kptr);
//           double dara[KMAX][],double arc[],double darb[][LMAX],int *kptr)
void bsplvb(double ara[],int jhigh,int index,double *xptr,int *iptr,double arb[]);
double chitest(double chi,int fr);
void dwtest(double x,double *xptr,double *yptr) ;
void gcf(double *xptr,double a,double x,double *yptr);
void graer(int ind);
void grasp();
void gser(double *xptr,double a,double x,double *yptr);
void interv(double ara[],int *iptr,double *xptr,int *jptr,int *kptr);
void l2appr(double ara[],int *iptr,int *jptr,
           double dara[][LMAX+KMAX],double arb[],double arc[]);
void l2err(int prfun,int grfun)  ;
void l2knts(double *ara,int *iptr,int *jptr,double *arb,int *kptr);
void l2sub(int *iptr,int iopt,int wr);
void newknt(double ara[],double dara[][LMAX],int *iptr,int *jptr,
           double arb[],int *kptr,double darb[][LMAX]);
double ppvalu(double ara[],double dara[][LMAX],int *iptr,
              int *jptr,double *xptr,int jderiv);
/* ---ANNOUNCE FUNCTIONS---  */
/*
int     l2sub(),
        newknt(),
        l2knts(),
        l2appr(),
        bchfac(),
        bchslv(),
        bsplpp(),
        bsplvb(),
        l2err(),
        interv(),
        dwtest(),
        gser(),
        gcf(),
        graer(),
        grasp();
double  ppvalu(),
        chitest(),
        betai(),
        betacf();

*/

/*----GLOBAL VARIABLES----*/
int    km1,             /* degree of polynomials */
       k,               /* order of polynomials */
       ntau,            /* number of datapoints */
       l, lnew,         /* number of intervals */
       ibeg,            /* starting index of knot optimization */
       n,               /* order of spline-approximation = k+l-1 */
       freed,           /* degrees of freedom left */
       nsing,           /* number of singularities */
       ihi, ilo = 1,    /* flags for function interv() */
       nl2sub,          /* index of highest-l non-optimized approximation */  
       il2sub,          /* counter of and number of approximations */
       isug,            /* index of suggested approximation */
       logtrans,        /* flag for log10-transformation of x-scale */
       silent,          /* flag for the -q option */
       jbsp,            /* counter for function bsplvb() */
       iresul[RMAX][3], /* l and freed for all approximations */
       lend, lbeg,      /* endpoints of interval for sigma determination */
       lfin,            /* final (?) choice */
       left,            /* counter for function newknt() */
       wind,            /* flag for warning status */
       sigma,           /* if =0: sigmas specified in input-file, otherwise =1*/
       fullon,          /* flag, =1 if full output is required */
       fix,             /* flag, =1 if sigma is fixed */
       rel,             /* flag, =1 if sigma is relative to y */
       done,            /* flag, =1 if Done? has been answered by yes */
       njan,            /* durbin-watson distance index */
       zero_search;
double ppvalue,         /* return value of program ppvalu() */
       rmsmin, rmsmax,  /* minimum and maximum rms-value */
       taumin, taumax,  /* minimum and maximum y-value of datapoints */
       dermax,          /* maximum absolute value of the spline derivative */
       totalw,          /* sum of weight factors */
       rms,             /* root mean square error */
       dws,             /* durbin-watson statistic(s) */
       ctest, ltest,    /* percentage points of various distributions */
       utest, test,
       fixval,          /* (if positive) value at which sigma is fixed */
       nonsig,          /* value of first nonsignificant digit range */
       tau[NMAX],       /* x-values of input data */
       gtau[NMAX],      /* y-values of input data */
       weight[NMAX],    /* weight factors of input data */
       maxweight,       /* largest weight (only used for negative fixval) */
       ftau[NMAX],      /* spline approximation data S(x) at input x-values */
       q[NMAX],         /* normalized errors */
       deriv[NMAX],     /* first derivative of spline approximation */
       brek[LMAX],      /* breakpoints */
       bcoef[LMAX+KMAX],/* b-coefficients */
       coef[KMAX][LMAX],/* pp-representation */
       resul[RMAX][6],  /* test statistics for all splines */
       deltal[KMAX],    /* used in function bsplvb() */
       deltar[KMAX];    /* used in function bsplvb() */
char   Accept,          /* indicator if final result is satisfying */
       line[200],       /* line read from input */
       rmsversion[80];  /* scaling mode of "rms" variable */
FILE   *fpd, *fpr, *fpp, *fps;

/* ---MAIN--- */
//main(argc,argv)  int argc; char *argv[];
void main(int argc,char *argv[])
        {
        /* ---DECLARATIONS (note: auto-arrays can't be initialized)--- */
        register int i, j;
        int          grend, xflag, Xflag, nn, m, ln, lind, lopt, lmax; 
        int          ldone; /* v5.0 */
        double       a, xbegin, xend, Xbegin, Xend, rmmin, dummy;
        char         dum, gragain, lstr[6], str[100];

        /* ---INITIALIZATION---*/
        fpd = stdin;
        Xflag = fullon = xflag = lind = silent = 0;
        fix = rel = done = 0;
        sigma = njan = 1;
        k = 4;
 
        /* ---SHOW OPTIONS--- */
        if (argc == 1)
            {
printf("====================================================================\n");
printf(".............spline2 version 5.02 (14 may 1997)...............\n");
printf("Usage: 'spline2 [datafile] [options]'\n");
printf("       [options] are:\n");
printf("[-x xbeg xend] Approximate only data between these x-values\n");
printf("[-X xbeg xend] Ignore data between these x-values\n");
printf("[-a val]       'Absolute': freeze std.dev. of y-errors (chi2-test)\n");
printf("                 val>0: sigma=val, val=-n: n sign. dig., val=0: 3rd col\n");
printf("[-r val]       'Relative': freeze std.dev. of y-errors (chi2-test)\n");
printf("                 val>0: sigma=val*|y|, val<0: sigma=|val|*sqrt(|y|)\n");
printf("[-n spacing]   Let dws measure correlation in data spaced >1 apart\n");
printf("[-o lrev]      Force knot improvement to start at l=lrev\n");
printf("[-k order]     Use splines of order other than 4\n");
printf("[-F]           Full output file collection instead of just 'splrep'\n");
printf("[-q]           Quick (=only suggested spline). Pipe output to 'evalsp'\n");
printf("[-l]           First take 10log of x\n");
printf("[-z]           ZeroSearch of optimum\n");
printf("====================================================================\n");
            exit(1);
            }
zero_search=0;
        /* ---GET RUN TIME OPTIONS---*/
        if (argc > 1)
        {
            for (m=1; m<argc; m++)
            {
                switch( *argv[m] )
                {
                case '-':
                    switch( *(argv[m]+1) )
                    {
                    case 's' : /* dummy */
                               break;
                    case 'X' : Xbegin = atof(argv[++m]);
                               Xend = atof(argv[++m]);
                               Xflag = 1;
                               break;
                    case 'x' : xbegin = atof(argv[++m]);
                               xend = atof(argv[++m]);
                               xflag = 1;
                               break;
                    case 'l' : logtrans = 1;
                               break;
                    case 'n' : njan = atoi(argv[++m]);
                               break;
                    case 'k' : k = atoi(argv[++m]);
                               if (k > KMAX) k = KMAX;
                               if (k < 1) k = 1;
                               break;
                    case 'r' : rel = 1;
                    case 'f' : ;
                    case 'a' : fix = 1;
                               fixval = atof(argv[++m]);
                               break;
                    case 'o' : lopt = atoi(argv[++m]);
                               lind = 1;
                               break;
                    case 'q' : silent = 1;
                               break;
                    case 'z' : zero_search = 1;
                               break;
                    case 'F' : fullon = 1;
                               break;
                    default :  break;
                    }
                    break;
                default : strcpy(str,argv[m]);
                          if ((fpd = fopen(str,"r")) == NULL)
                          {
                                printf("Cannot open %s\n", str);
                                exit(1);
                          }
                }
            }
        }

        /* ---OPEN FILES FOR WRITING---*/
        if (fullon)
        if ((fpr = fopen("splstat","w")) == NULL)
                {printf("....Cannot open file splstat....\n");}
        if ((fps = fopen("splrep","w")) == NULL)
                {printf("....Cannot open file splrep....\n");}
        if (fullon)
        if ((fpp = fopen("splres","w")) == NULL)
                {printf("....Cannot open file splres....\n");}

        /* ---READ DATA--- */
        maxweight = -99.99;
        i = 1;
// Formation of 
//    tau, gtau, weight  nonsig - (fixval<0)  ntau
//   weight - from 
//    sigma,  fix  rel  fixval  nonsig
        while (fgets(line,200,fpd)!=NULL) {
                j = sscanf(line,"%lf %lf %lf %lf", tau+i, gtau+i, weight+i, &a);
                if (j<2) {
                        fprintf(stderr,
                        "Line %d has less than two numbers: skipped\n", i);
                        continue;
                        }
                if (i==1) {
                        /* determine # input columns */
                        if (j==3) sigma = 0;
                        else if (j==4 || j<2)
                           {fprintf(stderr,"Wrong data format\n"); exit(1);}
                        }
                if (i>=NMAX-1) {fprintf(stderr,"Too many data\n"); exit(1);}
                if (sigma==0) {
                        /* sigmas are supposed to be in file */
                        if (j==2) weight[i] = weight[i-1];
                        if (j==3) {
                                if (weight[i]<=0.0) 
                                  {fprintf(stderr,"Zero or negative sigma\n"); 
                                  exit(1);}
                                weight[i] = 1.0/weight[i]/weight[i];
                                }
                        }
                if (sigma==1) {
                        /* sigmas are not supposed to be in file */
                        weight[i] = 1.0;
                        }
                if (fix==1) {
                        /* sigmas freezed: override assignments */
                        if (rel==0) {
                                /* absolute sigma freezed */
                                if (fixval>0.0) weight[i] = 1.0/fixval/fixval;
                             /* if (fixval==0.0) weight was already assigned */
                                if (fixval<0.0) {
                                        if (gtau[i]==0.0)
                                          /* temporary value: */
                                          weight[i]= -999.999;
                                        else {
                                          nonsig = log10(fabs(gtau[i]));
                                          /* repaired 26-2-1996 */
                                          /*nonsig = floor(nonsig)+fixval;*/
                                          nonsig = floor(nonsig)+fixval+1.0;
                                          nonsig = exp(2.302585*nonsig);
                                          nonsig /= 3.464102;
                                          weight[i] = 1.0/nonsig/nonsig;
                                          if (weight[i]>maxweight)
                                              maxweight=weight[i];
                                          }
                                        }
                                }
                        if (rel==1) {
                                /* relative sigma freezed */
                                if (fixval>0.0) {
                                        weight[i] = fixval*fabs(gtau[i]);
                                        weight[i] = 1.0/weight[i]/weight[i];
                                        }
                                if (fixval<0.0) {
                                        weight[i] = -fixval*sqrt(fabs(gtau[i]));
                                        weight[i] = 1.0/weight[i]/weight[i];
                                        }
                                }
                        }
                /* weight assigned; accept point for fit? */
                a = tau[i];
                if ((xflag==0 && Xflag==0) ||
                    (xflag==1 && Xflag==0 && a>=xbegin && a<=xend) ||
                    (xflag==0 && Xflag==1 && (a>Xend || a<Xbegin)) ||
                    (xflag==1 && Xflag==1 && 
                    ((a>=xbegin && a<Xbegin) || (a>Xend && a<=xend))))
                    i++;
                } /* end of read loop */

        ntau = i-1;

//   End of Formation of 
//    tau, gtau, weight  nonsig - (fixval<0)  ntau
//   weight - from 
//    sigma,  fix  rel  fixval  nonsig


// Form lopt - number of splines, weight

        /* If data have certain number of significant digits, assign
           largest weight to the data value(s) zero */
        if (fix==1 && rel==0 && fixval<0.0) {
                for (i=1; i<=ntau; i++) {
                        if (weight[i]== -999.999) weight[i]=maxweight;
                        }
                }

        if (lopt >= .8*ntau || lind == 0) lopt = .8*ntau-1;
        km1 = k-1;
        lmax = ntau-km1-1;
        if (lmax < lopt) lopt = lmax;

// End of formation

        for (i = 1; i <= ntau; i++) totalw += weight[i];
        taumax = taumin = gtau[1];
        for (i = 2; i <= ntau; i++)
        {
                if (gtau[i] < taumin) taumin = gtau[i];
                if (gtau[i] > taumax) taumax = gtau[i];
        }
        if (logtrans != 0) for(i = 1; i <= ntau; i++) tau[i] = log10(tau[i]);

        /* reading done */

        /* ---CLOSE READFILE--- */
        if (fpd != stdin) fclose(fpd);

        /* ---MESSAGES--- */
        if (!silent) {
        printf(".........spline2 version 5.02 (14 may 1997)...........\n");
        if (fpd == stdin) strcpy(str,"STDIN");
        printf("Name of data file: %s\n", str);
        if (sigma==1) printf("Format: x y\n");
        if (sigma==0) printf("Format: x y sigma\n");
        if (logtrans==1) printf("x -> log10(x)\n");
        if (xflag==1) printf("X-range limited to (%g, %g)\n", xbegin, xend);
        if (Xflag==1) printf("X-range (%g, %g) cut out\n", Xbegin, Xend);
        printf("Number of datapoints participating in spline-fit = %d\n", ntau);
        if (fullon==1) printf("Full output to files splstat and splres\n");
        if (lind == 1) printf("Search-reversal point set at l=%d\n", lopt);
        printf("Degree of spline = %d\n", k-1);
        if (fix==1) {
                printf("Uncertainties in y frozen at ");
                if (rel==0) {
                        if (fixval==0.0) printf("values specified in input\n");
                        if (fixval>0.0) printf("%g\n", fixval);
                        if (fixval<0.0) printf("%g-th digit\n", -fixval+1);
                        }
                if (rel==1) {
                        if (fixval>0.0) printf("%g*|y|\n", fixval);
                        if (fixval<0.0) printf("%g*sqrt(|y|)\n", -fixval);
                        }
                }
        printf("Spline approximations are tested according to the ");
        if (fix==0) printf("Durbin-Watson test\n");
        else        printf("Chi-square test\n");
        if (fix==0 && njan!=1)
                printf("Correlation test spacing = %d datapoints\n", njan);
        printf("...........................................................\n");
        }

        /* begin step 1: equal-information splines, increasing l */

        lnew = 1;

        /* v5.0: return point for hyperspline extension */
        lbegin1:
//  Formation of test, lnew
        test = 0.0;
        while (test < REJLEV && lnew <= lopt)
                {
                l2sub(&lnew,0,1);
                ln = dummy = UPFACTOR*lnew;
                ln = TRUNC_SPEC(ln,dummy);
                if (ln > lopt && lnew < lopt) ln = lopt;
                if (ln == lnew) lnew += 1; else lnew = ln;
                if (lind == 1) test = 0.0;
                else test = (fix == 0) ? ltest : ctest;
                }
        nl2sub = il2sub;
        lbeg = iresul[nl2sub][1];

        /* begin step 2: knot-optimized splines, decreasing l */

        lnew = dummy = DOWNFACTOR*lbeg;
        lnew = TRUNC_SPEC(lnew,dummy);
        while (lnew > 1) 
                {
                l2sub(&lnew,1,1);
                lnew = dummy = DOWNFACTOR*lnew;
                lnew = TRUNC_SPEC(lnew,dummy);
                }
        if (lbeg > 1)
                {
                il2sub++;
                for (j = 1; j <= 2; j++) iresul[il2sub][j] = iresul[1][j];
                for (j = 1; j <= 5; j++) resul[il2sub][j] = resul[1][j];
                }

        /* end step 2 */

        Accept = 'n';
        gragain = 'y';
        grend = 0; 
        while (Accept == 'n')
                {
                /*  error plot  */
                if (!silent) { if (gragain == 'y') graer(grend); }

                grend = 1;

                /* begin step 4 */

                if (!silent && !done) 
                  {
                  printf("'rms' means: Fit-estimated y-uncertainty");
                  if (fix==0) 
                    {
                    if (sigma==1) sprintf(rmsversion,".");
                    if (sigma==0) 
                    sprintf(rmsversion," / uncertainty given in input file.");
                    }
                  if (fix==1) 
                    {
                    if (rel==0 && fixval>0.0)
                      sprintf(rmsversion," / %g.", fixval);
                    else 
                      if (rel==0 && fixval==0.0)
                      sprintf(rmsversion," / uncertainty given in input file.");
                      else
                      sprintf(rmsversion," / uncertainty via command line.");
                    }
                  printf("%s\n", rmsversion);
        printf("...........................................................\n");
                  printf("Suggestion(s) for a good spline-fit:\n");
                  }

                wind = nn = 0;
                if (fix == 0)
                        {
                        /* check results so far, first from the conservative
                           viewpoint (j=5), and if no acceptable spline
                           can be found, also from the liberal viewpoint
                           (j=4).
                        */
                        for (j = 5; j >= 4 && nn == 0; j--)
                                {
                                if (j == 4) wind = 1;
                                /* only the most recent decreasing-l series */
                                for (i = nl2sub; i < il2sub; i++)
                                    {
                                    /* suggest spline i when
                                       no singularities were found AND
                                       spline i is acceptable AND
                                       spline i+1 is not acceptable
                                    */
                                    if (resul[i][1] > 0.0 && 
                                        resul[i][j] > REJLEV && 
                                        resul[i+1][j] < REJLEV)
                                        {
                                        if (!silent && !done) 
                                           printf("rms = %g, dws = %g: l=%d\n", 
                                           resul[i][1], resul[i][3], 
                                           iresul[i][1]);
                                        isug = i;
                                        nn++;
                                        }
                                    }
                                if (nl2sub == 1 && resul[i][j] > REJLEV)
                                    {
                                    if (!silent && !done)
                                      printf("rms = %g, dws = %g: l=1\n", 
                                      resul[il2sub][1], resul[il2sub][3]);
                                    isug = il2sub;
                                    nn++;
                                    }
                                }
                        }
                else /* fix == 1 */
                        {
                        for (i = nl2sub; i < il2sub; i++)
                            {
                            if (resul[i][1] > 0.0 && 
                                resul[i][2] > REJLEV && 
                                resul[i+1][2] < REJLEV)
                                {
                                if (!silent && !done)
                                    printf("rms = %g, dws = %g: l=%d\n", 
                                    resul[i][1], resul[i][3], iresul[i][1]);
                                isug = i;
                                nn++;
                                }
                            }
                        if (nl2sub == 1)
                            {
                            if (!silent && !done)
                                printf("rms = %g, dws = %g: l=1\n", 
                                resul[il2sub][1], resul[il2sub][3]);
                            isug = il2sub;
                            nn++;
                            }
                        }

                if (nn == 0)
                        {
                        if (!silent && !done) 
                            {
                            printf("No really good spline can be found...");
                            printf(" You'll have to live with this one:\n");
                            }
                        rmmin = resul[nl2sub][1];
                        j = nl2sub;
                        for (i = nl2sub; i <= il2sub; i++)
                            {
                            if (resul[i][1] < rmmin)
                                {
                                rmmin = resul[i][1];
                                j = i;
                                }
                            }
                        if (!silent && !done)
                            printf("rms = %g, dws = %g: l=%d\n", resul[j][1],
                            resul[j][3], iresul[j][1]);
                        isug = j;
                        }
if (zero_search) done=1;
                if (wind == 1 && !done) 
                    {
                    if (!silent)
printf("Mild warning: all suggested fits are in the indecisive D-W area\n");
                    /* v5.0: hyperspline extension starts here */
                    if (lind != 1) 
                        {
                        lopt = dummy = UPFACTOR*lbeg;
                        lopt = TRUNC_SPEC(lopt,dummy);
                        if (lopt==lbeg) lopt++;
                        if (!silent)
printf("Little success: now increasing lrev from %d to %d\n", lbeg, lopt);
                        lnew = lopt;
                        goto lbegin1;
                        }
                    }

                if (!silent) 
                        {
                        if (!done)
printf("Non-optimized breakpoints will be used for l=%d and above\n", lbeg);
printf("Choose the number of intervals for the spline \
('s' for suggested)? l=");
                        if (scanf("%d", &lfin)==0) lfin=iresul[isug][1];
                        }
                else 
                        {
                        lfin = iresul[isug][1];
                        fprintf(stderr,"Spline-fit: rms=%g, dws=%g, l=%d (%s) ",
                        resul[isug][1], resul[isug][3], iresul[isug][1],
                        (iresul[isug][1]>=lbeg) ? "eqd" : "opt");
                        if (nn == 0) fprintf(stderr,"(no good fit) ");
                        if (wind == 1) fprintf(stderr,"(DW indecisive) ");
                        fprintf(stderr,"\n");
                        }

                /* end step 4, begin step 5 */

                if (lfin > lbeg) 
                        l2sub(&lfin,0,0);
                else
                        {
                        l2sub(&lbeg,0,0);
                        lnew = dummy = DOWNFACTOR*lbeg;
                        lnew = TRUNC_SPEC(lnew,dummy);
                        while (lnew >= lfin) 
                                {
                                l2sub(&lnew,1,0);
                                lnew = dummy = DOWNFACTOR*lnew;
                                lnew = TRUNC_SPEC(lnew,dummy);
                                }
                        if (l > lfin) l2sub(&lfin,1,0);
                        }

                if (!silent) grasp();

                /* end step 5, begin step 6 */

                if (!silent) 
                        {
                        scanf("%c", &dum);
                        if (Accept == 'n') 
                            {
                /*
                printf("\n\n...Do you want to see the rms-plot again (y/n) ? ");
                scanf("%c", &gragain);
                */
                            }
                        }
                else 
                        {
                        Accept = 'y';
                        gragain = 'n';
                        }
                }
        
        /*---WRITE TO OUTPUT FILE(S)---*/
        l2err(1,0);
        fprintf(fps, "%d %d\n", k, l); 
        for (i = 1; i <= l+1; i++) fprintf(fps, "%g\n", brek[i]);
        for (i = 1; i <= l+km1; i++) fprintf(fps, "%g\n", bcoef[i]);

        if (silent) 
            {
            fprintf(stdout, "%d %d\n", k, l); 
            for (i = 1; i <= l+1; i++) fprintf(stdout, "%g\n", brek[i]);
            for (i = 1; i <= l+km1; i++) fprintf(stdout, "%g\n", bcoef[i]);
            }

        /* v5.0 try to write spline statistics to file in a more or
           less organized way, so that plotting becomes simple.
           (suspected bug: maybe this does not work so nicely when the
           user searches 'manually')
        */
        if (fullon) 
            {
            ldone = 0;
            /* 1st pass to splstat: 
            only equi-information splines (increasing l) */
            for (i = 1; i <= il2sub; i++)
                {
                if (iresul[i][1] > ldone) 
                         {
                         fprintf(fpr, " %d %d %g", 
                                iresul[i][1], iresul[i][2], resul[i][1]);
                         if (fix == 1) fprintf(fpr, " %g", resul[i][2]);
                         fprintf(fpr, " %g %g %g\n", 
                                resul[i][3], resul[i][4], resul[i][5]);
                         ldone = iresul[i][1];
                         /* mark all but the highest of a series */
                         if (i<il2sub && iresul[i+1][1] > iresul[i][1])
                                iresul[i][1] = 0; 
                         }
                }
            /* 2nd pass to splstat: all newnot splines (decreasing l) */
            for (i = 1; i <= il2sub; i++)
                {
                if (iresul[i][1] != 0) 
                        {
                        fprintf(fpr, " %d %d %g", 
                                iresul[i][1], iresul[i][2], resul[i][1]);
                        if (fix == 1) fprintf(fpr, " %g", resul[i][2]);
                        fprintf(fpr, " %g %g %g\n", 
                                resul[i][3], resul[i][4], resul[i][5]);
                        }
                }
            /* 3rd pass to splstat: final spline */
            fprintf(fpr, " %d -999 %g", lfin, rms);
            if (fix == 1) fprintf(fpr, " -999");
            fprintf(fpr, " %g -999 -999\n", dws);
            }


        /* ---EXIT--- */
        fclose(fps);
        if (fullon) {fclose(fpr); fclose(fpp);}
        if (!silent) 
                {
printf("=====================================================================\n");
printf("Contents of output file(s):\n");
printf("'splrep' : order(k), #intvls(l), l+1 brkpnts, l+k-1 spline coefficients\n");
                if (fullon) {
printf("'splstat': l, deg.free, rms [, P(rms)], dws, Plow(dws), Pup(dws)\n");
                if (!logtrans)
printf("'splres' : x, y, S(x), d, sigma^-2, S'(x), S''(x)\n");
                else
printf("'splres' : log(x), y, S(x), d, sigma^-2, S'(x), S''(x)\n");
                }
printf("=====================================================================\n");
printf("To use the spline S(x), run   'evalsp splrep [options...]'\n");
printf("                               -> file 'evlres': x, S(x), S'(x), S''(x)\n");
printf("                               By default: in 250 equidist. x-values\n");
printf("options: [-x xbeg xend xstep]              in these equidist. x-values\n");
printf("         [-f xfile]                        in x-values listed in xfile\n");
printf("         [-e]                  Show maxima and minima of S(x)\n");
printf("         [-i]                  Show inflection points of S(x)\n");
printf("         [-v val]              Show x-value at which S(x)=value\n");
printf("         [-a]                  Show integral of S(x)\n");
printf("         [-h [-b val]]         Show points at half-height of peak\n");
printf("                               (-b forces the background at val)\n");
printf("         [-F sbeg send sstep]  Fourier transform of S(x)\n");
printf("                               -> file 'evlfour': s, Re(FT), Im(FT)\n");
printf("=====================================================================\n");
                }

        exit(0);

        } /* --end main-- */


/*---------------------------------------------------------------------------*/
/*                              Functions                                    */
/*---------------------------------------------------------------------------*/
//int l2sub(iptr,iopt,wr) int *iptr, iopt, wr;
void l2sub(int *iptr,int iopt,int wr)
         
        /*      Calculates spline approximation to the data.
         *
         *      input:
         *         *iptr   = number of intervals;
         *          iopt   = 0: intervals will contain equal numbers of 
         *                      data points,
         *                 = 1: optimize breakpoint locations with respect 
         *                      to the previous locations;
         *          wr     = 0: nothing is written to iresul[] and resul[],
         *                 = 1: results are written to iresul[] and resul[].
         *
         *      output:
         *         iok = 1 if globall Powell test is ok,
         *             = 0 otherwise,
         *         ipowt = sum of local Powell numbers, 
         *         rms = weighted rms error for fit, should be equal to 
         *               about 1.0 for well estimated data uncertainty,
         *         nsing = number of singularities found while solving the
         *                 normal equations,
         *         ipo   = number of intervals where local powell test fails,
         *         brek = the new breakpoint sequence,
         *         coef = the matrix of the (i-1)th derivatives (to the right)
         *                in the jth breakpoint,
         *         ftau = the spline at the data points,
         *         q = normalized errors at the data points.
         */
{
        int     ip;
        double  coefg[3][LMAX], p[KMAX][KMAX+LMAX], scrtch[LMAX+KMAX],
                t[LMAX+KMAX], chi;                              
        ip = *iptr;
        if (iopt == 0) ip = -ip;
        newknt(brek,coef,&l,&k,scrtch,&ip,coefg);
        l2knts(scrtch,&ip,&k,t,&n);
        l2appr(t,&n,&k,p,scrtch,bcoef);
//        bsplpp(t,bcoef,&n,&k,p,brek,coef,&l);
        bsplpp(t,bcoef,&n,&k,p,brek,coef,&l);
        l2err(0,1);
        dwtest(dws,&ltest,&utest);
        if (fix == 1)
        {
                chi = rms*rms*freed;
                ctest = chitest(chi,freed);
        }
        if (wr == 1)
        {
                il2sub++;
                iresul[il2sub][1] = ip;
                iresul[il2sub][2] = freed;
                if (il2sub == 1) 
                {
                        if (fix == 0) rmsmin = rms;
                        else
                        {
                                rmsmin = 1.;
                                rmsmax = 5.;
                        }
                }
                if (rms < rmsmin) rmsmin = rms;
                if (rms > rmsmax) rmsmax = rms;
                if (nsing > 0) rms *= -1;
                resul[il2sub][1] = rms;
                if (fix == 1) resul[il2sub][2] = ctest;
                else resul[il2sub][2] = 0;
                resul[il2sub][3] = dws;
                resul[il2sub][4] = ltest;
                resul[il2sub][5] = utest;
        }
}

/*---------------------------------------------------------------------------*/
//int newknt(ara,dara,iptr,jptr,arb,kptr,darb)
//        double  ara[], arb[], dara[][LMAX], darb[][LMAX];
//        int     *iptr, *jptr, *kptr;
void newknt(double ara[],double dara[][LMAX],int *iptr,int *jptr,
           double arb[],int *kptr,double darb[][LMAX])

        /*  calculates new ("best") set of breakpoints, no intervals with
         *  less then two data-points will be made
         */
{
        int     ip, jp, kp, i, ii, j, lleft, mflag, iend, idif;
        double  a, oneovk, dif, difprv, step, stepi;            
        ip = *iptr;
        jp = *jptr;
        kp = *kptr;

        arb[1] = tau[1];
        arb[abs(kp)+1] = tau[ntau];
        if(kp < 0)
        {
                kp = -1*kp;
                /*$dir no_recurrence*/
                for (i = 2; i <= kp; i++)
                {
                        a = ((double)(i-1))*(ntau-1)/kp+1;
                        ii = a;
                        arb[i] = tau[ii]+(a-ii)*(tau[ii+1]-tau[ii]);
                }
                *kptr = kp;
        }
        else
        {
                oneovk = 1./jp;
                darb[1][1] = 0.;
                difprv = fabs((dara[jp][2]-dara[jp][1])/(ara[3]-ara[1]));
                for (i = 2; i <= ip; i++)
                {
        dif = fabs((dara[jp][i]-dara[jp][i-1])/(ara[i+1]-ara[i-1]));
                        darb[2][i-1] = pow((dif+difprv),oneovk);
                darb[1][i] = darb[1][i-1]+darb[2][i-1]*(ara[i]-ara[i-1]);
                        difprv = dif;
                }
                darb[2][ip] = pow((2*difprv),oneovk);
step = (darb[1][ip]+darb[2][ip]*(ara[ip+1]-ara[ip]))/kp;
                if (step > 0.)
                {
                        j = 1;
                        for (i = 2; i <= kp; i++)
                        {
                                stepi = (i-1)*step;
                                while (j != ip && stepi > darb[1][j+1]) j++;
                                if (darb[2][j] != 0)
                                {
                                arb[i] = ara[j]+(stepi-darb[1][j])/darb[2][j];
                                }
                                else arb[i] = (ara[j]+ara[j+1])/2;
                        }
                }
                else
                {
                        step = (tau[ntau]-tau[1])/kp;
                        /*$dir no_recurrence*/
                        for (i = 2; i <= kp; i++)
                        {
                                arb[i] = tau[i]+(i-1)*step;
                        }
                }
                lleft = 1;
                iend = idif = 0;
                for (i = 2; i <= kp+1; i++)
                {
                        interv(tau,&ntau,&arb[i],&left,&mflag);
                        if (left-lleft > 0) lleft = left;
                        else
                        {
                                lleft += 1;
                                if (lleft < ntau) 
                                {
                                        arb[i] = (tau[lleft]+tau[lleft+1])/2.;
                                }
                                else
                                {
                                        iend++;
                                        arb[i] = tau[ntau];
                                }
                        }
                }
                if (iend != 0)
                {
                        i = 1;
                        lleft = ntau-1;
                        while (idif < 1)
                        {
                        interv(tau,&ntau,&arb[kp+1-i],&left,&mflag);
                                idif = lleft-left;
                                if (idif < 1)
                                {
                                        lleft -= 1;
                                arb[kp+1-i] = (tau[lleft]+tau[lleft+1])/2.;
                                        i++;
                                }
                        }
                }
        }
} 

/*---------------------------------------------------------------------------*/
//int l2knts(ara,iptr,jptr,arb,kptr)
//        int     *iptr, *jptr, *kptr;
//        double  *ara, *arb;  
void l2knts(double *ara,int *iptr,int *jptr,double *arb,int *kptr)

        /*  breakpoints to knots  */
{
        int     ip, jp, kp, i, kk;
        kk = km1;
        /*$dir no_recurrence*/
        for (i = 1; i <= kk; i++) arb[i] = ara[1];
        ip = *iptr;
        jp = *jptr;
        /*$dir no_recurrence*/
        for (i = 1; i <= ip; i++) arb[km1+i] = ara[i];
        kp = km1+ip;
        /*$dir no_recurrence*/
        for (i = 1; i <= jp; i++) arb[kp+i] = ara[ip+1];
        *kptr = kp;
}

/*---------------------------------------------------------------------------*/
//int l2appr(ara,iptr,jptr,dara,arb,arc)
//        int     *iptr, *jptr;
//        double  ara[], arb[], arc[], dara[][LMAX+KMAX];
void l2appr(double ara[],int *iptr,int *jptr,
           double dara[][LMAX+KMAX],double arb[],double arc[])

        /*  calculates spline approximation to given data (tau,gtau)  */
{
        int     ip, jp, j, i, ll, mm, jj;
        double  dw, biatx[KMAX];
        ip = *iptr;
        jp = *jptr;
        ll = jp;
        for (j = 1; j <= ip; j++)
        {
                arc[j] = 0.;
                for (i = 1; i <= ll; i++) dara[i][j] = 0.;
        }
        left = jp;
        for (ll = 1; ll <= ntau; ll++)
        {
                while (left != ip && tau[ll] >= ara[left+1]) left++;
                bsplvb(ara,jp,1,&tau[ll],&left,biatx);
                for(mm = 1; mm <= jp; mm++)
                {
                        dw = biatx[mm]*weight[ll];
                        j = left-jp+mm;
                        arc[j] += dw*gtau[ll];
                        i = 1;
                        for (jj = mm; jj <= jp; jj++)
                        {
                                dara[i][j] += biatx[jj]*dw;
                                i++;
                        }
                }
        }
        bchfac(dara,&jp,&ip,arb);
        bchslv(dara,&jp,&ip,arc);
}

/*---------------------------------------------------------------------------*/
//int bchfac(dara,iptr,jptr,ara)
//        int     *iptr, *jptr;
//        double  ara[], dara[][LMAX+KMAX];
void bchfac(double dara[][LMAX+KMAX],int *iptr,int *jptr,double ara[])

        /*  constructs cholesky factorization  */
{
        int     ip, jp, i, j, imax, jmax, ii;
        double  ratio;
        ip = *iptr;
        jp = *jptr;
        nsing = 0;
        if (jp <= 1 && dara[1][1] > 0) dara[1][1] = 1/dara[1][1];
        else
        {
                /*$dir no_recurrence*/
                for (ii = 1; ii <= jp; ii++) ara[ii] = dara[1][ii];
                for (ii = 1; ii <= jp; ii++)
                {
                        if (dara[1][ii]+ara[ii] <= ara[ii])
                        {
                                nsing++;
                                for(j = 1; j <= ip; j++) dara[j][ii] = 0.;
                        }
                        else
                        {
                                dara[1][ii] = 1/dara[1][ii];
                                if (ip-1 < jp-ii) imax = ip-1;
                                else imax = jp-ii;
                                if (imax > 0)
                                {
                                        jmax = imax;
                                        for(i = 1; i <= imax; i++)
                                        {
                                        ratio = dara[i+1][ii]*dara[1][ii];
                                                /*$dir no_recurrence*/
                                                for(j = 1; j <= jmax; j++)
                                                {
                                        dara[j][ii+i] -= dara[j+i][ii]*ratio;
                                                }
                                                jmax--;
                                                dara[i+1][ii] = ratio;
                                        }
                                }
                        }
                }
        }
}

/*---------------------------------------------------------------------------*/
//int bchslv(dara,iptr,jptr,ara)
//        int *iptr, *jptr;
//        double  ara[], dara[][LMAX+KMAX];
void bchslv(double dara[][LMAX+KMAX],int *iptr,int *jptr,double ara[])

        /* solves a banded positive definite set of equations */
{
        int     ip, jp, kk, jmax, j, ii;
        double  nbndm1;
        ip = *iptr;
        jp = *jptr;
        if (jp <= 1) ara[1] *= dara[1][1];
        else
        {
                nbndm1 = ip-1;
                kk = jp;
                for(ii = 1; ii <= kk; ii++)
                {
                        if (nbndm1 < jp-ii) jmax = nbndm1;
                        else jmax = jp-ii;
                        /*$dir no_recurrence*/
                        if (jmax > 0) for (j = 1; j <= jmax; j++) 
                        {
                                ara[j+ii] -= dara[j+1][ii]*ara[ii];
                        }
                }
                for (ii = jp; ii >= 1; ii--)
                {
                        ara[ii] *= dara[1][ii];
                        if (nbndm1 < jp-ii) jmax = nbndm1;
                        else jmax = jp-ii;
                        if (jmax > 0) for (j = 1; j <= jmax; j++)
                        {
                                ara[ii] -= dara[j+1][ii]*ara[j+ii];
                        }
                }
        }
}

/*---------------------------------------------------------------------------*/
//int bsplpp(ara,arb,iptr,jptr,dara,arc,darb,kptr)
//        int     *iptr, *jptr, *kptr;
//        double  ara[], arb[], arc[], dara[][KMAX], darb[][LMAX];
void bsplpp(double ara[],double arb[],int *iptr,int *jptr,
           double dara[][KMAX+LMAX],double arc[],double darb[][LMAX],int *kptr)

        /*  converts spline to piecewise polynomial representation  */
{
        int     ip, jp, lsofar, j, i, jp1, kmj;
        double  diff, sum, biatx[KMAX];
        ip = *iptr;
        jp = *jptr;
        arc[1] = ara[jp];
        lsofar = 0;
        for (left = jp; left <= ip; left++)
        {
                if(ara[left+1] != ara[left])
                {
                        lsofar++;
                        arc[lsofar+1] = ara[left+1];
                        if (jp <= 1) darb[1][lsofar] = arb[left];
                        else
                        {
                                /*$dir no_recurrence*/
                                for (i = 1; i <= jp; i++)
                                {
                                        dara[i][1] = arb[left-jp+i];
                                }
                                for (jp1 = 2; jp1 <= jp; jp1++)
                                {
                                        j = jp1-1;
                                        kmj = k-j;
                                        /*$dir no_recurrence*/
                                        for(i = 1; i <= kmj; i++)
                                        {
                                        diff = ara[left+i]-ara[left+i-kmj];
                                                if (diff > 0.)
                                                {
                        dara[i][jp1] = ((dara[i+1][j]-dara[i][j])/diff)*kmj;
                                                }
                                        }
                                }
                                bsplvb(ara,1,1,&ara[left],&left,biatx);
                                darb[jp][lsofar] = dara[1][jp];
                                for(jp1 = 2; jp1 <= jp; jp1++)
                                {
                                bsplvb(ara,jp1,2,&ara[left],&left,biatx);
                                        kmj = k+1-jp1;
                                        sum = 0.;
                                        for(i = 1; i <=jp1; i++)
                                        {
                                                sum += biatx[i]*dara[i][kmj];
                                                darb[kmj][lsofar] = sum;
                                        }
                                }
                        }
                }
        }
        *kptr = lsofar;
}

/*---------------------------------------------------------------------------*/
//int bsplvb(ara,jhigh,index,xptr,iptr,arb)
//        int     jhigh, index, *iptr;
//        double  ara[], arb[], *xptr;
void bsplvb(double ara[],int jhigh,int index,double *xptr,int *iptr,double arb[])

        /*  calculates all nonzero beta-splines at *xptr  */
{
        int     ip, jp1, i;
        double  xp, saved, term;
        ip = *iptr;
        xp = *xptr;
        if (index == 1)
        {
                jbsp = 1;
                arb[1] = 1.;
        }
        while (jbsp < jhigh)
        {
                jp1 = jbsp+1;
                deltar[jbsp] = ara[ip+jbsp]-xp;
                deltal[jbsp] = xp-ara[ip+1-jbsp];
                saved = 0.;
                for (i = 1; i <= jbsp; i++)
                {
                        term = arb[i]/(deltar[i]+deltal[jp1-i]);
                        arb[i] = saved+deltar[i]*term;
                        saved = deltal[jp1-i]*term;
                }
                arb[jp1] = saved;
                jbsp++;
        }
}

/*---------------------------------------------------------------------------*/
//int l2err(prfun,grfun)  int prfun, grfun;
void l2err(int prfun,int grfun)  

        /*  calculates and prints details of spline fit;  
         *
         *  input:
         *     prfun = 0: no final results written to splres,
         *             1: final results written to splres;
         *     grfun = 0: no first derivative calculated for plot,
         *           = 1: first derivative calculated for plot.
         */
{
        int     ll; 
        double  err, errl2, errdw, a, dtau, ddtau;
        errl2 = errdw = dermax =  0.;
        for (ll = 1; ll <= ntau; ll++)
        {
                ftau[ll] = ppvalu(brek,coef,&l,&k,&tau[ll],0);
                q[ll] = (ftau[ll]-gtau[ll])*sqrt(weight[ll]);
                if (grfun == 1 && k != 1) 
                {
                        deriv[ll] = ppvalu(brek,coef,&l,&k,&tau[ll],1);
                        if (fabs(deriv[ll]) > dermax) dermax = fabs(deriv[ll]);
                }
                err = q[ll];
                errl2 += err*err;
                if (ll > njan) errdw += (err-q[ll-njan])*(err-q[ll-njan]);
        }
        freed = ntau-km1-l;
        rms = sqrt((errl2/freed));
        /* new, 9 jan 1996: scale up errdw when njan > 1, to compensate
           for fewer terms in the sum than ntau-1 */
        /* dws = errdw/errl2; */
        dws = errdw/errl2 * (ntau-1.0)/(ntau-njan);
        if (prfun == 1)
        {
                for (ll = 1; ll <= ntau; ll++)
                {
                        a = ntau*weight[ll]/totalw; /* why this variable? */
                        dtau = ppvalu(brek,coef,&l,&k,&tau[ll],1);
                        ddtau = ppvalu(brek,coef,&l,&k,&tau[ll],2);
                        if (fullon) {
        /* fprintf(fpp, " %g %g %g %g %g %g %g\n",
                tau[ll], gtau[ll], ftau[ll], q[ll], a, dtau, ddtau); */
        /* 22-6-1996: */
        fprintf(fpp, " %g %g %g %g %g %g %g\n", tau[ll], gtau[ll], ftau[ll], q[ll], weight[ll], dtau, ddtau); 
        }
                }
        }
}

/*---------------------------------------------------------------------------*/
//double ppvalu(ara,dara,iptr,jptr,xptr,jderiv)
//        int     *iptr, *jptr, jderiv;
//        double  *xptr, ara[], dara[][LMAX];
double ppvalu(double ara[],double dara[][LMAX],int *iptr,
              int *jptr,double *xptr,int jderiv)

        /*  evaluates the jderiv-th derivative of a pp-function  */
{
        int     ip, jp, fmmjdr, i, ndummy, m;
        double  xp, h;
        ip = *iptr;
        jp = *jptr;
        xp = *xptr;
        ppvalue = 0.;
        fmmjdr = jp-jderiv;
        if (fmmjdr > 0)
        {
                interv(ara,&ip,&xp,&i,&ndummy);
                h = xp-ara[i];
                for (m = jp; m >= jderiv+1; m--)
                {
                        ppvalue = (ppvalue/fmmjdr)*h+dara[m][i];
                        fmmjdr--;
                }
        }
        return(ppvalue);
}

/*---------------------------------------------------------------------------*/
//int interv(ara,iptr,xptr,jptr,kptr)
//        int     *iptr, *jptr, *kptr;
//        double  *xptr, ara[];
void interv(double ara[],int *iptr,double *xptr,int *jptr,int *kptr)

        /*  locates a point within an increasing sequence of points  */
{
        int     ip, jp, kp, istep, middle, ilos=0;
        double  xp;
        ip = *iptr;
        xp = *xptr;
        kp = 10;
        ihi = ilo+1;
        if (ihi >= ip)
        {
                if (xp >= ara[ip]) kp = 1;
                else
                {
                        if (ip <= 1) kp = -1;
                        else
                        {
                                ilo = ip-1;
                                ihi = ip;
                        }
                }
        }
        if (kp == 10)
        {
                if (xp < ara[ihi])
                {
                        if (xp >= ara[ilo]) kp = 0;
                        else
                        {
                                istep = 1;
                                while (ilo > 1 && xp < ara[ilo])
                                {
                                        ihi = ilo;
                                        ilo = ihi-istep;
                                        istep *= 2;
                                }
                                if (ilo <= 1)
                                {
                                        ilo = 1;
                                        if (xp < ara[1]) kp = -1;
                                }
                        }
                }
                else
                {
                        istep = 1;
                        while (ihi < ip && xp > ara[ihi])
                        {
                                ilo = ihi;
                                ihi = ilo+istep;
                                istep *= 2;
                        }
                        if (ihi >= ip)
                        {
                                ihi = ip;
                                if (xp > ara[ip]) kp = 1;
                        }
                }
                if (kp == 10)
                {
                        do
                        {
                                middle = (ilo+ihi)/2;
                                if (xp >= ara[middle])
                                {
                                        ilos = ilo;
                                        ilo = middle;
                                }
                                else ihi = middle;
                        }
                        while (middle != ilos);
                }
        }
        if (kp == -1) jp = 1;
        else
        {
                if (kp == 1) jp = ip;
                else
                {
                        kp = 0;
                        jp = ilo;
                }
        }
        *jptr = jp;
        *kptr = kp;
}

/*---------------------------------------------------------------------------*/
//int dwtest(x,xptr,yptr) double  x, *xptr, *yptr;
void dwtest(double x,double *xptr,double *yptr) 

        /* calculates percentages of lower (*xptr) and upper (*yptr) 
         * percentages of x
         */
{
        int     i;
        double  y, pin, ci, sc, scc, aa, pl, pu, xp, yp;
        pin = 4*atan(1.)/ntau;
        sc = scc = 0;
        for (i = 1; i < ntau-freed; i++)
        {
                ci = cos(pin*i);
                sc += ci;
                scc += ci*ci;
        }
        sc = sc/freed;
        aa = (double) freed*(freed+2)*(1-sc*sc)/(ntau-2-2*scc-2*freed*sc*sc)-1;
        pl = aa*(1-sc)/2;
        pu = aa*(1+sc)/2;
        y = x/4;
        xp = betai(pl,pu,y);
        yp = betai(pu,pl,y);
        *xptr = xp;
        *yptr = yp;
}

/*---------------------------------------------------------------------------*/
//double betai(a,b,x)     double a, b, x;
double lgamma(double a){return a;}
double betai(double a,double b,double x)

        /* calculates Ix(a,b)   */
{
        double  bt, betai;
        if (x == 0 || x == 1) bt = 0;
        else bt = exp(lgamma(a+b)-lgamma(a)-lgamma(b)+a*log(x)+b*log(1-x));
        if (x < (a+1)/(a+b+2)) betai = bt*betacf(a,b,x)/a;
        else betai = 1-bt*betacf(b,a,1-x)/b;
        return(betai);
}
        
/*---------------------------------------------------------------------------*/
//double betacf(a,b,x)    double  a, b, x;
double betacf(double a,double b,double x)

        /* continued fraction for Ix(a,b) */
{
        int     em, tem;
        double  aold, am, bm, az, qab, qap, qam, bz, d, ap, bp, app, bpp;
        aold = 0;
        em = 0;
        am = bm = az = 1;
        qab = a+b;
        qap = a+1;
        qam = a-1;
        bz = 1-qab*x/qap;
        while (fabs(az-aold) > 1e-7*fabs(az))
        {
                em++;
                tem = em+em;
                d = em*(b-em)*x/(qam+tem)/(a+tem);
                ap = az+d*am;
                bp = bz+d*bm;
                d = -(a+em)*(qab+em)*x/(a+tem)/(qap+tem);
                app = ap+d*az;
                bpp = bp+d*bz;
                aold = az;
                am = ap/bpp;
                bm = bp/bpp;
                az = app/bpp;
                bz = 1;
        }
        return(az);
}

/*---------------------------------------------------------------------------*/
//double chitest(chi,fr)
//        int     fr;
//        double  chi;
double chitest(double chi,int fr)
                
        /* calculates upper percentage points of chisquare of x with a degrees
           of freedom
         */
{
        double  a, x, gamser, gln, gammcf;
        a = (double) fr/2.;
        x = chi/2;
        if (x < a+1) 
        {
                gser(&gamser,a,x,&gln);
                gammcf = 1-gamser;
        }
        else gcf(&gammcf,a,x,&gln);
        return(gammcf);
}

/*---------------------------------------------------------------------------*/
//int gser(xptr,a,x,yptr) double  *xptr, a, x, *yptr;
void gser(double *xptr,double a,double x,double *yptr)

        /* returns the incomplete gamma function p(a,x) evaluated by its
           series representation as *xptr; also returns ln(gamma(a)) as 
           *yptr
         */
{
        double  xp, yp, ap, sum, del;
        yp = lgamma(a);
        ap = a;
        sum = 1/a;
        del = sum;
        while (fabs(del) > fabs(sum)*1e-7)
        {
                ap++;
                del *= x/ap;
                sum += del;
        }
        xp = sum*exp(-x+a*log(x)-yp);
        *xptr = xp;
        *yptr = yp;
}

/*---------------------------------------------------------------------------*/
//int gcf(xptr,a,x,yptr)  double  *xptr, a, x, *yptr;
void gcf(double *xptr,double a,double x,double *yptr)

        /* returns the incomplete gamma function q(a,x) evaluated by its 
           continued fraction representation as *xptr; also returns ln(gamma(a))
           as *yptr
         */
{
        int     n;
        double  xp, yp, gold, b0, g, a0, b1, fac, a1, ana, anf;
        yp = lgamma(a);
        gold = b0 = 0.;
        a0 = b1 = fac = g = 1.;
        a1 = x;
        n = 1;
        while (fabs((g-gold)/g) > 1e-7)
        {
                if (a1 != 0 && n != 1) gold = g;
                ana = n-a;
                a0 = (a1+a0*ana)*fac;
                b0 = (b1+b0*ana)*fac;
                anf = n*fac;
                a1 = x*a0+anf*a1;
                b1 = x*b0+anf*b1;
                if (a1 != 0)
                {
                        fac = 1/a1;
                        g = b1*fac;
                }
                n++;
        }
        xp = exp(-x+a*log(x)-yp)*g;
        *xptr = xp;
        *yptr = yp;
}

/*---------------------------------------------------------------------------*/
//int graer(ind)  int     ind;
void graer(int ind)
        /*   constructs log(rms) vs log(l) plot  */
{
/*
        int     i, j, xplot, yplot, ixmax, ymax, ymin;
        double  xmax, xmin, xrange, yrange;
        char    dum1, s, st[2], testtitle[50], xtitle[80], ytitle[80];
        openpl();
        erase();
        space(0,0,1000,1000);
        line(1050,200,50,200);
        cont(50,1000);
        cont(1050,1000);
        cont(1050,200);
        move(100,950);
        if (sigma==1)
        sprintf(ytitle,"log rms  [Estim.]");
        if (sigma==0 && (fix==0 || (fix==1 && fixval==0.0)))
        sprintf(ytitle,"log rms  [Estim./Specif.]");
        if (fix==1 && fixval>0.0)
        sprintf(ytitle,"log rms  [Estim./%g]", fixval);
        label(ytitle);
        move(250,230);
        strcpy(xtitle,"log l  [# of intervals]");
        label(xtitle);
        if (fix==1) strcpy(testtitle,"Chi2-test (rms<=1)");
        if (fix==0) strcpy(testtitle,"DW-test (dws>~=2)");
        move (700,950);
        label(testtitle);
        line(650,900,1000,900);
        cont(1000,1000);
        cont(650,1000);
        cont(650,900);
        xmax = log10((double)lbeg);
        if (xmax == 0) xmax = 1;
        xmin = 0.;
        ymax = ceil(log10(rmsmax));
        ymin = floor(log10(rmsmin));
        xrange = 1000/(xmax-xmin);
        yrange = 800./(ymax-ymin);
        ixmax = xmax;
        for (i = 0; i <= ixmax; i++)
        {
                xplot = 50+(i-xmin)*xrange;
                line(xplot,200,xplot,190);
                move(xplot,150);
                sprintf(&s, "%d", i);
                label(&s);
        }
        for (i = ymin; i <= ymax; i++)
        {
                yplot = 200+(i-ymin)*yrange;
                line(50,yplot,30,yplot);
                move(0,yplot);
                sprintf(st, "%d", i);
                label(st);
        }
        for (i = 1; i <= il2sub; i++)
        {
                xplot = 50+(log10((double)iresul[i][1])-xmin)*xrange;
                yplot = 200+(log10(fabs(resul[i][1]))-ymin)*yrange;
                if (i == 1) move(xplot,yplot);
                else cont(xplot,yplot);
                if (fix == 0) test = resul[i][4];
                else test = resul[i][2];
                if (test > REJLEV) 
                {
                        if (fix == 1 || resul[i][5] > REJLEV) 
                        {
                                circle(xplot,yplot,7);
                        }
                        else circle(xplot,yplot,5);
                        move(xplot,yplot);
                }
        }
        move(0,80);
        closepl();
        if (ind == 1) scanf("%c", &dum1);
        printf(" Hit return to continue...\n");
        scanf("%c", &dum1);
*/
}

/*---------------------------------------------------------------------------*/
void grasp()

        /*
         * constructs plot of spline, first derivatives, breakpoints 
         * and weighted errors
         *
         */
{
        int     xplot, yplot, xpl[NMAX], i, xpr, ypr, xd, yd, dist;
        double  xbegin, xend, xrange, ybegin, yend, yrange, aa;
        char    dum;
/*
        openpl();
        erase();
        space(0,0,1000,1000);
        line(50,1000,1050,1000);
        cont(1050,200);
        cont(50,200);
        cont(50,1000);
        line(50,300,1050,300);
        line(50,640,1050,640);
        xbegin = tau[1];
        xend = tau[ntau];
        xrange = 1000/(xend-xbegin);
        for (i = 2; i <= l; i++)
        {
                xplot = 50+(brek[i]-xbegin)*xrange;
                line(xplot,1000,xplot,980);
                line(xplot+1,1000,xplot+1,980);
        }
        yrange = taumax-taumin;
        ybegin = taumin-.05*yrange;
        yend = taumax+.05*yrange;
        yrange = 680/(yend-ybegin);
        for (i = 1; i <= ntau; i++)
        {
                xpl[i] = 50+(tau[i]-xbegin)*xrange;
                yplot = 300+(gtau[i]-ybegin)*yrange;
                point(xpl[i],yplot);
        }
        for (i = 1; i <= ntau; i++)
        {
                yplot = 300+(ftau[i]-ybegin)*yrange;
                if (i == 1) move(xpl[1],yplot);
                else cont(xpl[i],yplot);
        }
        if (k != 1)
        {
                yend = 1.05*dermax;
                ybegin = -yend;
                yrange = 680/2/yend;
                for (i = 1; i <= ntau; i++)
                {
                        yplot = 300+(deriv[i]-ybegin)*yrange;
                        if (i == 1) move(xpl[1],yplot);
                        else cont(xpl[i],yplot);
                }
        }
        yrange = 100./8;
        for (i = 1; i <= ntau; i++)
        {
                if (fix == 0) aa = q[i]/rms;
                else aa = q[i];
                if (aa > 4.) aa = 4.;
                if (aa < -4.) aa = -4.;
                yplot = 200+(aa+4)*yrange;
                if (i == 1) move(xpl[1],yplot);
                else cont(xpl[i],yplot);
        }
        move (0,180);
        closepl();
*/
        printf("rms = %g", rms);
        printf(", dws = %g.", dws);
        if (fix == 0) test = ltest;
        else test = ctest;
        if (test < REJLEV) printf(" <-- WARNING: not ok. ");
        else
        {
                if (fix == 0 && utest < REJLEV)
                {
                printf(" <-- Mild warning: indecisive. ");
                }
        }
        /* try out v5.0 */
        /* Code fragment moved to exit part of main */
        printf(" Done (y/n)? ");
        scanf("%c%c", &dum, &Accept);  
        done = 1;
}

#ifdef __cplusplus
	}
#endif
