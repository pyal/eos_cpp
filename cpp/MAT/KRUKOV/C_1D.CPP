/*
   ��������� ����  STEREO-PC.1d
   Copyright (c) 24.03.1991 SPI     All rights reserved.
*/
#include <dos.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <keys.h>
#include <twindow.h>
#include <dir.h>
#include <fstream.h>
#include <io.h>
#include "util.h"
#include <FCNTL.H>
#include <SYS\STAT.H>

#define TRUE 1   // Define some handy constants
#define FALSE 0   // Define some handy constants
#define PI 3.14159   // Define a value for PI 
#define ON 1   // Define some handy constants 
#define OFF 0   // Define some handy constants
#define CONT      77                    // �ਧ��� ���⠪⭮� �࠭��� 
#define VAK      177                    // �ਧ��� ᢮������  �࠭��� 
#define DLBL 4096 // ����� ��������� ����� � ����� 
#define DLUM 1024 // ����� ᮮ⢥�����饣� ���ᨢ� 
#define SZ   4    // ������ ᫮�� �⥭��/����� � ��⮪ 
#define CHKF 10   // ��������� �᫮ �����. ���

// ------------------------------------------------------- 
// ��ࠬ����, ��।��塞� � ����� ���. ������
// ------------------------------------------------------- 

#define MNCR        750         // ���ᨬ��쭮� ������⢮ �祥� �� �� R 
#define MPART       2*MNCR      // ���ᨬ��쭠� ����� ���ᨢ� ����      

#define CHSL 10 // ��������� �᫮ ⥫ ��� ᫮�� 
#define DLSL 11 // ����� char-���ᨢ�� ��� �࠭���� �ᥫ
    char s1[MAXPATH];
    char s2[80];
    char *namezad="?        ";     // ��� ����� 
    char *spath  ="��ப� ��� �ନ஢���� ��� ���᪠ �㦭��� 䠩��";
    size_t size;
     int frstent=0;
    FILE *stream;
   union FL {
         float f[DLUM];
          long l[DLUM];
          } fl;
    long ll, osl, nombl;
   int Kolk[CHSL];                   // ���-�� ����. 
 float Curs[CHSL] [CHKF];            // ���祭�� �����.

      // ��ࠬ���� ��� 
  char prr  [DLSL];            // ����襭��
  char chk  [DLSL];            // ��᫮ �1 
  char delt [DLSL];            // K2 
  char sho  [DLSL];            // ��� ����砭��
  char shz  [DLSL];            // ��� ����� 
  char vazmax  [DLSL];            // �離���� max 
  char vazmin  [DLSL];            // �離���� min 
  char obzro   [DLSL];            // ���⭮��� ���ࠧ�
  char obzv    [DLSL];            // ������� ���ࠧ� 
  char mkt     [DLSL];            // ��થ� ⥫� ��� ��࠭���� 
      // ����� ᯨ᪠ ��ࠬ��஢ ��� 

         // ���ᨢ� ��ࠬ��஢ ����
        int m [MPART+1];      // ��થ��                    
        int nb[MPART+1];      // ����� ����⢠             
       float r [MPART+1];      // R-���न���               
       float v [MPART+1];      // R-᪮����                 
       float ro[MPART+1];      // ���⭮���                  
       float e [MPART+1];      // ���ࣨ�                    
       float l [MPART+1];      // ������ �����             
       float p [MPART+1];      // ��������                   

         // ����� ���ᨢ�           
	  int c1[MNCR+2];
	float c2[MNCR+2];
	float c3[MNCR+2];
	float c4[MNCR+2];
	float c5[MNCR+2];

       // ��ࠬ���� ᫮�� 
   int nus [CHSL];            // ����� �ࠢ����� ���ﭨ� 
   int mrk [CHSL];            // ��થ�� 
 float sr1 [CHSL];            // ���� ࠤ���� 
 float sr2 [CHSL];            // �ࠢ� ࠤ���� 
 float sv  [CHSL];            // ������ 

	int i, i1, i2, j, j1, k, ii, i1or, i2or, index;
	int rad, nstp=0, stzap, izap=0, nzap=0, markt=0;
	int np, os, kolp, drob, ksl;
	int zgr1, zgr2, ncr, part;
	float robez, ebez, lbez, tbez, vbez, pbez, t, tstp;
	float razresh, kur, dt;
	float sumsk = 1.0, msk = 1.0, delta;
	float w1, w2, w3, w4, w5, w6, w7, w8, w9, w10;
	float ros, es, gamma, cs, ro0, a, lambda, dlina;
	int ststp, kchsl;
	float kvisc, minvisc, maxvisc;
	float pkr, vr, fr, pvr, rr, krdr, eps;
	  int x, y, xgr, height;
	  int grl, grt, grr, grb;
	  float rot, rob, pt, pb, vt, vb, et, eb;
	  int lxwind, lywind;
	  float stepx, realx, realy, niz, verh;
	  float massa, impulse, energy, q;
	    int un;
	  float uro, uc0, ul, ug, ue0;
      // ����७��� ��楤���
