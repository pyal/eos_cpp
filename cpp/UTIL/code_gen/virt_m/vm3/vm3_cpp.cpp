#include "vm3_cpp.h"

extern "C" void InitProgram() {
    cout << "UUUUUUUUUUUUUUUUUUUUU\n";
};
void InitVariables(){};

extern "C" void PrintSymbol(SYMBOL *fst) {
    if(fst)
        cout << (*fst);
    else
        cout << " symbol is ZERO\n";
};

extern "C" void PrintSymbols(SYMBOL *fst) {
    SYMBOL *cur = fst;
    while(cur != NULL) {
        PrintSymbol(cur);   //cout<<(*cur);
        cur = cur->next;
    }
}

/*
void CallProcedure(char *name,int narg,double *par)
{
	int k;
	for (k=0;k<narg;k++)
	{
		code( (INST)constpush );
		code( (INST) install( "", NUMBER, par[k] ) );
	}
	code( (INST)call );
	code((INST)(lookup(name)));
	code( (INST)narg ); 
};
void ReadProgram(int argc, char *argv[] )
{
}

*/