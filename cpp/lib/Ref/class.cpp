#include <lib/precompiled/Ref.h>


//
// class.cc
//


#include <stdlib.h>
#include <string.h>

#include "class.h"

//using namespace std;

std::map<ClassKey, ClassDescP> *ClassDesc::all_ = 0;
std::map<type_info_key, ClassDescP> *ClassDesc::type_info_all_ = 0;

/////////////////////////////////////////////////////////////////


ClassKey &ClassKey::operator=(const ClassKey &key) {
    delete[] classname_;
    if(key.classname_) {
        classname_ = ::strcpy(new char[strlen(key.classname_) + 1], key.classname_);
    } else {
        classname_ = 0;
    }
    return *this;
}

int ClassKey::cmp(const ClassKey &ck) const {
    if(!classname_) {
        if(!ck.classname_)
            return 0;
        return -1;
    }
    if(!ck.classname_)
        return 1;
    return strcmp(classname_, ck.classname_);
}


////////////////////////////////////////////////////////////////////////

int type_info_key::operator==(const type_info_key &t) const {
    if(!ti_ && !t.ti_)
        return 1;
    if(!ti_ || !t.ti_)
        return 0;

    return *ti_ == *t.ti_;
}

int type_info_key::operator<(const type_info_key &t) const {
    if(!ti_ && !t.ti_)
        return 0;
    if(!ti_)
        return 0;
    if(!t.ti_)
        return 1;

    return ti_->before(*t.ti_);
}

int type_info_key::cmp(const type_info_key &t) const {
    if(*this == t)
        return 0;
    if(*this < t)
        return -1;
    return 1;
}

////////////////////////////////////////////////////////////////////////

ClassDesc::ClassDesc(
    const type_info &ti,
    const char *name,
    int version,
    const char *category,
    DescribedClass *(*ctor)()
    //                    DescribedClass* (*keyvalctor)(const Ref<KeyVal>&),
    //                     DescribedClass* (*stateinctor)(FilterIn&)
    ) {
    if(!type_info_all_)
        type_info_all_ = new std::map<type_info_key, ClassDescP>;
    type_info_key key(&ti);
    if(type_info_all_->find(key) != type_info_all_->end()) {
        ExEnv::err() << node0 << indent
                     << "ERROR: duplicate ClassDesc detected for class " << name
                     << " type_info name = " << ti.name() << endl;
        abort();
    } else
        (*type_info_all_)[key] = this;
    // test the version number to see if it is valid
    if(version <= 0) {
        ExEnv::err() << "ERROR: ClassDesc ctor: version <= 0" << endl;
        exit(1);
    }
    //  init(name,version,parents,ctor,keyvalctor,stateinctor);
    init(name, version, category, ctor);
}


void ClassDesc::init(
    const char *name,
    int version,
    const char *category,
    DescribedClass *(*ctor)())
//                DescribedClass* (*keyvalctor)(const Ref<KeyVal>&),
//                DescribedClass* (*stateinctor)(FilterIn&))
{
    classname_ = 0;
    version_ = version;
    //  children_ = 0;
    ctor_ = ctor;
    //  keyvalctor_ = keyvalctor;
    //  stateinctor_ = stateinctor;

    // make sure that the static members have been initialized
    if(!all_)
        all_ = new std::map<ClassKey, ClassDescP>;

    // see if I'm already in the list
    ClassDesc *me = name_to_class_desc(name);
    int temp_copy_present = 0;
    if(me && me->version() != 0) {
        ExEnv::err() << node0 << indent
                     << "ERROR: ClassDesc ctor: ClassDesc already initialized for "
                     << name << endl;
        abort();
    } else if(me)
        temp_copy_present = 1;

    classname_ = strcpy(new char[strlen(name) + 1], name);
    if(category)
        category_ = strcpy(new char[strlen(category) + 1], category);
    else {
        category_ = new char[1];
        category_[0] = 0;
    }

    ClassKey key(name);

    if(all_->find(key) != all_->end()) {
        ExEnv::err() << node0 << indent << "ERROR: ClassDesc ctor: already created "
                     << name << endl;
        abort();
    }

    (*all_)[key] = this;
}

ClassDesc::~ClassDesc() {
    // delete this ClassDesc from the list of all ClassDesc's
    ClassKey key(classname_);
    all_->erase(key);
    // if the list of all ClassDesc's is empty, delete it
    if(all_->size() == 0) {
        delete all_;
        all_ = 0;
    }
    // delete local data
    delete[] classname_;
    // !!! Not deleting all_type_info !!!! ( no information to do, may be add type_info var ? )
}

ClassDesc *ClassDesc::class_desc(const type_info &ti) {
    if(!type_info_all_)
        throw info_except(" Trying to get %s before init stor!!!\n", ti.name());
    if(type_info_all_->find(type_info_key(&ti)) == type_info_all_->end())
        return 0;
    return (*type_info_all_)[type_info_key(&ti)];
}

