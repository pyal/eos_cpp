
#include "zubr_impl.h"

//enum {SIN,COS,EXP} Func;//,TETA,RND,INT,ABS} Func;
void *BuildInFunc::FPtr[]={(double(*)(double))&sin,
                           (double(*)(double))&cos,
                           (double(*)(double))&exp };


const char *Symbol::type_names[]={"Number","Var","Bltin","Undef","String","BadType"};
const char *Symbol::GetTypeName(int t){
	int Decode[512];Decode[NUMBER]=0;Decode[VAR]=1;Decode[BLTIN]=2;Decode[UNDEF]=3;Decode[STRING]=4;
	if (t<0 || t>4) type_names[5];
	return type_names[Decode[t]];
}
//extern int zubr_calculator_zubr_parse(void); // - not to redefine
//extern int zubr_calculator_zubr_lex(void);
//extern void zubr_calculator_zubr_error(char*);
//typedef union{   SYMBOL *sym;   INST   *inst;} ZUBR_CALCULATOR_ZUBR_STYPE;
//extern ZUBR_CALCULATOR_ZUBR_STYPE zubr_calculator_zubr_lval;

int zubr_calculator_zubr_lex(){
    Lex_Result *res = CalculatorProgam::GetCurPtr()->lex->Analyse();
	if (!res) 
		return 0;
	if (res->res_type==Lex_Result::Error)
		throw info_except(" Lexema extraction error: %s\n",res->res_str.c_str());
    if (res->res_type==Lex_Result::NumberDouble){
        zubr_calculator_zubr_lval=CalculatorProgam::GetCurPtr()->vars.AddConst(NUMBER,res->res_val,0);
        return NUMBER;
    }
    if (res->res_type==Lex_Result::NumberInt){
        zubr_calculator_zubr_lval=CalculatorProgam::GetCurPtr()->vars.AddConst(NUMBER,res->res_int,0);
        return NUMBER;
    }
    if (res->res_type==Lex_Result::Identifier){
        if (!CalculatorProgam::GetCurPtr()->vars.GetVar(res->res_str.c_str(),zubr_calculator_zubr_lval) )
		    zubr_calculator_zubr_lval=CalculatorProgam::GetCurPtr()->vars.AddVar(res->res_str.c_str(),UNDEF,0,0,0,0);
        return VAR;
    }
    if (res->res_type==Lex_Result::Token){
        if (res->res_int<1500)
            return res->res_int;
        zubr_calculator_zubr_lval=CalculatorProgam::GetCurPtr()->vars.AddVar(res->res_str.c_str(),BLTIN,0,0,BuildInFunc::FPtr[res->res_int-1500],0);
        return BLTIN;

    }
    if (res->res_type==Lex_Result::String){
        zubr_calculator_zubr_lval=CalculatorProgam::GetCurPtr()->vars.AddConst(STRING,0,res->res_str.c_str());
        return STRING;
    }
    if (res->res_type==Lex_Result::End)
        return 0;
    zubr_calculator_zubr_error("Do not understand token");
    return 0;
}   

void zubr_calculator_zubr_error(char*er){
    int Line, Pos;
    CalculatorProgam::GetCurPtr()->lex->GetBuferReadingPos(Line,Pos);
    cout<<" Error occured in Line:"<<Line<<" Pos "<<Pos<<"\n";
    if (er)  cout<<" Error is : "<<er<<"\n";
    exit(1);
}



//
//typedef struct FRAME
//{
//   SYMBOL *sp;
//   INST   *retpc;
//   Datum  *argn;
//   int     nargs;
//} FRAME;

//struct CalculatorProgam: RefCount{
//    ResizeVector<Datum> data_stack;
////    Datum *stack_ptr;
//    ResizeVector<INST>  progr;
//    INST *progr_ptr,*progr_base,*progr_counter;
//    int returning; // 1 if find operator
//
//    static CalculatorProgam* GetCurPtr(CalculatorProgam* prg=NULL){
//        if (prg){
//            Ptr_Old = Ptr;Ptr = prg;
//        }
//        return Ptr;
//    }
//    static CalculatorProgam *ReadProgram(istream &in){
//    }
//private:
//    static CalculatorProgam* Ptr,*Ptr_Old;
//};
CalculatorProgam *CalculatorProgam::Ptr,*CalculatorProgam::Ptr_Old;