//	void kadr_0();
	void kadr_im();
   int schitn(void);
	 int schito();
//  void im_ext(char *s, char *namezad, char ext[]);
	void contact(int s, int *n);
 float eos(int nt, int *nb, float ro, float e, float *cs);
	 int pause(void);
   int OP_F ( char *namezad, char ext[], int mode);
//   int WR_FL ( long nbl, long nsl, long dl, int mode);
 //  -------   � � � � �  � � � � � � � �  -------


void main(int argc, char *argv[])
{
      // -------------------------------------------------------
      //               ���� ��砫��� ������
      // -------------------------------------------------------

//      if (strcmp(argv[2], "dialog")==0)  // �室 �� �ணࠬ�� "������"
	 strcpy(namezad, argv[1]);
  kadr_im();         // ��אַ� �室
  i=argc;
   i=schitn();     // ���뢠��� ��砫쭮� ���⠭���� 
          if (i!=0) {cout<<"  ��� ��砫쭮� ���⠭���� !  ";exit(1);}
   i=schito();     // ������� ��� ��ᬮ�� ��ࠬ��஢ ���
          if (i!=0) {cout<<"  �� ���� �������� � 䠩�� <.�> !  ";exit (1);}

	  printf("\n��砫� ��� �����  %s\n\n",namezad);

	  razresh = atof(prr);
	  kur     = atof(chk);
	  delta   = atof(delt);
	  ststp   = atoi(sho);
	  stzap   = atoi(shz);
	  maxvisc = atof(vazmax);
	  minvisc = atof(vazmin);
	  markt   = atoi(mkt);
    ncr     = (int)(dlina*razresh);
    if (ncr>MNCR) {
       cout<<"  ������⢮ �祥� ���⭮�� ���� ����� �����⨬��� !!  ";
       exit(1);}
  	part    = 2*ncr;

        // ����⠭�� ����ࠧ��ਢ����
 lbez = 1/razresh;    // ����� �祩�� (��)
 robez= atof(obzro);  // ���⭮��� (�/�**3)
 vbez = atof(obzv);   // ������� (��/�)
 ebez = vbez*vbez;    // ���ࣨ� (��/�)**2
 tbez = lbez/vbez;    // �६� (���)
 pbez = robez*vbez*vbez; // �������� (���)

 eps  =    1e-10;     // ��१���� �ந�������
 pkr  =   -1.0/pbez;  // ��࠭�祭�� ᭨�� �� ��������
 krdr =    0.01;      // ���਩ �஡����� �����

      // -------------------------------------------------------
      //               ������� ��ࠬ��஢ ����
      // -------------------------------------------------------
	for (i=0; i<=part; i++) {
	    m[i] = 0;
	   nb[i] = 0;
	    r[i] = 0.0;
	    v[i] = 0.0;
	   ro[i] = 0.0;
	    e[i] = 0.0;
	    l[i] = 0.0;
     p[i] = 0.0; }  // ����� ���⪨ ��ࠬ��஢ ����

for (np=0,k=0; k<ksl; k++)  {          // ���� �� ᫮�
    if (nus[k]==1)   index=1;            // ��.���

    if (nus[k]==2) { index=3;            // ���
       Curs[k][0]/=robez;
       Curs[k][1]/=pbez;}

    if (nus[k]==3) { index=3;            // D-u 
       Curs[k][0]/=robez;
       Curs[k][1]/=vbez;}

       uro=Curs[k][index]/robez;       // ��砫쭠� ���⭮��� 
       ue0=Curs[k][index+1]/ebez;      // ��砫쭠� ���ࣨ� 

    kchsl=(sr2[k]-sr1[k])*razresh;     // ������⢮ ���� � ⥫�
cout<<" kchsl "<<kchsl<<" sr1[k] "<<sr1[k]<<" sr2[k] "<<sr2[k]<<" razresh "<<razresh;
 for (j=0,i=np+1; i<=np+kchsl; j++,i++) {    // ������� ��ࠬ��஢ ���� 
     m[i] = mrk[k]-1;      // ��થ��: 0-��ࢮ� ⥫�, 1-��஥... !!! 
	   nb[i] = nus[k];
	    r[i] = (sr1[k]*razresh+j+0.5);
	    v[i] = sv[k]/vbez;
	   ro[i] = uro;
	    e[i] = ue0+v[i]*v[i]/2.0;
	    l[i] = 0.5;
     p[i] = 0.0; }  // ����� 横�� ������� ��ࠬ��஢ ���� 

  np+=kchsl;                          // ��饥 �᫮ ���� 
     } // ����� 横�� �� ᫮� 

      // ======================================================= 
      //               ��砫� ���� 蠣� �� �६���            
      // ======================================================= 
begcalc:
 dt = 0.033*nstp;   // �����祭�� dt �� ��ଠ�쭮�� �� 30 蠣�� 
cout<<" SumSk "<<sumsk;
	w1 = kur/sumsk;
	if (dt>w1) dt=w1;
	if (dt*msk>delta)  dt = delta/msk;
	if (kur>10.0)  dt = kur/1000.0/tbez;
	sumsk = -975;
	msk   =    0.0;
	kvisc = maxvisc;
cout<<" D_T "<<dt<<" W1 "<<w1<<" msk "<<msk<<" kur "<<kur<<" np "<<np<<"\n";
      // ------------------------------------------------------- 
      //             ����� ����ࣥ�樨 ��� �離���             
      // ------------------------------------------------------- 
      //  ���⪠ ����� ���ᨢ��  
	 for (i=0; i<=ncr; i++) {
	 c1[i] = 0;
	 c2[i] = 0.0;
	 c3[i] = 0.0;
  c4[i] = 0.0; } // ����� 横�� ���⪨ ����� ���ᨢ�� 
       //  ���� �� ���栬          
	 for ( i=1; i<=np; i++) {
	 j = (int)(r[i]);
// c1 - ��થ��
//	 �2 - �㬬� v*ro
//	 c3 - �㬬� ro
//	 c4 - �⮣���� ᪮����
   w1 = ro[i];              // ���⭮���
   w4 = v[i]*w1;            // ���⭮��� ������
       // � ���᪮� ��砥 ࠧ���� ����� 
	  if (rad == 0) {
			w1 *= 2*l[i];
			w4 *= 2*l[i]; }
	  c1[j] = c1[j] | m[i];
   c1[j+1] = c1[j+1] | m[i]; // M�ન஢�� ??? 
	  c2[j] += w4;  c3[j] += w1;
	  c2[j+1] += w4;  c3[j+1] += w1;
   }                        //  ����� 横�� �� ���栬   
      // ------------------------------------------------------- 
      //             ��室�� ���祭�� ᪮���                   
      // ------------------------------------------------------- 
	  for (j=0; j<=ncr; j++)
	      {if (c3[j] != 0.0) c4[j] = c2[j]/c3[j];}
       // ������� �࠭��� - v=0 
	  if (zgr1 == 1) c4[0] = 0.0;
	  if (zgr2 == 1) c4[ncr] = 0.0;
       // ���⠪�� �࠭��� ??

       // �������� �࠭���  ??

      // ------------------------------------------------------- 
      //                 ����� ᨫ����� �⠯�                   
      // ------------------------------------------------------- 
      //  ���⪠ ����� ���ᨢ��  
cout<<" Force ";
	 for (i=0; i<=ncr; i++) {
	 c2[i] = 0.0;
	 c3[i] = 0.0;
  c5[i] = 0.0; } // ����� 横�� ���⪨ ����� ���ᨢ�� 
        //     ���� �� ���栬      
	 for ( i=1; i<=np; i++) {
	 j = (int)(r[i]);
  es = e[i] - v[i]*v[i]/2.0; //     ����७��� ���ࣨ�    
	 if (es<0.0) es = 0.0;
  ros = ro[i];               //     ���⭮���             

	 w3 = eos(m[i], &nb[i], ros, es, &cs);
//cout<<" eos "<<w3<<" ros "<<ros<<" es "<<es;

  vr = c4[j+1] - c4[j];      // �ந������� ᪮���      
  w1 = fabs(vr);             // ��१���� �ந�������
	 if (w1<eps) vr=0.0;
  w7 = -kvisc*vr*ro[i]*cs;   // �����⢥���� �離����    
	 w9 = fabs(v[i]);
  w5 = cs + (w9);            // ����. ᪮���� ᨣ����    
	 if (sumsk<w5) sumsk = w5;
	 if (msk<w9) msk = w9;
  w3 += (w7);                // �������� + �離����       
  w10= 1/ro[i];              // ������ ��ꥬ �����    
  if (w3<pkr) w3 = pkr;      // ��࠭�祭�� ����. ����.  
  p[i] = w3;                 // �������� - � ���ᨢ       
// c2 - �㬬� ��������
//	 �3 - �㬬� 㤥���� ��ꥬ��
//	 c5 - �⮣���� �������� */
  c2[j] += (w3*w10); c3[j] += (w10);
  c2[j+1] += (w3*w10); c3[j+1] += (w10);
  }                         //  ����� 横�� �� ���栬
      // -------------------------------------------------------
      //             ��室�� ���祭�� ��������
      // ------------------------------------------------------- 
	  for (j=0; j<=ncr; j++)
	      {if (c3[j] != 0) c5[j] = c2[j]/c3[j];}
       // �������� �� �. ��. = 0    
	  for (j=1; j<=ncr-1; j++)
	       if (c3[j] == 0) {c5[j-1] = 0; c5[j+1] = 0;}
      // ------------------------------------------------------- 
      //             ����� �஬������� ᪮��⥩              
      // ------------------------------------------------------- 
      //  ���⪠ ����� ���ᨢ��  
	 for (i=0; i<=ncr; i++) {
	 c2[i] = 0.0;
	 c3[i] = 0.0;
  c4[i] = 0.0; } // ����� 横�� ���⪨ ����� ���ᨢ�� 
       //  ���� �� ���栬          
	 for ( i=1; i<=np; i++) {
	 j = (int)(r[i]);
//       �2 - �㬬� v*ro
//	 c3 - �㬬� ro
//	 c4 - �⮣���� ᪮���� */
   fr = c5[j+1] - c5[j];    // �ந������� ��������       
  w1 = fabs(fr);            // ��१���� �ந�������
	 if (w1<eps) fr=0.0;
   w9 = v[i] - fr*dt/ro[i]; // ����� ᪮����             
   w1 = ro[i];              // ���⭮���                  
   w8 = (w9+v[i])/2.0;      // �����㬬� ��. � ���. ᪮�. 
   w4 = w8*w1;              // ���⭮��� ������         
 v[i] = w9;                 // ���࠭��� ᪮����         
       // � ���᪮� ��砥 ࠧ���� ����� 
	  if (rad == 0) {
			w1 *= (2*l[i]);
			w4 *= (2*l[i]); }

	  c2[j] += w4;  c3[j] += w1;
	  c2[j+1] += w4;  c3[j+1] += w1;
   }                        //  ����� 横�� �� ���栬   
      // ------------------------------------------------------- 
      //             ��室�� ���祭�� ����� ᪮��⥩
      // ------------------------------------------------------- 
	  for (j=0; j<=ncr; j++)
	      {if (c3[j] != 0) c4[j] = c2[j]/c3[j];}
       // ������� �࠭��� - v=0
	  if (zgr1 == 1) c4[0] = 0.0;
	  if (zgr2 == 1) c4[ncr] = 0.0;
      // ------------------------------------------------------- 
      //             ����� ����� ��ࠬ�⮢ ����               
      // ------------------------------------------------------- 
        //     ���� �� ���栬      
     for ( i=1; i<=np; i++) {
	 j = (int)(r[i]);
  vr = c4[j+1] - c4[j];      // �ந������� ᪮���      
  w1 = fabs(vr);             // ��१���� �ந�������     
	 if (w1<eps) vr=0.0;
	 w9 = c5[j+1] * c4[j+1];
	w10 = c5[j] * c4[j];
 pvr = w9 - w10;             // �ந������� pv ��� ���ࣨ�
  w1 = fabs(pvr);            // ��१���� �ந�������     
	 if (w1<eps) pvr=0.0;
        // ����ࣥ��� pv            
	 if (rad==1) pvr += (w9+w10)/r[i]/2.0;
  w6 = (c4[j+1]+c4[j])/2.0;  // ������� ��६�饭�� ���.
  e[i] -= (pvr*dt/ro[i]);    // ����� ������ ���ࣨ�
        // ���� ��ꥬ              
	 w9 = l[i];
	 if (rad==1) w9 *= r[i];
  l[i] *= (1+vr*dt);         // ���� ࠧ��� �����      
    rr = r[i]+(w6*dt);       // ����� ���न���          
        // �⭮襭�� ���. ��. � ���.
	 w9 /= l[i]; if (rad==1) w9 /= rr;
  ro[i] *= w9;               // ����� ���⭮���           
         r[i] = rr;                 // ����ᥭ�� ���न����      
      // ------------------------------------------------------- 
      //            ����������⢨� � �࠭�栬� ����              
      // ------------------------------------------------------- 
      if (j==0) { // �࠭�� R = 0 
  w2 = rr-l[i];             // ������  �࠭�� �����    
	 i1or=(w2<-krdr) ? 1 : 0;
  if (i1or) {          // ���嫥��  !
     if (zgr1==1) {      // ����⢨� �� �����⮩ �࠭�� 
        v[i] = 0.0;            // ����� ��⠭���������� 
        } // ����� �����⮩ �࠭��� R = 0
     } // ����� ���嫥��
  }    // ����� �࠭��� R = 0 
      if ((j+1)==ncr) { // �࠭�� R = ncr 
  w2 = rr+l[i];             // ������ �࠭�� �����    
  if ((w2-ncr)>krdr) {      // ���嫥�� !                
     if (zgr1==1) {      // ����⢨� �� �����⮩ �࠭�� 
        v[i] = 0.0;            // ����� ��⠭���������� 
        } // ����� �����⮩ �࠭���  R = ncr
     } // ����� ���嫥��      
  }    // ����� �࠭��� R = ncr 
     }                         //  ����� 横�� �� ���栬   
      // ------------------------------------------------------- 
      //                 �⠯ �஡����� ����                   
      // ------------------------------------------------------- 
     ii = part;                     // ��� ���ᨢ�� ����       
     kolp = 0;                      // ���稪 �᫠ ����� ����

        //     ���� �� ���栬      
     for ( i=np; i>=1; i--) {    // ��稭�� � ��᫥���� � ��砫� 
	 j = (int)(r[i]);
  w1 = r[i] - l[i];          // R ������
  w2 = r[i] + l[i];          // R ������                 
        // ���� �� �஡����� �����   
  w4 = w2 -(j+1);            // ����� � ���孥� �祩��    
  if (w4>krdr) { // �஡����� ����� - ��९��� � �ࠢ��   
	    m[ii]=m[i]; nb[ii]=nb[i]; v[ii]=v[i];
	    ro[ii]=ro[i]; e[ii]=e[i];
	    l[ii]=w4/2.0; r[ii]=w2-l[ii];
     w2 = j+1;                // ���孨� ࠤ��� ��७���  
     ii--; kolp++;} // ����� �஡����� ����� 
        // ���� �� �஡����� ����    
	 w4 = j - w1;
	 drob = w4>krdr ? 1 : 0;
        // ��९��� ��७��� ����� 
	 m[ii]=m[i]; nb[ii]=nb[i]; v[ii]=v[i];
	 ro[ii]=ro[i]; e[ii]=e[i];
	 w5 = drob==1 ? j : w1;
	 l[ii]=(w2-w5)/2.0; r[ii]=w2-l[ii];
  ii--; kolp++;              // ����� �ࠢ�� ��७���    
  if (drob==1) {// �஡����� ���� - ��९��� � �ࠢ��    
	    m[ii]=m[i]; nb[ii]=nb[i]; v[ii]=v[i];
	    ro[ii]=ro[i]; e[ii]=e[i];
	    l[ii]=(j-w1)/2.0; r[ii]=j-l[ii];
     ii--; kolp++;}           // ����� �஡����� ����     
     }                         //  ����� 横�� �� ���栬   
cout<<" Order ";
      // ------------------------------------------------------- 
      // ����� ����� ���� 横� �� 㯮�冷稢���� ���� ???      
      // ------------------------------------------------------- 
      // ------------------------------------------------------- 
      //                �⠯ ��ꥤ������ ����                  
      // ------------------------------------------------------- 
      k = 1;                        // ��砫� ��ꥤ������� ����
      i = ii+1;                     // ��砫� ࠧ�஡������� �.  
        //     ���� �� ���栬      
   loopp:
	 if (i>part) goto endloopp;
        // ��९��뢠�� ��।���
	 m[k]=m[i]; nb[k]=nb[i]; v[k]=v[i];
	 ro[k]=ro[i]; e[k]=e[i]; l[k]=l[i]; r[k]=r[i];
  j = (int)(r[i]);           // ���� 㧥�                
  i1 = m[i];                 // ��થ�
    sled:i++;
	 j1 = (int)(r[i]);
	 i2 = m[i];
  if ((j==j1) && (i1==i2)) { // ��ꥤ������               
        // ����� ����              
	    w3 = 2*ro[k]*l[k]; w4 = 2*ro[i]*l[i];
	    if (rad==1) {w3*=(2*PI*r[k]); w4*=(2*PI*r[i]);}
     w5 = w3 + w4;           // �㬬�ୠ� ����           
        // ��ࠬ���� ��ꥤ������     
	    r[k] = (r[k]+r[i]-l[k]+l[i])/2.0;
	   ro[k] = w5/(w3/ro[k]+w4/ro[i]);
	    v[k] = (w3*v[k]+w4*v[i])/w5;
	    e[k] = (w3*e[k]+w4*e[i])/w5;
	    l[k] += l[i];
     goto sled; } // ����� ��ꥤ������ 
	 k++;
	 goto loopp;
endloopp: np = k-1;
      // ���⪠ ��⠢襩�� ��� ���ᨢ�� 
	for (i=k; i<=part; i++) {
	    m[i] = 0;
	   nb[i] = 0;
	    r[i] = 0.0;
	    v[i] = 0.0;
	   ro[i] = 0.0;
	    e[i] = 0.0;
	    l[i] = 0.0;
     p[i] = 0.0; }  // ����� ���⪨ ��ࠬ��஢ ����   

      t += dt; nstp++; izap++;
cout<<" Compare to write? nstp "<<nstp<<" izap "<<izap;
   if (nstp==1) { // ���뢠�� 䠩� ��� ����� ⠡��� 
             {char tmp[256];SetExt(namezad,tmp,"t");}//ii = open(tmp, O_CREAT,S_IWRITE);
              ii=OP_F(namezad, ".t", 1);        
             if (ii<0) {
                 cout<<"����� ������ 䠩� <.t> �� ������ ! "<<ii<<"\n";exit(1);}}

      if ((nstp==1)||(izap==stzap)) {  // ������ १���⮢ �� ��� 
      if (izap==stzap) {izap=0; nzap++;}
      // -------------------------------------------------------
      //                 ���ᨢ� ��� ��ᢥ稢����                
      // ------------------------------------------------------- 
      //  ���⪠ ����� ���ᨢ��  
	 for (i=0; i<=ncr; i++) {
	 c1[i] = 0;
	 c2[i] = 0.0;
	 c3[i] = 0.0;
	 c4[i] = 0.0;
	 c5[i] = 0.0;
       }  // ����� 横�� ���⪨ ����� ���ᨢ�� 
	 massa=0.0000001;
  impulse=1.0000001;
  energy=1.0000001;
       //  ���� �� ���栬          
	 for ( i=1; i<=np; i++) {
	 j = (int)(r[i]);
//c1 - ���
//�2 - ���⭮���
//c3 - ��������
//�4 - ᪮����
//c5 - ����७��� ���ࣨ�
	  c1[j]++; c1[j+1]++;
	  c2[j] += ro[i]; c2[j+1] += ro[i];
	  c3[j] += p[i];  c3[j+1] += p[i];
	  c4[j] += v[i];  c4[j+1] += v[i];
	  c5[j] += e[i]-v[i]*v[i]/2.0;
    c5[j+1] += e[i]-v[i]*v[i]/2.0;
        // ���࠭���� 
	   q = (rad==0) ? 2 : 4*PI*r[i];
	   i1or=(markt==0) ? 1 : 0;
	   i2or=(markt==m[i]) ? 1 : 0;
	   if (i1or || i2or) {
		massa += (q*l[i]*ro[i]);
	      impulse += (massa*v[i]);
	       energy += (massa*e[i]);

        } // ������ ��࠭����
   }                        //  ����� 横�� �� ���栬   
      // ------------------------------------------------------- 
      //             ��室�� ���祭�� ��ࠬ��஢                 
      // -------------------------------------------------------
	  for (j=0; j<=ncr; j++) {
	      if (c1[j] != 0) {
		  c2[j] =robez*c2[j]/c1[j];
		  c3[j] = pbez*c3[j]/c1[j];
		  c4[j] = vbez*c4[j]/c1[j];
  	  c5[j] = ebez*c5[j]/c1[j];
		  }}
       // ������� �࠭��� - v=0 
	  if (zgr1 == 1) c4[0] = 0.0;
	  if (zgr2 == 1) c4[ncr] = 0.0;
       // �������� �࠭���-p=0  
	  for (j=1; j<ncr; j++) {
	       if (c1[j] == 0) {
	       c3[j-1] = 0.0; c3[j+1] = 0.0; }
        } // ����� 横�� ���᪠ ᢮������ �࠭��      

	       w1=t*tbez;
	       size=sizeof(nzap); fwrite(&nzap, size, 1, stream);
				  fwrite(&nstp, size, 1, stream);
				  fwrite(&ncr, size, 1, stream);
				  fwrite(&markt, size, 1, stream);
                                  ii=ncr+1;
	       size=sizeof(t);    fwrite(&w1, size, 1, stream);
				  fwrite(&dlina, size, 1, stream);
				  fwrite(&massa, size, 1, stream);
				  fwrite(&impulse, size, 1, stream);
				  fwrite(&energy, size, 1, stream);
		fwrite(&c2, size, ii, stream);
		fwrite(&c3, size, ii, stream);
		fwrite(&c4, size, ii, stream);
		fwrite(&c5, size, ii, stream);
printf(" ������ %d  Step = %d  t(mks) = %f  dt(mks) = %f\n",nzap,nstp,t*tbez,dt*tbez);
//pause();
   } // ����� ����� १���⮢ �� ��� 

      // ------------------------------------------------------- 
      //                ��� �� �६��� �����襭
      // ------------------------------------------------------- 

      if ((i=kbhit())!=0) {
printf("\n ��㧠...\n ����� *%s*  Step = %d  t(mks) = %f  dt(mks) = %f\n",
	namezad,nstp,t*tbez,dt*tbez);
       ii=pause(); if (ii==1) {
                      fclose(stream);exit(0);}}      // ����� ������ 

      if (nstp==ststp) { fclose(stream); exit(0);} // K���� ������ 
cout<<" After write ?  goto BegCalc \n";
      goto begcalc;

}   // ����� MAIN - �ணࠬ�� ================================== 

