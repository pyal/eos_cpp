

#ifndef __FOBJ_H
#define __FOBJ_H

#include <process.h>
#include <iostream.h>
#include <mem.h>

struct Item 
  { virtual  ~Item(){};};
      //        struct BlData : Item {int a};

struct Note { Note *NextN,*PrevN;Item *item;};
class LstCl
  {
   private:
     Note *FstN,*LstN,*CrrN,*Mark;
     void NtSet(Note *seting,Note *prev,Note *next,Item *it=NULL);
   public:
     LstCl();
     ~LstCl();
     Item* operator ++();
     Item* operator --();
     operator Item*/**/();
     void LstAdd(Item *item);  //Adds item to the right from CrrN and moves
                               //there

     // This Proc may be extr in separtate class
     Item* LstDetach(Item *it=NULL);
                               //Detaches and deletes Item& and moves 
                               //CrrN to the prev pos
                               //Returns Item& of detached if NULL = > err
     int  LstFind(Item *it);   // Searches item by addr, if found ret=0
                               //Else Returns to the curr pos
     void LstReset(int b=0);   //Returns to Zero El ( not exist )
                               //if b#0 - > Returns to the last El
     void LstMarkPos()  { Mark=CrrN;};
     void LstResetPos() { CrrN=Mark;};

  };
class ArrCl
  {
   private:
     int Fst,Lst,SizeE;
     void *Ptr;
     int InLim(int n) { return ((n>=Fst) && ( n<=Lst )); };
   public:
     ArrCl(int sizeIt,int F=0,int L=0,void *P=NULL);
//       {Ptr=P;Fst=F;Lst=L;SizeE=sizeIt; };
     ~ArrCl();
     void ArrSet(int F,int L,void *P=NULL);
     void ArrDel();
     Item* operator[](int num);
     int ArrFst() { return Fst; };
     int ArrLst() { return Lst; };
   };

#endif