CalculatorProgam &CalculatorProgam::Execute(){
    Ptr = this;
    data_stack.Release();
    //fp     = frame;
    returning = 0;
    execute( (INST*)progr );
    return *this;
};
CalculatorProgam &CalculatorProgam::Read(){
    Ptr = this;
    while (lex->IsGood())
		zubr_calculator_zubr_parse();
    code(STOP);
//    lex->ResetStream(NULL);
    return *this;
}


//#define NSTACK 512
//static Datum stack[NSTACK];
//static Datum *pstack;
//
//#define NPROG 1024
//INST  prog[NPROG];     /* machine */
//INST *pprog;
//INST *progbase = prog; /* begin of current prog */
//INST *pc;              /* program counter */
//int   returning;       /* 1, if find operator */

//#define NFRAME 256
//FRAME  frame[NFRAME];
//FRAME *fp;             /* ptr to domain of stack */

void warning(const char *fst, const char *sec){
    cout<<" Warning: "<<fst;
    if (sec) cout<<" | "<<sec;
    cout<<":\n";
}
void initcode( void )
{
    CalculatorProgam *Ptr = CalculatorProgam::GetCurPtr();
    Ptr->progr.Release();
    Ptr->data_stack.Release();
    //fp     = frame;
    Ptr->returning = 0;
}

void push( Datum d )
{
    CalculatorProgam *Ptr = CalculatorProgam::GetCurPtr();
    Ptr->data_stack.push(d);
}

Datum pop( void )
{
    CalculatorProgam *Ptr = CalculatorProgam::GetCurPtr();
    if( Ptr->data_stack.empty() )
      throw info_except( "stack underflow" );
    Datum ret = Ptr->data_stack.top();
    Ptr->data_stack.pop();
    return ret;
}

INST *code( INST f )
{
    CalculatorProgam *Ptr = CalculatorProgam::GetCurPtr();
    INST *opprog = &Ptr->progr.top();
    Ptr->progr.push(f);
    return opprog;
}

void execute( INST *p )
{
    CalculatorProgam *Ptr = CalculatorProgam::GetCurPtr();
    for( Ptr->progr_counter = p; 
        *(Ptr->progr_counter) != STOP && !Ptr->returning; )  
        (*(*(Ptr->progr_counter++)))();
	Ptr->progr_counter++;
	if (*(Ptr->progr_counter)!=STOP){
//		printf("Next instr\n");
		execute(Ptr->progr_counter);
	}
}

void constpush( void ){
    Datum d;
 //   CalculatorProgam *Ptr = CalculatorProgam::GetCurPtr();
 //   d.val = ((Symbol *) *(Ptr->progr_counter))->u.val;
	//Ptr->progr_counter++;
    d.val = CalculatorProgam::ExtractSmbFromPc()->u.val;
    push( d );
}

void varpush( void ){
    Datum d;
    d.sym = CalculatorProgam::ExtractSmbFromPc();
    push( d );
}

/* DATUM pop (void) not load to stack: error type-check in execute() OS/2 */
void void_pop( void ){ pop(); }



void Yadd( void )
{
   Datum d1, d2;
   d2 = pop();
   d1 = pop();
   d1.val += d2.val;
   push( d1 );
}

void Ysub( void )
{
   Datum d1, d2;
   d2 = pop();
   d1 = pop();
   d1.val -= d2.val;
   push( d1 );
}

void Ymul( void )
{
   Datum d1, d2;
   d2 = pop();
   d1 = pop();
   d1.val *= d2.val;
   push( d1 );
}

void Ydiv( void )
{
   Datum d1, d2;
   d2 = pop();
   if( d2.val == 0.0 ) warning( "division by zero", (char *)0 );
   d1 = pop();
   d1.val /= d2.val;
   push( d1 );
}

void power( void )
{
   Datum d1, d2;
   d2 = pop();
   d1 = pop();
//   d1.val = ZPow( d1.val, d2.val );
   d1.val = pow( d1.val, d2.val );
   push( d1 );
}

void negate( void )
{
   Datum d;
   d = pop();
   d.val = -d.val;
   push( d );
}

void eval( void )
{
   Datum d;
   d = pop();
   if( d.sym->type == UNDEF )
	   warning( "undefined variable - ", d.sym->name.c_str() );
   d.val = d.sym->u.val;
   push( d );
}

void assign( void )
{
   Datum d1, d2;
   d1 = pop();
   d2 = pop();
   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
	   warning( "assignment to non-variable", d1.sym->name.c_str() );
   d1.sym->u.val = d2.val;
   d1.sym->type  = VAR;
   push( d2 );
}
void print( void )
{
   Datum d;
   d = pop();
   printf( "\t%25.17e\n", d.val );
}



