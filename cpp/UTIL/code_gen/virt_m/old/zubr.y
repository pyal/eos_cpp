
%{
#include "zubr_use.h"



char *progname;

FILE *input_file;
char *input_file_name;

int lineno;

program *current_prog;

static unsigned long current_decl_flag;
static unsigned long current_decl_type;

%}

%union
{
   unsigned long      itype;
   z_symbol          *sym;
   enum tetrade_code  code;
}

%right <code> ASSIGN 907 '='
%right <code> '?' ':'
%left  <code> OROR 901
%left  <code> ANDAND 902
%left  <code> '|'
%left  <code> '^'
%left  <code> '&'
%left  <code> EQCOMPARE 906
%left  <code> ARITHCOMPARE 905
%left  <code> LSHIFT 910  RSHIFT 911
%left  <code> '+' '-'
%left  <code> '*' '/' '%'
%right <code> UNARY 908  PLUSPLUS 903  MINUSMINUS 904
%left  HYPERUNARY 909
%left  <code> POINTSAT 912 '.' '(' '['


%type <code> unop


%token <itype> TYPESPEC 800     TYPE_QUAL 802 
//SCSPEC 801

%token <sym> IDENTIFIER 500 CONSTANT 502 VARIABLE 503
%token <sym> STRING 505
%token <sym> MTH_BUILTIN 600


%type <itype> decl_mods type_spec 

%type <sym> expr nonull_exprlist expr_no_commas cast_expr primary unary_expr
%type <sym> string
%type <sym> init initdcl
%type <sym> declarator 

%start program
%%
program:          { ; /* finish file */ }
       | extdefs
       ;

extdefs:
         extdef
       | extdefs extdef
       ;

extdef:
         datadef
       ;

datadef:  initdecls ';'
         {
            current_decl_flag = 0;
            fprintf( stderr,
            "%s: %s:%d: WARNING: data definition with no type or storage class\n",
                     progname, input_file_name, lineno );
         }
       | decl_mods type_spec initdecls ';'
         {
            current_decl_type = 0;
            current_decl_flag = 0;
         }
       | type_spec initdecls';'
         {
            current_decl_type = 0;
            current_decl_flag = 0;
         }
       | decl_mods ';'
         {
            current_decl_flag = 0;
            current_decl_type = 0;
            fprintf( stderr,
            "%s: %s:%d: WARNING: empty declaration\n",
                     progname, input_file_name, lineno );
         }
       | error ';' { current_decl_type = 0;current_decl_flag=0; zubr_errok; }
       | ';'
         {
            current_decl_type = 0;
            current_decl_flag = 0;
            fprintf( stderr,
            "%s: %s:%d: WARNING: not allow extra ';' outside of a function\n",
                     progname, input_file_name, lineno );
         }
       ;

decl_mods:
         TYPE_QUAL
         { $$ = (current_decl_flag |= $1); }
       ;

type_spec:
         TYPESPEC
         { $$ = (current_decl_type = $1); }
       ;



initdecls:
         initdcl
       | initdecls ',' initdcl
       ;

initdcl:
         declarator '='
         {
            init_decl_prog(); /* set current_prog to init_declaration_prog */
         }
         init
         {
            $$ = build_modify_expr( $1, NOP_EXPR, $4 );
            execute_program( current_prog ); /* exec init_declaration_prog */
            /* DELETE TMP VARIABLE & CONSTANT */
            free_program( current_prog ); /* FREE init_declaration_prog */
            goto_text_prog(); /* RETURN TO GENERATION TEXT PROGRAM */
         }
       | declarator
       ;

declarator:
         '*'  declarator %prec UNARY
         {
            $$ = make_pointer_declarator( $2 );
         }
       | declarator '[' expr ']' %prec '.'
         {
            $$ = build_array( $1, $3 );
         }
       | declarator '[' ']' %prec '.'
         {
            $$ = build_array( $1, (z_symbol *)0 );
         }
       | IDENTIFIER
         {
			$$ = init_identifier($1,current_decl_type,current_decl_flag);
         }
       ;


init:
         expr_no_commas
         { $$ = $1;}
       ;




unop:
         '&'        { $$ = ADDR_EXPR;         }
       | '-'        { $$ = NEGATE_EXPR;       }
       | '+'        { $$ = CONVERT_EXPR;      }
       | PLUSPLUS   { $$ = PREINCREMENT_EXPR; }
       | MINUSMINUS { $$ = PREDECREMENT_EXPR; }
       | '~'        { $$ = BIT_NOT_EXPR;      }
       | '!'        { $$ = TRUTH_NOT_EXPR;    }
       ;

