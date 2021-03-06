
//#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <fstream.h>
#include <iomanip.h>
#include <alloc.h>
#include <dir.h>
#include <string.h>
#include "teklgrp.h"
//#include "fobj.h"
#include "util.h"
#include "grlib.h"
#include <alloc.h>
#include <iostream.h>

#define ENTER 7181
#define PLUS  20011
#define MINUS 18989
#define ZOOM  3849
#define ESC   283 
#define RIGHT 19712
#define LEFT  19200
#define MEAN  12909

#define MaxLen 100000

extern unsigned _stklen=60000;

struct CfgType
  {
   long SubGrd,AutoGrd;
   long FGrd,LGrd,FCh,LCh;
   long GivenGrd,GivenLim;
   double Tstp;
   double MisMin,SigMin,MisSig;
   int MaxReg,Smth;
   friend ostream& operator<<(ostream &output,CfgType &P1);
   friend istream& operator>>(istream &input,CfgType &P1);

  };
struct Reg { double s,o,l,a;long Fst,Lst;};
struct So { double s,o;};      

ostream& operator<<(ostream &output,CfgType &P1)
  { output<<"Sub_Ground(0-false) "<<P1.SubGrd<<" AutoGrd "<<P1.AutoGrd
          <<"\nFrom_Chan_Grond "<<P1.FGrd<<" To_Chan "<<P1.LGrd
          <<"\nFrom_Chan_Write "<<P1.FCh<<" To_Chan "<<P1.LCh
          <<"\nGiven_Ground_Lim "<<P1.GivenGrd<<" Given_Chanel_Lim "<<P1.GivenLim
          <<"\nTstp "<<P1.Tstp
          <<"\nMistake_Of_Mean(In_Sigma) "<<P1.MisMin
	  <<" Mistake_Of_Sigma "<<P1.MisSig<<" Minimum_Sigma "<<P1.SigMin
          <<" Smooth_by_Num "<<P1.Smth<<"\nMaxReg_To_Count "<<P1.MaxReg;
   output<<"\n";
   return output;};
istream& operator>>(istream &input,CfgType &P1)
  { char buf[100];
    input>>buf >>P1.SubGrd  >>buf>>P1.AutoGrd
          >>buf>>P1.FGrd    >>buf>>P1.LGrd    >>buf>>P1.FCh >>buf>>P1.LCh
          >>buf>>P1.GivenGrd>>buf>>P1.GivenLim>>buf>>P1.Tstp>>buf>>P1.MisMin
          >>buf>>P1.MisSig  >>buf>>P1.SigMin  >>buf>>P1.Smth>>buf>>P1.MaxReg;
   return input;};















int GetConst(unsigned char* buf,double &d,unsigned char *cstr)
  {
   if ((buf=strstr(buf,cstr))!=NULL) buf+=strlen(cstr);
   else return 0;
//cout<<"found:  "<<buf<<"\n    row val = "<<atof(buf)<<"\n";
    d=atof(buf);
//cout<<" val = "<<d<<"\n";
   return 1;
  }
void GetHead(istream &in,double &Time_Stp,double &Time_Shift,
		      double &Val_Coef,double &Val_Shift,long int &Ser_Num)
  {
   unsigned char buf[512];
   double tmp;
   in>>buf;
   if (strncmp(buf,"\"\"",2) != 0) { cout<<"Do not understand format";exit(1);}
cout<<buf<<"\n";
   in.getline(buf,512);in.getline(buf,512);
   if ( (!GetConst(buf,Time_Stp,"XINCR:")) ||
	   (!GetConst(buf,Time_Shift,"XZERO:")) ||
//     (!GetConst(buf,Val_Coef,"YINCR:")) ||
     (!GetConst(buf,Val_Coef,"YMULT:")) ||
	   (!GetConst(buf,Val_Shift,"YZERO:")) )
		   { cout<<"Do not understand format";exit(1);}
                                                  
   Val_Coef*=1000;Val_Shift*=1000;Time_Shift*=1e9;Time_Stp*=  1e9;
   cout<<"Sensetivity,  mV : "<<Val_Coef<<" \n";
   cout<<"Shift in cells : "<<Val_Shift<<"\n";
   cout<<"Time Shift in cells, ns : "<<Time_Shift<<"\n";
   cout<<"Time sensetivity, ns : "<<Time_Stp<<"\n";
//   Time_Shift=0;
//   Time_Shift=0;Time_Stp=4;
  };

void anal(long k,GrLine &ll,Rect dim,double &Cur,double &Stp)
  {
   switch (k) 
     { 
       case RIGHT:
       case LEFT :
                    ll.HideLine(Cur, dim.dn.y,Cur,dim.up.y);
                    if (k==LEFT) Cur-=Stp;else Cur+=Stp;
		    ll.DrawLine(Cur, dim.dn.y,Cur,dim.up.y);
                    break;
       case PLUS :Stp*=2;break;
       case MINUS:Stp/=2;break;
     }
  }

