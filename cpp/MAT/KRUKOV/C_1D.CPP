/*
   РАСЧЕТНЫЙ БЛОК  STEREO-PC.1d
   Copyright (c) 24.03.1991 SPI     All rights reserved.
*/
#include <dos.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <keys.h>
#include <twindow.h>
#include <dir.h>
#include <fstream.h>
#include <io.h>
#include "util.h"
#include <FCNTL.H>
#include <SYS\STAT.H>

#define TRUE 1   // Define some handy constants
#define FALSE 0   // Define some handy constants
#define PI 3.14159   // Define a value for PI 
#define ON 1   // Define some handy constants 
#define OFF 0   // Define some handy constants
#define CONT      77                    // Признак контактной границы
#define VAK      177                    // Признак свободной  границы
#define DLBL 4096 // Длина обменного блока в байтах
#define DLUM 1024 // Длина соответствующего массива
#define SZ   4    // Размер слова чтения/записи в поток
#define CHKF 10   // Возможное число коэфф. урс

// ------------------------------------------------------- 
// Параметры, определяемые в блоке нач. данных
// ------------------------------------------------------- 

#define MNCR        750         // Максимальное количество ячеек по оси R
#define MPART       2*MNCR      // Максимальная длина массива частиц

#define CHSL 10 // Возможное число тел или слоев
#define DLSL 11 // Длина char-массивов для хранения чисел
    char s1[MAXPATH];
    char s2[80];
    char *namezad="?        ";     // Имя задачи
    char *spath  ="Строка для формирования пути поиска нужного файла";
    size_t size;
     int frstent=0;
    FILE *stream;
   union FL {
         float f[DLUM];
          long l[DLUM];
          } fl;
    long ll, osl, nombl;
   int Kolk[CHSL];                   // Кол-во коэф.
 float Curs[CHSL] [CHKF];            // Значение коэфф.

      // Параметры счета
  char prr  [DLSL];            // Разрешение
  char chk  [DLSL];            // Число К1
  char delt [DLSL];            // K2 
  char sho  [DLSL];            // Шаг окончания
  char shz  [DLSL];            // Шаг записи
  char vazmax  [DLSL];            // Вязкость max
  char vazmin  [DLSL];            // Вязкость min
  char obzro   [DLSL];            // Плотность безразм
  char obzv    [DLSL];            // Скорость безразм
  char mkt     [DLSL];            // Маркер тела для сохранения
      // Конец списка параметров счета

         // Массивы параметров частиц
        int m [MPART+1];      // Маркеры
        int nb[MPART+1];      // Номер вещества
       float r [MPART+1];      // R-координата
       float v [MPART+1];      // R-скорость
       float ro[MPART+1];      // Плотность
       float e [MPART+1];      // Энергия
       float l [MPART+1];      // Размер частицы
       float p [MPART+1];      // Давление

         // Сеточные массивы
	  int c1[MNCR+2];
	float c2[MNCR+2];
	float c3[MNCR+2];
	float c4[MNCR+2];
	float c5[MNCR+2];

       // Параметры слоев
   int nus [CHSL];            // Номера уравнений состояния
   int mrk [CHSL];            // Маркеры
 float sr1 [CHSL];            // Левые радиусы
 float sr2 [CHSL];            // Правые радиусы
 float sv  [CHSL];            // Скорости

	int i, i1, i2, j, j1, k, ii, i1or, i2or, index;
	int rad, nstp=0, stzap, izap=0, nzap=0, markt=0;
	int np, os, kolp, drob, ksl;
	int zgr1, zgr2, ncr, part;
	float robez, ebez, lbez, tbez, vbez, pbez, t, tstp;
	float razresh, kur, dt;
	float sumsk = 1.0, msk = 1.0, delta;
	float w1, w2, w3, w4, w5, w6, w7, w8, w9, w10;
	float ros, es, gamma, cs, ro0, a, lambda, dlina;
	int ststp, kchsl;
	float kvisc, minvisc, maxvisc;
	float pkr, vr, fr, pvr, rr, krdr, eps;
	  int x, y, xgr, height;
	  int grl, grt, grr, grb;
	  float rot, rob, pt, pb, vt, vb, et, eb;
	  int lxwind, lywind;
	  float stepx, realx, realy, niz, verh;
	  float massa, impulse, energy, q;
	    int un;
	  float uro, uc0, ul, ug, ue0;
      // Внутренние процедуры
//	void kadr_0();
	void kadr_im();
   int schitn(void);
	 int schito();
//  void im_ext(char *s, char *namezad, char ext[]);
	void contact(int s, int *n);
 float eos(int nt, int *nb, float ro, float e, float *cs);
	 int pause(void);
   int OP_F ( char *namezad, char ext[], int mode);
