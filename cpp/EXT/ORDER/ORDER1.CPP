#include "ord_typ.h"
//  �������������� ���� ��������� - ��������� ���������� ��������
//  ������ ���������� ���� ��������� �����, �������� �������� ������������, 
//  ������� ���������� ASCII ������
//  ��������� ��� �������� �������� ������ ���������.

//  �������� ��������� ���������� - ������ ���������� �������������
//  ������� �� 2-3-8 ����

//  ���������� ���� ASCII ������� �������� ������
//  ���� bin ������, � ����������� �� ��������� � ord_typ.h
//  #define ASCI_FILE_DEF �� ����� ����������
//  
//  ������������ �������� ����� ������� � ��������� 
//  *.cfg �����
//


// =====================================================================
// ============ Reading Writing data files into collection.     ========
// =====================================================================

// ���������� � ����� ��� ������ ���� ������
// ������������ ��� ASCII �����, ������
#ifdef ASCI_FILE_DEF
	FieldType *FieldFullAr[FieldOrderMaxNum];
	int FieldFullArNum;
#endif


// ����� ��� �������� ���������� � ������� ���������� � ������ ������
char Error[512];
strstream ErrorOut(Error,256,ios::in);
// ��������� ��� ��������� ����� �� ���� ��������� ����
// ������, ����� ������ ( ������������ � qsort - ����������� �
// ��������� ��� ����������
extern int Order(const void *fst,const void *sec);
int Order(int fst,int sec) {return Order((void*)&fst,(void*)&sec);};

void ReadCollection(fstream &in,MyIndexCollection &col)//,FieldInd)
{
	int SizeStruct=col.SizeEl,Read;
	char *buf;
	while (!in.eof())
		{ 
		 buf=new char[SizeStruct];
   Read=0;
#ifdef ASCI_FILE_DEF
   while ((seek_eoln(in)) && (!seek_eof(in))) ;
		 if (!seek_eof(in)) 
		 {
		   for (int i=1;i<=FieldFullArNum;i++)
			{if (!FieldFullAr[i]->ReadPar(in,&buf[FieldFullAr[i]->Pos]))
				{delete buf;ErrorOut<<" Error reading file. Did not read Type ReadCollection";throw int();}
			}
     Read=1;
		 }
// ���� � ����� ������ �������� ���� �� enter - ������
		if ( (!seek_eoln(in))  && (!seek_eof(in)))
			{delete buf;ErrorOut<<" Error reading file. No eoln ReadCollection";throw int();}
#else
		 in.read( buf, SizeStruct);
   Read=1;
		 int Nread=in.gcount();
		 if (Nread==0) {delete buf;ErrorOut<<" Error reading file. ReadCollection";throw int();}
		 if (Nread!=SizeStruct)  {delete buf;ErrorOut<<" Error reading file. ReadCollection";throw int();}
#endif
   if (Read)
		   col.AddElement(buf);//if (FstElement==NULL) FstElement=buf;
   else delete buf;
		}
};


