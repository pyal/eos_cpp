	include 'debug.inc'

	subroutine apror(vpol,r,rw1,rw)

c *****************************************************************************
c **  The approximation of density for porous media                          **
c **                                                                         **
c **  kind: i:input; w:working; h:help; o:output; c:constant                 **
c **                                                                         **
c **                                                                         **
c **    VARIABLES:                                                   kind    **
c **    ---------                                                    ----    **
c **    vpol      porosity                                             i     **
c **    r         average density for cell                             i     **
c **    rw1       real density for condensed matter                    w     **
c **    rw        effective density which is used for calculations     o     **
c **    vpmin     down limit for porosity region                       c     **
c **    vpmax     upper limit for porosity region                      c     **
c **    al                                                             w     **
c **                                                                         **
c **                                                                         **
c *****************************************************************************
	implicit real*8 (a-h,o-z)
	data vpmin/0.1/,vpmax/0.3/

	rw1=r/(1.-vpol)
	if(vpol.lt.vpmin)then
	al=0.
	else  if(vpol.gt.vpmax)then
	al=1.
	else
	al=(vpol-vpmin)/(vpmax-vpmin)
	endif
	rw=r*al+rw1*(1.-al)
	return
	end

	subroutine adi1(id)
c *****************************************************************************
c **  The Riemann problem solver                                             **
c **                                                                         **
c **  kind: i:input; w:working; h:help; o:output; c:constant                 **
c **                                    oc:output conditional                **
c **  parameters for Riemann solver                                          **
c **  index 1 at the end of variable name denote parameter for left part     **
c **  index 2 at the end of variable name denote parameter for right part    **
c **    VARIABLES from common /cdat/:                                kind    **
c **    ---------                                                    ----    **
c **    vp1       porosity                \                            i     **
c **    r1        average density for cell \                           i     **
c **    p1        pressure                  for left part              i     **
c **    u1        X-velocity               /                           i     **
c **    v1        Y-velocity              /                            i     **
c **                                                                         **
c **    vp2       porosity                \                            i     **
c **    r2        average density for cell \                           i     **
c **    p2        pressure                  for right part             i     **
c **    u2        X-velocity               /                           i     **
c **    v2        Y-velocity              /                            i     **
c **    vb        front velocity                                       oc    **
c **                                                                         **
c **    input VARIABLES from common /ad/                                     **
c **    nm1       number of EOS             for left part              i     **
c **    nm2       number of EOS             for right part             i     **
c **                                                                         **
c **    id        control variables output                             i     **
c **              2-output ug,pg only                                        **
c **              4-for front conditions ug,pg,vb,rg,eg,vpg                  **
c **                                                                         **
c **    iar       control pressure calculation on free surface         i     **
c **              0- pressure=plim                                           **
c **              1- pressure is calculated                                  **
c **                                                                         **
c **    epsgas    velosity precision for gas EOS (nm1,nm2=31...37)     i     **
c **    epscon    velosity precision for condensed EOS (nm1,nm2=11...) i     **
c **                                                                         **
c **     VARIABLES from common /ad/:                                 kind    **
c **    ---------                                                    ----    **
c **    ra1       initial real density 	ra1=r1/(1.-vpp1)               w     **
c **    rz1       iterative density                  \                 w     **
c **    ez1       iterative energy                    \                w     **
c **    az1       iterative sound velocity             \               w     **
c **    pz1       iterative pressure                    for left part  w     **
c **    eps1      mass volocity precision              /               w     **
c **    nm1       number of EOS                       /                i     **
c **    duu1      velocity increment                 /                 w     **
c **    drr1      volume increment (drr1=(1./ra1-1./rz1))              w     **
c **                                                                         **
c **    rg        density      \                                       oc    **
c **    eg        energy        \                                      oc    **
c **    ug        velocity       goal variables for Riemann problem    o     **
c **    pg        pressure      /                                      o     **
c **    vpg       porosity     /                                       oc    **
c **                                                                         **
c **    imat      0-for contact between different physical regions.    i     **
c **               In this case the rebound pressure is 0.                   **
c **              1-for internal part of one physical region.                **
c **               In this case the rebound pressure less 0.                 **
c *****************************************************************************
	implicit real*8 (a-h,o-z)
	common /error/ ier
	include 'bigpar.inc'
	include 'ad.inc'
	include 'adch.inc'
	include 'am.inc'
	include 'cdat.inc'
c	common /pfrom/ rb,pb,eb,nurb,vpbeg,vpend
	include 'pfrom.inc'
	include 'cfg.inc'
	external deltu,deltud1
	real*8 vbd,ugd,rgd,u2d,ra2d

	data ipri/0/
c  The subroutine is the Riemann problem solver

c the output of adi1 is :
c		pg-pressure for undistructed part of media in cell
c		ug-velocity  for undistructed part of media in cell

	idadi=id
c	call adiwsav
	if((nm1.ge.30.and.nm1.lt.40).or.(nm2.ge.30.and.nm2.lt.40))then
	eps=epsgas
	else
	eps=epscon
	endif
	ier=0
	vpp1=vp1
	vpp2=vp2

c	if(vp2.gt.0.1)vpp2=0.
c	if(vp1.gt.0.1)vpp1=0.

	ra1=r1/(1.-vpp1)
	ra2=r2/(1.-vpp2)

c	call apror(vp1,r1,rx1,ra1)
c	call apror(vp2,r2,rx2,ra2)

	if(nm1.ne.0)then
	if(nm1.eq.37)gmg=gma1
	if(nm1.eq.40)ctnt=ctnt1
	call tota(p1,ra1,e1,a1,tr,nm1)
c	call sum(e1,ra1,p1,a1,tr,nm1)

c       if(ier.ne.0)then
c       print*,' incorrect parameters on 1 side'
c       print*,' adi ier=',ier
c       print*,
c     *' nm1=',nm1,' ra1=', ra1,
c     *' u1=', u1,
c     *' p1=',p1,' e1=',e1,'vp1=', vp1
c       ier=0
c       endif
	endif
	if(nm2.ne.0)then
	if(nm2.eq.37)gmg=gma2
	if(nm2.eq.40)ctnt=ctnt2
	call tota(p2,ra2,e2,a2,tr,nm2)
c	call sum(e2,ra2,p2,a2,tr,nm2)
c       if(ier.ne.0)then
c       print*,' uncorrect parameters on 2 side'
c       print*,' adi ier=',ier
c       print*,
c     *' nm2=',nm2,' ra2=', ra2,
c     *' u2=', u2,
c     *' p2=',p2,' e2=',e2,'vp2=', vp2
c       ier=0
c       endif
	endif

	if(nm1*nm2.ne.0)goto1
c-------------------free bound---------------------c
	if(nm1.eq.0)then
	if(iair.eq.1)then
	pk=pout(-u2)
	else
	pk=plim
	endif
	if(nm2.eq.37)gmg=gma2
	if(nm2.eq.40)ctnt=ctnt2
	call rim(eps,p2,ra2,u2,a2,pk,rr,e2,ug,nm2,2)
	else
	if(iair.eq.1)then
	pk=pout(u1)
	else
	pk=plim
	endif
	if(nm1.eq.37)gmg=gma1
	if(nm1.eq.40)ctnt=ctnt1
	call rim(eps,p1,ra1,u1,a1,pk,rr,e1,ug,nm1,1)
	endif
	pg=pk

	return
1	continue
c--------------porous media
	if(vp1.le.vpbeg.and.vp2.le.vpbeg)goto2
c	if(u2.le.u1)goto2

	if(vp1.gt.vpbeg.and.vp2.gt.vpbeg)then
	pg=(p1+p2)/2.
	ug=(u1+u2)/2.

	if(id.eq.2)return

	if(id.eq.4)then
c--------for indignation front only
	pg=p2
	vb=u2+a2
c 1.03.94 rg=r/(1.-vpg)
	rg=ra2
	eg=e2
	vpg=vp2

	return
	endif


	else if(vp1.gt.vpbeg)then
	pg=p1
	ug=u2
	if(id.eq.2)return

	else if(vp2.gt.vpbeg)then
	pg=p2
	ug=u1

	if(id.eq.2)return
	endif
c--------for internal region only (id.ne.2 .and. id.ne.4)
	if(vb.lt.u1)then
	ug=u1
	pg=p1
	eg=e1
c 1.03.94 rg=r/(1.-vpg)
	rg=ra1
	vpg=vp1
	return
	else
	if(vb.gt.u2)then
	ug=u2
	pg=p2
	eg=e2
c 1.03.94 rg=r/(1.-vpg)
	rg=ra2
	vpg=vp2
	return
	endif
	ug=vb
	pg=plim
	eg=0.
	rg=0.
	vpg=0.
	return
	endif
c-------------------------------------
2	continue
	call init1
	call init2
	if(id.eq.4) eps=eps/4.
	eps1=eps/2.
	eps2=eps/2.
c---------initial------------c
3	continue
	h1=ra1*a1
	h2=ra2*a2
	eps22=eps*eps
c for accustics eps22=999.
c	eps22=999.
c accustics for internal region
c	if(id.ne.4.and.nm1.eq.nm2) eps22=500.*eps22
	du12=u1-u2
	dp12=p1-p2
	ug=(dp12+h1*u1+h2*u2)/(h1+h2)
	pg=p2-h2*(u2-ug)
	if(pg.lt.plim)pg=pvacuum(pg)
c	if(du12*du12+dp12*dp12.lt.0.)then
	if((du12*du12+dp12*dp12).lt.eps22.or.(p1.lt.0.and.pg.lt.0.).
     *or.(p2.lt.0.and.pg.lt.0.).or.(ra1.le.rlim.or.ra2.le.rlim))then
c-----------acoustics
	dp=pg-p1
	az1=a1
	uz1=u1-dp/ra1/az1
	rz1=ra1+dp/az1/az1
	pz1=pg
	dp=pg-p2
	az2=a2
	uz2=u2+dp/ra2/az2
	rz2=ra2+dp/az2/az2
	pz2=pg
c-----------
	if(rz1.lt.0.d0.or.rz2.lt.0.d0.and.ipri.ne.0)then
c	print*,' adi1 rz1,rz2,ra1,ra2,dp,az1,az2'
c	print*, rz1,rz2,ra1,ra2,dp,az1,az2
c	print*,'  r1,r2,vp1,vp2,e1,e2'
c	print*,   r1,r2,vp1,vp2,e1,e2
c	call catch
c	if(rz1.lt.rlim)rz1=rlim
c	if(rz2.lt.rlim)rz2=rlim
	goto 11
	end if
	if(abs(uz1-uz2).gt.eps)ier=2
	goto 10
	endif
c-----------------
c	if(pg.le.-11..and.pg.ge.11.)then
11	continue
	if(pg.le.p1.and.pg.le.p2)then
	call rim12(eps1)
	if(ier.ne.4.and.ier.ne.2)goto 10
	ier=0
	call init1
	call init2
	endif
c---riemann solvers for strong shock
c---exact riemann problem solution
c--to use this exact solver please coment lines :
c  "call duk..." and till the coment line "end of approx.solver"
	call supordd(pg,deltu,deltud1,eps)