std::map<ClassKey, ClassDescP> &ClassDesc::all() {
    if(!all_) {
        ExEnv::err() << "ClassDesc::all(): all not initialized" << endl;
        abort();
    }
    return *all_;
}
ClassDesc *ClassDesc::name_to_class_desc(const char *name) {
    ClassKey key(name);
    //  if (all_->find(key) == all_->end()) return 0;
    //  return (*all_)[key];
    std::map<ClassKey, ClassDescP>::iterator itr = all_->find(key);
    if(itr == all_->end()) {
        //      throw info_except("ClassName:%s - not found",name);
        return 0;
    }
    return itr->second;
}

DescribedClass *ClassDesc::create() const {
    if(ctor_)
        return (*ctor_)();
    return 0;
}


//DescribedClass* ClassDesc::create(FilterIn&statein) const
//{
//  if (stateinctor_) return (*stateinctor_)(statein);
//  return 0;
//}

//void ClassDesc::list_all_classes()
//{
//  ExEnv::out() << "Listing all classes:" << endl;
//  for (std::map<ClassKey,ClassDescP>::iterator ind=all_->begin();
//       ind!=all_->end(); ind++) {
//      ClassDesc* classdesc = (*ind).second;
//      ExEnv::out() << "class " << classdesc->name() << endl;
//    }
//}
//void ClassDesc::list_all_classes(const char *categ)
//{
//	ExEnv::out() << "Listing all classes:" << endl;
//	if (categ!=NULL) ExEnv::out()<<"for category <"<<categ<<">"<<endl;
//  for (std::map<ClassKey,ClassDescP>::iterator ind=all_->begin();
//       ind!=all_->end(); ind++) {
//      ClassDesc* classdesc = (*ind).second;
//	  if ( (categ==NULL) || (Stricmp(categ,classdesc->category())==0) )
//      ExEnv::out() << "class " << classdesc->name() << endl;
//    }
//}

Ref<DescribedClass> ClassDesc::list_category_obj_start(const char *categ) {
    ClassDesc *desc = list_category_start(categ);
    if(desc)
        return Ref<DescribedClass>(desc->create());
    return NULL;
}
Ref<DescribedClass> ClassDesc::list_category_obj_next(
    DescribedClass *prev,
    const char *categ,
    int AutoDel) {
    ClassDesc *was = prev->class_desc();
    ClassDesc *desc = list_category_next(was, categ);
    if(desc)
        return Ref<DescribedClass>(desc->create());
    return NULL;
}
//DescribedClass *ClassDesc::list_category_desc_start(const char *categ){
//  for (std::map<ClassKey,ClassDescP>::iterator ind=ClassDesc::all_->begin();
//       ind!=ClassDesc::all_->end(); ind++)
//	{
//      ClassDesc* classdesc = ind->second;
//      if ((categ==NULL) || (Stricmp(classdesc->category(),categ)==0))
//		  return classdesc->create();
//
//
//    }
//  return NULL;
//
//}
ClassDesc *ClassDesc::list_category_start(const char *categ) {
    for(std::map<ClassKey, ClassDescP>::iterator ind = ClassDesc::all_->begin();
        ind != ClassDesc::all_->end();
        ind++) {
        ClassDesc *classdesc = ind->second;
        if((categ == NULL) || (Stricmp(classdesc->category(), categ) == 0))
            return classdesc;
    }
    return NULL;
}
ClassDesc *ClassDesc::list_category_next(ClassDesc *prev, const char *categ) {
    int found = 0;
    for(std::map<ClassKey, ClassDescP>::iterator ind = ClassDesc::all_->begin();
        ind != ClassDesc::all_->end();
        ind++) {
        ClassDesc *classdesc = ind->second;
        if((!found) && (classdesc != prev))
            continue;
        if(!found) {
            found = 1;
            continue;
        }
        if((categ == NULL) || (Stricmp(classdesc->category(), categ) == 0))
            return classdesc;
    }
    return NULL;
}


////////////////////////////////////////////////////

static ClassDesc DescribedClass_cd(typeid(DescribedClass), "DescribedClass");


//void DescribedClass::print(ostream &o) const
//{ o << indent << "Object of type " << class_name() << endl;}

ostream &operator<<(ostream &o, const RefBase &ref) {
    DescribedClass *dc = dynamic_cast<DescribedClass *>(ref.parentpointer());
    if(dc)
        o << indent << "Object of type " << dc->class_name() << endl;
    else
        o << indent << "reference to null" << endl;
    return o;
}


/////////////////////////////////////////////////////////////////////////////


/*
int
ClassKey::hash() const
{
  int r=0;
  size_t i;

  // Even numbered bytes make up the lower part of the hash index
  for (i=0; i < ::strlen(classname_); i+=2) {
      r ^= classname_[i];
    }

  // Odd numbered bytes make up the upper part of the hash index
  for (i=1; i < ::strlen(classname_); i+=2) {
      r ^= classname_[i]<<8;
    }

  return r;
}


*/
