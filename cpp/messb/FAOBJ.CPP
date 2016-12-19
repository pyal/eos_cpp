#include <lib\precompiled\Messb.h>
#include "faobj.h"
//#include "malloc.h"

   
//=====================================================================
//=========================    FAZer  =================================
//=====================================================================

void FAZer::DeleteFAZer()
  {
   
   if (name!=NULL) 
     {
	  int k;
      for (k=1;k<=NumPar;k++) delete namepar[k];
	  if (HelpPresent()) {for (k=0;k<=NumPar;k++) delete helppar[k]; helppar[0]=0;}
      delete name;name=NULL;NumPar=0;
     }
  };
void Ident(strstream &out,int MargLft)
{
	out<<"\n";for (int k=1;k<=MargLft;k++) out<<" ";
}
void MakeNewLine(strstream &out,char *tmp,int &Cur,int Add,int MargLft,int MargRgt)
{
	int width=MargRgt-MargLft,pos=0,SkipIdent=0;
	while (Add>=width) 
	{
		out.write(&tmp[pos],width-Cur);Ident(out,MargLft);
		pos+=width-Cur;Add-=width-Cur;Cur=MargLft;
		SkipIdent=1;
	}
	if (!SkipIdent) Ident(out,MargLft);
	out.write(&tmp[pos],Add);out<<" ";Cur=Add+MargLft;
}
void OutFormatted(strstream &out,strstream &in,int MargFstLft,int MargLft,int MargRgt)
{
	char tmp[1024];
//	while (!(!in)) { in>>tmp;out<<tmp<<" ";}return;
	int Cur=0,Add=MargFstLft,FstLine=1;
	while (!(!in))
	{ in>>tmp;Add+=(int)strlen(tmp);
	  if (Cur+Add>MargRgt) MakeNewLine(out,&tmp[0],Cur,Add,MargLft,MargRgt);
	  else {out<<tmp<<" ";Cur+=Add+1;}
	  Add=0;
	}
//	out<<"\n";
	out.put((char)0);
}
void FAZer::DrawHelp(ostream &out)
{
	if (!HelpPresent()) return ;
	out<<"\n Begin Description of function :      <<"<<name<<">>\n\n";
	int k;
	char buf[10240],buf1[10240];
	strstream *store=new strstream(&buf[0],10240,ios::in),*get=new strstream(&buf1[0],10240,ios::out);

	(*store)<<helppar[0]<<"\n";store->put((char)0);
	OutFormatted(*get,*store,0,0,65);
	out<<&buf1[0]<<"\n\n";
	delete get;get=new strstream(&buf1[0],10240,ios::out);
	delete store;store=new strstream(&buf[0],10240,ios::in);
	for (k=1;k<=NumPar;k++) {
		out<<setw(12)<< namepar[k]<<" : ";
		(*store)<<helppar[k]<<"\n";store->put((char)0);

  	    OutFormatted(*get,*store,15,15,65);
		out<<&buf1[0]<<"\n";
		delete get;get=new strstream(&buf1[0],10240,ios::out);
    	delete store;store=new strstream(&buf[0],10240,ios::in);
//		get->rdbuf()->pubseekpos(0,ios::out);
//		store->rdbuf()->pubseekpos(0,ios::in);store->unlock();
//		get->seekp(0,ios::out);store->seekp(0,ios::in);
//		get->clear();store->clear();get->unlock();
	}
	out<<"\n End of Description of function :      <<"<<name<<">>\n\n";
	delete get;delete store;
}



//=====================================================================
//=========================    FAZerBase   ============================
//=====================================================================

int FAZerBase::SetStdPar_GrInt(char **namepar,double *valpar,char **helppar,int StartNum){
   int i=StartNum-1;
   namepar[1+i ]=strdup("Ground   ");valpar[1+i ]=0;	  helppar[1+i ]=strdup("Added Ground int");
   namepar[2+i ]=strdup("Intencity");valpar[2+i ]=10;	  helppar[2+i ]=strdup("Spectr multiplied by ");
   return 2;
}

