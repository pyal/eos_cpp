#include <lib\precompiled\Ref.h>

//#include <stdlib.h>
//#include <sys/time.h>
//#include <unistd.h>

#include "class_sav.h"
#include "exenv.h"
#include "class.h"

//#include "stateio.h"


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

//std::map<char* , DataSource::DataSourceRef> *DataSource::data_list=NULL;
DataSource::MAPCLASS *DataSource::data_list=NULL;

static ClassDesc SavableClass_cd(typeid(SavableClass),"SavableClass");

//char KeyDesc::name[256];
//SavableClass::SavableClass(StateIn&si)
//int SavableClass::save_state(SavableClass *sc, FilterOut&so){return 1;};// return so.setobject(sc);};
// Restores objects saved with save_state.  The exact type of the next object in si can 
// be any type publically derived from the SavableState. Derived classes implement a 
// similar static function that returns a pointer to the derived class.  
//int SavableClass::restore_state(SavableClass *&sc,FilterIn& si){return 1;}// return si.getobject(sc);};

void SavableClass::save_data_post(FilterOut&so){DecIdent();so<<KeyDesc(EOLN())<<(const char *)"}"<<KeyDesc(EOLN()); };
void SavableClass::save_data_pref(FilterOut&so){so.setword("{");IncIdent();so<<KeyDesc(EOLN()); };
//void SavableClass::save_data_post(FilterOut&so){DecIdent();so<<(const char *)"}"<<KeyDesc(EOLN()); };
//void SavableClass::save_data_pref(FilterOut&so){so.setword("{");IncIdent(); };
int SavableClass::read_data_post(FilterIn&si){ExeptionCheck(si,"}");return 1;}
int SavableClass::read_data_pref(FilterIn&si){ExeptionCheck(si,"{");return 1;}
                                              //return (stricmp("{",tmp)==0);};

//SavableClass& SavableClass::operator=( SavableClass &from){
////    if (strcmp(from.class_name(),this->class_name())!=0)
////       throw info_except("Classes have different names: <%s>!=<%s>\n",this->class_name(), from.class_name());
////    Stroka Unic = DataSource::GenerateUnicName("CopySavable");
////    FilterTextOut fo(Unic.c_str(),DataSource::Memory,"CopySavable_Cat",0);
////    //FilterBinOut fo(Unic.c_str(),DataSource::Memory,"CopySavable_Cat",0);
////    fo.SetRefMask(GetRefOutMethod());
////    fo.setobject(from);
////    FilterTextIn fi(Unic.c_str(),DataSource::Memory,"CopySavable_Cat");
////    //FilterBinIn fi(Unic.c_str(),DataSource::Memory,"CopySavable_Cat");
////    fi.SetRefMask(GetRefOutMethod());
////    fi.getobject(*this);
//////      fi.CloseBuf();
////    fo.CloseBuf();
//    LoadClass(from);
//    return *this;
//};
void SavableClass::LoadClass( SavableClass &from, int method){
    if (strcmp(from.class_name(),this->class_name())!=0)
       throw info_except("Classes have different names: <%s>!=<%s>\n",this->class_name(), from.class_name());
    strstreambuf out;
    FilterTextOut fo(&out, 0);
    if (method == -1)
        method = GetRefOutMethod();
    fo.SetRefMask(method);
    fo.setobject(from);
    FilterTextIn fi(&out, 0);
    fi.SetRefMask(method);
    fi.getobject(*this);
    fo.CloseBuf();


//    if (strcmp(from.class_name(),this->class_name())!=0)
//       throw info_except("Classes have different names: <%s>!=<%s>\n",this->class_name(), from.class_name());
//    Stroka Unic = DataSource::GenerateUnicName("CopySavable");
//    FilterTextOut fo(Unic.c_str(),DataSource::Memory,"CopySavable_Cat",0);
//    //FilterBinOut fo(Unic.c_str(),DataSource::Memory,"CopySavable_Cat",0);
//    fo.SetRefMask(GetRefOutMethod());
//    fo.setobject(from);
//    FilterTextIn fi(Unic.c_str(),DataSource::Memory,"CopySavable_Cat");
//    //FilterBinIn fi(Unic.c_str(),DataSource::Memory,"CopySavable_Cat");
//    fi.SetRefMask(GetRefOutMethod());
////cout<<SavableClass::ShowStreamPosition(&fi);
//    fi.getobject(*this);
////      fi.CloseBuf();
//    fo.CloseBuf();
};
SavableClass *SavableClass::Duplicate(int Method) {

    return SavableClass::string2object(SavableClass::object2string(this, Method), Method);

    //Stroka Unic = DataSource::GenerateUnicName("DuplicateSavable");
    //FilterTextOut fo(Unic.c_str(),DataSource::Memory,"DuplicateSavable_Cat",0, NULL);
    //if (Method==-1)
    //    Method = GetRefOutMethod();
    //fo.SetRefMask(Method);
    //fo.setobject(this);
    //FilterTextIn fi(Unic.c_str(),DataSource::Memory,"DuplicateSavable_Cat");
    //fi.SetRefMask(Method);
    //SavableClass *ret = fi.getobject();
    //fo.CloseBuf();
    //return ret;
}

