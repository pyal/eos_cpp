#include "eno_main.h"






#include "eno_main.h"



DescribedClass* create_DivOper_cd(){return new DivOper;}


static ClassDesc DivOper_cd(typeid(DivOper),"DivOper",1,"OperatorOneD",&create_DivOper_cd);
