// vm3.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
//#include "cpp\main.h"
#ifdef __cplusplus
extern "C" {
#endif

//extern int main1(int ,char **argv);
extern void ReadProgram(char *prg_name);
extern void SetVariable(char *var_name,double val);
extern void SetProcedure(char *proc_name,int narg,double *arg);
extern void Execute();

#ifdef __cplusplus
	}
#endif

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc,  char * argv[])
{
//	main1(argc,argv);
double par[1];par[0]=5;
	ReadProgram(argv[1]);
//	SetVariable("test_var",100);
	SetVariable("test_var",1000.54634);
	SetProcedure("test",1,&par[0]);
	Execute();

	return 0;
}