//   int WR_FL ( long nbl, long nsl, long dl, int mode);
 //  -------   К о н е ц  о п и с а н и й  -------


void main(int argc, char *argv[])
{
      // -------------------------------------------------------
      //               Ввод начальных данных
      // -------------------------------------------------------

//      if (strcmp(argv[2], "dialog")==0)  // Вход из программы "диалог"
	 strcpy(namezad, argv[1]);
  kadr_im();         // Прямой вход
  i=argc;
   i=schitn();     // Считывание начальной постановки
          if (i!=0) {cout<<"  Нет начальной постановки !  ";exit(1);}
   i=schito();     // Задание или просмотр параметров счета
          if (i!=0) {cout<<"  Не могу обратиться к файлу <.о> !  ";exit (1);}

	  printf("\nНачало счета задачи  %s\n\n",namezad);

	  razresh = atof(prr);
	  kur     = atof(chk);
	  delta   = atof(delt);
	  ststp   = atoi(sho);
	  stzap   = atoi(shz);
	  maxvisc = atof(vazmax);
	  minvisc = atof(vazmin);
	  markt   = atoi(mkt);
    ncr     = (int)(dlina*razresh);
    if (ncr>MNCR) {
       cout<<"  Количество ячеек расчетного поля больше допустимого !!  ";
       exit(1);}
  	part    = 2*ncr;

        // Константы обезразмеривания
 lbez = 1/razresh;    // Длина ячейки (мм)
 robez= atof(obzro);  // Плотность (г/см**3)
 vbez = atof(obzv);   // Скорость (км/с)
 ebez = vbez*vbez;    // Энергия (км/с)**2
 tbez = lbez/vbez;    // Время (мкс)
 pbez = robez*vbez*vbez; // Давление (Гпа)

 eps  =    1e-10;     // Обрезание производных
 pkr  =   -1.0/pbez;  // Ограничение снизу по давлению
 krdr =    0.01;      // Критерий дробления частицы

      // -------------------------------------------------------
      //               Задание параметров частиц
      // -------------------------------------------------------
	for (i=0; i<=part; i++) {
	    m[i] = 0;
	   nb[i] = 0;
	    r[i] = 0.0;
	    v[i] = 0.0;
	   ro[i] = 0.0;
	    e[i] = 0.0;
	    l[i] = 0.0;
     p[i] = 0.0; }  // Конец очистки параметров частиц

for (np=0,k=0; k<ksl; k++)  {          // Цикл по слоям
    if (nus[k]==1)   index=1;            // Ид.газ

    if (nus[k]==2) { index=3;            // Тэт
       Curs[k][0]/=robez;
       Curs[k][1]/=pbez;}

    if (nus[k]==3) { index=3;            // D-u 
       Curs[k][0]/=robez;
       Curs[k][1]/=vbez;}

       uro=Curs[k][index]/robez;       // Начальная плотность
       ue0=Curs[k][index+1]/ebez;      // Начальная энергия

    kchsl=(sr2[k]-sr1[k])*razresh;     // Количество частиц в теле
cout<<" kchsl "<<kchsl<<" sr1[k] "<<sr1[k]<<" sr2[k] "<<sr2[k]<<" razresh "<<razresh;
 for (j=0,i=np+1; i<=np+kchsl; j++,i++) {    // Задание параметров частиц
     m[i] = mrk[k]-1;      // Маркеры: 0-первое тело, 1-второе... !!!
	   nb[i] = nus[k];
	    r[i] = (sr1[k]*razresh+j+0.5);
	    v[i] = sv[k]/vbez;
	   ro[i] = uro;
	    e[i] = ue0+v[i]*v[i]/2.0;
	    l[i] = 0.5;
     p[i] = 0.0; }  // Конец цикла задания параметров частиц

  np+=kchsl;                          // Общее число частиц
     } // Конец цикла по слоям

      // ======================================================= 
      //               Начало расчета шага по времени
      // ======================================================= 
begcalc:
 dt = 0.033*nstp;   // Увеличение dt до нормального за 30 шагов
cout<<" SumSk "<<sumsk;
	w1 = kur/sumsk;
	if (dt>w1) dt=w1;
	if (dt*msk>delta)  dt = delta/msk;
	if (kur>10.0)  dt = kur/1000.0/tbez;
	sumsk = -975;
	msk   =    0.0;
	kvisc = maxvisc;
cout<<" D_T "<<dt<<" W1 "<<w1<<" msk "<<msk<<" kur "<<kur<<" np "<<np<<"\n";
      // ------------------------------------------------------- 
      //             Расчет дивергенции для вязкости
      // ------------------------------------------------------- 
      //  Очистка сеточных массивов
	 for (i=0; i<=ncr; i++) {
	 c1[i] = 0;
	 c2[i] = 0.0;
	 c3[i] = 0.0;
  c4[i] = 0.0; } // Конец цикла очистки сеточных массивов
       //  Цикл по частицам
	 for ( i=1; i<=np; i++) {
	 j = (int)(r[i]);
// c1 - маркеры
//	 с2 - сумма v*ro
//	 c3 - сумма ro
//	 c4 - итоговая скорость
   w1 = ro[i];              // Плотность
   w4 = v[i]*w1;            // Плотность импульса
       // В плоском случае раздаем массы
	  if (rad == 0) {
			w1 *= 2*l[i];
			w4 *= 2*l[i]; }
	  c1[j] = c1[j] | m[i];
   c1[j+1] = c1[j+1] | m[i]; // Mаркировка ???
	  c2[j] += w4;  c3[j] += w1;
	  c2[j+1] += w4;  c3[j+1] += w1;
   }                        //  Конец цикла по частицам
      // ------------------------------------------------------- 
      //             Находим значения скорости
      // ------------------------------------------------------- 
	  for (j=0; j<=ncr; j++)
	      {if (c3[j] != 0.0) c4[j] = c2[j]/c3[j];}
       // Закрытые границы - v=0
	  if (zgr1 == 1) c4[0] = 0.0;
	  if (zgr2 == 1) c4[ncr] = 0.0;
       // Контактные границы ??

       // Свободные границы  ??

      // ------------------------------------------------------- 
      //                 Расчет силового этапа
      // ------------------------------------------------------- 
      //  Очистка сеточных массивов
cout<<" Force ";
	 for (i=0; i<=ncr; i++) {
	 c2[i] = 0.0;
	 c3[i] = 0.0;
  c5[i] = 0.0; } // Конец цикла очистки сеточных массивов
        //     Цикл по частицам
	 for ( i=1; i<=np; i++) {
	 j = (int)(r[i]);
  es = e[i] - v[i]*v[i]/2.0; //     Внутренняя энергия
	 if (es<0.0) es = 0.0;
  ros = ro[i];               //     Плотность

	 w3 = eos(m[i], &nb[i], ros, es, &cs);
//cout<<" eos "<<w3<<" ros "<<ros<<" es "<<es;

  vr = c4[j+1] - c4[j];      // Производная скорости
  w1 = fabs(vr);             // Обрезание производной
	 if (w1<eps) vr=0.0;
  w7 = -kvisc*vr*ro[i]*cs;   // Искусственная вязкость
	 w9 = fabs(v[i]);
  w5 = cs + (w9);            // Макс. скорость сигнала
	 if (sumsk<w5) sumsk = w5;
	 if (msk<w9) msk = w9;
  w3 += (w7);                // Давление + вязкость
  w10= 1/ro[i];              // Удельный объем частицы
  if (w3<pkr) w3 = pkr;      // Ограничение отриц. давл.
  p[i] = w3;                 // Давление - в массив
// c2 - сумма давлений
//	 с3 - сумма удельных объемов
//	 c5 - итоговое давление */
  c2[j] += (w3*w10); c3[j] += (w10);
  c2[j+1] += (w3*w10); c3[j+1] += (w10);
  }                         //  Конец цикла по частицам
      // -------------------------------------------------------
      //             Находим значения давления
      // ------------------------------------------------------- 
	  for (j=0; j<=ncr; j++)
	      {if (c3[j] != 0) c5[j] = c2[j]/c3[j];}
       // Давление на св. гр. = 0
	  for (j=1; j<=ncr-1; j++)
	       if (c3[j] == 0) {c5[j-1] = 0; c5[j+1] = 0;}
      // ------------------------------------------------------- 
      //             Расчет промежуточных скоростей
      // ------------------------------------------------------- 
      //  Очистка сеточных массивов
	 for (i=0; i<=ncr; i++) {
	 c2[i] = 0.0;
	 c3[i] = 0.0;
  c4[i] = 0.0; } // Конец цикла очистки сеточных массивов
       //  Цикл по частицам
	 for ( i=1; i<=np; i++) {
	 j = (int)(r[i]);
//       с2 - сумма v*ro
//	 c3 - сумма ro
//	 c4 - итоговая скорость */
   fr = c5[j+1] - c5[j];    // Производная давления
  w1 = fabs(fr);            // Обрезание производной
	 if (w1<eps) fr=0.0;
   w9 = v[i] - fr*dt/ro[i]; // Новая скорость
   w1 = ro[i];              // Плотность
   w8 = (w9+v[i])/2.0;      // Полусумма ст. и нов. скор.
   w4 = w8*w1;              // Плотность импульса
 v[i] = w9;                 // Сохранили скорость
       // В плоском случае раздаем массы
	  if (rad == 0) {
			w1 *= (2*l[i]);
			w4 *= (2*l[i]); }

	  c2[j] += w4;  c3[j] += w1;
	  c2[j+1] += w4;  c3[j+1] += w1;
   }                        //  Конец цикла по частицам
      // ------------------------------------------------------- 
      //             Находим значения сеточных скоростей
      // ------------------------------------------------------- 
	  for (j=0; j<=ncr; j++)
	      {if (c3[j] != 0) c4[j] = c2[j]/c3[j];}
       // Закрытые границы - v=0
	  if (zgr1 == 1) c4[0] = 0.0;
	  if (zgr2 == 1) c4[ncr] = 0.0;
      // ------------------------------------------------------- 
      //             Расчет новых параметов частиц
      // ------------------------------------------------------- 
        //     Цикл по частицам
     for ( i=1; i<=np; i++) {
	 j = (int)(r[i]);
  vr = c4[j+1] - c4[j];      // Производная скорости
  w1 = fabs(vr);             // Обрезание производной
	 if (w1<eps) vr=0.0;
	 w9 = c5[j+1] * c4[j+1];
	w10 = c5[j] * c4[j];
 pvr = w9 - w10;             // Производная pv для энергии
  w1 = fabs(pvr);            // Обрезание производной
	 if (w1<eps) pvr=0.0;
        // Дивергенция pv
	 if (rad==1) pvr += (w9+w10)/r[i]/2.0;
  w6 = (c4[j+1]+c4[j])/2.0;  // Скорость перемещения част.
  e[i] -= (pvr*dt/ro[i]);    // Новая полная энергия
        // Старый объем
	 w9 = l[i];
	 if (rad==1) w9 *= r[i];
  l[i] *= (1+vr*dt);         // Новый размер частицы
    rr = r[i]+(w6*dt);       // Новая координата
        // Отношение стар. об. к нов.
	 w9 /= l[i]; if (rad==1) w9 /= rr;
  ro[i] *= w9;               // Новая плотность
         r[i] = rr;                 // Занесение координаты
      // ------------------------------------------------------- 
      //            Взаимодействие с границами поля
      // ------------------------------------------------------- 
      if (j==0) { // Граница R = 0
  w2 = rr-l[i];             // Нижняя  граница частицы
	 i1or=(w2<-krdr) ? 1 : 0;
  if (i1or) {          // Перехлест  !
     if (zgr1==1) {      // Действия при закрытой границе
        v[i] = 0.0;            // Частица останавливается
        } // Конец закрытой границы R = 0
     } // Конец перехлеста
  }    // Конец границы R = 0
      if ((j+1)==ncr) { // Граница R = ncr
  w2 = rr+l[i];             // Верхняя граница частицы
  if ((w2-ncr)>krdr) {      // Перехлест !
     if (zgr1==1) {      // Действия при закрытой границе
        v[i] = 0.0;            // Частица останавливается
        } // Конец закрытой границы  R = ncr
     } // Конец перехлеста
  }    // Конец границы R = ncr
     }                         //  Конец цикла по частицам
      // ------------------------------------------------------- 
      //                 Этап дробления частиц
      // ------------------------------------------------------- 
     ii = part;                     // Дно массивов частиц
     kolp = 0;                      // Счетчик числа новых частиц

        //     Цикл по частицам
     for ( i=np; i>=1; i--) {    // Начиная с последней к началу
	 j = (int)(r[i]);
  w1 = r[i] - l[i];          // R нижняя
  w2 = r[i] + l[i];          // R верхняя
        // Есть ли дробление вверх
  w4 = w2 -(j+1);            // Часть в верхней ячейке
  if (w4>krdr) { // Дробление вверх - перепись и правка
	    m[ii]=m[i]; nb[ii]=nb[i]; v[ii]=v[i];
	    ro[ii]=ro[i]; e[ii]=e[i];
	    l[ii]=w4/2.0; r[ii]=w2-l[ii];
     w2 = j+1;                // Верхний радиус коренной
     ii--; kolp++;} // Конец дробления вверх
        // Есть ли дробление вниз
	 w4 = j - w1;
	 drob = w4>krdr ? 1 : 0;
        // Перепись коренной частицы
	 m[ii]=m[i]; nb[ii]=nb[i]; v[ii]=v[i];
	 ro[ii]=ro[i]; e[ii]=e[i];
	 w5 = drob==1 ? j : w1;
	 l[ii]=(w2-w5)/2.0; r[ii]=w2-l[ii];
  ii--; kolp++;              // Конец правки коренной
  if (drob==1) {// Дробление вниз - перепись и правка
	    m[ii]=m[i]; nb[ii]=nb[i]; v[ii]=v[i];
	    ro[ii]=ro[i]; e[ii]=e[i];
	    l[ii]=(j-w1)/2.0; r[ii]=j-l[ii];
     ii--; kolp++;}           // Конец дробления вниз
     }                         //  Конец цикла по частицам
cout<<" Order ";
      // ------------------------------------------------------- 
      // Здесь может быть цикл по упорядочиванию частиц ???
      // ------------------------------------------------------- 
      // ------------------------------------------------------- 
      //                Этап объединения частиц
      // ------------------------------------------------------- 
      k = 1;                        // Начало объединенных частиц
      i = ii+1;                     // Начало раздробленнных ч.
        //     Цикл по частицам
   loopp:
	 if (i>part) goto endloopp;
        // Переписываем очередную
	 m[k]=m[i]; nb[k]=nb[i]; v[k]=v[i];
	 ro[k]=ro[i]; e[k]=e[i]; l[k]=l[i]; r[k]=r[i];
  j = (int)(r[i]);           // Левый узел
  i1 = m[i];                 // Маркер
    sled:i++;
	 j1 = (int)(r[i]);
	 i2 = m[i];
  if ((j==j1) && (i1==i2)) { // Объединение
        // Массы частиц
	    w3 = 2*ro[k]*l[k]; w4 = 2*ro[i]*l[i];
	    if (rad==1) {w3*=(2*PI*r[k]); w4*=(2*PI*r[i]);}
     w5 = w3 + w4;           // Суммарная масса
        // Параметры объединения
	    r[k] = (r[k]+r[i]-l[k]+l[i])/2.0;
	   ro[k] = w5/(w3/ro[k]+w4/ro[i]);
	    v[k] = (w3*v[k]+w4*v[i])/w5;
	    e[k] = (w3*e[k]+w4*e[i])/w5;
	    l[k] += l[i];
     goto sled; } // Конец объединения
	 k++;
	 goto loopp;
endloopp: np = k-1;
      // Очистка оставшейся части массивов
	for (i=k; i<=part; i++) {
	    m[i] = 0;
	   nb[i] = 0;
	    r[i] = 0.0;
	    v[i] = 0.0;
	   ro[i] = 0.0;
	    e[i] = 0.0;
	    l[i] = 0.0;
     p[i] = 0.0; }  // Конец очистки параметров частиц

      t += dt; nstp++; izap++;
cout<<" Compare to write? nstp "<<nstp<<" izap "<<izap;
   if (nstp==1) { // Открываем файл для записи таблиц
             {char tmp[256];SetExt(namezad,tmp,"t");}//ii = open(tmp, O_CREAT,S_IWRITE);
              ii=OP_F(namezad, ".t", 1);        
             if (ii<0) {
                 cout<<"Нельзя открыть файл <.t> на запись ! "<<ii<<"\n";exit(1);}}

      if ((nstp==1)||(izap==stzap)) {  // Запись результатов на диск
      if (izap==stzap) {izap=0; nzap++;}
      // -------------------------------------------------------
      //                 Массивы для высвечивания
      // ------------------------------------------------------- 
      //  Очистка сеточных массивов
	 for (i=0; i<=ncr; i++) {
	 c1[i] = 0;
	 c2[i] = 0.0;
	 c3[i] = 0.0;
	 c4[i] = 0.0;
	 c5[i] = 0.0;
       }  // Конец цикла очистки сеточных массивов
	 massa=0.0000001;
  impulse=1.0000001;
  energy=1.0000001;
       //  Цикл по частицам
	 for ( i=1; i<=np; i++) {
	 j = (int)(r[i]);
//c1 - вес
//с2 - плотность
//c3 - давление
//с4 - скорость
//c5 - внутренняя энергия
	  c1[j]++; c1[j+1]++;
	  c2[j] += ro[i]; c2[j+1] += ro[i];
	  c3[j] += p[i];  c3[j+1] += p[i];
	  c4[j] += v[i];  c4[j+1] += v[i];
	  c5[j] += e[i]-v[i]*v[i]/2.0;
    c5[j+1] += e[i]-v[i]*v[i]/2.0;
        // Сохранение
	   q = (rad==0) ? 2 : 4*PI*r[i];
	   i1or=(markt==0) ? 1 : 0;
	   i2or=(markt==m[i]) ? 1 : 0;
	   if (i1or || i2or) {
		massa += (q*l[i]*ro[i]);
	      impulse += (massa*v[i]);
	       energy += (massa*e[i]);

        } // Подсчета сохранения
   }                        //  Конец цикла по частицам
      // ------------------------------------------------------- 
      //             Находим значения параметров
      // -------------------------------------------------------
	  for (j=0; j<=ncr; j++) {
	      if (c1[j] != 0) {
		  c2[j] =robez*c2[j]/c1[j];
		  c3[j] = pbez*c3[j]/c1[j];
		  c4[j] = vbez*c4[j]/c1[j];
  	  c5[j] = ebez*c5[j]/c1[j];
		  }}
       // Закрытые границы - v=0
	  if (zgr1 == 1) c4[0] = 0.0;
	  if (zgr2 == 1) c4[ncr] = 0.0;
       // Свободные границы-p=0
	  for (j=1; j<ncr; j++) {
	       if (c1[j] == 0) {
	       c3[j-1] = 0.0; c3[j+1] = 0.0; }
        } // Конец цикла поиска свободных границ

	       w1=t*tbez;
	       size=sizeof(nzap); fwrite(&nzap, size, 1, stream);
				  fwrite(&nstp, size, 1, stream);
				  fwrite(&ncr, size, 1, stream);
				  fwrite(&markt, size, 1, stream);
                                  ii=ncr+1;
	       size=sizeof(t);    fwrite(&w1, size, 1, stream);
				  fwrite(&dlina, size, 1, stream);
				  fwrite(&massa, size, 1, stream);
				  fwrite(&impulse, size, 1, stream);
				  fwrite(&energy, size, 1, stream);
		fwrite(&c2, size, ii, stream);
		fwrite(&c3, size, ii, stream);
		fwrite(&c4, size, ii, stream);
		fwrite(&c5, size, ii, stream);
printf(" Запись %d  Step = %d  t(mks) = %f  dt(mks) = %f\n",nzap,nstp,t*tbez,dt*tbez);
//pause();
   } // Конец записи результатов на диск

      // ------------------------------------------------------- 
      //                Шаг по времени завершен
      // ------------------------------------------------------- 

      if ((i=kbhit())!=0) {
printf("\n Пауза...\n Задача *%s*  Step = %d  t(mks) = %f  dt(mks) = %f\n",
	namezad,nstp,t*tbez,dt*tbez);
       ii=pause(); if (ii==1) {
                      fclose(stream);exit(0);}}      // КОНЕЦ РАБОТЫ

      if (nstp==ststp) { fclose(stream); exit(0);} // KОНЕЦ РАБОТЫ
cout<<" After write ?  goto BegCalc \n";
      goto begcalc;

}   // Конец MAIN - программы ==================================

