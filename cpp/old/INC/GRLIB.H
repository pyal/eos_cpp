// uses fobj,util;
#ifndef __GRLIB_H
#define __GRLIB_H

//#include <mem.h>
#include <stdlib.h>
#include <math.h>
//#include <graphics.h>
#include <string.h>
#include <stdio.h>
#include "fobj.h"
#include "lib/std/const.h"
#include "lib/std/util.h"

#define LST ( (GrLine*)(Item*)(*this) )
#define LST_PL ( (GrLine*)(Item*)++(*this) )
#define LST_MI ( (GrLine*)(Item*)--(*this) )

enum AxType{X,Y};
struct Cell { double x,y;};
struct Rect { Cell dn,up; };
               
struct ScrPixel:Cell
  {
   int Color,Hidden;
   virtual void Show();
   virtual void Hide(); 
   virtual void Move(double dx,double dy);
   int PixColor(int col=-1);
   double CellX();
   double CellY();
   double CellX(double newx);
   double CellY(double newy);
   ScrPixel();
   ScrPixel(double x_pix,double y_pix,int col_pix=-1);
   void SetPixel(double x_pix,double y_pix,int col_pix=-1);
   ~ScrPixel(){};//{Hide();};
  };
struct ScrCurve:ScrPixel  // Screen positions...........
  {
   Cell Dcell;
   linesettingstype CurveStyle;

// int thickness(NORNWIDTH,THICKWIDTH);
// int linestyle(SOLID_LINE,DOTTED_LINE,CENTER_LINE,USERBIT_LINE,DASHED_LINE);
// unsigned upattern;

   ScrCurve(double x_low,double y_low,double x_up,double y_up,
                                                  int col_curve=-1);
   ScrCurve(Rect scr,int col_curve=-1);
   ScrCurve();
   linesettingstype LStyle(int thick,int style,unsigned userpat);
   linesettingstype LStyle();
   void CurveResize(double x_low,double y_low,double x_up,double y_up);
//   virtual void ScrResize(double x_up,double y_up);
   virtual void Move(double dx,double dy);

  };
struct RectProc
  {  
   Rect BlowRect(Rect in,Cell center,double coef) ;
   Rect MoveRect(Rect in,Cell step) ;
   Cell TurnCell(Cell in,Cell center,double phi) ;
   Rect TurnRect(Rect in,Cell center,double phi) ;
   Cell CentRect(Rect in) ;
  };  

struct CellConv:RectProc
    
  {
   Rect Scr,Gr;
	 Rect Scr_Gr,Gr_Scr;

//   CellConv(Cell Bcell,Cell Dcell,Rect grph);
   CellConv(Rect screen,Rect grph);
   CellConv();
      
   Rect SetScr();
   Rect SetGr();
   void SetScr(Rect screen);
   void SetGr(Rect grph);

   Cell ScrCell(double x,double y);
   Cell GrCell(double x,double y);
	 private:
   void SetCoef(Rect screen,Rect grph);
   Rect CrCoef(Rect fr,Rect to) ;
   Cell Convert(Rect conv,Cell source);
  };


struct GrLine:ScrCurve,CellConv,Item
                                 // Item - to use GrLine in LstCl
  {
//   int DrawMode;
//   int PixStyle;
   GrLine(Rect scr,Rect grph,int col_curve=-1);
   GrLine(){Hidden=1;};

   void SetDraw(int dr=1)
     {
//     DrawMode=dr;
//     if (dr) setwritemode(COPY_PUT);   else
     setwritemode(XOR_PUT);}
   void DrawLine(double x0,double y0,double x1,double y1);
//===================================================
   void HideLine(double x0,double y0,double x1,double y1);
   void LineTo(double x1,double y1);
   void GrGoto(double x1,double y1);
//===================================================
   void SetScr(double x_low,double y_low,double x_up,double y_up);
   void SetScr(Rect to);
   void SetGr(double x_low,double y_low,double x_up,double y_up);
   void SetGr(Rect to);

  };
struct TextStrc { int Font,Direc,Just,BkColor;};

