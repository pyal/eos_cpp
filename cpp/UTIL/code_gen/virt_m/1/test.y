
/**************************************************************
  MAIN.Y
 **************************************************************/

%{
#include HEAD
%}

%union
{
   SYMBOL *sym;
   INST   *inst;
}
%token <sym>    NUMBER  VAR BLTIN UNDEF STRING
%type  <inst>   expr assign
%right '='
%left  '+' '-'   /* left associative, same precedence */
%left  '*' '/'   /* left associative, higher precedence */
%right '^'       /* power */

%start list

%%
list:    /* nothing */
       | list '\n'
       | list assign '\n' { code2( (INST)void_pop, STOP ); return( 1 ); }
       | list error  '\n'
         {
            zubr_errok;
         }
       ;

expr:    NUMBER       { code2( (INST)constpush, (INST)$1 ); }
       | VAR          { code3( (INST)varpush, (INST)$1, (INST)eval ); }
       | assign       { code3( (INST)varpush, (INST)$1, (INST)eval ); }
       | expr '+' expr             { code( (INST)Yadd );   }
       | expr '-' expr             { code( (INST)Ysub );   }
       | expr '*' expr             { code( (INST)Ymul );   }

       | expr '/' expr             { code( (INST)Ydiv );   }

       | expr '^' expr             { code( (INST)power );  }
       ;

assign:  VAR '=' expr   { code3( (INST)varpush, (INST)$1, (INST)assign ); }
       ;
%%

/********************  END OF GRAMMAR  ***********************/

Program
