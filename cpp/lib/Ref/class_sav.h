
#ifndef _util_class_sav_h
#define _util_class_sav_h

#include "class.h"

#include <map>
#include <vector>

#include "lib/std/util.h"
#include "allocator_intellect.h"
#include "Stroka.h"


struct FilterIn;
struct FilterOut;

#include "lib/std/logger.h"

struct KeyDesc {
    char name[1256];
    KeyDesc(){};
    KeyDesc(const char *n) {
        Init(n);
    };
    explicit KeyDesc(const Stroka &str) {
        Init(str.c_str());
    };
    void Init(const char *n) {
        strcpy(&name[0], n);
    };
    ~KeyDesc() {}
    KeyDesc &operator=(const char *n) {
        Init(n);
        return *this;
    };
};
static KeyDesc StdKey;
#include <ctype.h>
struct SavableClass : virtual DescribedClass {
protected:
    // Added to store references correctly, to know if the class have been sotred
    int storedid;
    SavableClass() : storedid(0){};
    SavableClass(SavableClass &from) : storedid(0) {
        LoadClass(from);
    };

public:
    //SavableClass& operator=(SavableClass& from);//  { return *this;};//copy to *this before!!!!
    void LoadClass(SavableClass &from, int method = -1);
    SavableClass *Duplicate(int Method = -1);
    virtual ~SavableClass(){};

    // This routine saves the state of the object (which includes the nonvirtual bases),
    // the virtual bases, and type information, will handle null pointers correctly.
    // Restores objects saved with save_state.  The exact type of the next object in si can
    // be any type publically derived from the SavableState. Derived classes implement a
    // similar static function that returns a pointer to the derived class.------------------
    //      static int save_state(SavableClass *sc, FilterOut&so); { return so.setobject(sc);};
    //      static int restore_state(SavableClass *&sc,FilterIn& si) { return si.getobject(sc);};

    // Can be added Control version mecanism here. Read version...Set version...
    // Make class friend of DecsClass ( version is there )
    virtual void save_data_post(FilterOut &so);   //{so<<" }\n "; };
    virtual void save_data_pref(FilterOut &so);   //{so<<" { "; };
    virtual int read_data_post(
        FilterIn &si);   //{char tmp[99];si>>tmp;return (Stricmp("}",tmp)==0);};
    virtual int read_data_pref(
        FilterIn &si);   //{char tmp[99];si>>tmp;return (Stricmp("{",tmp)==0);};

    // Save the base classes (with save_data_state) and the members in the same order that
    // the StateIn CTOR initializes them.  This must be implemented by the derived class if
    // the class has data.
    virtual int save_data_state(FilterOut &so) {
        return 1;
    };
    virtual int read_data_state(FilterIn &si) {
        return 1;
    };

    // Added to store references correctly, to know if the class have been sotred
    inline int SetClassStored(int stored = -1) {
        if(stored != -1)
            storedid = stored;
        return storedid;
    };
    // Used in (SavableClass::operator==) to determine method of storage
    //inline int SetRefMask(int mask=-1) { if (mask!=-1) refmask=mask;return refmask;};
    virtual Stroka MakeHelp() {
        return "";
    }


    enum { StorePtr, SingleFileStorage, UnlinkedObjects, SimpleEdit } RefMask;
    // Method - determines how to store object:
    // RefMask::StorePtr            : Store Ref pointers
    // RefMask::SingleFileStorage   : Store a num of connected Ref objects in a single file, if object found first time - store object
    //                                afterwords - store only reference to the stored object
    // RefMask::UnlinkedObjects     : Store unlinked Ref objects.
    // RefMask::SimpleEdit          : Store only name_of_class + internal params; name_of_class==NULL - null object


    static SavableClass *Read(istream &in, int Method = -1);
    static int Save(ostream &out, SavableClass *Clas, int Method = -1);
    static void Read(istream &in, SavableClass &Clas, int Method = -1);
    static int Save(ostream &out, SavableClass &Clas, int Method = -1);

