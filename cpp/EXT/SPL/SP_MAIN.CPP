
/* ---INCLUDES--- */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h> 
//#include "sp_fun.h"

/* ---DEFINES (NUMBERS)--- */
#define NMAX 60000
#define LMAX 2001
#define KMAX 10

/* ---ANNOUNCE FUNCTIONS--- */ 
#ifdef __cplusplus
extern "C" {
#endif

void bsplpp(double ara[],double arb[],int *iptr,int *jptr,double dara[][KMAX],
           double arc[],double darb[][LMAX],int *kptr);
void bsplvb(double ara[],int jhigh,int index,double *xptr,int *iptr,double arb[]);
void interv(double ara[],int *iptr,double *xptr,int *jptr,int *kptr);
void l2knts(double *ara,int *iptr,int *jptr,double *arb,int *kptr);
double ppigr(double ara[],double dara[][LMAX],int *iptr,int *jptr,double *xptr,double *yptr);
double ppvalu(double ara[],double dara[][LMAX],int *iptr,int *jptr,
              double *xptr,int jderiv);
double zbrent(double x1,double x2,int ind,double value);
/*
int    l2knts(),
       bsplpp(),
       bsplvb(),
       interv();
double ppvalu(),
       ppigr(),
       zbrent();
*/
/* ---GLOBAL VARIABLES (are zero-initialized)--- */
int    k, km1, l, n, left, jbsp, ihi, ilo = 1, hhon, nextremes, nomaxmax=1;
int    firsttime=1, very_uncertain;
double brek[LMAX], bcoef[LMAX+KMAX], coef[KMAX][LMAX], ppvalue,
       deltal[KMAX], deltar[KMAX]; 
double halfheight, extrsum, maxmax, zerolevel;
FILE   *fpw, *fps, *fpf, *fpr, *fpx;



/* ---MAIN--- */
//main(argc,argv)  int argc; char *argv[];
void main(int argc,char *argv[])
        {
        /* ---DECLARATIONS (note: auto-arrays can't be initialized)--- */
        register int i, j;
        int          xflag, ntau, m, fileind, val, ext, inf, are, fou;
        int          background;
        double       xbegin, xend, xstep, y, x, t[LMAX+KMAX], b, bb, a[KMAX],
                     scrtch[KMAX][KMAX], c, a0, a1, a2, yder, ymax, step,
                     xv[NMAX], value, smin, smax, sstep, pi, s, fourr, fouri,
                     tpis, pref, argb, arga, bg;
        char         str[100];

        /* ---INITIALIZATION---*/
          fpr = stdin;
          xflag = fileind = val = ext = inf = are = fou = 0;
          background = 0;
        /* ---GET RUN TIME OPTION---*/
          if (argc > 1)
          {
              for (m=1; m<argc; m++)
              {
                  switch( *argv[m] )
                  {
                  case '-':
                      switch( *(argv[m]+1) )
                      {
                      case 'x' : xbegin = atof(argv[++m]);
                                 xend = atof(argv[++m]);
                                 xstep = atof(argv[++m]);
                                 xflag = 1;
                                 break;
                      case 'v' : val = 1;
                                 value = atof(argv[++m]);
                                 break;
                      case 'b' : background = 1;
                                 bg = atof(argv[++m]);
                                 break;
                      case 'e' : ext = 1;
                                 break;
                      case 'h' : ext = hhon = 1;
                                 break;
                      case 'i' : inf = 1;
                                 break;
                      case 'a' : are = 1;
                                 break;
                      case 'F' : fou = 1;
                                 smin = atof(argv[++m]);
                                 smax = atof(argv[++m]);
                                 sstep = atof(argv[++m]);
                                 break; 
                      case 'f' : fileind = xflag = 1;
                                 strcpy(str,argv[++m]);
                                 if ((fpx = fopen(str,"r")) == NULL)
                                 {
                                        printf("\n cannot open %s\n", str);
                                        exit(1);
                                 }
                                 break;
                      default : printf("\n Flag unknown: ignored.\n");
                                break;
                      }
                      break;
                  default : strcpy(str,argv[m]);                   
                            if ((fpr = fopen(str,"r")) == NULL)
                            {
                                printf("\n cannot open %s\n", str);
                                exit(1);
                            }
                  }
              }
          }


        /* ---OPEN FILE FOR WRITING---*/
          if ((fpw = fopen("evlres","w")) == NULL)
                  printf("\n....Cannot open writefile....\n");

        /* ---OPEN FILE FOR WRITING---*/
          if ((fps = fopen("evlsum","w")) == NULL)
                  printf("\n....Cannot open writefile....\n");

        /* ---OPEN FILE FOR WRITING---*/
          if ((fpf = fopen("evlfour","w")) == NULL)
                  printf("\n....Cannot open writefile....\n");

        /* ---PROCEED---*/
        fscanf(fpr, "%d %d", &k, &l);
        if (k <= 1)
        {
                /*
                printf(" spline order too low for option -v, ignored\n");
                */
                val = 0;
        }
        if (k <= 2)
        {
                /*
                printf(" spline order too low for option -e, ignored\n");
                */
                ext = 0;
        }
        if (k <= 3)
        {
                /*
                printf(" spline order too low for option -i, ignored\n");
                */
                inf = 0;
        }
        for (i = 1; i <= l+1; i++) fscanf(fpr, "%lf", brek+i);
        km1 = k-1;
        for (i = 1; i <= l+km1; i++) fscanf(fpr, "%lf", bcoef+i);
        if (xflag == 0)
        {
                xbegin = brek[1];
                xend = brek[l+1];
                xstep = (xend-xbegin)/250;
        }
        if (fileind == 1)
        {
                i = 1;
                while (fscanf(fpx, "%lf", xv+i) != EOF && i < NMAX-1) i++;
                ntau = i-1;
                xbegin = xv[1];
                xend = xv[ntau];
        }
        else ntau = (xend-xbegin)/xstep+1;
        l2knts(brek,&l,&k,t,&n);
        bsplpp(t,bcoef,&n,&k,scrtch,brek,coef,&l);

        relaunch:
        for (i = 1; i <= ntau; i++)
        {
                if (fileind == 1) b = xv[i];
                else b = xbegin+(i-1)*xstep;
                if (firsttime) fprintf(fpw, " %g", b);
                for (j = 0; j <= km1; j++)
                {
                        a[j] = ppvalu(brek,coef,&l,&k,&b,j);
                        if (firsttime) fprintf(fpw, " %g", a[j]);
                }
                if (firsttime) fprintf(fpw, "\n");
        if (val == 1 && i > 1 && (a[0]-value)*(a0-value) <= 0. && a0 != 0)
                {
                        if (a[0]-value == 0.) x = b;
                        else x = zbrent(bb,b,0,value);
                        if (firsttime) {
                        printf(" root...: x = %g\ty = %g\n", x, value);
                        fprintf(fps, " root...: x = %g\ty = %g\n", x, value);
                        }
                        else {
                        printf(" 1/2w.pt: x = %g\ty = %g\n", x, value);
                        fprintf(fps, " 1/2w.pt: x = %g\ty = %g\n", x, value);
                        }
                }
                if (ext == 1 && i > 1 && a[1]*a1 <= 0. && a1 != 0 && a[2] != 0)
                {
                        if (a[1] == 0.) x = b;
                        else x = zbrent(bb,b,1,0);
                        ymax = ppvalu(brek,coef,&l,&k,&x,0);
                        if (a1 > 0.)
                        {
                        nextremes++;
                        if (hhon) {
                                extrsum += ymax;
                                if (ymax>maxmax || nomaxmax) 
                                        {maxmax = ymax; nomaxmax=0;}
                                }
                        printf(" //MAX\\\\: x = %g\ty = %g\n", x, ymax);
                        fprintf(fps, " //MAX\\\\: x = %g\ty = %g\n", x, ymax);
                        }
                        else
                        {
                        nextremes++;
                        if (hhon) {
                                extrsum += ymax;
                                if (ymax>maxmax) maxmax = ymax;
                                }
                        printf(" \\\\min//: x = %g\ty = %g\n", x, ymax);
                        fprintf(fps, " \\\\min//: x = %g\ty = %g\n", x, ymax);
                        }
                }
                if (inf == 1 && i > 1 && a[2]*a2 <= 0. && a2 != 0 && a[3] != 0)
                {
                        if (a[2] == 0.) x = b;
                        else x = zbrent(bb,b,2,0);
                        ymax = ppvalu(brek,coef,&l,&k,&x,0);
                        yder = ppvalu(brek,coef,&l,&k,&x,1);
        if(yder>0.0){
        printf(" //ifp//: x = %g\ty = %g   y' = %g\n", x, ymax, yder);
        fprintf(fps, " //ifp//: x = %g\ty = %g   y' = %g\n", x, ymax, yder);
        }
        if(yder<0.0){
        printf(" \\\\ifp\\\\: x = %g\ty = %g   y' = %g\n", x, ymax, yder);
        fprintf(fps, " \\\\ifp\\\\: x = %g\ty = %g   y' = %g\n", x, ymax, yder);
        }
        if(yder==0.0){
        printf(" --ifp--: x = %g\ty = %g   y' = %g\n", x, ymax, yder);
        fprintf(fps, " --ifp--: x = %g\ty = %g   y' = %g\n", x, ymax, yder);
        }
                }
                a0 = a[0];
                a1 = a[1];
                a2 = a[2];
                bb = b;
        }

        if (are == 1)
        {
                y = ppigr(brek,coef,&l,&k,&xbegin,&xend);
        printf(" area...[ x = %g\tx = %g ]  A = %g\n", xbegin, xend, y);
        fprintf(fps, " area...[ x = %g\tx = %g ]  A = %g\n", xbegin, xend, y);
        }

        if (fou == 1)
        {
                pi = 4*atan(1.);
                for (s = smin; s <= smax; s += sstep)
                {
                        if (fabs(s) < 1e-6*sstep)
                        {
                                fourr = ppigr(brek,coef,&l,&k,&xbegin,&xend);
                                fouri = 0;
                        }
                        else
                        {
                                fourr = fouri = 0;
                                tpis = 2*pi*s;
                                pref = 1;
                                for (j = 1; j <= k; j++)
                                {
                                        pref /= tpis;   
                                        c = ppvalu(brek,coef,&l,&k,&xbegin,j-1);
                                        b = ppvalu(brek,coef,&l,&k,&xend,j-1);
                                        arga = tpis*xbegin+.5*pi*j;
                                        argb = tpis*xend+.5*pi*j;
                                        fourr += pref*(c*cos(arga)-b*cos(argb));
                                        fouri += pref*(b*sin(argb)-c*sin(arga));
                                }
                                for (i = 2; i <= l; i++)
                                {
                                if (brek[i] > xbegin && brek[i] <= xend);
                                        {
                                                argb = tpis*brek[i]+.5*pi*k;
                                                b = coef[k][i]-coef[k][i-1];
                                                fourr += pref*b*cos(argb);
                                                fouri -= pref*b*sin(argb);
                                        }
                                }
                        }
                        fprintf(fpf, "%g %g %g\n", s, fourr, fouri);
                }
        }

        if (ext) {
                printf(" xtremes: N = %d\n", nextremes);
                fprintf(fps," xtremes: N = %d\n", nextremes);
                }

        if (hhon) {
                if (nextremes<1 || nomaxmax) {
                        printf(" half-height points cannot be determined\n");
                        fprintf(fps,
                        " half-height points cannot be determined\n");
                        }
                else {
                        if (nextremes>1 && !background) {
                        zerolevel = (extrsum-maxmax)/(nextremes-1);
                        halfheight = zerolevel+(maxmax-zerolevel)/2.0;
                        }
                        else {
                        if (!background) {
                                zerolevel = 0.0;
                                halfheight = maxmax/2.0;
                                very_uncertain = 1;
                                }
                        else {
                                zerolevel = bg;
                                halfheight = zerolevel+(maxmax-zerolevel)/2.0;
                                }
                        }
        printf(" bckgrnd:\t\ty = %g (estimated level", zerolevel);
        if (very_uncertain) printf("; very uncertain");
        printf(")\n");
        fprintf(fps," bckgrnd:\t\ty = %g (estimated level", zerolevel);
        if (very_uncertain) fprintf(fps,"; very uncertain");
        fprintf(fps,")\n");
        printf(" 1/2hght:\t\ty = %g (estimated half-height", halfheight);
        if (very_uncertain) printf("; very uncertain");
        printf(")\n");
        fprintf(fps, " 1/2 hght:\t\ty = %g (estimated half-height", 
        halfheight);
        if (very_uncertain) fprintf(fps,"; very uncertain");
        fprintf(fps,")\n");
                        /* reset options */
                        ext = hhon = inf = are = fou = firsttime = 0;
                        val = 1;
                        value = halfheight;
                        goto relaunch;
                        }
                }

        /* ---CLOSE READFILE---*/
if (fpx)       fclose(fpx);

        /* ---CLOSE READFILE---*/
        fclose(fpr);

        /* ---CLOSE WRITEFILE---*/
        fclose(fps);

        /* ---CLOSE WRITEFILE---*/
        fclose(fpw);

        /* ---CLOSE WRITEFILE---*/
        fclose(fpf);

        /* ---EXIT--- */
        exit(0);
        } /* --end main-- */




/*------------------------------------------------------------------*/
//double ppigr(ara,dara,iptr,jptr,xptr,yptr)
//        int     *iptr, *jptr;
//        double  *xptr, *yptr, ara[], dara[][LMAX];
double ppigr(double ara[],double dara[][LMAX],int *iptr,int *jptr,double *xptr,double *yptr)

        /*  calculates integral from *xptr to *yptr  */
{
        int     i, j, right, ndummy;
        double  h, aa, bb, ppintgr;
        ppintgr = 0.;
        interv(ara,iptr,xptr,&left,&ndummy);
        h = *xptr-ara[left];
        bb = 0.;
        for (j = *jptr; j >= 1; j--) bb = (bb+dara[j][left])*h/j;
        interv(ara,iptr,yptr,&right,&ndummy);
        for (i = left; i < right; i++)
        {
                aa = 0.;
                h = ara[i+1]-ara[i];
                for (j = *jptr; j >= 1; j--)
                {
                        aa = (aa+dara[j][i])*h/j;
                }
                ppintgr += aa;
        }
        h = *yptr-ara[right];
        aa = 0.;
        for (j = *jptr; j >= 1; j--) aa = (aa+dara[j][right])*h/j;
        ppintgr = ppintgr+aa-bb;
        return(ppintgr);
}

/*------------------------------------------------------------------*/
//int l2knts(ara,iptr,jptr,arb,kptr)
//        int     *iptr, *jptr, *kptr;
//        double  *ara, *arb;  
void l2knts(double *ara,int *iptr,int *jptr,double *arb,int *kptr)

        /*  breakpoints to knots  */
{
        int     i;
        for (i = 1; i <= km1; i++) arb[i] = ara[1];
        for (i = 1; i <= *iptr; i++) arb[km1+i] = ara[i];
        n = km1+(*iptr);
        for (i = 1; i <= *jptr; i++) arb[*kptr+i] = ara[*iptr+1];
}

/*------------------------------------------------------------------*/
//int bsplpp(ara,arb,iptr,jptr,dara,arc,darb,kptr)
//        int     *iptr, *jptr, *kptr;
//        double  ara[], arb[], arc[], dara[][KMAX], darb[][LMAX];
void bsplpp(double ara[],double arb[],int *iptr,int *jptr,double dara[][KMAX],
           double arc[],double darb[][LMAX],int *kptr)

        /*  converts spline to piecewise polynomial representation  */
{
        int     lsofar, j, i, jp1, kmj;
        double  diff, sum, biatx[KMAX];
        arc[1] = ara[*jptr];
        lsofar = 0;
        for (left = *jptr; left <= *iptr; left++)
        {
                if(ara[left+1] != ara[left])
                {
                        lsofar++;
                        arc[lsofar+1] = ara[left+1];
                        if (*jptr <= 1) darb[1][lsofar] = arb[left];
                        else
                        {
                                for (i = 1; i <= *jptr; i++)
                                {
                                        dara[i][1] = arb[left-*jptr+i];
                                }
                                for (jp1 = 2; jp1 <= *jptr; jp1++)
                                {
                                        j = jp1-1;
                                        kmj = k-j;
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
                                darb[*jptr][lsofar] = dara[1][*jptr];
                                for(jp1 = 2; jp1 <= *jptr; jp1++)
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

/*------------------------------------------------------------------*/
//int bsplvb(ara,jhigh,index,xptr,iptr,arb)
//        int     jhigh, index, *iptr;
//        double  ara[], arb[], *xptr;
void bsplvb(double ara[],int jhigh,int index,double *xptr,int *iptr,double arb[])

        /*  calculates all nonzero beta-splines at *xptr  */
{
        int     jp1, i;
        double  saved, term;
        if (index == 1)
        {
                jbsp = 1;
                arb[1] = 1.;
        }
        while (jbsp < jhigh)
        {
                jp1 = jbsp+1;
                deltar[jbsp] = ara[*iptr+jbsp]-*xptr;
                deltal[jbsp] = (*xptr)-ara[*iptr+1-jbsp];
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

/*------------------------------------------------------------------*/
//double ppvalu(ara,dara,iptr,jptr,xptr,jderiv)
//        int     *iptr, *jptr, jderiv;
//        double  *xptr, ara[], dara[][LMAX];
double ppvalu(double ara[],double dara[][LMAX],int *iptr,int *jptr,
              double *xptr,int jderiv)

        /*  evaluates the jderiv-th derivative of a pp-function  */
{
        int     fmmjdr, i, ndummy, m;
        double  h;
        ppvalue = 0.;
        fmmjdr = *jptr-jderiv;
        if (fmmjdr > 0)
        {
                interv(ara,iptr,xptr,&i,&ndummy);
                h = *xptr-ara[i];
                for (m = *jptr; m >= jderiv+1; m--)
                {
                        ppvalue = (ppvalue/fmmjdr)*h+dara[m][i];
                        fmmjdr--;
                }
        }
        return(ppvalue);
}

/*------------------------------------------------------------------*/
//int interv(ara,iptr,xptr,jptr,kptr)
//        int     *iptr, *jptr, *kptr;
//        double  *xptr, ara[];
void interv(double ara[],int *iptr,double *xptr,int *jptr,int *kptr)

        /*  locates a point within an increasing sequence of points  */
{
        int     istep, middle, ilos=0;
        *kptr = 10;
        ihi = ilo+1;
        if (ihi >= *iptr)
        {
                if (*xptr >= ara[*iptr]) *kptr = 1;
                else
                {
                        if (*iptr <= 1) *kptr = -1;
                        else
                        {
                                ilo = *iptr-1;
                                ihi = *iptr;
                        }
                }
        }
        if (*kptr == 10)
        {
                if (*xptr < ara[ihi])
                {
                        if (*xptr >= ara[ilo]) *kptr = 0;
                        else
                        {
                                istep = 1;
                                while (ilo > 1 && *xptr < ara[ilo])
                                {
                                        ihi = ilo;
                                        ilo = ihi-istep;
                                        istep *= 2;
                                }
                                if (ilo <= 1)
                                {
                                        ilo = 1;
                                        if (*xptr < ara[1]) *kptr = -1;
                                }
                        }
                }
                else
                {
                        istep = 1;
                        while (ihi < *iptr && *xptr > ara[ihi])
                        {
                                ilo = ihi;
                                ihi = ilo+istep;
                                istep *= 2;
                        }
                        if (ihi >= *iptr)
                        {
                                ihi = *iptr;
                                if (*xptr > ara[*iptr]) *kptr = 1;
                        }
                }
                if (*kptr == 10)
                {
                        do
                        {
                                middle = (ilo+ihi)/2;
                                if (*xptr >= ara[middle])
                                {
                                        ilos = ilo;
                                        ilo = middle;
                                }
                                else ihi = middle;
                        }
                        while (middle != ilos);
                }
        }
        if (*kptr == -1) *jptr = 1;
        else
        {
                if (*kptr == 1) *jptr = *iptr;
                else
                {
                        *kptr = 0;
                        *jptr = ilo;
                }
        }
}

/*------------------------------------------------------------------*/
//double zbrent(x1,x2,ind,value)
//        int     ind;
//        double  x1, x2, value;
double zbrent(double x1,double x2,int ind,double value)

        /* using Brent's method, find the root of a function known to lie
         * between x1 and x2; the root returned as x3, will be refined until
         * its accuracy is smaller than 1e-8 times f(x1) or f(x2), depending
         * which one is largest;
         */
{
        double  y1, y2, y, a, b, fa, fb, x3, fc, c, d, e, tol1, xm, s, p,
                q, r, eps;
        eps = 1e-8;
        a = x1;
        b = x2;
        x3 = 0;
        fa = ppvalu(brek,coef,&l,&k,&a,ind)-value;
        fb = ppvalu(brek,coef,&l,&k,&b,ind)-value;
        fc = fb;
        y1 = fabs(a);
        y2 = fabs(b);
        if (y1 > y2) y = y1;
        else y = y2;
        tol1 = 2*eps*y;
        while (x3 == 0)
        {
                if (fb*fc > 0)
                {
                        c = a;
                        fc = fa;
                        d = b-a;
                        e = d;
                }
                if (fabs(fc) < fabs(fb))
                {
                        a = b;
                        b = c;
                        c = a;
                        fa = fb;
                        fb = fc;
                        fc = fa;
                }
                xm = .5*(c-b);
                if (fabs(xm) <= tol1 || fb == 0) x3 = b;
                else
                {
                        if (fabs(e) >= tol1 && fabs(fa) > fabs(fb))
                        {
                                s = fb/fa;
                                if (a == c)
                                {
                                        p = 2*xm*s;
                                        q = 1-s;
                                }
                                else
                                {
                                        q = fa/fc;
                                        r = fb/fc;
                                        p = s*(2*xm*q*(q-r)-(b-a)*(r-1));
                                        q = (q-1)*(r-1)*(s-1);
                                }
                                if (p > 0) q *= -1;
                                p = fabs(p);
                                y1 = 3*xm*q-fabs(tol1*q);
                                y2 = fabs(e*q);
                                if (y1 < y2) y = y1;
                                else y = y2;
                                if (2*p < y)
                                {
                                        e = d;
                                        d = p/q;
                                }
                                else
                                {
                                        d = xm;
                                        e = d;
                                }
                        }
                        else
                        {
                                d = xm;
                                e = d;
                        }
                        a = b;
                        fa = fb;
                        if (fabs(d) > tol1) b += d;
                        else
                        {
                                if (xm >= 0) y = tol1;
                                else y = -1*tol1;
                                b += y;
                        }
                        fb = ppvalu(brek,coef,&l,&k,&b,ind)-value;
                }
        }
        return(x3);
}
#ifdef __cplusplus
	}
#endif


