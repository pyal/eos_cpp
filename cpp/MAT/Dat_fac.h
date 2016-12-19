#ifndef __DAT_FAC_H
#define __DAT_FAC_H

#include "lib/std/deffunc.h"
#include "lib/std/stdexception.h"
//#include "f2c.h"

//#include "alloc.h"


//format TColumn:  Number N row x
//format TData  :  Number N Numberi Ni
//format TTable :  NumberX Nx NumberY Ny

struct InterfaceIO
  {
//   const char * const name;
   virtual const char *InterfaceIOName() {return " InterfaceIO ";};

   virtual int ReadIni(istream &input){return 1;};
   virtual void SaveIni(ostream &output){};

   virtual void ReadPar(istream &input){};
   virtual void SavePar(ostream &output){};

   virtual ~InterfaceIO(){};
  };



template <class T>
struct TData:InterfaceIO
  {
   T **D;
   int *I,N;
   TData(){D=NULL;I=NULL;N=0;}
   TData(int n,int *i=NULL) {D=NULL;I=NULL;SetDim(n,i);}
   TData(TData &dat);
   virtual TData& operator=(TData &dat);
   virtual void SetDim(int n,int *i=NULL);
   virtual void Delete();
   void SaveIni(ostream &out);
   int  ReadIni(istream &in );
   ~TData(){Delete();}
   virtual void AddColumn(T* column,int Num);
   virtual void SetColumn(T* column,int Num,int Col);
   static TData *CurDat;
   static int SortColumn;
   static int SortCmp(const void *v1,const void *v2);
   static void FindMinMax(TData<T> &da,int row, double &Min, double &Max){
		Min = 1/MathZer;
		Max = -1/MathZer;
		for(int i1=0;i1<da.I[row];i1++){
			if (Min>da.D[row][i1])
				Min=da.D[row][i1];
			if (Max<da.D[row][i1])
				Max=da.D[row][i1];
		}
	}
   virtual void SortData(int SortCol=0);
  };

template <class T> extern int TTableTColumn(TData<T> &Res,TData<T> &dat);
template <class T> extern int TTColumnTable(TData<T> &Res,TData<T> &dat);

// NumX 10 NumY 10 ExpX 0 ExptY 0
template <class T> extern int ReadTableRow(istream &in,TData<T> *dat);

template <class T>
struct TTable:TData<T>
  { 
   static const char * const name;
   virtual const char *InterfaceIOName()  {return name;}
   TTable():TData<T>(3){};
   TTable(TData<T> &da_)
       : TData<T>(da_) {
   }
     //{ TData<T>::TData(da_);}
   virtual TData<T>& operator=(TData<T> &dat)
     {if (Stricmp(dat.InterfaceIOName(),"TColumn")==0)
        TTableTColumn((TData<T>&)(*this),dat);
//      else *this=(TData<T>::operator=)(dat);
      else (TData<T>::operator=)(dat);
//cout<<"TData<T>& operator=(TData<T> &dat) dat.InterfaceIOName() "<<
//     dat.InterfaceIOName()<<"\n";
      return *this;
     }
   T& operator()(int i,int j){return this->D[2][this->I[0]*i+j];}
   T* RowGet(int i,int RowNum)
   {if ((RowNum>1)||(RowNum<0)) return NULL;T* ret=new T[this->I[RowNum]];
    if (RowNum==1) {int strt=i*this->I[1];for (int k=0;k<this->I[0];k++) ret[k]=this->D[2][strt+k];}
    else {for (int k=0;k<this->I[0];k++)  ret[k]=this->D[2][k*this->I[1]+i];}
    return ret;
   }
   int RowSet(int i,int RowNum,T* val)
   {if ((RowNum>1)||(RowNum<0)) return 0;
    if (RowNum==1) {int strt=i*this->I[1];for (int k=0;k<this->I[0];k++) this->D[2][strt+k]=val[k];}
    else {for (int k=0;k<this->I[0];k++)  this->D[2][k*this->I[1]+i]=val[k];}
    return 1;
   }
   void SaveIni(ostream &out);
   int  ReadIni(istream &in );
  };  
   
