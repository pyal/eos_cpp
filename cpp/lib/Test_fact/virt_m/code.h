
/**************************************************************
  CODE.H
 **************************************************************/


typedef union DATUM         /* type of STACK of interpretator */
{
   double  val;
   SYMBOL *sym;
} DATUM;


typedef int (*INST)();      /* command of machine */
#define STOP (INST)0


typedef struct FRAME
{
   SYMBOL *sp;
   INST   *retpc;
   DATUM  *argn;
   int     nargs;
} FRAME;


extern INST prog[];
extern INST *pprog;
extern INST *progbase;

//extern void PrintStackSymbol();

extern DATUM pop( void );
extern void  void_pop( void );

extern void eval      ( void );
extern void Yadd      ( void );
extern void Ysub      ( void );
extern void Ymul      ( void );
extern void Ydiv      ( void );
extern void negate    ( void );
extern void power     ( void );

extern void assign    ( void );
extern void prexpr    ( void );
extern void bltin     ( void );
extern void bltin2    ( void );
extern void varpush   ( void );
extern void constpush ( void );
extern void print     ( void );

extern void initcode  ( void );
extern INST *code     ( INST f );
extern void execute   ( INST *p );

extern void whilecode ( void );
extern void ifcode    ( void );

extern void Yaddeq    ( void );
extern void Ysubeq    ( void );
extern void Ymuleq    ( void );
extern void Ydiveq    ( void );

extern void gt        ( void );
extern void ge        ( void );
extern void lt        ( void );
extern void le        ( void );
extern void eq        ( void );
extern void ne        ( void );
extern void and       ( void );
extern void or        ( void );
extern void not       ( void );

extern void defin     ( SYMBOL *sp );
extern void call      ( void );
extern void funcret   ( void );
extern void procret   ( void );
extern void arg       ( void );
extern void argassign ( void );

extern void prstr     ( void );
extern void varread   ( void );
extern void defnonly  ( char *s );


/*********************  END OF FILE CODE.H  *******************/
