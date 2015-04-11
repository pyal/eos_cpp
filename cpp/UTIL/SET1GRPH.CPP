#include "anal.h"
#include <stdio.h>
#include <stdlib.h>
#include "lib/std/util.h"
//#include "memf.h"
//#include <iomanip.h>

#define MaxCol 100
#define MaxSumFiles 50
#define MaxRow   24096          //2048
#define MaxCharInLine 2048     //2048

#ifndef LINUX
extern unsigned _stklen=60000;
#endif
struct srt {double v;int n;};
struct FileArr
  {
   int NumR,NumC;
   int CurRow,CurCol;
   int OnlyPoints,DeleteEnd;
   double *ar[MaxCol];
   int CurPos[MaxCol+1];
   FileArr(char *name,int DelEnd=1,int OnlyPnt=0);
   void Sort();
   int NextRow();
   int NextCol();
   void SetIterYval();               // Y - val for cur [Row,Col]
   double IterYval(double x);               // Y - val for cur [Row,Col]
   double Yval(double x);               // Y - val for cur [Row,Col]
   double Row();                        // X - val for cur  Row
   int NewXRow(int Xrow);               // Change X - column
   int RowPlus(int k);
   int RowMinus(int k);
  };
typedef FileArr *(FileArrPtr)[50];
int GOL(const void *v1,const void *v2)
  {
   srt *p1=(srt*)v1,*p2=(srt*)v2;
   if (p1->v>p2->v) return 1;
   else if (p1->v==p2->v) return 0;
     else return -1;
  }
void FileArr::Sort()
  {
   double *ar1[MaxCol];
	 int k;
   for (k=0;k<NumC;k++) ar1[k]=new double[NumR+1];
   srt Tab[MaxRow+1];
   for (k=1;k<=NumR;k++) {Tab[k].v=ar[0][k];Tab[k].n=k;}
   qsort(&Tab[1],NumR,sizeof(srt),GOL);
   for ( k=1;k<=NumR;k++)
     {
      for (int k1=0;k1<NumC;k1++)
           { ar1[k1][k]=ar[k1][Tab[k].n];/*cout<<ar1[k1][k]<<"   ";*/}
//      cout<<"\n";
     }
   for ( k=0;k<NumC;k++) {delete ar[k];ar[k]=ar1[k];}
  }
FileArr::FileArr(char *name,int DelEnd,int OnlyPnt)
  {
   double pnt[MaxCol+1];
   char s[MaxCharInLine+1];
   int Fst=1,k;
   fstream *in=new fstream(name,ios::in);
   if (seek_eof(*in)) {cout<<" Cannot read file: "<<name;exit(1);}
   NumR=0; NumC=0; CurRow=0; CurCol=0;
   while (!seek_eof(*in)) { *in>>&s[0];if (s[0]!=0) NumR++;line_feed(*in);}
   if (NumR>MaxRow) {cout<<"File is too long";NumR=MaxRow;}
   delete in;in=new fstream(name,ios::in);
   while ( (!seek_eof(*in)) && (NumR>CurRow) )
     {
      if (CurCol=EnterStr(*in,&pnt[0],MaxCol))
        {
         CurRow++;
         if (Fst) for (k=0;k<=CurCol-1;k++)
           { ar[k]=new double[NumR+1];NumC=CurCol;Fst=0;}
         for (k=0;k<=NumC-1;k++) ar[k][CurRow]=pnt[k+1];
        }
     }
   NumR=CurRow;CurRow=0;CurCol=0;
   DeleteEnd=DelEnd;OnlyPoints=OnlyPnt;
if (NumR==0) cout<<"Zer";
  };
int FileArr::NextRow(){ CurRow++;if (CurRow>NumR) CurRow=0;return CurRow;}
int FileArr::NextCol(){ CurCol++;if (CurCol>=NumC) CurCol=0;return CurCol;}
int FileArr::RowMinus(int k)
  {do k--; while ((k>=1) && (ar[CurCol][k]==NullNumber));return k;}
int FileArr::RowPlus(int k)
  {do k++; while ((k<=NumR) && (ar[CurCol][k]==NullNumber));return k;}