// ============================================================== 
       int schito(void)   // Считывание  или запись <namezad>.o
       {   
        int sig=5;   // Number digits after commar
        char tmp[256];SetExt(namezad,tmp,"o");
        fstream input=fstream(tmp,ios::in);
        if (input)
           {input>>tmp>>prr>>tmp>>chk>>tmp>>delt>>tmp>>sho>>tmp>>shz
                >>tmp>>vazmax>>tmp>>vazmin>>tmp>>obzro>>tmp>>obzv>>tmp>>mkt;
            input.close();}
        else { // Принудительное задание
         gcvt(razresh, sig, prr);
         gcvt(0.5    , sig, chk);
         gcvt(0.5    , sig, delt);
         itoa(100    , sho, 10);
         itoa(10     , shz, 10);
         gcvt(1.5    , sig, vazmax);
         gcvt(1.5    , sig, vazmin);
         gcvt(robez  , sig, obzro);
         gcvt( vbez  , sig, obzv);
         itoa(0      , mkt, 10);
        } 
         fstream output=fstream(tmp,ios::out);
         output<<"Пространственное_разрешение "<<prr<<" Устойчивость_K1 "
             <<chk<<" Устойчивость_K2 "<<delt<<"\nШаг_окончания "<<sho
             <<" Шаг_записи "<<shz<<"\nВязкость_min "<<vazmax
             <<" Вязкость_max "<<vazmin<<"\nОбезразмеривание_плотность "<<obzro
             <<" Обезразмеривание_скорость "<<obzv<<" Маркер_тела "<<mkt;
         cout<<"Пространственное_разрешение "<<prr<<" Устойчивость_K1 "
             <<chk<<" Устойчивость_K2 "<<delt<<"\nШаг окончания "<<sho
           <<" Шаг записи "<<shz<<"\nВязкость_min "<<vazmax
           <<" Вязкость_max "<<vazmin<<"\nОбезразмеривание_плотность "<<obzro
           <<" Обезразмеривание_скорость "<<obzv<<" Маркер_тела "<<mkt;
      return 0;
      }  // Конец процедуры считывания и записи параметров счета