c-------------approximate solver (two shock approximation)
c---to use this solver please coment call supordd
c          call duk(u1,u2,ra1,ra2,p1,p2,a1,a2,pgd,ugd,rza1,rza2,nm1,nm2)
c	rz1=rza1
c	rz2=rza2
c	ug=ugd
c	pg=pgd
c	uz1=ugd
c	uz2=ugd
c-------end of approx.solver------
10	continue
	if(ier.ne.0.and.ier.ne.2.and.ipri.eq.1)then
	print*,' adi ier=',ier
	print*,
     *' rz1=', rz1,' rz2=', rz2,' ra1=', ra1,' ra2=', ra2,
     *' u1=', u1,' u2=', u2,' ug=', ug,' pg=', pg,
     *' p1=',p1,' p2=',p2,'uz1=', uz1,' uz2=',uz2
	if(ier.eq.6)call adiwsav
c	call catch
	endif
	ug=(uz1+uz2)/2.
	if(id.eq.2)then
	if(ier.eq.2) then
c-------vacuum treatment
	pg=0.
	rg=0.
c      ug=0.
	ug1=uz1
	ug2=uz2
	eg=0.
	vpg=0.
	endif

	return
	endif

	if(id.eq.4)then
c-0------indignation front
	vb1=ug+az2
	vb2=u2+a2
	rg=rz2

	if(rg.gt.ra2)then
c-1-------compression
	if(rg/ra2.gt.1.+1.d-3)then
c-2----strong--shock-----------
c	vb=(ug*rg-u2*ra2)/(rg-ra2)
	ugd=ug
	rgd=rg
	u2d=u2
	ra2d=ra2
	vbd=(ugd*rgd-u2d*ra2d)/(rgd-ra2d)
	vb=vbd
	vbr=0.5*(vb1+vb2)
	if(ipri.eq.1)then
	vbr=0.5*(vb1+vb2)
	print*,' adi1 vb=',vb,' vbr=',vbr,
     *'vb1=',vb1,'vb2=',vb2,' rg=', rg,' ra1=', ra1,' ra2=', ra2,
     *' u1=', u1,' u2=', u2,' ug=', ug,' pg=', pg,
     *' p1=',p1,' p2=',p2,'vp1=', vp1,' vp2=',vp2
	call catch
	end if

	vpg=vp2

	else
c-----weak--shock------------
	vb=vb2
c	print*,' adi1 vb=',vb,' vbr=',vbr,
c     *'vb1=',vb1,'vb2=',vb2,' rg=', rg,' ra1=', ra1,' ra2=', ra2,
c     *' u1=', u1,' u2=', u2,' ug=', ug,' pg=', pg,
c     *' p1=',p1,' p2=',p2,'vp1=', vp1,' vp2=',vp2
c	call catch
	endif

	call tota(pg,rg,eg,ag,tg,nm2)

	if(vb.gt.ug)then
c-2
	vpg=vp2
c-2
	else
	vpg=vp1
c-2
	endif

c-1-----rarefaction
	else
	vb=vb2

	rg=ra2
	eg=e2
	pg=p2
	ug=u2
	vpg=vp2
c-1
	endif
	if(nm2.eq.37)call perch2

	if(nm2.eq.40)then
	call perch2
	if(rg.gt.ra2)then
	cag(2)=0.5*(pg+p2)*(1.d0/ra2-1.d0/rg)
	if(cag(2).lt.0.d0)cag(2)=0.d0
	else
	cag(2)=0.d0
	end if
	end if

c-0
	endif
	return

	entry adi2(id)
c *****************************************************************************
c **  The determination of parameters along rib moving with vb               **
c **      velocity.                                                          **
c **       VARIABLES are the same as for adi1 (see prev. comment table)      **
c **      but there are difference:                                          **
c **                                                                         **
c **  kind: i:input; w:working; h:help; o:output; c:constant                 **
c **    VARIABLES                                                    kind    **
c **    ---------                                                    ----    **
c **    vb        rib velocity                                         i     **
c **    rg        average density for cell \                           o     **
c **    pg        pressure                  along the rib              o     **
c **    ug        X-velocity               /                           o     **
c **    vg        Y-velocity              /                            o     **
c **    vpg       porosity               /                             o     **
c **    eg        energy                /                              o     **
c *****************************************************************************
c ---------------------determin rg,eg
c-------vacuum treatment
	if(ier.eq.2) then
c	pg=0.d0
c	rg=0.d0
c	ug1=uz1
c	ug2=uz2
c	eg=0.d0
c	vpg=0.d0
c	ug=(uz1+uz2)/2.d0
c	if(vb.ge.ug1.and.vb.le.ug2)return

	rg=0.d0
	ug1=uz1
	ug2=uz2
	eg=0.d0
	vpg=0.d0
	ug=(uz1+uz2)/2.d0
c	if(vb.ge.ug1.and.vb.le.ug2)return
	endif
c--------------
c--1
	if(vb.lt.ug)then
	if(nm1.eq.37)call perch1
	if(nm1.eq.40)call perch1
	ddp1=pg-p1
	epsp=eps*rz1*az1/2.d0
c--2
	if(abs(ddp1).lt.1.d-8*epsp)then
	rr1=ra1
	else
	rr1=rz1
	endif
c--2
	dr1=rr1-ra1
c--2
	if(rr1/ra1.gt.1.d0+1.d-9)then
	d1=(rr1*ug-ra1*u1)/dr1
	else
	d1=(ug-az1+u1-a1)/2.d0
	endif
c--2
	if(d1.gt.u1-a1)goto60
	if(vb.gt.d1)goto50
40	continue
	eg=e1
c 1.03.94 rg=r/(1.-vpg)
	rg=ra1
	pg=p1
	ug=u1
	vpg=vp1
	return
60	continue
c-----------acoustics
	if(vb.lt.u1-a1)goto 40
c rarefaction
	du=vb-(u1-a1)
	rg=ra1*(1.d0-du/az1)
	if(rg.lt.rz1)rg=rz1
	px=p1-du*rz1*az1
	if(px.gt.pg)pg=px
	goto 70
50	continue
c compression
	d1g=d1-ug
	d11=d1-u1
c 1.03.94 rg=r/(1.d0-vpg)
	rg=ra1

	if(abs(d1g).gt.eps)then
	ddg=d11/d1g
	if(ddg.gt.0.d0)rg=ra1*ddg
	end if
c	if(ipri.eq.1)then
c	print*,' rg=',rg,' rz1=',rz1
c	call catch
c	endif
	if(rg.le.0.d0)then
	print*,' adi21 rg,rz1,rz2,ra1,ra2,dp,az1,az2'
	print*, rg,rz1,rz2,ra1,ra2,dp,az1,az2
	call catch
	if(rg.lt.rlim)rg=rlim
	end if
70	continue
	call tota(pg,rg,eg,aa1,tr,nm1)
	vpg=vp1
c--1
	else
	if(nm2.eq.37)call perch2
	if(nm2.eq.40)call perch2
	ddp2=pg-p2
	epsp=eps*rz2*az2/2.
c--2
	if(abs(ddp2).lt.1.d-8*epsp)then
	rr2=ra2
	else
	rr2=rz2
	endif
c--2
	dr2=rr2-ra2
c--2
	if(rr2/ra2.gt.1.+1.d-9)then
	d2=(rr2*ug-ra2*u2)/dr2
	else
	d2=(ug+az2+u2+a2)/2.
	endif
c--2
	if(d2.lt.u2)goto61
	if(vb.lt.d2)goto51
41	continue
c 1.03.94 rg=r/(1.-vpg)
	rg=ra2
	eg=e2
	pg=p2
	ug=u2
	vpg=vp2
	return
61	continue
c-----------acoustics
	if(vb.ge.u2+a2)goto 41
c rarefaction
	du=vb-(u2+a2)
	rg=ra2*(1.+du/az2)
	if(rg.lt.rz2)rg=rz2
	px=p2+du*rz2*az2
	if(px.gt.pg)pg=px
	goto 71
51	continue
c compression
	d2g=d2-ug
	d22=d2-u2
c 1.03.94 rg=r/(1.-vpg)
	rg=ra2
	if(abs(d2g).gt.eps)rg=ra2*d22/d2g
c	if(ipri.eq.1)then
c	print*,' rg=',rg,' rz2=',rz2
c	call catch
c	endif
	if(rg.le.0.d0)then
	print*,' adi22 rg,rz1,rz2,ra1,ra2,dp,az1,az2'
	print*, rg,rz1,rz2,ra1,ra2,dp,az1,az2
	call catch
	if(rz1.lt.0.d0)rz1=0.d0
	if(rz2.lt.0.d0)rz2=0.d0
	end if
71	continue
	call tota(pg,rg,eg,aa2,tr,nm2)
	vpg=vp2

	endif
c--1
111	continue
	if(eg.le.510000.)return
112	format(3x,' -adi- eg,ug,pg,rg'/6(1x,e9.3))
	print 112,eg,ug,pg,rg
	print 113,ier,nm1,r1,p1,ra1,e1,a1,u1
113	format(3x,' adi ier,nm,rr,p,r,e,a,u'/2i5,6(1x,e9.3))
	print 113,ier,nm2,r2,p2,ra2,e2,a2,u2
	print*,'stop 27'
	call catch
	end

	subroutine inich1(cc)
	implicit real*8 (a-h,o-z)
	include 'bigpar.inc'
	include 'inchem.inc'
	include 'adch.inc'
	dimension cc(nchem1)
	do i=1,nchem1
	ca1(i)=cc(i)
	enddo
	return
	end
	subroutine inich2(cc)
	implicit real*8 (a-h,o-z)
	include 'bigpar.inc'
	include 'inchem.inc'
	include 'adch.inc'
	dimension cc(nchem2)
	do i=1,nchem2
	ca2(i)=cc(i)
	enddo
	return
	end
	subroutine perch1
	implicit real*8 (a-h,o-z)
	include 'bigpar.inc'
	include 'inchem.inc'
	include 'adch.inc'
	do i=1,nchem1
	cag(i)=ca1(i)
	enddo
	return
	end
	subroutine perch2
	implicit real*8 (a-h,o-z)
	include 'bigpar.inc'
	include 'inchem.inc'
	include 'adch.inc'
	do i=1,nchem2
	cag(i)=ca2(i)
	enddo
	return
	end

	function deltu(px)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	include 'ad.inc'
	include 'bigpar.inc'
	include 'adch.inc'
	include 'cdat.inc'

	ier=0

	pg=px
	if(pg.le.p1)then
c	if(pg.ge.pz1)call init1
	call init1
	if(nm1.eq.37)gmg=gma1
	if(nm1.eq.40)ctnt=ctnt1

	if(px.ne.pz1)call rim1(eps1)
	else
	if(px.ne.pz1)call shock1(eps1)
	if(ier.eq.3)then
	call init1

	if(px.ne.pz1)call rim1(eps1)
	endif
	endif
	if(pg.le.p2)then
c	if(pg.ge.pz2)call init2
	call init2
	if(nm2.eq.37)gmg=gma2
	if(nm2.eq.40)ctnt=ctnt2


	if(px.ne.pz2)call rim2(eps2)
	else
	if(px.ne.pz2)call shock2(eps2)
	if(ier.eq.3)then
	call init2

	if(px.ne.pz2)call rim2(eps2)
	endif
	endif
	deltu=uz1-uz2
	ug=(uz1+uz2)/2.
	return
	end

	function deltud1(px)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	common /error12/ ier1,ier2
	include 'ad.inc'
	include 'cdat.inc'
c	clim=2.d-6
	clim=2.d-1
	hh1=1./rz1/az1
