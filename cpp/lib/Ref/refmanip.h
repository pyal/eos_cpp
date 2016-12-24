#pragma once

#include "data_manip.h"
#include "class_sav.h"


FilterOut &operator<<(FilterOut &out, const Stroka &str);
FilterIn &operator>>(FilterIn &in, Stroka &str);


//namespace RefManip {


template <class T>
FilterIn &operator>>(FilterIn &in, Ref<T> &obj) {
    SavableClass *o;
    if(!in)
        throw info_except("Bad stream\n");
    in >> o;
    if(!o)
        throw info_except("Could not read object\n");
    obj << o;
    if(!(!o) && !obj)
        throw info_except(
            "Object of wrong type<%s> have to be <%s>\n",
            typeid(*o).name(),
            typeid(T).name());
    //if (!in)
    //    throw info_except("Bad result stream\n");
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
    Stroka tmp;
    //in>>tmp;
    TestNextWord(in, "list_{", "Wrong list prefix!!!");
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
    Stroka tmp;
    //in>>tmp;
    TestNextWord(in, "vector_{", "Wrong list prefix!!!");
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
    while(!SavableClass::TestNextChar(in, '}'))
        lst.push_back(SavableClass::TestType<T>(in.getobject()));
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