long GetPos(IntArr &ar,long &Fwrite,long &Lwrite,long Lchannel)
   {
   long Fpnt=1,ret=1,Num=Lwrite-Fwrite,k;
   Rect dim=ar.ArrRect();

   double CurL=dim.dn.x,CurR=dim.up.x,Stp=(dim.up.x-dim.dn.x)/100;
   GrLine ll(ar.CellConv::SetScr(),ar.CellConv::SetGr()),
          lr(ar.CellConv::SetScr(),ar.CellConv::SetGr());
   ll.PixColor(WHITE);lr.PixColor(BLUE);
   ll.SetDraw();
   ll.DrawLine(CurL, dim.dn.y,CurL,dim.up.y);
   lr.DrawLine(CurR, dim.dn.y,CurR,dim.up.y);
   while ( (((k=ChRead())!=ESC) && (k!=ZOOM)) )
     {
      if (k==ENTER) if (Fpnt) 
        { Fpnt=0;
          ll.HideLine(CurL, dim.dn.y,CurL,dim.up.y);
	  lr.HideLine(CurR, dim.dn.y,CurR,dim.up.y);
	  lr.PixColor(WHITE);ll.PixColor(BLUE);
	  ll.DrawLine(CurL, dim.dn.y,CurL,dim.up.y);
          lr.DrawLine(CurR, dim.dn.y,CurR,dim.up.y);
	}
      else break;
      if (Fpnt) anal(k,ll,dim,CurL,Stp);
      else  anal(k,lr,dim,CurR,Stp);
     }
   nxt:
   switch (k) 
     { 
       case ENTER:Fwrite+=(CurL-dim.dn.x)/(dim.up.x-dim.dn.x)*Num;
                  Lwrite+=(CurR-dim.up.x)/(dim.up.x-dim.dn.x)*Num;
                  if (Lwrite<=Fwrite) Fwrite=Lwrite-1;
                  if ((Lwrite<=Lchannel) && (Fwrite>=1)) break;
       case ZOOM :Fwrite=1;Lwrite=Lchannel;break;
       case ESC  :ret=0;break;
     }
//cout<<"ret\n";ChRead();
   return ret;
  }

void GetFL(ConvPar cvt,long Num,long &Fwrite,long &Lwrite)
  {
//  if (registerbgidriver(EGAVGA_driver)<0) exit(1);
  ConvPar cvt_cur=cvt;
//  BegGrph();
   InitGrph();
   IntArr *ar;
   ar=new IntArr(Num,cvt);
   ar->PixColor(LIGHTGREEN);
//cout<<coreleft()<<"\n";ChRead();
   GrScr *sum=AxGraph(ar,"  Time   ","  mV   ");
//cout<<coreleft()<<"\n";ChRead();
   sum->LstAdd(ar);
   sum->Show();
///*
   while (GetPos(*ar,Fwrite,Lwrite,Num)) 
     {
//cout<<Fwrite<<"  "<<Lwrite<<"\n";ChRead();
      sum->Hide();
      cvt_cur.ResetConv(&cvt.buf[Fwrite-1],(Fwrite-1)*cvt.ax+cvt.bx,cvt.ax,cvt.by,cvt.ay);
      ar->ArrReset(Lwrite-Fwrite+1,cvt_cur);
      sum->GrDim(ar->CellConv::SetGr());
      sum->Show();
     }
//*/  ChRead();
   sum->Hide();
   delete sum;
   closegraph();
//cout<<coreleft()<<"\n";ChRead();
  };
void GBuf(char *Name,long &Num,ConvPar &cvt)
  {  
   long int Ser_Num;
   double Time_Stp,Time_Shift,Val_Coef,Val_Shift;
   fstream in (Name,ios::in|ios::binary);
   GetHead(in,Time_Stp,Time_Shift,Val_Coef,Val_Shift,Ser_Num);
//cout<<coreleft()<<"  ";
   int huge *buf=(int*)farmalloc( MaxLen*2+2);
//cout<<coreleft()<<"    "<<sizeof(buf)<<"\n";
//cout<<buf<<" buf \n";
//   char huge *ch=(char*)&(*buf);
//   Flen=Flen*2;
//   int LtoRead=32000,LRead=32000;
   Num=1;
   int c;
   while ( (in) && (Num<MaxLen) )
     {
      in>>c;buf[Num]=c;
      Num++;
//    in.read((char*)&ch[Num],min(LtoRead,Flen));         // Num needed
//    LRead=in.gcount();
//    Flen-=LRead;
//    Num+=LRead;
     }
   if (Num>=MaxLen) {cout<<"Read only part of the points\n";ChRead();}
//   Num=long((Num)/2)-3;
   Num-=3;
   cvt.ResetConv(buf,-Time_Shift,Time_Stp,Val_Shift,Val_Coef);
 }







int InModel(So  s1,So  s2,CfgType Cfg)
  {  
//cout<<s1.o<<"\n";
   if ( (fabs(s1.s-s2.s)>Cfg.MisMin*sqrt((s1.o)) ) ||
        (s2.o/(s1.o)>Cfg.MisSig ) ) return 0;
   return 1;
  }
void EndReg(Reg *r,long &CurReg,long k) 
  {
   if (CurReg<=0) return; 
   r[CurReg].s/=r[CurReg].l;
   r[CurReg].o=(r[CurReg].o+r[CurReg].a)/r[CurReg].l-r[CurReg].s*r[CurReg].s;
   r[CurReg].Lst=k;
cout<<"Reg L= "<<r[CurReg].l<<" FrCh "<<r[CurReg].Fst<<" ToCh "
    <<r[CurReg].Lst<<" mean "<<r[CurReg].s<<" mist "<<r[CurReg].o<<"\n";
  };    
int BegReg(Reg *r,long &CurReg,So &s,CfgType Cfg,long k,double Mistake)
  {
   if (CurReg==Cfg.MaxReg) return 0;
   else
     {
      CurReg++;
      r[CurReg].s=s.s;
      r[CurReg].o=s.s*s.s;
      r[CurReg].a=s.o+(Mistake);
      r[CurReg].l=1;
      r[CurReg].Fst=k;
     }
//cout<<"BegReg "<<CurReg<<"\n";
   return 1;
  }
void AddReg(Reg *r,int CurReg,So s)
  {
   Reg tmp=r[CurReg];
   tmp.s+=s.s;
   tmp.o+=s.s*s.s;
   tmp.a+=s.o;
   tmp.l++;
   r[CurReg]=tmp;
  };    
double AnalReg(Reg *r,long &CurReg,CfgType &Cfg,double Mistake)
  {
   double ret=-MathInf,o;
   long cur;
   if (CurReg==0) 
     {cout<<"Could not find constant region , mean=0\n";ChRead();return 0;}
   if (CurReg<0)
     {cout<<"Too many regions. May be mistake\n";ChRead();CurReg=Cfg.MaxReg;}
   for (long k=1;k<=CurReg;k++) if (ret<r[k].l/(r[k].o+Mistake/10)) 
     {cur=k;ret=r[k].l/(r[k].o+Mistake/10);}
   o=r[cur].o;Cfg.FGrd=r[cur].Fst;Cfg.LGrd=r[cur].Lst;ret=r[cur].s;
if (o<0) cout<<"Something very strange hi2= "<<o<<"\n";
   cout<<"Misfit = "<<sqrt(fabs(o))<<" Mean value = "<<ret<<"\n";
   return ret;
  }
double GetMinMist(ConvPar cvt,long Num)
  {
   long double sum=0;
   for (long k=1;k<=Num-2;k++)   sum+=sqr(cvt(0,k)-cvt(0,k+1));
   sum/=(2*Num);
//cout<<" Mean misfit = "<<sum<<"\n";
   return sum;
  };
void FormSo(ConvPar cvt,So &s,long CurCh,CfgType Cfg,double meanmist)
  {  
   s.s=0;s.o=0;
   double tmp;
   for (long k=1;k<=Cfg.Smth;k++)
     { 
      tmp=cvt(0,k+CurCh);
      s.s+=tmp;
      s.o+=tmp*tmp;
     }
   s.s/=Cfg.Smth;
   s.o=s.o/Cfg.Smth-s.s*s.s+Cfg.SigMin*meanmist;
  };
void GetGrdAuto(ConvPar cvt,long Num,CfgType &Cfg)
  {
   double tmp;
   So *s=new So[3];
   Reg *r=new Reg[Cfg.MaxReg+2];
   double sum=GetMinMist(cvt,Num) ;
   long k;
   long CurReg=0,InReg=0;
   So ts;
   FormSo(cvt,s[1],1,Cfg,sum);FormSo(cvt,s[2],1+Cfg.Smth,Cfg,sum);
   for (k=1;k<=Num-2*Cfg.Smth;k+=Cfg.Smth) 
    {
     FormSo(cvt,s[3],k+Cfg.Smth*2,Cfg,sum);
     if (InReg)
       {
        ts.s=r[CurReg].s/r[CurReg].l;
        ts.o=(r[CurReg].o+r[CurReg].a)/r[CurReg].l-ts.s*ts.s;
       }
     else {ts=s[1];ts.o=sum;}
     if (!InModel(ts,s[2],Cfg))  
       { 
        if (InReg) 
          if (!InModel(ts,s[3],Cfg))  {InReg=0;EndReg(r,CurReg,k+Cfg.Smth-1);}
       }
     else 
       {
        if (!InReg) 
          { 
           if (!BegReg(r,CurReg,s[1],Cfg,k,sum)) goto exi;
           InReg=1;
          }
        AddReg(r,CurReg,s[2]);
       }
     s[1]=s[2];s[2]=s[3];
    }
   exi:
//cout<<coreleft()<<"\n";
   if (InReg) EndReg(r,CurReg,Num);
   AnalReg(r,CurReg,Cfg,sum);
   delete r;
  };

