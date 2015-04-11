/*
evalsp.c, and all derivatives
 *              dj(S(i))/dxj; from j=1 to j=k-1. One line for each
 *              x-value.
 *
 *      File "evlsum", containing the results of options -v, -e, -i, -h and -a.
 *              This information is also written to stdout (the terminal).
 *
 *      File "evlfour", containing the values of the Fourier transform 
 *              as follows: s, Re(Ft), Im(Ft). One line for each
 *              s-value.
 *
 ****   Info:
 *
 *      Authors and owners of this program are:
 *              Barend J. Thijsse and Mark A. Hollanders,
 *              Computational Materials Science group (Com,ma,s)
 *              Physical Materials Science Division,
 *              Laboratory of Materials Science, 
 *              Delft University of Technology,
 *              Rotterdamseweg 137, 2628 AL  Delft,
 *                      Phone: +31 15 278 2221
 *                      Fax:   +31 15 278 6730
 *                      E-mail: thijsse@stm.tudelft.nl
 *                      URL: http://dutsm183.stm.tudelft.nl
 *
 */

/* ---INCLUDES--- */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* ---DEFINES (NUMBERS)--- */
#define NMAX 2001
#define LMAX 1001
#define KMAX 24

/* ---ANNOUNCE FUNCTIONS--- */ 
int    l2knts(),
       bsplpp(),
       bsplvb(),
       interv();
double ppvalu(),
       ppigr(),
       zbrent();

/* ---GLOBAL VARIABLES (are zero-initialized)--- */
int    k, km1, l, n, left, jbsp, ihi, ilo = 1, hhon, nextremes, nomaxmax=1;
int    firsttime=1, very_uncertain;
double brek[LMAX], bcoef[LMAX+KMAX], coef[KMAX][LMAX], ppvalue,
       deltal[KMAX], deltar[KMAX]; 
double halfheight, extrsum, maxmax, zerolevel;
FILE   *fpw, *fps, *fpf, *fpr, *fpx;



/* ---MAIN--- */
main(argc,argv)  int argc; char *argv[];
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
        fclose(fpx);

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
double ppigr(ara,dara,iptr,jptr,xptr,yptr)
        int     *iptr, *jptr;
        double  *xptr, *yptr, ara[], dara[][LMAX];

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
int l2knts(ara,iptr,jptr,arb,kptr)
        int     *iptr, *jptr, *kptr;
        double  *ara, *arb;  

        /*  breakpoints to knots  */
{
        int     i;
        for (i = 1; i <= km1; i++) arb[i] = ara[1];
        for (i = 1; i <= *iptr; i++) arb[km1+i] = ara[i];
        n = km1+(*iptr);
        for (i = 1; i <= *jptr; i++) arb[*kptr+i] = ara[*iptr+1];
}

