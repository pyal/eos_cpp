//
// class.h
//
//


#ifndef _DESCRIBED_H
#define _DESCRIBED_H

#pragma warning( disable : 4786 )
#pragma warning( disable : 4800 )

#include "util.h"
//#include <stdio.h>
//#include <string.h>
//#include <stdarg.h>
//#include <iostream.h>
//#include <iomanip.h>
//#include <typeinfo.h>
//#include "ref.h"
//#include "exenv.h"
#include <map>
#include <typeinfo.h>

//#include <iostream>


class DescribedClass;
class ClassDesc;
typedef ClassDesc* ClassDescP;
//typedef const ClassDesc* CClassDescP;

struct ExEnv
{
    static ostream *out_;
    static void set_out(ostream *o) { out_=o; }
    static ostream &out() { if (!out_) set_out(&cout);return *out_; }
    static ostream &err() { return out(); }
};
/// Provides a key into a map of classes.
class ClassKey {
  private:
    char* classname_;
  public:
    ClassKey():  classname_(0){};
    ClassKey(const char* name): classname_(::strcpy(new char[strlen(name)+1],name)){};
    ClassKey(const ClassKey&key)
	{
     if (key.classname_) 
	   classname_ = ::strcpy(new char[strlen(key.classname_)+1],key.classname_);
     else classname_ = 0;
    };
    ~ClassKey(){ delete[] classname_;};
    int operator==(const ClassKey& ck) const { return cmp(ck) == 0;};
    int operator<(const ClassKey& ck) const  { return cmp(ck) < 0;};
    char* name() const{ return classname_;};
    ClassKey& operator=(const ClassKey&);
    int cmp(const ClassKey&ck) const;
//    int hash() const;
  };

//class ClassDesc;

class type_info_key {
  private:
    const type_info *ti_;
  public:
    type_info_key(): ti_(0) {}
    type_info_key(const type_info *ti): ti_(ti) {}
    type_info_key& operator=(const type_info_key&t) { ti_ = t.ti_; return *this;};
    int operator==(const type_info_key&) const;
    int operator<(const type_info_key&) const;
    int cmp(const type_info_key&) const;
};


//    This is used to pass a function that make void constructor calls to the
//    ClassDesc constructor. 
template <class T> DescribedClass* create() { return  new T(); };


// This class is used to contain information about classes.Each DescribedClass type has 
// a static ClassDesc member.  This member has lists of the parents, children and virtual 
// parents for each class.  The ClassDesc class also has a static member that is a list of 
// all described classes in the system.  These lists are constructed as the constructors 
// for the static ClassDesc members for each class are called and are completed before 
// main is entered.  See \ref class for more information.
class ClassDesc { 
  private:
    static std::map<ClassKey, ClassDescP> *all_;
    static std::map<type_info_key, ClassDescP> *type_info_all_;
    char* classname_;
    int version_;
    char *category_;

    DescribedClass* (*ctor_)();
//    DescribedClass* (*stateinctor_)(FilterIn&);

    // do not allow copy constructor or assignment
    ClassDesc(const ClassDesc&);
    void operator=(const ClassDesc&);

    // this is used for temporary parent class descriptors
    ClassDesc(const char* name) { init(name, 0);};
    void init(const char* name,int version=1,const char *category =0,
              DescribedClass* (*ctor)()=0);
  public:

    ClassDesc(const type_info&, const char* name,int version=1,const char* category=0,
              DescribedClass* ((*ctor)())=0);
//              DescribedClass* (*ctor)()=0);
    ~ClassDesc();

    static std::map<ClassKey,ClassDescP>& all();

    /// Writes a list of all of the classes to ExEnv::out().
    static void list_all_classes(const char *categ=NULL);
//    static char* start_list_name();
//    static char* next_list_name();
    // Given the name of the class, return a pointer to the class descriptor. /
    static ClassDesc* name_to_class_desc(const char*);
    // Given a type_info object return a pointer to the ClassDesc. /
    static ClassDesc *class_desc(const type_info &);
    // Returns the name of the class.
    const char* name() const { return classname_; }
    // Returns the category of the class.
    const char* category() const { return category_; }
    int version() const { return version_; }
    // This member has been replaced by create().
    DescribedClass* create_described_class() const{ return create();};
 // Create an instance of DescribedClass with exact type equal to the class to which 
	// this class descriptor belongs.  The constructor which takes no arguments is used.  
	// If this constructor doesn't exist or a static function that calls it with new 
	// wasn't given to this ClassDesc when it was created, then 0 will be returned. 
    virtual DescribedClass* create() const;
    // Create an instance of DescribedClass with exact type equal to the class to which 
	// this class descriptor belongs.  The StateIn& constructor is used. If this 
	// constructor doesn't exist or a static function that calls it with new wasn't 
	// passed to this ClassDesc, then 0 will be returned. 
//    virtual DescribedClass* create(FilterIn&) const;
	// Warning - creating 
	static DescribedClass *list_category_start(const char *categ);
	static DescribedClass *list_category_next(DescribedClass *prev,const char *categ,int AutoDel=1);
 };

