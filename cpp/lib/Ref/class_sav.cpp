#include <lib/precompiled/Ref.h>

#include "class_sav.h"


DataSource::MAPCLASS *DataSource::data_list = NULL;

static ClassDesc SavableClass_cd(typeid(SavableClass), "SavableClass");

void SavableClass::save_data_post(FilterOut &so) {
    DecIdent();
    so << KeyDesc(EOLN()) << (const char *)"}" << KeyDesc(EOLN());
};
void SavableClass::save_data_pref(FilterOut &so) {
    so.setword("{");
    IncIdent();
    so << KeyDesc(EOLN());
};
int SavableClass::read_data_post(FilterIn &si) {
    ExeptionCheck(si, "}");
    return 1;
}
int SavableClass::read_data_pref(FilterIn &si) {
    ExeptionCheck(si, "{");
    return 1;
}
void SavableClass::LoadClass(SavableClass &from, int method) {
    if(strcmp(from.class_name(), this->class_name()) != 0)
        throw info_except(
            "Classes have different names: <%s>!=<%s>\n",
            this->class_name(),
            from.class_name());
    strstreambuf out;
    FilterTextOut fo(&out, 0);
    if(method == -1)
        method = GetRefOutMethod();
    fo.SetRefMask(method);
    fo.setobject(from);
    FilterTextIn fi(&out, 0);
    fi.SetRefMask(method);
    fi.getobject(*this);
    fo.CloseBuf();
};
SavableClass *SavableClass::Duplicate(int Method) {
    return SavableClass::string2object(SavableClass::object2string(this, Method), Method);
}

Stroka SavableClass::ReadingError(istream &in, SavableClass *s) {
    Stroka ret("Could not read class:");
    if(!s)
        ret += "NULL";
    else
        ret += s->class_name();
    if(!in)
        in.clear();
    char tmp[512], tmp1[512];
    in.read(tmp1, 512);
    tmp1[511] = 0;
    ret += Stroka("\nError position: ") + Itoa(in.tellg(), tmp, 10) +
           "\nChars after error\n" + tmp1 + "\n";
    return ret;
};
SavableClass *SavableClass::Read(istream &in, int Method) {
    SavableClass *Func = 0;
    if(!in)
        throw info_except(" bad stream\n %s\n", ReadingError(in, Func).c_str());
    if(Method == -1)
        Method = GetRefOutMethod();
    Ref<SavableClass> cl;
    FilterTextIn input(in.rdbuf(), 0);
    input.SetRefMask(Method);   //SingleFileStorage);
    //input>>cl;
    input.getobject(cl);
    return cl.release_temp_settled_ptr();
}
int SavableClass::Save(ostream &out, SavableClass *Clas, int Method) {
    if(!out)
        throw info_except(" bad out stream\n ");
    if(Method == -1)
        Method = GetRefOutMethod();
    Ref<SavableClass> cl(Clas);
    FilterTextOut output(out.rdbuf(), 0);
    output.SetRefMask(Method);   //SingleFileStorage);
    output << cl;
    cl.release_temp_settled_ptr();
    return !(!out);
}
void SavableClass::Read(istream &in, SavableClass &Clas, int Method) {
    try {
        if(!in)
            throw info_except(" bad stream\n ");
        if(Method == -1)
            Method = GetRefOutMethod();
        FilterTextIn input(in.rdbuf(), 0);
        input.SetRefMask(Method);   //SingleFileStorage);
                                    //input>>Clas;
        input.getobject(Clas);
        if(!in)
            throw info_except(" Could not read class %s\n ", Clas.class_name());
        //} catch(stdexception &ex){
    } catch(exception &ex) {
        throw info_except(
            "Got an error %s.\n %s", ex.what(), ReadingError(in, &Clas).c_str());
    }
}

int SavableClass::Save(ostream &out, SavableClass &Clas, int Method) {
    if(!out)
        throw info_except(" bad out stream\n ");
    if(Method == -1)
        Method = GetRefOutMethod();
    FilterTextOut output(out.rdbuf(), 0);
    output.SetRefMask(Method);   //SingleFileStorage);
    output << Clas;
    return !(!out);
}

SavableClass *SavableClass::name2object(const char *objname) {
    SavableClass *sc;
    ClassDesc *cd = ClassDesc::name_to_class_desc(objname);
    if(cd)
        sc = dynamic_cast<SavableClass *>(cd->create());
    else
        throw info_except(" Class %s unknown \n", objname);
    if(!sc)
        throw info_except(
                " Class %s  CreateMethod is not registered add it.\n", objname);
    return sc;
}

char SavableClass::SingleIdent[10] = "   ";
int SavableClass::CurLevel = 0, SavableClass::OutMethod = SavableClass::SimpleEdit;
Stroka SavableClass::IdentStr = "";

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

