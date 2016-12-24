#ifndef __DAT_REG_H
#define __DAT_REG_H


//  Definitions for registering of various functions

#define  Data_Register

#include "dat_fac.h"

#ifdef Data_Register
template <class T> const char * const TTable <T>::name="TTable";
template <class T> const char * const TColumn<T>::name="TColumn";
template <class T> const char * const TDataF<T>::name="TDataF";

template <class T> int DataRegister(istream &in,TData<T> *&Func);
template <class T> int DataStore(ostream &out,TData<T> *Func);
template <class T> int DataRegister(char *name,TData<T> *&Func);
#endif




#ifdef Data_Register

template <class T> int DataRegister(const char *name,TData<T> *&Func)
  {
//#ifdef Data_Register
   if (Stricmp(name,"TTable")==0) Func=(TData<T>*)(new TTable<T>);
   else
     if (Stricmp(name,"TColumn")==0) Func=(TData<T>*)(new TColumn<T>);
   else
     if (Stricmp(name,"TDataF")==0) Func=(TData<T>*)(new TDataF<T>);
//   else
//#endif
   else
    { cout<<" DataRegister; There is no such function \n"<<name<<"\n";return 0; }
//cout<<" DataRegister name "<<name<<" Func_name "<<Func->InterfaceIOName()<<"\n";
   return 1;
  };

template <class T> int DataRegister(istream &in,TData<T> *&Func)
  {
   char name[256];in>>name;
   if (DataRegister(name,Func))  return Func->ReadIni(in);
   else return 0;
  };

template <class T> int DataStore(ostream &out,TData<T> *Func)
  {
   out<<Func->InterfaceIOName()<<"  ";
   Func->SaveIni(out);
   return 1;
  };

template <class T> int DataRegister(char* name,TData<T> *&Func)
{
    return DataRegister(name,Func);
};


#endif  // Data_Register

  
#endif
