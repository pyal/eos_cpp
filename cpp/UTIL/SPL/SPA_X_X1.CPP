//#include "dat_reg.h"
//#include "dat_fac.h"
#include "dat_mani.h"
#include "spl_fac.h"

#include "util.h"

void main( int argc, char *argv[] )
  {
   if (argc!=4)
     {
      cout<<" argc "<<argc<<" \nUsage \n"<<argv[0]
          <<" File_X_col File_To_Build_Spline File_To_Write\n"
          <<" File_To_Build_Spline: NumX_Grid 50 Misfit 0.1+ data\n";
      exit(1);
     };     
   char tmp[256];
   double Misf,Mis;
   int NumX;
   fstream f_out(argv[3],ios::out);
   TData<real> *in=NULL,*in1=NULL,*out=NULL;
cout<<" coreleft "<<coreleft()<<"\n";
   InputTDataF(argv[1],in);
cout<<" NC "<<in->N<<" NR "<<in->I[0]<<"\n";
   InputTDataF(argv[2],in1);
cout<<" NC "<<in1->N<<" NR "<<in1->I[0]<<"\n";
   fstream f_in(argv[2],ios::in);f_in>>tmp>>NumX>>tmp>>Misf;f_in.close();
   DataRegister("TDataF",out);
   int NC=in1->N,NR=in->I[0];
   int *I=new int[in1->N];for (int k=0;k<NC;k++) I[k]=NR;
   out->SetDim(NC,I);movmem(in->D[0],out->D[0],sizeof(out->D[0][0])*NR);
cout<<" NC "<<NC<<" NR "<<NR<<"\n";
   CurveSpline *S=new CurveSpline[NC-1];
   int n=NumX;
   Mis=Misf;
//   TData<double> **S=new TData<double>*[NC-1];
   for (k=1;k<NC;k++) {S[k-1].Generate(NumX,Misf,0,*in1,0,k);NumX=n;Misf=Mis;}
cout<<" Constructed\n";
cout<<" coreleft "<<coreleft()<<"\n";

// for (k=1;k<NR;k++) {cout<<out->D[0][k]<<"\n";}
   for (k=1;k<NC;k++) {S[k-1].Evaluate(*out,0,k);}
   OutputTDataF(f_out,*out);
   delete in;delete in1;delete out;delete I;
//   for (k=0;k<NC-1;k++) delete S[k];
   delete S;
   f_out.close();
cout<<" coreleft "<<coreleft()<<"\n";
  }; 
