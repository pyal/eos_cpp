
#ifndef NamedRegistry_h
#define NamedRegistry_h 1


struct NamedObject;


struct NamedRegistry : public SavableClass  //## Inherits: <unnamed>%3EF756E0010B
{
      NamedRegistry();
//      NamedRegistry(const NamedRegistry &right);
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



#endif




















/*
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

