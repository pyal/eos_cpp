
// NamedObject
#include "NamedRegistry.h"


NamedObject::NamedObject() {}

NamedObject::NamedObject(const NamedObject &right)
//## begin NamedObject::NamedObject%3EF6C4580378_copy.initialization preserve=yes
//## end NamedObject::NamedObject%3EF6C4580378_copy.initialization
{
    //## begin NamedObject::NamedObject%3EF6C4580378_copy.body preserve=yes
    //## end NamedObject::NamedObject%3EF6C4580378_copy.body
}


NamedObject::~NamedObject() {
    //## begin NamedObject::~NamedObject%3EF6C4580378_dest.body preserve=yes
    //## end NamedObject::~NamedObject%3EF6C4580378_dest.body
}


NamedObject &NamedObject::operator=(const NamedObject &right) {
    //## begin NamedObject::operator=%3EF6C4580378_assign.body preserve=yes
    //## end NamedObject::operator=%3EF6C4580378_assign.body
}


int NamedObject::operator==(const NamedObject &right) const {
    //## begin NamedObject::operator==%3EF6C4580378_eq.body preserve=yes
    //## end NamedObject::operator==%3EF6C4580378_eq.body
}

int NamedObject::operator!=(const NamedObject &right) const {
    //## begin NamedObject::operator!=%3EF6C4580378_neq.body preserve=yes
    //## end NamedObject::operator!=%3EF6C4580378_neq.body
}


// Class NamedStore


NamedStore::NamedStore()
//## begin NamedStore::NamedStore%3EF6CAF901E7_const.initialization preserve=yes
//## end NamedStore::NamedStore%3EF6CAF901E7_const.initialization
{
    //## begin NamedStore::NamedStore%3EF6CAF901E7_const.body preserve=yes
    //## end NamedStore::NamedStore%3EF6CAF901E7_const.body
}

NamedStore::NamedStore(const NamedStore &right)
//## begin NamedStore::NamedStore%3EF6CAF901E7_copy.initialization preserve=yes
//## end NamedStore::NamedStore%3EF6CAF901E7_copy.initialization
{
    //## begin NamedStore::NamedStore%3EF6CAF901E7_copy.body preserve=yes
    //## end NamedStore::NamedStore%3EF6CAF901E7_copy.body
}


NamedStore::~NamedStore() {
    //## begin NamedStore::~NamedStore%3EF6CAF901E7_dest.body preserve=yes
    //## end NamedStore::~NamedStore%3EF6CAF901E7_dest.body
}


NamedStore &NamedStore::operator=(const NamedStore &right) {
    //## begin NamedStore::operator=%3EF6CAF901E7_assign.body preserve=yes
    //## end NamedStore::operator=%3EF6CAF901E7_assign.body
}


int NamedStore::operator==(const NamedStore &right) const {
    //## begin NamedStore::operator==%3EF6CAF901E7_eq.body preserve=yes
    //## end NamedStore::operator==%3EF6CAF901E7_eq.body
}

int NamedStore::operator!=(const NamedStore &right) const {
    //## begin NamedStore::operator!=%3EF6CAF901E7_neq.body preserve=yes
    //## end NamedStore::operator!=%3EF6CAF901E7_neq.body
}


// Class NamedRegistry

//## begin NamedRegistry::all_%3EF758F903CA.attr preserve=no  private: static std::map* {U} 0
std::map *NamedRegistry::all_ = 0;
//## end NamedRegistry::all_%3EF758F903CA.attr


NamedRegistry::NamedRegistry()
//## begin NamedRegistry::NamedRegistry%3EF756AA02C6_const.initialization preserve=yes
//## end NamedRegistry::NamedRegistry%3EF756AA02C6_const.initialization
{
    //## begin NamedRegistry::NamedRegistry%3EF756AA02C6_const.body preserve=yes
    //## end NamedRegistry::NamedRegistry%3EF756AA02C6_const.body
}

NamedRegistry::NamedRegistry(const NamedRegistry &right)
//## begin NamedRegistry::NamedRegistry%3EF756AA02C6_copy.initialization preserve=yes
//## end NamedRegistry::NamedRegistry%3EF756AA02C6_copy.initialization
{
    //## begin NamedRegistry::NamedRegistry%3EF756AA02C6_copy.body preserve=yes
    //## end NamedRegistry::NamedRegistry%3EF756AA02C6_copy.body
}

NamedRegistry::NamedRegistry(
    char *name,
    char *cl_name,
    char *categor,
    char *descr,
    void (*print)(FilterOut &)) {}


NamedRegistry::~NamedRegistry() {
    //## begin NamedRegistry::~NamedRegistry%3EF756AA02C6_dest.body preserve=yes
    //## end NamedRegistry::~NamedRegistry%3EF756AA02C6_dest.body
}


NamedRegistry &NamedRegistry::operator=(const NamedRegistry &right) {
    //## begin NamedRegistry::operator=%3EF756AA02C6_assign.body preserve=yes
    //## end NamedRegistry::operator=%3EF756AA02C6_assign.body
}


int NamedRegistry::operator==(const NamedRegistry &right) const {
    //## begin NamedRegistry::operator==%3EF756AA02C6_eq.body preserve=yes
    //## end NamedRegistry::operator==%3EF756AA02C6_eq.body
}

int NamedRegistry::operator!=(const NamedRegistry &right) const {
    //## begin NamedRegistry::operator!=%3EF756AA02C6_neq.body preserve=yes
    //## end NamedRegistry::operator!=%3EF756AA02C6_neq.body
}


//## Other Operations (implementation)
void NamedRegistry::init(
    char *name,
    char *cl_name,
    char *categor,
    char *descr,
    void (*print)(FilterOut &)) {}

NamedRegistry *NamedRegistry::name_to_reg_data(void char_Xname) {}

void NamedRegistry::list_all_names(char *categor) {}

void NamedRegistry::print_all_help(char *categor) {}

int NamedRegistry::check_registered(char *name, DescribedClass *cl) {}

char *NamedRegistry::get_name(DescribedClass *cl, char *categor, int check_unic) {}

char *NamedRegistry::obj_name() {}

char *NamedRegistry::obj_descr() {}

char *NamedRegistry::category() {}

char *NamedRegistry::obj_classname() {}