template <class T>
struct TColumn:TData<T>
  { 
   static const char * const name;
   virtual const char *InterfaceIOName()  {return name;}
   TColumn():TData<T>(3){};
   TColumn(TData<T> &dat)
     { TData<T>::TData(dat);}
   virtual TData<T>& operator=(TData<T> &dat)
     {if (Stricmp(dat.InterfaceIOName(),"TTable")==0)
        TTColumnTable((TData<T>&)(*this),dat);
//      else *this=(TData<T>::operator=)(dat);
      else (TData<T>::operator=)(dat);
      return *this;
     }
   void SaveIni(ostream &out);
   int  ReadIni(istream &in );
  };  

template <class T> struct TDataF:TData<T>
  { 
   static const char * const name;
   virtual const char *InterfaceIOName()  {return name;}
  };                 

template <class T,class T1> void SetDifTData(TData<T> &Res,TData<T1> &dat)
  {
   char name[256];strcpy(name,Res.InterfaceIOName());
   TData<T1> *d;DataRegister(name,d);*d=dat;
//cout<<" SETdifdata core "<<coreleft()<<"\n";
   Res.SetDim(d->N,d->I);
//cout<<" SETdifdata core  mem "<<coreleft()<<"\n";
   for (int k=0;k<Res.N;k++)
     { for (int i=0;i<Res.I[k];i++) Res.D[k][i]=d->D[k][i];}
   delete d;
  }; 



template <class T> TData<T>::TData(TData<T> &dat)
  {D=NULL;I=NULL;SetDim(dat.N,dat.I);
   for (int k=0;k<N;k++)
     { for (int i=0;i<I[k];i++) D[k][i]=dat.D[k][i];}
  };   
template <class T> TData<T>& TData<T>::operator=(TData<T>& dat)
  {
//cout<<" TData= \n";
   if (Stricmp(dat.InterfaceIOName(),InterfaceIOName())!=0)
       throw info_except(" Different types from %s to %s . Exiting\n", dat.InterfaceIOName(),InterfaceIOName());
   SetDim(dat.N,dat.I);
   int l=sizeof(T);
   for (int k=0;k<N;k++) MemoryMove(dat.D[k],D[k],l*dat.I[k]);
   return *this;
  };   
#ifndef WCPP
template <class T> void TData<T>::SetDim(int n,int *i0)
#else
template <class T> void TData<T>::SetDim(int n,int *i0)//=NULL)
#endif
  {
   Delete();N=n;D=new T*[n];I=new int[n];
   if (i0!=NULL) MemoryMove(i0,I,n*sizeof(int));
   for (int k=0;k<n;k++) 
     if (i0==NULL) {I[k]=0;D[k]=NULL;}else D[k]=new T[I[k]];
  };
template <class T> void TData<T>::Delete()
   {if (I!=NULL) 
     {for (int k=0;k<N;k++) delete D[k];
      delete D;delete I;D=NULL;I=NULL;N=0;}
   };
template <class T> void TData<T>::SaveIni(ostream &out)
  {
   out<<"NumVar "<<N<<"\n";
   for (int k=0;k<N;k++) out<<" NumV["<<k+1<<"]  "<<I[k];
   out<<"\n";
   int s=0;
   for (int k=0;k<N;k++)
     { s=0; for (int i=0;i<I[k];i++) 
              {out<<FMT<<D[k][i];s++;if (s>120) {out<<"\n";s=0;}}
       out<<"\n"; }
  };    
template <class T> int  TData<T>::ReadIni(istream &in )
  {
   char tmp[256];in>>tmp>>N;
   int *i=new int[N];
   for (int k=0;k<N;k++) in>>tmp>>i[k];
   SetDim(N,i);delete[] i;
   for (int k=0;k<N;k++)
      {for (int p=0;p<I[k];p++) in>>D[k][p];}
   return 1;
  };
template <class T> void TTable<T>::SaveIni(ostream &out)
  {
   int k;
   out<<"NumberX "<<this->I[0]<<" NumberY "<<this->I[1]<<"\n";
   for ( k=0;k<this->I[0];k++) out<<this->D[0][k]<<"  ";out<<"\n";
   for ( k=0;k<this->I[1];k++) out<<this->D[1][k]<<"  ";out<<"\n";
   for ( k=0;k<this->I[2];k++)
     {out<<FMT<<this->D[2][k];if ((double)(k+1)/this->I[1]==(k+1)/this->I[1]) out<<"\n";}
   out<<"\n";  
  };    
