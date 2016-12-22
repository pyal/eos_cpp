#pragma once

#include <vector>
#include <list>
#include <math.h>
#include <algorithm>
#include <assert.h>
#include <map>
#include "lib/std/const.h"
#include "lib/ref/stroka.h"
#include "lib/std/util.h"


namespace DataManip {

    template<class T>
    std::ostream &operator<<(std::ostream &out, const std::list<T> &lst){
        for(typename std::list<T>::const_iterator it = lst.begin();it!=lst.end();it++)
            out<<(*it)<<" ";
        return out;
    }
    template<class T>
    std::ostream &operator<<(std::ostream &out, const std::vector<T> &vec){
        for(size_t k=0;k<vec.size();k++)
            out<<vec[k]<<" ";
        return out;
    }
    template<class T>
    std::ostream &operator<<(std::ostream &out, const std::vector<std::vector<T> > &vec){
        if (vec.size() == 0)
            return out;
        //for(size_t k = 0; k < vec.size(); k++) 
        //    if (vec[k].size() != vec[0].size())
        //        throw info_except("Working with same size vectors and col0(%0) col%i(%i)\n", vec[0].size(), k, vec[k].size());
        //for(size_t k1 = 0; k1 < vec[0].size(); k1++) {
        //    for(size_t k = 0; k < vec.size(); k++) 
        //        out << vec[k][k1] << " ";
        //    out << "\n";
        //}
        for(size_t k = 0; k < vec.size(); k++) {
            out << "Vector " << k << "\n";
            out << vec[k] << "\n";
        }
        return out;
    }
    template<class T>
    std::vector<T>& CopyVector(std::vector<T> &dst, const std::vector<T> &src){
        size_t s = src.size();
        dst.resize(s);
        if (s==0)
            return dst;
        memmove(&dst[0], &src[0], sizeof(dst[0])*s);
        return dst;
    }
    template<class T>
    std::vector<std::vector<T> >& CopyVector(std::vector<std::vector<T> > &dst, const std::vector<std::vector<T> > &src){
        size_t s = src.size();
        dst.resize(s);
        for(size_t i = 0; i < src.size(); i++)
            CopyVector(dst[i], src[i]);
        return dst;
    }
    template<class T>
    std::vector<T>& CopyVector(std::vector<T> &dst, const std::vector<T> &src, size_t iSrcBeg, size_t iSrcEnd){
        size_t s = iSrcEnd-iSrcBeg;
        assert(iSrcBeg>=0 && iSrcEnd>0 && iSrcEnd<=src.size());
        dst.resize(s);
        if (s==0)
            return dst;
        memmove(&dst[0], &src[iSrcBeg], sizeof(dst[0])*s);
        return dst;
    }
    template<class T>
    std::vector<T>& CopyVector(std::vector<T> &dst, size_t iDstBeg, const std::vector<T> &src, size_t iSrcBeg, size_t numpnt){
        assert(iDstBeg>=0 && iSrcBeg>0 && iDstBeg + numpnt <= dst.size() && iSrcBeg + numpnt <= src.size());
        if (numpnt == 0)
            return dst;
        memmove(&dst[iDstBeg], &src[iSrcBeg], sizeof(dst[0])*numpnt);
        return dst;
    }

    template<class T>
    std::vector<T>& SetVector(std::vector<T> &dst, const T &val){
        for(size_t i = 0; i < dst.size(); i++)
            dst[i] = val;
        return dst;
    }

    template<class T>
    std::vector<T> JoinArray(const std::vector< std::vector<T> > &arr) {
        std::vector<T> ret;
        for(size_t i = 0; i < arr.size(); i++) {
            for(size_t j = 0; j < arr[i].size(); j++)
                ret.push_back(arr[i][j]);
        }
        return ret;
    }

    template<class T>
    std::vector<T> operator+(const std::vector<T> &vec, const T &x){
        std::vector<T> ret(vec.size());
        for(size_t i = 0;i<vec.size();i++)
            ret[i] = vec[i] + x;
        return ret;
    };
    template<class T>
    std::vector<T> operator+(const std::vector<T> &vec, const std::vector<T> &vec1){
        assert(vec1.size() == vec.size());
        std::vector<T> ret(vec.size());
        for(size_t i = 0;i<vec.size();i++)
            ret[i] = vec[i] + vec1[i];
        return ret;
    };
    std::vector<double> MakeStepVector(int Num,double low,double up,int logscale, int centered);




