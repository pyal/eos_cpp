//#include "dat_reg.h"
//#include "dat_fac.h"
#include "dat_mani.h"

#include "util.h"

void main( int argc, char *argv[] )
  {
   if (argc!=4)
     {
      cout<<" argc "<<argc<<" \nUsage \n"<<argv[0]
          <<" File_X_col File_To_Build_Spline File_To_Write\n";
      exit(1);
     };     
   fstream f_out(argv[3],ios::out);
   TData<double> *in=NULL,*in1=NULL,*out=NULL;
cout<<" coreleft "<<coreleft()<<"\n";
   InputTDataF(argv[1],in);
cout<<" NC "<<in->N<<" NR "<<in->I[0]<<"\n";
   InputTDataF(argv[2],in1);
cout<<" NC "<<in1->N<<" NR "<<in1->I[0]<<"\n";
   DataRegister("TDataF",out);
   int NC=in1->N,NR=in->I[0];
   int *I=new int[in1->N];for (int k=0;k<NC;k++) I[k]=NR;
cout<<" NC "<<NC<<" NR "<<NR<<"\n";
   TData<double> **S=new TData<double>*[NC-1];
   out->SetDim(NC,I);
   for (k=1;k<NC;k++) {S[k-1]=NULL;ConstructSpl(*in1,0,k,S[k-1]);}
cout<<" Constructed\n";
cout<<" coreleft "<<coreleft()<<"\n";
   for (k=0;k<in->I[0];k++)
     {
      double x=in->D[0][k];
      out->D[0][k]=x;
      for (int k1=1;k1<NC;k1++) out->D[k1][k]=EvalSpl(*in1,0,k1,*S[k1-1],x);
     };
   OutputTDataF(f_out,*out);
   delete in;delete in1;delete out;delete I;
   for (k=0;k<NC-1;k++) delete S[k];
   delete S;
   f_out.close();
cout<<" coreleft "<<coreleft()<<"\n";
  }; 
