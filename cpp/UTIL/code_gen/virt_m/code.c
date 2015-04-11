
/**************************************************************
  CODE.C
 **************************************************************/

#include <stdio.h>

#include "init.h"
#include "symtab.h"
#include "mathf.h"
#include "code.h"
#include "main.h"

#include "z_tab.h"


#define NSTACK 512
static DATUM stack[NSTACK];
static DATUM *pstack;

#define NPROG 1024
INST  prog[NPROG];     /* machine */
INST *pprog;
INST *progbase = prog; /* begin of current prog */
INST *pc;              /* program counter */
int   returning;       /* 1, if find operator */

#define NFRAME 256
FRAME  frame[NFRAME];
FRAME *fp;             /* ptr to domain of stack */


void PrintStackSymbol()
{printf("Stored on Stack symbols:\n");	PrintSymbols(stack[1].sym);}
void initcode( void )
{
   pprog  = progbase;
   pstack = stack;
   fp     = frame;
   returning = 0;
}

void push( DATUM d )
{
   if( pstack >= &stack[NSTACK] )
      warning( "stack overflow", (char *)0 );
   *pstack++ = d;
//   printf("push :");PrintSymbol(d.sym);
}

DATUM pop( void )
{
//   printf("pop \n");
   if( pstack <= stack )
      warning( "stack underflow", (char *)0 );

   return( *--pstack );
}

INST *code( INST f )
{
   INST *opprog = pprog;
   if( pprog >= &prog[NPROG] )
      warning( "program too big", (char *)0 );
   *pprog++ = f;

   return( opprog );
}

void execute( INST *p )
{
   for( pc = p; *pc != STOP && !returning; )  (*(*pc++))();
}

void constpush( void )
{
   DATUM d;
   d.val = ((SYMBOL *) *pc++)->u.val;
//   if (d.sym) d.sym->name=NULL;
//   d.sym=NULL;
   push( d );
}

void varpush( void )
{
   DATUM d;
   d.sym = (SYMBOL *)(*pc++);
   push( d );
}

/* DATUM pop (void) not load to stack: error type-check in execute() OS/2 */
void void_pop( void )
{
   pop();
}

void whilecode( void )
{
   DATUM d;
   INST *savepc = pc;

   execute( savepc + 2 );               /* условие */
   d = pop();
   while( d.val )
   {
      execute( *((INST **)(savepc)) );  /* тело */
      if( returning ) break;
      execute( savepc + 2 );            /* условие */
      d = pop();
   }

   if( !returning )
      pc = *((INST **)(savepc + 1));    /* следующий оператор */
}

void ifcode( void )
{
   DATUM d;
   INST *savepc = pc;                   /* часть "then" */

   execute( savepc + 3 );               /* условие */
   d = pop();

   if( d.val ) execute( *((INST **)(savepc)) );
   else if( *((INST **)(savepc + 1)) )  /* часть "else" ? */
           execute( *((INST **)(savepc + 1)) );

   if( !returning )
      pc = *((INST **)(savepc + 2));    /* следующий оператор */
}

void defin( SYMBOL *sp )
{
   sp->u.defn = (INST)progbase; /* begin of prog */
   progbase = pprog;
}

void call( void )
{
   SYMBOL *sp = (SYMBOL *)pc[0];

   if( fp++ >= &frame[NFRAME-1] )
      warning( sp->name, "call nested too deeply" );

   fp->sp    = sp;
   fp->nargs = (int)pc[1];
   fp->retpc = pc + 2;
   fp->argn  = pstack - 1;

   execute( (INST *)sp->u.defn );

   returning = 0;
}

void ret( void )
{
   int i;

   for( i = 0; i < fp->nargs; i++ )
      pop();

   pc = (INST *)fp->retpc;
   --fp;

   returning = 1;
}

void funcret( void )
{
   DATUM d;

   if( fp->sp->type == PROCEDURE )
      warning( fp->sp->name, "proc returns value" );

   d = pop();
   ret();
   push( d );
}

void procret( void )
{
   if( fp->sp->type == FUNCTION )
      warning( fp->sp->name, "func returns no value" );

   ret();
}

double *getarg( void )
{
   int nargs = (int)*pc++;

   if( nargs > fp->nargs )
      warning( fp->sp->name, "not enough arguments" );

   return &fp->argn[nargs - fp->nargs].val;
}

void arg( void )
{
   DATUM d;
   d.val = *getarg();
   push( d );
}

void argassign( void )
{
   DATUM d;
   d = pop();
   push( d );
   *getarg() = d.val;
}


/********** addeq, subeq, muleq, diveq ***********/

void Yaddeq( void )
{
   DATUM d1, d2;
   d1 = pop();
   d2 = pop();
   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
      warning( "assignment to non-variable", d1.sym->name );
   d1.sym->u.val += d2.val;
   d1.sym->type   = VAR;
   push( d2 );
}

void Ysubeq( void )
{
   DATUM d1, d2;
   d1 = pop();
   d2 = pop();
   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
      warning( "assignment to non-variable", d1.sym->name );
   d1.sym->u.val -= d2.val;
   d1.sym->type   = VAR;
   push( d2 );
}

