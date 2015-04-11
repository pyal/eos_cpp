
#ifndef NamedObject_h
#define NamedObject_h 1

#include "SparceArray.h"

struct NamedObject;
struct NamedStore;
struct NamedObjectStoreAbstract
 {
  virtual void DeleteObject(NamedObject *obj)=0;
  virtual int Add (NamedObject* dat, int pos = -1)=0; 
 };

// Copying, looses information about storage. Have to add to store manually.
// Local objects - unmanage - or double deleting will occur, after storing in store.
struct NamedObject : public SavableClass  //## Inherits: <unnamed>%3EF6CA430254
{

   NamedObject(char *obj_name_=NULL,SavableClass *store=NULL,int store_num=-1,int UmanagePtrForLocal=0):obj_name(0),StoredBy(0),StoredByNum(-1),ObjectStored(0)
    {if (!obj_name_) set_obj_name(".");else set_obj_name(obj_name_);
     StoredBy=store;NamedObjectStoreAbstract *tmp=get_store();
     if (tmp) {tmp->Add(this,store_num);};if (UmanagePtrForLocal) unmanage();}
   virtual ~NamedObject()
    {
     remove_from_store();
     delete obj_name;obj_name=NULL;
    };

   NamedObject(const NamedObject &right):obj_name(NULL),StoredBy(0),StoredByNum(-1),ObjectStored(0)
    {set_obj_name(right.obj_name);};//put_in_store(right.StoredBy);
   
   virtual int save_data_state (FilterOut &so){if (!obj_name) set_obj_name(".");so<<KeyDesc("ObjectName")<<obj_name<<KeyDesc("StoredBy")<<StoredBy<<KeyDesc("StoredByNum")<<StoredByNum;return !(!so);}
   virtual int read_data_state (FilterIn &si) {char tmp[256];si>>StdKey>>tmp>>StdKey>>StoredBy>>StdKey>>StoredByNum;set_obj_name(tmp);return 1;}

// =====================================  To be redefined =====================================   
// To create a copy on new store
   virtual NamedObject* CreateCopyOnStore(SavableClass* newStor,int newNum=-2){NamedObject* ret=new NamedObject(*this);if (newNum==-2) newNum=StoredByNum;ret->put_in_store(newStor,newNum);return ret;}
// To be Called each time var is setted to the new store   
   virtual void StoreChanged(){};


   inline void set_obj_name (char* value)  {delete obj_name;obj_name=NULL;if (value==NULL) return;obj_name=new char[strlen(value)+1];strcpy(obj_name,value);}
   inline const char* get_obj_name () const{return obj_name==0?"":obj_name;}

   inline const int get_StoredByNum () const{return StoredByNum;};
   inline SavableClass* get_StoredBy () const
    {return StoredBy;
    }

 
   inline void remove_from_store(int CallChanges=1) 
    {
     NamedObjectStoreAbstract *tmp=get_store();
     if (tmp)  
      { 
       tmp->DeleteObject(this);
       StoredBy=NULL;
       StoredByNum=-1;
       if (CallChanges) StoreChanged();
      }
    };
// inc reference before changing store otherwise - object will be lost, deleted.
   inline int put_in_store(SavableClass *stor,int store_num=-1){this->reference();remove_from_store(0);StoredBy=stor;if (get_store()) get_store()->Add(this,store_num);this->dereference();StoreChanged();return StoredByNum;}

 private:
   inline NamedObjectStoreAbstract* get_store(){ return dynamic_cast<NamedObjectStoreAbstract*>(StoredBy);}
 protected:
// To be used by NamedStore
   inline void set_StoredByNum (int value)  {StoredByNum=value;}
   inline void set_store(SavableClass *stor,int store_num=-1) 
    {
     this->reference();
     remove_from_store(0);
     StoredBy=stor;StoredByNum=store_num;
     this->dereference();
     StoreChanged();
    }
   inline void set_not_stored() {if (StoredBy==NULL) return; StoredBy=NULL;StoredByNum=-1;StoreChanged();};
   inline void force_store(SavableClass *stor,int store_num=-1) {StoredBy=stor;StoredByNum=store_num;}

