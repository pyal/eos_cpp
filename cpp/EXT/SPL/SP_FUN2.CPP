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

