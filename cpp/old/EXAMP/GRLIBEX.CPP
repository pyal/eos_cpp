// uses grlib,memf,fobj,util;
//#include "util.h"
#include "grlib.h"
#include "memf.h"
#include <stdio.h>
extern unsigned _stklen=40000;
void main(int argc,char *argv[])
  {
   double pnt[5][512];//=new double [10000];
   int Row,Col;
//   memf *f=new memf(argv[1],ios::in,"Graphic  points",1);
//   InputArr(*f->mfbuf,(double*)(&pnt),Row,Col,5,512);
   fstream in(argv[1],ios::in);
cout<<coreleft();ChRead();
   InputArr(in,&pnt[0][0],Row,Col,5,512 );
cout<<argv[1]<<"  "<<Row<<"  "<<Col<<"\n";
   if (Row<2) exit(1);
cout<<coreleft();ChRead();

   int graphdrive=DETECT,graphmode;
   initgraph(&graphdrive,&graphmode,"");
   cleardevice();
   setcolor(YELLOW);
   int color=YELLOW;
   GrArr *ar;
   ar=new GrArr(&pnt[0][0],&pnt[1][0],Row);
cout<<coreleft();ChRead();
   GrScr *sum=AxGraph(ar,"X- text","Y - text ");
   sum->LstAdd(ar);
   for (int k=2;k<Col;k++)
    {
     ar=new GrArr(&pnt[0][0],&pnt[k][0],Row);
     ar->PixColor(++color);
     sum->LstAdd(ar);
    }
   Rect Gr=sum->MaxGrDim();
   sum->GrDim(Gr);
   cout<<" Beg to Show\n";ChRead();
   sum->Show();
   ChRead();
   Rect cur=sum->ScrDim();
   sum->ScrDim( ar->BlowRect(cur,ar->CentRect(cur),0.5) );
   ChRead(); //   sum->Hide();
cout<<coreleft();ChRead();
delete sum;
   ChRead();
   closegraph();
cout<<coreleft();ChRead();
  
   graphdrive=DETECT,graphmode;
   initgraph(&graphdrive,&graphmode,"");
   cleardevice();
   setcolor(YELLOW);
   color=YELLOW;
   ar=new GrArr(&pnt[0][0],&pnt[1][0],Row);
cout<<coreleft();ChRead();
   sum=AxGraph(ar,"X- text","Y - text ");
   sum->LstAdd(ar);
   for (k=2;k<Col;k++)
    {
     ar=new GrArr(&pnt[0][0],&pnt[k][0],Row);
     ar->PixColor(++color);
     sum->LstAdd(ar);
    }
   Gr=sum->MaxGrDim();
   sum->GrDim(Gr);
   cout<<" Beg to Show\n";ChRead();
   sum->Show();
   ChRead();
   cur=sum->ScrDim();
   sum->ScrDim( ar->BlowRect(cur,ar->CentRect(cur),0.5) );
   ChRead(); //   sum->Hide();
cout<<coreleft();ChRead();
delete sum;
   ChRead();
   closegraph();
cout<<coreleft();
  };
