#include <lib\precompiled\Messb.h>
#include "faregist.h"
//=====================================================================
//=========================    Registration      ======================
//=====================================================================


int FARegister(istream &in,FAIO *&Func)
{
  if (!in) {cout<<" bad stream in FARegister\n";return 0;}
	char name[256];in>>name;
	ClassDesc* descr=ClassDesc::name_to_class_desc(name);
	Func=0;
	if (!descr) return 0;
	if (Stricmp(descr->category(),"FAIO")==0) 
	{Func=dynamic_cast<FAIO*>(descr->create());Func->Input(in);return 1;}
	if ( (Stricmp(descr->category(),"FAZer")!=0) 
     && (Stricmp(descr->category(),"FAZer_Test")!=0) ) return 0;

    //FASingleIO *F=new FASingleIO;
    //F->Func=dynamic_cast<FAZer*>(descr->create());
    //Func=dynamic_cast<FAIO*>(F);   

	Func=FAIO::create( dynamic_cast<FAZer*>(descr->create()) );
	Func->Input(in);
  return 1;
}
/*
int FARegister(fstream &in,FAIO *&Func)
  {
//cout<<" Stream "<<in.bad()<<"  "<<in.fail()<<"\n";
   char name[256];in>>name;
   int ret;
   if (!(ret=FAStdRegister(in,Func,name)))
#ifdef __FAMessbRegist_H
     if (!(ret=FAMessbRegister(in,Func,name)))
#endif
#ifdef __FAStateRegist_H
     if (!(ret=FAStateRegister(in,Func,name)))
#endif
#ifdef __TMSTD_H
     if (!(ret=FATMRegister(in,Func,name)))
#endif
       {
        cout<<" There is no such function "<<name<<" I\n";
       }
   return ret;
  };
*/
