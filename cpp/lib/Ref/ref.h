//
// ref.h --- definitions of the reference counting classes
//

//   This is the main include file for the reference counting classes.
// This includes two other files: reftmpl.h and refmacr.h.  The
// former is a template declaration for the reference counted classes
// and the latter is generated from the former by a perl script and
// provides CPP macros that declare reference counting classes.
//
//   The behaviour of the package can be modified with the following five
// macros, each of which should be undefined, 0, or 1:
//
// REF_CHECK_STACK:  If this is 1 referenced objects are checked to see if they
// reside on the stack, in which case storage for the object is not managed,
// if management is enabled.  This feature can be confused by multiple threads
// and memory checking libraries.
//
// REF_MANAGE:  If this is 1 the manage and unmanage members are enabled.
//
// REF_CHECK_MAX_NREF:  If this is 1 the reference count is checked before
// it is incremented to make sure it isn't too big.
//
// REF_CHECK_MIN_NREF:  If this is 1 the reference count is checked before
// it is decremented to make sure it isn't already zero.
//
// REF_USE_LOCKS:  If this is 1 then critical regions are locked before they
// are entered.  This prevents erroneous behavior when multiple threads
// share reference counted objects.  This will slow down certain operations,
// so it should be set to 0 if your application does not need to be thread
// safe.
//
// If a macro is undefined, then the behaviour is architecture
// dependent--usually, the macro will be set to 1 in this case.
// For maximum efficiency and for normal operation after the program is
// debugged, compile with all of the above macros defined to zero.
// This can also be done with -DREF_OPTIMIZE.
//
//   An include file can be used to set these options as well.  This has
// the advantage that dependency checking will force an automatic
// recompile of all affected files if the options change.  The file
// <scconfig.h> will be include if -DHAVE_CONFIG_H is specified.
//
//   Note that all source code that uses references must be compiled with
// the same value REF_MANAGE.  Changing this can change the storage layout
// and the interpretation of the reference count data.


#ifndef _util_ref_ref_h
#define _util_ref_ref_h

#include "lib/std/util.h"
//#include <iostream.h>
//#include <stdlib.h>
//#include <limits.h>

//#include "identity.h"


typedef unsigned long refcount_t;

/** The base class for all reference counted objects.  If multiple
    inheritance is used, RefCount must be virtually inherited from,
    otherwise references to invalid memory will likely result.

    Reference counting information is usually maintained by smart
    pointer classes Ref, however this mechanism can be
    supplemented or replaced by directly using the public
    interface to RefCount.

    The unmanage() member is only needed for special cases where memory
    management must be turned off.  For example, if a reference counted
    object is created on the stack, memory management mechanisms based on
    reference counting must be prohibited from deleting it.  The unmanage()
    member accomplishes this, but a better solution would be to allocate
    the object on the heap with new and let a smart pointer manage the
    memory for the object.

    When using a debugger to look at reference counted objects the count is
    maintained in the _reference_count_ member.  However, this member is
    encoded so that memory overwrites can be sometimes detected.  Thus,
    interpretation of _reference_count_ is not always straightforward.

*/
//#define REF_CHECK_MAX_NREF
#define REF_CHECK_MIN_NREF
struct SavableClass;

class RefCount {   //: public Identity {
private:
#define REF_MAX_NREF (UINT_MAX - 1)
#define REF_MANAGED_CODE UINT_MAX
    unsigned int _reference_count_;
    int ManageRef;

    void error(const char *) const;
    void too_many_refs() const;
    void not_enough_refs() const;

protected:
    RefCount() : _reference_count_(0), ManageRef(1) {}
    RefCount(const RefCount &) : _reference_count_(0), ManageRef(1) {}
    /// Assigment should not overwrite the reference count.
    RefCount &operator=(const RefCount &) {
        return *this;
    }

public:
    virtual ~RefCount();


    /// Return the reference count.
    refcount_t nreference() const {
        return _reference_count_;
    }

    /// Increment the reference count and return the new count.
    refcount_t reference() {
#if REF_CHECK_MAX_NREF
        if(_reference_count_ >= REF_MAX_NREF)
            too_many_refs();
#endif
        _reference_count_++;
        refcount_t r = _reference_count_;
        return r;
    }

    /// Decrement the reference count and return the new count.
    refcount_t dereference() {
#ifdef REF_CHECK_MIN_NREF
        if(_reference_count_ == 0)
            not_enough_refs();
#endif
        _reference_count_--;
        refcount_t r = _reference_count_;
        return r;
    }

    int managed() const {
        return ManageRef;
    }
    /** Turn off the reference counting mechanism for this object.
        The value returned by nreference() will always be
        1 after this is called.  The ability to unmanage()
        objects must be turned on at compile time by defining
        REF_MANAGE.  There is a slight performance penalty. */
    void unmanage() {
        ManageRef = 0;
    }
};

/** Provides a few utility routines common to all
    Ref template instantiations.
*/

struct SavableClass;
class RefBase {
protected:
    /// Print a warning message.
    void warn(const char *msg) const;
    /// Called when stack data is referenced.
    //    void warn_ref_to_stack() const;
    /// Called when the deletion of stack data is skipped.
    //    void warn_skip_stack_delete() const;
    /// Called when the reference count is corrupted.
    void warn_bad_ref_count() const;
    /// Print information about the reference.
    void ref_info(RefCount *p, ostream &os) const;
    void check_pointer() const;
    void reference(RefCount *);
    int dereference(RefCount *);
    int managed(RefCount *);
    void unmanage(RefCount *);

public:
    void ref_info(ostream &os) const;
    virtual ~RefBase();
    /// Returns the DescribedClass pointer for the contained object.
    virtual RefCount *parentpointer() const = 0;
    /** Requires that a nonnull reference is held.  If not,
        the program will abort. */
    void require_nonnull() const;
    // Added to be able to store refernces in the stream.
    virtual SavableClass *GetSavableBase(RefCount *set = NULL);
};

/** A template class that maintains references counts.

    Several of these operations can cause a reference to an object to be
    replaced by a reference to a different object.  If a reference to a
    nonnull object is eliminated, the object's reference count is
    decremented and the object is deleted if the reference count becomes
    zero.

    There also may be a to convert to T*, where T is the type of the object
    which Ref references.  Some compilers have bugs that prevent the use of
    operator T*().  The pointer() member should be used instead.
 
*/
/// Less than for two Identity pointers.
inline int lt(const void *i, const void *j) {
    return i < j;
}
/// Greater than for two void pointers.
inline int gt(const void *i, const void *j) {
    return i > j;
}
/// Less than or equal for two void pointers.
inline int le(const void *i, const void *j) {
    return i <= j;
}
/// Greater than or equal for two void pointers.
inline int ge(const void *i, const void *j) {
    return i >= j;
}
/// Equal for two void pointers.
inline int eq(const void *i, const void *j) {
    return i == j;
}
/// Not equal for two void pointers.
inline int ne(const void *i, const void *j) {
    return i != j;
}
/** Compare for two void pointers.  Returns -1, 0, or 1, like
    the C library function strcmp. */
inline int cmp(const void *i, const void *j) {
    return (i == j) ? 0 : ((i < j) ? -1 : 1);
}

//struct SavableClass;
//struct FilterIn;
#include "lib/std/stdexception.h"
template <class T>
class Ref : public RefBase {
private:
    T *p;

public:
    /// Create a reference to a null object.
    Ref() : p(0) {}
    /// Create a reference to the object a.
    Ref(T *a) : p(0) {
        if(a) {
            p = a;
            reference(p);
        }
    }
    /// Create a reference to the object referred to by a.
    Ref(const Ref<T> &a) : p(0) {
        //SavableClass *tmp = dynamic_cast<SavableClass*>(a.pointer);
        //cout<<"Creating Ref:"<<(void*)a.pointer()<<" name "<<(tmp!=NULL)?tmp->class_name():""<<"\n";
        if(a.pointer()) {
            p = a.pointer();
            reference(p);
        }
    }
    /** Delete this reference to the object.  Decrement the object's reference
        count and delete the object if the count is zero. */
    ~Ref() {
        clear();
    }
    /** Returns the reference counted object.  The behaviour is undefined if
        the object is null. */
    T *operator->() const {
        return p;
    }
    /// Returns a pointer the reference counted object.
    T *pointer() const {
        return p;
    }
    /// Implements the parentpointer pure virtual in the base class.
    RefCount *parentpointer() const {
        return dynamic_cast<RefCount *>(p);
    }