// ============================================================== 
       int schito(void)   // ���뢠���  ��� ������ <namezad>.o   
       {   
        int sig=5;   // Number digits after commar
        char tmp[256];SetExt(namezad,tmp,"o");
        fstream input=fstream(tmp,ios::in);
        if (input)
           {input>>tmp>>prr>>tmp>>chk>>tmp>>delt>>tmp>>sho>>tmp>>shz
                >>tmp>>vazmax>>tmp>>vazmin>>tmp>>obzro>>tmp>>obzv>>tmp>>mkt;
            input.close();}
        else { // �ਭ㤨⥫쭮� ������� 
         gcvt(razresh, sig, prr);
         gcvt(0.5    , sig, chk);
         gcvt(0.5    , sig, delt);
         itoa(100    , sho, 10);
         itoa(10     , shz, 10);
         gcvt(1.5    , sig, vazmax);
         gcvt(1.5    , sig, vazmin);
         gcvt(robez  , sig, obzro);
         gcvt( vbez  , sig, obzv);
         itoa(0      , mkt, 10);
        } 
         fstream output=fstream(tmp,ios::out);
         output<<"����࠭�⢥����_ࠧ�襭�� "<<prr<<" ��⮩稢����_K1 "
             <<chk<<" ��⮩稢����_K2 "<<delt<<"\n���_����砭�� "<<sho
             <<" ���_����� "<<shz<<"\n�離����_min "<<vazmax
             <<" �離����_max "<<vazmin<<"\n����ࠧ��ਢ����_���⭮��� "<<obzro
             <<" ����ࠧ��ਢ����_᪮���� "<<obzv<<" ��થ�_⥫� "<<mkt;
         cout<<"����࠭�⢥����_ࠧ�襭�� "<<prr<<" ��⮩稢����_K1 "
             <<chk<<" ��⮩稢����_K2 "<<delt<<"\n��� ����砭�� "<<sho
           <<" ��� ����� "<<shz<<"\n�離����_min "<<vazmax
           <<" �離����_max "<<vazmin<<"\n����ࠧ��ਢ����_���⭮��� "<<obzro
           <<" ����ࠧ��ਢ����_᪮���� "<<obzv<<" ��થ�_⥫� "<<mkt;
      return 0;
      }  // ����� ��楤��� ���뢠��� � ����� ��ࠬ��஢ ���
// =================================================================
       // �������� ��ப� ��� ⥪�饩 ��४�ਨ <s> = <file>.<ext>
/*
   void im_ext(char *s, char *file, char ext[])
    {
    char drive[MAXDRIVE];
    char dir[MAXDIR];
    char fi[MAXFILE];
    char ex[MAXEXT];

    getcwd(s,MAXPATH);              // get the current working directory
    strcat(s,"\\");                  // append on a trailing \ character
    fnsplit(s,drive,dir,fi,ex); // split the string to separate elems
    strcpy(fi,file);
    strcpy(ex,ext);
    fnmerge(s,drive,dir,fi,ex);   // merge everything into one string
    }  // ����� ᮧ����� ��ப�
*/
// ==============================================================
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      //   CONTACT - �㭪�� ��宦����� ���� ������ � ����
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void contact(int s, int *n)
     { int k, l, m;
	     *n = 0; m = 1;
	      for (k=0; k<8; k++) {
		     l = s & m; *n += l;
		     if (*n>1) break;
		     m *= 2;       }
     }  // ����� �㭪樨 CONTACT
// ==============================================================
       // �㫥��� ����
     void kadr_0()
     {
       } // ����� �㫥���� ����
// ==============================================================
//     ���� - ����� ����� �����
//    ������� - � namezad
     void kadr_im()
     {
      char tmp[256];SetExt(namezad,tmp,"n");
      int handle = open(tmp, O_CREAT);
      if (filelength(handle)<=0) {cout<<" No such file "<<namezad<<"\n";exit(1);}
     } // ����� ���� ����� ����� ����� 