//void negate    ( void );
void prexpr( void )
{
   Datum d;
   d = pop();
   printf( "\t%25.17e", d.val );
}

void prstr( void )
{
    printf( "%s", CalculatorProgam::ExtractSmbFromPc()->u.str.c_str() );
}
void bltin( void )
{
   Datum d;
   d = pop();
   d.val = (*(double (*) (double))
       CalculatorProgam::ExtractSmbFromPc()->u.ptr)( d.val );
   push( d );
}

void bltin2( void )
{
   Datum d1, d2;
   d2 = pop();
   d1 = pop();
   d1.val = (*(double (*) (double,double))
       CalculatorProgam::ExtractSmbFromPc()->u.ptr)( d1.val, d2.val );
//   d1.val = (*(double (*) ())(*pc++))( d1.val, d2.val );
   push (d1);
}

void varread( void )
{
   Datum d;
   FILE *f = stdin;
   Symbol *var = CalculatorProgam::ExtractSmbFromPc();

   fprintf( stderr, "type number: %s = ", var->name.c_str() );
   switch( fscanf( f, "%lf", &var->u.val) )
   {
      case EOF:
         d.val = var->u.val = 0.0;
         break;
      case 0:
          warning( "non-number read into", var->name.c_str() );
         break;
      default:
         d.val = 1.0;
         break;
   }
   var->type = VAR;
   push( d );
}

void Yaddeq( void )
{
   Datum d1, d2;
   d1 = pop();
   d2 = pop();
   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
       warning( "assignment to non-variable", d1.sym->name.c_str() );
   d1.sym->u.val += d2.val;
   d1.sym->type   = VAR;
   push( d2 );
}

void Ysubeq( void )
{
   Datum d1, d2;
   d1 = pop();
   d2 = pop();
   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
       warning( "assignment to non-variable", d1.sym->name.c_str() );
   d1.sym->u.val -= d2.val;
   d1.sym->type   = VAR;
   push( d2 );
}

void Ymuleq( void )
{
   Datum d1, d2;
   d1 = pop();
   d2 = pop();
   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
       warning("assignment to non-variable", d1.sym->name.c_str() );
   d1.sym->u.val *= d2.val;
   d1.sym->type   = VAR;
   push( d2 );
}

void Ydiveq( void )
{
   Datum d1, d2;
   d1 = pop();
   d2 = pop();
   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
       warning( "assignment to non-variable", d1.sym->name.c_str() );
   d1.sym->u.val /= d2.val;
   d1.sym->type   = VAR;
   push( d2 );
}


///***** gt, ge, lt, le, eq, ne, and, or, not ******/
//
//void gt( void )
//{
//   Datum d1, d2;
//
//   d2 = pop();
//   d1 = pop();
//   d1.val = (double)(d1.val > d2.val);
//   push( d1 );
//}
//
//void ge( void )
//{
//   Datum d1, d2;
//
//   d2 = pop();
//   d1 = pop();
//   d1.val = (double)(d1.val >= d2.val);
//   push( d1 );
//}
//
//void lt( void )
//{
//   Datum d1, d2;
//
//   d2 = pop();
//   d1 = pop();
//   d1.val = (double)(d1.val < d2.val);
//   push( d1 );
//}
//
//void le( void )
//{
//   Datum d1, d2;
//
//   d2 = pop();
//   d1 = pop();
//   d1.val = (double)(d1.val <= d2.val);
//   push( d1 );
//}
//
//void eq( void )
//{
//   Datum d1, d2;
//
//   d2 = pop();
//   d1 = pop();
//   d1.val = (double)(d1.val == d2.val);
//   push( d1 );
//}
//
//void ne( void )
//{
//   Datum d1, d2;
//
//   d2 = pop();
//   d1 = pop();
//   d1.val = (double)(d1.val != d2.val);
//   push( d1 );
//}
//
//void and( void )
//{
//   Datum d1, d2;
//
//   d2 = pop();
//   d1 = pop();
//   d1.val = (double)(d1.val != 0.0 && d2.val != 0.0);
//   push( d1 );
//}
//
//void or( void )
//{
//   Datum d1, d2;
//
//   d2 = pop();
//   d1 = pop();
//   d1.val = (double)(d1.val != 0.0 || d2.val != 0.0);
//   push( d1 );
//}
//
//void not( void )
//{
//   Datum d;
//
//   d = pop();
//   d.val = (double)(d.val == 0.0);
//   push( d );
//}
//
//
//
///*************************************************/