    operator T *() const {
        return p;
    }
    /** Returns a C++ reference to the reference counted object.
        The behaviour is undefined if the object is null. */
    T &operator*() const {
        return *p;
    };
    /** Return 1 if this is a reference to a null object.  Otherwise
        return 0. */
    int null() const {
        return p == 0;
    }
    /// Return !null().
    int nonnull() const {
        return p != 0;
    }
    /** A variety of ordering and equivalence operators are provided using
        the Identity class. */
    template <class A>
    int operator==(const Ref<A> &a) const {
        return eq(p, a.pointer());
    }
    template <class A>
    int operator>=(const Ref<A> &a) const {
        return ge(p, a.pointer());
    }
    template <class A>
    int operator<=(const Ref<A> &a) const {
        return le(p, a.pointer());
    }
    template <class A>
    int operator>(const Ref<A> &a) const {
        return gt(p, a.pointer());
    }
    template <class A>
    int operator<(const Ref<A> &a) const {
        return lt(p, a.pointer());
    }
    template <class A>
    int operator!=(const Ref<A> &a) const {
        return ne(p, a.pointer());
    }
    /** Compare two objects returning -1, 0, or 1. Similar
        to the C library routine strcmp. */
    int compare(const Ref<T> &a) const {
        return eq(p, a.p) ? NULL : ((lt(p, a.p) ? -1 : 1));
    }
    //
    T *release_temp_settled_ptr() {
        T *ret = p;
        dereference(ret);
        p = NULL;
        return ret;
    }
    /// Refer to the null object.
    void clear() {
        if(p) {
            int ref = dereference(p);
            if((ref == 0) && (managed(p))) {
                //SavableClass* cr = dynamic_cast<SavableClass*>(parentpointer());
                //if (cr) {cout<<"Deleting obj "<<cr->class_name()<<"\n";cout.flush();}

                // Not to generate double deleting of the same object
                T *tmp = p;
                p = NULL;
                delete tmp;
                //            delete p;
            }
            //else { SavableClass *sc=dynamic_cast<SavableClass*>(p);
            // if (sc) ExEnv::err()<<"  Object name "<<sc->class_name()<<" num ref "<<sc->nreference() <<"\n";}
            p = NULL;
        }
    }
    /// Assignment to c.
    Ref<T> &operator=(const Ref<T> &c) {
        T *cp = c.pointer();
        if(cp) {
            cp->reference();
            clear();
            p = cp;
        } else
            clear();
        return *this;
    }
    /// Assignment to the object that a references using dynamic_cast.

    ///*
    Ref<T> &operator<<(const RefBase &a) {
        T *cr = dynamic_cast<T *>(a.parentpointer());
        if(cr) {
            reference(cr);
            clear();
        }
        p = cr;
        return *this;
    }

    //  Assigns to the given base class pointer using dynamic_cast.  If
    //    the dynamic_cast fails and the argument is nonnull and has a
    //    reference count of zero, then it is deleted.
    Ref<T> &operator<<(RefCount *a) {
        T *cr = dynamic_cast<T *>(a);
        if(!cr)
            warn("In Ref<T>& operator<<(RefCount *a):  dynamic_cast<T*>(a)==NULL ");
        assign_pointer(cr);
        //        if (cr) assign_pointer(cr);
        //        else if (a && a->nreference() <= 0) delete a;
        return *this;
    }
    //*/

    /// Assignment to cr.
    Ref<T> &operator=(T *cr) {
        assign_pointer(cr);
        return *this;
    }
    /// Assignment to cr.
    void assign_pointer(T *cr) {
        if(cr)
            cr->reference();
        clear();
        p = cr;
    }
    /// Check the validity of the pointer.
    void check_pointer() const {
        if(p && p->nreference() <= 0) {
            warn_bad_ref_count();
        }
    }
    /// Print information about the reference to os.
    void ref_info(ostream &os) const {
        RefBase::ref_info(p, os);
    }
    /// Print a warning concerning the reference.
    void warn(const char *s) const {
        RefBase::warn(s);
    }

    // Added to be able to store refernces in the stream.
    //virtual SavableClass* GetSavableBase(SavableClass *set=NULL) {
    virtual SavableClass *GetSavableBase(RefCount *set = NULL) {
        T *cr = NULL;
        if(set != NULL) {
            cr = dynamic_cast<T *>(set);
            if(!cr)
                //throw exception("In SavableClass* GetSavableBase(void *set=NULL) :  dynamic_cast<T*>(a)==NULL\n");
                throw info_except(
                    "In SavableClass* GetSavableBase(void *set=NULL) :  dynamic_cast<%s>(%s)==NULL\n",
                    typeid(T).name(),
                    typeid(*set).name());
            assign_pointer(cr);
        }
        return RefBase::GetSavableBase(p);

        //#ifdef _util_class_sav_h
        //        return dynamic_cast<SavableClass*>(p);
        //#else
        //        return NULL;
        //#endif
    };
};

//#include "class.h"
//#include "class_sav.h"


#endif

// ///////////////////////////////////////////////////////////////////////////

// Local Variables:
// mode: c++
// c-file-style: "CLJ"
// End:
