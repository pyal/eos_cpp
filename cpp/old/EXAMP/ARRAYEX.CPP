
// uses fobj;
#include <malloc.h>
#include <conio.h>
#include <iostream.h>
#include <string.h>
#include "fobj.h"

/*

void main()
  {
   char c;
   char buf[255];
   while((c=getche())!='e')
     {
      cout<<"\n";
      cin>>buf;
      cout<<buf<<"\n";
     }
   };

struct El { char *str;};
void main()
  {
   char c;
   char buf[255];
   int Num,Fst,Lst;
   int n;
        cout<<" Free "<<farcoreleft();
   cout<<" Enter Beg\n";
   cin>>Fst;cout<<" \nEnter Last\n";cin>>Lst;
   ArrCl a(Lst,Fst,sizeof(El));
        cout<<" Free "<<farcoreleft();
   cout<<"\nEnter Command\n";
   while((c=getche())!='e')
     {
      switch(c) {
   case 'a' :
        cout<<"Num to add? ";cin>>n;cout<<"Str";cin>>buf;
        ((El&)(a[n])).str=new char[strlen(&buf[0])+1];
        strcpy( ((El&)(a[n])).str,&buf[0] );break;
   case 'd' :
        cout<<"Num to del? ";cin>>n;
        delete ((El&)(a[n])).str;
        //String(((El&)(a[n])).str);
        break;
   case 'g' :
        cout<<"Num to go? ";cin>>n;
        cout<<((El&)(a[n])).str;
        break;
   case 'p' :
        int f,l;
        cout<<" Free "<<farcoreleft();
        cout<<"F Num ? ";cin>>f;
        cout<<"L Num ? ";cin>>l;
        a.ArrSet(f,l);
        cout<<" Free "<<farcoreleft();
        break;
   default  :cout<<"(a)dd\n(d)elete\n(g)oto\n(p)rint\n(e)xit\n\n";
		 }
   cout<<"\nEnter Command\n";
     }
  }
*/
struct El:Item { char str[256];};
void main()
  {
   char c;
   char *buf;
   int Num,Fst,Lst;
   int n;

   cout<<" Free "<<coreleft(); cout<<" Free "<<farcoreleft();
   cout<<" Enter Beg\n";

   cin>>Fst;cout<<" \nEnter Last\n";cin>>Lst;

   ArrCl a(sizeof(El),Fst,Lst);
   cout<<" Free "<<farcoreleft();   cout<<" Free "<<coreleft();
   cout<<"\nEnter Command\n";
   while((c=getche())!='e')
     {
      switch(c) {
   case 'a' :
      	cout<<"Num to add? ";cin>>n;cout<<"Str:";
      	cin>>((El*)(a[n]))->str;
      	break;
   case 'm' :
        cout<<" Free "<<farcoreleft(); cout<<" Free "<<coreleft();
       	break;
   case 'g' :
       	cout<<"Num to go? ";
       	cin>>n;
	buf=(char*)(&(( (El*)(a[n]) )->str));
       	cout<<buf;
       	break;
   case 'r' :
       	int f,l;
       	cout<<" Free "<<farcoreleft()<<"\n"; cout<<" Free "<<coreleft();
       	cout<<"F Num ? ";cin>>f;
       	cout<<"L Num ? ";cin>>l;
       	a.ArrSet(f,l);
        cout<<" Free "<<coreleft();cout<<" Free "<<farcoreleft();
       	break;
   default  :cout<<"(a)dd\n(m)emory\n(g)oto\n(r)eset\n(e)xit\n\n";
		 }
   cout<<"\nEnter Command\n";
     }
  }























   //