/*------------------------------------------------------------------*/
int bsplpp(ara,arb,iptr,jptr,dara,arc,darb,kptr)
        int     *iptr, *jptr, *kptr;
        double  ara[], arb[], arc[], dara[][KMAX], darb[][LMAX];

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
int bsplvb(ara,jhigh,index,xptr,iptr,arb)
        int     jhigh, index, *iptr;
        double  ara[], arb[], *xptr;

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
double ppvalu(ara,dara,iptr,jptr,xptr,jderiv)
        int     *iptr, *jptr, jderiv;
        double  *xptr, ara[], dara[][LMAX];

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
int interv(ara,iptr,xptr,jptr,kptr)
        int     *iptr, *jptr, *kptr;
        double  *xptr, ara[];

        /*  locates a point within an increasing sequence of points  */
{
        int     istep, middle, ilos;
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
double zbrent(x1,x2,ind,value)
        int     ind;
        double  x1, x2, value;

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
9.8 20040 895.321
663 18880 869.022
666.1 17720 841.903
668.5 16840 820.731
672.5 16680 816.823
674.9 15520 787.909
678.8 15680 791.96
682 13360 731.027
684.4 12760 714.423
688.3 12320 701.997
690.7 11480 677.643
693.9 11880 689.348
697 10160 637.495
700.9 10240 640
704.1 10160 637.495
706.5 10040 633.719
709.6 11200 669.328
712.7 12320 701.997
716.7 11360 674.092
719 12480 706.541
722.1 14880 771.492
725.3 15840 795.99
728.4 14760 768.375
730.7 16000 800
733.9 18240 854.166
737.8 19480 882.723
740.1 21280 922.605
744 23520 969.948
747.1 26520 1029.95
751 27800 1054.51
754.1 32800 1145.43
756.4 32480 1139.82
759.5 38680 1243.86
762.6 40800 1277.5
766.5 42560 1304.76
768.9 43880 1324.84
772 49600 1408.55
775.8 53080 1457.12
778.1 57280 1513.67
782 55720 1492.92
784.3 58440 1528.92
788.2 59800 1546.61
790.5 58320 1527.35
793.6 61880 1573.28
797.5 59480 1542.47
800.6 59280 1539.87
803.6 55800 1493.99
806 55160 1485.4
809.8 51560 1436.11
812.9 46440 1362.94
816 44320 1331.47
819.1 38640 1243.22
822.1 33120 1151
825.2 29480 1085.91
828.3 26240 1024.5
831.4 22920 957.497
834.4 20920 914.768
837.5 17720 841.903
840.6 16960 823.65
843.7 17080 826.559
846.7 15000 774.597
849.8 16160 803.99
852.9 15040 775.629
855.2 16400 809.938
859 15800 794.984
862.1 15800 794.984
865.2 16480 811.911
869 16600 814.862
871.3 17160 828.493
875.1 18880 869.022
877.4 18800 867.179
881.3 20440 904.212
884.3 21520 927.793
886.7 19200 876.356
890.5 23960 978.979
893.6 24200 983.87
896.6 23160 962.497
898.9 25160 1003.19
902.8 24840 996.795
905.8 25960 1019.02
908.9 26400 1027.62
912 27280 1044.61
915 28120 1060.57
918.9 29920 1093.98
921.2 30520 1104.9
925 30920 1112.12
927.3 30440 1103.45
931.1 31640 1124.99
933.4 34480 1174.39
937.3 35640 1193.98
940.3 37240 1220.49
942.7 37120 1218.52
945.7 36880 1214.58
948.8 37520 1225.07
952.6 36600 1209.96
955.7 36040 1200.67
958.8 38160 1235.48
961.9 39080 1250.28
964.9 37600 1226.38
968 37600 1226.38
970.3 38000 1232.88
974.2 38120 1234.83
977.2 38040 1233.53
980.3 36680 1211.28
983.4 39560 1257.93
986.5 40160 1267.44
990.3 42200 1299.23
992.6 42320 1301.08
995.7 46560 1364.7
999.6 45720 1352.33
1001.9 47760 1382.17
1005.7 51840 1440
1008.8 48760 1396.57
1011.9 49720 1410.25
1014.2 53000 1456.02
1018.1 51600 1436.66
1021.2 54240 1472.96
1024.3 55080 1484.32
1027.3 54000 1469.69
1030.4 55600 1491.31
1033.5 54600 1477.84
1035.9 54760 1480
1039.7 53120 1457.67
1042 51520 1435.55
1045.9 51320 1432.76
1049 48840 1397.71
1051.3 47720 1381.59
1055.2 47280 1375.21
1058.3 46720 1367.04
1061.4 41200 1283.74
1064.5 42880 1309.66
1067.7 39320 1254.11
1070.8 38640 1243.22
1073.9 35560 1192.64
1077 35280 1187.94
1080.1 35080 1184.57
1083.2 32960 1148.22
1085.6 32520 1140.53
1089.5 30360 1102
1092.6 29440 1085.17
1094.9 29320 1082.96
1098 29160 1080
1101.2 28840 1074.06
1105.1 28920 1075.55
1107.4 29760 1091.05
1110.6 27760 1053.76
1113.7 27680 1052.24
1116.8 29520 1086.65
1120 29440 1085.17
1123.1 29280 1082.22
1127 30280 1100.55
1129.4 31840 1128.54
1133.3 30560 1105.62
1135.7 32320 1137.01
1138.8 31360 1120
1142 31040 1114.27
1144.3 33120 1151
1148.3 36240 1203.99
1150.6 32960 1148.22
1153.8 33640 1160
1157 36200 1203.33
1160.1 41200 1283.74
1163.3 38000 1232.88
1166.4 37920 1231.58
1169.6 39640 1259.21
1172.8 42200 1299.23
1176 42760 1307.82
1179.1 43920 1325.44
1181.5 44760 1338.06
1185.5 44320 1331.47
1187.9 46480 1363.52
1191.1 45080 1342.83
1194.2 46520 1364.11
1197.4 47040 1371.71
1200.6 45400 1347.59
1203.8 43280 1315.75
1206.2 45920 1355.29
1209.4 45440 1348.18
1212.6 44080 1327.86
1215.8 41360 1286.23
1219.8 42400 1302.31
1222.2 40360 1270.59
1225.4 38440 1240
1227.8 39760 1261.11
1231.8 37280 1221.15
1235.1 37440 1223.76
1238.3 35480 1191.3
1240.7 32440 1139.12
1243.9 32000 1131.37
1247.1 31800 1127.83
1250.4 29480 1085.91
1252.8 31320 1119.29
1256 29840 1092.52
1259.3 28480 1067.33
1262.5 27600 1050.71
1265.8 26920 1037.69
1269 24720 994.384
1272.2 24080 981.428
1275.5 22720 953.31
1278.8 22880 956.661
1282 22240 943.186
1285.3 20480 905.097
1287.7 21200 920.869
1291 18560 861.626
1294.2 20160 897.998
1297.5 19000 871.78
1300 19920 892.637
1303.2 19080 873.613
1306.5 18480 859.767
1309.8 18480 859.767
1313.1 17560 838.093
1315.6 17760 842.852
1318.9 16880 821.706
1322.1 17360 833.307
1325.4 16160 803.99
1328.7 14640 765.245
1331.2 14840 770.454
1335.4 14280 755.778
1337.8 14000 748.331
1340.3 14000 748.331
1343.6 14120 751.532
1346.1 14480 761.052
1350.3 13480 734.302
1352.8 13120 724.431
1356.1 12560 708.802
1358.6 12960 720
1362.8 12040 693.974
1365.3 12400 704.273
1368.6 10720 654.828
1371.9 12880 717.774
1374.4 11560 680
1377.8 10600 651.153
1381.1 11280 671.714
1384.5 9640 620.967
1387 9320 610.574
1390.4 9800 626.099
1392.9 8840 594.643
1396.3 10040 633.719
1399.7 8720 590.593
1402.2 7520 548.452
1405.6 8280 575.5
1408.1 7040 530.66
1411.5 8360 578.273
1414.9 7080 532.165
1418.3 7040 530.66
1421.7 6680 516.914
1423.4 6320 502.792
1427.6 7040 530.66
1430.2 7200 536.656
1433.6 6560 512.25
1436.1 5520 469.894
1439.5 5920 486.621
1443 6280 501.199
1445.5 5400 464.758
1449.8 5920 486.621
1452.4 5880 484.974
1455.8 6480 509.117
1458.4 6240 499.6
1461.8 5760 480
1465.3 5800 481.664
1467.9 6080 493.153
1471.3 5960 488.262
1473.9 5320 461.303
1477.4 5760 480
1480.8 5760 480
1483.4 5400 464.758
1486 5960 488.262
1489.5 6600 513.809
1493 5640 474.974
1495.6 6640 515.364
1499.1 6240 499.6
1501.7 5280 459.565
1504.3 6960 527.636
1507.8 5440 466.476
1510.4 5680 476.655
1514.8 5520 469.894
1517.4 5880 484.974
1521 5840 483.322
1522.7 6160 496.387
1526.2 5720 478.33
1529.8 5200 456.07
1533.3 5360 463.033
1535.9 5320 461.303
1539.5 5560 471.593
1543 5760 480
1545.7 5560 471.593
1548.3 4880 441.814
1551.9 5480 468.188
1554.5 4440 421.426
1558.1 5080 450.777
1560.7 5520 469.894
1564.3 4840 440
1567 4880 441.814
1569.7 3880 393.954
1573.2 3960 397.995
1575.9 3440 370.945
1579.5 4040 401.995
1582.2 4680 432.666
1584.9 4240 411.825
1589.4 4080 403.98
1592.1 4040 401.995
1594.8 3840 391.918
1599.3 3920 395.98
1602 4920 443.621
1604.7 4240 411.825
1607.4 3800 389.872
1611.1 3800 389.872
1614.7 4000 400
1617.4 3600 379.473
1620.2 4160 407.922
1623.8 3920 395.98
1626.5 3560 377.359
1629.3 3240 360
1632.9 3760 387.814
1636.6 4400 419.524
1639.3 3920 395.98
1642.1 3360 366.606
1645.8 3840 391.918
1647.6 4440 421.426
1652.2 4280 413.763
1655 3880 393.954
1657.7 5040 448.999
1662.3 4040 401.995
1664.2 4240 411.825
1667.9 3880 393.954
1670.7 3800 389.872
1674.4 4000 400
1678.1 4800 438.178
1680 4640 430.813
1683.7 4360 417.612
1686.5 4560 427.083
1689.3 4360 417.612
1692.1 4200 409.878
1695.8 3720 385.746
1698.6 4720 434.511
1701.4 4880 441.814
1705.2 5000 447.214
1709 4520 425.206
1711.8 4920 443.621
1714.6 4960 445.421
1718.4 4960 445.421
1720.3 4240 411.825
1724 3960 397.995
1726.9 4080 403.98
1729.7 5000 447.214
1733.5 5040 448.999
1736.4 4800 438.178
1741.1 5240 457.821
1743 5640 474.974
1746.8 3760 387.814
1749.7 4400 419.524
1752.6 5120 452.548
1756.4 4240 411.825
1758.3 4920 443.621
1762.2 4240 411.825
1766 4920 443.621
1768.9 4360 417.612
1772.7 4640 430.813
1774.7 5400 464.758
1778.5 4320 415.692
1781.4 5120 452.548
1785.3 4720 434.511
1787.2 4520 425.206
1790.1 5080 450.777
1794 5040 448.999
1797.9 4200 409.878
1800.8 4560 427.083
1803.8 4160 407.922
1806.7 4160 407.922
1809.6 4160 407.922
1813.5 4600 428.952
1816.5 4080 403.98
1820.4 4360 417.612
1822.4 4840 440
1826.3 4240 411.825
1828.3 3760 387.814
1832.2 4440 421.426
1835.2 4720 434.511
1838.1 4640 430.813
1842.1 4880 441.814
1845.1 4160 407.922
1848.1 4200 409.878
1852 4240 411.825
1855 5920 486.621
1859 4240 411.825
1860 5480 468.188
1864 4720 434.511
1868 4800 438.178
1871 4080 403.98
1874 4040 401.995
1877 4160 407.922
1880 5360 463.033
1883.1 4920 443.621
1887.1 5360 463.033
1890.1 5320 461.303
1894.2 4640 430.813
1896.2 4560 427.083
1900.2 4640 430.813
1903.3 6080 493.153
1906.3 4840 440
1909.4 4920 443.621
1911.4 5160 454.313
1915.5 5040 448.999
1919.6 5760 480
1921.6 5280 459.565
1925.7 4640 430.813
1927.8 5640 474.974
1931.9 5560 471.593
1933.9 5360 463.033
1938.1 4440 421.426
1941.2 5200 456.07
1944.3 5000 447.214
1947.4 5480 468.188
1950.5 5480 468.188
1952.5 5240 457.821
1956.7 5880 484.974
1959.8 5160 454.313
1964 4440 421.426
1966.1 5040 448.999
1970.2 5720 478.33
1973.4 5080 450.777
1976.5 5840 483.322
1978.6 6760 520
1981.7 5440 466.476
1987 5800 481.664
1989.1 5520 469.894
1992.3 5680 476.655
1996.5 5640 474.974
1999.7 6320 502.792
 1493.29
635.106 1579.11
638.215 1649.2
641.325 1654.7
644.434 1653.85
647.544 1646.26
650.653 1559.45
653.763 1442.94
656.873 1345.52
659.983 1221.56
663.093 1178.58
666.203 1121.11
669.314 1060.53
672.424 990.372
675.535 901.878
678.645 870.612
681.756 807.554
684.867 748.247
687.978 709.193
691.089 672.17
694.2 652.323
697.311 712.868
700.422 734.514
703.533 751.894
706.645 819.954
709.756 955.015
712.868 1056.21
715.979 1092.44
719.091 1155.52
722.202 1219.88
725.314 1294.37
728.426 1392.91
731.538 1552.49
734.649 1658.36
737.761 1917.86
740.873 2101.89
743.985 2341.09
747.097 2592.44
750.209 2869.71
753.32 3045.64
756.432 3231.62
759.544 3431.99
762.656 3769.73
765.768 4029.7
768.88 4242.98
771.992 4486.8
775.104 4529.7
778.216 4668.73
781.328 4682.86
784.44 4679.02
787.552 4571.11
790.664 4510.32
793.776 4424.71
796.887 4210
799.999 3933.51
803.111 3646.28
806.223 3256.83
809.334 2810.22
812.446 2422.23
815.558 2002.04
818.669 1684.64
821.781 1378.49
824.893 1160.07
828.004 1045.91
831.116 941.919
834.227 955.917
837.338 931.206
840.45 963.264
843.561 1023.33
846.672 1071.26
849.783 1118.28
852.894 1155.18
856.005 1150.65
859.116 1208.19
862.227 1253.56
865.338 1298.26
868.449 1449.18
871.559 1445.06
874.67 1526.11
877.78 1638.24
880.891 1691.87
884.001 1772.16
887.112 1814.13
890.222 1861.8
893.332 1907.25
896.442 1935.75
899.552 1963.88
902.662 2014.46
905.772 2069.35
908.881 2217.08
911.991 2278.69
915.1 2273.33
918.21 2327.37
921.319 2381.58
924.429 2503.07
927.538 2634.05
930.647 2737.31
933.756 2842.06
936.865 2910.12
939.973 2859.7
943.082 2789.54
946.191 2769.96
949.299 2766.64
952.408 2781.44
955.516 2798.03
958.624 2826.44
961.732 2857.62
964.84 2792.7
967.948 2766.6
971.056 2733.71
974.164 2789.94
977.271 2847.51
980.379 3024.11
983.486 3162.15
986.593 3271.12
989.701 3525.41
992.808 3691.2
995.915 3733.32
999.021 3814.2
1002.13 3848
1005.23 3876.61
1008.34 3962.24
1011.45 3988.16
1014.55 4083.6
1017.66 4109.08
1020.77 4175.14
1023.87 4215.43
1026.98 4105.8
1030.08 4034.61
1033.19 3934.75
1036.3 3760.12
1039.4 3625.54
1042.51 3586.28
1045.61 3401.25
1048.72 3315.48
1051.82 3169.69
1054.93 3066.77
1058.03 2891.12
1061.14 2709.86
1064.24 2601.55
1067.35 2476.59
1070.45 2421.9
1073.55 2371
1076.66 2290.1
1079.76 2170.97
1082.87 2132.69
1085.97 2062.03
1089.08 2018.23
1092.18 2037.31
1095.28 2070.69
1098.39 2058.08
1101.49 2062.21
1104.59 2092.57
1107.7 2086.15
1110.8 2084.47
1113.9 2164.58
1117.01 2204.42
1120.11 2288.28
1123.21 2361.87
1126.32 2361.99
1129.42 2296.83
1132.52 2398.99
1135.62 2473.47
1138.73 2467.76
1141.83 2418.68
1144.93 2587.69
1148.03 2751.76
1151.14 2824.78
1154.24 2870.78
1157.34 2922.8
1160.44 2989.27
1163.54 3097.23
1166.65 3226.68
1169.75 3229.46
1172.85 3326.28
1175.95 3401.48
1179.05 3479.09
1182.16 3494.71
1185.26 3520.77
1188.36 3440.59
1191.46 3401.82
1194.56 3380.54
1197.66 3392.02
1200.76 3307.03
1203.87 3244.93
1206.97 3201.06
1210.07 3051.48
1213.17 2993.65
1216.27 2890.31
1219.37 2801.9
1222.47 2751.51
1225.58 2691.81
1228.68 2572.88
1231.78 2472.55
1234.88 2290.16
1237.98 2215.75
1241.08 2155.25
1244.18 2114.64
1247.28 2108.4
1250.38 2098.32
1253.49 2061.3
1256.59 1977.28
1259.69 1831.47
1262.79 1721.3
1265.89 1653.98
1268.99 1547.48
1272.09 1556.75
1275.19 1472.43
1278.3 1437.31
1281.4 1388.84
1284.5 1382.57
1287.6 1373.77
1290.7 1368.02
1293.8 1373.71
1296.91 1367.82
1300.01 1359.32
1303.11 1306.84
1306.21 1294.63
1309.31 1262.67
1312.41 1223.98
1315.52 1165.51
1318.62 1120.28
1321.72 1055.99
1324.82 1000.98
1327.92 964.44
1331.03 969.952
1334.13 991.075
1337.23 1006.23
1340.33 1002.23
1343.44 989.906
1346.54 942.467
1349.64 939.951
1352.75 857.072
1355.85 842.845
1358.95 852.13
1362.06 833.708
1365.16 852.626
1368.26 828.24
1371.37 773.461
1374.47 739.424
1377.58 677.996
1380.68 659.522
1383.78 665.719
1386.89 619.696
1389.99 638.432
1393.1 578.006
1396.2 566.39
1399.31 553.988
1402.41 521.588
1405.52 503.857
1408.63 475.859
1411.73 455.585
1414.84 481.702
1417.94 494.317
1421.05 471.15
1424.16 453
1427.26 446.123
1430.37 436.425
1433.48 408.965
1436.59 385.318
1439.7 385.909
1442.8 409.154
1445.91 437.517
1449.02 446.971
1452.13 445.134
1455.24 445.366
1458.35 433.135
1461.46 419.858
1464.57 405.263
1467.68 390.172
1470.79 375.974
1473.9 410.557
1477.01 407.815
1480.12 425.729
1483.24 464.538
1486.35 446.026
1489.46 470.207
1492.57 458.975
1495.69 449.976
1498.8 421.851
1501.91 400.57
1505.03 392.274
1508.14 406.68
1511.26 416.344
1514.37 425.091
1517.49 414.347
1520.6 393.158
1523.72 394.256
1526.84 378.333
1529.95 391.293
1533.07 378.346
1536.19 397.945
1539.31 382.915
1542.43 364.47
1545.55 362.619
1548.67 351.818
1551.79 365.125
1554.91 354.056
1558.03 346.213
1561.15 309.995
1564.27 264.491
1567.39 249.201
1570.52 256.511
1573.64 257.455
1576.76 283.219
1579.89 300.963
1583.01 284.151
1586.14 300.79
1589.27 307.184
1592.39 305.71
1595.52 289.697
1598.65 296.412
1601.77 285.429
1604.9 276.287
1608.03 275.063
1611.16 273.099
1614.29 251.65
1617.42 234.688
1620.56 257.3
1623.69 273.279
1626.82 265.771
1629.95 254.507
1633.09 264.792
1636.22 283.932
1639.36 281.035
1642.49 307.592
1645.63 312.626
1648.77 320.629
1651.9 327.383
1655.04 319.132
1658.18 280.95
1661.32 266.51
1664.46 281.928
1667.6 297.17
1670.75 312.822
1673.89 327.186
1677.03 352.719
1680.18 318.519
1683.32 301.014
1686.47 289.027
1689.61 302.811
1692.76 312.897
1695.91 335.709
1699.06 350.914
1702.2 374.462
1705.35 382.891
1708.51 360.603
1711.66 310.635
1714.81 282.585
1717.96 294.942
1721.12 301.151
1724.27 313.178
1727.43 371.533
1730.59 386.091
1733.74 369.977
1736.9 372.391
1740.06 335.119
1743.22 323.301
1746.38 300.837
1749.54 315.183
1752.71 315.018
1755.87 305.264
1759.04 342.836
1762.2 342.708
1765.37 335.514
1768.54 351.325
1771.7 348.763
1774.87 340.376
1778.04 362.5
1781.22 347.819
1784.39 348.774
1787.56 330.073
1790.74 317.375
1793.91 299.76
1797.09 289.558
1800.27 288.204
1803.45 275.205
1806.63 294.341
1809.81 324.88
1812.99 317.316
1816.17 300.606
1819.36 304.393
1822.54 300.969
1825.73 322.505
1828.91 331.204
1832.1 333.702
1835.29 293.135
1838.48 330.694
1841.68 326.876
1844.87 341.209
1848.06 347.113
1851.26 374.439
1854.46 377.277
1857.65 357.426
1860.85 308.886
1864.05 288.01
1867.26 279.653
1870.46 299.947
1873.66 357.164
1876.87 372.759
1880.08 394.165
1883.28 358.082
1886.49 373.679
1889.7 364.052
1892.92 351.299
1896.13 352.747
1899.35 354.184
1902.56 368.999
1905.78 398.65
1909 381.413
1912.22 368.481
1915.44 371.674
1918.66 397.677
1921.89 395.721
1925.11 374.287
1928.34 353.627
1931.57 349.259
1934.8 358.364
1938.03 362.186
1941.26 380.811
1944.5 402.909
1947.74 404.076
1950.97 379.125
1954.21 375.232
1957.45 346.569
1960.7 335.781
1963.94 378.992
1967.18 409.665
1970.43 437.377
1973.68 446.583
1976.93 452.107
1980.18 419.821
1983.44 398.607
1986.69 420.93
1989.95 414.929
1993.21 411.369
1996.47 444.764
1999.73 465.295
2002.99 476.884
2006.26 474.091
2009.52 465.705
2012.79 482.358
2016.06 449.357
And a reprint, of course.

Have fun.

                Barend J. Thijsse and Mark A. Hollanders
                Computational Materials Science group (Com,ma,s)
                Physical Materials Science Division
                Laboratory of Materials Science 
                Delft University of Technology
                Rotterdamseweg 137, 2628 AL  Delft
                Netherlands
                        Phone: +31 15 278 2221
                        Fax:   +31 15 278 6730
                        E-mail: thijsse@stm.tudelft.nl
                        URL: http://dutsm183.stm.tudelft.nl
 
tion.
 *      There are 3 possibilities:
 *              value = 0    sigmas are used as specified in the 3rd column
 *                           of the input file.
 *              value > 0    sigma=value is used for all y.
 *              value = -n   input y-values are assumed to have n significant
 *                           digits. (data values equal to zero get the smallest 
 *                           sigma found; this was implemented in version 4.2)
 *      The -a option has the effect that the Chi-squared test
 *      is used instead of the Durbin-Watson test (see section "Program
 *      summary", below).
 *
 *      [-r value]
 *      If the sigmas are exactly known as relative values, and/or you want 
 *      them to be considered strictly true, use this "relative freeze" option. 
 *      There are 2 possibilities:
 *              value > 0    sigma=value*|y| is used for all y.
 *              value < 0    sigma=|value|*sqrt(|y|) is used for all y.
 *      In both cases the program crashes if a value y=0 is found, because
 *      that would make the weight factor for that point infinitely large.
 *      The -r option has the effect that the Chi-squared test
 *      is used instead of the Durbin-Watson test (see section "Program
 *      Summary", below).
 *
 *      [-n spacing]
 *      The dws parameter is calculated for further-than-successive
 *      weighted fit residuals, i.e. using terms like (d[i+3] - d[i])**2
 *      (if spacing=3) rather than (d[i+1] - d[i])**2 (if spacing=1). The
 *      default value for "spacing" is 1. The idea is to "jump over" serial
 *      correlation that might have affected closely neighboring points.
 *      Jan Hendrikse came up with this idea. NOTE: Normal DW statistical
 *      tests are still applied, since there doesn't seem to be any
 *      theory on this stuff. This is probably okay as long as "spacing"
 *      is much smaller than the number of data points.
 *
 *      [-o lrev]
 *      The -o option can be used to specify the number of intervals l at which
 *      the "search-direction" reverses and knot-optimization starts ("lrev"
 *      stands for l-reversal). Normally this is done automatically. This
 *      option allows you to interfere with the search-strategy (see below).
 *
 *      [-q]
 *      The -q option ("quick" or "quiet") is described in a separate 
 *      paragraph further down.
 *
 *      [-l]
 *      Sometimes a transformation of the x-values to log10(x) is convenient.
 *      This can be achieved with the -l option.
 *
 *      [-s]
 *      Older versions of spline2 needed the -s option to signal that
 *      the input was a 2-column file. This option is not longer necessary,
 *      but may still be used.
 *
 *      [-f]
 *      This option is exactly equal to the -a option. We found -a easier to
 *      remember, that's all.
 *
 *
 ****   Program summary:
 *
 *      INTRODUCTION
 *      The important point to realize when one tries to fit a spline
 *      funtion to datapoints is that one can obtain a fit as close to
 *      the data as one wants, simply by adding more and more breakpoints
 *      to the spline funcion. However, what one really wants is a
 *      smooth curve, flexible enough to capture the (unknown) functional
 *      relationship underlying the data, yet smooth enough NOT to follow
 *      the noise component in the data due to e.g. measurement errors.
 *      In other words, one wants an 'adequate' fit, not a precise fit.
 *
 *      The problem of separating the noise from the underlying trend
 *      boils down to two issues:
 *            - Which statistic should be used to assess the 'adequacy'
 *              of the fit, and which statistical test should be
 *              performed on it?
 *            - Which search path should be followed (in 'knot-position space')
 *              so that the number and the distribution of the knots 
 *              converge to their 'optimum' values?
 *      The algorithm described below provides solutions to these questions. 
 *      After many years of experience with widely varying types of data we
 *      find that this algorithm works reliably and robustly. Almost always
 *      it ends up with the 'correct' spline without any user decisions.
 *      Only in exceptional cases 'manual' tuning of some parameters is
 *      necessary; some tips are given below in the section "User interference
 *      with the search strategy".
 *      
 *      TEST STATISTICS USED
 *      The search strategy, to be outlined below, involves the computation
 *      of a considerable number of spline-approximations, each
 *      with a different set of breakpoints. The user does not see this.
 *
 *      For each spline-fit two test statistics, "rms" and "dws", are 
 *      determined:
 *               rms = sqrt(D**2/(N-m))
 *      and 
 *               dws = G**2/D**2,
 *      where 
 *              D**2 = SUM(i=1..N){ (d[i])**2 },
 *              G**2 = SUM(i=1..N-1){ (d[i+1]-d[i])**2 },
 *      in which
 *              d[i] = (y[i]-S(x[i])/sigma[i]. 
 *      N is the number of datapoints participating in the fit, and N-m is
 *      the number of degrees of freedom. The program uses m = l + [k-1],
 *      where l is the number of intervals between the breakpoints of the
 *      spline and k-1 the polynomial degree of the spline. S(x) is the spline
 *      function itself and sigma[i] is the RMS estimate of the error component
 *      of y[i]. If the sigmas are not given to the program (in the 3rd
 *      data column or via the -a or -r options), they all get the value 1.
 *
 *      If the sigmas are frozen (via the -a or -r options), the statistical
 *      acceptance of a fit is evaluated by the well-known Chi-squared 
 *      test on D**2. This is not a common situation. Normally one does not
 *      know the experimental errors so precisely that the Chi-squared
 *      test will perform well. This test is so powerful that even a 
 *      modest under- or overestimation of the noise leads to unwanted
 *      over- or underfitting of the data.
 *
 *      If the sigmas are not frozen, which is the normal situation, the 
 *      statistical acceptance of a fit is decided upon by the so-called
 *      Durbin-Watson test on dws, using the beta-function approximations 
 *      for the two extreme theoretical distributions of dws. A statistically
 *      acceptable fit has a dws-value of around 2. Smaller values indicate
 *      the presence of systematic fit errors, larger values are just lucky
 *      (or indicate that the data have been tampered with).
 *      More on the Durbin-Watson statistic dws and on the associated 
 *      theoretical distributions can be found in J. Durbin and G.S. Watson, 
 *      Biometrika 37 (1950) 409, Biometrika 38 (1951) 159, and Biometrika 58 
 *      (1971) 1, in Chapter IX of Mark A. Hollanders, Thesis TU-Delft
 *      (1990), and in Ref. 2, below. 
 *
 *      The crucial property of dws is that any unknown common multiplicative
 *      factor in the sigmas cancels out, because the sigmas appear in the
 *      numerator and in the denominator. Experience shows that even if the
 *      sigmas, or an unknown part of them, vary from datapoint to datapoint,
 *      the value of dws is not too sensitive to this variation (if, at
 *      least, it is not varying too wildly) to make the program find
 *      unsatisfactory splines. This makes dws a far better statistic than
 *      chi**2. In some ways, dws can be compared to the number of sign
 *      changes in the sequence of fit-residuals.
 *              
 *      In all cases a significance level of 5% is used, but another value 
 *      may be used by changing the line "#define REJLEV 0.05" in the code.
 *
 *      SERIAL CORRELATION
 *      The dws test is crucially dependent on the assumption that the
 *      noise values of successive datapoints are statistically independent.
 *      This is in real situations often not the case, since many datasets
 *      contain some sort of serial correlation, for example as a result of
 *      smoothing, filtering, or limited experimental resolution along
 *      the x-axis (these three are in fact the same). Jan Hendrikse has
 *      shown that there is an elegant way around this, by letting dws
 *      'skip over' the correlation 'length' of the data. The generalized
 *      form of the dws parameter therefore becomes
 *               dws = A*(G**2/D**2),
 *      with 
 *              D**2 = SUM(i=1..N) { (d[i])**2 },
 *              G**2 = SUM(i=1..N-spacing) { (d[i+spacing]-d[i])**2 },
 *                 A = (N-1)/(N-spacing),
 *      where "spacing" can be specified using the -n option. The default
 *      value is 1, the classical Durbin-Watson value.
 *
 *      Note: the sum G**2 is not cyclic over the data (as probably would be 
 *      better), so it is scaled up by (N-1)/(N-spacing) to account for the 
 *      loss of terms in the numerator. This is a very crude way to be able
 *      to keep using the same statistical percentages.
 *
 *      SEARCH STRATEGY
 *      The search strategy for the "best" spline-approximation consists of
 *      the following steps (again, l denotes the number of intervals, 
 *      i.e. the number of separate polynomial pieces of the spline):
 *        1) First, spline fits are calculated while the number of intervals
 *           increases from l = 1 up to the lowest number of intervals where 
 *           the fit is statistically accepted, or (with option -o) up to 
 *           "lrev", which stands for l-reversal.
 *           If the Durbin-Watson test is on, there is a certain indecisive
 *           acceptance range, because there are in fact two different 
 *           theoretical distribtuions involved. Fits inside the indecisive 
 *           range are accepted in this stage.
 *           In this first stage the knot-positions are uniformly distributed,
 *           but uniformly with respect to the datapoints, not uniformly
 *           over the x-axis. Each spline interval is made to contain the same
 *           number of datapoints (or as close to it as possible). 
 *           This approach is based on experimental efficiency: one should
 *           measure more datapoints in the ranges where the data vary strongly  
 *           than in the ranges where the data follow a slow trend.
 *           (On the other hand, in many practical cases the experimentalist
 *           takes his data at constant increments of 'x'. So maybe we should
 *           have an option in the program to switch from 'equal information
 *           content' in the spline intervals to 'equal x-increment'.)
 *        2) Next, the program calculates a new series of splines, while it
 *           decreases the number of intervals and simultaneously optimizes
 *           the knot-positions according to the "newnot"-algorithm of De Boor. 
 *           (To prevent singularities, all intervals are made to contain
 *           at least one datapoint at all times.)
 *        3) The "spline wizard" then suggests one (or more) splines 
 *           as "best" if it meets the following criteria:
 *              a) No singularities were found in solving the fit-equations.
 *              b) The fit has passed the acceptance test; fits within the 
 *                 indecisive area of the Durbin-Watson test are initially
 *                 rejected.
 *              c) The spline with the next lower value of l does NOT meet
 *                 the criteria a) and b). This is to make sure
 *                 that one always gets the fit with the lowest possible
 *                 number of intervals (to avoid "overfitting").
 *        4) When no spline-fit lies in the indecisive Durbin-Watson region, 
 *           "lrev" is increased, a new equidistant spline is calculated
 *           for l = lrev, and the "newnot"-sequence is repeated. (This 
 *           step is not invoked when the -o option is used to fix "lrev".)
 *        5) If no acceptable fit is found, a warning appears and the fit with 
 *           the lowest rms is suggested.
 *        6) The number of intervals, together with the values of rms and 
 *           dws for the suggested fits are tabulated and you may choose one of
 *           them (or in fact any other if you like). Typing "s" at this
 *           point selects the best "suggested" spline. You'll get as many
 *           opportunities to try an l value as you need.
 *           Note that when you request a spline-fit with l < lrev,
 *           you always get "optimized" breakpoint locations. If l >= lrev,
 *           you get regular "equispaced" breakpoint locations. Hence, by 
 *           using the option -o 1 on the command line, you can make sure 
 *           that every spline is "equispaced"; by using -o 200 (or so), you
 *           can make sure that every spline is (in a way) "optimized".
 *        7) Finally, if you think you're satisfied, you can inform the spline
 *           wizard that you are done with her assistance, and the program
 *           exits. See under "Using evalsp", below, what to do next.
 *           
 ****   User interference with the search strategy:
 *
 *      It sometimes happens that optimization of the knot positions starts
 *      too early (especially when there are only a few data points) or 
 *      too late (when there are trends in the deviations which you consider 
 *      to be noise, for instance when a 'slow' noise component is
 *      superimposed on a 'fast' one); the best thing to do then, is to use 
 *      option "-o lrev" to force the start of knot-number reversal and
 *      optimization at a desired point l=lrev, or to freeze the sigmas 
 *      at a desired level (-a or -r options).
 *      
 ****   Output:
 *
 *      Always:
 *         File "splrep" contains the following information about
 *         the spline S(x) that was finally accepted (i.e., the last one):
 *         a) the spline order, b) the number of intervals l,
 *         c) the l+1 breakpoints and d) the l+k-1 beta-spline coefficients.
 *         These data form a complete representation of S(x), the
 *         so-called Beta-Spline (BS) representation. These data
 *         are intended to be used by the program "evalsp", with which you
 *         can use, analyze and manipulate the spline. 
 *
 *      Only if the -F option is used:
 *         File "splstat" contains 6 (or 7) data columns giving statistical
 *         information on all spline approximations in "plottable" order of
 *         calculation (including the ones you tried yourself):
 *              1) # of intervals, initially increasing, then, during knot 
 *                 optimization, decreasing,
 *              2) # of degrees of freedom,
 *              3) value of rms,
 *             3a) percentage point of rms (only when -a or -r option is used),
 *              4) value of dws,
 *              5) percentage point of lower distribution of dws,
 *              6) percentage point of upper distribution of dws.
 *         (Some data for the ones you tried yourself appear as "-999" in the
 *         file, because we have forgotten where to get the proper
 *         values from).
 *
 *      Only if the -F option is used:
 *         File "splres" contains 7 columns giving data & fit information
 *         for each data point:
 *              1) x (or log(x), when the -l option is used),
 *              2) y,
 *              3) value of final spline approximation in point x,
 *              4) weighted error,
 *              5) weight factor 1/sigma**2,
 *              6) first derivative of the final spline approximation,
 *              7) second derivative of the final spline apprximation.
 *
 ****   Using evalsp
 *
 *      Type "evalsp splrep [options]" (after running "spline2") in order to:
 *              1) Evaluate S(x) and all its derivatives in any set of
 *                 x-values [no options, or -x xbegin xend xstep, or
 *                 -f xfilename]; results go to file "evlres".
 *              2) Determine the maxima, minima, and inflection points of S(x)
 *                 [options -e and -i].
 *              3) Determine the x-values in which S(x) has a certain value,
 *                 e.g. for computing level crossing points [option -v value].
 *              4) Calculate the integral of S(x)dx from the first to the
 *                 last data point [option -a].
 *              5) Calculate the Fourier transform of S(x) [option -F sbegin
 *                 send sstep]; results go to file "evlfour".
 *              6) Estimate the x-values in which a single-peaked spline
 *                 reaches half its peak height [option -h, with or without
 *                 option -b backgroundvalue].
 *      For detailed information see the "evalsp" manual page.
 *
 ****   The -q option
 *
 *      The -q option is intended for those cases where one trusts
 *      spline2's suggestion for the best approximation, and wants the
 *      program to run silently and quickly. There is no output at all,
 *      except for the rms-, dws-, and l-values of the best spline (to stderr),
 *      and the BS-representation of the spline to stdout (it goes to the file
 *      "splrep" as well). In this way "spline2" can be simply used in pipes,
 *      for example:
 *
 *              generate_data | spline2 -q [options] | evalsp [options]
 *
 *      where "generate_data" denotes any process that writes a two-
 *      or three-column data set to stdout.
 *
 ****   Bugs and version information:
 *
 *      If the x-values of the data are not in increasing order, everything 
 *      goes wrong!
 *
 *      Program versions lower than 5.02 sometimes followed different
 *      search paths through knot-space on different platforms.
 *      The reason was that up- or downscaling the number of knots
 *      to an 'exact' integer number (for instance, lnew = 0.95*40) led
 *      on some machines to a double-to-int truncation to the correct
 *      integer (here: 38), while on others to the next lower integer
 *      (here: 37). This 'bug' has been 'fixed' in version 5.02: all
 *      values are still truncated, unless they are within UPMARGIN
 *      of the 'exact' integer.
 *
 *      Don't expect miracles. The program is good, but not perfect.
 *      Be wise and check your spline by plotting it together with the data.
 *      The spline values are most conveniently found in columns 1 and 2 of
 *      the file "evlres" that is created by running "evalsp splrep".
 *
 *****  Note: 
 *
 *      In the functions graer and grasp, which generate plots (among
 *      other things) the actual graphics parts have been commented
 *      out, since they were old Tektronix-style routines that nobody
 *      ever uses these days.
 *
 ****   Info: 
 *
 *      For information about splines and the algorithm-kernels used, see
 *      (Ref. 1) Carl de Boor, "A Practical Guide to Splines",
 *               (Springer, 1978).
 *
 *      The curve-fitting algorithms and strategies employed are described in:
 *      (Ref. 2) Barend J. Thijsse, Mark A. Hollanders, and J. Hendrikse,
 *               "A practical algorithm for least-squares spline approximation
 *               of data containing noise", Computers In Physics, 
 *               Jul/Aug (1998).
 *
 *      Authors and owners of this program are:
 *              Barend J. Thijsse and Mark A. Hollanders
 *              Computational Materials Science group (Com,ma,s)
 *              Physical Materials Science Division
 *              Laboratory of Materials Science 
 *              Delft University of Technology
 *              Rotterdamseweg 137, 2628 AL  Delft
 *              Netherlands
 *                      Phone: +31 15 278 2221
 *                      Fax:   +31 15 278 6730
 *                      E-mail: thijsse@stm.tudelft.nl
 *                      URL: http://dutsm183.stm.tudelft.nl
 *
 *
 *----------------------------------------------------------------------------
 *                      END OF MANUAL PAGES
 *----------------------------------------------------------------------------
 */