c	if(px-p1.le.1.d-7*eps1.or.ier1.eq.3.or.duu1.le.0.)then
	if(px-p1.le.1.d-7*eps1.or.duu1.le.0.)then
	if(az2.lt.clim)then
	h1=1./rz1/clim
	else
	h1=1./rz1/az1
	endif
	else
	h1=drr1/duu1/2.
	endif
	hh2=1./rz2/az2
c	if(px-p2.le.1.d-7*eps2.or.ier2.eq.3.or.duu2.le.0.)then
	if(px-p2.le.1.d-7*eps2.or.duu2.le.0.)then
	if(az2.lt.clim)then
	h2=1./rz2/clim
	else
	h2=1./rz2/az2
	endif
	else
	h2=drr2/duu2/2.
	endif
	deltud1=-h1-h2
	return
	end
	subroutine hord(x1,x2,f1,f2,fun,eps,xg)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	external fun
	data iter/100/
	data ipri/1/

	ier=0
	ner=0
	if(f1*f2.ge.0.) then
	print*,' hord: f1*f2 >= 0.'
	print*,' hord convergence  is not achived ',
     *'  x1=',x1,' f1=',f1,' x2=',x2,' f2=',f2,' eps=',eps
	stop 28
	endif
	i1=1
	i2=1
	dx=x1-x2
	df=f1-f2
	xo=x1
	fo=f1

	do 1 i=1,iter
	if(i2.gt.3*i1.or.i1.gt.3*i2)then
c /2
	xn=(x1+x2)/2.
	i1=1
	i2=1
	else
c Newton
	if(dabs(df).lt.eps*dabs(fo))goto 2
	xn=xo-fo*dx/df
	dx1=xn-x1
	dx2=xn-x2
	if(dx1*dx2.lt.0.d0)goto 3
	if(ner.eq.2)then
	ner=1
	goto 2
	else
	ner=ner+1
	if(dabs(dx1).gt.dabs(dx2))then
	xn=x2*0.9d0+x1*0.1d0
	xo=x2
	fo=f2
	else
	xn=x1*0.9d0+x2*0.1d0
	xo=x1
	fo=f1
	end if
	goto 3
	end if
c hord
2	continue
	df=f2-f1
	xn=(x1*f2-x2*f1)/df
	end if
3	continue
	fn=fun(xn)
	if(abs(x1-x2).lt.0.1d0*eps.or.abs(fn).lt.eps)then
	xg=xn
	return
	endif
	if(f2*fn.gt.0.d0)then
	i2=i2+1
	x2=xn
	f2=fn*i2
	fn=f2
	else
	i1=i1+1
	x1=xn
	f1=fn*i1
	fn=f1
	endif
	dx=xn-xo
	df=fn-fo
	xo=xn
	fo=fn
1	continue
	if(ipri.eq.1)
     *print*,' hord convergence  is not obtained for ',iter,
     *' iterations x1=',x1,' f1=',f1,' x2=',x2,' f2=',f2,' eps=',eps
	xg=(x1*f2-x2*f1)/(f2-f1)
	ier=6
	return
	end

	subroutine hord1(x1,x2,f1i,f2i,fun,eps,xg)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	external fun
	data iter/100/
	data ipri/1/
	f1=f1i
	f2=f2i

	ier=0
	ner=0
	if(f1*f2.ge.0.) then
c	print*,' hord1: f1*f2 >= 0.'
c	print*,' hord1 wrong limits ',
c     *'  x1=',x1,' x2=',x2,' f1=',f1,' f2=',f2,' eps=',eps
	if(dabs(f1).lt.dabs(f2))then
	xg=x1
	else
	xg=x2
	end if
	return
c	stop 28
	endif
	i1=1
	i2=1
	dx=x1-x2
	df=f1-f2
	xo=x1
	fo=f1

	do 1 i=1,iter
	if(i2.gt.3*i1.or.i1.gt.3*i2)then
c /2
	xn=(x1+x2)/2.
	i1=1
	i2=1
	else
c Newton
	if(dabs(df).lt.eps*dabs(fo))goto 2
	xn=xo-fo*dx/df
	dx1=xn-x1
	dx2=xn-x2
	if(dx1*dx2.lt.0.d0)goto 3
	if(ner.eq.2)then
	ner=1
	goto 2
	else
	ner=ner+1
	if(dabs(dx1).gt.dabs(dx2))then
	xn=x2*0.9d0+x1*0.1d0
	xo=x2
	fo=f2
	else
	xn=x1*0.9d0+x2*0.1d0
	xo=x1
	fo=f1
	end if
	goto 3
	end if
c hord
2	continue
	df=f2-f1
	xn=(x1*f2-x2*f1)/df
	end if
3	continue
	fn=fun(xn)
	if(abs(x1-x2).lt.0.1d0*eps.or.abs(fn).lt.eps)then
	xg=xn
	return
	endif
	if(f2*fn.gt.0.d0)then
	i2=i2+1
	x2=xn
	f2=fn*i2
	fn=f2
	else
	i1=i1+1
	x1=xn
	f1=fn*i1
	fn=f1
	endif
	dx=xn-xo
	df=fn-fo
	xo=xn
	fo=fn
1	continue
	if(ipri.eq.1)
     *print*,' hord1 convergence  is not obtained for ',iter,
     *' iterations x1=',x1,' f1=',f1,' x2=',x2,' f2=',f2,' eps=',eps
	xg=(x1*f2-x2*f1)/(f2-f1)
	ier=6
	return
	end

	subroutine hord0(x1,x2,f1,f2,fun,eps,xg)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	external fun
	data iter/100/
	data ipri/1/
	ier=0
	if(f1*f2.ge.0.) then
	print*,' hord0: f1*f2 >= 0.'
	print*,' hord0 convergence  is not obtained ',
     *'  x1=',x1,' f1=',f1,' x2=',x2,' f2=',f2,' eps=',eps
	stop 29
	endif
	i1=1
	i2=1
	do 1 i=1,iter
	if(i2.gt.3*i1.or.i1.gt.3*i2)then
	xn=(x1+x2)/2.
	else
	df=f2-f1
	xn=(x1*f2-x2*f1)/df
	endif
	fn=fun(xn)
	if(abs(x1-x2).lt.0.1*eps.or.abs(fn).lt.eps)then
	xg=xn
	return
	endif
	if(f2*fn.gt.0.)then
	i2=i2+1
	x2=xn
	f2=fn
	else
	i1=i1+1
	x1=xn
	f1=fn
	endif
1	continue
	if(ipri.eq.1)
     *	print*,i,' hord0 convergence  is not obtained for ',iter,
     *' iterations x1=',x1,' f1=',f1,' x2=',x2,' f2=',f2,' eps=',eps
	xg=(x1*f2-x2*f1)/(f2-f1)
	ier=6
	return
	end


	function potk(n,vp,r)
c------potk(norm.units)
	implicit real*8 (a-h,o-z)
	include 'pfrom.inc'
	include 'pspal.inc'
c al 5
c fe,cu,au,w,tantalum lead,lit$fluoride ...
c 	data p0/5.,2.,1.,1.5,2.,3.,4.,5.,2.,2.,5.,8.,5.,15.,16*10./
c pspl is strength in kbar
c 3-h2o
	vpk=0.01*r
c	potk=pspl(n)*1.d-3/0.7556/(1.d0+(vp/vpk))
	potk=pspl(n)*1.d-3/0.7556/(1.d0+vp*1.d3)
c	potk=pspl(n)*1.d-3/0.7556
c	if(vp.gt.vpbeg) potk=0.
	return
	end
c----------------------------------------------------
	function pfromvp(vp)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	include 'pfrom.inc'
	ier=0
	if(vp.gt.vpend)then
	vp=vpend
c	ier=1
	endif
	if(vp.lt.0.)then
	vp=0.
c	ier=1
	endif
	rw=rb/(1.-vp)
	call sum(eb,rw,ppr,sr1,atw,nurb)
	ier=0
	if(ppr.gt.pb)ppr=pb+ppr/100.
	pfromvp=pb-ppr
	return
	end
c***********************************************
	subroutine porr(vpol,vppn,e,r,rold,ppr,atw,nur,st)
	implicit real*8 (a-h,o-z)
	include 'cdat.inc'
	external pfromvp
	include 'pfrom.inc'
	common /error/ ier
	data ico /10/
	data stpvp/0.1/,epsvp/1.d-3/
	data ipri/1/
c-------st(ns)

c-26.08.92------shutov input + (air urs nurs=31;explosive urs nurs=32)
	if(nur.eq.40)then
	vpol=0.
	vppn=0.
	call eosetnt(e,r,ppr,sr,atw,nur)
	return
	else if(nur.ge.30.and.nur.lt.40)then
	vpol=0.
	vppn=0.
	call eoseair(e,r,ppr,sr,atw,nur)
	return
	endif
c-end---shutov + (air urs nurs=31;explosive urs nurs=32)
c	if(vpol.gt.0.9)write(*,*) 'vpol=',vpol
	if(vpol.gt.vpend)vpol=vpend
	if(vpol.lt.0.)vpol=0.
	if(vpol.gt.10000.d0*vpbeg)then
c------------ spall out of kinetic model
	ppr=plim
	e=0.
c-- vppn=?
c	nurb=nur
c	rb=r
c	pb=ppr
c	eb=e
c	vppn=vppn+stpvp*3
c	if(vppn.ge.vpend-stpvp)vppn=vpend-stpvp
c	call supordv(vppn,pfromvp,stpvp,epsvp)
c	if(ier.gt.0)then
c	if(ipri.eq.1)
c     *	print*,' porr the vppn didn''t find'
c	if(ipri.eq.1)
c     *	print*,' vppn=',vppn,' vpol=',vpol,' ier=',ier
c	endif
	vppn=1.-r*(1.-vpol)/rold
c	if(vppn.gt.vpbeg)return
	return
c	vpol=vppn
	endif
c-- vppn=!

	pot=potk(nur,vpol,r)
	rw=r/(1.-vpol)
	call sum(e,rw,ppr,sr1,atw,nur)
	if(abs(ppr).le.pot)return
	if(ppr.lt.pot)goto1
	if(vpol.eq.0.)return
c-----porous dissapear----------
	vpp1=0.9d0*vpol
	vpp2=vpol
2	vppn=0.5*(vpp1+vpp2)
	rw=r/(1.-vppn)
	call sum(e,rw,ppr,sr1,atw,nur)
	p0=potk(nur,vppn,r)
	funct=-vppn+vpol-fkin(ppr,p0,vppn,r,st)
	if(abs(funct).lt.1.d-4.or.abs(vpp1-vpp2).lt.1.d-4)return
	if(funct.lt.0.d0)vpp1=vppn
	if(funct.gt.0.d0)vpp2=vppn
c	print*,'porr pot ',pot,' ppr ',ppr,' err ',err,' rwn' ,rwn
c	print*,' vppn ',vppn,' funct ',funct
	goto2
1	continue
c   ---------growth  of damage
	vpp1=vpol
	vpp2=vpol
5	vpp2=vpp2+0.01
	if(vpp2.gt.0.9)then
	ico=ico-1
	if(ico.ge.0)write(*,*) 'attention !! porr'
	vpp2=0.9
	if(ico.lt.-1000)ico=1
	goto 3
	endif

	rw=r/(1.-vpp2)
	call sum(e,rw,ppr,sr1,atw,nur)
	p0=potk(nur,vpp2,r)
	funct=vpp2-vpol-fkin(ppr,p0,vpp2,r,st)
	if(funct.gt.0.)goto3
	goto5
