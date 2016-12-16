//
// clas_sav_stest.cc
//
// a simple program to test the class stuff

//#include <iostream.h>
//
//#include "exenv.h"
//#include "class.h"
//#include "class_sav.h"
//#include <iomanip.h>
//
//#include <stdiostr.h>
//
//#include <malloc.h>
//long int Coreleft()
// {
//   long int Used=0;
//   _HEAPINFO hinfo;
//   int heapstatus;   hinfo._pentry = NULL;
//   while( ( heapstatus = _heapwalk( &hinfo ) ) == _HEAPOK )
//     if (hinfo._useflag == _USEDENTRY) Used+=hinfo._size;
//   if ( (heapstatus!=_HEAPEND) && (heapstatus!=_HEAPEMPTY) ) return -1;
////   cout<<" stack "<<_stackavail()<<"\n";
//   return Used;
// };

#include "lib/std/Util.h"
#include "lib/Ref/class.h"
#include "lib/Ref/class_sav.h"
#include "unit_test.h"

namespace class_sav_test {
    class AAA : virtual public SavableClass {
    private:
    public:
        int i;

        AAA() : i(1) { };

        ~AAA() { cout << "AAA dtor\n"; };

        virtual void Test() { cout << " Test AAA\n"; };

        int save_data_state(FilterOut &so) {
          so << KeyDesc("A_i")
          << i;
          return 1;
        };

        int read_data_state(FilterIn &si) {
          si >> StdKey >> i;
          return 1;
        };

    };


    class BBB : public AAA {
    private:
    public:
        int ib;

        BBB() : ib(2) { };

        ~BBB() { cout << "BBB dtor\n"; };

        virtual void Test() { cout << " Test BBB\n"; };

        int save_data_state(FilterOut &so) {
          AAA::save_data_state(so);
          so << KeyDesc("B_ib") << ib;
          return 1;
        };

        int read_data_state(FilterIn &si) {
          AAA::read_data_state(si);
          si >> StdKey >> ib;
          return 1;
        };
    };

    static ClassDesc BBB_cd(typeid(BBB), "BBB", 1, "Category", create<BBB>);

    class CCC : virtual public SavableClass {
    private:
    public:
        int i;

        CCC() : i(3) { };

        ~CCC() { cout << "CCC dtor\n"; };

        virtual void Test() { cout << " Test CCC\n"; };

        int save_data_state(FilterOut &so) {
          so << KeyDesc("C_i") << i;
          return 1;
        };

        int read_data_state(FilterIn &si) {
          si >> StdKey >> i;
          return 1;
        };
    };

    static ClassDesc CCCC_cd(typeid(CCC), "CCC");

    class DDD : public BBB, virtual public CCC {
    private:
        AAA *atst;
        double *d;
    public:
        int id;

        DDD() : atst(new AAA) {
          d = new double[5000];
          id = 4;
        };

        ~DDD() {
          delete atst;
          cout << "D dtor\n";
          delete d;
        };

        virtual void Test() { cout << " Test DDD\n"; };

        int save_data_state(FilterOut &so) {
          BBB::save_data_state(so);
          CCC::save_data_state(so);
          so << KeyDesc("D_id") << id;
          return 1;
        };

        int read_data_state(FilterIn &si) {
          BBB::read_data_state(si);
          CCC::read_data_state(si);
          si >> StdKey >> id;
          return 1;
        };
    };

//template DescribedClass* create<D>();
//template<> DescribedClass* create<A>();
    void TempTemp_D_cd() {
      create<AAA>();
      create<DDD>();
    };
    static ClassDesc D_cd(typeid(DDD), "DDD", 1, "Category", create<DDD>);
    static ClassDesc A_cd(typeid(AAA), "AAA", 1, "Category", create<AAA>);