// =================================================================
       // Создание строки для текущей директории <s> = <file>.<ext>
/*
   void im_ext(char *s, char *file, char ext[])
    {
    char drive[MAXDRIVE];
    char dir[MAXDIR];
    char fi[MAXFILE];
    char ex[MAXEXT];

    getcwd(s,MAXPATH);              // get the current working directory
    strcat(s,"\\");                  // append on a trailing \ character
    fnsplit(s,drive,dir,fi,ex); // split the string to separate elems
    strcpy(fi,file);
    strcpy(ex,ext);
    fnmerge(s,drive,dir,fi,ex);   // merge everything into one string
    }  // Конец создания строки
*/
// ==============================================================
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      //   CONTACT - функция нахождения двух единиц в байте
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void contact(int s, int *n)
     { int k, l, m;
	     *n = 0; m = 1;
	      for (k=0; k<8; k++) {
		     l = s & m; *n += l;
		     if (*n>1) break;
		     m *= 2;       }
     }  // Конец функции CONTACT
// ==============================================================
       // Нулевой кадр
     void kadr_0()
     {
       } // Конец нулевого кадра
// ==============================================================
//     Кадр - ввода имени задач
//    Результат - в namezad
     void kadr_im()
     {
      char tmp[256];SetExt(namezad,tmp,"n");
      int handle = open(tmp, O_CREAT);
      if (filelength(handle)<=0) {cout<<" No such file "<<namezad<<"\n";exit(1);}
     } // Конец кадра ввода имени задачи

