#ifndef __VM_ANALYSE_H
#define __VM_ANALYSE_H




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


#endif