//
//SavableClass* SavableClass::CreateCopyPtr(int DeepCopy)
//  {
//   int CopyMeth=(DeepCopy)?SavableClass::SingleFileStorage:SavableClass::StorePtr;
//   const char *name=this->class_name();
//   SavableClass *sc; 
//   ClassDesc* cd=ClassDesc::name_to_class_desc(name);
//   if (cd) 
//       sc=dynamic_cast<SavableClass*>(cd->create());
//   else 
//       throw info_except("ERROR: SavableClass::CreateNewPtr: class <%s> unknown\n",name);
//   FilterBinOut fo("CopySavable",DataSource::Memory,"CreateCopySavable_Cat",0);
//   fo.SetRefMask(CopyMeth);
//   fo.setobject(*this);
//   FilterBinIn fi("CopySavable",DataSource::Memory,"CreateCopySavable_Cat");
//   fi.SetRefMask(CopyMeth);
//   fi.getobject(sc);
//   fo.CloseBuf();
//   return sc;
//  };

// SavableClass *SavableClass::Read(istream &in){
//    if (!in) throw info_except(" bad stream\n ");
//	char name[256];in>>name;
//	ClassDesc* descr=ClassDesc::name_to_class_desc(name);
//	SavableClass *Func=0;
//	if (!descr) throw info_except(" Could not register Class %s\n ",name);
//	Func=dynamic_cast<SavableClass*>(descr->create());
//	if (!Func) throw info_except(" Class %s is not of SavableClass type\n ",name);
//	FilterTextIn input(in.rdbuf(),0);
//	Func->read_data_state(input);
//    if (!in) throw info_except(" Could not read class %s\n ", name);
//	return Func;
//}
// int SavableClass::Save(ostream &out, SavableClass *Clas){
//	if  (!out) 
//	    throw info_except(" bad stream\n ");
//	if  (!Clas) 
//	    throw info_except(" bad class\n ");
//	out<<Clas->class_name()<<"  "; 
//	FilterTextOut output(out.rdbuf(),0);
//	Clas->save_data_state(output);
//	return !(!out);
//}
  Stroka SavableClass::ReadingError(istream &in, SavableClass *s){
      Stroka ret("Could not read class:");
      if (!s)
          ret += "NULL";
      else
          ret += s->class_name();
      if (!in)
          in.clear();
      char tmp[512], tmp1[512];
      in.read(tmp1,512);tmp1[511]=0;
      ret += Stroka("\nError position: ") + itoa(in.tellg(),tmp,10) + "\nChars after error\n"+ tmp1 + "\n";
      return ret;
  };
 SavableClass *SavableClass::Read(istream &in, int Method){
    SavableClass *Func=0;
    if (!in) 
        throw info_except(" bad stream\n %s\n",ReadingError(in, Func).c_str());
    if (Method==-1)
        Method = GetRefOutMethod();
	Ref<SavableClass> cl;
    FilterTextIn input(in.rdbuf(),0);input.SetRefMask(Method);//SingleFileStorage);
    //input>>cl;
    input.getobject(cl);
	return cl.release_temp_settled_ptr();

}
 int SavableClass::Save(ostream &out, SavableClass *Clas, int Method){
	if  (!out) 
	    throw info_except(" bad out stream\n ");
    if (Method==-1)
        Method = GetRefOutMethod();
	Ref<SavableClass> cl(Clas);
	FilterTextOut output(out.rdbuf(),0);output.SetRefMask(Method);//SingleFileStorage);
	output<<cl;
    cl.release_temp_settled_ptr();
	return !(!out);
}
void SavableClass::Read(istream &in, SavableClass &Clas, int Method){
     try {
        if (!in) 
            throw info_except(" bad stream\n ");
        if (Method==-1)
            Method = GetRefOutMethod();
        FilterTextIn input(in.rdbuf(),0);input.SetRefMask(Method);//SingleFileStorage);
	    //input>>Clas;
        input.getobject(Clas);
        if (!in) 
            throw info_except(" Could not read class %s\n ", Clas.class_name());
     //} catch(stdexception &ex){
     } catch(exception &ex){
         throw info_except("Got an error %s.\n %s",ex.what(), ReadingError(in, &Clas).c_str());
     }

}
 int SavableClass::Save(ostream &out, SavableClass &Clas, int Method){
	if  (!out) 
	    throw info_except(" bad out stream\n ");
    if (Method==-1)
        Method = GetRefOutMethod();
    FilterTextOut output(out.rdbuf(),0);output.SetRefMask(Method);//SingleFileStorage);
	output<<Clas;
	return !(!out);
}

