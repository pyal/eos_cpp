
/**************************************************************
  MAIN.Y
 **************************************************************/

%{

#include "zubr_import.h"
#define code2(c1,c2) code(c1); code(c2)
#define code3(c1,c2,c3) code(c1); code(c2); code(c3)

%}

%union
{
   SYMBOL sym;
   INST   *inst;
}
%token <sym>    NUMBER VAR BLTIN UNDEF STRING
%token <sym>    ADDEQ SUBEQ MULEQ DIVEQ
%token <sym>    PRINT READ
%type  <inst>   expr assign 
%type  <inst>   prlist stmt
%right '='
%right ADDEQ SUBEQ MULEQ DIVEQ
%left  '+' '-'   /* left associative, same precedence */
%left  '*' '/'   /* left associative, higher precedence */
%left  UNARYMINUS 
%right '^'       /* power */

%start list

%%

list:    /* nothing */
       | list '\n'
       | list assign '\n' { code2( (INST)void_pop, STOP ); return( 1 ); }
       | list expr   '\n' { code2( (INST)void_pop, STOP );    return( 1 ); }
       | list stmt   '\n' { code( STOP );    return( 1 ); }
       | list error  '\n'
         {
            zubr_calculator_zubr_error(NULL);
         }
       ;

expr:    NUMBER       { code2( (INST)constpush, (INST)$1 ); }
       | VAR          { code3( (INST)varpush, (INST)$1, (INST)eval ); }
       | assign       { code3( (INST)varpush, (INST)$1, (INST)eval ); }
       | READ '(' VAR ')'
         { $$ = code2( (INST)varread, (INST)$3 ); }
//       | BLTIN '(' expr ')'   { code2( (INST)bltin, (INST)$1->u.ptr); }
       | BLTIN '(' expr ')'   { code2( (INST)bltin, (INST)$1); }
       | BLTIN '(' expr ',' expr ')'
         {
            code2( (INST)bltin2, (INST)$1 );
         }
       | expr '+' expr             { code( (INST)Yadd );   }
       | expr '-' expr             { code( (INST)Ysub );   }
       | expr '*' expr             { code( (INST)Ymul );   }

       | expr '/' expr             { code( (INST)Ydiv );   }

       | expr '^' expr             { code( (INST)power );  }
       | '(' expr ')'              { $$ =  $2; }
       | '-' expr %prec UNARYMINUS { code( (INST)negate ); }
       ;



assign:  VAR '=' expr   { code3( (INST)varpush, (INST)$1, (INST)assign ); }
       | VAR ADDEQ expr { code3( (INST)varpush, (INST)$1, (INST)Yaddeq ); }
       | VAR SUBEQ expr { code3( (INST)varpush, (INST)$1, (INST)Ysubeq ); }
       | VAR MULEQ expr { code3( (INST)varpush, (INST)$1, (INST)Ymuleq ); }
       | VAR DIVEQ expr { code3( (INST)varpush, (INST)$1, (INST)Ydiveq ); }
       ;
prlist:  expr              { code( (INST)prexpr ); }
       | STRING            { $$ = code2( (INST)prstr, (INST)$1 ); }
       | prlist ',' expr   { code( (INST)prexpr ); }
       | prlist ',' STRING { $$ = code2( (INST)prstr, (INST)$3 ); }
       ;
stmt:    PRINT prlist       { $$ = $2; }
       ;
       
%%

/********************  END OF GRAMMAR  ***********************/


 