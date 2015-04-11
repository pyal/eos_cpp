#include <lib\precompiled\math.h>


#include "fun_fac.h"
//static ClassDesc OneVarFunction_ClassDesc(typeid(OneVarFunction),"OneVarFunction",1,"OneVarFunction_category",create<OneVarFunction>);
//static ClassDesc TwoVarFunction_ClassDesc(typeid(TwoVarFunction),"TwoVarFunction",1,"TwoVarFunction_category",create<TwoVarFunction>);
static ClassDesc OneVarFormula_ClassDesc(typeid(OneVarFormula),"OneVarFormula",1,"OneVarFunction_category",create<OneVarFormula>);

static ClassDesc ManyVarFunc2OneVar_ClassDesc(typeid(ManyVarFunc2OneVar),"ManyVarFunc2OneVar",1,"OneVarFunction_category",create<ManyVarFunc2OneVar>);

#include "mat/quad.h"

void OneVarFunction::Calculate(double *x,double *y,int Num)
{  for (int k=1;k<=Num;k++)  y[k]=Calculate(x[k]);}
double OneVarFunction::Derivative(double x)
{  return (Calculate(x+0.5*DerivStep)-Calculate(x-0.5*DerivStep))/DerivStep;}

OneVarFunction* OneVarFunction::StaticCurPtr = NULL;

double OneVarFunction::Integral(double xFrom,double xTo) {
	double ResErr;
	int Err;
	OneVarFunction *oldPtr = StaticCurPtr;
	StaticCurPtr = this;
	double ret = Qags_IntSubInt(StaticClc, xFrom, xTo, DerivStep, DerivStep, 10000, ResErr, Err);
	StaticCurPtr = oldPtr;
	return  ret;
}
//{return 1;}


void TwoVarFunction::Calculate(double *x,double *y,double *res,int Num)
{  for (int k=1;k<=Num;k++)  res[k]=Calculate(x[k],y[k]);}


int OneVarFuncRegister(istream &in,OneVarFunction *&Func){
    SavableClass *cl = SavableClass::Read(in);
	Func=dynamic_cast<OneVarFunction *>(cl);
	if (!Func) throw info_except(" Class %s is not of OneVarFunction type\n ",cl->class_name());
	return 1;
}
int TwoVarFuncRegister(istream &in,TwoVarFunction *&Func){
	SavableClass *cl = SavableClass::Read(in);
	Func=dynamic_cast<TwoVarFunction *>(cl);
	if (!Func) throw info_except(" Class %s is not of TwoVarFunction type\n ",cl->class_name());
	return 1;
}

#include "onevarcurves.h"
ManyVarFunc2OneVar::ManyVarFunc2OneVar()
    :OneVarFunction()
    , Func(new Spl2DGenerator)
    , InVarName("X")
    , OutVarName("Z"){
    FixedParameters["Y"] = 100;
}