// Classes which need runtime information about themselves and their relationship to 
// other classes can virtually inherit from DescribedClass.  This will provide the class 
// with the ability to query its name and its version. Furthermore, the class's static 
// ClassDesc can be obtained which permits several other operations.  See \ref class for
// more information. 
class DescribedClass  {
  public:
	DescribedClass(){};
    DescribedClass(const DescribedClass&){};
    DescribedClass& operator=(const DescribedClass&){ return *this;};
    virtual ~DescribedClass(){};
    // This returns the unique pointer to the ClassDesc corresponding 
	// to the given type_info object. 
    ClassDesc* class_desc() const { return ClassDesc::class_desc(typeid(*this));};
    /// Return the name of the object's exact type.
    const char* class_name() const
    { ClassDesc* cur=class_desc();return cur==NULL ?  
      cout<<"No class descr, in DescribedClass.name\n"<<flush,NULL:cur->name();};
    /// Return the version of the class.
    int class_version() const     
     { ClassDesc* cur=class_desc();return cur==NULL ? 
      cout<<"No class descr, in DescribedClass.name\n"<<flush,NULL:cur->version();};

    const char* class_category() const
    { ClassDesc* cur=class_desc();return cur==NULL ?  
      cout<<"No class descr, in DescribedClass.name\n"<<flush,NULL:cur->category();};
    /// Print the object.
    virtual void print(ostream& out) const;
  };


// Return the ClassDesc corresponding to template argument. 
template <class T> inline ClassDesc *class_desc()
{ 
  return ClassDesc::class_desc(typeid(T)); 
}

// Return the ClassDesc corresponding to the exact type for the argument. 
inline ClassDesc *class_desc(DescribedClass *d) { return ClassDesc::class_desc(typeid(*d));}



// This, together with ForceLink, is used to force code for particular classes 
// to be linked into executables. 
template <class A>
class ForceLinkBase {
  public:
    virtual ~ForceLinkBase() {};
    virtual DescribedClass *create(A) = 0;
};

//This, together with ForceLinkBase, is used to force code for particular
//classes to be linked into executables.  Objects are created from input and
//checkpoint files by using class name lookup to find that class's ClassDesc
//object.  The ClassDesc object has members that can create the class.
//Unfortunately, linking in a library doesn't cause code for the the
//ClassDesc, and thus the class itself, to be linked.  ForceLink objects are
//created in linkage.h files for each library.  The code containing the main
//routine for an executable can include these linkage files to force code for
//that library's classes to be linked. 
template <class T, class A = const Ref<KeyVal> &>
class ForceLink: public ForceLinkBase<A> {
  public:
    DescribedClass *create(A a) { return new T(a); }
};
class ForceLinkBaseVoid {
  public:
    virtual ~ForceLinkBaseVoid() {};
    virtual DescribedClass *create() = 0;
};
template <class T>
class ForceLinkVoid :ForceLinkBaseVoid{
  public:
    DescribedClass *create() { return new T(); }
};


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


/*

// Attempt to cast a DescribedClass pointer to a DescribedClass descendent.  
// It is an error for the result to be a null pointer. 
template<class T> inline T 
require_dynamic_cast(DescribedClass*p,const char * errmsg,...)
{
  T t = dynamic_cast<T>(p);
  if (p && !t) {
      va_list args;
      va_start(args,errmsg);
      fprintf(stderr,"A required dynamic_cast failed in: ");
      vfprintf(stderr,errmsg,args);
      fprintf(stderr,"\nwanted type \"%s\" but got \"%s\"\n",
              typeid(T).name(),typeid(p).name());
      fflush(stderr);
      va_end(args);
      abort();
  }
  return t;
}

// Attempt to cast a const DescribedClass pointer to a DescribedClass descendent.  
// It is an error for the result to be a null pointer. 
template<class T> inline T
require_dynamic_cast(const DescribedClass*p,const char * errmsg,...)
{
  T t = dynamic_cast<T>(p);
  if (p && !t) {
      va_list args;
      va_start(args,errmsg);
      fprintf(stderr,"A required dynamic_cast failed in: ");
      vfprintf(stderr,errmsg,args);
      fprintf(stderr,"\nwanted type \"%s\" but got \"%s\"\n",
              typeid(T).name(),typeid(p).name());
      fflush(stderr);
      va_end(args);
      abort();
  }
  return t;
}
*/

#endif