 private: //## implementation
   char* obj_name;
   SavableClass* StoredBy;
   int StoredByNum,ObjectStored;

   friend struct NamedStore;
};

#define NAMED_SPARCE this->SparceArray<Ref<NamedObject>,CopyStructRef<Ref<NamedObject> > >
// DefaultStorage - to be used in construction
// struct AAA:SavabeClass,NamedObjectStoreAbstract {NamedStore Vars;}
struct NamedStore : public SparceArray<Ref<NamedObject>,CopyStructRef<Ref<NamedObject> > >,
                    public NamedObjectStoreAbstract  //## Inherits: <unnamed>%3EF7566D0173
 {
  public:
   NamedStore(SavableClass *def=NULL){Ref<NamedObject> no_=NULL;SetEmpty(&no_);DefaultStorage=((def==NULL)?this:def);}
   NamedStore(const NamedStore &right){DefaultStorage=this;this->operator=(right);}
   ~NamedStore(){};

   NamedStore & operator=(const NamedStore &right);

   inline NamedObject* GetPtr(int i)                           
    {if (IsEmpty(i)) 
      {i=NAMED_SPARCE::Add(i);
       if (NamedObject *tmp=NAMED_SPARCE::operator[](i)) tmp->set_store(DefaultStorage,i);
      }
     return NAMED_SPARCE::operator[](i).pointer();
    };
   inline int SetPtr(NamedObject* obj)
    {
     if (obj==NULL) return -1;
     if (obj->get_StoredBy()==DefaultStorage) 
      {
       int StoreNum=obj->get_StoredByNum();
       if (obj!=GetPtr(StoreNum)) {fcout<<" NamedStore::SetPtr To different objects in one point\n";abort();}
       return StoreNum;
      }
     else return Add(obj);
    };
//   inline Ref<NamedObject>& operator [] (int i)                 {if (IsEmpty(i)) {i=NAMED_SPARCE::Add(i);if (NAMED_SPARCE::operator[](i)) NAMED_SPARCE::operator[](i)->set_store(DefaultStorage,i);}return NAMED_SPARCE::operator[](i);}
   virtual inline int Add (Ref<NamedObject> &dat, int pos = -1) 
    {
     if (pos>=0) {if (!IsEmpty(pos)) {if (NamedObject *tmp=GetPtr(pos)) tmp->set_not_stored();NAMED_SPARCE::Del(pos);}}
     else if ((dat)&&(dat->get_StoredBy()==DefaultStorage)) return dat->get_StoredByNum();
     int i=NAMED_SPARCE::Add(dat,pos);
     if (dat) dat->set_store(DefaultStorage,i);
     return i;
    }
   virtual inline void Del (int pos)                            
    {
     if (!IsEmpty(pos)) 
      {if (operator[](pos)) operator[](pos)->set_not_stored();NAMED_SPARCE::Del(pos);}
    }
   virtual inline void Move (int frpos, int topos)              
    {NAMED_SPARCE::Add(NAMED_SPARCE::operator[](frpos),topos);NAMED_SPARCE::Del(frpos);
     GetPtr(topos)->set_StoredByNum(topos);}
   virtual int save_data_state (FilterOut& so){NAMED_SPARCE::save_data_state(so);so<<KeyDesc("DefStorage")<<DefaultStorage;return 1;};
   virtual int read_data_state (FilterIn& si) {NAMED_SPARCE::read_data_state(si);si>>StdKey>>DefaultStorage;return 1;};

   inline void set_DefaultStorage(SavableClass *def)
    {DefaultStorage=def;SparceArrayInd* ind_p=GetInd ();int cur=-1;
     while ((cur=ind_p->Next(cur))>=0) operator[](cur)->force_store(DefaultStorage,cur);};
   virtual int Name2Pos(char *name);                       
// from NamedObjectStoreAbstract
   virtual void DeleteObject(NamedObject *obj)      { if (!obj) return;Del(obj->get_StoredByNum()); };
   virtual inline int Add (NamedObject* dat, int pos = -1) {Ref<NamedObject> tmp=dat;return Add(tmp,pos);}
   virtual void StoreChanged();
  protected:
   SavableClass *DefaultStorage;
//   inline Ref<NamedObject>& operator [] (int i)                 {fcout<<"Error NamedStore::operator []";abort();}
   inline Ref<NamedObject>& operator [] (int i)                 {if (IsEmpty(i)) {i=NAMED_SPARCE::Add(i);if (NAMED_SPARCE::operator[](i)) NAMED_SPARCE::operator[](i)->set_store(DefaultStorage,i);}return NAMED_SPARCE::operator[](i);}
 };