DataSource::~DataSource() {
    if(databuf == NULL)
        return;
    //cout<<"Deleting DataSource\n";print();cout.flush();
    switch(datatype) {
    case Memory:
        delete databuf;
        break;
    case Disk: {
        filebuf *fbuf = (filebuf *)databuf;
        fbuf->close();
        delete databuf;
    } break;
    case Console:
        delete databuf;
        break;
    default:
        cout << "Error in DataSource ~DataSource switch : dat_type - is not known/n";
        exit(0);
    }
    databuf = NULL;
};

int DataSource::DuplicatePresent(const char *name, Ref<DataSource> &ref) {
    MAPCLASS::iterator itr = data_list->find(name);
    if(itr == data_list->end())
        return 0;
    ref = itr->second;
    return 1;
}

streambuf *DataSource::OpenSource(
    const char *name,
    OpenMode op_mode,
    DataType dat_type,
    char *category,
    const char *file_name) {
    if(!data_list)
        data_list = new MAPCLASS();
    Ref<DataSource> dref = new DataSource;
    dref->openmode = op_mode;
    dref->datatype = dat_type;
    dref->Category[0] = 0;
    if(category != NULL)
        strcpy(dref->Category, category);
    strcpy(dref->dataname, name);
    dref->databuf = NULL;

    Ref<DataSource> wasref;
    if(DuplicatePresent(name, wasref))
        if(dat_type != Memory || op_mode != In) {
            cout << " DataSource::OpenSource - duplicate_name. Will close previous:\n";
            wasref->print();
        } else
            ;
    else if(dat_type == Memory && op_mode == In) {
        cout << " DataSource::OpenSource - trying to open not existing Memory Buf:\n";
        dref->print();
        exit(0);
    }

    switch(dat_type) {
    case Memory:
        if(op_mode == Out)
            dref->databuf = new memstreambuf();
        else {
            if(wasref->datatype == Memory) {
                memstreambuf *wasbuf = (memstreambuf *)wasref->databuf;
                wasbuf->SetGet();
                dref->databuf = wasbuf;
                wasref->databuf = NULL;
                // Insertion will automaticcaly delete old Data
            } else {
                cout << " DataSource::OpenSource - trying to open Memory Buf: ";
                cout << " with wrong type\n";
                dref->print();
                exit(0);
            }
        }
        break;
    case Disk: {
        filebuf *fbuf = new filebuf;
        const char *open_name = file_name ? file_name : name;

        if(op_mode == In)
            dref->databuf = fbuf->open(open_name, ios::in);
        else
            dref->databuf = fbuf->open(open_name, ios::out);
        if(dref->databuf == NULL)
            delete fbuf;
    } break;
    case Console:
        // TODO(MAC/WIDOWS)
        //         if (op_mode==In) dref->databuf = new filebuf(stdin);//stdiobuf(stdin);
        //         else dref->databuf = new filebuf(stdout);//stdiobuf(stdout);
        //         break;
        if(op_mode == In)
            dref->databuf =
                (new filebuf)->open("/dev/stdin", ios_base::in | ios_base::app);
        else
            dref->databuf =
                (new filebuf)->open("/dev/stdout", ios_base::out | ios_base::app);
        break;
    default:
        cout << "Error in DataSource OpenSource switch : dat_type - is not known/n";
        exit(0);
    }
    if(dref->databuf == NULL)
        return NULL;   //dref=NULL;
    (*data_list)[dref->dataname] = dref;
    return dref->databuf;
};


string DataSource::GetStr(const char *name) {
    Ref<DataSource> fromref = (*data_list)[name];
    if((fromref != NULL) && (fromref->datatype == Memory)) {
        memstreambuf *frombuf = (memstreambuf *)fromref->databuf;
        int N = frombuf->out_waiting();

        string ret;
        ret.insert(0, frombuf->str(), N);
        ret += '\x0';
        //          char *ret=new char[N+1];memmove(ret,frombuf->str(),N);ret[N]=0;
        return ret;
    }
    //       return NULL;
    return string("");
};

int DataSource::NumUnicObjects = 0;
Stroka DataSource::GenerateUnicName(const char *basic) {
    Stroka Ret = basic ? basic : "";
    //Ret += Stroka::Int2Str((int)RandomClass::Rnd(1e8));
    Ret += "_" + Stroka::Int2Str(NumUnicObjects++);
    log_debug("Generated unic name: " + Ret);
    return Ret;
}