// ============================================================== 
     // ��㧠 
 int pause(void)
{
  int c, rez=0;
  printf("                        <Esc>-��室, �� ������ - �த�������\n");
  c = getch(); c = getch();
  if( ESC == c ) rez=1;
  if( 0 == c ) c = getch();
  return(rez);
}
// ============================================================== 

            // ����⨥ 䠩�� 
            // �������� ! �㦭� ������쭮� ���ᠭ�� FILE *stream; 
            // �����e� � ��� � fclose(stream); 
   int OP_F( char *namezad, char ext[], int mode)
    {
     SetExt(namezad,spath,ext);
// im_ext(spath,namezad,ext);    // Pass - � s 

   if (mode==3) { // ���������� (�⥭�� � ������ ᭠砫�) 
      if ((stream = fopen(spath, "r+b")) == NULL) return 1;
      fseek(stream, 0L, SEEK_SET);}

   if (mode==2) { // ��ᮥ������� (������ � �����) 
      if ((stream = fopen(spath, "ab")) == NULL) return 1;
         fseek(stream, 0L, SEEK_END);}

   if (mode==1) { // ������ � ��� 
      if ((stream = fopen(spath, "wb")) == NULL) return 1;}

   if (mode==0) { // ���쪮 �⥭�� 
      if ((stream = fopen(spath, "rb")) == NULL) return 1;}
        return 0;
     } // ����� OP_F 

