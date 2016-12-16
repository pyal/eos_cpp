#ifndef __DAT_MANI_H
#define __DAT_MANI_H

#include "dat_fac.h"
#include "dat_reg.h"
#include "spline.h"

#define MaxNumCol 500
#define MaxNumRow 5000
//template <class T> void SetTDataColumn(TData<T> &dat,int Col,double  Val);

void ConstructSpl(TData<double> &XY,int X,int Y,TData<double> *&S);
   
double EvalSpl(TData<double> &XY,int X,int Y,TData<double> &S,double x_v);
          
template <class T> void SetTDataColumn(TData<T> &dat,int Col,double  Val)
  {if ((Col<=dat.N) && (Col>0))
    {for (int k=0;k<dat.I[Col];k++) dat.D[Col][k]=Val;}
   else cout<<" Wrong Col in SetTDataColumn\n";};
   
template <class T> void InputTDataF(const char *name,TData<T> *&dat)
  {
   DataRegister("TDataF",dat);
   int NumR=0,NumC=0,CurRow=0,CurCol=0;
   double *pnt=new double[MaxNumCol];
   fstream in(name,ios::in);if (seek_eof(in)) {cout<<" Cannot read file: "<<name;exit(1);}
   
   while (!seek_eof(in)) 
     { if ((NumC=EnterStr(in,&pnt[0],MaxNumCol)))  NumR++;}
   if (NumR>MaxNumRow) {cout<<"File is too long";NumR=MaxNumRow;}
   int *i=new int[NumC];for (int k=0;k<NumC;k++) i[k]=NumR;
   dat->SetDim(NumC,i);

   in.close();
   in.clear();
   in.open(name,ios::in);
   while ( (!seek_eof(in)) && (NumR>CurRow) )
     {
      if ((CurCol=EnterStr(in,&pnt[0],MaxNumCol)))
        {
         CurRow++;
         for (int k=0;k<min(CurCol,NumC);k++) dat->D[k][CurRow-1]=pnt[k+1];
         for (int k=CurCol;k<NumC;k++) dat->D[k][CurRow-1]=pnt[k+1];
        }
     }
    
   delete []i;delete []pnt;
   in.close();
  } 
template <class T> int OutputTDataF(ostream &out,TData<T> &dat)
  {
   int NumR=dat.I[0],NumC=dat.N;
   for (int k=1;k<NumC;k++) if (dat.I[k]!=NumR) 
     { cout<<" Cannot OutputTDataF - length is different.\n";return 0;}
   for (int y=0;y<NumR;y++) 
     {for (int x=0;x<NumC;x++) out<<FMT<<dat.D[x][y];out<<"\n";}
   return 1;
  } 

#endif