    friend ostream &operator<<(ostream &out, SavableClass *cl) {
        Save(out, cl);
        return out;
    }
    friend istream &operator>>(istream &in, SavableClass *&cl) {
        cl = Read(in);
        return in;
    }
    static Stroka object2string(SavableClass *obj, int Method = -1) {
        ostrstream out;
        Save(out, obj, Method);
        return Stroka(out.str(), 0, out.pcount());
    }
    static SavableClass *string2object(const Stroka &str, int Method = -1) {
        std::istrstream in(~str, str.length());
        return Read(in, Method);
    }
    static SavableClass *name2object(const char *objname);

    static Stroka list_all_classes(const char *categ);
    static Stroka HelpForCategory(const char *categ, int Method = -1);


    static int SetRefOutMethod(int Method) {
        if(Method >= -1 || Method <= 2)
            return (OutMethod = Method);
        throw info_except("Trying to set bad out method %i\n", Method);
    }
    static int GetRefOutMethod() {
        return OutMethod;
    }
    // Identing
    static int IncIdent() {
        return SetIdentStr(++CurLevel);
    }
    static int DecIdent() {
        return SetIdentStr(--CurLevel);
    }
    static int ClearIdent() {
        return SetIdentStr(0);
    }
    static Stroka ppEOLN() {
        IncIdent();
        Stroka ret = "\n" + IdentStr;
        return ret;
    }
    static Stroka mmEOLN() {
        DecIdent();
        Stroka ret = "\n" + IdentStr;
        return ret;
    }
    static Stroka EOLN() {
        Stroka ret = "\n" + IdentStr;
        return ret;
    }
    // Utils
    // Test if next char in the stream is a given one, extract this char if OK
    static bool TestNextChar(std::istream &in, char ch, int ExtractGood = 1) {
        if(!in)
            throw info_except("bad input stream");
        char c = (char)in.peek();
        while(isspace(c) && !(!in)) {
            in.get(c);
            c = (char)in.peek();
        }
        if(ExtractGood && c == ch)
            in.get(c);
        return c == ch;
    }
    //TODO FIXIT !!!!  Depricated, use Verify !!!!!!!!!!!!!!
    // Throw exception if the next word int the sream ins not the given one
    static void ExceptionCheck(std::istream &in, const char *test, const char *mes = 0) {
        char tmp[256];
        in >> tmp;
        if(Stricmp(tmp, test) != 0) {
            Stroka Mes = Stroka(" Looks like an error, have to read ") + test +
                         " and read " + tmp + "\n";
            Mes += ShowStreamPosition(&in);
            if(mes)
                Mes += Stroka("Error message: ") + mes;
            throw(info_except("%s", Mes.c_str()));
        }
    }
    static Stroka ShowStreamPosition(istream *in, int ShowLen = 100) {
        Stroka ret("");
        if(in == NULL)
            return ret + "Input stream is NULL\n";
        if(!(*in))
            in->clear();
        char tmp[2512];
        streampos here = in->tellg();
        in->get(tmp, ShowLen, 0);
        Stroka after = Stroka(tmp);
        if(!(*in))
            in->clear();
        in->seekg(here - streampos(ShowLen - 1));
        in->get(tmp, ShowLen, 0);
        Stroka before = Stroka(tmp);
        if(!(*in))
            in->clear();
        in->seekg(here);
        ret += Stroka("Stream position: ") + Stroka::Int2Str(here) + "\nChars before:\n" +
               before + "\nChars after:\n" + after + "\n";
        return ret;
    }
    template <class T>
    static T *TestType(RefCount *var, const char *breakNull = NULL) {
        if(!var) {
            if(breakNull)
                throw info_except("variable is NULL:\n%s\n", breakNull);
            else
                return NULL;
        }
        T *ret = dynamic_cast<T *>(var);
        if(!ret)
            throw info_except(
                "variable's type is %s cannot convert to %s\n",
                typeid(*var).name(),
                typeid(T).name());

        return ret;
    }

private:
    static Stroka ReadingError(istream &in, SavableClass *s);
    static char SingleIdent[10];
    static int CurLevel, OutMethod;
    static Stroka IdentStr;
    static int SetIdentStr(int newLevel) {
        if(newLevel < 0)
            newLevel = 0;
        CurLevel = newLevel;
        IdentStr = "";
        for(int i = 0; i < CurLevel; i++)
            IdentStr += SingleIdent;
        return CurLevel;
    }
};