3	vppn=0.5*(vpp1+vpp2)
	rw=r/(1.-vppn)
	call sum(e,rw,ppr,sr1,atw,nur)
	p0=potk(nur,vppn,r)
	funct=vppn-vpol-fkin(ppr,p0,vppn,r,st)
	if(abs(funct).lt.0.0001.or.abs(vpp1-vpp2).lt.0.0001)return
	if(funct.lt.0.)vpp1=vppn
	if(funct.gt.0.)vpp2=vppn
	goto3
	end
c---------how many porr will grow during time st
	function fkin(p,p0,vp,r,st)
c-    st(ns),r,p-in units
	implicit real*8 (a-h,o-z)
	dp=p0-abs(p)
	if(p.lt.0.)dp=abs(p)-p0
	if(abs(p).lt.p0)dp=0.
	if(p.gt.0)then
	fkin=7.5*dp*((p*15.)**4+vp)*st
c	fkin=10.*7.5*dp*((p*15.)**4+vp)*st
	else
	fkin=7.5*dp*vp*st
c	fkin=10.*7.5*dp*vp*st
	endif
	return
	end
	subroutine init1
	implicit real*8 (a-h,o-z)
	include 'ad.inc'
	include 'cdat.inc'
	rz1=ra1
	pz1=p1
	uz1=u1
	az1=a1
	ez1=e1
	return

	entry init2
	rz2=ra2
	pz2=p2
	uz2=u2
	az2=a2
	ez2=e2
	return
	end
c---------------------------------
	function pout(u)
c r0-            air density              (g/sm**3)
c p0-            air pressure              (bar)
c c0-            air sound speed              (km/s)
c g0-            air adiabat exponent

c r_norm-        standard density       (g/sm**3)
c c_norm-        standard sound speed       (km/s)
c rc2_norm-       r_norm*c_norm**2       (bar)
	implicit real*8 (a-h,o-z)
	parameter
     1  (r0=1.25e-3,
     1  p0=1.01325/10000.,
     1  c0=0.33 ,
     1  g0=1.4 ,

     2  r_norm=2.7104,
     2  c_norm=5.28,
     2  rc2_norm=r_norm*c_norm*c_norm,


     3  g_norm=g0,
     3  p_norm=p0/rc2_norm,
     3  a_norm=c0/c_norm)
	if(u.lt.0.)then
	pout=p_norm
	return
	else
	am=u/a_norm
	if(am.lt.0.3)then
	g=g_norm/2.
	else
	if(am.gt.10.)then
	g=g_norm
	else
	g=g_norm*(0.5+(am-0.3)/19.4)
	endif
	endif
	pout=p_norm*(1.+g*am*am)
	return
	endif
	end
	function pvacuum(pvac)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	include 'pfrom.inc'
	include 'ad.inc'
	include 'cdat.inc'
	pvacuum=plim
	if(vp1.gt.vpbeg) return
	if(vp2.gt.vpbeg) return
	if(nm1.gt.30.and.nm1.lt.40) return
	if(imat.eq.0) return
	if(nm1.ne.nm2) return
	pq=-2.*potk(nm1,(vp1+vp2)/2.,(r1+r2)/2.)
	pvacuum=pq
	if(pq.lt.pvac)pvacuum=pvac
	return
	end
c++++++++++++++random

	subroutine random(x1,f1,step,f,a)
	implicit real*8 (a-h,o-z)
	common /error/ ier
c------search x1 in region ( x1-a*step , x1+a*step ),
c----- wheare f(x1) is determined
c-----  a > 0.
	external f
	data ipri/1/
	call savz
	x0=x1
	f0=f1
	n=a+1
	ast=step/n
	do k=1,3
	stp=ast
	do i=1,n
	do j=1,2
	x1=x0+stp
	ier=0
	f1=f(x1)
	if(ier.eq.0)return
	stp=-stp
	enddo
	stp=ast+stp
	enddo
	ast=ast/n
	enddo
	if(ipri.eq.1)
     *	print*,' random :',
     *'there are no good x values  (',x0-step,';',x0+step,')'

	call initz

	f1=f0
	x1=x0

	end

	subroutine random1(x1,f1,step,f,a)
	implicit real*8 (a-h,o-z)
	common /error/ ier
c------search x1 in region ( x1-a*step , x1+a*step ),
c----- wheare f(x1) is determined
c-----  a > 0.
	external f
	data ipri/1/
	f2=abs(f1)
	x0=x1
	f0=f1
	n=a+1
	ast=step/n
	stp=ast
	do i=1,n
	do j=1,2
	x1=x0+stp
	ier=0
	f1=f(x1)
	if(ier.eq.0.and.(f1*f0.lt.0.or.abs(f1).lt.f2))return
	stp=-stp
	enddo
	stp=ast+stp
	enddo
	f1=f0
	x1=x0
	ier=1
	if(ipri.eq.1)
     *	print*,' random1 :',
     *'there are no good x on interval (',x0-step,';',x0+step,')'
	return
	end
	subroutine random2(x1,f1,step,f,a)
	implicit real*8 (a-h,o-z)
	common /error/ ier
c------search x1 in region ( x1-a*step , x1+a*step ),
c-----  a > 0.
	external f
	data ipri/1/
	f2=abs(f1)
	x0=x1
	f0=f1
	n=a+1
	ast=step/n
	stp=ast
	do i=1,n
	do j=1,2
	x1=x0+stp
	ier=0
	f1=f(x1)
	if(f1*f0.lt.0.or.abs(f1).lt.f2)return
	stp=-stp
	enddo
	stp=ast+stp
	enddo
	if(ipri.eq.1)
     *	print*,' random2 :',
     *'there are no good x on interval (',x0-step,';',x0+step,')'
	f1=f0
	x1=x0
	ier=1
	return
	end
	subroutine random10(x1,f1,step,f,a)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	include 'ad.inc'
	include 'cdat.inc'
c------search x1 in region ( x1-a*step , x1+a*step ),
c----- wheare f(x1) is determined
c-----  a > 0.
	external f
	f2=abs(f1)
	x0=x1
	f0=f1
	n=a+1
	ast=step/n
	stp=ast
c--------
	call savz
	do i=1,n
	do j=1,2
	x1=x0+stp
	ier=0
	f1=f(x1)
	if(ier.eq.0.and.(f1*f0.lt.0.or.abs(f1).lt.f2))return
	stp=-stp
c--------
	call initz
	enddo
	stp=ast+stp
	enddo
	f1=f0
	x1=x0
	ier=1
	return
	end
	subroutine random20(x1,f1,step,f,a)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	include 'ad.inc'
	include 'cdat.inc'
c------search x1 in region ( x1-a*step , x1+a*step ),
c-----  a > 0.
	external f
	data ipri/1/
	f2=abs(f1)
	x0=x1
	f0=f1
	n=a+1
	ast=step/n
	stp=ast
c--------
	uo=ug
	po=pg
	call savz
c--------
	do i=1,n
	do j=1,2
	x1=x0+stp
	ier=0
	f1=f(x1)
	if(ipri.eq.1)
     *	print*,' random20 : x1,f1,ier',x1,f1,ier,' (',-step,';',step,')'
	if(f1*f0.lt.0.or.abs(f1).lt.f2)return
	stp=-stp
c--------
	call initz
	ug=uo
	pg=po
c--------
	enddo
	stp=ast+stp
	enddo
	f1=f0
	x1=x0
	ier=1
	return
	end
c  *******isoentropy     *******************
	subroutine rim(epsu,p0,r0,u0,s0,p,r,emax,u,n,m)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	include 'ad.inc'
	include 'cdat.inc'
	include 'cfg.inc'
	data ipri/1/
	is=1
	if(m.ne.1)is=-1
	dp=p-p0
c-----------------
	epsp=epsu*r0*s0/2.
c-------small pressure jump
	if(abs(dp).lt.epsp)then
	du=dp/r0/s0
	r=r0+du*r0/s0
	u=u0-is*du
	if(r.lt.rlim)r=1./(1./r0-du*du/dp)
	if(r.lt.0.)r=r0/2.
c-----------------
	call rimetst(n,p,p0,r,r0,u,du,u0,emax,de,iert)
c-----------------
c
	return
	endif

c---------strong rarefaction----
	abdp=abs(dp)
	step=abdp/epsp
	dumax=2*abdp/r0/s0
	if(p.lt.0.)step=step/3.
	if(step.gt.99)step=99
	ik=step+1
	jj=0
5	continue
	ik=ik+jj*2
	dp=dp/(ik*1.)
	po=p0
	ro=r0
	uo=u0
	so=s0
	ds=0.
c- - - - - - - - - - - - -
	do 1 j=1,ik
	ps=po
	rs=ro
	us=uo
	ss=so

	po=ps+dp

	if(po.lt.0.d0.and.ps.gt.0.d0)then
	dp=p-ps
	du=dp/ro/so
	r=ro+du*ro/so
	u=uo-is*du
	if(r.lt.rlim)r=1./(1./ro-du*du/dp)
	if(r.lt.0.)r=ro/2.
c-----------------
	call rimetst(n,po,ps,r,rs,u,du,us,emax-ds,de,iert)
c-----------------
	s0=so
	return
	endif
	du=dp/ro/so
	ro=rs+du*ro/so
	uo=us-is*du
c-----------------
	call rimetst(n,po,ps,ro,rs,uo,du,us,emax-ds,de,iert)
	if(iert.eq.1.)	goto2
	ds=ds+de
c-----------------


	if(ro.lt.rlim)then
c	write(*,3)ro,po,uo,so,dp,r0,p0,u0,s0,p
3	format( '--attension!- rim !!!:r<0.'/
     *'  ro / r0   ;  po / p0   ;  uo / u0   ;',
     *' so /s0     ;  dp / pk   ;',2(/(1x,5e13.6)))
c	write(*,*)'n=',n,'i=',j,'ik=',ik,' jj=',jj
	jj=jj+1
	if(jj.lt.5) goto 5
c	if(ro.lt.rlim)ro=rlim
	if(ro.lt.rlim)ro=1./(1./rs-du*du/dp)

	if(ro.lt.0.)then

	print*,' rim ro=',ro
	call adiwsav
	ro=rs/2.
	stop 36
	endif

	goto 2
	endif
	er=emax-ds
	call sum(er,ro,ppr,so,atw,n)
1	continue
2	continue
	s0=so
	r=ro
	u=uo
	return
	end

c *******isoentropy line    ********************======
	subroutine rim1(epsu)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	include 'ad.inc'
	include 'cdat.inc'
	include 'cfg.inc'
	data ipri/1/
	emax=ez1
	dp=pg-pz1
c-----------------
	ro=rz1
	uo=uz1
	so=az1
	po=pz1
	epsp=epsu*rz1*az1/2.
c	print *,'3 epsp=',epsp
	if(abs(dp).lt.epsp)then
	du=dp/ro/so
	uz1=uo-du
	rz1=ro+dp/az1/az1
	if(rz1.lt.rlim)rz1=1./(1./ro-du*du/dp)
	if(rz1.lt.0.)then
	if(ipri.eq.1)
     *	print*,' rim1 rz1=',rz1
	call adiwsav
	rz1=ro/2.
	stop 30
	endif
c-----------------
	call rimetst(nm1,pg,pz1,rz1,ro,uz1,du,uo,emax,de,iert)
