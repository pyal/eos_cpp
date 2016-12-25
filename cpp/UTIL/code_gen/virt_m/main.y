
/**************************************************************
  MAIN.Y
 **************************************************************/

%{

#include "init.h"
#include "symtab.h"
#include "mathf.h"
#include "code.h"
#include "main.h"

#define code2(c1,c2) code(c1); code(c2)
#define code3(c1,c2,c3) code(c1); code(c2); code(c3)

int indef;

%}

%union
{
   SYMBOL *sym;
   INST   *inst;
   int     narg;
}
%token <sym>    NUMBER VAR BLTIN UNDEF STRING
%token <sym>    ADDEQ SUBEQ MULEQ DIVEQ
%token <sym>    PRINT WHILE IF ELSE
%token <sym>    FUNCTION PROCEDURE RETURN FUNC PROC READ
%token <narg>   ARG
%type  <inst>   expr assign
%type  <inst>   stmt stmtlist prlist cond while if begin end
%type  <sym>    procname
%type  <narg>   arglist
%right '='
%right ADDEQ SUBEQ MULEQ DIVEQ
%left  OR
%left  AND
%left  GT GE LT LE EQ NE
%left  '+' '-'   /* left associative, same precedence */
%left  '*' '/'   /* left associative, higher precedence */
%left  UNARYMINUS NOT
%right '^'       /* power */

%start list

%%
list:    /* nothing */
       | list '\n'
       | list defn   '\n'
       | list assign '\n' { code2( (INST)void_pop, STOP ); return( 1 ); }
       | list stmt   '\n' { code ( STOP );                 return( 1 ); }
       | list expr   '\n' { code2( (INST)print, STOP );    return( 1 ); }
       | list error  '\n'
         {
            zubr_errok;
         }
       ;

assign:  VAR '=' expr   { code3( (INST)varpush, (INST)$1, (INST)assign ); }
       | VAR ADDEQ expr { code3( (INST)varpush, (INST)$1, (INST)Yaddeq ); }
       | VAR SUBEQ expr { code3( (INST)varpush, (INST)$1, (INST)Ysubeq ); }
       | VAR MULEQ expr { code3( (INST)varpush, (INST)$1, (INST)Ymuleq ); }
       | VAR DIVEQ expr { code3( (INST)varpush, (INST)$1, (INST)Ydiveq ); }
       | ARG '=' expr
         { defnonly( "$" ); code2( (INST)argassign, (INST)$1 ); $$ = $3; }
       ;

stmt:    expr               { code( (INST)pop ); }
       | RETURN
         { defnonly( "return" ); code( (INST)procret ); }
       | RETURN expr
         { defnonly( "return" ); $$ = $2; code( (INST)funcret ); }
       | PROCEDURE begin '(' arglist ')'
         { $$ = $2; code3( (INST)call, (INST)$1, (INST)$4 ); }
       | PRINT prlist       { $$ = $2; }
       | while cond stmt end
         {
            ($1)[1] = (INST)$3; /* тело цикла */
            ($1)[2] = (INST)$4; /* end */
         }
       | if cond stmt end
         {
            ($1)[1] = (INST)$3; /* часть then */
            ($1)[3] = (INST)$4; /* end */
         }
       | if cond stmt end ELSE stmt end
         {
            ($1)[1] = (INST)$3; /* часть then */
            ($1)[2] = (INST)$6; /* часть else */
            ($1)[3] = (INST)$7; /* end */
         }
       | '{' stmtlist '}'  { $$ = $2; }
       ;

cond:    '(' expr ')' { code( STOP ); $$ = $2; }
       ;

while:   WHILE { $$ = code3( (INST)whilecode, STOP, STOP ); }
       ;

if:      IF
         {
            $$ = code( (INST)ifcode );
            code3( STOP, STOP, STOP );
         }
       ;

begin:   /* nothing */  { $$ = pprog; }
       ;

end:     /* nothing */  { code( STOP ); $$ = pprog; }
       ;

stmtlist: /* nothing */ { $$ = pprog; }
       | stmtlist '\n'
       | stmtlist stmt
       ;

