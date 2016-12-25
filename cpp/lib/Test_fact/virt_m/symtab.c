
/**************************************************************
  SYMBOL.C
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "init.h"
#include "symtab.h"
#include "mathf.h"
#include "code.h"
#include "main.h"

#include "z_tab.h"

static SYMBOL *symlist = 0; /* указатель на таблицу символов */

char * emalloc( unsigned n )
{
   char *p;

   p = (char *)malloc( n );
   if( p == 0 )
   {
      /*
        при неудачном выполнении malloc()
        завершаем работу
       */
      printf( "calc: ERROR: out of memory\n" );
      exit( 1 );
   }

   return( p );

}


SYMBOL *lookup( char *s )
{
   SYMBOL *sp;

   for( sp = symlist; sp != (SYMBOL *)0; sp = sp->next )
      if( strcmp( sp->name, s ) == 0 ) return( sp );

   return( 0 );  /* запись не найдена */
}


SYMBOL *install( char *s, int t, double d )
{
   SYMBOL *sp;

   sp = (SYMBOL *)emalloc( sizeof( SYMBOL ) );
   sp->name  = emalloc( strlen( s ) + 1 );

   strcpy( sp->name, s );

   sp->type  = t;
   sp->u.val = d;
   sp->next  = symlist;  /* alloc in begin of list */
   symlist   = sp;

   return( sp );
}

void PrintInitSymbol(){printf("Inited symbols:\n");PrintSymbols(symlist);};
/*******************  END OF FILE SYMTAB.C  *******************/