struct memstreambuf : public streambuf {
#define _ptr allocator.Ptr()
#define _n allocator.AllocSize()
public:
    memstreambuf() : allocator(100) {
        setp(_ptr, _ptr + _n);
    };
    memstreambuf(int s) : allocator(s) {
        setp(_ptr, _ptr + _n);
    };
    memstreambuf(char *ptr, int s) : allocator(s, ptr) {
        setg(_ptr, _ptr, _ptr + _n);
    }

    ~memstreambuf(){};
    streamsize out_waiting() {   // return count of buffered output characters
        streamsize ret_ =
            (pbase() != 0 && pbase() < pptr() ? (streamsize)(pptr() - pbase()) : 0);
        return ret_;
    }
    void realloc_s(long size_add) {
        long add = out_waiting();
        allocator.Allocate(_n + size_add);
        setp(_ptr, _ptr + _n);
        pbump(add);
    };
    void SetGet() {
        setg(_ptr, _ptr, _ptr + out_waiting());
    };

    //     void    freeze(int f=1){ allocator.Frozen=f;}
    char *str() {
        return _ptr;
    }
    virtual int overflow(int c) {
        realloc_s(1);
        return sputc(c);
    }
    virtual int underflow() {
        return egptr() > gptr() ? *gptr() : EOF;
    }
    virtual long xsputn(const char *buf, long s) {
        if(pptr() + s > epptr())
            realloc_s((long)(pptr() + s - epptr()));
        memmove(pptr(), buf, s);
        pbump(s);
        return s;
    };

    virtual long xsgetn(char *buf, long s) {
        int get = s < egptr() - gptr() ? s : egptr() - gptr();
        memmove(buf, gptr(), get);
        gbump(get);
        return get;
    };

private:
    AllocatorIntellect<char> allocator;
};


struct DataSource : virtual DescribedClass {
    enum DataType { Memory, Disk, Console };
    enum OpenMode { In, Out };
    OpenMode openmode;
    DataType datatype;
    char dataname[1256];
    streambuf *databuf;
    char Category[1256];
    typedef Ref<DataSource> DataSourceRef;
    typedef std::map<string, DataSourceRef> MAPCLASS;
    static MAPCLASS *data_list;


    static void CloseCategories(char *category = NULL);
    static void CloseSource(char *name);
    static void CloseSource(streambuf *buf);
    static int DuplicatePresent(const char *name, Ref<DataSource> &ref);
    static int NumUnicObjects;
    static Stroka GenerateUnicName(const char *basic);
    //  char *FormKey();
    DataSource() : databuf(NULL){};
    ~DataSource();

    static streambuf *OpenSource(
        const char *name,
        OpenMode op_mode = DataSource::In,
        DataType dat_type = DataSource::Disk,
        char *category = NULL,
        const char *file_name = NULL);

    // GetContent of given type stream buf
    static string GetStr(const char *name);

    virtual void print(ostream &out = ExEnv::out());
};

struct FilterIn : virtual istream {
    // Part added to restore objects (SavableClass) as new ones
    // with correct references between them.
private:
    AllocatorIntellect<void *> refsaved;
    int refmask, numrefsaved;
    int AutomaticallyCloseBuf;

public:
    inline int SetRefMask(int mask = -1) {
        if(mask != -1)
            refmask = mask;
        return refmask;
    }
    inline void ClearRef() {
        numrefsaved = 0;
        refsaved.Empty();
    };

    FilterIn(streambuf *s, int autoclosebuf = 1)
        : istream(NULL), refmask(SavableClass::GetRefOutMethod()), numrefsaved(0) {
        init(s);
        AutoCloseBuf(autoclosebuf);
    };
    FilterIn(int autoclosebuf = 1)
        : istream(NULL), refmask(SavableClass::GetRefOutMethod()), numrefsaved(0) {
        AutoCloseBuf(autoclosebuf);
    };
    virtual ~FilterIn() {
        if(AutomaticallyCloseBuf)
            CloseBuf();
    }
    int AutoCloseBuf(int set = -1) {
        if(set != -1)
            AutomaticallyCloseBuf = set;
        return AutomaticallyCloseBuf;
    }
    void OpenBuf(
        const char *name,
        int CloseBufBefore = 0,
        DataSource::DataType dat_type = DataSource::Disk,
        char *category = NULL,
        const char *file_name = NULL) {
        SetNewBuf(
            DataSource::OpenSource(name, DataSource::In, dat_type, category, file_name),
            CloseBufBefore);
    };
    void CloseBuf() {
        ClearRef();
        DataSource::CloseSource(rdbuf());
        init(NULL);
    }
    void SetNewBuf(streambuf *sb, int CloseBufBefore = 0) {
        if(CloseBufBefore)
            DataSource::CloseSource(rdbuf());
        init(sb);
        refmask = SavableClass::GetRefOutMethod();
    };

