#include "memf.h"
void main()
  {
cout<<coreleft();
   int num; cout<<"input num ?"; cin>>num;
   double par[4];
   cout<<"input x2 coef ?"; cin>>par[1];
   cout<<"input x coef ?";  cin>>par[2];
   cout<<"input gr coef ?"; cin>>par[3];
   memf *f=new memf("out",ios::out," Where to write",1);
//   delete f;
   *(f->mfbuf)<<" x2 coef-> " <<par[1]<<";x coef-> "<<par[2]<<";gr coef-> "<<par[3]
				<<"\n";
   for (int k=1;( (k<=num) && (k<=255) );k++)
     *(f->mfbuf)
//     <<k<<"   "
		  <<par[1]*par[3]/(sqr(k+par[2])+sqr(par[3]))<<"\n";
    delete f;
cout<<coreleft();
  };
/*
*/