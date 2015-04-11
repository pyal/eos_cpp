#include "grid_array.h"


DescribedClass* create_GridVectorField_double_cd(){return new GridVectorField<double>;}
static ClassDesc GridVectorField_double_cd(typeid(GridVectorField<double>),"GridVectorField_double",1,"Grid",&create_GridVectorField_double_cd);
DescribedClass* create_GridVectorField_int_cd(){return new GridVectorField<int>;}
static ClassDesc GridVectorField_int_cd(typeid(GridVectorField<int>),"GridVectorField_int",1,"Grid",&create_GridVectorField_int_cd);