template <class T> int  TTable<T>::ReadIni(istream &in )
  {
   char tmp[256];
   int x,y,k;
   in>>tmp>>x>>tmp>>y;
   int i[3]={x,y,x*y};
      this->SetDim(3,i);
   for (k=0;k<x;k++)   in>>this->D[0][k];
   for (k=0;k<y;k++)   in>>this->D[1][k];
   for (k=0;k<x*y;k++) in>>this->D[2][k];
   return 1;
  };
template <class T> void TColumn<T>::SaveIni(ostream &out)
  {
   int i,k;
   out<<"NumberCol "<<this->N<<" NumberRow  "<<this->I[0]<<"\n";
   for (k=0;k<this->I[0];k++)
     { for (i=0;i<this->N;i++) out<<FMT<<this->D[i][k]; out<<"\n"; }
  };    
template <class T> int  TColumn<T>::ReadIni(istream &in )
  {
   int row,i,k,*a;
   char tmp[256];in>>tmp>>this->N>>tmp>>row;
   a=new int[this->N];for (k=0;k<this->N;k++) a[k]=row;this->SetDim(this->N,a);delete a;
   for (k=0;k<this->I[0];k++)
     {for (i=0;i<this->N;i++) in>>this->D[i][k]; }
   return 1;
  };



template <class T> int TTableTColumn(TData<T> &Res,TData<T> &dat)
  {
   if ((Stricmp(dat.InterfaceIOName(),"TColumn")!=0) ||
       (Stricmp(Res.InterfaceIOName(),"TTable")!=0))
     throw info_except("Wrong types in TTableTColumn %s != TColumn || %s !=TTable\n", dat.InterfaceIOName(), Res.InterfaceIOName());
   if ( (dat.N!=3) || (dat.I[0]!=dat.I[1]) || (dat.I[0]!=dat.I[2]) )
     throw info_except(" Bad dimensions in TTableTColumn %i  %i %i (have to be same)\n", dat.I[0], dat.I[1], dat.I[2]);
   int k=1,x=0,y=1,t=0;
   while ((y<dat.I[0]) && (fabs(dat.D[0][y]-dat.D[0][y-1])<MathZer)) y++;
   if (int(dat.I[0]/y)!=(double )dat.I[0]/y || y==1) {
       dat.SaveIni(cout);
       throw info_except(" Bad y dimension in TTableTColumn %i and got num same x %i\n", dat.I[0], y); 
   }
   x=dat.I[0]/y;
   int i[3]={x,y,dat.I[0]};Res.SetDim(3,i);
   for (k=0;k<x;k++) {Res.D[0][k]=dat.D[0][t];t+=y;}
   for (k=0;k<y;k++) Res.D[1][k]=dat.D[1][k];      
   for (k=0;k<dat.I[0];k++) Res.D[2][k]=dat.D[2][k];      
   return 1;
  }; 
     

template <class T> int TTColumnTable(TData<T> &Res,TData<T> &dat)
  {
   if ( (Stricmp(dat.InterfaceIOName(),"TTable")!=0) ||
        (Stricmp(Res.InterfaceIOName(),"TColumn")!=0))
        throw info_except(" Wrong types in TTColumnTable %s %s \n", dat.InterfaceIOName(), Res.InterfaceIOName());
   if ( (dat.N!=3) || (dat.I[0]*dat.I[1]!=dat.I[2]) )
       throw info_except(" Bad dimensions in TTColumnTable %i * %i != %i\n", dat.I[0], dat.I[1], dat.I[2]);
   int i[3]={dat.I[2],dat.I[2],dat.I[2]};Res.SetDim(3,i);
   int k,k1,t=0;
   for (k=0;k<dat.I[0];k++) 
     {for (k1=0;k1<dat.I[1];k1++) 
       {Res.D[0][t]=dat.D[0][k];Res.D[1][t]=dat.D[1][k1];
        Res.D[2][t]=dat.D[2][t];t++;}}
   return 1;
  }; 

