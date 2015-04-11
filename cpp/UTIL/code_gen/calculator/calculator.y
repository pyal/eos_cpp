
/**************************************************************
  MAIN.Y
 **************************************************************/

%{
#include <lib\precompiled\calc_lib.h>
#include "zubr_import.h"

%}

%union
{
   SYMBOL sym;
}
%token <sym>    IDENT NUMBER STRING VAR TYPE
/* %token <sym>    ADDEQ SUBEQ MULEQ DIVEQ */
%type  <sym>   expr assign declare   
%type  <sym>   func args arr decl_lst
%right '='
//%right ADDEQ SUBEQ MULEQ DIVEQ
%left  '+' '-'   /* left associative, same precedence */
%left  '*' '/'   /* left associative, higher precedence */
%left  UNARYMINUS 
%right '^'       /* power */

%start list

%%

list:    /* nothing */
       | list '\n'
       | list assign '\n' { EndStmt($2);    }
       | list func   '\n' { EndStmt($2);    }
       | list error  '\n' { zubr_calculator_zubr_error(NULL); }
       | list expr   '\n' { EndStmt($2);    } /* temp only */
       | list declare '\n' {EndStmt($2); }
       ;



expr:    
		NUMBER    
       | VAR          
       | STRING       
       | arr          
       | func         
       | assign       /*{ $$ = $1; }*/
       | expr '+' expr             { $$ = AddOper($1,$3,'+');   }
       | expr '-' expr             { $$ = AddOper($1,$3,'-');   }
       | expr '*' expr             { $$ = AddOper($1,$3,'*');   }
       | expr '/' expr             { $$ = AddOper($1,$3,'/');   }
       | expr '^' expr             { $$ = AddOper($1,$3,'^');   }
       | '(' expr ')'              { $$ =  $2; }
       | '-' expr %prec UNARYMINUS { $$ = AddOper($2,NULL,'-');   }
       ;

assign: 
			VAR '=' expr		       { $$ = AddOper($1,$3,'=');   }
		| declare '=' expr		   { $$ = AddOper($1,$3,'=');   } 
       ;
args:    
			 expr                     {ListStart();ListAdd($1);}
        | args ',' expr            {ListAdd($3);}
        ;
        
func:     
		  IDENT '(' args ')'       {$$ = AddFunc($1);}
        | IDENT '(' ')'            {ListStart();$$ = AddFunc($1);}
		| VAR '(' args ')'       {$$ = AddFunc($1);}
        | VAR '(' ')'            {ListStart();$$ = AddFunc($1);}
        ;
        
arr:     
			VAR '[' expr ']'          {$$ = GetEl($1,$3);}
        | arr '[' expr ']'         {$$ = GetEl($1,$3);}
        ;
decl_lst:
			IDENT					{ListStart();ListAdd($1);}
		|	decl_lst ',' IDENT			{ListAdd($3);}
		;
declare:    
			TYPE IDENT             {$$ = DefineVar($1, $2);}        
		|    TYPE decl_lst              {$$ = DefineVar($1);}		
		;	
/*		| IDENT '=' expr		   { $$ = AddOper($1,$3,'=');   }   */
%%

/********************  END OF GRAMMAR  ***********************/


 