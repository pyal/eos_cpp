#pragma once

#include "data_manip.h"
#include "class_sav.h"

struct TVerify {
    Stroka Wrd;
    Stroka Message;
    TVerify(const char *wrd, const char *message): Wrd(wrd), Message(message){};
};
#define Verify(str) TVerify(str, info_mark)

FilterIn &operator>>(FilterIn &in, Stroka &str);
FilterIn &operator>>(FilterIn &in, const TVerify &wrd);
istream &operator>>(istream &in,   const TVerify &wrd);

//namespace RefManip {


template <class T>
FilterIn &operator>>(FilterIn &in, Ref<T> &obj) {
    SavableClass *o;
    verify(in, "Reading object from Bad stream");
    in >> o;
    verify(o, "Could not read object");
    obj << o;
    verify(o && obj, "Object of wrong type<" + typeid(*o).name() + "> have to be <" + typeid(T).name() + ">");
    return in;
}

template <class T>
FilterOut &operator<<(FilterOut &out, const std::list<T> &lst) {
    out << " list_{ ";
    SavableClass::IncIdent();
    out << SavableClass::EOLN();
    for(typename std::list<T>::const_iterator it = lst.begin(); it != lst.end(); it++)
        out << (*it) << " ";
    SavableClass::DecIdent();
    out << SavableClass::EOLN() << " } ";
    return out;
}
template <class T>
FilterOut &operator<<(FilterOut &out, const std::vector<T> &vec) {
    out << " vector_{ ";
    SavableClass::IncIdent();
    out << SavableClass::EOLN();
    for(size_t i = 0; i < vec.size(); i++)
        out << vec[i] << " ";
    SavableClass::DecIdent();
    out << SavableClass::EOLN() << " } ";
    return out;
}
template <class T>
FilterIn &operator>>(FilterIn &in, std::list<T> &lst) {
    in >> Verify("list_{");
    lst.clear();
    T dat;
    while(!SavableClass::TestNextChar(in, '}')) {
        in >> dat;
        lst.push_back(dat);
    }
    return in;
}
template <class T>
FilterIn &operator>>(FilterIn &in, std::vector<T> &vec) {
    in >> Verify("vector_{");
    vec.clear();
    T dat;
    while(!SavableClass::TestNextChar(in, '}')) {
        in >> dat;
        vec.push_back(dat);
    }
    return in;
}

template <class T>
FilterOut &operator<<(FilterOut &out, const std::list<Ref<T>> &lst) {
    out << " list_{ ";
    SavableClass::IncIdent();
    out << SavableClass::EOLN();
    for(typename std::list<Ref<T>>::const_iterator it = lst.begin(); it != lst.end();
        it++) {
        out.setobject(*it);
        out << " ";
    }
    SavableClass::DecIdent();
    out << SavableClass::EOLN() << " } ";
    return out;
}
template <class T>
FilterOut &operator<<(FilterOut &out, const std::vector<Ref<T>> &vec) {
    out << " vector_{ ";
    SavableClass::IncIdent();
    out << SavableClass::EOLN();
    for(size_t i = 0; i < vec.size(); i++) {
        out.setobject(vec[i]);
        out << " ";
    }
    SavableClass::DecIdent();
    out << SavableClass::EOLN() << " } ";
    return out;
}
template <class T>
FilterIn &operator>>(FilterIn &in, std::list<Ref<T>> &lst) {
    Stroka tmp;
    TestNextWord(in, "list_{", "Wrong list prefix!!!");
    //in>>tmp;

    lst.clear();
    while(!SavableClass::TestNextChar(in, '}')) {
        SavableClass *o = in.getobject();
        log_debug(string("Read: ") + ~SavableClass::object2string(o));
        lst.push_back(SavableClass::TestType<T>(o));
    }
    return in;
}
template <class T>
FilterIn &operator>>(FilterIn &in, std::vector<Ref<T>> &vec) {
    Stroka tmp;
    //in>>StdKey;
    //in>>tmp;
    TestNextWord(in, "vector_{", "Wrong list prefix!!!");
    vec.clear();
    while(!SavableClass::TestNextChar(in, '}'))
        vec.push_back(SavableClass::TestType<T>(in.getobject()));
    return in;
}


//}; //namespace RefManip