CfgType GetCfg(char *CfgName,char* PathName)
  {
   CfgType ret={1,1,1,1000,1,1000,0,0,0,2,0,3,10,5};
   char name[50];
//cout<<CfgName<<"\n";
   if (CfgName!=NULL)
      {
       SetPath(CfgName,name,PathName);
//cout<<name<<"\n";
       fstream in (name,ios::in);in>>ret;
      };
   return ret;
  }
void MakeCvt(CfgType &Cfg,ConvPar &cvt,long Num)
  {
   if (Cfg.Tstp) cvt.ax=Cfg.Tstp;
   else Cfg.Tstp=cvt.ax;
   if (!Cfg.GivenLim) 
     {  cout<<"Enter limits for ouput\n";ChRead();
        Cfg.FCh=1;Cfg.LCh=Num;
//cout<<coreleft()<<"\n";ChRead();
        GetFL(cvt,Num,Cfg.FCh,Cfg.LCh);
//cout<<coreleft()<<"\n";ChRead();
        Cfg.GivenLim=1;
     }
   if (Cfg.SubGrd)
     { 
      if (Cfg.AutoGrd) GetGrdAuto(cvt,Cfg.LCh/*Num*/,Cfg);
      else
        {
	 if (!Cfg.GivenGrd)
           {  cout<<"Enter limits for ground \n";ChRead();
             Cfg.FGrd=1;Cfg.LGrd=Num;
//cout<<coreleft()<<"\n";ChRead();
             GetFL(cvt,Num,Cfg.FGrd,Cfg.LGrd);
//cout<<coreleft()<<"\n";ChRead();
           }
        }
     double s=0;
     for (long k=Cfg.FGrd;k<=Cfg.LGrd;k++)  s+=cvt(0,k);//cout<<cvt(0,k);}
     cvt.by-=s/(Cfg.LGrd-Cfg.FGrd+1);
cout<<"Ground Level= "<<s/(Cfg.LGrd-Cfg.FGrd+1)<<"\n";
     Cfg.GivenGrd=1;
    }
  }







int AnalPar(int argc,char *argv[],char *&CfgName,char *&PathName)
  {
   if ((argc==0) || (GetCmd("/h",argc,argv)!=NULL)) return 0;
   PathName=GetCmd("/p",argc,argv);
   CfgName=GetCmd("/c",argc,argv);
   if ((PathName==NULL) && (CfgName==NULL) && (argc==2)) CfgName=argv[2];
   return 1;
  };
void main(int argc,char *argv[])
  {
   if (registerbgidriver(EGAVGA_driver)<0) exit(1);
   long Num;
   ConvPar cvt;
   argc--;
//cout<<argc<<"/n";
   char *CfgName,*PathName;
   int IsHlp;
   char Nput[50],Conf[50];
   if (!AnalPar(argc,argv,CfgName,PathName))
     {cout<<"usage:\n     dsa_dat [tek file] [/cfile /h /pname]\n"<<
	   "      /c - use  configure file \n"<<
	   "      /p - directory to write \n"<<
	   "      /h - display help\n";exit(1);}
//   SetPath(argv[1],Nput,PathName);
   GBuf(argv[1],Num,cvt);// rem Nput- argv[1]
   CfgType Cfg=GetCfg(CfgName,PathName);
   MakeCvt(Cfg,cvt,Num);
//   if (CfgName==NULL)
    {
     SetPathExt(argv[1],Nput,PathName,"cfg");
     fstream outcfg(Nput,ios::out);
     outcfg<<Cfg;
    }

   SetPathExt(argv[1],Nput,PathName,"dat");
   fstream out(Nput,ios::out);
   char buf[256];
   cvt.bx=-Cfg.FCh*cvt.ax;
   for (long k=Cfg.FCh;k<=Cfg.LCh;k++)
     {
      sprintf(buf,"% 10.4e % 10.5e",(double )cvt(1,k),(double )cvt(0,k));
      out<<buf<<"\n";
     }
//   delete cvt.buf;
  }












