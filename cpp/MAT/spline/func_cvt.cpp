#include <lib\precompiled\math.h>
#include "func_cvt.h"

static ClassDesc TExpConverter_ClassDesc(typeid(TExpConverter),"TExpConverter",1,"FunctionConverter_category",create<TExpConverter>);

static ClassDesc TAutoExpConverter_ClassDesc(typeid(TAutoExpConverter),"TAutoExpConverter",1,"FunctionConverter_category",create<TAutoExpConverter>);

TExpConverter::~TExpConverter(){}

TAutoExpConverter *TAutoExpConverter::CurPtr = NULL;