// ============================================================== 
     // Пауза
 int pause(void)
{
  int c, rez=0;
  printf("                        <Esc>-выход, любая клавиша - продолжение\n");
  c = getch(); c = getch();
  if( ESC == c ) rez=1;
  if( 0 == c ) c = getch();
  return(rez);
}
// ============================================================== 

            // Открытие файла
            // Внимание ! Нужно глобальное описание FILE *stream;
            // Работаeт в паре с fclose(stream);
   int OP_F( char *namezad, char ext[], int mode)
    {
     SetExt(namezad,spath,ext);
// im_ext(spath,namezad,ext);    // Pass - в s

   if (mode==3) { // Обновление (чтение и запись сначала)
      if ((stream = fopen(spath, "r+b")) == NULL) return 1;
      fseek(stream, 0L, SEEK_SET);}

   if (mode==2) { // Присоединение (запись в конец)
      if ((stream = fopen(spath, "ab")) == NULL) return 1;
         fseek(stream, 0L, SEEK_END);}

   if (mode==1) { // Запись с нуля
      if ((stream = fopen(spath, "wb")) == NULL) return 1;}

   if (mode==0) { // Только чтение
      if ((stream = fopen(spath, "rb")) == NULL) return 1;}
        return 0;
     } // Конец OP_F

