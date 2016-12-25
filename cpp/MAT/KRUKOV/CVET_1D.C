/*
   БЛОК  ГРАФИКИ STEREO-PC.1d С ГРАФИКОЙ
   Copyright (c) 26.03.1991 SPI     All rights reserved.
*/

#include <dos.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <graphics.h>
#include <twindow.h>
#include <dir.h>

#include <keys.h>
#define TRUE	1			/* Define some handy constants	*/
#define FALSE	0			/* Define some handy constants	*/
#define PI	3.14159 		/* Define a value for PI	*/
#define ON	1			/* Define some handy constants	*/
#define OFF	0			/* Define some handy constants	*/

/* ------------------------------------------------------- */
/* Параметры, определяемые в блоке нач. данных             */
/* ------------------------------------------------------- */

#define MNCR       750        /* Максимальное количество ячеек по оси R */
#define KOLGR       4         /* Количество графиков (4)   */

#define CHSL  5 /* Возможное число тел или слоев */
#define DLSL 16 /* Длина char-массивов для хранения чисел */
    char s1[MAXPATH];
    char s2[80];
    size_t size;
     int frstent=0;
    FILE *stream1;
    FILE *stream2;
	 char *namezad="?        ";     /* Имя задачи */
   char *spath  ="Строка для формирования пути поиска нужного файла";

			   /* Параметры отображения */
	 char gr  [DLSL];
	 char gp  [DLSL];
	 char gv  [DLSL];
	 char ge  [DLSL];
	 char grl  [DLSL];
	 char gpl  [DLSL];
	 char gvl  [DLSL];
	 char gel  [DLSL];
	 char grr  [DLSL];
	 char gpr  [DLSL];
	 char gvr  [DLSL];
	 char ger  [DLSL];
			   /* Конец списка параметров отображения */
char *VidGraf[] = {
  "Density",  "Presure", "Velocity", "Energy"
};

int    GraphDriver;		/* The Graphics device driver		*/
int    GraphMode;		/* The Graphics mode value		*/
double AspectRatio;		/* Aspect ratio of a pixel on the screen*/
int    MaxX, MaxY;		/* The maximum resolution of the screen */
int    MaxColors;		/* The maximum # of colors available	*/
int    ErrorCode;		/* Reports any graphics errors		*/
struct palettetype palette;		/* Used to read palette info	*/

			      /* Сеточные массивы           */
	  int c1[MNCR+2];
	float c2[MNCR+2];
	float c3[MNCR+2];
	float c4[MNCR+2];
	float c5[MNCR+2];

	int i, i1, i2, j, j1, k, ii, i1or, i2or;
	int rad, nstp, stepzap, nzap, np, os, kolp, drob;
	int zgr1, zgr2, ncr, fst;
	float robez, ebez, lbez, tbez, vbez, pbez, t, tstp;
	float kur, dt, dlina, w1;
	int mvgr[KOLGR];
	float granl[KOLGR];
	float granr[KOLGR];
	int marker=0, ststp,np;
	float kvisc, minvisc, maxvisc;
	float pkr, vr, fr, pvr, rr, krdr, eps;
  struct viewporttype vp;
	  int x, y, xgr, height;
	  int grrl, grt, ggrr, grb;
	  float rot, rob, pt, pb, vt, vb, et, eb;
	  int lxwind, lywind;
	  float stepx, realx, realy, niz, verh;
	  float massa, impulse, energy, q;
void Initialize(void);
void ReportStatus(void);
void SayGoodbye(void);
 int Paus (void);
void Pause(void);
void DrawBorder(int col);
void MainWindow(char *header,int col);
void GrWindow(char *header,int col);
void changetextstyle(int font, int direction, int charsize);
void StatusLine(char *msg);
int  gprintf(int *xloc, int *yloc, char *fmt, ... );
	     /* Внутренние процедуры */
	void kadr_0();
	void kadr_im();
	void readrez(FILE *stream);
  void im_ext(char *s, char *namezad, char ext[]);
   int schitg(void);
	/*  -------   К о н е ц  о п и с а н и й  -------   */


