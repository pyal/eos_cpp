
#include "lib/std/Util.h"
#include "lib/Ref/class.h"
#include "lib/Ref/class_sav.h"
#include "unit_test.h"

namespace classtest {
    class A : virtual public DescribedClass {
    private:
        int i;
    public:
        A() : i(1) { };

        int g() { return i; }

        ~A() { cout << "A dtor\n"; };

        virtual void Test() { cout << " Test A\n"; };

    };

    static ClassDesc A_cd(typeid(A), "A");

#ifndef SIMPLE_TEST

    class B : public A {
    private:
        int ib;
    public:
        B() : ib(2) { };

        int g() { return ib; }

        ~B() { cout << "B dtor\n"; };

        virtual void Test() { cout << " Test B\n"; };
    };

    static ClassDesc B_cd(typeid(B), "B", 1);

    class C : virtual public DescribedClass {
    private:
        int i;
    public:
        C() : i(3) { };

        int g() { return i; }

        ~C() { cout << "C dtor\n"; };

        virtual void Test() { cout << " Test C\n"; };
    };

    static ClassDesc C_cd(typeid(C), "C");

    class D : public B, public C {
    private:
        int id;
        A *atst;
        double *d;
    public:
        D() : id(4), atst(new A) { d = new double[5000]; };

        ~D() {
          delete atst;
          cout << "D dtor\n";
          delete d;
        };

        virtual void Test() { cout << " Test D\n"; };

        int g() { return id; }
    };

    void TempTemp() { create<D>(); }
//template DescribedClass* create<D>();

    static ClassDesc D_cd(typeid(D), "D", 1, "", (create<D>));

#endif /* ! SIMPLE_TEST */

    void MemTest() {
      cout << "Before  dynamic_cast" << Coreleft() << endl;

      C *dtst = dynamic_cast<C *>(ClassDesc::name_to_class_desc("D")->create());
      cout << "dynamic_cast D ";
      dtst->Test();
      delete dtst;
      cout << "After dynamic_cast " << Coreleft() << endl;

      cout << "Before new D " << Coreleft() << endl;
      D *dt = new D;
//  C* ct = dt;
//  B* bt = dt;
/*
  cout << "virtual inheritance test:" << endl;
  dt->reference();
  cout << "The following three numbers should be equal:" << endl;
  cout << ' ' << dt->nreference()
       << ' ' << ct->nreference()
       << ' ' << bt->nreference() << endl;
  ct->reference();
  cout << "The following three numbers should be equal:" << endl;
  cout << ' ' << dt->nreference()
       << ' ' << ct->nreference()
       << ' ' << bt->nreference() << endl;
  bt->reference();
  cout << "The following three numbers should be equal:" << endl;
  cout << ' ' << dt->nreference()
       << ' ' << ct->nreference()
       << ' ' << bt->nreference() << endl;
  cout << "done with virtual inheritance test:" << endl;
*/
//  double *t=new double[500];
//  dt->dereference();
//  delete dt;
//  if (dt->nreference() == 0) delete dt;
//  ct->dereference();
//  if (ct->nreference() == 0) delete ct;
//  bt->dereference();
//  if (bt->nreference() == 0) delete bt;
      delete dt;
      cout << "After new D " << Coreleft() << endl;
      FilterTextOut fo(DataSource::OpenSource("Test", DataSource::Out,
                                              DataSource::Memory));
//  char tmp[256];
      cout << "entering ...";
      string t;
      for (int o = 1; o < 10; o++) {
//    t=dat.GetBuf("Test");
        cout << " O " << o << "\n" << flush;
//     fo.fill('*');
        fo.precision(o);
        for (int k = 1; k <= 10; k++) {
          fo << 22 << "HaHa" <<
          2.333333333333333333333333333333333333333333333333 << "\n" << '|';
          fo << 22 << "HaHa" <<
          2.355555555555555555555555555555555555555555555555555555 << "\n" <<
          '|';
        }
      }
      cout << "entered\n" << flush;
      t = DataSource::GetStr("Test");
      int i1;
      double d1;
      char s1[256];
      FilterTextIn fi(DataSource::OpenSource("Test", DataSource::In,
                                             DataSource::Memory));
      fi >> i1 >> s1 >> d1;
      cout << i1 << " " << s1 << " " << d1 << "\n";
      cout << t << "\n";
      fo.SetNewBuf(DataSource::OpenSource("Test", DataSource::Out,
                                          DataSource::Disk));
      fo << t;
//  delete t;
//  cout<<" Deleted !!!\n"<<flush;
      DataSource::CloseCategories();
    };

    void classtest() {
      SavableClass::list_all_classes(0);

      cout << node0 << indent << "using 0" << endl;
      const Ref<DescribedClass> descl2(0);
      Ref<A> aaa;
      cout << "getting aaaa" << endl;
      A *aaaa = 0; //aaa.pointer();
      cout << "using aaaa" << endl;
      const Ref<DescribedClass> descl(
              (aaaa == (A *) 0) ? (DescribedClass *) 0 : aaaa);
      cout << "using aaa.pointer()" << endl;
      const Ref<DescribedClass> descl3(
              (aaa.pointer() == (A *) 0) ? (DescribedClass *) 0
                                         : aaa.pointer());

      A a;
      cout << "A name:" << a.class_name() << '\n';


      // check the compiler's handling of virtual inheritance

      cout << "Before MemTest " << Coreleft() << endl;
//while (1)  
      MemTest();
      cout << "After MemTest " << Coreleft() << endl;
      D d;
      cout << "D name:" << d.class_name() << '\n';

      cout << "&d = " << (void *) &d << '\n';
      cout << "dynamic_cast<D*>(&d) = " << (void *) dynamic_cast<D *>(&d) <<
      '\n';
      cout << "dynamic_cast<B*>(&d) = " << (void *) dynamic_cast<B *>(&d) <<
      '\n';
      cout << "dynamic_cast<A*>(&d) = " << (void *) dynamic_cast<A *>(&d) <<
      '\n';
      cout << "dynamic_cast<C*>(&d) = " << (void *) dynamic_cast<C *>(&d) <<
      '\n';
      cout << "dynamic_cast<DescribedClass*>(&d) = "
      << (void *) dynamic_cast<DescribedClass *>(&d) << '\n';
      dynamic_cast<C *>(&d)->Test();
//  Ref<D> dref(new D);
//  Ref<A> aref(dref);
//  Ref<D> dref;
//  Ref<A> aref(new D);
      Ref<D> dref;
      Ref<A> aref(new A);


      cout << "aref.pointer() is " << aref.pointer() << '\n';
      cout << "dref.pointer() is " << dref.pointer() << '\n';
      cout << "aref == dref gives " << (aref == dref) << '\n';

      dref << aref;

      cout << "aref.pointer() is " << aref.pointer() << '\n';
      cout << "dref.pointer() is " << dref.pointer() << '\n';
      cout << "aref == dref gives " << (aref == dref) << '\n';
//  dref->Test();

    }

    UnitTest unit_classtest("classtest", classtest);
}
/////////////////////////////////////////////////////////////////////////////

// Local Variables:
// mode: c++
// c-file-style: "CLJ"
// End:
