#include <lib\precompiled\Ref.h>

//
// ref.cc --- implementation of the reference counting classes
//

#include "ref.h"
#include "exenv.h"

//using namespace std;

/*
int
RefCount::lock_ptr() const
{
#if REF_USE_LOCKS
    if (ref_lock_ == 0xff)
        return 1;
    return __LOCK(sRefLocks[ref_lock_]);
#else
    return 1;
#endif    
}

int
RefCount::unlock_ptr() const
{
#if REF_USE_LOCKS
    if (ref_lock_ == 0xff)
        return 1;
    return __UNLOCK(sRefLocks[ref_lock_]);
#else
    return 1;
#endif    
}

void
RefCount::use_locks(bool inVal)
{
#if REF_USE_LOCKS
    if (inVal) {
        ref_lock_ = sRefLock;
        unsigned char tmp_sRefLock = sRefLock+1;
        if (tmp_sRefLock >= NLOCKS)
            tmp_sRefLock = 0;
        sRefLock = tmp_sRefLock;
    }
    else
        ref_lock_ = 0xff;
#endif
}

*/

void
RefCount::error(const char * w) const
{
  ExEnv::err() << "RefCount: ERROR: " << w << endl;

}

void
RefCount::too_many_refs() const
{
  error("Too many refs.");abort();
}

void
RefCount::not_enough_refs() const
{
  error("Ref count dropped below zero.");abort();
}
#include "class_sav.h"
RefCount::~RefCount()
{
//#if REF_MANAGE
  if (managed() && nreference()) {
      error("Deleting a referenced object.");
//SavableClass* cr = dynamic_cast<SavableClass*>(this);
//if (cr) {cout<<"Deleting obj "<<cr->class_name();}
//cout<<" ~RefCount ref "<<this;
//cout<<" NumRef "<<(int)nreference();
//cout<<"\n";cout.flush();
    }
//#endif
}

///////////////////////////////////////////////////////////////////////

void
RefBase::warn ( const char * msg) const
{
  ExEnv::err() << "WARNING: " << msg << endl;
}
/*
void
RefBase::warn_ref_to_stack() const
{
  warn("Ref: creating a reference to stack data");
}

void
RefBase::warn_skip_stack_delete() const
{
  warn("Ref: skipping delete of object on the stack");
}
*/
void
RefBase::warn_bad_ref_count() const
{
  warn("Ref: bad reference count in referenced object\n");
}
void
RefBase::ref_info(RefCount*p, ostream& os) const
{
  if (p)
      os << "nreference() = " << p->nreference() << endl;
  else
      os << "reference is null" << endl;
}

void
RefBase::require_nonnull() const
{
  if (parentpointer() == 0) {
      ExEnv::err() << "RefBase: needed a nonnull pointer but got null"
           << endl;
      abort();
    }
}

RefBase::~RefBase()
{
}

void
RefBase::check_pointer() const
{
  if (parentpointer() && parentpointer()->nreference() <= 0) {
      warn_bad_ref_count();
    }
}

void
RefBase::ref_info(ostream& os) const
{
  RefBase::ref_info(parentpointer(),os);
}

void
RefBase::reference(RefCount *p)
{
  if (p) {
//#if REF_CHECK_STACK
//      if (DO_REF_CHECK_STACK(p)) {
//          DO_REF_UNMANAGE(p);
//          warn_ref_to_stack();
//        }
//#endif
      p->reference();
    }
}

int
RefBase::dereference(RefCount *p)
{
  if (p) 
      return p->dereference();
  else
      return -1;
}

int 
RefBase::managed(RefCount *p)
{
  if (p) 
      return p->managed();
  else
      return -1;
}
void 
RefBase::unmanage(RefCount *p){ if (p) p->unmanage();}


//#include "lib\std\stdexception.h"
//SavableClass* Ref::GetSavableBase(RefCount *set=NULL) {
//    T* cr = NULL;
//	if (set!=NULL) {
//		cr = dynamic_cast<T*>(set);
//      	if (!cr) 
//            throw info_except("In SavableClass* GetSavableBase(void *set=NULL) :  dynamic_cast<T*>(a)==NULL\n");
//        assign_pointer(cr);
//	}
//#ifdef _util_class_sav_h
//    return dynamic_cast<SavableClass*>(p);
//#else
//    return NULL;
//#endif
//};


/////////////////////////////////////////////////////////////////////////////

// Local Variables:
// mode: c++
// c-file-style: "CLJ"
// End:
