// Calculating density shift Molecular - Metal
// input file clumns: Density, Pcold molecular, Pcold Metal
// no output Density, DensCoef
// output Deby1, Deby2
#include "dat_mani.h"
#include "zero.h"

#include "util.h"

double StndErr=1e-6;

TData<double> *G_in1;
TData<double> **G_S;

#define H2_Mol_Met 1.31

double Deby(double Dens,int k1) //k1 1-2
  {
      double dr=Dens*StndErr;
      double Pc=EvalSpl(*G_in1,0,k1,*G_S[k1-1],Dens);
      double C2=(EvalSpl(*G_in1,0,k1,*G_S[k1-1],Dens+dr)-Pc)/dr;
      return pow(Dens,1./3)*sqrt(max(C2-2*Pc/(3*Dens),StndErr));
  };
double MetCoef,H2_Deby;
double EqDebyExpr(double x)
  { return H2_Deby-MetCoef*Deby(x,2);}
double EqPresExpr(double x)
  { return EvalSpl(*G_in1,0,1,*G_S[0],x)-EvalSpl(*G_in1,0,2,*G_S[1],x);}
double SetDensCoef(double x)
  {
   double Dens=H2_Mol_Met;
//   if (!ZeroNewtonIt(EqPresExpr,Dens,StndErr*1e-3,10000))
//        {cout<<" Bad Dens in Eq_Pres "<<EqPresExpr(Dens)<<"\n";}
   MetCoef=Deby(Dens,1)/Deby(Dens,2);
   H2_Deby=Deby(x,1);
   Dens=x;
   if (!ZeroNewtonIt(EqDebyExpr,Dens,StndErr*1e-3,10000))
        {cout<<" Bad Dens in Eq_Deby "<<EqDebyExpr(Dens)<<"\n";}
   return 1-x/Dens;
  }; 
   

void main( int argc, char *argv[] )
  {
cout<<" coreleft "<<Coreleft()<<"\n";
   TData<double> *in1=NULL;
   if (argc!=3)
     {
      cout<<" argc "<<argc<<" \nUsage \n"<<argv[0]
          <<" File_Dens_PMol_PMet FileToWrite_Deby1_Deby2\n";
      exit(1);
     };     
   fstream f_out(argv[2],ios::out);
   InputTDataF(argv[1],in1);
   int NC=in1->N,NR=in1->I[0],k;
cout<<" NC "<<NC<<" NR "<<NR<<"\n";

   TData<double> **S=new TData<double>*[NC-1];
   for (k=1;k<NC;k++) {S[k-1]=NULL;ConstructSpl(*in1,0,k,S[k-1]);}
cout<<" Constructed\n";
cout<<" coreleft "<<coreleft()<<"\n";

   double Row[5];
   G_S=S;G_in1=in1;

   MetCoef=Deby(H2_Mol_Met,1)/Deby(H2_Mol_Met,2);
   for (k=0;k<NR;k++)
     {
      Row[1]=in1->D[0][k];
      Row[2]=Deby(Row[1],1);Row[3]=Deby(Row[1],2)*MetCoef;
      Row[4]=SetDensCoef(Row[1]);
      WriteStr(f_out,&Row[0],4);
     }
   delete in1;
   for (k=0;k<NC-1;k++) delete S[k];delete S;
   f_out.close();
cout<<" coreleft "<<coreleft()<<"\n";
  }; 


