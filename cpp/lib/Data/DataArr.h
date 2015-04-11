
#ifndef DataArr_h
#define DataArr_h 1

#include "lib\ref\class_sav.h"
#include "lib\std\util.h"
#include "datavec.h"



template <class T>
struct DataArray : public SavableClass  //## Inherits: <unnamed>%3D5CFB0D01CA
{
//      DataArray ();
      DataArray (int NumCol=0, int LenCol = 0):data(0),columns(0) {SetDim(NumCol,LenCol);}
      DataArray (const DataArray<T> &vec);
	  DataArray (DataVector<int, CopyStructFast<int> > &vec):data(0),columns(0){SetDim(vec);}
      inline virtual ~DataArray(){Delete();}

      virtual int read_data_state (FilterIn &si);
      virtual int save_data_state (FilterOut &so);

      DataArray<T>& operator = (const T& vec);
      DataArray<T>& operator = (const DataVector<T>& vec);
      DataArray<T>& operator = (const DataArray<T>& vec);

	  inline DataVector<int, CopyStructFast<int> > SetDim () const {return columns;}
      inline DataVector<int, CopyStructFast<int> > SetDim (int NumCol, int LenCol = 0)
                              { DataVector<int, CopyStructFast<int> > dim(NumCol);dim=LenCol;return SetDim(dim);};
      DataVector<int, CopyStructFast<int> > SetDim (DataVector<int, CopyStructFast<int> > &vec);
      DataVector<int, CopyStructFast<int> > Resize (DataVector<int, CopyStructFast<int> > &vec);

      void AddColumn (int ColLen, T* ColVar = NULL);
      template <class CpM_>
      inline void AddColumn (const DataVector<T, CpM_>& Col) {AddColumn(Col.SetDim(),Col.Ptr());}
      template<class CpM_>
      DataVector<T, CpM_> SetColumn (int Col, int ColLen = -1, T* ColVar = NULL);
      template <class CpM_>
      inline void SetColumn (int Col, DataVector<T, CpM_> &Dat) { SetColumn<CpM_>(Col,Dat.SetDim(),(T*)Dat);}
      template<class CpM_>
      DataVector<T, CpM_> SetRow (int Row, T* RowVar = NULL);
      template <class CpM_>
      void SetRow (int Row,DataVector<T, CpM_>& RowVar );
      void CheckProperties() ;
      void Delete ();

      template<class T1>
      friend DataArray<T1> Transpon ( DataArray<T1>& ar);

      inline T& operator()  (int row,int col) { return data[col][row];}

      DataArray<T> operator * (const T& coef);
      DataArray<T> operator / (const T& coef);
      DataArray<T> operator + (const T& coef);
      DataArray<T> operator - (const T& coef);


      template<class CpM_>
      DataVector<T, CpM_> operator * (const DataVector<T, CpM_>& coef);
      template<class CpM_>
      DataArray<T> operator + (const DataVector<T, CpM_>& coef);
      template<class CpM_>
      DataArray<T> operator - (const DataVector<T, CpM_>& coef);

      DataArray<T> operator * ( DataArray<T>& coef);
      DataArray<T> operator + ( DataArray<T>& coef);
      DataArray<T> operator - ( DataArray<T>& coef);

  public:
  protected:
      int same_length;
  private:
  private: //## implementation

      DataVector<T*> data;
      DataVector<int, CopyStructFast<int> > columns;
      int square, checked;
//      T NullNumber;
};












// ===========================================================
// =================   End of the head =======================
// ===========================================================







template <class T>
DataArray<T>::DataArray (const DataArray<T> &vec):data(0),columns(0)
 {
  SetDim(vec.SetDim());
  for (int k=0;k<columns.SetDim();k++) 
    MemoryMove(vec.data[k],data[k],sizeof(T)*columns[k]);
  square=vec.square;same_length=vec.same_length;checked=vec.checked;
 };


template <class T>
int DataArray<T>::read_data_state (FilterIn &si)
 {
  DataVector <int, CopyStructFast<int> > column;
  si>>StdKey>>column>>StdKey;
  SetDim(column);
  int col,row,Ncol=columns.SetDim();
  for (col=0;col<Ncol;col++)
    { for (row=0;row<columns[col];row++) si>>data[col][row];}
  return 1;
 };