void main(int argc, char *argv[])
     {
      i=argc;
      if (strcmp(argv[2], "dialog")==0)  /* Вход из программы "диалог" */
	 strcpy(namezad, argv[1]);
      else
	 { kadr_0(); kadr_im();}         /* Прямой вход */

  	im_ext(spath,namezad,".t");    /* Pass - в spath */
	  if ((stream1 = fopen(spath, "rb")) == NULL)
  {error_message("  У задачи с этим именем нет одномерного решения !  ");
	  i = get_char(); clear_message(); exit(1);}
	  readrez(stream1);

 begingr: i=schitg(); /* Задание или просмотр параметров высвечивания */

	  fst=0;
	  mvgr[0] = atoi(gr); /* Плотность             */
	  mvgr[1] = atoi(gp); /* Давление              */
	  mvgr[2] = atoi(gv); /* Скорость              */
	  mvgr[3] = atoi(ge); /* Энергия               */

		       /* Соответствующие границы */
	  granl[0] = atof(grl);   granr[0] = atof(grr);
	  granl[1] = atof(gpl);   granr[1] = atof(gpr);
	  granl[2] = atof(gvl);   granr[2] = atof(gvr);
	  granl[3] = atof(gel);   granr[3] = atof(ger);

      /* ======================================================= */
      /*                Задание графического окна                */
      /* ======================================================= */
  Initialize(); 		/* Set system into Graphics mode	*/

  MainWindow( "STEREO-PC 1d", 11 );
  settextjustify( LEFT_TEXT, TOP_TEXT );
  changetextstyle( DEFAULT_FONT, HORIZ_DIR, 1 );
  setlinestyle( SOLID_LINE, 0, NORM_WIDTH );
  setfillstyle( EMPTY_FILL, 0 );
  height = textheight( "H" );           /* Detemine current height */
  grrl=80; ggrr=630; grt=80; grb=400;   /* Размеры граф. окна      */
  lxwind = ggrr-grrl;                   /* Длина окна в дискретах  */
  lywind = grb-grt;                     /* Высота окна в дискретах */
  stepx = (float)lxwind/(float)ncr;     /* Шаг по оси х            */
  x = 10; y = grt-2*height;
  for (ii=0; ii<4; ii++) {              /* Вывод VidGraf     */
  setcolor(ii+9);                       /* Set current color */
    if (mvgr[ii]!=0) {
    gprintf( &x, &y, "%s", VidGraf[ii]);
    y+=2*height;
    gprintf( &x, &y, "%g", granr[ii]);
    y+=2*height;
    gprintf( &x, &y, "%g", granl[ii]);
    y+=4*height;
    }
  }  /* Конец вывода VidGraf */

  x = 10; y = grb-3*height;
  setcolor(13);
  gprintf( &x, &y, "%s", "Absciss (mm)");
  y+=2*height;
  gprintf( &x, &y, "%g - %d (%g)", 0.0, ncr, dlina);

  GrWindow( namezad, 13 );  /* Заказ графического окна */

      /* ------------------------------------------------------- */
      /*               Границы изменения параметров              */
      /* ------------------------------------------------------- */
       if ((granl[0]==0.0) && (granr[0]==0)) {
	  rot=-1e10; rob=1e10;
	  for (j=0; j<=ncr; j++) {
	      if (c2[j] <rob) rob=c2[j];
	      if (c2[j] >rot) rot=c2[j]; }}
	else  { rot=granr[0]; rob=granl[0]; }

       if ((granl[1]==0.0) && (granr[1]==0)) {
	  pt=-1e10; pb=1e10;
	  for (j=0; j<=ncr; j++) {
	      if (c3[j] <pb) pb=c3[j];
	      if (c3[j] >pt) pt=c3[j]; }}
	else  { pt=granr[1]; pb=granl[1]; }

       if ((granl[2]==0.0) && (granr[2]==0)) {
	  vt=-1e10; vb=1e10;
	  for (j=0; j<=ncr; j++) {
	      if (c4[j] <vb) vb=c4[j];
	      if (c4[j] >vt) vt=c4[j]; }}
	else  { vt=granr[2]; vb=granl[2]; }

       if ((granl[3]==0.0) && (granr[3]==0)) {
	  et=-1e10; eb=1e10;
	  for (j=0; j<=ncr; j++) {
	      if (c5[j] <eb) eb=c5[j];
	      if (c5[j] >et) et=c5[j]; }}
	else  { et=granr[3]; eb=granl[3]; }

   fseek(stream1,0L, SEEK_SET); /* Установка указателя потока в начало */
	ii=Paus(); 
      if (ii==1) {closegraph(); exit(0);} /* КОНЕЦ РАБОТЫ */

      setviewport( grrl, grt, ggrr, grb, 1 );	/* Open port for grafic */

			 /* Цикл по записям */
  begloop:
  while( !kbhit() ){  /* Пока не нажата клавиша - просмотр */
getch(); 

  readrez(stream1);
  clearviewport();
  setcolor(11);                         /* Вывод шага по времени */
  changetextstyle( DEFAULT_FONT, HORIZ_DIR, 1 );
  x = lxwind-50; y = 5;
  gprintf( &x, &y, "step=%d", nstp);
  y += 2*height;
  gprintf( &x, &y, "   t=%g", t);

  for (ii=0; ii<4; ii++) {              /* Вывод графиков    */
  setcolor(ii+9);                       /* Set current color */
    if (mvgr[ii]!=0) {
       realx = 0.0;
       if (ii==0) {realy=c2[0]; niz=rob; verh=rot;}
       if (ii==1) {realy=c3[0]; niz=pb; verh=pt;}
       if (ii==2) {realy=c4[0]; niz=vb; verh=vt;}
       if (ii==3) {realy=c5[0]; niz=eb; verh=et;}
       w1 = verh-niz;
       if (w1<1e-10) {niz -= 1; verh += 1;} /* Избегаем деления на ноль */
       w1 = lywind/(verh-niz);
	x = 0; y = (int)(lywind-(realy-niz)*w1);
	moveto(x, y);
	for (j=1; j<=ncr; j++) { /* Цикл по точкам графика */
	realx += stepx;
       if (ii==0) realy=c2[j];
       if (ii==1) realy=c3[j];
       if (ii==2) realy=c4[j];
       if (ii==3) realy=c5[j];
	     x = (int)(realx);
       y = (int)(lywind-(realy-niz)*w1);
	     lineto(x, y);
       k++;
      } /* Конец цикла по точкам */
    }                   /* Конец VidGraf         */
  }                     /* Конец вывода графиков */
			/* Вывод сохранения */
  if (fst==0) {
     fst=1;
     setviewport( 0, 0, MaxX, MaxY, 1 );	/* Open fool screen */
     height = textheight( "H" );           /* Determine current height      */
     x = 250; y = grb+height+5;
     setcolor(3);
     gprintf( &x,&y,"Conservation (mark=%d)",marker);
     x+=150;
     gprintf( &x,&y,"%g", massa);
     x+=100;
     gprintf( &x,&y,"%g", impulse);
     x+=100;
     gprintf( &x,&y,"%g", energy);
     x = 250; y+=2*height;
     gprintf( &x,&y,"Mass  Impulse  Energy");
     setviewport( grrl, grt, ggrr, grb, 1 );	/* Open port for grafic */
     } /* Вывода для шагa 0 */

     else

     {
     x = 350; y = grb+3*height+5;
     setviewport( x, y, MaxX-2, y+15, 1 );       /* Open fool screen */
     clearviewport ();
     setcolor(4);
     x=50; y=0;
     gprintf( &x,&y,"%g", massa);
     x+=100;
     gprintf( &x,&y,"%g", impulse);
     x+=100;
     gprintf( &x,&y,"%g", energy);
     setviewport( grrl, grt, ggrr, grb, 1 );	/* Open port for grafic */
     } /* Вывода сохранения */

      setviewport( grrl, grt, ggrr, grb, 1 );	/* Open port for grafic */
   if (feof(stream1)) {
      ii=Paus(); if (ii==1) {closegraph(); exit(0);} /* КОНЕЦ РАБОТЫ */
    closegraph(); goto begingr;}

      } /* Конец прокрутки без остановки */

      x = getch();				/* Пропуск нажатой для паузы клавиши */
      ii=Paus();
      if (ii==1) { closegraph(); goto begingr;}  
      setviewport( grrl, grt, ggrr, grb, 1 );	/* Open port for grafic */
      goto begloop;
 } /* Конец MAIN-программы */
/* ==================================================================== */

/*									*/
/*	INITIALIZE: Initializes the graphics system and reports 	*/
/*	any errors which occured.					*/
/*									*/

void Initialize(void)

{
  int xasp, yasp;			/* Used to read the aspect ratio*/

  GraphDriver = DETECT; 		/* Request auto-detection	*/
  initgraph( &GraphDriver, &GraphMode, "D:\\TC\\BGI" );
  ErrorCode = graphresult();		/* Read result of initialization*/
  if( ErrorCode != grOk ){		/* Error occured during init	*/
    printf(" Graphics System Error: %s\n", grapherrormsg( ErrorCode ) );
    exit( 1 );
  }

  getpalette( &palette );		/* Read the palette from board	*/
  MaxColors = getmaxcolor() + 1;	/* Read maximum number of colors*/

  MaxX = getmaxx();
  MaxY = getmaxy();			/* Read size of screen		*/

  getaspectratio( &xasp, &yasp );	/* read the hardware aspect	*/
  AspectRatio = (double)xasp / (double)yasp; /* Get correction factor	*/

}

/*									*/
/*	GPRINTF: Used like PRINTF except the output is sent to the	*/
/*	screen in graphics mode at the specified co-ordinate.		*/
/*									*/

int gprintf( int *xloc, int *yloc, char *fmt, ... )
{
  va_list  argptr;			/* Argument list pointer	*/
  char str[140];			/* Buffer to build sting into	*/
  int cnt;				/* Result of SPRINTF for return */

  va_start( argptr, fmt );		/* Initialize va_ functions	*/

  cnt = vsprintf( str, fmt, argptr );	/* prints string to buffer	*/
  outtextxy( *xloc, *yloc, str );	/* Send string in graphics mode */
/*  *yloc += textheight( "H" ) + 2; */      /* Advance to next line         */

  va_end( argptr );			/* Close va_ functions		*/

  return( cnt );			/* Return the conversion count	*/

}