//void print( void )
//{
//   Datum d;
//   d = pop();
//   printf( "\t%25.17e\n", d.val );
//}
//

//void defnonly( char *s )
//{
//   if( !indef )
//      warning( s, "used outside definition" );
//}
//
//
//


///********** addeq, subeq, muleq, diveq ***********/
//
//void Yaddeq( void )
//{
//   Datum d1, d2;
//   d1 = pop();
//   d2 = pop();
//   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
//      warning( "assignment to non-variable", d1.sym->name );
//   d1.sym->u.val += d2.val;
//   d1.sym->type   = VAR;
//   push( d2 );
//}
//
//void Ysubeq( void )
//{
//   Datum d1, d2;
//   d1 = pop();
//   d2 = pop();
//   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
//      warning( "assignment to non-variable", d1.sym->name );
//   d1.sym->u.val -= d2.val;
//   d1.sym->type   = VAR;
//   push( d2 );
//}
//
//void Ymuleq( void )
//{
//   Datum d1, d2;
//   d1 = pop();
//   d2 = pop();
//   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
//      warning("assignment to non-variable", d1.sym->name );
//   d1.sym->u.val *= d2.val;
//   d1.sym->type   = VAR;
//   push( d2 );
//}
//
//void Ydiveq( void )
//{
//   Datum d1, d2;
//   d1 = pop();
//   d2 = pop();
//   if( d1.sym->type != VAR && d1.sym->type != UNDEF )
//      warning( "assignment to non-variable", d1.sym->name );
//   d1.sym->u.val /= d2.val;
//   d1.sym->type   = VAR;
//   push( d2 );
//}
//
/******* add, sub, mul, div, negate, power *******/


//void whilecode( void )
//{
//   Datum d;
//   INST *savepc = pc;
//
//   execute( savepc + 2 );               /* условие */
//   d = pop();
//   while( d.val )
//   {
//      execute( *((INST **)(savepc)) );  /* тело */
//      if( returning ) break;
//      execute( savepc + 2 );            /* условие */
//      d = pop();
//   }
//
//   if( !returning )
//      pc = *((INST **)(savepc + 1));    /* следующий оператор */
//}
//
//void ifcode( void )
//{
//   Datum d;
//   INST *savepc = pc;                   /* часть "then" */
//
//   execute( savepc + 3 );               /* условие */
//   d = pop();
//
//   if( d.val ) execute( *((INST **)(savepc)) );
//   else if( *((INST **)(savepc + 1)) )  /* часть "else" ? */
//           execute( *((INST **)(savepc + 1)) );
//
//   if( !returning )
//      pc = *((INST **)(savepc + 2));    /* следующий оператор */
//}
//
//void defin( SYMBOL *sp )
//{
//   sp->u.defn = (INST)progbase; /* begin of prog */
//   progbase = pprog;
//}
//
//void call( void )
//{
//   SYMBOL *sp = (SYMBOL *)pc[0];
//
//   if( fp++ >= &frame[NFRAME-1] )
//      warning( sp->name, "call nested too deeply" );
//
//   fp->sp    = sp;
//   fp->nargs = (int)pc[1];
//   fp->retpc = pc + 2;
//   fp->argn  = pstack - 1;
//
//   execute( (INST *)sp->u.defn );
//
//   returning = 0;
//}
//
//void ret( void )
//{
//   int i;
//
//   for( i = 0; i < fp->nargs; i++ )
//      pop();
//
//   pc = (INST *)fp->retpc;
//   --fp;
//
//   returning = 1;
//}
//
//void funcret( void )
//{
//   Datum d;
//
//   if( fp->sp->type == PROCEDURE )
//      warning( fp->sp->name, "proc returns value" );
//
//   d = pop();
//   ret();
//   push( d );
//}
//
//void procret( void )
//{
//   if( fp->sp->type == FUNCTION )
//      warning( fp->sp->name, "func returns no value" );
//
//   ret();
//}
//
//double *getarg( void )
//{
//   int nargs = (int)*pc++;
//
//   if( nargs > fp->nargs )
//      warning( fp->sp->name, "not enough arguments" );
//
//   return &fp->argn[nargs - fp->nargs].val;
//}
//
//void arg( void )
//{
//   Datum d;
//   d.val = *getarg();
//   push( d );
//}
//
//void argassign( void )
//{
//   Datum d;
//   d = pop();
//   push( d );
//   *getarg() = d.val;
//}