    streambuf *GetBuf() {
        return rdbuf();
    }

    virtual void get(int &buf) = 0;
    virtual void get(double &buf) = 0;
    virtual void get(char &buf) = 0;
    virtual void get(long &buf) = 0;
    virtual void get(KeyDesc &buf) = 0;
    virtual void get(void *&buf) = 0;
    virtual void getword(char *buf, const char *delims = " \t\r\v\f\n") = 0;

    //  virtual void getstring(char *buf,char delim='\n');

    virtual int get_arr(char *buf) {
        int Num;
        get(Num);
        read(buf, Num);
        return Num;
    };
    virtual int get_arr(int *buf) {
        int Num;
        get(Num);
        for(int k = 0; k < Num; k++)
            get(buf[k]);
        return Num;
    };
    virtual int get_arr(long *buf) {
        int Num;
        get(Num);
        for(int k = 0; k < Num; k++)
            get(buf[k]);
        return Num;
    };
    virtual int get_arr(double *buf) {
        int Num;
        get(Num);
        for(int k = 0; k < Num; k++)
            get(buf[k]);
        return Num;
    };

    virtual int GetSCStored(int &numobj);

private:
    void ConstructObject(SavableClass *&sc, const char *objname) ;
    void GetObjectBody(SavableClass *&sc);
    Stroka CheckReadNullObj(SavableClass *&sc);
    void getobjstrt();
    int getobjfin(RefBase &buf, SavableClass *sc);
    Stroka getword();


public:
    // if StorePtr          - store referenced pointer          :
    //          ptr
    // if SingleFileStorage - store head, ref_number, object    :
    //          NULL_POINTER
    //          Key(ObjectRefNumber) 1
    //          Key(ObjectRefNumber) 3 ObjName Pref{ Obj Post} Key(})
    // if UnlinkedObjects   - store  head, not store ref_number, store object :
    //          NULL_POINTER
    //          ObjName Pref{ Obj Post}
    // if SimpleEdit       - store  head, not store ref_number, store object :
    //          NULL_POINTER
    //          ObjName Pref{ Obj Post}
    virtual int getobject(SavableClass *&sc);
    // if StorePtr          - store referenced pointer          :
    //          Obj
    // if SingleFileStorage - store head, ref_number, object    :
    //          Key(ObjectRefNumber) 3 Obj
    // if UnlinkedObjects   - store  head, not store ref_number, store object :
    //          Obj
    // if SimpleEdit       - store  head, not store ref_number, store object :
    //          Obj
    virtual int getobject(SavableClass &sc);
    virtual SavableClass *getobject();
    virtual int getobject(RefBase &buf);
private:
    virtual string ReadingError(SavableClass *s) {
        string ret("Could not read(save) class:");
        if(!s)
            ret.append("NULL");
        else
            ret.append(typeid(*s).name());
        ret += string(~SavableClass::ShowStreamPosition(this));
        return ret;
    };
};

struct FilterTextIn : FilterIn {
#define WORDLENGTH 256
    //  FilterTextIn(streambuf*s,int autoclosebuf=1):FilterIn(s,autoclosebuf){};
    FilterTextIn(streambuf *s, int autoclosebuf = 1)
        : istream(NULL), FilterIn(s, autoclosebuf){};   //{SetNewBuf(s);};
    FilterTextIn(int autoclosebuf = 1) : istream(NULL), FilterIn(autoclosebuf){};
    FilterTextIn(
        const char *name,
        DataSource::DataType dat_type = DataSource::Disk,
        char *category = NULL,
        int autoclosebuf = 1,
        const char *file_name = NULL)
        : istream(NULL), FilterIn(NULL, autoclosebuf) {
        OpenBuf(name, 0, dat_type, category, file_name);
    }