struct ScrText:ScrPixel  // Screen positions...........

  {
   char* buf;
   char* fil;
   TextStrc DrawMeth;
//   double ChSizeX,ChSizeY;
   ScrText()                                      //????????????????????????????
      {buf=NULL;fil=NULL;
       DrawMeth.Font=DEFAULT_FONT;DrawMeth.Direc=HORIZ_DIR;
       DrawMeth.Just=CENTER_TEXT;
       DrawMeth.BkColor=getbkcolor();Hidden=1;
      };
   ScrText(char* mes,double x_mes,double y_mes,TextStrc meth)
					     :ScrPixel(x_mes,y_mes)
      {
       Hidden=1;
       DrawMeth=meth;
       buf=strdup(mes);fil=strdup(buf);strset(fil,'█');
      };
   void SetScrText(char* mes,double x_mes,double y_mes,TextStrc meth,int Color)
      {
       Hide();
       DrawMeth=meth;
       if (buf!=NULL) {delete buf;delete fil;}
       buf=strdup(mes);fil=strdup(buf);strset(fil,'█');
       CellX(x_mes);CellY(y_mes);PixColor(Color);
      };
   void SetTxtMeth(TextStrc meth){Hide();DrawMeth=meth;Show();};
   void Show();
   void Hide();
   ~ScrText();
  };


struct ValStrc { int NumPos,NumBpoint,NumApoint,DrawNum,Horis;};
struct TicStrc { int Ntic,Ltic,DrawGrid;};

struct GrTic:virtual GrLine
  {
   TicStrc Tic;
   ValStrc Val;
   AxType type;
   int Hid;
   GrTic()
     {
      Tic.Ntic=0;Tic.Ltic=0;Tic.DrawGrid=0;Hid=0;type=X;
      Val.NumPos=0;Val.NumBpoint=0;Val.NumApoint=0;Val.DrawNum=0;Val.Horis=0;
      Hidden=1;
     };
   GrTic(TicStrc ticst,ValStrc valst,AxType t){Tic=ticst;Val=valst;type=t;Hidden=1;};
   GrTic(Rect screen,Rect grph,TicStrc ticst,ValStrc valst,AxType t);
   void Show();
   void Hide();
   private:
   void DrVal(double *ar,int col);
   void DrGrid(double *ar);
   double* TicAr();

  };

//===================================================

struct Axis:virtual GrLine,GrTic,ScrText
  {
   AxType type;
   Axis(Rect scr,Rect grph,char *buf,double x_mes,double y_mes,TextStrc meth,
	     TicStrc ticst,ValStrc valst,AxType ax);
   Axis(Rect scr,Rect grph,TicStrc ticst,ValStrc valst,AxType ax);
   Axis(Rect scr,Rect grph,char *buf,double x_mes,double y_mes,TextStrc meth,
								 AxType ax);
   Axis(Rect scr,Rect grph,AxType ax);
   Axis(){type=X;};
   void Show();
   void Hide();
   ~Axis();
  };
struct GrArr:GrLine
  {
   double* Xar; 
   double* Yar; 
   int Num;
   GrArr(double* xgr,double* ygr,int n,Rect screen,Rect grph,int gr_col);
   GrArr(double* xgr,double* ygr,int n);
   GrArr();
   ~GrArr();
   void Show();
   void Hide();
   void Draw();
   void ArrReset(double* xgr,double* ygr,int n);
   Rect ArrRect();
   private:
   virtual void GetArMax(double *Ar,int n,double &xmin,double &xmax);
  };
struct GrScr:LstCl
  {
   Rect Scr,Gr;
   void Show();
   void Hide();
   void Move(double x1,double y1);
   Rect ScrDim();
   void ScrDim(Rect rectnew);
   Rect GrDim();
   void GrDim(Rect rectnew);
   Rect MaxGrDim();
   GrScr();
   ~GrScr();
  };
//================================================================
//==========      Simple Procedure using Classes  ================
//================================================================

GrScr *AxGraph(GrLine *ar,char *Xstr,char *Ystr);
void InitGrph();

#endif





























/*
 
  
  
 struct GrText:
//               virtual GrLine,
			 ScrText
   {
    GrText(char* mes,double x_mes,double y_mes,TextStrc meth)
      :ScrText(mes,x_mes,y_mes,meth){};
    GrText(){};
//    void Show(){};// simply to recalculate Xscr from Xgr
//    void Hide(){};
   };
  
*/  
  