/* ---INCLUDES--- */
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

/* ---DEFINES (NUMBERS)--- */
#define NMAX 10000      /* Maximum number of data points */
#define LMAX 2001       /* Maximum number of intervals */
#define KMAX 24         /* Maximum spline order */
#define RMAX 2000       /* Maximum number of intermediate results */  
#define REJLEV 0.05     /* Rejection level of statistical tests (5%) */
#define UPFACTOR  1.1   /* Increment factor for l */
#define DOWNFACTOR 0.95 /* Decrement factor for l */
#define UPMARGIN 1e-7   /* Calculated (double) values of l this close 
                           under an integer are rounded upward instead
                           of truncated downward (v. 5.02) */

/* ---DEFINES (MACROS)--- */
#define TRUNC_SPEC(i,d) (((double)((i)+1)-(d)) < UPMARGIN ? ((i)+1) : (i))

/* ---ANNOUNCE FUNCTIONS---  */
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
       njan;            /* durbin-watson distance index */
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
main(argc,argv)  int argc; char *argv[];
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
printf("====================================================================\n");
            exit(1);
            }

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
int l2sub(iptr,iopt,wr) int *iptr, iopt, wr;
         
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
int newknt(ara,dara,iptr,jptr,arb,kptr,darb)
        double  ara[], arb[], dara[][LMAX], darb[][LMAX];
        int     *iptr, *jptr, *kptr;

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
int l2knts(ara,iptr,jptr,arb,kptr)
        int     *iptr, *jptr, *kptr;
        double  *ara, *arb;  

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
int l2appr(ara,iptr,jptr,dara,arb,arc)
        int     *iptr, *jptr;
        double  ara[], arb[], arc[], dara[][LMAX+KMAX];

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
int bchfac(dara,iptr,jptr,ara)
        int     *iptr, *jptr;
        double  ara[], dara[][LMAX+KMAX];

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
int bchslv(dara,iptr,jptr,ara)
        int *iptr, *jptr;
        double  ara[], dara[][LMAX+KMAX];

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
int bsplpp(ara,arb,iptr,jptr,dara,arc,darb,kptr)
        int     *iptr, *jptr, *kptr;
        double  ara[], arb[], arc[], dara[][KMAX], darb[][LMAX];

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
int bsplvb(ara,jhigh,index,xptr,iptr,arb)
        int     jhigh, index, *iptr;
        double  ara[], arb[], *xptr;

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
int l2err(prfun,grfun)  int prfun, grfun;

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
double ppvalu(ara,dara,iptr,jptr,xptr,jderiv)
        int     *iptr, *jptr, jderiv;
        double  *xptr, ara[], dara[][LMAX];

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
int interv(ara,iptr,xptr,jptr,kptr)
        int     *iptr, *jptr, *kptr;
        double  *xptr, ara[];

        /*  locates a point within an increasing sequence of points  */
{
        int     ip, jp, kp, istep, middle, ilos;
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
int dwtest(x,xptr,yptr) double  x, *xptr, *yptr;

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
double betai(a,b,x)     double a, b, x;

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
double betacf(a,b,x)    double  a, b, x;

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
double chitest(chi,fr)
        int     fr;
        double  chi;
                
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
int gser(xptr,a,x,yptr) double  *xptr, a, x, *yptr;

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
int gcf(xptr,a,x,yptr)  double  *xptr, a, x, *yptr;

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
int graer(ind)  int     ind;
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
int grasp()

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
