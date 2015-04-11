

#include "util.h"

void main( int argc, char *argv[] )
  {
   double BufArr[256],Min=1e30;
   if (argc<3) {cout<<" Usage: hauk_min file_read file_write Num_Pnt_in_Circle Min_Make\n"<<
                      "        revert data and [sets in circle min value]  \n";exit(0);}
   fstream in(argv[1],ios::in);
   fstream out(argv[2],ios::out);
   int NumPnt=atoi(argv[3]),Pos,MinMake=atoi(argv[4]);
//cout<<" NumPnt "<<NumPnt<<"\n";ChRead();
   while (!seek_eof(in))
     {
      Min=1e30;Pos=1;
      for (int k=1;k<=NumPnt;k++)
        {in>>BufArr[k];if (BufArr[k]<Min) {Min=BufArr[k];Pos=k;}}
      if (MinMake) { for (k=1;k<=Pos;k++) BufArr[k]=Min;}
      for (k=NumPnt;k>=1;k--) out<<BufArr[k]<<"\n";
     }
   in.close();out.close();
  };    