expr:    NUMBER       { code2( (INST)constpush, (INST)$1 ); }
       | VAR          { code3( (INST)varpush, (INST)$1, (INST)eval ); }
       | ARG
         { defnonly( "$" ); $$ = code2( (INST)arg, (INST)$1 ); }
       | assign       { code3( (INST)varpush, (INST)$1, (INST)eval ); }
       | FUNCTION begin '(' arglist ')'
         { $$ = $2; code3( (INST)call, (INST)$1, (INST)$4 ); }
       | READ '(' VAR ')'
         { $$ = code2( (INST)varread, (INST)$3 ); }
       | BLTIN '(' expr ')'   { code2( (INST)bltin, (INST)$1->u.ptr); }
       | BLTIN '(' expr ',' expr ')'
         {
            code2( (INST)bltin2, (INST)$1->u.ptr );
         }
       | expr '+' expr             { code( (INST)Yadd );   }
       | expr '-' expr             { code( (INST)Ysub );   }
       | expr '*' expr             { code( (INST)Ymul );   }

       | expr '/' expr             { code( (INST)Ydiv );   }

       | expr '^' expr             { code( (INST)power );  }
       | '(' expr ')'              { $$ =  $2; }
       | '-' expr %prec UNARYMINUS { code( (INST)negate ); }
       | expr GT  expr             { code( (INST)gt );     }
       | expr GE  expr             { code( (INST)ge );     }
       | expr LT  expr             { code( (INST)lt );     }
       | expr LE  expr             { code( (INST)le );     }
       | expr EQ  expr             { code( (INST)eq );     }
       | expr NE  expr             { code( (INST)ne );     }
       | expr AND expr             { code( (INST)and );    }
       | expr OR  expr             { code( (INST)or );     }
       | NOT expr                  { $$ = $2; code( (INST)not ); }
       ;

prlist:  expr              { code( (INST)prexpr ); }
       | STRING            { $$ = code2( (INST)prstr, (INST)$1 ); }
       | prlist ',' expr   { code( (INST)prexpr ); }
       | prlist ',' STRING { $$ = code2( (INST)prstr, (INST)$3 ); }
       ;

defn:    FUNC procname
         { $2->type = FUNCTION; indef = 1; }
         '(' ')' stmt
         { code( (INST)funcret ); defin( $2 ); indef = 0; }
       | PROC procname
         { $2->type = PROCEDURE; indef = 1; }
         '(' ')' stmt
         { code( (INST)procret ); defin( $2 ); indef = 0; }
       ;

procname: VAR
       | FUNCTION
       | PROCEDURE
       ;

arglist: /* nothing */    { $$ = 0; }
       | expr             { $$ = 1; }
       | arglist ',' expr { $$ = $1 + 1; }
       ;

%%

/********************  END OF GRAMMAR  ***********************/

char *progname;

FILE *input_file;
char *input_file_name;

int lineno = 0;


void warning( char *s, char *t )
{
   fprintf( stderr, "%s: %s", progname, s );
   if( t ) fprintf( stderr, " %s", t );
   fprintf( stderr, " near line %d\n", lineno );
}


void zubr_error( char *s )
{
   warning( s, (char *)0 );
}

int follow( int expect, int ifyes, int ifno )
{
   register FILE *f = input_file;
   register int   c;

   c = getc( f );

   if( c == expect ) return( ifyes );
   ungetc( c, f );
   return( ifno );
}

int backslash( int c )
{
   register FILE *f = input_file;
   static   char  tabl[] = "b\bf\fn\nr\rt\t";
   char *p = (char *)0;

   if( c != '\\' ) return( c );

   c = getc( f );
   p = (char *)strchr( tabl, c ); /* or strchr() */
   if( islower( c ) && p != (char *)0 )
      return( p[1] );
   return( c );
}


int zubr_lex( void )
{
   register FILE *f = input_file;
   register int   c;

//   lineno++;


   while( (c = getc( f )) == ' ' || c == '\t' || c == '\r' )
   ;
   if( c == EOF ) return( 0 );
   if ( c=='/') 
   {
    int n=getc(f);
	if (n=='/') { while ((c!='\n') && (c!=EOF)) c=getc(f); if (c=='\n') lineno++;return zubr_lex();}
	else 
	if (n=='*') { while (1) 
	     {
	      while (c!='*') {c=getc(f);if  (c==EOF) return 0; if (c=='\n') lineno++;} 
	      c=getc(f);
	      if (c=='/') return zubr_lex();
	      }}
	else ungetc(n,f);
	}
   
   if( c == '.' || isdigit( c ) )
   {
      double d = 0.0;
      int n = getc( f );

      /* моей функции scanf() нельзя отдавать число "." */
      if( c == '.' && !isdigit( n ) )
      {
         ungetc( n, f );
         zubr_lval.sym = install( "", NUMBER, d );
         return( NUMBER );
      }
      ungetc( n, f );

      /* number */
      ungetc( c, f );
      n = fscanf( f, "%lf", &d );
      zubr_lval.sym = install ("", NUMBER, d);

      return( NUMBER );
   }
   if( isalpha( c ) )
   {
      SYMBOL *sp;
      char sbuf[256], *p = sbuf;

      do
      {
         *p++ = c;
      } while( (c=getc( f )) != EOF && (isalnum( c )||c=='_') );

      ungetc( c, f );
      *p = '\0';

      if( (sp = lookup( sbuf ) ) == (SYMBOL *)0 )
         sp = install( sbuf, UNDEF, 0.0 );
      zubr_lval.sym = sp;

      return( sp->type == UNDEF ? VAR : sp->type );
   }

   if( c == '$' )
   {
      int n = 0;

      while( isdigit( c = getc( f ) ) )
         n = 10 * n + c - '0';
      ungetc( c, f );

      if( n == 0 )
         warning( "strange $...", (char *)0 );
      zubr_lval.narg = n;

      return( ARG );
   }

   if( c == '"' )
   {
      SYMBOL *sp;
      char sbuf[256], *p;

      for( p =sbuf; (c = getc( f )) != '"'; p++ )
      {
         if( c == '\n' || c == EOF )
            warning( "missing quote", "\"" );
         if( p >= sbuf + sizeof( sbuf ) - 1 )
         {
            *p = '\0';
            warning( "string too long", sbuf );
         }
         *p = backslash( c );
      }
      *p = '\0';

      zubr_lval.sym = (SYMBOL *)emalloc( strlen( sbuf ) + 1 );
      strcpy( (char *)zubr_lval.sym, sbuf );

      return( STRING );
   }

   if( c == '\n' ) { lineno++; return( c ); }


   switch( c )
   {
      case '+': return follow( '=', ADDEQ, '+' );
      case '-': return follow( '=', SUBEQ, '-' );
      case '*': return follow( '=', MULEQ, '*' );
      case '/': return follow( '=', DIVEQ, '/' );

      case '>': return follow( '=', GE,   GT );
      case '<': return follow( '=', LE,   LT );
      case '=': return follow( '=', EQ,  '=' );
      case '!': return follow( '=', NE,  NOT );
      case '|': return follow( '|', OR,  '|' );
      case '&': return follow( '&', AND, '&' );

      default : return( c );
   }

}

void SetProcedure(char *name,int narg,double *par)
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
void SetVariable(char *name,double val)
{
SYMBOL *sp;
    if( (sp = lookup( name ) ) == (SYMBOL *)0 )
         sp = install( name, VAR, val );
    else {sp->type=VAR;sp->u.val=val;}
//PrintSymbols(sp);    
	code( (INST)varpush );
	code( (INST) sp );
	code( (INST)eval );

}
void ReadProgram(char *name )
{
//   progname = argv[0];

   input_file_name = name;

   if( (input_file = fopen( input_file_name, "rb" )) == NULL )
   {
      printf( "Cannot open file: %s\n", input_file_name );
      exit( 1 );
   }

   init();
   for( initcode(); zubr_parse(); initcode() )
   {
      execute( progbase );
   }


   if( input_file ) fclose( input_file );

   return( 1 );
}
void Execute()
{
	execute(progbase);
}
int main1( int argc, char *argv[] )
{
double par[1];par[0]=5;
	ReadProgram(argv[1]);
//	SetVariable("test_var",100);
	SetVariable("test_var",1000.54634);
	SetProcedure("test",1,&par[0]);
	Execute();
	


   return( 0 );
}

/*******************  END OF FILE MAIN.Y  *********************/