double FileArr::Yval(double x)
  {
   int k=0,k1;
   do k=RowPlus(k);
   while ((ar[0][k]<x) && (k<=NumR));
//   if (k==1) k=RowPlus(k);
   if (k>NumR) {k=RowMinus(k);if (k<=1) return 0;}
   k1=RowMinus(k);
   if (k1<1) {int k2=RowPlus(k);if (k2>NumR) return 0;k1=k;k=k2;}
   return ar[CurCol][k1]+(ar[CurCol][k]-ar[CurCol][k1])*(x-ar[0][k1])/
                                        (ar[0][k]-ar[0][k1]);
  };
void FileArr::SetIterYval()
  { int k;while (k=NextCol()) CurPos[k]=RowPlus(0);};
double FileArr::IterYval(double x)
  {
   int k=CurPos[CurCol],k1;
   while ((ar[0][k]<x-MathZer) && (k<=NumR)) k=RowPlus(k);
   if (k<=NumR)
     if ( fabs(ar[0][k]-x)<MathZer ) {CurPos[CurCol]=k;return ar[CurCol][k];}
   k1=RowMinus(k);
   if (( (k1<1) || (k>NumR) ) && DeleteEnd)  return NullNumber;
   if (k1<1) {int k2=RowPlus(k);k1=k;k=k2;}
   if (k>NumR) {int k2=RowMinus(k1);k=k1;k1=k2;}
   if ( (k1<1) && (k>NumR) ) return NullNumber;
   CurPos[CurCol]=k1;
   if ((OnlyPoints) && (min(fabs(ar[0][k]-x),fabs(ar[0][k1]-x))>MathZer))
      return NullNumber;
   return ar[CurCol][k1]+(ar[CurCol][k]-ar[CurCol][k1])*(x-ar[0][k1])/
                                        (ar[0][k]-ar[0][k1]);
  };
double FileArr::Row()
  {return ar[0][CurRow];}
int FileArr::NewXRow(int Xrow)
  {if ((Xrow!=1) && (Xrow<=NumC))
    { double *b=ar[0];ar[0]=ar[Xrow-1];ar[Xrow-1]=b;
//ExchVar(ar[0],ar[Xrow-1],sizeof(ar[0]));
      cout <<" CH";return 1;}
   return 0;
  }
void FormStp(FileArr &ar,fstream &out,double &stp,double &x)
  {
   x=ar.ar[0][1];
   stp=(ar.ar[0][ar.NumR]-x)/(ar.NumR-1);
   out<<" Step = "<<stp<<" Zero Chanel= "<<-(x-stp)/stp<<"\n";
   x-=stp;
  };
void SetNames(int argc,char *argv[],
              char *file_names[MaxSumFiles],int &num_files)
  {
   char* name=GetCmd("/f",argc,argv) ;
   if ( NULL==name )
     {for (int k=1;k<=num_files;k++) file_names[k]=strdup(argv[k]); }
   else
     {
      fstream input(name,ios::in);
      char tmp[256];
      argc=0;
      while (!seek_eof(input)) {input>>tmp;argc++;file_names[argc]=strdup(tmp); }
      num_files=argc;
     }
   for (int k=1;k<=num_files;k++) cout<<file_names[k]<<"\n";
  };
void SumFiles(char *file_names[],int num_files,fstream &out)
  {
	 int k;
   fstream in_files[MaxSumFiles+1];
   for ( k=1;k<num_files;k++) in_files[k].open(file_names[k],ios::in);
   int Cur_Col=0,Add;
   double Cur_Row[MaxCol+1],tmp;
   while (!seek_eof(in_files[1]))
     {
      Cur_Col=0;
      for (k=1;k<num_files;k++)
        {
         Add=EnterStr(in_files[k],&Cur_Row[Cur_Col],MaxCol-Cur_Col);
         if (k!=1) Cur_Row[Cur_Col+1]=tmp;
         Cur_Col+=Add;
         tmp=Cur_Row[Cur_Col];Cur_Col--;
        }
      Cur_Col++;
      WriteStr(out,Cur_Row,Cur_Col);
     }
  };
/*
double *SetOneRow(FileArrPtr &ar,int num_files,int OnlyPnt)
  {
   double *tmp=new double[ar[1]->NumR+1];tmp[0]=ar[1]->NumR;
   movmem(ar[1]->ar[0],tmp,sizeof(double )*(ar[1]->NumR+1));
   return tmp;
  };
*/
void main(int argc,char *argv[])
  {
   argc--;
   char *file_names[MaxSumFiles];
   int AutoOverwrite=0,num_files=argc;
   if (NULL!=GetCmd("/a",argc,argv)) {AutoOverwrite=1;num_files--;}
   char *NewXRow=GetCmd("/x",argc,argv);
   int Xrow=0;if (NewXRow!=NULL) {Xrow=atoi(NewXRow);num_files--;cout<<Xrow<<"\n";}
   char *ConstStp=GetCmd("/c",argc,argv);
   if (ConstStp!=NULL) num_files--;
   char *JustSum= GetCmd("/j",argc,argv);
   if (JustSum!=NULL) num_files--;
   int OnlyPnt=1,DelEnd=1,FstOrder=1;
   if (GetCmd("/i",argc,argv)!=NULL) {num_files--;OnlyPnt=0;}
   if (GetCmd("/e",argc,argv)!=NULL) {num_files--;DelEnd =0;}
   if (GetCmd("/o",argc,argv)!=NULL) {num_files--;FstOrder=0;}
   if ( (argc==0) || (GetCmd("/h",num_files,argv)) )
      {cout<<"Usage:\nset1grph [files to sum] [ouput file] /key\n"<<
             "key :[c]onstant step \n"<<
             "    :[f]ile of files to sum,write (if there are too many files to write them in one string) \n"<<
             "    :[x] row\n"<<
             "    :[h] this help\n"<<
             "    :[i] interpolate points up to the first row\n"<<
             "    :[e] display also end points\n"<<
             "    :[j] just to add files (files may be of any size) \n"<<
             "    :[o] not to sort first file \n"<<
             "    :[a]uto - do not ask to overwrite\n";
       exit(1);
      }
   SetNames(argc,argv,file_names,num_files);
   fstream out;
   if (!OpenOutFileControl(file_names[num_files],out,!AutoOverwrite)) exit(1);
   if (JustSum!=NULL) { SumFiles(file_names,num_files,out);out.close();exit(0);}
   if (ConstStp!=NULL) { OnlyPnt=0;}
//   FileArr *ar[50];
   FileArrPtr ar;
	 int k;
   for (k=1;k<num_files;k++)
     { ar[k]=new FileArr(file_names[k],DelEnd,OnlyPnt);
       if (Xrow) ar[k]->NewXRow(Xrow);
       if ((k==1) && (!FstOrder)) ;
       else ar[k]->Sort();
     }
   double x,stp,y;
   if (ConstStp!=NULL) { FormStp(*ar[1],out,stp,x);}
   out<<setiosflags(ios::showpoint|ios::scientific|ios::left);
   for (k=1;k<num_files;k++) ar[k]->SetIterYval();
//   double *FirstRow=SetOneRow(ar,num_files,OnlyPnt);
   while (ar[1]->NextRow())
//   for (int k0=1;k0<=FirstRow[0];k0++)
     {
      if (ConstStp!=NULL) x+=stp;
      else x=ar[1]->Row();
//      else x=FirstRow[k0];
      if (x==NullNumber) break;
      out<<setw(13)<<setprecision(6)<<x<<"   ";
      for (int k=1;k<num_files;k++)
        {
         while (ar[k]->NextCol())
           {
            if (!FstOrder)y=ar[k]->Yval(x);
            else y=ar[k]->IterYval(x);
            if ( y==NullNumber ) {out<<setw(13)<<"\"\""<<"  ";}
            else  out<<setw(13)<<setprecision(6)<<y<<"  ";
           }
        }
      out<<"\n";
     }
   out.close();
//   delete FirstRow;
  };
