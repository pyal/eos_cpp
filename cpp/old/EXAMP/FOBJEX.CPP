//  uses fobj;
//
// Interface Dependencies ---------------------------------------------------
#ifndef __FOBJ_H
#include "fobj.h"
#endif
#include <iostream.h>
#include <conio.h>
#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mem.h>
#include <alloc.h>

// End Interface Dependencies ------------------------------------------------
#define LST   ((Reg_Block*)(void *)(Item*)lst )
#define PLLST ((Reg_Block*)(void *)(Item*)++lst )
#define MILST ((Reg_Block*)(void *)(Item*)--lst )

char *gets()
      { 
        char string[80];char *p;
        printf("Input a string: ");
        p=strdup(gets(string));
	printf("\n");
	      return p;
	    };
struct Reg_Block :  public Item
  {
   char * Name;
   Reg_Block()
     {
      Name=gets();
      printf("The string input was:  %s\n",Name);
     };
   void printOn()
    {printf("%s\n",Name);};
   ~Reg_Block()
      {
     cout<<"coreleft: "<<(unsigned long)coreleft()<<"\n";
       delete Name;
     cout<<"coreleft: "<<(unsigned long)coreleft()<<"\n";
      };
  };

// End Implementation Dependencies -------------------------------------------
void rbwalk(LstCl &lst)
  {
    char c;
    while((c=getche())!='e')
      {
       printf("\n");
       switch (c) {
   case 'n' :if ( PLLST==NULL ) {lst.LstReset();PLLST;}break;
   case 'r' :lst.LstReset();PLLST;break;
   case 'p' :if ( MILST==NULL ) {lst.LstReset(1);MILST;}break;
	 default  :printf("(n)ext\n(r)estart\n(p)rev\n(e)xit\n----------\n");
                   }
       LST->printOn();
      }
  };
void rbgo(LstCl &lst)
  {
   int n;
   cout<<"Enter Num to go: ";cin>>n;cout<<"\n";
   lst.LstReset();
   for (int k=1;k<n;k++) if (PLLST==NULL) { lst.LstReset();break;}
   PLLST->printOn();
  };

void rbprint(LstCl &lst)
  {
    lst.LstMarkPos();
    lst.LstReset();
    while (PLLST!=NULL) LST->printOn();
    lst.LstResetPos();     
  };
void del(LstCl &lst)
  {
    lst.LstReset();
    while (PLLST!=NULL) delete LST;
   };
void main()
  {
    cout<<"coreleft: "<<coreleft()<<"\n";
    char c;
    LstCl *lst1=new LstCl;
    LstCl &lst=*lst1;
    Reg_Block *rb;
    while((c=getche())!='e')
      {
       printf("\n");
       cout<<"coreleft: "<<coreleft()<<"\n";
       switch(c) {
   case 'a' :rb=new Reg_Block;lst.LstAdd(rb);break;
   case 'd' :rbgo(lst);
//       delete LST;
	     lst.LstDetach();break;
   case 'g' :rbgo(lst);break;
   case 'w' :rbwalk(lst);break;
   case 'p' :rbprint(lst);break;
   default  :printf("(a)dd\n(d)elete\n(g)oto\n(w)alk\n(p)rint\n(e)xit\n\n");
		 }
     }
//del(lst);
delete lst1;
     cout<<"coreleft: "<<(unsigned long)coreleft()<<"\n";
  };