    void MemTest() {
      cout << "============================  MemTest ==================\n";
      cout << "Before  dynamic_cast" << Coreleft() << endl;

      CCC *dtst = dynamic_cast<CCC *>(ClassDesc::name_to_class_desc("DDD")->create());
      cout << "dynamic_cast D ";
      dtst->Test();
      delete dtst;
      cout << "After dynamic_cast " << Coreleft() << endl;

      cout << "Before new D " << Coreleft() << endl;
      DDD *dt = new DDD;
      CCC *ct = dt;
      BBB *bt = dt;

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
      dt->unmanage();
      delete dt;
      cout << "After new D " << Coreleft() << endl;
      FilterTextOut fo(DataSource::OpenSource("Test", DataSource::Out,
                                              DataSource::Memory));
//  char tmp[256];
      cout << "entering ...";
      string t;
      for (int o = 1; o <= 1; o++) {
//    t=dat.GetBuf("Test");
        cout << " O " << o << "\n" << flush;
//     fo.fill('*');
        fo.precision(o);
        for (int k = 1; k <= 3; k++) {
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
//  fo<<t;
//  delete t;
//  cout<<" Deleted !!!\n"<<flush;
      cout << " ClosingCategories\n" << flush;
      DataSource::CloseCategories();
    };

    void RefTst() {
      cout << "=========RefTst ========================\n";
      SavableClass::list_all_classes(0);
      cout << node0 << indent << "using 0" << endl;
      const Ref<DescribedClass> descl2(0);
      Ref<AAA> aaa;
      cout << "getting aaaa" << endl;
      AAA *aaaa = 0; //aaa.pointer();
      cout << "using aaaa" << endl;
      const Ref<DescribedClass> descl(
              (aaaa == (AAA *) 0) ? (DescribedClass *) 0 : aaaa);
      cout << "using aaa.pointer()" << endl;
//      const Ref<DescribedClass> descl3(
//              (aaa.pointer() == (AAA *) 0) ? (DescribedClass *) 0
//                                         : aaa.pointer());

      AAA a;
      cout << "A name:" << a.class_name() << '\n';
    };

    void LibTest(int storage_type) {
      Ref<AAA> tmpref, aref = dynamic_cast<AAA *>(ClassDesc::name_to_class_desc(
              "BBB")->create());
      cout << " ============================ LibTest " << storage_type << " =======================\n";
      cout << "dynamic_cast A\n " << aref;
      {
        FilterTextOut fcout("Console", DataSource::Console);
        fcout.SetRefMask(storage_type);
        fcout << "FO Object:\n" << aref << "\n";
        fcout.flush();
      }
      FilterTextOut fo("Test", DataSource::Memory);fo.SetRefMask(storage_type);
      fo << aref;
      FilterTextIn fi("Test", DataSource::Memory);fi.SetRefMask(storage_type);
      tmpref = dynamic_cast<AAA *>(fi.getobject());
//      cout << " Reading object\n";
//      SavableClass *obj = fi.getobject();
//      cout << " Reading object done\n";
//      cout << "COUT Object: " << tmpref << "\n";
//      tmpref = dynamic_cast<AAA *>(obj);
      tmpref->i = 1000;
//      fo << "FO Cast: " << tmpref << "\n";
//      cout << "COUT Cast: " << tmpref << "\n";
//      fo.flush();
      cout << " ClosingCategories\n" << flush;
      DataSource::CloseCategories();
    };

    void class_sav_test() {
      cout << "=========class_sav_test ========================\n";
// check the compiler's handling of virtual inheritance
      RefTst();
      cout << "Before MemTest " << Coreleft() << endl;
      MemTest();
      cout << "After MemTest " << Coreleft() << endl;
      LibTest(SavableClass::SimpleEdit);
      LibTest(SavableClass::SingleFileStorage);
      Ref<DDD> d = new DDD;
      cout << "D name:" << d->class_name() << '\n';
      d->id = 50;
      d->AAA::i = 10;
      d->ib = 20;
      d->CCC::i = 20;

      FilterTextOut fo("Test", DataSource::Memory);
      fo.SetRefMask(SavableClass::SimpleEdit);
      fo << d;
      FilterTextIn fi("Test", DataSource::Memory);
      fi.SetRefMask(SavableClass::SimpleEdit);
      Ref<CCC> sc;//=new D;//sc->unmanage();
      sc = dynamic_cast<CCC *>(fi.getobject());
//  Input(sc);
      FilterTextOut fcout("Console", DataSource::Console);
      fcout << (SavableClass *) sc;
      fcout.flush();
      DDD u;
      (SavableClass &) u = *sc;
      fcout << &u;
      fcout.flush();
      fcout << &u;
      fcout.flush();
      fcout << &u;
      fcout.flush();
      DataSource::CloseCategories();
//  DataSource::CloseCategories();
//  delete sc;
      cout.flush();
    };
    UnitTest unit_class_sav_test("class_sav_test", class_sav_test);
}
/////////////////////////////////////////////////////////////////////////////

// Local Variables:
// mode: c++
// c-file-style: "CLJ"
// End:
