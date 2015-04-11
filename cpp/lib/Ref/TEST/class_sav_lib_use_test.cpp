//
// clas_sav_stest.cc
//
// a simple program to test the class stuff

#include "exenv.h"
#include "class_sav_lib.h"

#include <stdiostr.h>




//static ClassDesc C_cd(typeid(C),"C");
class E: public D  {
  private:
  public:
    int ie;
    E():ie(5) {};

    ~E() {  cout << "D dtor\n";  };
	virtual void Test() { cout<<" Test E\n";};
    int save_data_state(FilterOut&so)
		{D::save_data_state(so);  so<<KeyDesc("E_ie")<<ie; return 1;};
    int read_data_state(FilterIn&si)
		{D::read_data_state(si); si>>StdKey>>ie;return 1;};
};
//template DescribedClass* create<D>();
//template<> DescribedClass* create<A>();
DescribedClass *create_E_cd(){ return new E;}
static ClassDesc E_cd(typeid(E),"E",1,"Category",create_E_cd);
//static ClassDesc A_cd(typeid(A),"A",1,"Category",create<A>);