void FAZerBase::ParStp(double *par,double *stp)
{
  for (int k=1;k<=NumPar;k++)
    if (stp[k]!=0) if (fabs(par[k])<=MathZer) stp[k]=0.1;
                  else stp[k]=par[k]*0.01;
};
void   FAZerBase::Clc(double *par,double *Xval,double *Res)
{
  ClcBase(par,Xval,Res);
  double Gr=par[1],Amp=par[2];
  for (int k=1;k<=Xval[0];k++) Res[k]=Gr-Amp*Res[k];
}

//=====================================================================
//=========================    FAIO   =================================
//=====================================================================

void FAIO::SaveInPar(ostream &output)
   {
    int N=GetNumPar();
    double *Par=new double [N+1];
    GetPar(Par);
    for (int k=1;k<=N;k++) output<<Par[k]<<" ";
    delete Par;
   };
void FAIO::GetInPar(istream &input)
   {
    int N=GetNumPar();
    double *Par=new double [N+1];
    for (int k=1;k<=N;k++) input>>Par[k];
    SetPar(Par);
    delete Par;
   };
void FAIO::list_all_functions(ostream &out)
{
	ClassDesc* tmp=ClassDesc::list_category_start("FAIO");
	while (tmp)
	{
        //Ref<DescribedClass> tmpr;
        Ref<FAIO> func;func<<tmp->create();
		func->DrawHelp(out);
        tmp=ClassDesc::list_category_next(tmp,"FAIO");
		out<<"\n\n";
	}
	//DescribedClass* tmp=ClassDesc::list_category_start("FAIO");
	//FAIO *func=dynamic_cast<FAIO*>(tmp);
	//while (func)
	//{
	//	func->DrawHelp(out);
 //       tmp=ClassDesc::list_category_next(func,"FAIO",1);
	//    func=dynamic_cast<FAIO*>(tmp);
	//	out<<"\n\n";
	//}
}
FAIO *FAIO::create(FAZer *clc_func)
   {
    FASingleIO *F=new FASingleIO;
    F->Func=clc_func;
    return dynamic_cast<FAIO*>(F);   
   }



//=====================================================================
//==========================    FASingleIO =============================
//=====================================================================


#ifndef WCPP_NET
DescribedClass * Dummy_FASingleIO(){return new FASingleIO();}
static ClassDesc FASingleIO_ClassDesc(typeid(FASingleIO),"FASingle",1,"FAIO",Dummy_FASingleIO);
#else
static ClassDesc FASingleIO_ClassDesc(typeid(FASingleIO),"FASingle",1,"FAIO",create<FASingleIO>);
#endif

void FASingleIO::Output(ostream &output)
  {
   char tmp[256];
   output<<Func->name<<"\n";
   Func->OutputInternPar(output);
   for (int k=1;k<=NumPar;k++)
     {
      strcpy(tmp,Func->namepar[k]);SetLength(tmp,15);
      output<<tmp<<"  ";
      for (int k1=1;k1<=MaxIter;k1++)
        {
         if (Stp[k1][k]==0)  output<<"   F   ";
         else           output     <<"   V   ";
        }
      output<<Par[k]<<"\n";
     }
  };

void FASingleIO::Input(istream &input)
  {
   Func->InputInternPar(input);
   char tmp[256],*tmp_=tmp;
   double tmp_d[MaxIterations+1],dtmp;
   if (MaxIter!=0) 
     {cout<<"FASingleIO::Input; Is not deleted. Exit.\n";exit(1);}
//   input>>tmp;  it was name for registration
//   if (Func!=NULL) DeleteFASingleIO();
//   if (!Register(input,Func)) 
//     {cout>>" Error registration. Halt.\n";exit(0);};
   NumPar=Func->GetNumPar();
//cout<<" FASingleIO::Input numpar "<<NumPar<<"\n";
   input>>tmp>>tmp;MaxIter=0;
   while ( !IsDouble(tmp_,dtmp,1) )
     {
      if (MaxIter++>=MaxIterations) 
        {cout<<"Number of iterations is more then "<<MaxIterations
             <<" Program halted\n";exit(1);}
      if (seek_eof(input)) {cout<<" Unexpected end of file\n";exit(1);}
      if (!Stricmp(tmp,"f")) tmp_d[MaxIter]=0;
      else                   tmp_d[MaxIter]=1;
      input>>tmp;tmp_=tmp;
//cout<<tmp_<<"\n";
     }
   if (MaxIter==0) {cout<<"FASingleIO::Input; MaxIter=0. Halt.\n";exit(1);}
//   Stp=new double*[MaxIter+1];
//cout<<" FASingleIO::Input MaxIter "<<MaxIter<<"\n";
   for (int k=1;k<=MaxIter;k++) 
     {Stp[k]=new double [NumPar+1];Stp[k][0]=NumPar;Stp[k][1]=tmp_d[k];}
   Par=new double [NumPar+1];Par[0]=NumPar;Par[1]=dtmp;
   for (int k=2;k<=NumPar;k++)
     {
      input>>tmp;
      for (int k1=1;k1<=MaxIter;k1++)
        {
         input>>tmp;
//cout<<tmp<<"\n";
         if (!Stricmp(tmp,"f")) Stp[k1][k]=0;
         else                   Stp[k1][k]=1;
        }
      input>>Par[k];
     }
   Func->ParStp(Par,Stp[1]);
//cout<<" FASingleIO::Input Par ";for (k=1;k<=Par[0];k++) cout<<Par[k]<<"  ";cout<<"\n";
   CurIter=1;
  };
   
void FASingleIO::DeleteFASingleIO()
  {
   if (!Par) return;
   delete Par;for (int k=1;k<=MaxIter;k++) {delete Stp[k];Stp[k]=NULL;}
   delete Func;Par=NULL;Func=NULL;NumPar=0;MaxIter=0;
  };

void FASingleIO::SetNewFunc(FAZer *func,int NumIter)
{
	DeleteFASingleIO();
	MaxIter=NumIter;Func=func;NumPar=Func->GetNumPar();
	int k,k1;
    for (k=1;k<=MaxIter;k++) 
     {Stp[k]=new double [NumPar+1];Stp[k][0]=NumPar;
	  for (k1=1;k1<=NumPar;k1++) Stp[k][k1]=0;
	 }
   Par=new double [NumPar+1];Par[0]=NumPar;
   if (Func->HelpPresent()) {for (k1=1;k1<=NumPar;k1++) Par[k1]=Func->valpar[k1];}
   else {for (k1=1;k1<=NumPar;k1++) Par[k1]=0;}
   CurIter=1;
}
void FASingleIO::DrawHelp(ostream &out,const char *category)
{
	ClassDesc *desc = ClassDesc::list_category_start(category);
	while (desc){
		FAZer *func=dynamic_cast<FAZer*>(desc->create());
		if (!func)
			throw info_except("Class <%s> of category <%s> have no <FAZer*> base...\n", desc->name(), category);
		SetNewFunc(func,4);
		Output(out);
		out<<" Help for Parameters\n";
		func->DrawHelp(out);
		out<<"\n\n";
		desc = ClassDesc::list_category_next(desc, category);
	}
}

void FASingleIO::DrawHelp(ostream &out)
{
#ifdef FAMESSB_TEST
  DrawHelp(out,"FAZER_Test");
#endif
  DrawHelp(out,"FAZER");
}

//=====================================================================
//=========================    FAArr          =========================
//=====================================================================
   
int FAArr::GetNumIter(int iter)
  { 
   if (!iter) return MaxIter;
   if (!(CurIter=ControlIter(iter))) 
     {cout<<"FAArr::GetNumIter=0 Error; Halt\n";exit(1);};
   for (int k=1;k<=NumFunc;k++) if (FuncArr[k]->GetNumIter(CurIter)!=CurIter) 
     {cout<<"FAArr::GetNumIter circle Error; Halt\n";
      exit(1);};
   return CurIter;
  } 

void FAArr::DeleteFAArr() {
	if (NumFunc!=0) {
        for (int k=1;k<=NumFunc;k++) {delete FuncArr[k];FuncArr[k]=NULL;}
    }
	NumFunc=0;
};

void FAArr::GetPar(double *par)
  {
   int Cur=0;
   double tmp=NumParArr[0];
   for (int k=1;k<=NumFunc;k++)
     { FuncArr[k]->GetPar(&par[Cur]);par[Cur]=tmp;
       Cur+=NumParArr[k];tmp=par[Cur];}
  };
void FAArr::SetPar(double *par)
  {
   int Cur=0;
   for (int k=1;k<=NumFunc;k++)
     { FuncArr[k]->SetPar(&par[Cur]);Cur+=NumParArr[k];}
//   par[0]=Cur;
  };
void FAArr::GetParStp(double *par,double *stp)
  {
   int Cur=0;
   double tmppar=NumParArr[0],tmpstp=NumParArr[0];
   for (int k=1;k<=NumFunc;k++)
     { FuncArr[k]->GetParStp(&par[Cur],&stp[Cur]);
       par[Cur]=tmppar;stp[Cur]=tmpstp;
       Cur+=NumParArr[k];tmppar=par[Cur];tmpstp=stp[Cur];}
  };
void FAArr::SetParStp(double *par,double *stp)
  {
   int Cur=0;
   for (int k=1;k<=NumFunc;k++)
     { FuncArr[k]->SetParStp(&par[Cur],&stp[Cur]);Cur+=NumParArr[k];}
  };
//fstream& operator<<(fstream &output,FAArr &Func)
void FAArr::Output(ostream &output)
  {
//   output<<name<<"    "<<NumFunc<<"\n";
//   output<<"    "<<NumFunc<<"\n";
   for (int k=1;k<=NumFunc;k++) FuncArr[k]->Output(output);
//   return output;
  };
//fstream& operator>>(fstream &input,FAArr &Func)
void FAArr::Input(istream &input)
  {
   int Cur=0;
   for (int k=1;k<=NumFunc;k++)
     {
      FuncArr[k]=NULL;if (!FARegister(input,FuncArr[k]))
        {cout<<"Could not register. Halt\n";exit(0);}
	 }
   	SetNumPar();CheckIterNumber();
    CurIter=1;

  };
   
void FAArr::SaveInPar(ostream &output)
  {
   for (int k=1;k<=NumFunc;k++) 
     { FuncArr[k]->SaveInPar(output);output<<"      ";};
  };

void FAArr::GetInPar(istream &input)
  {
   for (int k=1;k<=NumFunc;k++) FuncArr[k]->GetInPar(input);
  };

FAArr::FAArr():FAIO()
{
	NumFunc=0;
	for (int k=0;k<=MaxFunctions;k++) FuncArr[k]=NULL;
};

void FAArr::SetNumPar()
{
	int NumPar=0;

	for (int k=1;k<=NumFunc;k++) {NumParArr[k]=FuncArr[k]->GetNumPar();NumPar+=NumParArr[k];}
    NumParArr[0]=NumPar;
}
void FAArr::CheckIterNumber()
{
	if (NumFunc<1) return;
    MaxIter=FuncArr[1]->GetNumIter();
    for (int k=2;k<=NumFunc;k++) if (MaxIter!=FuncArr[k]->GetNumIter()) 
      {cout<<"FAArr::Input; FuncArr[i].MaxIter differs<=0; Halt.\n";exit(1);}
}
void FAArr::SetNewFunc(FAIO *func,int Num)
{
	if (Num==0) {NumFunc++;Num=NumFunc;}
	else delete FuncArr[Num];
    FuncArr[Num]=func;
    NumParArr[Num]=FuncArr[Num]->GetNumPar();
	SetNumPar();CheckIterNumber();
    CurIter=1;

}

void FAArr::DrawHelp(ostream &out)
{
	out<<"<<"<<name<<">>"<<" is IO envelope for FAZer = Clc functions\n";
	out<<" Usage example \n";
	FASingleIO *func1=new FASingleIO,*func2=new FASingleIO,*func3=new FASingleIO;
	ClassDesc *desc = ClassDesc::list_category_start("FAZer");
	FAZer *func=dynamic_cast<FAZer*>(desc->create());
	func1->SetNewFunc(func,2);
	func=dynamic_cast<FAZer*>( (desc = ClassDesc::list_category_next(desc,"FAZer"))->create() );
	func2->SetNewFunc(func,2);
	func=dynamic_cast<FAZer*>( (desc = ClassDesc::list_category_next(desc,"FAZer"))->create() );
	func3->SetNewFunc(func,2);
	SetNewFunc(func1);SetNewFunc(func2);SetNewFunc(func3);
	Output(out);
}