c-----------------
	if(iert.eq.1)then
	ez1=0.
	else
	ez1=emax-de
	endif
	pz1=pg
	return
	endif
c-------------------------
	step=abs(dp)/epsp
c       if(pg.lt.0.)step=step/3.
	if(step.gt.99)step=99
	ik=step+1
	jj=1
5	continue
	ik=ik*jj
	dp=dp/(ik*1.)
	ds=0.
c- - - - - - - - - -
	do 1 j=1,ik
	ps=po
	rs=ro
	us=uo
	ss=so
	po=po+dp
	if(po.lt.0.d0.and.ps.gt.0.d0)then
	dp=pg-ps
	du=dp/ro/so
	uz1=uo-du
	rz1=ro+dp/so/so
	if(rz1.lt.rlim)rz1=1./(1./ro-du*du/dp)
	if(rz1.le.0.)then
	if(ipri.eq.1)
     *	print*,' rim1 rz1=',rz1
	call adiwsav
	stop 31
	endif
c-----------------
	call rimetst(nm1,pg,ps,rz1,rs,uz1,du,us,emax,de,iert)
	ds=de
c-----------------
	pz1=pg
	az1=so
	if(iert.eq.1)then
	ez1=0.
	else
	ez1=emax-de
	endif
	return
	endif
	du=dp/ro/so
	uo=uo-du
	roo=ro+dp/so/so
c	roo=1./(1./ro-du*du/dp)

	if(roo.lt.rlim)then
c	write(*,3)roo,po,uo,so,dp,rz1,pz1,uz1,az1,pg
3	format( '--attension!- rim1 !!!:r<0.'/
     *'  roo / rz1   ;  po / pz1   ;  uo / uz1   ;',
     *' so /az1     ;  dp / pk   ;',2(/(1x,5e13.6)))
c	write(*,*)'nm1=',nm1,'i=',j,'ik=',ik,' jj=',jj
	jj=jj+1
	if(jj.lt.5) then
	dp=pg-pz1
	ro=rz1
	uo=uz1
	so=az1
	po=pz1
	goto 5
	endif
	roo=1./(1./ro-du*du/dp)

	if(roo.lt.0.)then
	if(ipri.eq.1)
     *	print*,' rim1 ro=',roo
	call adiwsav
	roo=ro/2.
	stop 32
	endif
	goto 2
	else
	ro=roo
	endif
c-----------------
	call rimetst(nm1,po,ps,ro,rs,uo,du,us,emax-ds,de,iert)
	if(iert.eq.1.)	goto2
	ds=ds+de
c-----------------

c	call tota(po,ro,er,so,tr,nm1)
	er=emax-ds
	call sum(er,ro,ppr,so,atw,nm1)
1	continue
2	continue
	if(iert.eq.1)then
	ez1=0.
	else
	ez1=emax-ds
	endif

	rz1=ro
	uz1=uo
	pz1=pg
	az1=so
	return
	end

	subroutine rim12(epsu)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	include 'ad.inc'
	include 'bigpar.inc'
	include 'adch.inc'
	include 'cdat.inc'
	include 'cfg.inc'
	data ipri/0/
	if(p1.eq.p2)goto 2
	if(p1.lt.p2)then
	pg=p1
	if(nm2.eq.37)gmg=gma2
	if(nm2.eq.40)ctnt=ctnt2
	call rim2(epsu)
	else
	pg=p2
	if(nm1.eq.37)gmg=gma1
	if(nm1.eq.40)ctnt=ctnt1
	call rim1(epsu)
	endif

	h1=rz1*az1
	h2=rz2*az2

	ug=(pz1-pz2+h1*uz1+h2*uz2)/(h1+h2)
	pg=pz2-h2*(uz2-ug)
	if(pg.lt.plim)pg=pvacuum(pg)
	if(pg.gt.pz1)then
	ier=4
	return
	endif
2	continue
	dp=pg-pz1
c-----------------
	epsp=epsu*rz1*az1/2.
	epsp2=epsu*rz2*az2/2.
	if(epsp2.lt.epsp)epsp=epsp2

	if(abs(dp).lt.epsp)then
	uz1=uz1-dp/rz1/az1
	rzz=rz1+dp/az1/az1
	if(rzz.gt.0.) rz1=rzz
	pz1=pg
	uz2=uz2+dp/rz2/az2
	rzz=rz2+dp/az2/az2
	if(rzz.gt.0.) rz2=rzz
	pz2=pg
	if(abs(uz1-uz2).gt.epsu)ier=2
	return
	endif
c-------------------------
	delug=uz1-uz2
	ro1=rz1
	uo1=uz1
	so1=az1
	ro2=rz2
	uo2=uz2
	so2=az2
	po=pz1

	step=abs(dp)/epsp
	if(pg.lt.0.)step=step/3.
	if(step.gt.99)step=99

	ik=step+1.1
	dp=dp/(ik*1.)
	do 1 j=1,ik
	pa=po+dp

	du1=dp/ro1/so1
	ua1=uo1-du1
	rq1=ro1+dp/so1/so1
c	rq1=1./(1./ro1-du1*du1/dp)

	du2=dp/ro2/so2
	ua2=uo2+du2
	rq2=ro2+dp/so2/so2
c	rq2=1./(1./ro2-du2*du2/dp)

c       rq=ro+dp/so**2

	if(nm1.eq.37)gmg=gma1
	if(nm1.eq.40)ctnt=ctnt1
	call tota(pa,rq1,er1,sa1,tr1,nm1)
	if(nm2.eq.37)gmg=gma2
	if(nm2.eq.40)ctnt=ctnt2
	call tota(pa,rq2,er2,sa2,tr2,nm2)
c        if(ier.eq.1)then
c       ier=2
c       goto 3
c       endif

	pn=po+dp

	ss1=0.5*(sa1+so1)
	du1=dp/ro1/ss1
	un1=uo1-du1
	rzz=ro1+dp/ss1/ss1
	if(rzz.gt.0.) rn1=rzz

	ss2=0.5*(sa2+so2)
	du2=dp/ro2/ss2
	un2=uo2+du2
	rzz=ro2+dp/ss2/ss2
	if(rzz.gt.0.) rn2=rzz

	if((un1-un2)*delug.le.0.)then
	call str1(po,uo1,pn,un1,po,uo2,pn,un2,pg,ug)

	if(ier.eq.7)then

	q1=abs(un1-un2)
	q2=abs(uo1-uo2)
	if(ipri.eq.1)print*,' rim12 pg,po,pn,q1,q2',pg,po,pn,q1,q2

	ug=((un1+un2)/2.*q2+(uo1+uo2)/2.*q1)/(q1+q2)
	pg=(pn*q2+po*q1)/(q1+q2)
	if(ipri.eq.1)print*,' rim12 pg,ug,un1,un2,uo1,uo2',
     * pg,ug,un1,un2,uo1,uo2

	ier=0

	endif

	dp=pg-po
	pz1=pg
	rzz=ro1+dp/ss1/ss1
	if(rzz.gt.0.) rz1=rzz
c	du1=ug-uo1
c	rz1=1./(1./ro1-du1*du1/dp)
	uz1=ug

	pz2=pg
	rzz=ro2+dp/ss2/ss2
	if(rzz.gt.0.) rz2=rzz
c	du2=ug-uo2
c	rz2=1./(1./ro2-du2*du2/dp)
	uz2=ug

	if(nm1.eq.37)gmg=gma1
	if(nm2.eq.40)ctnt=ctnt1
	call tota(pz1,rz1,ez1,az1,tr1,nm1)
	if(nm2.eq.37)gmg=gma2
	if(nm2.eq.40)ctnt=ctnt2
	call tota(pz2,rz2,ez2,az2,tr2,nm2)
	return

	endif

	po=pn
	so1=sa1
	uo1=un1
	ro1=rn1
	so2=sa2
	uo2=un2
	ro2=rn2

1	continue
	ier=2
3	continue
	pz1=po
	rz1=ro1
	uz1=uo1
	az1=so1
	pz2=po
	rz2=ro2
	uz2=uo2
	az2=so2
	return
	end
	subroutine rim2(epsu)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	include 'ad.inc'
	include 'cdat.inc'
	include 'cfg.inc'
	data ipri/1/

	emax=ez2
	dp=pg-pz2
c-----------------
	ro=rz2
	uo=uz2
	so=az2
	po=pz2
	epsp=epsu*rz2*az2/2.
c	print *,'4 epsp=',epsp
	if(abs(dp).lt.epsp)then
	du=dp/ro/so
	uz2=uo+du
	rz2=ro+dp/az2/az2
	if(rz2.lt.rlim)rz2=1./(1./ro-du*du/dp)
	if(rz2.lt.0.)then
	if(ipri.eq.1)
     *	print*,' rim2 rz2=',rz2,' ro=',ro,' du=',du,' dp=',dp
	call adiwsav
	rz2=ro/2.
c	stop 33
	endif
c-----------------
	call rimetst(nm2,pg,pz2,rz2,ro,uz2,-du,uo,emax,de,iert)
c-----------------
	if(iert.eq.1)then
	ez2=0.
	else
	ez2=emax-de
	endif
	pz2=pg
	return
	endif
c-------------------------

	step=abs(dp)/epsp
	if(pg.lt.0.)step=step/3.
	if(step.gt.99)step=99
	ik=step+1
	jj=1
5	continue
	ik=ik*jj
	dp=dp/(ik*1.)
	ds=0.
	do 1 j=1,ik
	ps=po
	rs=ro
	us=uo
	ss=so
	po=po+dp
	if(po.lt.0.d0.and.ps.gt.0.d0)then
	dp=pg-ps
	du=dp/ro/so
	uz2=uo+du
	rz2=ro+dp/so/so
	if(rz2.lt.rlim)rz2=1./(1./ro-du*du/dp)
	if(rz2.lt.0.)then
	if(ipri.eq.1)
     *	print*,' rim2 rz2=',rz2
	call adiwsav
	stop 34
	endif
c-----------------
	call rimetst(nm2,pg,ps,rz2,rs,uz2,-du,us,emax,de,iert)
	ds=de
c-----------------
	pz2=pg
	az2=so
	return
	endif
	du=dp/ro/so
	uo=uo+du
	roo=ro+dp/so/so
c	roo=1./(1./ro-du*du/dp)

	if(roo.lt.rlim)then
c	write(*,3)roo,po,uo,so,dp,rz2,pz2,uz2,az2,pg
3	format( '--attension!- rim2 !!!:r<0.'/
     *'  roo / rz2   ;  po / pz2   ;  uo / uz2   ;',
     *' so /az2     ;  dp / pk   ;',2(/(1x,5e13.6)))
c	write(*,*)'nm2=',nm2,'i=',j,'ik=',ik,' jj=',jj
	jj=jj+1
	if(jj.lt.5)then
	dp=pg-pz2
	ro=rz2
	uo=uz2
	so=az2
	po=pz2
	goto 5
	endif
	ro=1./(1./ro-du*du/dp)
	if(ro.lt.0.)then
	if(ipri.eq.1)
     *	print*,' rim2 ro=',ro
	call adiwsav
	stop 35
	endif
	goto 2
	else
	ro=roo
	endif
c-----------------
	call rimetst(nm2,po,ps,ro,rs,uo,-du,us,emax-ds,de,iert)
	if(iert.eq.1.)	goto2
	ds=ds+de
c-----------------

c	call tota(po,ro,er,so,tr,nm2)
	er=emax-ds
	call sum(er,ro,ppr,so,atw,nm2)
