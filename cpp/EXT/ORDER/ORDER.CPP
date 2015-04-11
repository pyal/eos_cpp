#include "ord_typ.h"

// =====================================================================
// ============ Reading Writing data files into collection.     ========
// =====================================================================
//char *FstElement;
char Error[512];
strstream ErrorOut(Error,256,ios::in);
extern int Order(const void *fst,const void *sec);
int Order(int fst,int sec) {return Order((void*)&fst,(void*)&sec);};

void ReadCollection(fstream &in,MyIndexCollection &col)//,FieldInd)
{
	int SizeStruct=col.SizeEl;
	char *buf;
	while (!in.eof())
		{ 
		 buf=new char[SizeStruct];
		 in.read( buf, SizeStruct);
		 int Nread=in.gcount();
		 if (Nread==0) {delete buf;return ;}
		 if (Nread!=SizeStruct) 
			{ErrorOut<<" Error reading file. File size do not match struct size";throw int();};
		 col.AddElement(buf);//if (FstElement==NULL) FstElement=buf;
		}
};

void WriteCollection(fstream &out,MyIndexCollection &col,int Distinct=0)//,FieldInd)
{
	int SizeStruct=col.GetSizeElement();
	int NumRead=col.NumElement(),*IndexAr=col.IndexAr;
	int k=1;
	while (k<=NumRead)
	{
		char *buf=(char*)col.GetElement(IndexAr[k]);
		out.write(buf,SizeStruct);
		k++;
		if (Distinct) 
			{ int beg=k-1;while ( (k<=NumRead) && (Order(IndexAr[beg],IndexAr[k])==0) ) k++;}
		if (out.bad()) 
			{ErrorOut<<" Error writing file.";throw int();};
	}
};

// =====================================================================
// ============ Reading configuration file, setting order records. =====
// =====================================================================

int GetTypeName(fstream &cfg,char *name,int &Length)
{
	char tmp[256];
	if (!seek_eof(cfg))	{cfg>>tmp>>name>>tmp>>Length;return 1;}
	return 0;
}


int SetFieldAr(FieldType** FieldAr,char *FileConfig,int *FieldSortNumArr,
			   int FieldSortNum,int &Pos)
{
	fstream cfg(FileConfig,ios::in);
	char name[256];
	int NumType=0,CurField=1,OK_exit=0,Length;Pos=0;
	while (GetTypeName(cfg,name,Length))
	{
		NumType++;
		if (!OK_exit)
		{
			if (NumType==FieldSortNumArr[CurField]) 
			{
				if (!RegisterField(name,Length,Pos,FieldAr[CurField]))
				{ErrorOut<<" SetFieldAr: Could not register type name:"<<name;throw int();}
				if (++CurField>FieldSortNum) OK_exit=1;
			}
		}
		Pos+=Length;
	}
	if (OK_exit) return 1;
	ErrorOut<<" SetFieldAr: Read Fields "<<NumType<<" CurSearchField "<<FieldSortNumArr[CurField];
	throw int();
//	return 0;
};

// =====================================================================
// ============ Globals neccessary for ordering( order arrays )  . =====
// =====================================================================

FieldType *FieldAr[FieldOrderMaxNum];
int FieldNum;
int FieldSortOrder;
MyIndexCollection col;


int Order(const void *fst,const void *sec)
{
	int ret,pos,k=0;
	int *fst_n=(int*)fst,*sec_n=(int*)sec;
	char *ch1=(char*)col.GetElement(*fst_n),*ch2=(char*)col.GetElement(*sec_n);
	
//	for (int k=1;k<=FieldNum;k++)
	do
	{	k++;
		pos=FieldAr[k]->Pos;
		int *ctr1=(int*)&ch1[pos],*ctr2=(int*)&ch2[pos];
	}
	while ( 
		((ret=FieldAr[k]->Order(&ch1[pos],&ch2[pos]))==0) && 
												(k<FieldNum) );
	if (!FieldSortOrder) ret*=-1;
	return ret;
};


int SortByProc(int FieldSortNum,int *FieldSortNumArr,int SortOrder,char *FileConfig,char *FileDat,
		   MyIndexCollection &col,char *FileOut,int Distinct=0)
{
	try 
	{
	int LengthElement;
//	ErrorOut(Error,512);//out.init(bufstr[0],512);
	if (!SetFieldAr(FieldAr,FileConfig,FieldSortNumArr,FieldSortNum,LengthElement)) throw int();
//int SetFieldAr(FieldType** FieldAr,char *FileConfig,int *FieldSortNumArr,
//			   int FieldSortNum,int &Pos)
	col.Init(LengthElement,100);
	fstream in(FileDat,ios::in);
	ReadCollection(in,col);
	FieldSortOrder=SortOrder;FieldNum=FieldSortNum;
	int *IndexAr=col.SetIndex(),num=col.NumElement();
	qsort(&IndexAr[1],num,sizeof(int),Order);
	fstream out(FileOut,ios::out);
	WriteCollection(out,col,Distinct);
	out.close();

	for (int k=1;k<=FieldNum;k++) delete FieldAr[k];
	return 1;
	}
	catch(int)
	{
		cout<<" Error occured "<<Error<<"\n";
		return 0;
	}
}

// =====================================================================
// ============ Main working procedures                          . =====
// =====================================================================

int SortBy(int FieldSortNum,int *FieldSortNumArr,int SortOrder,char *FileConfig,char *FileDat,char *FileOut)
{
//	MyIndexCollection col;
	if (!SortByProc(FieldSortNum,FieldSortNumArr,SortOrder,FileConfig,FileDat,col,FileOut)) return 0;
	return 1;
}
int Distinct(int FieldDistinctInd,char *FileConfig,char *FileDat,char *FileOut)
{
	int FieldSortNum=1;
	int FieldSortNumArr[2];FieldSortNumArr[1]=FieldDistinctInd;
//	MyIndexCollection col;
	if (!SortByProc(FieldSortNum,&FieldSortNumArr[0],1,FileConfig,FileDat,col,FileOut,1)) return 0;
	return 1;
}
//
//                Just example, to control work
//
void main( int argc, char *argv[] )
  {
//   FstElement=NULL;
   if (argc<=3) 
   {
		cout<<" use :  \n"<<argv[0]<<" in_dat_file in_cfg_file bin_out_file \n";
		exit(0);
   }
///	Distinct(3,argv[2],argv[1],argv[3]);
	int S[3]={1,1,3};
	SortBy(2,S,1,argv[2],argv[1],argv[3]);
};