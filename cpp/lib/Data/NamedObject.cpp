#include <lib/precompiled/data.h>

// NamedObject
#include "NamedObject.h"

//NamedStore
DescribedClass *create_NamedStore_cd() {
    return new NamedStore;
}
static ClassDesc NamedStore_cd(
    typeid(NamedStore),
    "NamedStore",
    1,
    "",
    &create_NamedStore_cd);

//ResizeVector<T,CpM>
//DescribedClass* create_NamedStore_cd(){return new NamedStore;}
static ClassDesc ResizeVector_Ref_NamedObject_cd(
    typeid(ResizeVector<Ref<NamedObject>, CopyStructRef<Ref<NamedObject>>>),
    "ResizeVector_Ref_NamedObject");

NamedStore &NamedStore::operator=(const NamedStore &right) {
    NAMED_SPARCE::operator=(right);
    SparceArrayInd *ind_p = GetInd();
    int cur = -1;
    while((cur = ind_p->Next(cur)) >= 0)
        operator[](cur) = operator[](cur)->CreateCopyOnStore(DefaultStorage);
    return *this;
};


int NamedStore::Name2Pos(char *name) {
    SparceArrayInd *ind_p = GetInd();
    int ret = -1, cur = -1;
    while((cur = ind_p->Next(cur)) >= 0) {
        NamedObject *ptr = GetPtr(cur);
        if((ptr) && (ptr->obj_name))
            if(Stricmp(ptr->obj_name, name) == 0) {
                ret = cur;
                break;
            }
    }
    return ret;
};


void NamedStore::StoreChanged() {
    SparceArrayInd *ind_p = GetInd();
    int cur = -1;
    while((cur = ind_p->Next(cur)) >= 0)
        GetPtr(cur)->StoreChanged();
};
