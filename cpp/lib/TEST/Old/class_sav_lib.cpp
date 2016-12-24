//
// clas_sav_stest.cc
//
// a simple program to test the class stuff


#include "class_sav_lib.h"

long int Coreleft() {
    long int Used = 0;
    _HEAPINFO hinfo;
    int heapstatus;
    hinfo._pentry = NULL;
    while((heapstatus = _heapwalk(&hinfo)) == _HEAPOK)
        if(hinfo._useflag == _USEDENTRY)
            Used += hinfo._size;
    if((heapstatus != _HEAPEND) && (heapstatus != _HEAPEMPTY))
        return -1;
    //   cout<<" stack "<<_stackavail()<<"\n";
    return Used;
};


//void TempTemp_D_cd(){delete create<A>();delete create<D>();};
//template DescribedClass* create<D>();
//template DescribedClass* create<A>();
//static ClassDesc D_cd(typeid(D),"D",1,"Category",create<D>);
//static ClassDesc A_cd(typeid(A),"A",1,"Category",create<A>);
DescribedClass *create_A_cd() {
    return new A;
}
DescribedClass *create_D_cd() {
    return new D;
}
static ClassDesc D_cd(typeid(D), "D", 1, "Category", create_D_cd);
static ClassDesc A_cd(typeid(A), "A", 1, "Category", create_A_cd);
static ClassDesc B_cd(typeid(B), "B", 1);
static ClassDesc C_cd(typeid(C), "C");

void MemTest() {
    cout << " Begin MemTest\n";
    cout << "Before  dynamic_cast" << Coreleft() << endl;
    DescribedClass *dc = ClassDesc::name_to_class_desc("D")->create();
    cout << dc->class_name() << flush;
    C *dtst = dynamic_cast<C *>(dc);
    cout << "dynamic_cast D ";
    dtst->Test();
    delete dtst;
    cout << "After dynamic_cast " << Coreleft() << endl;

    cout << "Before new D " << Coreleft() << endl;
    D *dt = new D;
    C *ct = dt;
    B *bt = dt;

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
    cout << "After new D " << Coreleft() << endl;
    FilterTextOut fo(DataSource::OpenSource("Test", DataSource::Out, DataSource::Memory));
    //  char tmp[256];
    cout << "entering ...";
    char *t;
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
    delete t;
    //  cout<<" Deleted !!!\n"<<flush;
    cout << " ClosingCategories\n" << flush;
    DataSource::CloseCategories();
    cout << " Emd MemTest\n";
};
void RefTst() {
    cout << " Begin RefTst\n";
    ClassDesc::list_all_classes();

    cout << node0 << indent << "using 0" << endl;
    const Ref<DescribedClass> descl2(0);
    Ref<A> aaa;
    cout << "getting aaaa" << endl;
    A *aaaa = 0;   //aaa.pointer();
    cout << "using aaaa" << endl;
    const Ref<DescribedClass> descl((aaaa == (A *)0) ? (DescribedClass *)0 : aaaa);
    cout << "using aaa.pointer()" << endl;
    const Ref<DescribedClass> descl3(
        (aaa.pointer() == (A *)0) ? (DescribedClass *)0 : aaa.pointer());

    A a;
    cout << "A name:" << a.class_name() << '\n';
    cout << " Begin RefTst\n";
};

void LibTest() {
    cout << " Begin LibTest\n";
    Ref<A> tmpref, aref = dynamic_cast<A *>(ClassDesc::name_to_class_desc("A")->create());
    cout << "dynamic_cast A\n ";
    FilterTextOut fo("Test", DataSource::Memory);
    fo << aref;
    FilterTextIn fi("Test", DataSource::Memory);
    //  tmpref=dynamic_cast<A*>(fi.getobject());
    tmpref << fi.getobject();

    FilterTextOut fcout("Test1", DataSource::Console);
    tmpref->i = 1000;
    fcout << "TestObjectPut_HaveToBe_A"
          //    <<setprecision(2)                 // Does not work. Wau................
          << 1.333333333333 << 2222.333;
    fcout << (SavableClass *)tmpref << (SavableClass *)aref;
    fcout.flush();
    //  fo<<" A ";
    //cout<<" ClosingCategories\n"<<flush;
    DataSource::CloseCategories();
    cout << " End LibTest\n" << flush;
};

int class_sav_lib(int, const char *) {
    // check the compiler's handling of virtual inheritance
    //TempTemp_D_cd();
    cout << "Before RefTst " << Coreleft() << endl;
    RefTst();
    cout << "After RefTst " << Coreleft() << endl;

    //  cout<<"Before MemTest "<<Coreleft()<<endl;while (1)
    MemTest();
    cout << "After MemTest " << Coreleft() << endl;
    LibTest();
    cout << "After LibTest " << Coreleft() << endl;
    Ref<D> d = new D;
    cout << "D name:" << d->class_name() << '\n';
    d->id = 50;
    d->A::i = 10;
    d->ib = 20;
    d->C::i = 20;
    FilterTextOut fcout("Console", DataSource::Console);
    fcout << (SavableClass *)d << "\n";
    FilterTextOut fo("Test1", DataSource::Memory);
    fo << d;
    //  delete d;
    FilterTextIn fi("Test1", DataSource::Memory);
    //  SavableClass *sc;//=new D;//sc->unmanage();
    Ref<C> sc;   //=new D;//sc->unmanage();
                 //  char name[256]="D";
                 //  ClassDesc* cd=ClassDesc::name_to_class_desc(name);
                 //  if (cd) sc=dynamic_cast<SavableClass*>(cd->create());
                 //  fi>>(SavableClass*)(sc.pointer());
    sc = dynamic_cast<C *>(fi.getobject());
    //  Input(sc);
    fcout << (SavableClass *)sc;
    fcout.flush();
    D u;
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
/*
 //cout<<&d<<"\n";
  cout << "&d = " << (void*) &d << '\n';
  cout << "dynamic_cast<D*>(&d) = " << (void*) dynamic_cast<D*>(&d) << '\n';
  cout << "dynamic_cast<B*>(&d) = " << (void*) dynamic_cast<B*>(&d) << '\n';
  cout << "dynamic_cast<A*>(&d) = " << (void*) dynamic_cast<A*>(&d) << '\n';
  cout << "dynamic_cast<C*>(&d) = " << (void*) dynamic_cast<C*>(&d) << '\n';
  cout << "dynamic_cast<DescribedClass*>(&d) = "
       << (void*) dynamic_cast<DescribedClass*>(&d) << '\n';
  dynamic_cast<C*>(&d)->Test();
//  Ref<D> dref(new D);
//  Ref<A> aref(dref);
//  Ref<D> dref;
//  Ref<A> aref(new D);
  Ref<D> dref;//=dynamic_cast<D*>(sc);
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
*/
/////////////////////////////////////////////////////////////////////////////

// Local Variables:
// mode: c++
// c-file-style: "CLJ"
// End:
