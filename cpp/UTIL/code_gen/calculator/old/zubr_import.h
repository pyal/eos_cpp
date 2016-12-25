


typedef int (*INST)(); /* command of machine */
#define STOP (INST)0

typedef void *SYMBOL;

extern int zubr_calculator_zubr_parse(void);   // External


extern int zubr_calculator_zubr_lex(void);
extern void zubr_calculator_zubr_error(char *);

//extern void error_void    ( void );
extern void assign(void);
extern void varpush(void);
extern void void_pop(void);
extern void constpush(void);
extern void eval(void);
extern INST *code(INST f);
//extern void execute   ( INST *p );

extern void Yadd(void);
extern void Ysub(void);
extern void Ymul(void);
extern void Ydiv(void);
extern void power(void);

extern void print(void);


extern void negate(void);
extern void prstr(void);
extern void prexpr(void);
extern void bltin(void);
extern void bltin2(void);
extern void varread(void);

extern void Yaddeq(void);
extern void Ysubeq(void);
extern void Ymuleq(void);
extern void Ydiveq(void);


//
//
//typedef union DATUM         /* type of STACK of interpretator */
//{
//   double  val;
//   SYMBOL *sym;
//} DATUM;
//
//
//typedef int (*INST)();      /* command of machine */
//#define STOP (INST)0
//
//typedef struct symbol SYMBOL;
////#include "..\vm3_cpp.h"
//struct symbol
//{
//   char *name;          /* имя переменной или функции */
//   int   type;          /* тип: VAR, BLTIN, UNDEF     */
//   union
//   {
//      double  val;       /* значение для переменной( VAR )           */
//      double  (*ptr)();  /* указатель на встроенную функцию( BLTIN ) */
//      int    (*defn)();  /* FUNCTION, PROCEDURE */
//      char   *str;       /* string */
//   } u;
//
//   struct symbol *next; /* связь со следующей записью */
//};
//
///************************************************************
//  Декларация функций добавления записи и поиска существующей
// ************************************************************/
//extern SYMBOL *install( char *s, int t, double d );
//extern SYMBOL *lookup( char *s );
//