void Ymuleq( void )
{
   DATUM d1, d2;
   d1 = pop();
   d2 = pop();
   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
      warning("assignment to non-variable", d1.sym->name );
   d1.sym->u.val *= d2.val;
   d1.sym->type   = VAR;
   push( d2 );
}

void Ydiveq( void )
{
   DATUM d1, d2;
   d1 = pop();
   d2 = pop();
   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
      warning( "assignment to non-variable", d1.sym->name );
   d1.sym->u.val /= d2.val;
   d1.sym->type   = VAR;
   push( d2 );
}

/******* add, sub, mul, div, negate, power *******/

void Yadd( void )
{
   DATUM d1, d2;
   d2 = pop();
   d1 = pop();
   d1.val += d2.val;
   push( d1 );
}

void Ysub( void )
{
   DATUM d1, d2;
   d2 = pop();
   d1 = pop();
   d1.val -= d2.val;
   push( d1 );
}

void Ymul( void )
{
   DATUM d1, d2;
   d2 = pop();
   d1 = pop();
   d1.val *= d2.val;
   push( d1 );
}

void Ydiv( void )
{
   DATUM d1, d2;
   d2 = pop();
   if( d2.val == 0.0 ) warning( "division by zero", (char *)0 );
   d1 = pop();
   d1.val /= d2.val;
   push( d1 );
}

void negate( void )
{
   DATUM d;
   d = pop();
   d.val = -d.val;
   push( d );
}

void power( void )
{
   DATUM d1, d2;
   d2 = pop();
   d1 = pop();
   d1.val = ZPow( d1.val, d2.val );
   push( d1 );
}

/***** gt, ge, lt, le, eq, ne, and, or, not ******/

void gt( void )
{
   DATUM d1, d2;

   d2 = pop();
   d1 = pop();
   d1.val = (double)(d1.val > d2.val);
   push( d1 );
}

void ge( void )
{
   DATUM d1, d2;

   d2 = pop();
   d1 = pop();
   d1.val = (double)(d1.val >= d2.val);
   push( d1 );
}

void lt( void )
{
   DATUM d1, d2;

   d2 = pop();
   d1 = pop();
   d1.val = (double)(d1.val < d2.val);
   push( d1 );
}

void le( void )
{
   DATUM d1, d2;

   d2 = pop();
   d1 = pop();
   d1.val = (double)(d1.val <= d2.val);
   push( d1 );
}

void eq( void )
{
   DATUM d1, d2;

   d2 = pop();
   d1 = pop();
   d1.val = (double)(d1.val == d2.val);
   push( d1 );
}

void ne( void )
{
   DATUM d1, d2;

   d2 = pop();
   d1 = pop();
   d1.val = (double)(d1.val != d2.val);
   push( d1 );
}

void and( void )
{
   DATUM d1, d2;

   d2 = pop();
   d1 = pop();
   d1.val = (double)(d1.val != 0.0 && d2.val != 0.0);
   push( d1 );
}

void or( void )
{
   DATUM d1, d2;

   d2 = pop();
   d1 = pop();
   d1.val = (double)(d1.val != 0.0 || d2.val != 0.0);
   push( d1 );
}

void not( void )
{
   DATUM d;

   d = pop();
   d.val = (double)(d.val == 0.0);
   push( d );
}



/*************************************************/

void eval( void )
{
   DATUM d;
   d = pop();
   if( d.sym->type == UNDEF )
      warning( "undefined variable", d.sym->name );
   d.val = d.sym->u.val;
   push( d );
}

void assign( void )
{
   DATUM d1, d2;
   d1 = pop();
   d2 = pop();
   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
      warning( "assignment to non-variable", d1.sym->name );
   d1.sym->u.val = d2.val;
   d1.sym->type  = VAR;
   push( d2 );
}


void print( void )
{
   DATUM d;
   d = pop();
   printf( "\t%25.17e\n", d.val );
}

void prexpr( void )
{
   DATUM d;
   d = pop();
   printf( "\t%25.17e", d.val );
}

void prstr( void )
{
   printf( "%s", (char *)*pc++ );
}

void varread( void )
{
   DATUM d;
   FILE *f = stdin;
   SYMBOL *var = (SYMBOL *)*pc++;

again:
   fprintf( stderr, "type number: %s = ", var->name );
   switch( fscanf( f, "%lf", &var->u.val) )
   {
      case EOF:
         d.val = var->u.val = 0.0;
         break;
      case 0:
         warning( "non-number read into", var->name );
         break;
      default:
         d.val = 1.0;
         break;
   }
   var->type = VAR;
   push( d );
}

void defnonly( char *s )
{
   if( !indef )
      warning( s, "used outside definition" );
}

void bltin( void )
{
   DATUM d;
   d = pop();
   d.val = (*(double (*) ())(*pc++))( d.val );
   push( d );
}

void bltin2( void )
{
   DATUM d1, d2;
   d2 = pop();
   d1 = pop();
   d1.val = (*(double (*) ())(*pc++))( d1.val, d2.val );
   push (d1);
}


/*******************  END OF FILE CODE.C  *********************/
