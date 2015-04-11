//
// Interface Dependencies ---------------------------------------------------
#define __FOBJEX_H

#ifndef __FOBJ_H
#include "fobj.h"
#endif

#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <mem.h>

// End Interface Dependencies ------------------------------------------------
char *gets(){char string[80];char *p;
	      printf("/nInput a string: ");
	      p= new char[strlen(gets(string))+1];
	      strcpy(p,string);
	      return p;
	    };
struct Reg_Block : virtual public Item
  {
   char * Name;
   Reg_Block()
     {  Name=gets();
  printf("/nThe string input was:  %s\n",Name);
     };
   void printOn()
    {printf(Name);};
   ~Reg_Block()  { delete Name; };
  };