// ============================================================= 
/*            // Запись(чтение) в поток stream
            // Нужно описание union FL {...} fl
   int WR_FL ( long nbl, long nsl, long dl, int mode)
    {
      long curpos, length, lrw, tind, ostdl, nb;
      nb=nbl; 
      fseek(stream, 0L, SEEK_END);
      length = ftell(stream);
      curpos=nbl*DLBL+4*nsl;
      if (length<curpos) {
         error_message("Начало записи больше длины файла !");
         (void) get_char(); clear_message(); return (-1);}
      fseek(stream, curpos, SEEK_SET);   // Встали в начало

      lrw=(nbl+1)*DLUM-curpos/4l;                // Остаток до конца блока
      if (lrw>=dl) {                     // Запись вмещается в начальный блок
         if (mode==0) {                  // Чтение только начального блока
           fread(&fl.f[0], SZ, dl, stream);}
         else fwrite(&fl.f[0], SZ, dl, stream);// Запись только в начальный блок
         return (nb);} 

      if (mode==0) {                  // Дочитываем начальный блок
         fread(&fl.f[0], SZ, lrw, stream);}
      else fwrite(&fl.f[0], SZ, lrw, stream); // Дописываем начальный блок

      tind=lrw;                          // Текущий индекс m
      ostdl=dl-lrw;                      // Осталось дописать
      while (ostdl>=DLUM) {              // Куски длиной в блок
      if (mode==0) {                  // Читаем
         fread(&fl.f[tind], SZ, DLUM, stream);}
      else fwrite(&fl.f[tind], SZ, DLUM, stream); // Пишем
      nb+=1;
      tind+=DLUM;       
      ostdl-=DLUM;}

      if (ostdl!=0l) { nb+=1;
         if (mode==0) {
            fread(&fl.f[tind], SZ, ostdl, stream);}
         else fwrite(&fl.f[tind], SZ, ostdl, stream);}
      return (nb);
    } // Конец WR_FL
*/
// ============================================================== 
       int schitn(void)   // Считывание <namezad>.n
       {   int l;
/*
             ii=OP_F(namezad, ".n", 0);
             if (ii==0) {// Чтение начальных данных о задаче
        nombl=0l;
        osl=0l;
        i=WR_FL(nombl, osl, 10l, 0);
        ll=fl.l[8];
        fseek(stream, 0L, SEEK_SET);
        i=WR_FL(nombl, osl, ll, 0);
   if (fl.l[9]==2l) {       // Двумерная задача
   error_message("  Это задача в двумерной постановке !  ");
    i = get_char(); clear_message(); return 1;} // Конец двумерной задачи
*/
   char tmp[256];SetExt(namezad,tmp,"n");
   fstream input=fstream(tmp,ios::in);
   input>>tmp>>rad>>tmp>>dlina>>tmp>>razresh;
/*
    rad=fl.l[10];
  dlina=fl.f[12];
razresh=fl.f[13];
*/
   input>>tmp>>zgr1>>tmp>>zgr2>>tmp>>robez>>tmp>>vbez>>tmp>>ksl; k=ksl;
/*
   zgr1=fl.l[14];
   zgr2=fl.l[15];

   robez=fl.f[19];
   vbez =fl.f[20];

   ksl=fl.l[27];

   k=(int)fl.l[27];            // Число тел
*/
   for (j=0;j<k;j++)
     {
      input>>tmp>>mrk[j]>>tmp>>nus[j]>>tmp>>sv[j]>>tmp>>sr1[j]>>tmp>>sr2[j];
      input>>tmp>>Kolk[j];
/*
   l=29+k;                     // Сдвиг адресов

for (j=0; j<k; j++) {        // Цикл по телам

   mrk[j]=fl.l[l+11];
   nus[j]=(int)fl.l[l+14];    // Номер УРС:
                              //          1-Ид.газ, 2-Тэт,
                              //          3-Du, 4-ВВ, 5-Бушман
   sv[j]=fl.f[l+23];
   l+=26;
       sr1[j]=fl.f[l+3];
       sr2[j]=fl.f[l+6];
   l+=7;         
   Kolk[j]=fl.f[l+1];
   l++;
*/

      for (i=0; i<Kolk[j]; i++) input>>tmp>>Curs[j][i];
     }
/*
       Curs[j][i]=fl.f[l+1+i];// Значение коэфф.
   l+=Kolk[j];                 // Cдвиг для следующего тела
   } // Конец цикла по телам

        fclose(stream);
*/
        return 0;
           } // Конец открытия потокa