void DataSource::CloseCategories(char *category) {
    if(data_list == NULL)
        return;
    Ref<DataSource> refd;
    for(MAPCLASS::iterator ind = data_list->begin(); ind != data_list->end();) {
        if((category == NULL) || (Stricmp(category, ind->second->Category) == 0))
            ind = data_list->erase(ind);
        else
            ind++;
    }
    if(data_list->size() == 0) {
        delete data_list;
        data_list = NULL;
    } else if(category == NULL) {
        cout << " Very bad. Not all DataSource::CloseCategories, have to be all\n";
    }
}

void DataSource::CloseSource(char *name) {
    if(data_list == NULL)
        return;
    data_list->erase(name);
};

void DataSource::CloseSource(streambuf *buf) {
    if(data_list == NULL)
        return;
    Ref<DataSource> refd;
    for(MAPCLASS::iterator ind = data_list->begin(); ind != data_list->end();) {
        if(ind->second->databuf == buf) {
            //cout<<" CloseBuf size "<<data_list->size();
            //cout<<" "<<ind->second->dataname;
            ind = data_list->erase(ind);
            //cout<<" "<<data_list->size()<<"\n"<<flush;
        } else
            ind++;
    }
}

void DataSource::print(ostream &out) {
    const char *type[] = {"Memory", "Disk", "Console"}, *mode[] = {"In", "Out"};
    out << " DataSource_data name: \"" << dataname << "\" type: \"" << type[datatype]
        << "\" Openmode: \"" << mode[openmode] << "\" Category: \"" << Category
        << "\"\n";
};




int FilterIn::GetSCStored(int &numobj) {
    get(numobj);
    if(!(*this))
        throw info_except("Bad stream - could not read numobj!!!\n");

    if(numobj <= numrefsaved)
        return 1;
    if(numrefsaved + 1 != numobj)
        throw info_except(
                "Error restoring RefBase - num objects saved %i  number of object %i \n%s",
                numrefsaved,
                numobj,
                ReadingError(NULL).c_str());
    numrefsaved++;
    return 0;
};


void FilterIn::ConstructObject(SavableClass *&sc, const char *objname) {
    ClassDesc *cd;
    Stroka tmp_name(objname);
    if(!objname) {
        char name[256];
        getword(&name[0]);
        tmp_name = Stroka(name);
        cd = ClassDesc::name_to_class_desc(name);
    } else
        cd = ClassDesc::name_to_class_desc(objname);
    if(cd)
        sc = dynamic_cast<SavableClass *>(cd->create());
    else
        throw info_except(" StateIn: class %s unknown \n", tmp_name.c_str());
    if(!sc)
        throw info_except(
                " StateIn: class %s  CreateMethod is not registered add it.\n",
                tmp_name.c_str());
}
void FilterIn::GetObjectBody(SavableClass *&sc) {
    if((!sc->read_data_pref(*this)) || (!sc->read_data_state(*this)) ||
       (!sc->read_data_post(*this)))
        throw info_except(
                "Error while reading by pref,state or post savable class functions.\n ");
}
Stroka FilterIn::CheckReadNullObj(SavableClass *&sc) {
    Stroka ret = getword();
    log_debug(string("Read ") + ~ret);
    if(ret == "NULL_POINTER") {
        sc = NULL;
        return "";
    }
    return ret;
}
void FilterIn::getobjstrt() {
    if(refmask != SavableClass::SimpleEdit)
        get(StdKey);
}
int FilterIn::getobjfin(RefBase &buf, SavableClass *sc) {
    if(refmask != SavableClass::SimpleEdit)
        get(StdKey);
    buf.GetSavableBase(sc);
    return (!(!*this));
}
Stroka FilterIn::getword() {
    char tmp[256];
    getword(tmp);
    return Stroka(tmp);
}




int FilterIn::getobject(SavableClass *&sc) {
    sc = NULL;
    try {
        if(!(*this))
            throw info_except("Bad stream.\n");
        if(refmask == SavableClass::StorePtr) {
            void *tmp;
            get(tmp);
            sc = (SavableClass *)(tmp);
            return (!(!*this));
        };
        Stroka str = CheckReadNullObj(sc);
        if(str.length() == 0)
            return 1;
        bool packedReferenceClass = false;

        if(refmask == SavableClass::SingleFileStorage) {
            verify(str == "ObjectRefNumber" || str == "ReferenceClass_{", string("Got: ") + ~str);
            if (str == "ReferenceClass_{") {
                packedReferenceClass = true;
                Stroka tmp = getword();
                verify( tmp == "ObjectRefNumber", string("Got: ") + ~tmp);
            }
            int numobj;
            if(GetSCStored(numobj)) {
                sc = (SavableClass *)refsaved[numobj - 1];
                return 1;
            }
            str = getword();
        }
        log_debug(string(" ConstructObject::str ") + ~str);
        ConstructObject(sc, ~str);
        if(refmask == SavableClass::SingleFileStorage)
            refsaved[numrefsaved - 1] = (void *)sc;
        GetObjectBody(sc);
        if (packedReferenceClass) {
            Stroka tmp = getword();
            verify(tmp == "}", string("Got: ") + ~tmp);
        }

        return (sc != NULL);
    } catch(exception &ex) {
        throw info_except(
                "Got an error %s.\n %s", ex.what(), ReadingError(sc).c_str());
    }
};