    void get(int &buf) {
        istream::operator>>((int &)buf);
    }
    void get(double &buf) {
        istream::operator>>((double &)buf);
    };
    //  void get(char &buf)  {istream::operator>>((char &)buf);};
    void get(char &buf) {
        istream::get(buf);
    };
    void get(long &buf) {
        istream::operator>>((long &)buf);
    };
    void get(KeyDesc &buf) {
        char tmp[256];
        getword(&tmp[0]);
        buf = &tmp[0];
    };
    void getword(char *buf, const char *delims = " \t\r\v\f\n") {
        do {
            get(buf[0]);
        } while(!istream::operator!() && strchr(delims, buf[0]) != NULL);
        if(istream::operator!()) {
            buf[0] = 0;
            return;
        }
        int i = 0;
        do {
            i++;
            get(buf[i]);
        } while(!istream::operator!() && strchr(delims, buf[i]) == NULL);
        if(istream::operator!())
            buf[i + 1] = 0;
        else {
            istream::unget();
            buf[i] = 0;
        }
    };
    void get(void *&buf) {
        char tmp[256];
        istream::get(tmp, 255);
        buf = NULL;
    };
};

struct FilterBinIn : FilterIn {
    FilterBinIn(streambuf *s, int autoclosebuf = 1)
        : istream(NULL), FilterIn(s, autoclosebuf){};
    FilterBinIn(int autoclosebuf = 1) : istream(NULL), FilterIn(autoclosebuf){};
    FilterBinIn(
        const char *name,
        DataSource::DataType dat_type = DataSource::Disk,
        char *category = NULL,
        int autoclosebuf = 1,
        const char *file_name = NULL)
        : istream(NULL), FilterIn(NULL, autoclosebuf) {
        OpenBuf(name, 0, dat_type, category, file_name);
    }

    void get(char &buf) {
        read(&buf, 1);
    };
    void get(int &buf) {
        read((char *)&buf, sizeof(int));
    };
    void get(double &buf) {
        read((char *)&buf, sizeof(double));
    };
    void get(long &buf) {
        read((char *)&buf, sizeof(long));
    };
    void get(void *&buf) {
        read((char *)&buf, sizeof(void *));
    };
    void get(KeyDesc &buf){};   //{getword(buf.name);};
    void getword(char *buf, const char *delims = " \t\r\v\f\n") {
        int len;
        read((char *)&len, sizeof(int));
        read(buf, len);
        buf[len] = 0;
    };

    virtual int get_arr(char *buf) {
        int Num;
        read((char *)&Num, sizeof(int));
        read(buf, Num);
        return Num;
    };
    virtual int get_arr(int *buf) {
        int Num;
        read((char *)&Num, sizeof(int));
        read((char *)buf, Num * sizeof(int));
        return Num;
    };
    virtual int get_arr(long *buf) {
        int Num;
        read((char *)&Num, sizeof(int));
        read((char *)buf, Num * sizeof(long));
        return Num;
    };
    virtual int get_arr(double *buf) {
        int Num;
        read((char *)&Num, sizeof(int));
        read((char *)buf, Num * sizeof(double));
        return Num;
    };
};


struct FilterOut : virtual ostream {
    // Part added to restore objects (SavableClass) as new ones
    // with correct references between them.
private:
    AllocatorIntellect<void *> refsaved;
    int refmask, numrefsaved;
    int AutomaticallyCloseBuf;


public:
    inline int SetRefMask(int mask = -1) {
        if(mask != -1)
            refmask = mask;
        return refmask;
    }
    inline void ClearRef() {
        numrefsaved = 0;
        refsaved.Empty();
    };
    FilterOut(streambuf *s, int autoclosebuf = 1)
        : ostream(NULL), refmask(SavableClass::GetRefOutMethod()), numrefsaved(0) {
        init(s);
        AutoCloseBuf(autoclosebuf);
    }   //:istream(s){};
    FilterOut(int autoclosebuf = 1)
        : ostream(NULL), refmask(SavableClass::GetRefOutMethod()), numrefsaved(0) {
        AutoCloseBuf(autoclosebuf);
    };
    virtual ~FilterOut() {
        if(AutomaticallyCloseBuf)
            CloseBuf();
    }
    int AutoCloseBuf(int set = -1) {
        if(set != -1)
            AutomaticallyCloseBuf = set;
        return AutomaticallyCloseBuf;
    }

