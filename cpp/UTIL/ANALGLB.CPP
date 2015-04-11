#include "analglb.h"
#define DROWD 1
#define DROWN 2
#define DCOLD 3
#define DCOLN 4
#define FORMR 5
#define FAIL1 6
#define MEAN_N  7
#define MEAN_V  8

WorkDo analglb(char *&input)
  {
   WorkDo ret;ret.Type=FAIL1;
   while (((*input)==' ') ) input++;
   if (*input==0) return ret;
   if (StrBeg(input,"DEL_ROW_N_FRV_TOV("))
     {if (!IsInt  (input,ret.p1)) return ret;
      if (!IsDouble(input,ret.d1)) return ret;
      if (!IsDouble(input,ret.d2)) return ret;
      if (!StrBeg(input,")")) return ret;
      ret.Type=DROWD;
     }
   else
     if (StrBeg(input,"DEL_ROW_FR_TO(")) 
     {if (!IsInt(input,ret.p1)) return ret;
      if (!IsInt(input,ret.p2)) return ret;
      ret.p3=0;
      if (!StrBeg(input,")")) return ret;
      ret.Type=DROWN;
     }
   else
     if (StrBeg(input,"DEL_COL_FR_TO("))
     {if (!IsInt(input,ret.p1)) return ret;
      if (!IsInt(input,ret.p2)) return ret;
      if (!StrBeg(input,")")) return ret;
      ret.Type=DCOLD;
     }
   else
     if (StrBeg(input,"MEAN_N("))
     {if (!IsInt(input,ret.p1)) return ret; //1 - 2- Row sum Fr To 3-4 - for Col
      if (!IsInt(input,ret.p2)) return ret;
      if (!IsInt(input,ret.p3)) return ret;
      if (!IsInt(input,ret.p4)) return ret;
      if (!StrBeg(input,")")) return ret;
      ret.sum=new double [ret.p4-ret.p3+2];
      for (int k=1;k<=ret.p4-ret.p3+1;k++) ret.sum[k]=0;
      ret.d1=0;
      ret.Type=MEAN_N;
     }
   else
     if (StrBeg(input,"MEAN_V("))
     {if (!IsDouble(input,ret.d1)) return ret; //1 - 2- Row sum Fr To 3-4 - for Col
      if (!IsDouble(input,ret.d2)) return ret;
      if (!IsInt(input,ret.p3)) return ret;
      if (!IsInt(input,ret.p4)) return ret;
      if (!StrBeg(input,")")) return ret;
      ret.sum=new double [ret.p4-ret.p3+2];
      for (int k=1;k<=ret.p4-ret.p3+1;k++) ret.sum[k]=0;
      ret.p1=0;ret.p2=0;
      ret.Type=MEAN_V;
     }
   else
     if (StrBeg(input,"FORM_FILE(")) 
     {if (!IsInt(input,ret.p1)) return ret;
cout<<" p1= "<<ret.p1<<"\n";
      int error=0,n=0;ret.arf=new int[ret.p1+1];ret.arn=new int[ret.p1+1];
      while ((++n<=ret.p1) && (!error)  )
        {  
         if (!IsInt(input,ret.arf[n])) error=1;
         if (!IsInt(input,ret.arn[n])) error=1;
        }
      if ((!StrBeg(input,")")) || (error)) 
        {delete ret.arf; delete ret.arn;return ret;}
      ret.Type=FORMR;
     }
  return ret;
 };

int FormWork(char *s,WorkDo *wdo)
  {
   char *input=new char[256];
   char *in_del=input;
   strupr(strcpy(input, s));
   int k=0;
   while (*input!=0) wdo[++k]=analglb(input);
   wdo[++k].Type=FAIL1;
   delete in_del;
   return 1;
  };
int oneaction(WorkDo &wdo,double (&ar)[MAXFILEWORK][MAXROWWORK],int col)
  {
   if (wdo.Type==FAIL1) return 0;
   int k; 
   switch (wdo.Type)
     {
      case DROWD :if ((ar[1][wdo.p1]<=wdo.d1) 
                            || (ar[1][wdo.p1]>=wdo.d2)) col=0;
                  
                  break;
      case DROWN :wdo.p3++;if ((wdo.p3>=wdo.p1) 
                            && (wdo.p3<=wdo.p2)) col=0;
                  break;
      case DCOLD :break;
      case MEAN_N:
                  wdo.d1++;if ((wdo.d1>=wdo.p1) && (wdo.d1<=wdo.p2)) 
                    {
                     for (int k=1;k<=wdo.p4-wdo.p3+1;k++) 
                                       wdo.sum[k]+=ar[1][k+wdo.p3-1];
                    }
                  if (wdo.d1==wdo.p2) 
                    {
                     for (int k=1;k<=wdo.p4-wdo.p3+1;k++) 
                         ar[1][k]= wdo.sum[k]/(wdo.p2-wdo.p1+1);
                     col=wdo.p4-wdo.p3+1;break;
                    }
                      
                  col=0;     break;
      case MEAN_V:
                  if ((ar[1][1]>=wdo.d1) && (ar[1][1]<=wdo.d2)) 
                    {
                     for (int k=1;k<=wdo.p4-wdo.p3+1;k++) 
                                       wdo.sum[k]+=ar[1][k+wdo.p3-1];
                     wdo.p1=1;wdo.p2++;
                    }
                  else if (wdo.p1) 
                    {
                     for (int k=1;k<=wdo.p4-wdo.p3+1;k++) 
                         ar[1][k]= wdo.sum[k]/wdo.p2;
                     wdo.p2=0;
                     col=wdo.p4-wdo.p3+1;wdo.p1=0;break;
                    }
                      
                  col=0;     break;


      case DCOLN :
                  {
                   int d=wdo.p2-wdo.p1+1;
                   for (k=wdo.p2+1;k<=col;k++) ar[1][k-d]=ar[1][k];
                   break;
                  }
      case FORMR :double ar1[MAXROWWORK];
                  col=wdo.p1;
                  for (k=1;k<=col;k++) ar1[k]=ar[wdo.arf[k]][wdo.arn[k]];
                  for (k=1;k<=col;k++) ar[1][k]= ar1[k];
                  break;

     }
   return col;
  };
          
int DoMes(WorkDo *wdo,double (&ar)[MAXFILEWORK][MAXROWWORK],int col)
  {
   int k=0;
   while (wdo[++k].Type!=FAIL1)
     { col=oneaction(wdo[k],ar,col); }
   return col;
  };

          


