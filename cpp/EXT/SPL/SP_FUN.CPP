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

