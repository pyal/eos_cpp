//
// clas_sav_stest.cc
//
// a simple program to test the class stuff


#include "class_sav_lib.h"

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


//void TempTemp_D_cd(){delete create<A>();delete create<D>();};
//template DescribedClass* create<D>();
//template DescribedClass* create<A>();
//static ClassDesc D_cd(typeid(D),"D",1,"Category",create<D>);
//static ClassDesc A_cd(typeid(A),"A",1,"Category",create<A>);
DescribedClass *create_AA_cd() {
    return new AA;
}
DescribedClass *create_DD_cd() {
    return new DD;
}
static ClassDesc DD_cd(typeid(DD), "DD", 1, "Category", create_DD_cd);
static ClassDesc AA_cd(typeid(AA), "AA", 1, "Category", create_AA_cd);
static ClassDesc BB_cd(typeid(BB), "BB", 1);
static ClassDesc CC_cd(typeid(CC), "CC");

void MemTest() {
    cout << " Begin MemTest\n";
    cout << "Before  dynamic_cast" << Coreleft() << endl;
    DescribedClass *dc = ClassDesc::name_to_class_desc("DD")->create();
    cout << dc->class_name() << flush;
    CC *dtst = dynamic_cast<CC *>(dc);
    cout << "dynamic_cast DD ";
    dtst->Test();
    delete dtst;
    cout << "After dynamic_cast " << Coreleft() << endl;

    cout << "Before new DD " << Coreleft() << endl;
    DD *dt = new DD;
    CC *ct = dt;
    BB *bt = dt;

    cout << "virtual inheritance test:" << endl;
    dt->reference();
    cout << "The following three numbers should be equal:" << endl;
    cout << ' ' << dt->nreference() << ' ' << ct->nreference() << ' ' << bt->nreference()
         << endl;
    ct->reference();
    cout << "The following three numbers should be equal:" << endl;
    cout << ' ' << dt->nreference() << ' ' << ct->nreference() << ' ' << bt->nreference()
         << endl;
    bt->reference();
    cout << "The following three numbers should be equal:" << endl;
    cout << ' ' << dt->nreference() << ' ' << ct->nreference() << ' ' << bt->nreference()
         << endl;
    cout << "done with virtual inheritance test:" << endl;
    dt->unmanage();
    delete dt;
    cout << "After new DD " << Coreleft() << endl;
    FilterTextOut fo(DataSource::OpenSource("Test", DataSource::Out, DataSource::Memory));
    //  char tmp[256];
    cout << "entering ...";
    string t;
    for(int o = 1; o <= 1; o++) {
        //    t=dat.GetBuf("Test");
        cout << " O " << o << "\n" << flush;
        //     fo.fill('*');
        fo.precision(o);
        for(int k = 1; k <= 10; k++) {
            fo << 22 << "HaHa" << 2.333333333333333333333333333333333333333333333333
               << "\n"
               << '|';
            fo << 22 << "HaHa" << 2.355555555555555555555555555555555555555555555555555555
               << "\n"
               << '|';
        }
    }
    cout << "entered\n" << flush;
    t = DataSource::GetStr("Test");
    int i1;
    double d1;
    char s1[256];
    FilterTextIn fi(DataSource::OpenSource("Test", DataSource::In, DataSource::Memory));
    fi >> i1 >> s1 >> d1;
    cout << i1 << " " << s1 << " " << d1 << "\n";
    cout << t << "\n";

    fo.SetNewBuf(DataSource::OpenSource("Test", DataSource::Out, DataSource::Disk));
    //  fo<<t;
    //  cout<<" Deleted !!!\n"<<flush;
    cout << " ClosingCategories\n" << flush;
    DataSource::CloseCategories();
    cout << " Emd MemTest\n";
};
void RefTst() {
    cout << " Begin RefTst\n";
    SavableClass::list_all_classes(0);

    cout << node0 << indent << "using 0" << endl;
    const Ref<DescribedClass> descl2(0);
    Ref<AA> aaa;
    cout << "getting aaaa" << endl;
    AA *aaaa = 0;   //aaa.pointer();
    cout << "using aaaa" << endl;
    const Ref<DescribedClass> descl((aaaa == (AA *)0) ? (DescribedClass *)0 : aaaa);
    cout << "using aaa.pointer()" << endl;
    const Ref<DescribedClass> descl3(
        (aaa.pointer() == (AA *)0) ? (DescribedClass *)0 : aaa.pointer());

    AA a;
    cout << "AA name:" << a.class_name() << '\n';
    cout << " Begin RefTst\n";
};

void LibTest() {
    cout << " Begin LibTest\n";
    Ref<AA> tmpref,
        aref = dynamic_cast<AA *>(ClassDesc::name_to_class_desc("AA")->create());
    cout << "dynamic_cast AA\n ";
    FilterTextOut fo("Test", DataSource::Memory);
    fo << aref;
    FilterTextIn fi("Test", DataSource::Memory);
    tmpref = dynamic_cast<AA *>(fi.getobject());
    FilterTextOut fcout("Test1", DataSource::Console);
    tmpref->i = 1000;
    fcout << "TestObjectPut_HaveToBe_AA"
          //    <<setprecision(2)                 // Does not work. Wau................
          << 1.333333333333 << 2222.333;
    fcout << (SavableClass *)tmpref << (SavableClass *)aref;
    fcout.flush();
    //  fo<<" A ";
    //cout<<" ClosingCategories\n"<<flush;
    DataSource::CloseCategories();
    cout << " End LibTest\n" << flush;
};

void class_sav_lib_test() {
    // check the compiler's handling of virtual inheritance
    //TempTemp_D_cd();
    cout << "Before RefTst " << Coreleft() << endl;
    RefTst();
    cout << "After RefTst " << Coreleft() << endl;
    cout << "Default refmask " << SavableClass::GetRefOutMethod() << "\n";

    //  cout<<"Before MemTest "<<Coreleft()<<endl;while (1)
    MemTest();
    cout << "After MemTest " << Coreleft() << endl;
    cout << "Default refmask " << SavableClass::GetRefOutMethod() << "\n";
    LibTest();
    cout << "After LibTest " << Coreleft() << endl;
    cout << "Default refmask " << SavableClass::GetRefOutMethod() << "\n";
    Ref<DD> d = new DD;
    cout << "DD name:" << d->class_name() << '\n';
    d->id = 50;
    d->AA::i = 10;
    d->ib = 20;
    d->CC::i = 20;
    FilterTextOut fcout("Console", DataSource::Console);
    fcout << (SavableClass *)d << "\n";
    FilterTextOut fo("Test1", DataSource::Memory);
    fo << d;
    //  delete d;
    FilterTextIn fi("Test1", DataSource::Memory);
    //  SavableClass *sc;//=new D;//sc->unmanage();
    Ref<CC> sc;   //=new D;//sc->unmanage();
                  //  char name[256]="D";
                  //  ClassDesc* cd=ClassDesc::name_to_class_desc(name);
                  //  if (cd) sc=dynamic_cast<SavableClass*>(cd->create());
                  //  fi>>(SavableClass*)(sc.pointer());
    sc = dynamic_cast<CC *>(fi.getobject());
    //  Input(sc);
    fcout << (SavableClass *)sc;
    fcout.flush();
    DD u;
    (SavableClass &)u = *sc;
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
UnitTest unit_class_sav_test("class_sav_lib_test", class_sav_lib_test);