void WriteCollection(fstream &out,MyIndexCollection &col,int Distinct=0)//,FieldInd)
{
	int SizeStruct=col.GetSizeElement();
	int NumRead=col.NumElement(),*IndexAr=col.IndexAr;
	int k=1;
	while (k<=NumRead)
	{
// ������ �� ��������������� �������� ( �� ������� )
		char *buf=(char*)col.GetElement(IndexAr[k]);
#ifdef ASCI_FILE_DEF
		for (int i=1;i<=FieldFullArNum;i++)
		{	FieldFullAr[i]->SavePar(out,&buf[FieldFullAr[i]->Pos]);out<<" ";}
		out<<"\n";
#else
		out.write(buf,SizeStruct);
#endif
		k++;
// ���� Distinct - �� ������� ������ ������ ������
// ���������� - ������������
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
	if (!seek_eof(cfg))	
		{
			cfg>>tmp>>name>>tmp>>Length;
			return 1;
		}
	return 0;
}


int SetFieldAr(FieldType** FieldAr,char *FileConfig,int *FieldSortNumArr,
			   int FieldSortNum,int &Pos)
{
	fstream cfg(FileConfig,ios::in);
	char name[256];
	int NumType=0,CurField=0,Length;Pos=0;
	while ( (GetTypeName(cfg,name,Length)) && (NumType<FieldOrderMaxNum) )
	{
		NumType++;
		for (int i=1;i<=FieldSortNum;i++)
		{
// ����������� ������ ����� ����������� ������
			if (NumType==FieldSortNumArr[i]) 
			{
// ���� ��� ���������� - ����� �� ������
				if (!RegisterField(name,Length,Pos,FieldAr[i]))
				{ErrorOut<<" SetFieldAr: Could not register type name:"<<name;throw int();}
				CurField++;
				break;
			}
		}
#ifdef ASCI_FILE_DEF
// ����������� ������ ����� ������� ������
		if (!RegisterField(name,Length,Pos,FieldFullAr[NumType]))
				{ErrorOut<<" SetFieldAr: Could not register type name:"<<name;throw int();}
#endif
// ������������ ������ ������ ���� ������ ( ����� �������� ����� )
		Pos+=Length;
	}
#ifdef ASCI_FILE_DEF
	FieldFullArNum=NumType;
#endif

	if (CurField==FieldSortNum) return 1;
	ErrorOut<<" SetFieldAr: Read Fields "<<NumType<<" ReadSearchField "<<FieldSortNum
		<<" GivenSearchFieldNumber "<<FieldSortNum;
	throw int();
//	return 0;
};

// =====================================================================
// ============ Globals neccessary for ordering( order arrays )  . =====
// =====================================================================

// ������  ����� �����     ���� ������
FieldType *FieldAr[FieldOrderMaxNum];
// ���������� ����� ���� ������
int FieldNum;
// ������������ ��� ��������� ����������
int FieldSortOrder;
// ��������� ������� ���� ������
MyIndexCollection col;

// ��������� ��� ��������� ����� �� ���� ������� ����
// ������, ����� ������ ( ������������ � qsort - ����������� �
// ��������� ��� ����������

int Order(const void *fst,const void *sec)
{
	int ret,pos,k=0;
// fst, sec  - ������ ������������ ������� ���� ������
	int *fst_n=(int*)fst,*sec_n=(int*)sec;
// ����������� �� ������
	char *ch1=(char*)col.GetElement(*fst_n),*ch2=(char*)col.GetElement(*sec_n);
// ���������������� ��������� ���� ��������������� ��������
// ���������� ���� �� ����������� ������� �������
// ��� �� �������� ��� ��� ������������ ���� � ������� - �����
// ������������ ������ FieldAr - ������ ����� ������������ ������
// � ������ �������� ������� - �������� �������� �� ������ ������
// Pos, ��������� ��� ��������� ��������� ������� ���� Order
	do
	{	k++;
		pos=FieldAr[k]->Pos;
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
// ������ exeption - ���� ��������� ������ ������ �������� catch
	try 
	{
	int LengthElement;
// ����������� ���������� � ����� ������, �����������
// ������ ����� ����������� ������ FieldAr
	if (!SetFieldAr(FieldAr,FileConfig,FieldSortNumArr,FieldSortNum,LengthElement)) throw int();
// ���������������� ��������� ��� �������� ������� ���� ������
// 100 - ���������� ����������� ���������, ����� 
// ��������� �������������, LengthElement - ����� �������������
// ���������� � ������� ��������.
	col.Init(LengthElement,100);
	fstream in(FileDat,ios::in);
// ����������� ������, ������ �������� � ���������
	ReadCollection(in,col);
	FieldSortOrder=SortOrder;
	FieldNum=FieldSortNum;
// ����������� ������ �������� 1,2,3...
	int *IndexAr=col.SetIndex(),num=col.NumElement();
// ���������� �� ��������, ������ ��������� ������
// ������� ���� ������ ���� ������ � ������� �������
// ������ ������ 
	qsort(&IndexAr[1],num,sizeof(int),Order);
	fstream out(FileOut,ios::out);
// ������ �� ��������������� ��������
	WriteCollection(out,col,Distinct);
	out.close();
// ������������ ���������� � ���� ����������� �����
	for (int k=1;k<=FieldNum;k++) delete FieldAr[k];
#ifdef ASCI_FILE_DEF
// ���� ���� ASCII -�������� � ������������ ���������� � ���� ����
// �����, �� ������ �����������
// ��� ����� ��� ����������� ������ ��������������� ����
// ������ ��� ������������ ��-������
	for (int i=1;i<=FieldFullArNum;i++) delete FieldFullAr[i];;
#endif
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
//  FieldSortNum - Number of indexes to sort by
//  FieldSortNumArr - Indexes to sort by (1,4,5 - for example)
//  SortOrder - Sorting order (1-normal,0-reverce)
//  FileConfig - file with structure discription - of the kind :
//  Type String Length 80 Type double Length 8 Type int Length 4 ....
//  FileDat  -  file with binary data,
//  FileOut  -  file to store result
//

//  �������� ���������, ������������ ����������
int SortBy(int FieldSortNum,int *FieldSortNumArr,int SortOrder,char *FileConfig,char *FileDat,char *FileOut)
{
//	MyIndexCollection col;
	if (!SortByProc(FieldSortNum,FieldSortNumArr,SortOrder,FileConfig,FileDat,col,FileOut)) return 0;
	return 1;
}
//  �������� ���������, ������������ ������ ������ ���������
//  �� ������������� ����: FieldDistinctInd - ������
int Distinct(int FieldDistinctInd,char *FileConfig,char *FileDat,char *FileOut)
{
	int FieldSortNum=1;
	int FieldSortNumArr[2];FieldSortNumArr[1]=FieldDistinctInd;
	if (!SortByProc(FieldSortNum,&FieldSortNumArr[0],1,FileConfig,FileDat,col,FileOut,1)) return 0;
	return 1;
}
//
//                Just example, to control work of the main procedures
//
void main( int argc, char *argv[] )
  {
   if (argc<=3) 
   {
		  cout<<" use :  \n"<<argv[0]<<" in_dat_file in_cfg_file bin_out_file \n"<<
          " FileConfig - file with structure discription - of the kind :\n"<<
          " Type String Length 80 Type double Length 8 Type int Length 4 ....\n"<<
          " Sizes are valid only for bin mode to determin the position of field \n"<<
#ifdef ASCI_FILE_DEF
" ASCII mode, read and output ASCII files\n"; 
#else
" BIN mode, read and output BIN files\n"; 
#endif

		  exit(0);
   }
// Array determin how to sort 2 - 3 - 8 
   int S[4]={1,2,3,8};
	SortBy(3,S,1,argv[2],argv[1],argv[3]);
};