char SavableClass::SingleIdent[10] = "   ";
int SavableClass::CurLevel = 0, SavableClass::OutMethod = SimpleEdit;
Stroka SavableClass::IdentStr="";

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/*
void* AllocatorIntelect::Allocate(long size)
  {
//ut<<" Last "<<LastAllocationSize<<" size "<<size<<"\n"<<flush;
   if (LastAllocationSize>=size) return LastAllocationPtr;
   long new_s=LastAllocationSize;
   while (new_s<=size) 
     {double tmp=new_s*1.3;
//cout<<" tmp Add "<<int(tmp)<<"\n"<<flush;
      new_s=tmp+StartAllocationSize;
     }
   char *new_p=new char[new_s];
   memmove(new_p,LastAllocationPtr,LastAllocationSize);
   delete LastAllocationPtr;
   LastAllocationPtr=new_p;LastAllocationSize=new_s;
//cout<<" Last AA "<<LastAllocationSize<<" size "<<size<<"\n"<<flush;
   return LastAllocationPtr;
  }
*/
DataSource::~DataSource()
  {
   if (databuf==NULL) return;
//cout<<"Deleting DataSource\n";print();cout.flush();
   switch ( datatype )
     {
      case Memory: delete databuf;break;
      case Disk:   {filebuf *fbuf=(filebuf *)databuf;fbuf->close();delete databuf;} break;
      case Console:delete databuf;break;
      default:cout<<"Error in DataSource ~DataSource switch : dat_type - is not known/n";
              exit(0);
     }
   databuf=NULL;
 };

int DataSource::DuplicatePresent(const char *name,Ref<DataSource> &ref)
{
  MAPCLASS::iterator itr= data_list->find(name);
  if (itr==data_list->end()) 
      return 0;
  ref = itr->second;
  return 1;
}

streambuf* DataSource::OpenSource(const char *name,OpenMode op_mode,DataType dat_type,char *category,const char *file_name)
  {
   if (!data_list) data_list=new MAPCLASS;
   Ref<DataSource> dref=new DataSource;
   dref->openmode=op_mode;dref->datatype=dat_type;
   dref->Category[0]=0;
   if (category!=NULL) strcpy(dref->Category,category);
   strcpy(dref->dataname,name);dref->databuf=NULL;

   Ref<DataSource> wasref;
   if (DuplicatePresent(name,wasref)) 
     if (dat_type != Memory || op_mode != In) {
         cout<<" DataSource::OpenSource - duplicate_name. Will close previous:\n";
         wasref->print();
     } else;
   else if (dat_type==Memory && op_mode==In) 
      {cout<<" DataSource::OpenSource - trying to open not existing Memory Buf:\n";dref->print();exit(0);}

   switch ( dat_type )
    {
     case Memory: if (op_mode==Out) dref->databuf=new memstreambuf();
                  else 
                    { 
                     if (wasref->datatype==Memory)
                        {
                         memstreambuf *wasbuf=(memstreambuf *)wasref->databuf;
                         wasbuf->SetGet();
                         dref->databuf=wasbuf;wasref->databuf=NULL;
// Insertion will automaticcaly delete old Data
                        }
                     else
                        {cout<<" DataSource::OpenSource - trying to open Memory Buf: ";
                         cout<<" with wrong type\n"; dref->print();exit(0);}
                    }
                  break;
     case Disk:   {filebuf *fbuf=new filebuf;
					const char *open_name = file_name?file_name:name;
					
                  if (op_mode==In) dref->databuf=fbuf->open(open_name,ios::in);
                  else dref->databuf=fbuf->open(open_name,ios::out);
                  if (dref->databuf==NULL) delete fbuf;}
                  break;
     case Console:if (op_mode==In) dref->databuf=new filebuf(stdin);//stdiobuf(stdin);
                  else dref->databuf=new filebuf(stdout);//stdiobuf(stdout);
                  break;
     default:cout<<"Error in DataSource OpenSource switch : dat_type - is not known/n";
             exit(0);
    }
   if (dref->databuf==NULL) return NULL;//dref=NULL;
   (*data_list)[dref->dataname]=dref;
   return dref->databuf;
  };

int DataSource::NumUnicObjects = 0;
Stroka DataSource::GenerateUnicName(const char *basic){
	Stroka Ret = basic?basic:"";
    //Ret += Stroka::Int2Str((int)RandomClass::Rnd(1e8));
    Ret += "_" + Stroka::Int2Str(NumUnicObjects++);
cout<<"Unic name rnd "<<Ret<<"\n";
	return Ret;
}

