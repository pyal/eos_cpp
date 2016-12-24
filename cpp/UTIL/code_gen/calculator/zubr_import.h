


//typedef int (*INST)();      /* command of machine */
//#define STOP (INST)0

typedef void *SYMBOL;

extern int zubr_calculator_zubr_parse(void);   // External


extern int zubr_calculator_zubr_lex(void);
extern void zubr_calculator_zubr_error(const char *);


extern SYMBOL EndStmt(SYMBOL fst);
extern SYMBOL AddVar(SYMBOL fst);
extern SYMBOL AddOper(SYMBOL fst, SYMBOL sec, char oper);
extern void ListStart(void);
extern SYMBOL ListAdd(SYMBOL fst);
extern SYMBOL AddFunc(SYMBOL fst);
extern SYMBOL GetEl(SYMBOL fst, SYMBOL sec);
extern SYMBOL DefineVar(SYMBOL fst, SYMBOL sec);
extern SYMBOL DefineVar(SYMBOL fst);
