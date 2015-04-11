// Программа для создания бинарного файла, для сортировки

// Создание бинарной базы данных
// Считывается структура - формата store!! в ASCII виде и 
// записывается в бинарном виде
// Сейчас работает с данными типа tab.dat
//fst   1.35    1       1
//sec   3.35    2       1
//aut  31.35    4       1
//beg  11.35    67      4
//endfst   5.35    4    4
//stor 41.35    51      3
//resu 21.35    13      2
//fst  71.35    12      5
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Чтобы поменять формат конвертируемых в bin данных
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Надо 
//			1 Изменить store
//			2 Изменить строку	   
//in>>cur->str>>cur->Num>>cur->count>>cur->count1;
//            в соответствии с новым входным форматом
//			3 Изменить строку	   
//dat.write((char*)&(datar[k]),96); 
//			  вместо 96 - поставить новый размер структуры

struct store
{
	char str[80];
	double Num;
	int count;
	int count1;
};

#include "ord_typ.h" // Используется функция seek_eof() только

void main( int argc, char *argv[] )
  {
   if (argc<=2) 
   {
		cout<<" use :  \n"<<argv[0]<<" get_file  bin_out_file \n";
		exit(0);
   }
   fstream in(argv[1],ios::in);  // Входной поток - ACII файл с данными
   fstream dat(argv[2],ios::out);// Выходной поток - бинарный файл с данными
   store datar[200];
   int Num=0;
   while (!seek_eof(in))
   {
	   Num++;store *cur=&datar[Num];
	   in>>cur->str>>cur->Num>>cur->count>>cur->count1;
   }
   for (int k=1;k<=Num;k++)
//		dat.write((char*)&(datar[k]),sizeof(store)); // sizeof - work not very  good
		dat.write((char*)&(datar[k]),96);  // sizeof - дает не точный размер
                                           //приходится ручками вычислять размер структуры 

   dat.close();
};


	