//
// exenv.h
//

#ifndef _util_misc_exenv_h
#define _util_misc_exenv_h

#include "lib/std/util.h"
//#include <iostream.h>

//#define BinaryOutput
//#include "formio.h"
//#include "class_sav.h"

/** The ExEnv class is used to find out about how the program is being run. */
struct FilterOut;
struct FilterTextOut;
struct FilterTextIn;

struct FilterBinOut;
//#ifdef BinaryOutput
//extern FilterBinOut fcout;//("Test",DataSource::Console);
//#else
//extern FilterTextOut fcout;//("Test",DataSource::Console);
//#endif
//extern FilterTextIn fcin;//("Test",DataSource::Console);

class ExEnv {
protected:
    static int initialized_;
    static int *argc_;
    static char ***argv_;
    static char hostname_[256];
    static char username_[9];

    static unsigned long mem_;
    static int nproc_;

    static ostream *out_;
    static FilterOut *fout_;

public:
    /// Set the argument count and vector.
    static void init(int &argcref, char **&argvref);
    /// Return nonzero if ExEnv has been initialized.
    static int initialized() {
        return argc_ != 0;
    }
    /// Return an reference to the argument count.
    static int &argc() {
        return *argc_;
    }
    /// Return an reference to the argument vector.
    static char **&argv() {
        return *argv_;
    }
    /// Return argv[0] with the path removed.
    static const char *program_name();
    /// Return the host name.
    static const char *hostname() {
        return hostname_;
    }
    static const char *username() {
        return username_;
    }

    static void set_out(ostream *o) {
        out_ = o;
    }
    static ostream &out() {
        if(!out_)
            set_out(&cout);
        return *out_;
    }
    static ostream &err() {
        return out();
    }

    static void set_fout(FilterOut *o) {
        fout_ = o;
    }
    //    static FilterOut &fout();// { if (!out_) set_out(&cout_);return *out_; }
    //    static FilterOut &ferr() { return fout(); }
    //    static FilterOut &ferr() { return fout(); }

    /// The amount of memory on this node.
    static unsigned long memory() {
        if(!initialized_)
            err();
        return mem_;
    }
    /// The number of processors on this node.
    static int nproc() {
        if(!initialized_)
            err();
        return nproc_;
    }
};


// ///////////////////////////////////////////////////////////////////////////

ios &indent(ios &);
ios &node0(ios &);


// ///////////////////////////////////////////////////////////////////////////

/** This class allows <tt>printf</tt> like output to put sent
    to an <tt>ostream</tt>. */
class scprintf {
private:
    char str[1024];

public:
    scprintf(const char *, ...);
    friend ostream &operator<<(ostream &, const scprintf &);
};

ostream &operator<<(ostream &, const scprintf &);

#endif
// Local Variables:
// mode: c++
// c-file-style: "CLJ"
// End:
