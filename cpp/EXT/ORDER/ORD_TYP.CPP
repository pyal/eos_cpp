#include "ord_typ.h"

int RegisterField(char *name,int Length,int Pos,FieldType *&Field)
{
//   char name[256];in>>name;
   if (Stricmp(name,"String")==0) Field=new FieldString;
   else
	   if (Stricmp(name,"Real")==0) Field=new FieldReal;

	else
	   if (Stricmp(name,"Long")==0) Field=new FieldLong;

	   else
		   if (Stricmp(name,"Int")==0) Field=new FieldInt;
		   else
    { cout<<" RegisterField; There is no such function \n"<<name<<"\n";return 0; }
//   Func->ReadIni(in);
	Field->Pos=Pos;Field->Length=Length;
	return 1;
  };

int *MyIndexCollection::SetIndex()
{ 
	int Num=NumElement();
	delete IndexAr;IndexAr=new int[Num+1];
	for (int k=1;k<=Num;k++) IndexAr[k]=k;
	return IndexAr;
}
int seek_eof(istream &i)
  {
   char ch=' ';
   int ret=0;
   while ( (i) && ((ch==' ') || (ch=='\n')) ) i.get(ch);
//   if (ch==0) return 1;
   if (i) { i.putback(ch);}
   if (!i) ret=1;
   return ret;
  };
int seek_eoln(istream &i)
  {
   char ch=' ';
   int ret=0;
   while ( (i) && (ch==' ') ) i.get(ch);
   if (ch=='\n') ret=1;
   else if (i) i.putback(ch);
   if (!i) ret=1;
   return ret;
  };