1	continue
2	continue
	if(iert.eq.1)then
	ez2=0.
	else
	ez2=emax-ds
	endif
	rz2=ro
	uz2=uo
	pz2=pg
	az2=so
	return
	end

	function defrz1(ier)
c maximum of density for compression
c density for pressure on cold curve
	implicit real*8 (a-h,o-z)
	common /error12/ ier1,ier2
	include 'ad.inc'
	include 'cdat.inc'
	if(nm1.lt.50.or.nm1.ge.60)goto 11

	niv=nm1-49

	goto(1,2,3,4,5,6),niv
	print*,'defrz1 indefinite coldens'
	stop

1	continue
	call  coldens0(pg,coldr,ierc)
	if(ierc.ne.0)goto 11
	defrz1=coldr
	return

2	continue
	call  coldens1(pg,coldr,ierc)
	if(ierc.ne.0)goto 11
	defrz1=coldr
	return

3	continue
	call  coldens2(pg,coldr,ierc)
	if(ierc.ne.0)goto 11
	defrz1=coldr
	return

4	continue
	call  coldens3(pg,coldr,ierc)
	if(ierc.ne.0)goto 11
	defrz1=coldr
	return

5	continue
	call  coldens4(pg,coldr,ierc)
	if(ierc.ne.0)goto 11
	defrz1=coldr
	return

6	continue
	call  coldens5(pg,coldr,ierc)
	if(ierc.ne.0)goto 11
	defrz1=coldr
	return

11	continue
	defrz1=1000.
	return
	end

	function defrz2(ier)
c maximum of density for compression
c density for pressure on cold curve
	implicit real*8 (a-h,o-z)
	common /error12/ ier1,ier2
	include 'ad.inc'
	include 'cdat.inc'
	if(nm2.lt.50.or.nm2.ge.60)goto 11

	niv=nm2-49

	goto(1,2,3,4,5,6),niv
	print*,'defrz2 indefinite coldens'
	stop

1	continue
	call  coldens0(pg,coldr,ierc)
	if(ierc.ne.0)goto 11
	defrz2=coldr
	return

2	continue
	call  coldens1(pg,coldr,ierc)
	if(ierc.ne.0)goto 11
	defrz2=coldr
	return

3	continue
	call  coldens2(pg,coldr,ierc)
	if(ierc.ne.0)goto 11
	defrz2=coldr
	return

4	continue
	call  coldens3(pg,coldr,ierc)
	if(ierc.ne.0)goto 11
	defrz2=coldr
	return

5	continue
	call  coldens4(pg,coldr,ierc)
	if(ierc.ne.0)goto 11
	defrz2=coldr
	return

6	continue
	call  coldens5(pg,coldr,ierc)
	if(ierc.ne.0)goto 11
	defrz2=coldr
	return

11	continue
	defrz2=1000.
	return
	end

c ***********shock ad.************
	subroutine shock1(epsu)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	common /error12/ ier1,ier2
	include 'ad.inc'
	include 'cdat.inc'
	external hugon1
	data ipr/10/
	ier1=0
	dp=pg-p1

	call tota(pg,ra1,er1,a11,tr1,nm1)
	call ddrz(ra1,p1,a11,pg,rz1,drz,nm1)
	rzs=rz1
	epse=epsu*ra1
c	pgp1=pg+p1
c	if(pgp1.gt.epsu*1.d-7)then
c	epse=epsu*ra1
c	epse=epsu*1.d-10
c	else
c	epse=epsu*1.d-7
c	endif

	rz1max=defrz1(ier1)
	if(rz1max.le.ra1)then
	rz1max=100.d0*ra1
	end if
	if(p1.gt.0.)then
	call supord(rz1,hugon1,drz,rz1max,epse)
	else
c	rzz=ra1+dp/az1/az1
	call supord(rz1,hugon1,drz,rz1max,epse)
	endif

	if(ier.eq.3)then
	ier1=3
	return
	endif
	if(rz1.lt.ra1)then
	rzz=ra1+dp/a1/a1
	if(ipr.gt.0)then
	print *,
     *	' shock1 rz1=', rz1,' ra1=',ra1,' dp=',dp,' a1=',a1,
     *	' drz=',drz,' rzz=',rzz,' p1=',p1,' nm1=',nm1
	ipr=ipr-1
	endif
	rz1=rzz
	endif
	drr1=(1./ra1-1./rz1)
	duu1=dp*drr1
	if(duu1.gt.0.)then
	duu1=sqrt(duu1)
	else
	if(ipr.gt.0)then
	print *,' shock1 duu1=', duu1, ' dp=',dp
	ipr=ipr-1
	endif
	endif

	uz1=u1-duu1
	pz1=pg
	return
	end

	subroutine shock2(epsu)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	common /error12/ ier1,ier2
	include 'ad.inc'
	include 'cdat.inc'
	external hugon2
	data ipr/0/
	dp=pg-p2
	ier2=0
	call tota(pg,ra2,er2,a22,tr2,nm2)
	call ddrz(ra2,p2,a22,pg,rz2,drz,nm2)
	epse=epsu*ra2
c	pgp2=pg+p2
c	if(pgp2.gt.epsu*1.d-7)then
c	epse=epsu*pgp2*a2/ra2/ra2/ra2
c	epse=epsu*1.d-10
c	else
c	epse=epsu*1.d-7
c	endif
c	print *,'6 epse=',epse

	rz2max=defrz2(ier2)
	if(rz2max.le.ra2)then
	rz2max=100.d0*ra2
	end if

	if(p2.gt.0.)then
	call supord(rz2,hugon2,drz,rz2max,epse)
	else
c	rzz=ra2+dp/az2/az2
	call supord(rz2,hugon2,drz,rz2max,epse)
	endif
	if(ier.eq.3)then
	ier2=3
	return
	endif

	if(rz2.le.ra2)then
	rzz=ra2+dp/az2/az2
	if(ipr.gt.0)then
	ipr=ipr-1
	print *,' shock2 rz2=', rz2,' ra2=',ra2,' dp=',dp,' a2=',a2,
     *	' drz=',drz,' rzz=',rzz,' p2=',p2,' nm2=',nm2
	end if
	rz2=rzz
	endif
	drr2=(1./ra2-1./rz2)
	duu2=dp*drr2
	if(duu2.gt.0.)then
	duu2=sqrt(duu2)
	else

	if(ipr.gt.0)then
	ipr=ipr-1
	print *,' shock2 duu2=', duu2
	print *,' rz2=', rz2,' ra2=',ra2,' dp=',dp,' a2=',a2,
     *	' drz=',drz,' rzz=',rzz,' p2=',p2
	endif
	endif

	uz2=u2+duu2
	pz2=pg
	return
	end

	function hugon1(rz)
	implicit real*8 (a-h,o-z)
	common /error/ ier
c  input : e1,p1,ra1,nm1,pg,rz
	include 'ad.inc'
	include 'cdat.inc'
	ier=0
	if(rz.lt.0.999*ra1)then
	print*,' hugon1 rz1=',rz,' ra1=',ra1
c	rz=ra1
c	ier=5
	endif
	call tota(pg,rz,ez1,az1,tr,nm1)
	hugon1=ez1-e1-0.5*(pg+p1)*(1./ra1-1./rz)
	if(hugon1.gt.100.)hugon1=hugon1/10.+100.
	if(hugon1.lt.-100.)hugon1=hugon1/10.-100.
	return
	end

	function hugon2(rz)
	implicit real*8 (a-h,o-z)
	common /error/ ier
c  input : e2,p2,ra2,nm2,pg,rz
	include 'ad.inc'
	include 'cdat.inc'
	ier=0
	if(rz.lt.0.999*ra2)then
	print*,' hugon2 rz2=',rz,' ra2=',ra2
c	rz=ra2
c	ier=5
	endif
	call tota(pg,rz,ez2,az2,tr,nm2)
	hugon2=ez2-e2-0.5*(pg+p2)*(1./ra2-1./rz)
	if(hugon2.gt.100.)hugon2=hugon2/10.+100.
	if(hugon2.lt.-100.)hugon2=hugon2/10.-100.
cif(rz-ra2.gt.1.d-8)az2=sqrt((pg-p2)/(rz-ra2))
	return
	end

	function de(a,b,c,d)
	implicit real*8 (a-h,o-z)
	double precision a,b,c,d,de
	de=a*d-b*c
	return
	end
	subroutine str1(x1,y1,x2,y2,x3,y3,x4,y4,xg,yg)
	implicit real*8 (a-h,o-z)
	include 'bigpar.inc'
	common /error/ ier
	include 'am.inc'
	external de
	double precision a,b,c,d,e,f,dt,de
	double precision sx(4),sy(4),s,si,sa
	double precision x(4),y(4),ss(4)
	dimension ik(4),jk(4)
	data ic/0/
c	call errset(207,300,-1,1,1,208)
c      det(a,b,c,d,g)=a*g*d-b*g*c
	a=y2-y1
	b=x1-x2
	c=y4-y3
	d=x3-x4
c      g=1./sqrt(a*a+b*b+c*c+d*d)
	e=de(dble(y2),dble(x2),-a,b)
	f=de(dble(y4),dble(x4),-c,d)
	dt=de(a,b,c,d)
	if(dabs(dt).lt.1.d-17)goto1
	xg=de(e,b,f,d)/dt
	yg=de(a,e,c,f)/dt
	return
1	continue

	x(1)=x1
	x(2)=x2
	x(3)=x3
	x(4)=x4
	y(1)=y1
	y(2)=y2
	y(3)=y3
	y(4)=y4
	k=0
	do i=1,2
	do j=3,4
	k=k+1
	ik(k)=i
	jk(k)=j

	sx(k)=x(j)-x(i)
	sy(k)=y(j)-y(i)
	enddo
	enddo

	si=sx(1)*sx(1)+sy(1)*sy(1)
	sa=si
	iv=1
	jv=1
	ss(1)=si
	do i=2,4
	s=sx(i)*sx(i)+sy(i)*sy(i)
	ss(i)=s

	if(s.lt.si)then
	si=s
	iv=i
	endif
	if(s.gt.sa)then
	jv=i
	sa=s
	endif

	enddo
c--1
	if(a*a+b*b.gt.1.d-17)then
c 1_2 > 0
c--2
	if(c*c+d*d.gt.1.d-17)then
c 3_4 > 0 ; 1_2 > 0
	do i=1,4
c--3
	if(i.ne.iv.and.i.ne.jv)then
	do j=i+1,4
c--4
	if(j.ne.iv.and.j.ne.jv)then
c--5
	if(ss(j).gt.ss(i))then
	i2=i
	i3=j
	else
	i2=j
	i3=i
	endif
c--5
	goto 3
	endif
c--4
	enddo
	print*,' str1 1 its impossible'
	endif
c--3
	enddo
	print*,' str1 2 its impossible'
3	continue
c--3
	if(dabs(x(iv)*y(jv))-dabs(y(iv)*x(jv)).gt.1.d-17)then
	xg=(x(ik(iv))+x(jk(iv)))/2.
	yg=(x(ik(iv))+x(jk(iv)))/2.
	return
	endif
c--3
	endif
c--2
	endif
c--1
c 3_4=0 or 1_2 =0


	if(dabs(si).le.1.d-17)then
	xg=(x(ik(iv))+x(jk(iv)))/2.
	yg=(x(ik(iv))+x(jk(iv)))/2.
	return
	endif

