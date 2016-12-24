
#include <lib/precompiled/eno_simple.h>

#include "lib/data/DataVec.h"
//#include "IndBase.h"
#include "lib/data/DataArr.h"
#include "lib/data/DataTyp.h"
//#include "exenv.h"

//FilterTextOut fcout("Test",DataSource::Console);
//FilterTextIn fcin("Test",DataSource::Console);

void TestVector() {
    SpacePoint vec1(10), vec2, tmp;
    //	DataVector<double> vec1(10),vec2,tmp;
    vec1 = 5;
    //	(SavableClass&)vec2=(SavableClass&)vec1;
    vec1[5] = 55;
    vec2 = vec1 = 10;
    fcout << vec2 << "\n" << vec1 << "\n";
    fcout << "vec1 " << vec1 << "\n vec2 " << vec2 << "\n";
    tmp = vec1 + vec2;
    //	DataVector<double> tmp=vec1+vec2;
    fcout << tmp << "\n";
    fcout << "sum " << vec1 + vec2 << " \n min " << vec2 - vec1 << " \n mul "
          << vec1 * vec2 << "\n";
    {
        FilterTextOut fo("Test1", DataSource::Memory);
        vec2 = 100;
        fo << vec2;
        FilterTextIn fi("Test1", DataSource::Memory);
        fi >> vec1;
    }
    fcout << "vec1" << vec1 << "\n vec2" << vec2 << "\n";
    DataVector<SpacePoint> dat(10);
    dat[0].SetDim(10);
    dat[0] = 5;
    for(int k = 1; k < 9; k++)
        dat[k] = dat[0];
    fcout << dat << "\n" << (void *)&vec2 << "\n";
    void *tmpv = NULL;
    FilterTextOut fo("Test1", DataSource::Disk);
    fo << (void *)&vec1 << 10;
    //fo<<10;
    fo.CloseBuf();
    FilterTextIn fi("Test1", DataSource::Disk);
    int tmp_int;   //fi>>tmp_int;fcout<<tmp_int;
    //FilterTextIn::operator >>(fi, tmpv);
    fi.get(tmpv);
    fi >> tmp_int;
    //fi>>tmpv>>tmp_int;
    fcout << tmpv << tmp_int;
}

void TestArray() {
    DataArray<double> ar(5, 3), ar2;
    ar = 1;
    DataVector<double> vec(5);
    vec = 1;
    ar2 = ar;   //=vec;
    ExEnv::ferr() << Transpon<double>(ar) << Transpon<double>(ar2);
    ar2 = ar = vec;
    ar2 = ar + 1;
    ExEnv::ferr() << Transpon(ar) << Transpon(ar2);
    ar = ar + ar2;
    ExEnv::ferr() << Transpon(ar) << Transpon(ar2);
    ar = ar - ar2;
    ExEnv::ferr() << Transpon(ar) << Transpon(ar2);
    // ExEnv::ferr()<<vec<<"\n";
    ExEnv::ferr() << ar2 * ar << "\n" << ar2 * vec << "\n" << ar2 * ar;
    ar = ar + 10;
    ar.AddColumn(vec);
    ExEnv::ferr() << "Added\n"
                  << Transpon(ar) << ar.SetColumn<CopyStructFast<double>>(2) << "\n"
                  << ar.SetRow<CopyStructFast<double>>(2) << "\n";
    // fcout<<Transpon(ar);
    // for (int k=0;k<100000;k++) ar=Transpon(ar2);
    // ExEnv::ferr()<<Transpon(ar)<<Transpon(ar2);
}


void main() {
    //fcout<<" Test";fcout.flush();
    //  TestVector();
    //  IndRegBase reg;
    TestArray();
};