//          return 1;
//        }  // Конец чтения файла <>.n
//=================================================================
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
      //                EOS - уравнение состояния
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
float eos(int nt, int *nb, float ro, float e, float *cs)
     {float rez;
	switch (*nb) { 
 case 1:                                 // Идеальный газ
          ug = Curs[nt][0];               // Гамма
        *cs = sqrt(ug*(ug-1)*e);         // Скорость звука
        rez = (ug-1)*ro*e;               // Давление
	       break;
  case 2:                                 // Тэт
         uro = Curs[nt][0];               // Ro0 
         uc0 = Curs[nt][1];               // A 
          ul = Curs[nt][2];               // N 
        *cs = sqrt(uc0*ul*pow(ro,ul-1)/pow(uro,ul));  // Скорость звука
         rez = (ul-1)*ro*e+uc0*ul*(ro/uro-1);          // Давление
	       break;

 case 3:                                 // D-u 

         uro = Curs[nt][0];               // Ro0 
         uc0 = Curs[nt][1];               // c0 
          ul = Curs[nt][2];               // L 
        *cs = uc0;                       // Скорость звука
//cout<<" uro "<<uro<<" uc0 "<<uc0<<" ul  "<<ul;
	       rez = uc0*uc0*ro*(ro/uro-1)/
    pow((ul-(ul-1)*ro/uro),2.0);        // Давление
	       break;

	default:    printf ("\n Ошибка EOS !\n  nb = %d\n", *nb);
	       exit(1);
     }  //конец switch
	return(rez);
     }  // Конец функции EOS
// ============================================================== 