/*									*/
/*	 PAUS: Pause until the user enters a keystroke. If the		*/
/*	key is an ESC, then exit program, else simply return.		*/
/*									*/

 int Paus(void)
{
  static char msg[] = "<Esc>-выход, любая клавиша - продолжение";
  int c, rez=0;

  StatusLine( msg );			/* Put msg at bottom of screen	*/

  c = getch();				/* Read a character from kbd	*/

  if( ESC == c ){			/* Does user wish to leave?	*/
    rez=1;
  }

  if( 0 == c ){ 			/* Did use hit a non-ASCII key? */
    c = getch();			/* Read scan code for keyboard	*/
  }
  setviewport( 0, MaxY-2*height, MaxX, MaxY, 1 );
  clearviewport (); 
  return(rez);
}

/*									*/
/*	STATUSLINE: Display a status line at the bottom of the screen.	*/
/*									*/

void StatusLine( char *msg )
{
  int height;

  setviewport( 0, 0, MaxX, MaxY, 1 );	/* Open port to full screen	*/
  setcolor( MaxColors - 1 );		/* Set current color to white	*/

  changetextstyle( DEFAULT_FONT, HORIZ_DIR, 1 );
  settextjustify( CENTER_TEXT, TOP_TEXT );
  setlinestyle( SOLID_LINE, 0, NORM_WIDTH );
  setfillstyle( EMPTY_FILL, 0 );

  height = textheight( "H" );           /* Detemine current height      */
  bar( 0, MaxY-(height+4), MaxX, MaxY );
  rectangle( 0, MaxY-(height+4), MaxX, MaxY );
  outtextxy( MaxX/2, MaxY-(height+2), msg );
  setviewport( 1, height+5, MaxX-1, MaxY-(height+5), 1 );
}

/*									*/
/*	MAINWINDOW: Establish the main window for the demo and set	*/
/*	a viewport for the demo code.					*/
/*									*/

void MainWindow( char *header, int col )
{
  int height;

  cleardevice();			/* Clear graphics screen	*/
  setcolor( col+1 );	        	/* Set current color         	*/
  setviewport( 0, 0, MaxX, MaxY, 1 );	/* Open port to full screen	*/


  changetextstyle( TRIPLEX_FONT, HORIZ_DIR, 2 );
  settextjustify( CENTER_TEXT, TOP_TEXT );
  outtextxy( MaxX/2, 5, header );

  height = textheight( "H" );           /* Get basic text height        */
  setviewport( 0, height+20, MaxX, MaxY-(height+10), 1 );
  DrawBorder( col );

}

/*									*/
/*	  GRWINDOW: Establish the main window for the demo and set	*/
/*	a viewport for the demo code.					*/
/*									*/

void GrWindow( char *header, int col )
{
  int height;
  setviewport( 0, 0, MaxX, MaxY, 1 );	/* Open port to full screen	*/
  setcolor( col+1 );	        	/* Set current color         	*/
  changetextstyle( TRIPLEX_FONT, HORIZ_DIR, 3 );
  settextjustify( CENTER_TEXT, TOP_TEXT );
  height = textheight( "H" );           /* Get basic text height        */
  outtextxy( (grrl+ggrr)/2, grt-1.5*height, header );
  setcolor(col);
  rectangle( grrl-1, grt-1, ggrr+1, grb+1);
  setviewport( grrl, grt, ggrr, grb, 1 );	/* Open port for grafic 	*/

}

/*									*/
/*	DRAWBORDER: Draw a solid single line around the current 	*/
/*	viewport.							*/
/*									*/

void DrawBorder( int col )
{
  struct viewporttype vp;

  setcolor( col );		/* Set current color to white	*/

  setlinestyle( SOLID_LINE, 0, NORM_WIDTH );

  getviewsettings( &vp );
  rectangle( 0, 0, vp.right-vp.left, vp.bottom-vp.top );

}

/*									*/
/*	CHANGETEXTSTYLE: similar to settextstyle, but checks for	*/
/*	errors that might occur whil loading the font file.		*/
/*									*/