// ============================================================= 
/*            // ������(�⥭��) � ��⮪ stream
            // �㦭� ���ᠭ�� union FL {...} fl   
   int WR_FL ( long nbl, long nsl, long dl, int mode)
    {
      long curpos, length, lrw, tind, ostdl, nb;
      nb=nbl; 
      fseek(stream, 0L, SEEK_END);
      length = ftell(stream);
      curpos=nbl*DLBL+4*nsl;
      if (length<curpos) {
         error_message("��砫� ����� ����� ����� 䠩�� !");
         (void) get_char(); clear_message(); return (-1);}
      fseek(stream, curpos, SEEK_SET);   // ��⠫� � ��砫� 

      lrw=(nbl+1)*DLUM-curpos/4l;                // ���⮪ �� ���� ����� 
      if (lrw>=dl) {                     // ������ ���頥��� � ��砫�� ���� 
         if (mode==0) {                  // �⥭�� ⮫쪮 ��砫쭮�� ����� 
           fread(&fl.f[0], SZ, dl, stream);}
         else fwrite(&fl.f[0], SZ, dl, stream);// ������ ⮫쪮 � ��砫�� ���� 
         return (nb);} 

      if (mode==0) {                  // ����뢠�� ��砫�� ���� 
         fread(&fl.f[0], SZ, lrw, stream);}
      else fwrite(&fl.f[0], SZ, lrw, stream); // �����뢠�� ��砫�� ���� 

      tind=lrw;                          // ����騩 ������ m 
      ostdl=dl-lrw;                      // ��⠫��� ������� 
      while (ostdl>=DLUM) {              // ��᪨ ������ � ���� 
      if (mode==0) {                  // ��⠥� 
         fread(&fl.f[tind], SZ, DLUM, stream);}
      else fwrite(&fl.f[tind], SZ, DLUM, stream); // ��襬 
      nb+=1;
      tind+=DLUM;       
      ostdl-=DLUM;}

      if (ostdl!=0l) { nb+=1;
         if (mode==0) {
            fread(&fl.f[tind], SZ, ostdl, stream);}
         else fwrite(&fl.f[tind], SZ, ostdl, stream);}
      return (nb);
    } // ����� WR_FL
*/
// ============================================================== 
       int schitn(void)   // ���뢠��� <namezad>.n 
       {   int l;
/*
             ii=OP_F(namezad, ".n", 0);
             if (ii==0) {// �⥭�� ��砫��� ������ � �����   
        nombl=0l;
        osl=0l;
        i=WR_FL(nombl, osl, 10l, 0);
        ll=fl.l[8];
        fseek(stream, 0L, SEEK_SET);
        i=WR_FL(nombl, osl, ll, 0);
   if (fl.l[9]==2l) {       // ��㬥ୠ� ����� 
   error_message("  �� ����� � ��㬥୮� ���⠭���� !  ");
    i = get_char(); clear_message(); return 1;} // ����� ��㬥୮� ����� 
*/
   char tmp[256];SetExt(namezad,tmp,"n");
   fstream input=fstream(tmp,ios::in);
   input>>tmp>>rad>>tmp>>dlina>>tmp>>razresh;
/*
    rad=fl.l[10];
  dlina=fl.f[12];
razresh=fl.f[13];
*/
   input>>tmp>>zgr1>>tmp>>zgr2>>tmp>>robez>>tmp>>vbez>>tmp>>ksl; k=ksl;
/*
   zgr1=fl.l[14];
   zgr2=fl.l[15];

   robez=fl.f[19];
   vbez =fl.f[20];

   ksl=fl.l[27];

   k=(int)fl.l[27];            // ��᫮ ⥫   
*/
   for (j=0;j<k;j++)
     {
      input>>tmp>>mrk[j]>>tmp>>nus[j]>>tmp>>sv[j]>>tmp>>sr1[j]>>tmp>>sr2[j];
      input>>tmp>>Kolk[j];
/*
   l=29+k;                     // ����� ���ᮢ   

for (j=0; j<k; j++) {        // ���� �� ⥫��   

   mrk[j]=fl.l[l+11];
   nus[j]=(int)fl.l[l+14];    // ����� ���:
                              //          1-��.���, 2-���,
                              //          3-Du, 4-��, 5-��謠�   
   sv[j]=fl.f[l+23];
   l+=26;
       sr1[j]=fl.f[l+3];
       sr2[j]=fl.f[l+6];
   l+=7;         
   Kolk[j]=fl.f[l+1];
   l++;
*/

      for (i=0; i<Kolk[j]; i++) input>>tmp>>Curs[j][i];
     }
/*
       Curs[j][i]=fl.f[l+1+i];// ���祭�� �����.   
   l+=Kolk[j];                 // C���� ��� ᫥���饣� ⥫�   
   } // ����� 横�� �� ⥫��   

        fclose(stream);
*/
        return 0;
           } // ����� ������ ��⮪a 
