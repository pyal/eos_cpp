// ��������� ��� �������� ��������� �����, ��� ����������

// �������� �������� ���� ������
// ����������� ��������� - ������� store!! � ASCII ���� � 
// ������������ � �������� ����
// ������ �������� � ������� ���� tab.dat
//fst   1.35    1       1
//sec   3.35    2       1
//aut  31.35    4       1
//beg  11.35    67      4
//endfst   5.35    4    4
//stor 41.35    51      3
//resu 21.35    13      2
//fst  71.35    12      5
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ����� �������� ������ �������������� � bin ������
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ���� 
//			1 �������� store
//			2 �������� ������	   
//in>>cur->str>>cur->Num>>cur->count>>cur->count1;
//            � ������������ � ����� ������� ��������
//			3 �������� ������	   
//dat.write((char*)&(datar[k]),96); 
//			  ������ 96 - ��������� ����� ������ ���������

struct store
{
	char str[80];
	double Num;
	int count;
	int count1;
};

#include "ord_typ.h" // ������������ ������� seek_eof() ������

void main( int argc, char *argv[] )
  {
   if (argc<=2) 
   {
		cout<<" use :  \n"<<argv[0]<<" get_file  bin_out_file \n";
		exit(0);
   }
   fstream in(argv[1],ios::in);  // ������� ����� - ACII ���� � �������
   fstream dat(argv[2],ios::out);// �������� ����� - �������� ���� � �������
   store datar[200];
   int Num=0;
   while (!seek_eof(in))
   {
	   Num++;store *cur=&datar[Num];
	   in>>cur->str>>cur->Num>>cur->count>>cur->count1;
   }
   for (int k=1;k<=Num;k++)
//		dat.write((char*)&(datar[k]),sizeof(store)); // sizeof - work not very  good
		dat.write((char*)&(datar[k]),96);  // sizeof - ���� �� ������ ������
                                           //���������� ������� ��������� ������ ��������� 

   dat.close();
};


	