template <class T>
int DataArray<T>::save_data_state (FilterOut &so)
 {
  so<<KeyDesc("ArrDim")<<columns<<KeyDesc("ArrData")<<"\n";
  int col,row,Ncol=columns.SetDim();
  for (col=0;col<Ncol;col++)
    { for (row=0;row<columns[col];row++) so<<data[col][row];so<<KeyDesc("\n");}
  return 1;
 };

template <class T>
DataArray<T>& DataArray<T>::operator = (const T& vec)
 {
  int row,col;
  for (col=0;col<columns.SetDim();col++) {for (row=0;row<columns[col];row++) data[col][row]=vec;}
  return (*this);
 };

template <class T>
DataArray<T>& DataArray<T>::operator = (const DataVector<T>& vec)
 {int N=vec.SetDim();SetDim(N,N);
  operator=((T)0);
  operator+(vec);
  return (*this);
 };

template <class T>
DataArray<T>& DataArray<T>::operator = (const DataArray<T>& vec)
 {
  SetDim(vec.SetDim());
  for (int k=0;k<columns.SetDim();k++) 
    MemoryMove(vec.data[k],data[k],sizeof(T)*columns[k]);
  square=vec.square;same_length=vec.same_length;checked=vec.checked;
  return (*this);
 }

template <class T>
DataVector<int, CopyStructFast<int> > DataArray<T>::SetDim (DataVector<int, CopyStructFast<int> > &vec)
 {
  if (vec==columns) return columns;
  Delete();
  int k,N=vec.SetDim();
  data.SetDim(N);
  for (k=0;k<N;k++) data[k]=new T[vec[k]];
  columns=vec;
  return columns;
 }

template <class T>
DataVector<int, CopyStructFast<int> > DataArray<T>::Resize (DataVector<int, CopyStructFast<int> > &vec)
 {
  if (vec==columns) return columns;
  int k,N1=columns.SetDim(),N2=vec.SetDim();
  DataVector<int, CopyStructFast<int> > OldCol=columns;
  DataVector<T*> NewData(N2);//=data;NewData.Resize(N2);
  OldCol.Resize(N2);
  for (k=N1;k<N2;k++) 
      OldCol[k]=0;
  for (k = 0; k < N2; k++) 
      NewData[k]=new T[vec[k]];
  for (k = 0; k < min(N1, N2);k++) 
      MemoryMove(data[k],NewData[k],sizeof(T)*min(OldCol[k],vec[k]));
  Delete();
  data=NewData;
  columns=vec;
  return columns;
 }

template <class T>
void DataArray<T>::AddColumn (int ColLen, T* ColVar )
 {
  int N=columns.SetDim()+1;
  DataVector<int, CopyStructFast<int> > NewCol=columns;
  NewCol.Resize(N);NewCol[N-1]=ColLen;
  Resize(NewCol);
  if (ColVar!=NULL) MemoryMove(ColVar,data[N-1],sizeof(T)*ColLen);
 }

template <class T>
template <class CpM_>
DataVector<T, CpM_ > DataArray<T>::SetColumn (int Col, int ColLen , T* ColVar )
 {
  DataVector<T, CpM_ > ret(columns[Col]);
  if (ColLen==-1) return ret=data[Col];
  int N=columns.SetDim(),k;
  DataVector<int, CopyStructFast<int> > NewDim=columns;
  if (N<=Col)  { NewDim.Resize(Col+1);for (k=N;k<Col;k++) NewDim[k]=ColLen;}
  NewDim[Col]=ColLen;Resize(NewDim);
  if (ColVar!=NULL) MemoryMove(ColVar,data[Col],sizeof(T)*ColLen);
  ret.SetDim(columns[Col]);ret=data[Col];
  return ret;
 }

template <class T>
template<class CpM_>
DataVector<T, CpM_> DataArray<T>::SetRow (int Row, T* RowVar )
 {
  int k,N=columns.SetDim();
  DataVector<T, CpM_ > ret(N);ret=(T)0;
  CheckProperties();if (!same_length) {ExEnv::ferr()<<"SetRow, Arr not same_length. Use SetRowMask. col:"<<columns;return ret;}
  if (RowVar==NULL) if ( (N!=0) && (Row>=N) ) {ExEnv::ferr()<<"SetRow, Arr rows<RowToGet:"<<N<<" "<<Row<<"\n";return ret;}
                    else {for (k=0;k<N;k++) ret[k]=data[k][Row];return ret;}
  else if (Row>N) {DataVector<int, CopyStructFast<int> > tmp(N);Resize(tmp=Row);for (k=N;k<Row;k++) SetRow<CpM_>(k,RowVar);return ret=RowVar;}
       else {for (k=0;k<N;k++) data[k][Row]=RowVar[k];}
  return ret=RowVar;
 };
