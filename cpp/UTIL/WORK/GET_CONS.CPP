#include "util.h"

#define MaxCol 10

// Used just to enter Array file and to use it;

struct ConvPar
  {
   double *buf;
   int NumCh,NumCol;
   double operator()(int k,int n) 
{
//cout<<" n "<<n<<" val "<<buf[(k-1)*(NumCol+1)+n]<<"\n";
 return buf[(k-1)*(NumCol+1)+n];}
   void ResetConv(ConvPar &cnv)
     {
      DeleteConv();
      NumCh=cnv.NumCh;NumCol=cnv.NumCol;buf =new double[NumCh*(NumCol+1)];
      MemoryMove(cnv.buf,buf,sizeof(double)*NumCh*(NumCol+1));
     };
    void DeleteConv(){if (buf!=NULL) {delete buf;buf=NULL;NumCh=0;NumCol=0;};};
    void FileRead(char *name);

   ~ConvPar(){DeleteConv();};
    ConvPar(){buf=NULL;NumCol=0;NumCh=0;}
    ConvPar(char *name){buf=NULL;NumCol=0;NumCh=0;FileRead(name);}
    ConvPar& operator=(ConvPar &cnv)
      {  ResetConv(cnv);return *this; };
  };
void ConvPar::FileRead(char *name)
  {
   int s=0,ResCol,GetCol,s1=0;
   double lbuf[MaxCol];
   fstream in(name,ios::in);
   while ( !seek_eof(in) )
     if ((GetCol=EnterStr(in,lbuf,MaxCol,1))!=0) {ResCol=GetCol;s++;}
//cout<<" NumChRead "<<s;
   in.close();in.open(name,ios::in);
   buf=new double[s*(ResCol+1)];
   while ( !seek_eof(in) )
     if ( (GetCol=EnterStr(in,&buf[s1*(ResCol+1)],ResCol+1,1))==ResCol ) 
        s1++;
//cout<<" NumChRes "<<s1<<"\n";
   NumCh=s1;NumCol=ResCol;
   in.close();
  };


struct CfgType
  {
   double MisMin,SigMin,MisSig;
   int MaxReg,Smth,NumMCol;
   friend ostream& operator<<(ostream &output,CfgType &P1);
   friend istream& operator>>(istream &input,CfgType &P1);
  };
ostream& operator<<(ostream &output,CfgType &P1)
  { output<<"\nMistake_Of_Mean(In_Sigma) "<<P1.MisMin
   <<" Mistake_Of_Sigma "<<P1.MisSig<<" \nMinimum_Sigma "<<P1.SigMin
          <<" Smooth_by_Num "<<P1.Smth<<"\nMaxReg_To_Count "<<P1.MaxReg
          <<" NumMCol "<<P1.NumMCol;
   output<<"\n";
   return output;};
istream& operator>>(istream &input,CfgType &P1)
  { char buf[100];
    input>>buf>>P1.MisMin
          >>buf>>P1.MisSig  >>buf>>P1.SigMin  >>buf>>P1.Smth>>buf>>P1.MaxReg
          >>buf>>P1.NumMCol;
   return input;};


struct Reg { double s,o,l,a;int Fst,Lst;};
struct So { double s,o;};      

 

 

 

 

 
int InModel(So  s1,So  s2,CfgType Cfg)
  {  
//cout<<s1.o<<"\n";
   if ( (fabs(s1.s-s2.s)>Cfg.MisMin*sqrt((s1.o)) ) ||
        (s2.o/(s1.o)>Cfg.MisSig ) ) return 0;
   return 1;
  }
void EndReg(Reg *r,int &CurReg,int Lst,ConvPar &cvt) 
  {
   if (CurReg<=0) return; 
//cout<<" Cur.o "<<r[CurReg].o<<" Cur.s "<<r[CurReg].s<<" Cur.a "<<r[CurReg].a<<"\n";
   r[CurReg].s/=r[CurReg].l;
   r[CurReg].o=(r[CurReg].o+r[CurReg].a)/r[CurReg].l-r[CurReg].s*r[CurReg].s;
   r[CurReg].Lst=Lst;
//cout<<" Reg Ln= "<<r[CurReg].l<<" FrCh "<<r[CurReg].Fst<<" ToCh "
//    <<r[CurReg].Lst<<" mean "<<r[CurReg].s
//    <<" FstT "<<cvt(r[CurReg].Fst,1)<<" LstT "<<cvt(r[CurReg].Lst,1)<<"\n";

int NumC=cvt.NumCol,F=r[CurReg].Fst,L=r[CurReg].Lst,k1,k;
double Mean[MaxCol+1];
for (k1=2;k1<=NumC;k1++) 
  {
//cout<<" F= "<<F<<" L "<<L<<"\n";
   Mean[k1]=0;for (k=F+1;k<=L+1;k++) 
     {Mean[k1]+=cvt(k,k1);
//cout<<cvt(k,k1)<<"\n";
     }
//   Mean[k1]/=r[CurReg].l;}
   Mean[k1]/=(L-F+1);}
//cout<<" Reg Ln= "<<r[CurReg].l<<" FrCh "<<F<<" ToCh "<<L<<" ";
//for (k1=2;k1<=NumC;k1++) cout<<Mean[k1]<<" ";
//cout<<" mean "<<r[CurReg].s<<" FstT "<<cvt(F,1)<<" LstT "<<cvt(L,1)<<"\n";
cout<<r[CurReg].l<<" "<<cvt(F+1,1)<<" "<<cvt(L+1,1)<<" ";
for (k1=2;k1<=NumC;k1++) cout<<Mean[k1]<<" ";cout<<r[CurReg].s<<" "<<F<<" "<<L<<"\n";

  };    