4	continue
	if(ic.lt.3)print 2,x1,y1,x2,y2,x3,y3,x4,y4
2	format(' uncrossed straight lines lay on the '/
     *'       nodes  1,2 with coordinates'/4e12.5/
     *'  and nodes 3,4 with coordinates:'/4e12.5)
	yg=-10000000.*h3
	xg=-10000000.*h3
	ier=7
	ic=ic+1
	return
	end
c++++++++++++++supord
	subroutine supord(xg,f,step,xmax,eps)
c-----search solution f(xg)=0. in region (xg-step:xmax)
c-----s/r consists from : section , random , chord method
c------f in this case is hugoniot
	implicit real*8 (a-h,o-z)
	common /error/ ier
	external f
	data iter /100/
	data ipri/2/, iprcount/2/,iprc/0/

	ier=0
	iers=0
	i=0
	xmin=xg-step
	x1=xg
	f1=f(x1)
c	aa=11.
c	if(ier.ne.0.and.iers.eq.0) call random(x1,f1,step,f,aa)
c	if(ier.ne.0) iers=1
	if(abs(f1).lt.eps) goto 1
	step2=step/2.
	x2=x1+step2
	f2=f(x2)
	step2=0.9*step2

c	if(ier.ne.0.and.iers.eq.0) then
c	aa=11.
c	call random(x2,f2,step2,f,aa)

c	if(ier.ne.0) iers=1

	if(abs(x1-x2).lt.1.d-12)then
	x2=x1+1.d-12
	f2=f(x2)
	endif



	if(abs(f2).lt.eps) goto 2

	if(f1*f2.lt.0.)then
	f1=f(x1)
	f2=f(x2)

	if(f1*f2.lt.0.)then
	call hord(x1,x2,f1,f2,f,eps,xg)
	if(ier.eq.6)call adiwsav

	return
	endif

	endif

c---------section
	dmini=-1.d-3
c	dmaxi=1.d0*iter*dmini
	do 5 i=1,iter
	f12=f1-f2
	x12=x1-x2
	if(abs(x12/x1).lt.1.d-16)goto 1
	if(abs(f12).gt.1.d-18*abs(x12))then
	dd=x12/f12
	if(dd.gt.0.)then
	dd0=dd
	dmin=dmini*i
	dd=dmin
	end if
	else
	dd0=dd
	dmin=dmini*i
	dd=dmin
	endif
c	print*,' dd=',dd,' dmin=',dmin
c	if(dd.gt.0.) call catch
c	if(dd.gt.dmin)then
c	dd=dmin
c	else
c	endif

	a=1.+f1*dd/x1
	if(a.gt.0.)then
	xg=x1/a
	else
	xg=x1-dd*f1
	endif
	call limxg(xg,x1,x2,xmin,xmax)
	fg=f(xg)

	if(ipri.eq.2.and.i.gt.iter/2.and.iprc.lt.iprcount)then
	print*,i,xg,fg
	print*,'1',dd,x2,f2
	end if

	f1=fg
	x1=xg
	if(abs(f1).lt.eps) goto 1
	if(f1*f2.lt.0)then
	call hord(x1,x2,f1,f2,f,eps,xg)
	if(ier.eq.6)call adiwsav

	return
	endif

	f12=f1-f2
	x12=x1-x2
	if(abs(x12/x1).lt.1.d-16)goto 2
	if(abs(f12).gt.1.d-18*abs(x12))then
	dd=x12/f12
	if(dd.gt.0.)then
	dd0=dd
	dmin=dmini*i
	dd=dmin
	end if
	else
	dd0=dd
	dmin=dmini*i
	dd=dmin
	endif
c	if(f12/x12.gt.dmin)then
c	dd=dmin
c	else
c	dd=x12/f12
c	endif


	a=1.+f1*dd/x1
	if(a.gt.0.)then
	xg=x1/a
	else
	xg=x1-dd*f1
	endif
	call limxg(xg,x1,x2,xmin,xmax)
	fg=f(xg)
	if(ipri.eq.2.and.i.gt.iter/2.and.iprc.lt.iprcount)then
	print*,i,xg,fg
	print*,'2',dd,x2,f2
	end if
	f2=fg
	x2=xg
	if(abs(f2).lt.eps) goto 2
	if(f1*f2.lt.0)then
	call hord(x1,x2,f1,f2,f,eps,xg)
	if(ier.eq.6)call adiwsav

	return
	endif




5	continue

4	continue

	if(ipri.ge.1.and.iprc.lt.iprcount)then
	print*,' supord section convergence is not achived for ',i,
     *' iterations'
	print*,'xg=',xg,' fg=',fg
	print*,'x1=',x1,' f1=',f1
	print*,' x2=',x2,' f2=',f2,' eps=',eps
	print*,' dd=',dd,' dmin=',dmin,' dd0=',dd0
	print*,' xmin=',xmin,' step=',step
	iprc=iprc+1
	call adiwsav
	if(ipri.eq.2) call catch
	endif
	a=abs(f2)
	b=abs(f1)
	xg=(x1*a+x2*b)/(a+b)
	fg=f(xg)
	ier=3

	return
1	continue
	xg=x1
	fg=f(xg)
	return
2	continue
	xg=x2
	fg=f(xg)
	return
	end
	subroutine supordv(xg,f,step,eps)
c-----search solution f(xg)=0. in region (xg-step:xg+step)
c-----s/r concists from : section , random , chord methods
c------f in this case is hugoniot
	implicit real*8 (a-h,o-z)
	common /error/ ier
	external f
	data iter /100/
	data ipri/1/
	ier=0
	iers=0
	i=0
	x1=xg
	f1=f(x1)
	aa=11.
	if(ier.ne.0.and.iers.eq.0)
     *call random(x1,f1,step,f,aa)
	if(ier.ne.0) iers=1
	if(abs(f1).lt.eps) goto 1
	step2=step/2.
	x2=x1-step2
	f2=f(x2)
	step2=0.9*step2

	if(ier.ne.0.and.iers.eq.0) then
	aa=11.
	call random(x2,f2,step2,f,aa)
	if(ier.ne.0) iers=1

	if(abs(x1-x2).lt.1.d-4)then
	x2=x1+1.d-4
	f2=f(x2)
	endif

	endif

	if(abs(f2).lt.eps) goto 2

	if(f1*f2.lt.0.)then
	f1=f(x1)
	f2=f(x2)

	if(f1*f2.lt.0.)then
	call hord(x1,x2,f1,f2,f,eps,xg)

	if(ier.eq.6)then
	if(ipri.eq.1)
     *	print*,' supordv xg didn''t find'
	endif
	return

	endif

	endif

c---------section
	do 5 i=1,iter
	f12=f1-f2

	if(abs(f12).lt.0.1e-8*eps)then
c-----search of the region with d<0

	call snegrev(x1,f1,x2,f2,xg,fg,f,eps)
	if(ier.eq.3) goto4
	f12=f1-f2
	endif

	dd=(x1-x2)/f12
c------test dd
c-----dd must be less "0" in case of hugoniot adiabat

	if(dd.gt.0.)then
c-----search of the region with d<0

	call snegrev(x1,f1,x2,f2,xg,fg,f,eps)
	if(ier.eq.3) goto4
	f12=f1-f2
	dd=(x1-x2)/f12
	endif

	xg=x1-dd*f1


	if(xg.lt.0.001)then
	xg=0.001
	if(x1.eq.0.001)goto 4
	endif

	fg=f(xg)

	if(abs(fg).lt.eps) return
c--1
	if(f1*fg.lt.0.)then
	x2=xg
	f2=fg
	f1=f(x1)
	f2=f(x2)
	if(f1*f2.lt.0.)then
	call hord(x1,x2,f1,f2,f,eps,xg)
	if(ier.eq.6)then
	if(ipri.eq.1)
     *	print*,' supordv xg didn''t find'
	endif
	return
	endif
c--3
	endif

	if(f2*fg.lt.0.)then
	x1=xg
	f1=fg
	f1=f(x1)
	f2=f(x2)

	if(f1*f2.lt.0)then
	call hord(x1,x2,f1,f2,f,eps,xg)
	if(ier.eq.6)then
	if(ipri.eq.1)
     *	print*,' supordv xg didn''t find'
	endif
	return
	endif

	endif

	if(abs(f1).lt.abs(f2))then

	f2=fg
	x2=xg
	else

	f1=fg
	x1=xg
	endif

5	continue
4	continue
	if(ipri.eq.1)
     *	print*,' supordv i=',i,' x1=',x1,' f1=',f1,
     *	' x2=',x2,' f2=',f2,' xg=',xg,' fg=',fg
	a=abs(f2)
	b=abs(f1)
	xn=(x1*a+x2*b)/(a+b)
	ier=3
	return
1	continue
	xg=x1
	return
2	continue
	xg=x2
	return
	end
c++++++++++++++snegrev
	subroutine snegrev(x1,f1,x2,f2,xg,fg,f,eps)
	implicit real*8 (a-h,o-z)
	common /error/ ier
	include 'pfrom.inc'
	parameter (n=100)
c------search x1,x2,f1,f2 in region near x1 , x2
c----- wheare f(x) is determined and df/dx<0
c----------( region with negative slope)
c-----
	external f
	ex=x2+x1
	do 4 iez=1,2
	iv=2
	if(ex.lt.1.7)iv=1
1	continue
	goto(2,3,3,2,4,4),iv
2	continue
c---------treatment of the region r0-> 0

	if(x2.lt.x1)then
	stp=(x1-x2)/10.
	if(stp.lt.5.d-10)stp=5.d-10
	xp=x1
	fp=f1
	x0=x1

	else
	stp=x2-x1
	if(stp.lt.5.d-10)stp=5.d-10
	xp=x2
	fp=f2
	x0=x2
	endif

	do i=1,n

	x0=x0+stp
	if(x0.gt.vpend)x0=vpend
	f0=f(x0)
	f12=fp-f0
	if((ier.eq.0.or.iez.eq.2).
     *and.f12.gt.0.and.f12.gt.1.d-7*eps)then
c----- step to be shure that this is region with negative slope
	xp=x0
	fp=f0
	x0=x0+stp
	if(x0.gt.vpend)x0=vpend
	f0=f(x0)
	f12=fp-f0
	if((ier.eq.0.or.iez.eq.2).
     *and.f12.gt.0.and.f12.gt.1.d-7*eps)then
	x1=x0
	f1=f0
	x2=xp
	f2=fp
	return
	endif
	endif

	xp=x0
	fp=f0
	if(x0.eq.vpend)goto 7
	enddo

7	continue
	iv=iv+2
	goto 1
6	continue
	if(jj.gt.5)goto7
	xmin=x0+(jj-1)*1.d-9
	jj=jj+1
	goto 5
3	continue
c---------treatment of the region x0 -> +oo
	xmin=0.
	jj=1
5	continue
	if(x1.lt.x2)then
	if(x1.lt.xmin)goto 7
	stp=-(x2-xmin)/n
	xp=x2
	fp=f2
	x0=x2+stp
	else
	if(x2.lt.xmin)goto 7
	stp=-(x1-xmin)/n
	xp=x1
	fp=f1
	x0=x1+stp
	endif

	do i=1,n

	x0=x0+stp
	f0=f(x0)
	if(ier.eq.5)goto 6
	f12=f0-fp
	if((ier.eq.0.or.iez.eq.2).
     *and.f12.gt.0.and.f12.gt.1.d-7*eps)then