    void OpenBuf(
        const char *name,
        int CloseBufBefore = 0,
        DataSource::DataType dat_type = DataSource::Disk,
        char *category = NULL,
        const char *file_name = NULL) {
        SetNewBuf(
            DataSource::OpenSource(name, DataSource::Out, dat_type, category, file_name),
            CloseBufBefore);
    };

    void CloseBuf(int AlsoInit = 1) {
        ClearRef();
        DataSource::CloseSource(rdbuf());
        if(AlsoInit)
            init(NULL);
    };
    void SetNewBuf(streambuf *sb, int CloseBufBefore = 0) {
        if(CloseBufBefore)
            CloseBuf(0);   //DataSource::CloseSource(rdbuf());
                           //     refmask=SavableClass::StorePtr;
        refmask = SavableClass::GetRefOutMethod();
        numrefsaved = 0;
        init(sb);
    };
    streambuf *GetBuf() {
        return rdbuf();
    }


    virtual void set(int buf) = 0;
    virtual void set(double buf) = 0;
    virtual void set(char buf) = 0;
    virtual void set(long buf) = 0;
    virtual void set(void *buf) = 0;
    virtual void set(const KeyDesc &buf) = 0;
    virtual void setword(const char *buf) = 0;

    virtual void set_arr(char *buf, int Num) {
        set(Num);
        write(buf, Num);
    };
    virtual void set_arr(int *buf, int Num) {
        set(Num);
        for(int k = 0; k < Num; k++)
            set(buf[k]);
    };
    virtual void set_arr(long *buf, int Num) {
        set(Num);
        for(int k = 0; k < Num; k++)
            set(buf[k]);
    };
    virtual void set_arr(double *buf, int Num) {
        set(Num);
        for(int k = 0; k < Num; k++)
            set(buf[k]);
    };

    virtual int CheckSCStored(SavableClass &sc) {
        int stored = sc.SetClassStored(), ret = 1;
        if((!stored) || (stored > numrefsaved) || (refsaved[stored - 1] != (void *)&sc)) {
            ret = 0;
            refsaved[numrefsaved] = (void *)&sc;
            numrefsaved++;
            stored = numrefsaved;
            sc.SetClassStored(stored);
        }
        setword("ObjectRefNumber");
        set(stored);
        return ret;
    };

    virtual int setobject(SavableClass *sc) {
        // if StorePtr          - store referenced pointer          :
        //          ptr
        // if SingleFileStorage - store head, ref_number, object    :
        //          NULL_POINTER
        //          Key(ObjectRefNumber) 1
        //          Key(ObjectRefNumber) 3 ObjName Pref{ Obj Post} Key(})
        // if UnlinkedObjects   - store  head, not store ref_number, store object :
        //          NULL_POINTER
        //          ObjName Pref{ Obj Post}
        // if SimpleEdit       - store  head, not store ref_number, store object :
        //          NULL_POINTER
        //          ObjName Pref{ Obj Post}
        if(refmask == SavableClass::StorePtr) {
            set((void *)sc);
            return (!(!*this));
        };
        if(!sc) {
            setword("NULL_POINTER");
            return (!(!*this));
        }
        if(refmask == SavableClass::SingleFileStorage && CheckSCStored(*sc))
            return 1;

        setobjectbody(sc);
        return (!(!*this));
    };