int FilterIn::getobject(SavableClass &sc) {
    try {
        if(!(*this))
            throw info_except("Bad stream\n");
        if(refmask == SavableClass::SingleFileStorage) {
            int numobj;
//            get(StdKey);
//            log_debug(string("Got mistery: ") + StdKey.name);
//            log_debug(string("Got: ") + StdKey.name);
            get(StdKey);
            verify(string(StdKey.name) == "ObjectRefNumber", string("Got:") + StdKey.name);
            if(GetSCStored(numobj))
                throw info_except(
                        "Error, already stored. ups.Object Saved under Number %i Total num of stored objects %i\n",
                        numobj,
                        numrefsaved);
            refsaved[numrefsaved - 1] = (void *)&sc;
        }
        if(!sc.read_data_state(*this))
            throw info_except(" ERROR: StateIn: could not read_data_state\n ");
        return 1;
    } catch(exception &ex) {
        throw info_except(
                "Got an error %s.\n %s", ex.what(), ReadingError(&sc).c_str());
    }
};

SavableClass *FilterIn::getobject() {
    SavableClass *sc = NULL;
    getobject(sc);
    return sc;
};


int FilterIn::getobject(RefBase &buf) {
    try {
        if(!(*this))
            throw info_except("Bad stream\n");
        SavableClass *sc = NULL;
        getobjstrt();
        if(refmask == SavableClass::StorePtr) {
            void *ptr;
            get(ptr);
            return getobjfin(buf, (SavableClass *)ptr);
        }
        Stroka str = CheckReadNullObj(sc);
        if(str.length() == 0)
            return getobjfin(buf, NULL);

        if(refmask == SavableClass::SingleFileStorage) {
            int numobj;
//            get(StdKey);
            verify(Stroka("ReferenceClass_{") == StdKey.name, string("Got:") + StdKey.name);
            if(GetSCStored(numobj))
                return getobjfin(buf, (SavableClass *)refsaved[numobj - 1]);
            //str.read_token(*this);// pot error !!!!
            str = getword();
        }
        ConstructObject(sc, str.c_str());
        if(refmask == SavableClass::SingleFileStorage)
            refsaved[numrefsaved - 1] = (void *)sc;
        GetObjectBody(sc);
        return getobjfin(buf, sc);
    } catch(stdexception &ex) {
        throw info_except(
                "Got an error %s.\n %s",
                ex.what(),
                ReadingError(buf.GetSavableBase()).c_str());
    }
};







Stroka SavableClass::list_all_classes(const char *categ) {
    Stroka res = "Listing all classes ";
    if(categ != NULL)
        res += Stroka("for category <") + categ + "> :\n";
    else
        res += ":\n";
    for(ClassDesc *desc = ClassDesc::list_category_start(categ); desc;
        desc = ClassDesc::list_category_next(desc, categ)) {
        if((categ == NULL) || (Stricmp(categ, desc->category()) == 0))
            res += Stroka("class ") + desc->name() + "\n";
    }
    return res;
}
Stroka SavableClass::HelpForCategory(const char *Categ, int Method) {
    Stroka HelpStr;
    HelpStr += "\n===============================================================\n";
    HelpStr += Stroka("Category <") + Categ + "> \n" + list_all_classes(Categ) +
               "\ndetailed help for each of them:\n";

    for(ClassDesc *desc = ClassDesc::list_category_start(Categ); desc;
        desc = ClassDesc::list_category_next(desc, Categ)) {
        Ref<SavableClass> obj;
        Ref<DescribedClass> cl = desc->create();
        obj << cl;
        if(!obj && Categ != NULL)
            throw(info_except(
                "Wau class <%s> was not converted to savable. It was created <%i>?\n",
                desc->name(),
                cl != NULL));
        if(!obj)
            continue;
        HelpStr += "---------------------------------------------\n";
        HelpStr += Stroka(" Config for ") + obj->class_name() + ":\n   ";
        HelpStr += object2string(obj, Method) + "\n";
        Stroka str = obj->MakeHelp();
        if(str.length() != 0)
            HelpStr += Stroka("\nClass provides help:\n") + obj->MakeHelp() + "\n";
        HelpStr += Stroka("---------------------------------------------\n");
    }
    HelpStr += "End of help for category <" + Stroka(Categ) + ">\n";
    HelpStr += "===============================================================\n";
    return HelpStr;
}