//          return 1;
//        }  // ����� �⥭�� 䠩�� <>.n 
//=================================================================
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
      //                EOS - �ࠢ����� ���ﭨ�                
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
float eos(int nt, int *nb, float ro, float e, float *cs)
     {float rez;
	switch (*nb) { 
 case 1:                                 // ������� ���
          ug = Curs[nt][0];               // ����� 
        *cs = sqrt(ug*(ug-1)*e);         // ������� ��㪠 
        rez = (ug-1)*ro*e;               // �������� 
	       break;
  case 2:                                 // ��� 
         uro = Curs[nt][0];               // Ro0 
         uc0 = Curs[nt][1];               // A 
          ul = Curs[nt][2];               // N 
        *cs = sqrt(uc0*ul*pow(ro,ul-1)/pow(uro,ul));  // ������� ��㪠 
         rez = (ul-1)*ro*e+uc0*ul*(ro/uro-1);          // �������� 
	       break;

 case 3:                                 // D-u 

         uro = Curs[nt][0];               // Ro0 
         uc0 = Curs[nt][1];               // c0 
          ul = Curs[nt][2];               // L 
        *cs = uc0;                       // ������� ��㪠 
//cout<<" uro "<<uro<<" uc0 "<<uc0<<" ul  "<<ul;
	       rez = uc0*uc0*ro*(ro/uro-1)/
    pow((ul-(ul-1)*ro/uro),2.0);        // �������� 
	       break;

	default:    printf ("\n �訡�� EOS !\n  nb = %d\n", *nb);
	       exit(1);
     }  //����� switch
	return(rez);
     }  // ����� �㭪樨 EOS     
// ============================================================== 