    virtual int setobject(SavableClass &sc) {
        // if StorePtr          - store referenced pointer          :
        //          Obj
        // if SingleFileStorage - store head, ref_number, object    :
        //          Key(ObjectRefNumber) 3 Obj
        // if UnlinkedObjects   - store  head, not store ref_number, store object :
        //          Obj
        // if SimpleEdit       - store  head, not store ref_number, store object :
        //          Obj
        if((void *)&sc == NULL)
            throw info_except(" trying to save NULL object?? is it possible?\n");
        //        {ExEnv::err()<<" Error setobject, trying to save NULL object\n";ExEnv::err().flush();abort();}
        if(refmask == SavableClass::SingleFileStorage)
            CheckSCStored(sc);
        sc.save_data_state(*this);
        return (!(!*this));
    };
    void setobjectbody(SavableClass *sc) {
        setword(sc->class_name());
        sc->save_data_pref(*this);
        sc->save_data_state(*this);
        sc->save_data_post(*this);
    }
    void setobjstrt() {
        if(refmask != SavableClass::SimpleEdit) {
            SavableClass::IncIdent();
            set(KeyDesc(Stroka("ReferenceClass_{") + SavableClass::EOLN()));
        }
    }
    int setobjfin() {
        if(refmask != SavableClass::SimpleEdit) {
            SavableClass::DecIdent();
            set(KeyDesc(Stroka("}") + SavableClass::EOLN()));
        }
        return (!(!*this));
    }
    virtual int setobject(RefBase &buf) {
        // if StorePtr          - store referenced pointer          :
        //          Key(ReferenceClass_{) ptr Key(})
        // if SingleFileStorage - store head, ref_number, object    :
        //          Key(ReferenceClass_{) NULL_POINTER Key(})
        //          Key(ReferenceClass_{) Key(ObjectRefNumber) 1 Key(})
        //          Key(ReferenceClass_{) Key(ObjectRefNumber) 3 ObjName Pref{ Obj Post} Key(})
        // if UnlinkedObjects   - store  head, not store ref_number, store object :
        //          Key(ReferenceClass_{) NULL_POINTER Key(})
        //          Key(ReferenceClass_{) ObjName Pref{ Obj Post} Key(})
        // if SimpleEdit       - store  head, not store ref_number, store object :
        //          NULL_POINTER
        //          ObjName Pref{ Obj Post}
        setobjstrt();
        if(refmask == SavableClass::StorePtr) {
            void *ptr = buf.parentpointer();
            set(ptr);
            return setobjfin();
        }

        SavableClass *sc = buf.GetSavableBase();
        if(!sc) {
            setword("NULL_POINTER");
            return setobjfin();
        }

        if(refmask == SavableClass::SingleFileStorage && CheckSCStored(*sc))
            return setobjfin();
        setobjectbody(sc);

        return setobjfin();
    };

};


struct FilterTextOut : FilterOut {
    FilterTextOut(streambuf *s, int autoclosebuf = 1)
        : ostream(NULL), FilterOut(s, autoclosebuf){};
    FilterTextOut(int autoclosebuf = 1) : ostream(NULL), FilterOut(autoclosebuf){};
    FilterTextOut(
        const char *name,
        DataSource::DataType dat_type = DataSource::Disk,
        char *category = NULL,
        int autoclosebuf = 1,
        const char *file_name = NULL)
        : ostream(NULL), FilterOut(NULL, autoclosebuf) {
        OpenBuf(name, 0, dat_type, category, file_name);
    }

    void set(char buf) {
        put(buf);
    };
    void set(int buf) {
        ostream::operator<<((int)buf);
        put(fill());
    }
    void set(double buf) {
        ostream::operator<<((double)buf);
        put(fill());
    };
    void set(long buf) {
        ostream::operator<<((long)buf);
        put(fill());
    };
    ;
    void set(void *buf) {
        ostream::operator<<((void *)buf);
        put(fill());
    };
    ;
    void set(const KeyDesc &buf) {
        setword(buf.name);
    };
    //  void setword(const char *buf){ostream::operator<<((char *)buf);put(fill());};
    void setword(const char *buf) {
        write(buf, std::streamsize(strlen(buf)));
        put(fill());
    };
};

struct FilterBinOut : FilterOut {
    FilterBinOut(streambuf *s, int autoclosebuf = 1)
        : ostream(NULL), FilterOut(s, autoclosebuf){};
    FilterBinOut(int autoclosebuf = 1) : ostream(NULL), FilterOut(autoclosebuf){};
    FilterBinOut(
        const char *name,
        DataSource::DataType dat_type = DataSource::Disk,
        char *category = NULL,
        int autoclosebuf = 1,
        const char *file_name = NULL)
        : ostream(NULL), FilterOut(NULL, autoclosebuf) {
        OpenBuf(name, 0, dat_type, category, file_name);
    }

    void set(char buf) {
        put(buf);
    };
    void set(int buf) {
        write((char *)&buf, sizeof(int));
    }
    void set(double buf) {
        write((char *)&buf, sizeof(double));
    };
    void set(long buf) {
        write((char *)&buf, sizeof(long));
    };
    ;
    void set(void *buf) {
        write((char *)&buf, sizeof(void *));
    };
    ;
    void set(const KeyDesc &buf){};   //{setword(buf.name);};
    void setword(const char *buf) {
        size_t len = strlen(buf);
        write((char *)&len, sizeof(int));
        write(buf, (streamsize)len);
    };

    virtual void set_arr(char *buf, int Num) {
        set(Num);
        write(buf, Num * sizeof(int));
    };
    virtual void set_arr(int *buf, int Num) {
        set(Num);
        write((char *)buf, Num * sizeof(int));
    };
    virtual void set_arr(long *buf, int Num) {
        set(Num);
        write((char *)buf, Num * sizeof(long));
    };
    virtual void set_arr(double *buf, int Num) {
        set(Num);
        write((char *)buf, Num * sizeof(double));
    };
};


inline FilterIn &operator>>(FilterIn &si, char *buf) {
    verify(si, "failed reading char*");
    si.getword(buf);
    verify(si, "failed reading buf " + buf);
    return si;
}
inline FilterIn &operator>>(FilterIn &si, char &buf) {
    verify(si, "failed reading char");
    si.get(buf);
    verify(si, "failed reading char " + buf);
    return si;
}
inline FilterIn &operator>>(FilterIn &si, double &buf) {
    verify(si, "Failed reading double");
    si.get(buf);
    verify(si, "Failed reading double " + buf);
    return si;
}
inline FilterIn &operator>>(FilterIn &si, int &buf) {
    verify(si, "Failed reading int");
    si.get(buf);
    verify(si, "Failed reading int " + buf);
    return si;
}
inline FilterIn &operator>>(FilterIn &si, long &buf) {
    verify(si, "Failed reading long");
    si.get(buf);
    verify(si, "Failed reading long" + buf);
    return si;
}
inline FilterIn &operator>>(FilterIn &si, KeyDesc &buf) {
    assert(!(!si));
    si.get(buf);
    assert(!(!si));
    return si;
}
inline FilterIn &operator>>(FilterIn &si, SavableClass *&buf) {
    verify(si, "Failed reading object");
    si.getobject(buf);
    verify(si, "Failed after reading object" + SavableClass::object2string(buf));
    return si;
}
inline FilterIn &operator>>(FilterIn &si, SavableClass &buf) {
    si.getobject(buf);
    return si;
}
inline FilterIn &operator>>(FilterIn &si, RefBase &buf) {
    verify(si, "Failed reading object");
    si.getobject(buf);
    verify(si, "Failed reading object" + typeid(buf).name());
    return si;
}


inline FilterOut &operator<<(FilterOut &so, const char *buf) {
    so.setword(buf);
    return so;
}
inline FilterOut &operator<<(FilterOut &so, char buf) {
    so.set(buf);
    return so;
}
inline FilterOut &operator<<(FilterOut &so, double buf) {
    so.set(buf);
    return so;
}
inline FilterOut &operator<<(FilterOut &so, int buf) {
    so.set(buf);
    return so;
}
inline FilterOut &operator<<(FilterOut &so, long buf) {
    so.set(buf);
    return so;
}
inline FilterOut &operator<<(FilterOut &so, const KeyDesc &buf) {
    so.set(buf);
    return so;
}
inline FilterOut &operator<<(FilterOut &so, SavableClass *buf) {
    so.setobject(buf);
    return so;
}
inline FilterOut &operator<<(FilterOut &so, SavableClass &buf) {
    so.setobject(buf);
    return so;
}
inline FilterOut &operator<<(FilterOut &so, RefBase &buf) {
    so.setobject(buf);
    return so;
}



#endif

