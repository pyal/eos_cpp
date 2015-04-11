#include <alloc.h>

#ifndef __FOBJ_H
#include "fobj.h"
#endif

LstCl::LstCl()
  {
   FstN=new Note;LstN=new Note;CrrN=FstN;
   FstN->PrevN=NULL;FstN->NextN=LstN;FstN->item=NULL;
   LstN->NextN=NULL;LstN->PrevN=FstN;LstN->item=NULL;
  };
LstCl::~LstCl()
  {
   LstReset();
   Item* it;
   while (NULL!=(it=++(*this)) ) 
      { delete CrrN->PrevN;
      delete it;    
      };
   delete CrrN->PrevN; delete CrrN;
  };

void LstCl::NtSet(Note *seting,Note *prev,Note *next,Item *it )
  {
   if (seting!=NULL)
    {
     if ( it==NULL ) it=seting->item;
     seting->NextN=next;seting->PrevN=prev;seting->item=it;
    }
  };
Item* LstCl::operator ++()
  {
   if (CrrN!=LstN) CrrN=CrrN->NextN;
   return (CrrN->item);
  };
Item* LstCl::operator --()
  {
   if (CrrN != FstN)  CrrN=CrrN->PrevN;
   return (CrrN->item);
  };

LstCl::operator Item* ()
  {
   return (CrrN->item);
  };

void LstCl::LstAdd(Item *item)            //=(Item&)(*(CrrN->item)))
  {
   Note *note=new Note;
   NtSet(note,CrrN,CrrN->NextN,item);
   NtSet((CrrN->NextN),note,(CrrN->NextN->NextN));
   NtSet(CrrN,(CrrN->PrevN),note);
   CrrN=CrrN->NextN;
  };

Item* LstCl::LstDetach(Item *it) //  (Item& Del)  (Item&)(*(CrrN->item)))
  {
   Item *ret=NULL;
   if ( it!=NULL ) if ( LstFind(it)!=0 ) goto exi;
   if ((CrrN!=LstN) && (CrrN!=FstN))
     {
      ret=CrrN->item;
      NtSet((CrrN->PrevN),(CrrN->PrevN->PrevN),CrrN->NextN);
      NtSet((CrrN->NextN),(CrrN->PrevN),CrrN->NextN->NextN);
      Note *note=CrrN->PrevN;
      delete CrrN;
      delete ret;
      CrrN=note;
     }
   /* else printf(" Error tried to delete not exist el from LstCl "); */

   exi: return ret;
  };
void LstCl::LstReset(int b )
  {
   if ( b == 0 ) {CrrN=FstN;/* ++(*this); */}
   else          {CrrN=LstN;/* --(*this); */};
  };
int  LstCl::LstFind(Item *it)   // Searches item by addr, if found ret=0
  {
   Note *note=CrrN;
   int ret=1;
   LstReset();
   while ( (++(*this))!=NULL )
    {
     if (it==(CrrN->item))
       {
        note=CrrN;ret=0;break;
       }
    }
   CrrN=note;
   return ret;
  };

ArrCl::ArrCl(int sizeIt,int F,int L,void *P)
  {
   Ptr=P;Fst=F;Lst=L;
   SizeE=sizeIt;
   if (((F!=0) || (L!=0)) && (Ptr==0))
      {
        Ptr=(void *)calloc(Lst-Fst+1,SizeE);
        if (Ptr==NULL)
             { cout<<"Cannot allocate block size"<<(Lst-Fst+1)*SizeE;exit(1);}
      }
  };
ArrCl::~ArrCl()
  {
   ArrDel();
  };
void ArrCl::ArrSet(int F,int L,void *P)
  {
   if ( (L-F)!=(Lst-Fst) && (P!=NULL) )
     {  ArrDel();
        Ptr=(void *)calloc(L-F+1,SizeE);
        if (Ptr==NULL)
           {cerr<<"Cannot allocate block size"<<(L-F+1)*SizeE;exit(1);}
     }
   Lst=L;Fst=F;
  };
void ArrCl::ArrDel() { free(Ptr); };
Item* ArrCl::operator[](int num)
  { if ( !(InLim(num)) )
      { cerr<<"ArrCl = wrong argument= "<<num;exit(1);}
    char *c=(char *)Ptr;
    return (Item*)(c+(num-Fst)*SizeE);
  };

/*
void main()
  {LstCl l;
   Item *i;
   if (  (i=&(++l) )==NULL) ;
  }
*/
