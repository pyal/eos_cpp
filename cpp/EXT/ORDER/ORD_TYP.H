#ifndef __ORD_TYP_H
#define __ORD_TYP_H
//#include "util.h"
#include <afxcoll.h>
#include <fstream.h>
#include <strstrea.h>

#define FieldOrderMaxNum 100
#define ASCI_FILE_DEF
// ����� ����� ����� �� - ���� ������
int seek_eof(istream &i);
// ����� ����� ����� ��� ������ �� - ���� ������
int seek_eoln(istream &i);



// ��������� ��������� ������������ ��� ������
// ������ ������ ���������� - ����� ������
// ��� ASCII ������ ����� ������������ ��������� ������ � ������
// ����� Pos - ��������� ������� ���� ������������ ������ 0 ��������
// Length  - ����� ����
struct FieldType
{
//	const char * const name;
//    virtual const char *InterfaceIOName() {return " InterfaceIO ";};

	int Length;
	int Pos;
	virtual int Order(void *Fst,void *Sec){return 0;};
#ifdef ASCI_FILE_DEF
	virtual void SavePar(fstream &out,void *Ptr)=0;
	virtual int ReadPar(fstream &in,void *Ptr)=0;
#endif
	virtual ~FieldType(){};
// ��������� ��� ����������� ���� - �� ����� ���������� ����� ������
// ��������� ����� ��������� ������ ��� real int long
	friend int RegisterField(char *name,int Length,int Pos,FieldType *&Field);
};

// �������� ���� ������
struct FieldString:FieldType
{int Order(void *Fst,void *Sec) { char *ch1=(char *)Fst,*ch2=(char *)Sec;return Stricmp(ch1,ch2);};
#ifdef ASCI_FILE_DEF
void SavePar(fstream &out,void *Ptr) {char *ch1=(char *)Ptr;out<<ch1;};
int ReadPar(fstream &in,void *Ptr) {if (seek_eof(in))return 0;char *ch1=(char *)Ptr;in>>ch1;return 1;};
#endif
};
// �������� ���� real
struct FieldReal:FieldType
{int Order(void *Fst,void *Sec) { double *ch1=(double *)Fst,*ch2=(double *)Sec;
int ret=0;if (*ch1-*ch2>0) ret=1;else if (*ch1-*ch2<0) ret=-1;return ret;};
#ifdef ASCI_FILE_DEF
void SavePar(fstream &out,void *Ptr) {double *ch1=(double *)Ptr;out<<*ch1;};
int ReadPar(fstream &in,void *Ptr) {if (seek_eof(in))return 0;double *ch1=(double *)Ptr;in>>*ch1;return 1;};
#endif
};
// �������� ���� int
struct FieldInt:FieldType
{int Order(void *Fst,void *Sec)
{ int *ch1=(int*)Fst,*ch2=(int*)Sec;return *ch1-*ch2;};
#ifdef ASCI_FILE_DEF
void SavePar(fstream &out,void *Ptr) {int *ch1=(int *)Ptr;out<<*ch1;};
int ReadPar(fstream &in,void *Ptr) {if (seek_eof(in))return 0;int *ch1=(int *)Ptr;in>>*ch1;return 1;};
#endif
};
// �������� ���� long
struct FieldLong:FieldType
{int Order(void *Fst,void *Sec)
{ int *ch1=(int*)Fst,*ch2=(int*)Sec;return *ch1-*ch2;};
#ifdef ASCI_FILE_DEF
void SavePar(fstream &out,void *Ptr) {int *ch1=(int *)Ptr;out<<*ch1;};
int ReadPar(fstream &in,void *Ptr) {if (seek_eof(in))return 0;int *ch1=(int *)Ptr;in>>*ch1;return 1;};
#endif
};





// ���������. ��������� ������ ����
// SizeEl - ������ ��������
// IndexAr - ������� �������� - �������������
// NumEl - ����� ������� � ���� ������
struct MyIndexCollection
{
	MyIndexCollection(){ IndexAr=NULL;};
	CPtrArray Data;
	int *IndexAr;
	int SizeEl;
	int NumEl;
	int NumElement(){return Data.GetSize();}//NumEl;}
	void Init(int SizeElement,int AddByNum=100)
		{Data.SetSize(0,AddByNum);SizeEl=SizeElement;NumEl=0;};
	void AddElement(void *El) {Data.Add(El);NumEl++;};
	int *SetIndex();
	void *GetElement(int ElNum){return Data.GetAt(ElNum-1);}
	int GetSizeElement(){return SizeEl;}
	~MyIndexCollection(){delete IndexAr;IndexAr=NULL;};
};


#endif