expr:
         nonull_exprlist
       ;

nonull_exprlist:
         expr_no_commas
       | nonull_exprlist ',' expr_no_commas
                             { $$ = $3; }
       ;

unary_expr:
         primary
       | '*' cast_expr %prec UNARY
         { $$ = build_indirect_ref( $2, "unary *" ); }
       | unop cast_expr %prec UNARY
         { $$ = build_unary_op( $1, $2 ); }
       ;

cast_expr:
         unary_expr
       ;

expr_no_commas:
         cast_expr
       | expr_no_commas '+' expr_no_commas
                { $$ = build_binary_op( $2, $1, $3 ); }
       | expr_no_commas '-' expr_no_commas
                { $$ = build_binary_op( $2, $1, $3 ); }
       | expr_no_commas '/' expr_no_commas
                { $$ = build_binary_op( $2, $1, $3 ); }
       | expr_no_commas '*' expr_no_commas
                { $$ = build_binary_op( $2, $1, $3 ); }
       | expr_no_commas '%' expr_no_commas
                { $$ = build_binary_op( $2, $1, $3 ); }
       | expr_no_commas '&' expr_no_commas
                { $$ = build_binary_op( $2, $1, $3 ); }
       | expr_no_commas '|' expr_no_commas
                { $$ = build_binary_op( $2, $1, $3 ); }
       | expr_no_commas '^' expr_no_commas
                { $$ = build_binary_op( $2, $1, $3 ); }
       | expr_no_commas LSHIFT expr_no_commas
                { $$ = build_binary_op( $2, $1, $3 ); }
       | expr_no_commas RSHIFT expr_no_commas
                { $$ = build_binary_op( $2, $1, $3 ); }
       | expr_no_commas ARITHCOMPARE expr_no_commas
                { $$ = build_binary_op( $2, $1, $3 ); }
       | expr_no_commas EQCOMPARE expr_no_commas
                { $$ = build_binary_op( $2, $1, $3 ); }
       | expr_no_commas ANDAND expr_no_commas
                { $$ = build_binary_op( ANDIF_EXPR, $1, $3 ); }
       | expr_no_commas OROR expr_no_commas
                { $$ = build_binary_op( ORIF_EXPR, $1, $3 ); }
       | expr_no_commas '=' expr_no_commas
                { $$ = build_modify_expr( $1, NOP_EXPR, $3 ); }
       | expr_no_commas ASSIGN expr_no_commas
                { $$ = build_modify_expr( $1, $2, $3 ); }
       | MTH_BUILTIN '(' expr_no_commas ')'
         {
            $$ = build_mth_builtin1_fn( $1, $3 );
         }
       | MTH_BUILTIN '(' expr_no_commas ',' expr_no_commas ')'
         {
            $$ = build_mth_builtin2_fn( $1, $3, $5 );
         }
       ;

primary: VARIABLE
       | CONSTANT
       | string
       | '(' expr ')'
         { $$ = $2; }
       | primary '[' expr ']' %prec '.'
         {
            $$ = build_array_ref( $1, $3 );
         }
       | primary PLUSPLUS
         { $$ = build_unary_op( POSTINCREMENT_EXPR, $1 ); }
       | primary MINUSMINUS
         { $$ = build_unary_op( POSTDECREMENT_EXPR, $1 ); }
       ;

string:
         STRING
       | string STRING
         { $$ = build_chainon_string( $1, $2 ); }
       ; 





%%
/*********************  END OF GRAMMAR  ***********************/


void zubr_error( char *s )
{
   printf( "%s\n" );
}

int main_zubr( int argc, char *argv[] )
{
   int c;
//   bucket b, *fn, *rc, *op1, *op2;


   progname = argv[0];
   if (argc < 2)
   {
      fprintf( stderr, "usage: %s file.name\n", progname );
      exit( 1 );
   }

   input_file_name = argv[1];

   if( (input_file = fopen( input_file_name, "rb" )) == NULL )
   {
      printf( "Cannot open file: %s\n", input_file_name );
      exit( 1 );
   }

   init();

   zubr_parse();

   if( input_file ) fclose( input_file );


   execute_program( current_prog );


   free_program( current_prog );
   free_symbol_table();


   return( 0 );
}

/**********************  END OF MAIN.Y  ***********************/