struct NamedObjectInited:public NamedObject
 { virtual void Init(){};};



#endif




















/*
struct NamedRegistry : public SavableClass  //## Inherits: <unnamed>%3EF756E0010B
{
      NamedRegistry();
      NamedRegistry(const NamedRegistry &right);
      NamedRegistry (char *name, char* cl_name, char* categor = NULL, char *descr = NULL, void(*print)(FilterOut&) = NULL);
      ~NamedRegistry();

      NamedRegistry & operator=(const NamedRegistry &right);


      void init (char *name, char* cl_name, char* categor = NULL, char *descr = NULL, void(*print)(FilterOut&)  = NULL);
      NamedRegistry* name_to_reg_data (void char_Xname);
      void list_all_names (char *categor = NULL);
      void print_all_help (char *categor = NULL);
      int check_registered (char* name, DescribedClass* cl = NULL);
      char * get_name (DescribedClass* cl, char *categor = NULL, int check_unic = 0);
      char* obj_name ();
      char* obj_descr ();
      char* category ();
      char* obj_classname ();

  public:
  protected:
  private:
      static const std::map* get_all_ ();
      static void set_all_ (std::map* value);

      const char * get_obj_name_ () const;
      void set_obj_name_ (char * value);

      const char * get_obj_descr_ () const;
      void set_obj_descr_ (char * value);

      const char * get_obj_classname_ () const;
      void set_obj_classname_ (char * value);

      const char * get_obj_category_ () const;
      void set_obj_category_ (char * value);

      const void(*)(FilterOut&) get_obj_print_ () const;
      void set_obj_print_ (void(*)(FilterOut&) value);

  private: //## implementation
      static std::map* all_;
      char *obj_name_;
      char *obj_descr_;
      char *obj_classname_;
      char *obj_category_;
      void(*obj_print_)(FilterOut&);

};



// Class NamedObject 

//## Get and Set Operations for Class Attributes (inline)

inline const char* NamedObject::get_obj_name () const
{
  //## begin NamedObject::get_obj_name%3EF6C4B003D9.get preserve=no
  return obj_name;
  //## end NamedObject::get_obj_name%3EF6C4B003D9.get
}

inline void NamedObject::set_obj_name (char* value)
{
  //## begin NamedObject::set_obj_name%3EF6C4B003D9.set preserve=no
  obj_name = value;
  //## end NamedObject::set_obj_name%3EF6C4B003D9.set
}

inline const SavableClass* NamedObject::get_StoredBy () const
{
  //## begin NamedObject::get_StoredBy%3EF6CA620208.get preserve=no
  return StoredBy;
  //## end NamedObject::get_StoredBy%3EF6CA620208.get
}

inline void NamedObject::set_StoredBy (SavableClass* value)
{
  //## begin NamedObject::set_StoredBy%3EF6CA620208.set preserve=no
  StoredBy = value;
  //## end NamedObject::set_StoredBy%3EF6CA620208.set
}

inline const int NamedObject::get_StoredByNum () const
{
  //## begin NamedObject::get_StoredByNum%3EF768F00290.get preserve=no
  return StoredByNum;
  //## end NamedObject::get_StoredByNum%3EF768F00290.get
}

inline void NamedObject::set_StoredByNum (int value)
{
  //## begin NamedObject::set_StoredByNum%3EF768F00290.set preserve=no
  StoredByNum = value;
  //## end NamedObject::set_StoredByNum%3EF768F00290.set
}

// Class NamedStore 

//## Get and Set Operations for Class Attributes (inline)

inline const SparceArray<NamedObject> NamedStore::get_stor () const
{
  //## begin NamedStore::get_stor%3EF74322026C.get preserve=no
  return stor;
  //## end NamedStore::get_stor%3EF74322026C.get
}

inline void NamedStore::set_stor (SparceArray<NamedObject> value)
{
  //## begin NamedStore::set_stor%3EF74322026C.set preserve=no
  stor = value;
  //## end NamedStore::set_stor%3EF74322026C.set
}

// Class NamedRegistry 

//## Get and Set Operations for Class Attributes (inline)

inline const std::map* NamedRegistry::get_all_ ()
{
  //## begin NamedRegistry::get_all_%3EF758F903CA.get preserve=no
  return all_;
  //## end NamedRegistry::get_all_%3EF758F903CA.get
}

inline void NamedRegistry::set_all_ (std::map* value)
{
  //## begin NamedRegistry::set_all_%3EF758F903CA.set preserve=no
  all_ = value;
  //## end NamedRegistry::set_all_%3EF758F903CA.set
}

inline const char * NamedRegistry::get_obj_name_ () const
{
  //## begin NamedRegistry::get_obj_name_%3EF759680120.get preserve=no
  return obj_name_;
  //## end NamedRegistry::get_obj_name_%3EF759680120.get
}

inline void NamedRegistry::set_obj_name_ (char * value)
{
  //## begin NamedRegistry::set_obj_name_%3EF759680120.set preserve=no
  obj_name_ = value;
  //## end NamedRegistry::set_obj_name_%3EF759680120.set
}

inline const char * NamedRegistry::get_obj_descr_ () const
{
  //## begin NamedRegistry::get_obj_descr_%3EF7597900A3.get preserve=no
  return obj_descr_;
  //## end NamedRegistry::get_obj_descr_%3EF7597900A3.get
}

inline void NamedRegistry::set_obj_descr_ (char * value)
{
  //## begin NamedRegistry::set_obj_descr_%3EF7597900A3.set preserve=no
  obj_descr_ = value;
  //## end NamedRegistry::set_obj_descr_%3EF7597900A3.set
}

inline const char * NamedRegistry::get_obj_classname_ () const
{
  //## begin NamedRegistry::get_obj_classname_%3EF7633603E7.get preserve=no
  return obj_classname_;
  //## end NamedRegistry::get_obj_classname_%3EF7633603E7.get
}

inline void NamedRegistry::set_obj_classname_ (char * value)
{
  //## begin NamedRegistry::set_obj_classname_%3EF7633603E7.set preserve=no
  obj_classname_ = value;
  //## end NamedRegistry::set_obj_classname_%3EF7633603E7.set
}

inline const char * NamedRegistry::get_obj_category_ () const
{
  //## begin NamedRegistry::get_obj_category_%3EF76382004D.get preserve=no
  return obj_category_;
  //## end NamedRegistry::get_obj_category_%3EF76382004D.get
}

inline void NamedRegistry::set_obj_category_ (char * value)
{
  //## begin NamedRegistry::set_obj_category_%3EF76382004D.set preserve=no
  obj_category_ = value;
  //## end NamedRegistry::set_obj_category_%3EF76382004D.set
}

inline const void(*)(FilterOut&) NamedRegistry::get_obj_print_ () const
{
  //## begin NamedRegistry::get_obj_print_%3EF993F500A8.get preserve=no
  return obj_print_;
  //## end NamedRegistry::get_obj_print_%3EF993F500A8.get
}

inline void NamedRegistry::set_obj_print_ (void(*)(FilterOut&) value)
{
  //## begin NamedRegistry::set_obj_print_%3EF993F500A8.set preserve=no
  obj_print_ = value;
  //## end NamedRegistry::set_obj_print_%3EF993F500A8.set
}

*/