int BegReg(Reg *r,int &CurReg,So &s,CfgType &Cfg,int k,double Mistake)
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
double AnalReg(Reg *r,int &CurReg,CfgType &Cfg,double Mistake)
  {
   double ret=-MathInf,o;
   int cur;
   if (CurReg==0) 
     {cout<<"Could not find constant region , mean=0\n";ChRead();return 0;}
   if (CurReg<0)
     {cout<<"Too many regions. May be mistake\n";ChRead();CurReg=Cfg.MaxReg;}
   for (int k=1;k<=CurReg;k++) if (ret<r[k].l/(r[k].o+Mistake/10)) 
     {cur=k;ret=r[k].l/(r[k].o+Mistake/10);}
   o=r[cur].o;ret=r[cur].s;
//   Cfg.FGrd=r[cur].Fst;Cfg.LGrd=r[cur].Lst;
if (o<0) cout<<"Something very strange hi2= "<<o<<"\n";

//   cout<<"Misfit = "<<sqrt(fabs(o))<<" Mean value = "<<ret<<"\n";
   return ret;
  }
double GetMinMist(ConvPar &cvt,int Num,int NumMCol)
  {
   double sum=0;
   int k,k1,NumC=cvt.NumCol,n=0;
   for (k=1;k<=Num-1;k++)   sum+=sqr(cvt(k,NumMCol)-cvt(k+1,NumMCol));
   sum/=(2*Num);
double Max[MaxCol+1];for (k1=1;k1<=NumC;k1++) Max[k1]=-MathInf;
for (k=1;k<=Num;k++)  if (Max[NumMCol]<cvt(k,NumMCol)) 
  {for (k1=2;k1<=NumC;k1++) {Max[k1]=cvt(k,k1);n=k;}}
//cout<<" Reg Ln= "<<0<<" FrCh "<<n<<" ToCh "<<n<<" ";
//for (k1=2;k1<=NumC;k1++) cout<<Max[k1]<<" ";
//cout<<" mean "<<Max[NumMCol]<<" FstT "<<cvt(n,1)<<" LstT "<<cvt(n,1)<<"\n";
cout<<" RegLn=  FstT LstT ..... mean FrCh  ToCh \n";
cout<<0<<" "<<cvt(n,1)<<" "<<cvt(n,1)<<" ";
for (k1=2;k1<=NumC;k1++) cout<<Max[k1]<<" ";
cout<<Max[NumMCol]<<" "<<" "<<n<<" "<<n<<"\n";
 
//cout<<" Mean misfit = "<<sum<<"\n";
   return sum;
  };
void FormSo(ConvPar &cvt,So &s,int CurCh,CfgType &Cfg,double meanmist)
  {  
   s.s=0;s.o=0;
   double tmp;
   for (int k=1;k<=Cfg.Smth;k++)
     { 
//cout<<" SO "<<k+CurCh<<"\n";
      tmp=cvt(k+CurCh,Cfg.NumMCol);
      s.s+=tmp;
      s.o+=tmp*tmp;
     }
   s.s/=Cfg.Smth;
   s.o=s.o/Cfg.Smth-s.s*s.s+Cfg.SigMin*meanmist;
  };


void GetGrdAuto(ConvPar &cvt,int Num,CfgType &Cfg)
  {
   double tmp;
   So *s=new So[4];
   Reg *r=new Reg[Cfg.MaxReg+2];
   double sum=GetMinMist(cvt,Num,Cfg.NumMCol);
//cout<<" Num "<<Num<<" MinMisf "<<sum<<"\n";
   int k;
   int CurReg=0,InReg=0;
   So ts;
   FormSo(cvt,s[1],1,Cfg,sum);FormSo(cvt,s[2],1+Cfg.Smth,Cfg,sum);
   for (k=1;k<=Num-3*Cfg.Smth;k+=Cfg.Smth) 
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
          if (!InModel(ts,s[3],Cfg))  {InReg=0;EndReg(r,CurReg,k+Cfg.Smth-1,cvt);}
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
   if (InReg) EndReg(r,CurReg,Num,cvt);
   AnalReg(r,CurReg,Cfg,sum);
   delete r;delete s; 
  };



void main( int argc, char *argv[] )
  {
   CfgType Cfg={2,0.01,0.9,30,1,1};
   if (argc<2) 
     {cout<<" use : \nget_cons get_file [key] \n"<<
            "        key : /fcfg_name  input cfg file\n"<<
            "              /cNumber - Number of column to use\n";
      fstream out("get_cons.cf",ios::out);out<<Cfg;out.close();
          exit(1);}
   ConvPar cvt(argv[1]);
//  MisMin,SigMin,MisSig, MaxReg,Smth;
   char *tmp;
   if ( (tmp=GetCmd("/f",argc-1,argv))!=NULL )  
     {fstream in(tmp,ios::in);in>>Cfg;in.close();}
   if ( (tmp=GetCmd("/c",argc-1,argv))!=NULL )  Cfg.NumMCol=atoi(tmp);

//fstream out("1.cfg",ios::out);out<<Cfg;out.close();
//fstream out1("1.dat",ios::out);for (int k=1;k<=cvt.NumCh;k++) 
//{for (int k1=1;k1<=cvt.NumCol;k1++) out1<<setw(13)<<setprecision(6)<<cvt(k,k1)<<" ";out1<<"\n";}out1.close();
   GetGrdAuto(cvt,cvt.NumCh,Cfg);
  }; 