    extern void *StaticSortArray;
    template<class T>
    bool greaterWeight( int w1, int w2 ){
        return ((T*)StaticSortArray)[w1] > ((T*)StaticSortArray)[w2];
    }
    template<class T>
    bool lessWeight( int w1, int w2 ){
        return ((T*)StaticSortArray)[w1] < ((T*)StaticSortArray)[w2];
    };
    template<class T>
    std::vector<int> Sort(const std::vector<T> &vec, int AscendingOrder){
        std::vector<int> ind(vec.size());
        for(size_t i = 0;i<ind.size();i++)
            ind[i] = (int)i;
        StaticSortArray = (void*)&vec[0 ];
        if (AscendingOrder)
            std::sort(ind.begin(), ind.end(), lessWeight<T>);
        else
            std::sort(ind.begin(), ind.end(), greaterWeight<T>);
        return ind;
    };


    template<class T, class T1>
    std::vector<T>& CopyVector(std::vector<T> &dst, const std::vector<T1> &src){
        size_t s = src.size();
        dst.resize(s);
        if (s==0)
            return;
        for(size_t i = 0;i<s;i++)
            dst[i] = src[i];
        return dst;
    }
    template<class T, class T1>
    std::vector<T>& CopyVector(std::vector<T> &dst, T1* src){
        size_t len = dst.size();
        for(size_t i = 0;i<len;i++)
            dst[i] = (T)src[i];
        return dst;
    }
     template<class T, class T1>
    T* CopyVector(T* dst, std::vector<T1> &src){
        for(size_t i = 0;i<src.size();i++)
            dst[i] = (T)src[i];
        return dst;
    }
   template<class T, class T1>
    std::vector<T>& SetVector(std::vector<T> &dst, const T1 &val) {
        for(size_t i =0; i < dst.size(); i++)
            dst[i] = (T)val;
        return dst;
    }
   template<class T>
    void SetVector(std::vector<T> &dst, T *src, int num) {
        dst.resize(num);
        MemoryMove(src, &dst[0], sizeof(T) * num);
    }
   template<class T>
    void SetData(T *dst, const std::vector<T> &src) {
        MemoryMove(&src[0], dst, sizeof(T) * src.size());
    }
    template<class T, class T1>
    std::vector<T> operator+(const std::vector<T> &vec, const T1 &x) {
        std::vector<T> ret(vec.size());
        for(size_t i = 0;i<vec.size();i++)
            ret[i] = vec[i] + (T)x;
        return ret;
    };
    template<class T, class T1>
    std::vector<T> operator*(const std::vector<T> &vec, const T1 &x){
        std::vector<T> ret(vec.size());
        for(size_t i = 0;i<vec.size();i++)
            ret[i] = vec[i] * (T)x;
        return ret;
    };
    template<class T1, class T2>
    int MapsSame(const std::map<T1, T2> &m1, const std::map<T1, T2> &m2){
        map<T1, T2>::const_iterator it1 = m1.begin(), it2 = m2.begin();
        while(it1 != m1.end() && it2 != m2.end()){
            if (it1->first != it2->first || it1->second != it2->second)
                return 0;
            it1++;
            it2++;
        }
        if (it1 != m1.end() || it2 != m2.end())
            return 0;
        return 1;
    };

    template<class T1, class T2>
    vector<T1> Map2Vector(const std::map<T1, T2> &m){
		vector<T1> ret(m.size());
		int i = 0;
		for(map<T1, T2>::const_iterator it = m.begin(); it != m.end(); it++, i++)
			ret[i] = it->first;
        return ret;
    };