void changetextstyle(int font, int direction, int charsize)
{
  int ErrorCode;

  graphresult();			/* clear error code		*/
  settextstyle(font, direction, charsize);
  ErrorCode = graphresult();		/* check result 		*/
  if( ErrorCode != grOk ){		/* if error occured		*/
    closegraph();
    printf(" Graphics System Error: %s\n", grapherrormsg( ErrorCode ) );
    exit( 1 );
  }
}
      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
      /*                    Нулевой кадр                         */
      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

     void kadr_0()
     {
	WINDOW *wnd0, *wndA;
	wnd0 = establish_window(0,0,25,80);
	set_border(wnd0, 1);
	set_colors(wnd0, BORDER, BLACK, AQUA, DIM);

	wndA = establish_window(17, 6, 11, 44);
	set_border(wndA, 1);
	set_colors(wndA, ALL, AQUA, WHITE, BRIGHT);

	wprintf(wndA, "\n\n   П Р О Г Р А М М Н А Я  С И С Т Е М А");
	wprintf(wndA,     "\n\n            S T E R E O - P C          ");
	wprintf(wndA,     "\n\n               (версия 1.0)            ");

	display_window(wnd0);
	display_window(wndA);
       } /* Конец нулевого кадра */
      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
      /*	       Кадр - ввода имени задачи                 */
      /*	       Результат - в namezad                     */
      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
     void kadr_im()
     {
	WINDOW *wnd;
        FIELD *fld;
	load_help("kdr3.hlp");

	wnd = establish_window(25, 18, 3, 30);
	set_title(wnd, " Введите имя задачи ");
	set_border(wnd,1);
	set_colors(wnd, ALL, AQUA, WHITE, BRIGHT);
	set_colors(wnd, ACCENT, WHITE, BLACK, DIM);
	display_window(wnd);
	wprompt(wnd, 6, 0, "Имя: ");
	init_template(wnd);
	fld = establish_field(wnd, 11, 0, "________", namezad, 'a');
	field_window(fld,"kdr3.1  ", 80, 25);
	clear_template(wnd);
        data_entry(wnd);
	delete_window(wnd);
     } /* Конец кадра ввода имени задачи */