template <class T>  int ReadTableRow(istream &in,TData<T> *dat)
  {
   if (Stricmp(dat->InterfaceIOName(),"TTable")!=0)
     { cout<<" ReadTableRow work only with TTable. Wrong.";return 0;}
   char tmp[256];
   int i0,i1,ExpX,ExpY,k;
// NumX 10 NumY 10 ExpX 0 ExptY 0
   in>>tmp>>i0>>tmp>>i1>>tmp>>ExpX>>tmp>>ExpY;
   int i[3]={i0,i1,i1*i0};dat->SetDim(3,i);
    
   double Xl,Yl,Xu,Yu;
// Xlow 1 Xup  5 Ylow  0  Yup 10
   in>>tmp>>Xl>>tmp>>Xu>>tmp>>Yl>>tmp>>Yu;
cout<<" Yup "<<Yu<<"\n";
   if ( ((ExpX) && (Xu*Xl<=0)) || ((ExpY) && (Yu*Yl<=0)) )
     if (Xu*Xl<=0)  { cout<<"Different Singns in exp format. Exiting";return 0;}
   double ExpSX=1,ExpSY=1;
   if ( (ExpX) && (Xu<0) )ExpSX=-1;if ( (ExpY) && (Yu<0) )ExpSY=-1;
   if  (ExpX) { Xl=log(Xl*ExpSX);Xu=log(Xu*ExpSX);}
   if  (ExpY) { Yl=log(Yl*ExpSY);Yu=log(Yu*ExpSY);}
   double Xstp=(Xu-Xl)/(i0-1),Ystp=(Yu-Yl)/(i1-1),Xcur=Xl,Ycur=Yl;
   for (k=0;k<i0;k++) { dat->D[0][k]=Xcur;Xcur+=Xstp;}
   for (k=0;k<i1;k++) { dat->D[1][k]=Ycur;Ycur+=Ystp;}
   if  (ExpX) {for (k=0;k<i0;k++) dat->D[0][k]=ExpSX*exp(dat->D[0][k]);}
   if  (ExpY) {for (k=0;k<i1;k++) dat->D[1][k]=ExpSY*exp(dat->D[1][k]);}
   for (k=0;k<i1*i0;k++) dat->D[2][k]=0;
   return 1;
  }; 

template <class T> void TData<T>::AddColumn(T* column,int Num) {
   int *i = new int[N+1];
   if (I != NULL) 
       MemoryMove(I, i, N*sizeof(int));
   T **d = new T*[N + 1];
   int l = sizeof(T);
   for (int k = 0; k < N; k++) {
       d[k] = new T[I[k]];
       MemoryMove(D[k], d[k], l * I[k]);
   }
   i[N] = Num;
   d[N] = new T[Num];
   MemoryMove(column, d[N], l * Num);
   if (I != NULL) {
       for (int k = 0; k < N; k++) 
           delete D[k];
       delete D;
       delete I;
   }
   N++;
   D = d;
   I = i;
  };   
template <class T>  void TData<T>::SetColumn(T* column,int Num,int Col)
  {
   if ( (Col>=N) || (Col<0) )
       throw info_except("Cannot set column <%i> Total number of columns <%i>\n", Col, N);
   delete D[Col];
   I[Col]=Num;D[Col]=new T[Num];MemoryMove(column,D[Col],sizeof(T)*Num);
  };   

//static int SortCmp(const void *v1,const void *v2)
 template <class T> int TData<T>::SortCmp(const void *v1,const void *v2)
  {
//   int i1=*((int*)v1),i2=*((int*)v2);
   T *t1=&(CurDat->D[SortColumn][*((int*)v1)]),*t2=&(CurDat->D[SortColumn][*((int*)v2)]);
   if (*t1>*t2) return 1;
   if (*t1==*t2) return 0;
   return -1;
  }
//extern  int DataStore<class T>(ostream &out,TData<T> *dat);

template <class T>  void TData<T>::SortData(int SortCol)
  {
   if (N<=0) return;
   int k,Num=I[0];
   if ( (SortCol>=N) || (SortCol<0) )
     { cout<<" Formal Error in TData::SortData. NumCol is:"<<N+1
           <<" Setting Col:"<<SortCol+1<<"\n";return ;}
   for ( k=0;k<N-1;k++) if (I[k]!=I[k+1]) 
     { cout<<" Error in TData::SortData. Dif Sizes of columns I[k]= "<<I[k]
           <<" I[k+1]= "<<I[k+1]<<"\n";return ;};
   int *i=new int[Num];for (k=0;k<Num;k++) i[k]=k;
   SortColumn=SortCol;CurDat=this;
   qsort(i,Num,sizeof(int),SortCmp);
   TData<T> tmp=*this;
   for (k=0;k<Num;k++) if (i[k]!=k) { for (int k1=0;k1<N;k1++) D[k1][k]=tmp.D[k1][i[k]];}
//DataStore(cout,&tmp);cout"\n\n\n\n\n";DataStore(cout,this);

   delete []i; return;
  }


#endif