    template<class T, class T1>
    Stroka Map2Str(const std::map<T, T1> &m, const char *fstSec = " ", const char *elemSep = "\n") {
        typedef typename std::map<T, T1>::const_iterator  cIter;
        const int bufSize = 100000;
        char buf[bufSize+1];
        ostrstream out(buf, bufSize);
        //TExtraInfoOutput out(new TMemoryOutput(buf, BufSize));
        for (cIter it = m.begin(); it != m.end(); it++)
            out << it->first << fstSec << it->second << elemSep;
        return Stroka(buf, 0, out.pcount());
    }
    template<class T, class T1>
    ostream &operator<<(ostream &out, const pair<T, T1> &p) {
        out << "( " << p.first << ", " << p.second << " )";
        return out;
    }

    template<class T>
    void XYTable2Planar(std::vector<std::vector<T> > &planar, const std::vector<std::vector<T> > &table) {
        assert(table.size() >= 2);
        int dim = table[0].size() * table[1].size();
        for(size_t i = 2; i < table.size(); i++)
            assert(dim == table[2].size());
        planar.resize(2);
        planar[0].resize(dim);
        planar[1].resize(dim);
        int curN = 0;
        for(size_t i = 0; i < table[0].size(); i++) {
            for(size_t j = 0; j < table[1].size(); j++) {
                planar[0][curN] = table[0][i];
                planar[1][curN] = table[1][j];
                curN++;
            }
        }
        for(size_t i = 2; i < table.size(); i++)
            planar.push_back(table[i]);
    }
    template<class T>
    void XYPlanar2Table(std::vector<std::vector<T> > &table, const std::vector<std::vector<T> > &planar) {
        assert(planar.size() >= 2 && planar[0].size() > 1);
        for(size_t i = 1; i < planar.size(); i++) 
            assert(planar[i].size() == planar[0].size());
        table.clear();
        table.resize(2);
        double lastVal = planar[0][0], f = planar[0][0] * (1 - 10 * M_Eps), t = planar[0][0] * (1 + 10 * M_Eps);
        for(size_t i = 0; i < planar[0].size(); i++) {
            if (planar[0][i] != lastVal)
            //if (!In_Lim(planar[0][i], f, t, 1))
                break;
            table[1].push_back(planar[1][i]);
//cout << i << " " << lastVal << " " << planar[0][i] << "\n";
        }
        //cout << table[1].size() << " " << lastVal << " " << planar[0][table[1].size()] << "\n";cout.flush();
        int dimY = table[1].size();
        for(size_t i = 0; i < planar[0].size(); i += dimY) 
            table[0].push_back(planar[0][i]);
        assert(planar[0].size() == dimY * table[0].size());
        for(size_t i = 2; i < planar.size(); i++)
            table.push_back(planar[i]);
    }

    template<class T>
    Stroka SaveTextVector(const std::vector<T> &vec) {
        Stroka ret = Stroka(" Size ") + Stroka::Int2Str((int)vec.size()) + " data ";
        for(size_t k=0;k<vec.size();k++)
            ret += Stroka(vec[k]) + " ";
        return ret;
    }

    template<class T>
    std::ostream &SaveTextVector(std::ostream &out, const std::vector<T> &vec){
        out << " Size " << vec.size() << " data ";
        for(size_t k=0;k<vec.size();k++)
            out<<vec[k]<<" ";
        return out;
    }
    template<class T>
    std::istream &ReadTextVector(std::istream &in, std::vector<T> &vec){
        Stroka tmp;
        int s;
        in >> tmp >> s >> tmp;
        vec.resize(s);
        for(size_t k=0;k<vec.size();k++)
            in >> vec[k];
        return in;
    }
    template<class T>
    void SetLimits(const std::vector<std::vector<T> > &dat, std::vector<T> &minV, std::vector<T> &maxV) {
        maxV.resize(dat.size());
        minV.resize(dat.size());
        for(size_t i = 0; i < dat.size(); i++) {
            assert(dat[i].size() > 0);
            minV[i] = maxV[i] = dat[i][0];
            for(size_t i1 = 1; i1 < dat[i].size(); i1++) {
                minV[i] = min(dat[i][i1], minV[i]);
                maxV[i] = max(dat[i][i1], maxV[i]);
            }
        }
    }


}; //namespace DataManip