/* ================================================================= */
       /* Создание строки для текущей директории <s> = <file>.<ext> */
   void im_ext(char *s, char *file, char ext[])
    {
    char drive[MAXDRIVE];
    char dir[MAXDIR];
    char fi[MAXFILE];
    char ex[MAXEXT];

    getcwd(s,MAXPATH);              /* get the current working directory */
    strcat(s,"\\");                  /* append on a trailing \ character */
    fnsplit(s,drive,dir,fi,ex); /* split the string to separate elems */
    strcpy(fi,file);
    strcpy(ex,ext);
    fnmerge(s,drive,dir,fi,ex);   /* merge everything into one string */
      }  /* Конец создания строки */


      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
      /*               Чтение результатов счета                  */
      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
		   /* Чтение результатов счета */
	  void readrez(FILE *stream) {
	       size=sizeof(nzap); fread(&nzap, size, 1, stream);
				  fread(&nstp, size, 1, stream);
				  fread(&ncr, size, 1, stream);
				  fread(&marker, size, 1, stream);
                                  ii=ncr+1;
	       size=sizeof(t);    fread(&t, size, 1, stream);
				  fread(&dlina, size, 1, stream);
				  fread(&massa, size, 1, stream);
				  fread(&impulse, size, 1, stream);
				  fread(&energy, size, 1, stream);
		fread(&c2, size, ii, stream);
		fread(&c3, size, ii, stream);
		fread(&c4, size, ii, stream);
		fread(&c5, size, ii, stream);
	  } /* Конец чтения результатов с диска */

       int schitg(void)   /* Считывание  или запись <namezad>.g */
       {
	WINDOW *wnd;
  FIELD *fld;
	FILE *stream;
	im_ext(spath,namezad,".g");    /* Pass - в spath */
   if ((stream = fopen(spath, "rb")) == NULL) { /* Не заданы параметры света */
	frstent = 1;}
   if (frstent == 0) {   /* Считывание параметров света */

   size=sizeof(gr);
   fread(&gr, size, 1, stream);
   fread(&gp, size, 1, stream);
   fread(&gv, size, 1, stream);
   fread(&ge, size, 1, stream);
   fread(&grl, size, 1, stream);
   fread(&grr, size, 1, stream);
   fread(&gpl, size, 1, stream);
   fread(&gpr, size, 1, stream);
   fread(&gvl, size, 1, stream);
   fread(&gvr, size, 1, stream);
   fread(&gel, size, 1, stream);
   fread(&ger, size, 1, stream);
   fclose(stream);   }    /* Конец считывания */


		    /* Задание или просмотр параметров света */
	load_help("kdr3.hlp");
	wnd = establish_window(2, 1, 23, 76);
	set_title(wnd, namezad);
	set_colors(wnd, ALL, AQUA, BLACK, DIM);
	set_colors(wnd, ACCENT, WHITE, BLACK, DIM);
	display_window(wnd);

	wprompt(wnd, 2, 1, "Высвечиваемые параметры:");
	wprompt(wnd, 2, 3, "Плотность - ");
	wprompt(wnd, 2, 5, "Давление  - ");
	wprompt(wnd, 2, 7, "Скорость  - ");
	wprompt(wnd, 2, 9, "Энергия   - ");
	wprompt(wnd, 2, 11,"Границы изменения параметров:");
	wprompt(wnd, 5, 13,"Плотность");
	wprompt(wnd, 5, 15,"Давление");
	wprompt(wnd, 5, 17,"Скорость");
	wprompt(wnd, 5, 19,"Энергия");
      if (frstent==1)                 init_template(wnd);
	fld = establish_field(wnd, 15, 3, "_", gr, 'a');
	field_window(fld,"kdr3.21 ", 80, 25);
	fld = establish_field(wnd, 15, 5, "_", gp, 'a');
	field_window(fld,"kdr3.22 ", 80, 25);
	fld = establish_field(wnd, 15, 7, "_", gv, 'a');
	field_window(fld,"kdr3.23 ", 80, 25);
	fld = establish_field(wnd, 15, 9, "_", ge, 'a');
	field_window(fld,"kdr3.24 ", 80, 25);
	fld = establish_field(wnd, 16, 13, "________", grl, 'a');
	field_window(fld,"kdr3.25 ", 80, 25);
	fld = establish_field(wnd, 27, 13, "________", grr, 'a');
	field_window(fld,"kdr3.25 ", 80, 25);
	fld = establish_field(wnd, 16, 15, "________", gpl, 'a');
	field_window(fld,"kdr3.25 ", 80, 25);
	fld = establish_field(wnd, 27, 15, "________", gpr, 'a');
	field_window(fld,"kdr3.25 ", 80, 25);
	fld = establish_field(wnd, 16, 17, "________", gvl, 'a');
	field_window(fld,"kdr3.25 ", 80, 25);
	fld = establish_field(wnd, 27, 17, "________", gvr, 'a');
	field_window(fld,"kdr3.25 ", 80, 25);
	fld = establish_field(wnd, 16, 19, "________", gel, 'a');
	field_window(fld,"kdr3.25 ", 80, 25);
	fld = establish_field(wnd, 27, 19, "________", ger, 'a');
	field_window(fld,"kdr3.25 ", 80, 25);
      if (frstent==1)	clear_template(wnd);
	data_entry(wnd);
		    /* Конец задания или просмотра параметров света */
		    
		    /* Запись файла .g */
   if ((stream = fopen(spath, "wb")) == NULL) 
       printf("\n\n\n\nНе могу открыть файл.g на запись");

   /* Запись параметров счета */
   size=sizeof(gr);
   fwrite(&gr, size, 1, stream);
   fwrite(&gp, size, 1, stream);
   fwrite(&gv, size, 1, stream);
   fwrite(&ge, size, 1, stream);
   fwrite(&grl, size, 1, stream);
   fwrite(&grr, size, 1, stream);
   fwrite(&gpl, size, 1, stream);
   fwrite(&gpr,  size, 1, stream);
   fwrite(&gvl, size, 1, stream);
   fwrite(&gvr,  size, 1, stream);
   fwrite(&gel, size, 1, stream);
   fwrite(&ger,  size, 1, stream);
      fclose(stream);
      frstent=0;
      return(0);

       }	 /* Конец процедуры считывания и записи параметров счета */