void DataSource::CloseCategories(char *category)
 {
  if (data_list==NULL) return;
  Ref<DataSource> refd;
  for (MAPCLASS::iterator ind=data_list->begin();ind!=data_list->end(); ) 
     {
      if ((category==NULL) || (stricmp(category,ind->second->Category)==0)) 
         ind=data_list->erase(ind);
      else ind++;
     }
  if (data_list->size()==0) { delete data_list;data_list=NULL;}
  else if (category==NULL) { cout<<" Very bad. Not all DataSource::CloseCategories, have to be all\n";}
 }

void DataSource::CloseSource(char *name){if (data_list==NULL) return;data_list->erase(name);};

void DataSource::CloseSource(streambuf *buf)
 {
  if (data_list==NULL) return;
  Ref<DataSource> refd;
  for (MAPCLASS::iterator ind=data_list->begin(); ind!=data_list->end();) 
     {
      if (ind->second->databuf==buf)
      { 
//cout<<" CloseBuf size "<<data_list->size();
//cout<<" "<<ind->second->dataname;
        ind=data_list->erase(ind);
//cout<<" "<<data_list->size()<<"\n"<<flush;
      }
      else ind++;
     }
 };


//int FilterTextIn::getobject(SavableClass *&sc);



//void ClassDesc::WriteHelpOnCategory(FilterOut &out,char *name,int PrintIt)
// {
//  out<<"=================================================================\n";
//  out<<"Category "<<name<<"consists of objects \n";
//  out<<"=================================================================\n";
//  out.flush();
//  Ref<SavableClass> obj;
//  Ref<DescribedClass> dsc;
////  int OldMask=out.SetRefMask();out.SetRefMask(SavableClass::SingleFileStorage);
//  for (std::map<ClassKey,ClassDescP>::iterator ind=ClassDesc::all_->begin();
//       ind!=ClassDesc::all_->end(); ind++) {
//      ClassDesc* classdesc = ind->second;
//      if (stricmp(classdesc->category(),name)==0) { 
//		  dsc = classdesc->create();
//          obj=dynamic_cast<SavableClass*> ((DescribedClass*)dsc);
//          if (obj && PrintIt&PrintObj){ 
//                 //obj->SetRefMask(SavableClass::SingleFileStorage);
////                 obj->SetRefMask(SavableClass::UnlinkedObjects);
////                 obj->SetRefMask(SavableClass::StorePtr);
//                 //Ref<SavableClass> ref = obj;
//                 //out<<ref<<"\n";
//                 out<<obj<<"\n";
//		  }
//          if (dsc && PrintIt&Print) 
//                 dsc->print();
//          if (dsc && PrintIt&PrintHelp) 
//                 out<<dsc->print_help().c_str()<<"\n";
//
//      }
//    }
//
//  out<<"=================================================================\n";
//  out<<"End of Category "<<name<<"objects \n";
//  out<<"=================================================================\n";
//  out.flush();
//
////  out.SetRefMask(OldMask);out.ClearRef();
//
// }


Stroka SavableClass::list_all_classes(const char *categ){
    Stroka res = "Listing all classes ";
    if (categ !=NULL)
        res += Stroka("for category <") + categ + "> :\n";
    else
        res += ":\n";
    for (ClassDesc *desc = ClassDesc::list_category_start(categ);desc;desc = ClassDesc::list_category_next(desc,categ)){
	    if ( (categ==NULL) || (Stricmp(categ,desc->category())==0) )
            res += Stroka("class ") + desc->name() + "\n";
    }
    return res;
}
Stroka SavableClass::HelpForCategory(const char *Categ, int Method){
    Stroka HelpStr;
    HelpStr += "\n===============================================================\n";
    HelpStr += Stroka("Category <") + Categ + "> \n" + list_all_classes(Categ) + "\ndetailed help for each of them:\n" ;

    for (ClassDesc *desc = ClassDesc::list_category_start(Categ);desc;desc = ClassDesc::list_category_next(desc,Categ)){
        Ref<SavableClass> obj ;
        Ref<DescribedClass> cl = desc->create();
        obj << cl;
        if (!obj && Categ != NULL) 
            throw(info_except("Wau class <%s> was not converted to savable. It was created <%i>?\n",desc->name(),
                cl!=NULL));
        if (!obj)
            continue;
        HelpStr += "---------------------------------------------\n";
        HelpStr += Stroka(" Config for ") + obj->class_name() + ":\n   ";
        HelpStr += object2string(obj, Method) + "\n";
        Stroka str = obj->MakeHelp();
        if (str.length()!=0)
            HelpStr += Stroka("\nClass provides help:\n") + obj->MakeHelp() + "\n";
        HelpStr += Stroka("---------------------------------------------\n");
    }
    HelpStr += "End of help for category <" + Stroka(Categ) + ">\n";
    HelpStr += "===============================================================\n";
    return HelpStr;
}

 
