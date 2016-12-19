// uses grlib,memf,fobj,util;
//#include "util.h"
//#include "grlib.h"
//#include "memf.h"
#include <stdio.h>
#include <stdlib.h>
#include <iomanip.h>
#include <fstream.h>
#include "util.h"
#include "anal.h"
//#define MaxOperation 100
#define MaxDifOper 300
#define MaxCol 512
#define MaxStrLen 2048
int UserNumCol;

#ifndef LINUX
extern unsigned _stklen=32000;
#endif
void FormatWriteStr(ostream &out,double *ar,int NumCol,char *format)
  {
   int width=0;if (format!=NULL) width=atoi(format);
   for (int k=1;k<=NumCol;k++)
     {
      if (format==NULL)
       {
        if (ar[k]==NullNumber) out<<setw(13)<<"\"\""<<"  ";
        else out<<setw(13)<<setprecision(6)<<ar[k]  <<"  ";
       }
      else
       {
        if (ar[k]==NullNumber) out<<setw(13)<<"\"\""<<"  ";
        else 
         {
          if ((int)ar[k]-ar[k]<MathZer) 
           {
            int l=atoi(format),n,o;l=min(100,l);
            char val[256],val1[256];
            Itoa((int)ar[k],val,10);n=strlen(val);
            if (l<=0) out<<val<<" ";
            else
             {
              for ( o=0;o<l-n;o++) val1[o]='0';
              for ( o=l-n;o<l;o++) val1[o]=val[o-l+n];val1[l]=0;
              out<<val1<<" ";
             }
           }
          else out<<setw(13)<<setprecision(6)<<ar[k]  <<"  ";
         }
       }

    };
      out<<"\n";
 };

void GetWork(char *name,Action *act[],int &NumOper)
  {
   char s[MaxStrLen];
   fstream in(name,ios::in);
   int c,k=0;
   while ( (++k<MaxDifOper) && (!seek_eof(in)))
     {
      do {if (seek_eof(in)) goto exit_proc;in.getline(s,511);}
      while (Strnicmp(s,"//",2)==0) ;
//cout<<" s "<<s<<"\n";
      parsestr(s,act[k],c);
//cout<<" s "<<s<<"\n";exit(1);
      PackAct(act[k]);
     }
  exit_proc:
   NumOper=k-1;
  }
int DoWork(double *Row,Action *act[],double GetCol,int NumOper)
  {
   for (int k=1;k<=NumOper;k++) EvalAct(act[k],Row);
   if (UserNumCol) return UserNumCol;
   return int(GetCol);
  }
void OutUser(long UserNumRow,double *Row,int GetCol,fstream &out,
                                              Action *act[],int NumOper,char *format)
  {
   double Row1[MaxCol];
   int NumCol=NumOper;
   if (UserNumCol) NumCol=UserNumCol;
   for (long k=1;k<=UserNumRow;k++)
     {
	  MemoryMove(Row,Row1,sizeof(Row1));
	  Row1[0]=k;
      GetCol=DoWork(Row1,act,GetCol,NumOper);
      FormatWriteStr(out,Row1,NumCol,format);
     }
  }
void main( int argc, char *argv[] )
  {
//char res[256];strcpy(res,"start");SetExt(argv[1],res,argv[3]);
//cout<<"Set Ext:"<<argv[1]<<" "<<argv[3]<<" "<<res<<"\n";
//SetPathExt(argv[1],res,argv[2],argv[3]);cout<<"Set Path Ext:"<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<" "<<res<<"\n";
//SetPath(argv[1],res,argv[2]);cout<<"Set Path:"<<argv[1]<<" "<<argv[3]<<" "<<res<<"\n";
//void *a=new char[256];strcpy((char *)a,argv[1]);cout<<"Control void:"<<(char *)a<<"\n";delete a;
//exit(0);
  if (argc<=2)
     {cout<<" use :  \narderiv get_file put_file calc_mtd_file [options]\n"<<
            "options:\n   /r - number of rows to output \n"<<
		      "       (use for calculation in the input only first row)\n"<<
                      "   /n - no header information\n"<<
                      "   /fnum - output integers eith fixed length num ( add zero ) \n"<<
                      "   /c - number of columns to output\n";exit(1);}

   Action *act[MaxDifOper];
   int NumOper,k;
   RandomInit();
   for (k=1;k<=MaxDifOper;k++)
     {act[k]=new Action[MaxOperation];}
   GetWork(argv[3],act,NumOper);
//cout<<" in "<<argv[1]<<" out "<<argv[2]<<"\n";exit(1);
   fstream in(argv[1],ios::in);
   fstream out(argv[2],ios::out);
   out<<setiosflags(ios::showpoint|ios::scientific|ios::left);
   double Row[MaxCol];
   int GetCol,NoHead=0;k=1;
   long UserNumRow=0;char *tmp;
   if ( (tmp=GetCmd("/c",argc-1,argv))!=NULL )  UserNumCol=atoi(tmp);
   else UserNumCol=0;
   char *format=NULL;
   if ( (tmp=GetCmd("/f",argc-1,argv))!=NULL )  format=tmp;
   
   if ( (tmp=GetCmd("/n",argc-1,argv))!=NULL )  NoHead=1;
// New adition
			int s=0;
			while (((GetCol=EnterStr(in,Row,MaxCol,1))==0) && ( !seek_eof(in) )) s++;
			in.close();in.open(argv[1],ios::in);char name[512];
//cout<<"S "<<s<<" GetCol "<<GetCol<<"\n";
   if (!NoHead) {while (s>0) {in.getline(name,512);out<<name<<"\n";s--;}}
//
   if ( (tmp=GetCmd("/r",argc-1,argv))!=NULL )
     {
      UserNumRow=atoi(tmp);
      GetCol=EnterStr(in,Row,MaxCol);
//cout<<"GetCol "<<GetCol<<"\n";      
      OutUser(UserNumRow,Row,GetCol,out,act,NumOper,format);
     }
   else
     {
      do
        {
         if ((GetCol=EnterStr(in,Row,MaxCol))==0) continue;
									Row[0]=k++;
//cout<<"ret "<<GetCol<<"\n";
         GetCol=DoWork(Row,act,GetCol,NumOper);
         FormatWriteStr(out,Row,GetCol,format);
        }
      while ( !seek_eof(in) );
    }
//   for (k=MaxDifOper;k>=1;k--) delete act[k];
  };
/*
int  WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
		       	   LPSTR lpCmdLine,int nCmdShow)
{
//	MSG msg;
//    while (GetMessage( (LPMSG) &msg, NULL, 0, 0) )
//    {
//        TranslateMessage( (LPMSG) &msg );
//        DispatchMessage( (LPMSG) &msg );
//    }
	return 0;
};

*/