template <class T>
template <class CpM_>
void DataArray<T>::SetRow (int Row,DataVector<T, CpM_>& RowVar )
 {
  int k,N=columns.SetDim();
  CheckProperties();if (!same_length) {ExEnv::ferr()<<"SetRow, Arr not same_length. Use SetRowMask. col:"<<columns;return ret;}
  if (Row>N) {Resize(DataVector<double, CopyStructFast<double> > tmp(N)=Row);for (k=N;k<Row;k++) SetRow(k,RowVar);}
  else {for (k=0;k<N;k++) data[k][Row]=RowVar[k];}
  return ret=RowVar;
 };
template <class T>
void DataArray<T>::CheckProperties() 
 {
  if (checked) return;
  int k,N=columns.SetDim(),L0;
  if (N<1) {square=1;same_length=1;return;}
  L0=columns[0];same_length=1;square=0;
  for (k=1;k<N;k++) if (columns[k]!=L0) {same_length=0;break;}
  if ((same_length) && (N==L0)) square=1;
  return;
 };
template <class T>
void DataArray<T>::Delete ()
 {
  checked=0;
  for (int k=0;k<columns.SetDim();k++) {delete data[k];}//data[k]=NULL;}
  data.SetDim(0);columns.SetDim(0);
 };

template <class T>
DataArray<T> Transpon (DataArray<T>& ar)
 {
  ar.CheckProperties();
  DataVector<int, CopyStructFast<int> > ColVec=ar.SetDim();
  if (!ar.same_length) {ExEnv::ferr()<<"Error in transpon; Array not same_length. Its dim: \n"<<ColVec;return ar;}
  DataArray<T> ret(0);
  int col,row,Ncol=ColVec.SetDim();if (Ncol==0) return ret;
  int Nrow=ColVec[0];ret.SetDim(Nrow,Ncol);
  for (col=0;col<Ncol;col++)
  { for (row=0;row<Nrow;row++) ret(col,row)=ar(row,col);}
  return ret;
 };

template <class T>
DataArray<T> DataArray<T>::operator * (const T& coef)
 {
  int col,row,N=columns.SetDim();
  DataArray<T> ret=(*this);
  for (col=0;col<N;col++)
    { for (row=0;row<columns[col];row++) ret(row,col)=data[col][row]*coef;}
  return ret;
 }
template <class T>
DataArray<T> DataArray<T>::operator / (const T& coef)
 {
  int col,row,N=columns.SetDim();
  DataArray<T> ret=(*this);
  for (col=0;col<N;col++)
    { for (row=0;row<columns[col];row++) ret(row,col)=data[col][row]/coef;}
  return ret;
 }
template <class T>
DataArray<T> DataArray<T>::operator + (const T& coef)
 {
  int col,row,N=columns.SetDim();
  DataArray<T> ret=(*this);
  for (col=0;col<N;col++)
    { for (row=0;row<columns[col];row++) ret(row,col)=data[col][row]+coef;}
  return ret;
 }
template <class T>
DataArray<T> DataArray<T>::operator - (const T& coef)
 {
  int col,row,N=columns.SetDim();
  DataArray<T> ret=(*this);
  for (col=0;col<N;col++)
    { for (row=0;row<columns[col];row++) ret(row,col)=data[col][row]-coef;}
  return ret;
 }

template <class T>
template <class CpM_>
DataVector<T, CpM_> DataArray<T>::operator * (const DataVector<T, CpM_>& coef)
 {
  CheckProperties();
  if (!same_length) {ExEnv::ferr()<<"Error in Ar mul Vec; Array not same_len. Its dim: \n"<<columns;return coef;}
  DataVector<T> ret(0);
  int col,row,Ncol=columns.SetDim();if (Ncol==0) return ret;
  int Nrow=columns[0];ret.SetDim(Nrow);ret=(T)0;
  if (coef.SetDim()!=Ncol) {ExEnv::ferr()<<"Error in Ar mul Vec; Vec_len!=Ncol."<<coef.SetDim()<<" "<<Ncol;return ret;}
  for (row=0;row<Nrow;row++)
    {
     T sum=0;
     for (col=0;col<Ncol;col++) sum=sum+data[col][row]*coef[col];
     ret[row]=sum;
    }
  return ret;
 }