c---------- adding step to be shure that this is region with negative slope
	xp=x0
	fp=f0
	x0=x0+stp
	f0=f(x0)
	if(ier.eq.5)goto 6
	f12=f0-fp
	if((ier.eq.0.or.iez.eq.2).
     *and.f12.gt.0.and.f12.gt.1.d-7*eps)then
	x2=x0
	f2=f0
	x1=xp
	f1=fp
	return
	endif
	endif
	xp=x0
	fp=f0
	enddo
	iv=iv+2
	goto 1

4	continue
c-------search is not successful
	ier=3
	write(*,*)' snegrev search is not succesful'
	return
	end

c++++++++++++++supordd
	subroutine supordd(xn,f,fd,eps)
	implicit real*8 (a-h,o-z)
	common /error/ ier
c-----search solution f(xg)=0.
c-----s/r concists from : newton , random , chord methods
	external f,fd
	data ipri/1/
	data iter /100/
	jter=0
	ier=0
	iers=0
	x1=xn
	f1=f(x1)
	ier1=ier
	if(ier.eq.2)return
	if(abs(f1).lt.eps) return

c---------newton
	itvac=1
	x2=x1
	dmini=-1.d-1
	dmaxi=-1.d-1
	do 5 i=1,iter
	dmin=dmini+dmaxi/i

	fdd=fd(x1)
	if(x2.ne.x1)then
	fd2=(f2-f1)/(x2-x1)
	if(fd2.lt.fdd)then
	fdd=fd2
	else
	if(fd2.lt.0..and.fdd.lt.2.*fd2)fdd=2.*fd2
	endif
	endif
	if(fdd*fd2.lt.0.d0)then
	fdd=fdd/(30.d0+i)*30.d0
	endif
	if(fdd.gt.dmin)then
	fdd=dmin
	endif

	xg=x1-f1/fdd

	if(xg.lt.0.)then
	xg=pvacuum(xg)
	if(itvac.eq.1)then
	xgs=xg
	itvac=2
	else
	if(xg.eq.xgs)then
	ier=2
	return
	else
	xgs=xg
	endif
	endif
	endif

	fg=f(xg)
	ierg=ier
	if(abs(fg).lt.eps)then
	return
	else
	jter=jter+1
	if(jter.gt.2*iter)then
	print *,' supordd jter > 200'
	print *,' xg=',xg,' eps=',eps,' fg=',fg
	jter=0
	goto 4
	endif
	endif
c-2
	if(f1*fg.lt.0.)then
	f1=f(x1)
c-3
	if(f1*fg.lt.0.)then
	x2=xg
	f2=fg
	call hord0(x1,x2,f1,f2,f,eps,xn)
	return
	endif
c-3
	endif
c-2

	f2=f1
	x2=x1
	f1=fg
	x1=xg
	if(ipri.eq.1.and.i.gt.30)then
	print*,' supordd newton convergence  is not achived for ',i,
     *' iterations'
	print*,' xg=',xg,' fg=',fg,' fdd=',fdd,' dmin=',dmin
	print*,' x1=',x1,' f1=',f1,' fd2=',fd2
	print*,' x2=',x2,' f2=',f2,' eps=',eps
	call catch
	endif
5	continue
4	continue
	call adiwsav

	if(ipri.eq.1)then
	print*,' supordd newton convergence  is not achived for ',i,
     *' iterations'
	print*,' xg=',xg,' fg=',fg,' fdd=',fdd,' dmin=',dmin
	print*,' x1=',x1,' f1=',f1,' fd2=',fd2
	print*,' x2=',x2,' f2=',f2,' eps=',eps
	call catch
	endif

	a=abs(f2)
	b=abs(f1)
	xn=(x1*a+x2*b)/(a+b)
	ier=4
	return
	end

	subroutine adiwsav
	implicit real*8 (a-h,o-z)
	include 'bigpar.inc'
	include 'ad.inc'
	include 'adch.inc'
	include 'cdat.inc'
	include 'mai.inc'
	common /error/ ier
	data iop/1/
	if(iop.eq.1)then
	iop=2
c kfk   open(22,file='/inr265.adi.sav',
	open(22,file='adi.sav',
     *status='unknown',form='formatted')
	endif

	write(22,*)' ier ,kadr'
	write(22,*)ier,kadr
	write(22,*)' id '
	write(22,*)idadi
	write(22,*)' nm '
	write(22,*)nm1,nm2
	write(22,*)' ra '
	write(22,*)ra1,ra2
	write(22,*)' ez '
	write(22,*)ez1,ez2
	write(22,*)' rz '
	write(22,*)rz1,rz2
	write(22,*)' uz '
	write(22,*)uz1,uz2
	write(22,*)' az '
	write(22,*)az1,az2
	write(22,*)' eps '
	write(22,*)eps1,eps2
	write(22,*)' pz '
	write(22,*)pz1,pz2
	write(22,*)' drr '
	write(22,*)drr1,drr2
	write(22,*)' duu '
	write(22,*)duu1,duu2
	write(22,*)' rg,ug,eg,vpg,pg'
	write(22,*) rg,ug,eg,vpg,pg
	write(22,*)' vp '
	write(22,*)vp1,vp2
	write(22,*)' p '
	write(22,*)p1,p2
	write(22,*)' r '
	write(22,*)r1,r2
	write(22,*)' u '
	write(22,*)u1,u2
	write(22,*)' v '
	write(22,*)v1,v2
	write(22,*)' e '
	write(22,*)e1,e2
	write(22,*)' vb,vbg '
	write(22,*)vb,vbg
	write(22,*)' a '
	write(22,*)a1,a2
	write(22,*)' vi,plim,ei '
	write(22,*)vi,plim,ei
	end file 22
	back space 22
c	close(22)
	return
	end


	subroutine rimetst(nur,p,p0,r,r0,u,du,u0,emax,de,iert)
	implicit real*8 (a-h,o-z)
	data icpri/20/


	dv=1./r-1./r0
	de=(p+p0)/2.*dv

	if(de.gt.emax)then
	call ei0det(ei0,r0,nur)

	if(emax.lt.ei0)then
	emx=0.d0
	else
	emx=emax-ei0
	end if

	if(de.gt.emx)then
	iert=1

	dv=emx*2./(p+p0)
	rn=1./(dv+1/r0)
	un=u0-du*(r0-rn)/(r0-r)

	if((icpri.gt.0.and.emx.gt.1.d-6).or.rn.lt.0.)then

	print*,' rimetst de=',de,' > emax=',emx,' p=',p,' p0=',p0
	print*,' old u=',u,' new u=',un,'    u0=',u0
	print*,' old r=',r,' new r=',rn,'    r0=',r0
	icpri=icpri-1
c	call adiwsav
	endif

	u=un
	r=rn

	else
	iert=0
	endif

	else
	iert=0
	endif

	return
	end

	subroutine savz
	implicit real*8 (a-h,o-z)
	common /error/ ier
	include 'ad.inc'
	include 'cdat.inc'
c--------
	uo=ug
	po=pg
	ro1=rz1
	uo1=uz1
	so1=az1
	po1=pz1
	eo1=ez1
	ro2=rz2
	uo2=uz2
	so2=az2
	po2=pz2
	eo2=ez2
c--------
	entry initz
c--------
	rz1=ro1
	uz1=uo1
	az1=so1
	rz2=ro2
	pz1=po1
	ez1=eo1
	uz2=uo2
	az2=so2
	pz2=po2
	ez2=eo2
	ug=uo
	pg=po
c--------
	end
	subroutine limxg(xg,x1,x2,xmin,xmax)
	implicit real*8 (a-h,o-z)
c-1
	if(xg.lt.xmin)then

	if(x1.lt.x2.and.x1.gt.xmin)then
	xg=(x1+xmin)/2.
	elseif (x2.gt.xmin)then
	xg=(x2+xmin)/2.
	else
	xg=xmin

	endif

c-1
	else if (xg.gt.xmax)then

	if(x1.gt.x2.and.x1.lt.xmax)then
	xg=(x1+xmax)/2.
	elseif (x2.lt.xmax)then
	xg=(x2+xmax)/2.
	else
	xg=xmax
	endif

	endif
c-1
	if(xg.gt.5.*x1)xg=5.*x1
	return
	end

	subroutine savee1
	implicit real*8 (a-h,o-z)
	include 'ad.inc'
	include 'cdat.inc'
	character a*1
	r1o=r1
	u1o=u1
	a1o=a1
	p1o=p1
	e1o=e1
	r2o=r2
	u2o=u2
	a2o=a2
	p2o=p2
	e2o=e2
	return
	entry teste1(a)

	if(e1.ne.e1o)then
	print*,a
	print*,' e1=',e1,' e1o=',e1o
	call adiwsav
	call catch
	endif

	if(p1.ne.p1o)then
	print*,a
	print*,' p1=',p1,' p1o=',p1o
	call adiwsav
	call catch
	endif

	if(r1.ne.r1o)then
	print*,a
	print*,' r1=',r1,' r1o=',r1o
	call adiwsav
	call catch
	endif

	if(a1.ne.a1o)then
	print*,a
	print*,' a1=',a1,' a1o=',a1o
	call adiwsav
	call catch
	endif
	if(u1.ne.u1o)then
	print*,a
	print*,' u1=',u1,' u1o=',u1o
	call adiwsav
	call catch
	endif
	if(e2.ne.e2o)then
	print*,a
	print*,' e2=',e2,' e2o=',e2o
	call adiwsav
	call catch
	endif

	if(p2.ne.p2o)then
	print*,a
	print*,' p2=',p2,' p2o=',p2o
	call adiwsav
	call catch
	endif

	if(r2.ne.r2o)then
	print*,a
	print*,' r2=',r2,' r2o=',r2o
	call adiwsav
	call catch
	endif

	if(a2.ne.a2o)then
	print*,a
	print*,' a2=',a2,' a2o=',a2o
	call adiwsav
	call catch
	endif
	if(u2.ne.u2o)then
	print*,a
	print*,' u2=',u2,' u2o=',u2o
	call adiwsav
	call catch
	endif
	end

	subroutine ddrz(ra1,p1,a1,pg,rz1,drz,nur)

	implicit real*8 (a-h,o-z)
	parameter (g1=0.1d0,g2=0.2d0,pmin=-3.d-2)

	if(nur.ge.30.and.nur.lt.40)then
	d=p1/pg

	d1=g1*(d+1.)
	r1=ra1*(1.+d1)/(d+d1)

	d2=g2*(d+1.)
	r2=ra1*(1.+d2)/(d+d2)

	drz=r1-r2
	rz1=r2

	else
c	drz=ra1*0.1+0.1*dp
c	drz=ra1*0.02+0.02*dp
	clim=1.d-2

	if(a1.gt.clim)then
	drz=(pg-p1)/a1/a1*2./33.
	else
	drz=(pg-p1)/clim/clim*2./33.
c	drz=(pg-p1)/a1/a1*2./13.
c	else
c	drz=(pg-p1)/clim/clim*2./13.
	endif

	rz1=ra1+drz
	if(rz1.gt.1.1*ra1)rz1=1.1*ra1
	endif

	if(rz1.lt.ra1) then
	print*,' drz rz1 < ra1'
	print*,' ra1,rz1,pg,p1,a1,drz'
	print*,ra1,rz1,pg,p1,a1,drz
	rz1=1.01*ra1
	drz=rz1-ra1
	endif
	end