template <class T>
template <class CpM_>
DataArray<T> DataArray<T>::operator + (const DataVector<T, CpM_>& coef)
 {
  CheckProperties();
  if (!square) { ExEnv::ferr()<<"Error in Ar plus Vec; Array not square. Its dim: \n"<<columns;return (*this);}
  DataArray<T> ret;ret=0;
  int col,Ncol=columns.SetDim();
  if (coef.SetDim()!=Ncol) {ExEnv::ferr()<<"Error in Ar plus Vec; Vec_len!=Ncol."<<coef.SetDim()<<" "<<Ncol;return ret;}
  for (col=0;col<Ncol;col++) data[col][col]=data[col][col]+coef[col];
  return ret;
 }
template <class T>
template <class CpM_>
DataArray<T> DataArray<T>::operator - (const DataVector<T, CpM_>& coef)
 {
  CheckProperties();
  if (!square) {ExEnv::ferr()<<"Error in Ar minus Vec; Array not square. Its dim: \n"<<columns;return (*this);}
  DataArray<T> ret=coef;
  int col,Ncol=columns.SetDim();
  if (coef.SetDim()!=Ncol) {ExEnv::ferr()<<"Error in Ar minus Vec; Vec_len!=Ncol."<<coef.SetDim()<<" "<<Ncol;return ret;}
  for (col=0;col<Ncol;col++) data[col][col]=data[col][col]-coef[col];
  return ret;
 }

template <class T>
DataArray<T> DataArray<T>::operator * ( DataArray<T>& coef)
 {
  CheckProperties();
  if (!same_length) {ExEnv::ferr()<<"Error in Ar1 mul Ar2; Ar1 not same_len. Its dim: \n"<<columns;return coef;}
  coef.CheckProperties();
  if (!coef.same_length) {ExEnv::ferr()<<"Error in Ar1 mul Ar2; Ar2 not same_len. Its dim: \n"<<coef.SetDim();return coef;}
  DataArray<T> ret(0);
  int col,row,k,N1col=columns.SetDim();if (N1col==0) return ret;int N1row=columns[0];
  int N2col=coef.SetDim().SetDim();if (N2col==0) return ret;int N2row=coef.SetDim()[0];
  if (N2row!=N1col) {ExEnv::ferr()<<"Error in Ar mul Ar; Ar1_col!=Ar2_row."<<N1col<<" "<<N2row;return ret;}
  ret.SetDim(N2col,N1row);
  for (row=0;row<N1row;row++)
    {
     for (col=0;col<N2col;col++)
       {
        T sum=0;
        for (k=0;k<N1col;k++) sum=sum+data[k][row]*coef(k,col);
        ret(row,col)=sum;
       }
    }
  return ret;
 }
template <class T>
DataArray<T> DataArray<T>::operator + (DataArray<T>& coef)
 {
  if (columns!=coef.SetDim()) {ExEnv::ferr()<<"Error in Ar plus Ar; Array with dif dim. Ar dim, coef dim: \n"<<columns<<"\n"<<coef.SetDim()<<"\n";return coef;}
  DataArray<T> ret(columns);
  int col,row,Ncol=columns.SetDim();
  for (col=0;col<Ncol;col++) {for (row=0;row<Ncol;row++) ret(row,col)=data[col][row]+coef(row,col);}
  return ret;
 };

template <class T>
DataArray<T> DataArray<T>::operator - (DataArray<T>& coef)
 {
  if (columns!=coef.SetDim()) {ExEnv::ferr()<<"Error in Ar minus Ar; Array with dif dim. Ar dim, coef dim: \n"<<columns<<"\n"<<coef.SetDim()<<"\n";return coef;}
  DataArray<T> ret(columns);
  int col,row,Ncol=columns.SetDim();
  for (col=0;col<Ncol;col++) {for (row=0;row<Ncol;row++) ret(row,col)=data[col][row]-coef(row,col);}
  return ret;
 }










#endif
