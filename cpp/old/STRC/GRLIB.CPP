//uses graphics,fobj;
#include "grlib.h"

//========================================================
//================= ScrPixel:Cell ========================
//========================================================

void ScrPixel::Show()
    { if (Hidden) putpixel((int)x,(int)y,Color);Hidden=0;};
void ScrPixel::Hide()
    { if (!Hidden) putpixel((int)x,(int)y,getbkcolor());Hidden=1;};
void ScrPixel::Move(double dx,double dy)
    { int hid=Hidden;Hide();x+=dx;y+=dy;if (!hid) Show();};
int ScrPixel::PixColor(int col)
   {
    if (col ==-1) return Color;
    int hid=Hidden;Hide();Color=col;if (!hid) Show();return Color;
   };
double ScrPixel::CellX(){return x;};
double ScrPixel::CellY(){return y;};
double ScrPixel::CellX(double newx){x=newx;return x;};
double ScrPixel::CellY(double newy){y=newy;return y;};
ScrPixel::ScrPixel(){Hidden=1;x=0;y=0;Color=YELLOW;};
ScrPixel::ScrPixel(double x_pix,double y_pix,int col_pix)
   {
     Hidden=1;SetPixel(x_pix,y_pix,col_pix);
   };
void ScrPixel::SetPixel(double x_pix,double y_pix,int col_pix)
   { x=x_pix;y=y_pix;Hide();
     if (col_pix==-1) col_pix=getcolor();//YELLOW;
     PixColor(col_pix);
   };
//-----------ScrPixel::~ScrPixel(){Hide();};

//========================================================
//=================  ScrCurve:ScrPixel====================
//========================================================

ScrCurve::ScrCurve(double x_low,double y_low,double x_up,double y_up,
                   int col_curve):ScrPixel(x_low,y_low,col_curve)
  {Dcell.x=x_up-x_low;Dcell.y=y_up-y_low; getlinesettings(&CurveStyle); };

ScrCurve::ScrCurve(Rect scr,int col_curve)
    :ScrPixel(scr.dn.x,scr.dn.y,col_curve)
  {
   Dcell.x=scr.up.x-scr.dn.x;Dcell.y=scr.up.y-scr.dn.y;
   getlinesettings(&CurveStyle);
   };
ScrCurve::ScrCurve()
  {getlinesettings(&CurveStyle);Dcell.x=1;Dcell.y=1; };
linesettingstype ScrCurve::LStyle(int thick,int style,unsigned userpat)
  {
   CurveStyle.thickness=thick;CurveStyle.linestyle=style;
   CurveStyle.upattern=userpat;
   return CurveStyle;
  };
linesettingstype ScrCurve::LStyle(){ return CurveStyle;};
void ScrCurve::CurveResize(double x_low,double y_low,
					       double x_up,double y_up)
  { int hid=Hidden;Hide();
    x=x_low;y=y_low;
    Dcell.x=x_up-x_low;Dcell.y=y_up-y_low;
    if (!hid) Show();
  };
//-----------void ScrCurve::ScrResize(double x_up,double y_up) 
//-----------------  {ScrResize(x,y,x_up,y_up);};
void ScrCurve::Move(double dx,double dy) {CurveResize(x,y,x+dx,y+dy);};

//========================================================
//=================  RectProc  ===========================
//========================================================

Rect RectProc::BlowRect(Rect in,Cell center,double coef) 
  {
   Rect ret;
   ret.dn.x=(in.dn.x-center.x)*coef+center.x;
   ret.up.x=(in.up.x-center.x)*coef+center.x;
   ret.dn.y=(in.dn.y-center.y)*coef+center.y;
   ret.up.y=(in.up.y-center.y)*coef+center.y;
   return ret;
  };
Rect RectProc::MoveRect(Rect in,Cell step)
  {
   Rect ret;
   ret.dn.x=(in.dn.x+step.x);ret.up.x=(in.up.x+step.x);
   ret.dn.y=(in.dn.y+step.y);ret.up.y=(in.up.y+step.y);
   return ret;
  };
Cell RectProc::TurnCell(Cell in,Cell center,double phi) 
  {
   Cell ret,d;
   d.x=in.x-center.x;d.y=in.y-center.y;
   ret.x= d.x*cos(phi)+d.y*sin(phi);
   ret.y=-d.x*sin(phi)+d.y*cos(phi); 
   return ret;
  };
Rect RectProc::TurnRect(Rect in,Cell center,double phi) 
  {
   Rect ret;
   ret.dn=TurnCell(in.dn,center,phi);
   ret.dn=TurnCell(in.up,center,phi);
   return ret;
  };
Cell RectProc::CentRect(Rect in) 
  {
   Cell ret;
   ret.x=(in.up.x+in.dn.x)/2;
   ret.y=(in.up.y+in.dn.y)/2;
   return ret;
  };

//========================================================
//=================  CellConv:RectProc ===================
//========================================================

CellConv::CellConv(Rect screen,Rect grph){ SetCoef(screen,grph);};
CellConv::CellConv()
 {
   Rect screen={0,0,0,0};
   Rect  grph={0,0,1,1};
   screen.up.x=getmaxx();screen.dn.y=getmaxy();
   screen=BlowRect(screen,CentRect(screen),0.8);
   SetCoef(screen,grph);
 };
Rect CellConv::SetScr() { return Scr;};
Rect CellConv::SetGr()  { return Gr;};
void CellConv::SetScr(Rect screen) { SetCoef(screen,Gr); };
void CellConv::SetGr(Rect grph)    { SetCoef(Scr,grph);  };

Cell CellConv::ScrCell(double x,double y){Cell ret={x,y};return Convert(Gr_Scr,ret);};
Cell CellConv::GrCell(double x,double y){Cell ret={x,y};return Convert(Scr_Gr,ret);};
void CellConv::SetCoef(Rect screen,Rect grph)
  {
   Scr=screen;Gr=grph;
   Scr_Gr=CrCoef(Scr,Gr);
   Gr_Scr=CrCoef(Gr,Scr);
  };
Rect CellConv::CrCoef(Rect fr,Rect to)
  {
   Rect ret;
   double a;
   a=(fr.up.x-fr.dn.x);
   if (fabs(a)<=MathZer) a=1;
   a=(to.up.x-to.dn.x)/a;
   ret.up.x=a;
   ret.dn.x=to.dn.x-fr.dn.x*a;

   a=(fr.up.y-fr.dn.y);
   if (fabs(a)<=MathZer) a=1;
   a=(to.up.y-to.dn.y)/a;
   ret.up.y=a;
   ret.dn.y=to.dn.y-fr.dn.y*a;
   return ret;
  };
Cell CellConv::Convert(Rect conv,Cell source)
  {
   source.x=conv.up.x*source.x+conv.dn.x;
   source.y=conv.up.y*source.y+conv.dn.y;
   return source;
  };

//========================================================
//=====GrLine:virtual ScrCurve,virtual CellConv,Item  ====
//========================================================

GrLine::GrLine(Rect scr,Rect grph,int col_curve)
   :ScrCurve(scr,col_curve),CellConv(scr,grph){Hidden=1;};
//---------------------GrLine::GrLine(){};

void GrLine::DrawLine(double x0,double y0,double x1,double y1)
  {
   Cell fr=ScrCell(x0,y0),to=ScrCell(x1,y1);
   int Temp=getcolor();setcolor(Color);
   line(fr.x,fr.y,to.x,to.y);
//   putpixel(to.x,to.y,Color);
   setcolor(Temp);
  };
void GrLine::HideLine(double x0,double y0,double x1,double y1)
  {
   Cell fr=ScrCell(x0,y0),to=ScrCell(x1,y1);
   int Temp=getcolor();setcolor(/*getbkcolor()*/Color);
//   if (fabs(to.x-fr.x)+fabs(to.y-fr.y)>3)
      line(fr.x,fr.y,to.x,to.y);
//   putpixel(to.x,to.y,Color);
   setcolor(Temp);
  };
void GrLine::LineTo(double x1,double y1)
  {
   Cell to=ScrCell(x1,y1);
   int Temp=getcolor();setcolor(Color);
//   if (fabs(to.x-getx())+fabs(to.y-gety())>3)
   line(getx(),gety(),getx(),gety());
   lineto(to.x,to.y);
   setcolor(Temp);
  };
void GrLine::GrGoto(double x1,double y1)
  {Cell to=ScrCell(x1,y1);moveto((int)to.x,(int)to.y);};

void GrLine::SetScr(double x_low,double y_low,double x_up,double y_up)
  {
   Rect screen={x_low,y_low,x_up,y_up};
   int hid=Hidden;Hide();
   CellConv::SetScr(screen);
   if (!hid) Show();
  };
void GrLine::SetScr(Rect to) {SetScr(to.dn.x,to.dn.y,to.up.x,to.up.y);};
void GrLine::SetGr(double x_low,double y_low,double x_up,double y_up)
  {
   Rect grph={x_low,y_low,x_up,y_up};
   int hid=Hidden;Hide();
   ScrCurve::CurveResize(x_low,y_low,x_up,y_up);
   CellConv::SetGr(grph);
   if (!hid) Show();
  };
void GrLine::SetGr(Rect to) {SetGr(to.dn.x,to.dn.y,to.up.x,to.up.y);};

//========================================================
//====================ScrText:ScrPixel====================
//========================================================

void ScrText::Show()
  {
   if (!Hidden) return;
   if (buf==NULL) return;
   settextjustify(DrawMeth.Just,DrawMeth.Just);
   settextstyle(DrawMeth.Font,DrawMeth.Direc,0);
   int Temp=getcolor();
   setcolor(DrawMeth.BkColor);
   outtextxy((int)x,(int)y,fil);
   setcolor(PixColor());
   outtextxy((int)x,(int)y,buf);
   setcolor(Temp);
//cout<<"  buf "<<buf<<" X "<<x<<" Y "<<y<<"\n";
   Hidden=0;
  };
void ScrText::Hide()
  {
   if (Hidden) return;
   if (buf==NULL) return;
   settextjustify(DrawMeth.Just,DrawMeth.Just);
   settextstyle(DrawMeth.Font,DrawMeth.Direc,0);
   int Temp=getcolor();
   setcolor(getbkcolor());
   outtextxy((int)x,(int)y,fil);
   setcolor(Temp);
   Hidden=1;
  };
ScrText::~ScrText(){if (buf==NULL) return;  delete buf;delete fil;};

//========================================================
//====================GrTic:GrLine  ======================
//========================================================

   GrTic::GrTic(Rect screen,Rect grph,TicStrc ticst,ValStrc valst,AxType t):
     GrLine(screen,grph,getcolor()){Tic=ticst;Val=valst;type=t;Hidden=1;};
void GrTic::Show()
  {
   if (Hidden)
     {
      double *ar=TicAr();
      SetDraw();
      DrGrid(ar);DrVal(ar,PixColor());
      delete ar;
      Hidden=0;
     }
  };
void GrTic::Hide()
  {
   if (!Hidden)
     {
      double *ar=TicAr();
      SetDraw(0);
      DrGrid(ar);DrVal(ar,getbkcolor());
      delete ar;
      SetDraw();
      Hidden=1;
     }
  };
void GrTic::DrGrid(double *ar)
  {
   if (Tic.DrawGrid)
     {
      Rect gr=CellConv::SetGr();
      double len=(gr.up.x-gr.dn.x)/Tic.Ltic;
      if (type==X) len=len=(gr.up.y-gr.dn.y)/Tic.Ltic;
      for (int k=1;k<=ar[0];k++)
	{
	 if (type==X) { GrGoto(ar[k],gr.dn.y);LineTo(ar[k],gr.dn.y-len);}
	 else         { GrGoto(gr.dn.x,ar[k]);LineTo(gr.dn.x-len,ar[k]);}
	}
     }
  }
void GrTic::DrVal(double *ar,int col)
  {
   if (Val.DrawNum)
     {
      Rect gr=CellConv::SetGr();
      double len=(gr.up.x-gr.dn.x)/Tic.Ltic;
      if (type==X) len=(gr.up.y-gr.dn.y)/Tic.Ltic;
      setcolor(col);
      if (type==X) {settextjustify(CENTER_TEXT,CENTER_TEXT);
		 settextstyle(DEFAULT_FONT,HORIZ_DIR,0);}
       else         {settextjustify(RIGHT_TEXT,RIGHT_TEXT);
		 settextstyle(DEFAULT_FONT,/*VERT*/HORIZ_DIR,0);}
      char str[80];
//      int a,b;
      for (int k=1;k<=ar[0];k++)
	{
	 sprintf(str,"% .5g ",ar[k]);
	 if (type==X)  {GrGoto(ar[k],gr.dn.y-2*len);
			outtext(str);}
	 else          {GrGoto(gr.dn.x-2*len,ar[k]);
			outtext(str);}
	}
     }
  }

double* GrTic::TicAr()
  {
   Rect gr=CellConv::SetGr();
   if (type==Y) {gr.up.x=gr.up.y;gr.dn.x=gr.dn.y;}

   double dx=(gr.up.x-gr.dn.x)/Tic.Ntic;
//   normdb(dx,val,e);
//   double arstp[]={4,1,2,5,10};
//   double stp=GetMin(arstp,val);

   double lnDx=exp(   log(10)*ceil( log(dx)/log(10) )  );
   double stp,x0;

   if ((dx/lnDx)<0.31) stp=0.2;
   else if ((dx/lnDx)<0.75) stp=0.5;
     else if ((dx/lnDx)<1.5) stp=1;
       else stp=2;
   stp=stp*lnDx;
   x0=(int)(gr.dn.x/stp)*stp;
   if (x0<gr.dn.x) x0+=stp;
   double *ar=new double[floor((gr.up.x-x0)/stp)+2];
   int k=1;ar[1]=x0;
   while (ar[k]+stp<gr.up.x ) {k++;ar[k]=ar[k-1]+stp;}
   ar[0]=k;
   return ar;
  };

//========================================================
//==================== Axis:GrLine  ======================
//========================================================

Axis::Axis(Rect scr,Rect grph,char *buf,double x_mes,double y_mes,TextStrc meth,
	  TicStrc ticst,ValStrc valst,AxType ax):
        GrLine(scr,grph,YELLOW),ScrText(buf,x_mes,y_mes,meth),
            GrTic(ticst,valst,ax){type=ax;GrLine::Hidden=1;} ;
Axis::Axis(Rect scr,Rect grph,TicStrc ticst,ValStrc valst,AxType ax):
        GrLine(scr,grph,YELLOW),GrTic(ticst,valst,ax){type=ax;GrLine::Hidden=1;} ;
Axis::Axis(Rect scr,Rect grph,char *buf,double x_mes,double y_mes,TextStrc meth,
							      AxType ax):
         GrLine(scr,grph,YELLOW),ScrText(buf,x_mes,y_mes,meth){type=ax;GrLine::Hidden=1;} ;
Axis::Axis(Rect scr,Rect grph,AxType ax):GrLine(scr,grph,YELLOW){type=ax;GrLine::Hidden=1;} ;

void Axis::Show()
  {
    if (!GrLine::Hidden) return;
    GrTic::Show();ScrText::Show();
    SetDraw();
    if (type==X) DrawLine(Gr.dn.x,Gr.dn.y,Gr.up.x,Gr.dn.y);
    else DrawLine(Gr.dn.x,Gr.up.y,Gr.dn.x,Gr.dn.y);
    GrLine::Hidden=0;
  };
void Axis::Hide()
  {
    if (GrLine::Hidden) return;
    GrTic::Hide();ScrText::Hide();
    SetDraw(0);
    if (type==X) DrawLine(Gr.dn.x,Gr.dn.y,Gr.up.x,Gr.dn.y);
    else DrawLine(Gr.dn.x,Gr.up.y,Gr.dn.x,Gr.dn.y);
    GrLine::Hidden=1;
    SetDraw();
  };
Axis::~Axis(){};//{Hide();};

//========================================================
//==================== GrArr:GrLine ======================
//========================================================
GrArr::GrArr(double* xgr,double* ygr,int n,Rect screen,Rect grph,int gr_col):
	       GrLine(screen,grph,gr_col)
   { Xar=xgr;Yar=ygr;Num=n;Hidden=1;};
GrArr::GrArr(double* xgr,double* ygr,int n)
  { Xar=xgr;Yar=ygr;Num=n;Rect gr=ArrRect();
    SetGr(BlowRect(gr,CentRect(gr),1.2));};
GrArr::GrArr() { Num=0;};
GrArr::~GrArr(){};// { Hide();};
void GrArr::Show()
  {
   if ((!Hidden) || (Num<2)) return;
   SetDraw();
   Draw();
   Hidden=0;
  };
void GrArr::Hide()
  {
   if ((Hidden) || (Num<2)) return;
   SetDraw(0); 
   Draw();
   Hidden=1;
   SetDraw();
  };
void GrArr::Draw()
  {
   DrawLine(Xar[1],Yar[1],Xar[2],Yar[2]);GrGoto(Xar[2],Yar[2]);
   for (int k=3;k<=Num;k++) LineTo(Xar[k],Yar[k]);
   Hidden=0;
  };
void GrArr::ArrReset(double* xgr,double* ygr,int n)
   { Hide();Xar=xgr;Yar=ygr;Num=n; };
Rect GrArr::ArrRect()
  {
   Rect ret;
   GetArMax(Xar,Num,ret.dn.x,ret.up.x);
   GetArMax(Yar,Num,ret.dn.y,ret.up.y);
   if (ret.up.x-ret.dn.x<MathZer)  { ret.up.x+=1; ret.dn.x-=1; }
   if (ret.up.y-ret.dn.y<MathZer)  { ret.up.y+=1; ret.dn.y-=1; }
   return ret;
  };
void GrArr::GetArMax(double *Ar,int n,double &xmin,double &xmax)
  {
   xmin=MathInf,xmax=-MathInf;
   for (int k=1;k<=n;k++)
     {
      if (xmin>Ar[k]) xmin=Ar[k];
      if (xmax<Ar[k]) xmax=Ar[k];
     };
  };
//========================================================
//==================== GrScr:LstCl =======================
//========================================================
void GrScr::Show()
  {
   GrDim(Gr);ScrDim(Scr);
   LstReset();
   while (LST_PL!=NULL)
                        LST->Show();
   LST_MI;
  };
void GrScr::Hide() { LstReset();while (LST_PL!=NULL) LST->Hide();LST_MI; };
void GrScr::Move(double x1,double y1)
   { LstReset();while (LST_PL!=NULL) LST->Move(x1,y1);LST_MI; };
Rect GrScr::ScrDim(){return Scr;};
void GrScr::ScrDim(Rect rectnew)
  {
   LstReset();
   while (LST_PL!=NULL)
       LST->SetScr(rectnew);
   Scr=rectnew;
   LST_MI;
  };
Rect GrScr::GrDim(){return Gr;};
void GrScr::GrDim(Rect rectnew)
  {
   LstReset();
   while (LST_PL!=NULL)
       LST->SetGr(rectnew);
   Gr=rectnew;
   LST_MI;
  };
GrScr::GrScr()
  {
   CellConv conv;
   Scr=conv.SetScr();
   Gr=Scr;
  };
Rect GrScr::MaxGrDim()
  {
   LstReset();LST_PL;
   Rect ret=LST->CellConv::SetGr(),cur;
   while (LST_PL!=NULL)
    {
     cur=LST->CellConv::SetGr();
     ret.dn.x=min<double>(cur.dn.x,ret.dn.x);
     ret.dn.y=min<double>(cur.dn.y,ret.dn.y);
     ret.up.x=max<double>(cur.up.x,ret.up.x);
     ret.up.y=max<double>(cur.up.y,ret.up.y);
    }
   LST_MI;
   return ret;
  };
GrScr::~GrScr(){ Hide();LstReset();};


//================================================================
//==========      Simple Procedure using Classes  ================
//================================================================

GrScr *AxGraph(GrLine *ar,char *Xstr,char *Ystr)
  {
   GrScr *sum=new GrScr();

   TextStrc meth={DEFAULT_FONT,HORIZ_DIR,CENTER_TEXT,BLUE};
   TicStrc ticst={5,30,1};
   ValStrc valst={6,2,4,1,1};
   sum->LstAdd(new Axis(ar->CellConv::SetScr(),ar->CellConv::SetGr(),
                           Xstr,350,470,meth,ticst,valst,(AxType)0) );
   meth.Direc=VERT_DIR;ticst.Ltic=70;
   sum->LstAdd(new Axis(ar->CellConv::SetScr(),ar->CellConv::SetGr(),
                           Ystr,10,200,meth,ticst,valst,(AxType)1) );
   sum->GrDim(ar->CellConv::SetGr());
   sum->ScrDim(ar->CellConv::SetScr());
   return sum;
  };
void InitGrph()
  {  
   int graphdrive=DETECT,graphmode;
   initgraph(&graphdrive,&graphmode,"");
   cleardevice();
   setcolor(YELLOW);
  };
























/*
  gr_abs::gr_abs (int number;double wax[];)
    {
     num=number;
     wax=new double[num];
     movmem(ax,*wax,sizeof(double)*num);
    }
  gr_abs::~gr_abs (){ delete wax;}
  double gr_abs::max<double>()
    {
     double m=MachInf;
     for (int k=0; k<=num;k++ ) { m=MAX(m,*wax[k]) }
     return m;
    }
  double gr_abs::min() 
    {
     double m=-MachInf;
     for (int k=0; k<=num;k++ ) { m=MIN(m,*wax[k]) }
     return m;  
    }
  void gr_abs::moveax(double shift;) 
    { for (int k=0; k<=num;k++ )  wax[k]+=shift;}
  void gr_abs::blowax(double blow;)
    { for (int k=0; k<=num;k++ )  wax[k]*=blow;}
  void gr_abs::fitdim(maxv,minv;)
    {
       void blow(double maxv,minv;)
         {
          double maxc=max<double>(),minc=min(),del;
          if ((del=maxc-minc)>MachZer) 
            { 
             del=(maxv-minv)/del;
             for (int k=0; k<=num;k++ )  wax[k]*=del;
            }
         }
      blow(maxv,minv);
      moveax((maxv+minv-max<double>()-min())/2);
     }
   void gr_abs::limax(double maxv,minv;)
    {
     if maxv>minv { double t=maxv;maxv=minv;minv=t;}
     for (int k=0; k<=num;k++ )  
       {
        wax[k]=MIN(wax[k],maxv );
        wax[k]=MAX(wax[k],minv );
       }
    }

double scrxl,scrxh,scryl,scryh,grxl,grxh,gryl,gryh;
void drawstr::setscr(double xl,yl,xh,yh)
  {scrxl=getmaxx()*xl/100;scrxh=getmaxx()*xh/100;
   scryl=getmaxy()*yl/100;scryh=getmaxy()*yh/100;
  }
void drawstr::setgr(double xl,yl,xh,yh);
  {grxl=xl;grxh=xh;gryl=yl;gryh=yh;}
void drawstr::setgr(int num;double xar[],yar[];double xl,yl,xh,yh);
  {
   gr_abs *ax=new gr_abs(num,xar[]);
   gr_abs *ay=new gr_abs(num,yar[]);
   double xma=*ax.max<double>(),xmi=*ax.min(),yma=*ay.max<double>(),ymi=*ay.min();
   double dx=*xma-xmi,dy=yma-ymi;
   grxl=xmi-dx*xl;grxh=xma-dx*xh;
   gryl=ymi-dy*yl;gryl=ymi-dy*xl;
   delete ax;
   delete ay;
  }
void drawstr::drawax (int numx,numy;boolean drawxset=false,drawyset=false;);
  {
   setln;
   drawb;
   calcst(numx,grxl,grxh);
   calcst(numy,gryl,gryh);
   if drawxset then 

void drawstr::drawgr (int num;double xar[],yar[];);
void drawstr::drawst (line_styles ls=SOLID_LINE;line_width lw=NORMAL_WIDTH;
             int wrmode=0,wrcolor=16;);

procedure axises(dim,mfit:dimenshions;var coef:convcoef;drawx,drawy:boolean;
                          nomlinex,nomliney,nom:integer);
var
  k,k1,kx,ky    :integer;
  LS            :myLineStyleType;
  rea1,rea2     :arraybadreal;
  st            :string;
  x,x1,x2,delx,y,
  y1,y2,dely,stepx,stepy    :real;
  ax,ay                     :boolean;
begin
  with myLineStyle do Begin
    settextjustify(centertext,centertext);settextstyle(smallfont,horizdir,1);
    getmyLineStyle(ls);setmyLineStyle(myLineStyle);
    moveto(round(mfit.xmi),round(mfit.ymi));
    lineto(round(mfit.xma),round(mfit.ymi));
    lineto(round(mfit.xma),round(mfit.yma));
    lineto(round(mfit.xmi),round(mfit.yma));
    lineto(round(mfit.xmi),round(mfit.ymi));
    if dim.yma=dim.ymi then begin
      dim.yma:=dim.yma+abs(dim.yma)/2; dim.ymi:=dim.ymi-abs(dim.ymi)/2;
    end;
    if dim.xma=dim.xmi then begin
      dim.xma:=dim.xma+abs(dim.xma)/2; dim.xmi:=dim.xmi-abs(dim.xmi)/2;
    end;
    delx:=abs(dim.xma-dim.xmi); dely:=abs(dim.yma-dim.ymi);
    x1:=delx/nomlinex; y1:=dely/nomliney;
    x2:=exp(ln(10)*round(ln(x1)/ln(10)));
    y2:=exp(ln(10)*round(ln(y1)/ln(10)));
    if x1/x2<0.35 then stepx:=0.2
    else if x1/x2<0.75 then stepx:=0.5
         else if x1/x2<1.5 then stepx:=1
              else stepx:=2;
    if y1/y2<0.35 then stepy:=0.2
    else if y1/y2<0.75 then stepy:=0.5
         else if y1/y2<1.5 then stepy:=1
              else stepy:=2;
    stepx:=stepx*x2;stepy:=stepy*y2;
    if dim.xma-dim.xmi<0 then stepx:=-stepx;
    if dim.yma-dim.ymi<0 then stepy:=-stepy;
    kx:=2;ky:=2;
    if round(dim.xmi/stepx)>(dim.xmi/stepx)
                        then rea1[1,kx]:=round(dim.xmi/stepx)*stepx
    else rea1[1,kx]:=(round(dim.xmi/stepx)+1)*stepx;
    if round(dim.ymi/stepy)>(dim.ymi/stepy)
                        then rea1[2,ky]:=round(dim.ymi/stepy)*stepy
    else rea1[2,ky]:=(round(dim.ymi/stepy)+1)*stepy;
  while rea1[1,kx]<=dim.xma do begin inc(kx);rea1[1,kx]:=rea1[1,kx-1]+stepx;end;
  while rea1[2,ky]<=dim.yma do begin inc(ky);rea1[2,ky]:=rea1[2,ky-1]+stepy;end;
    rea1[1,kx]:=dim.xma;rea1[2,ky]:=dim.yma;
    rea1[1,1]:=dim.xmi; rea1[2,1]:=dim.ymi;
    if ky>kx then begin
      k:=ky;for k1:=kx to ky do rea1[1,k1]:=dim.xma;end
    else begin k:=kx;for k1:=ky to kx do rea1[2,k1]:=dim.yma;end;
    for k1:=1 to k do begin rea2[1,k1]:=rea1[1,k1];rea2[2,k1]:=rea1[2,k1];end;
    changecoord(mfit,coef,rea1,k,true);
    if mfit.xma>mfit.xmi then ax:=true else ax:=false;
    if mfit.yma>mfit.ymi then ay:=true else ay:=false;
    for k:=2 to kx-1 do begin
      y:=mfit.ymi;
      moveto(round(rea1[1,k]),round(y));
      lineto(round(rea1[1,k]),round(y+5));
      if drawx then begin
        while ((y>mfit.yma) xor ay) do begin
          putpixel(round(rea1[1,k]),round(y),white);
          if ay then y:=y+10 else y:=y-10;
        end;
      end;
      setcolor(getbkcolor);setwritemode(normalput);
      outtextxy(round(rea1[1,k]),round(mfit.ymi+10),#219+#219+#219+#219+#219+#219+#219+#219+#219+#219);
      setwritemode(putstyle);setcolor(colordraw);
      outtextxy(round(rea1[1,k]),round(mfit.ymi+10),myform(rea2[1,k],5));
    end;
    for k:=2 to ky-1 do begin
      x:=mfit.xmi;
      moveto(round(x),round(rea1[2,k]));
      lineto(round(x-5),round(rea1[2,k]));
      if drawy then begin
        while ((x>mfit.xma) xor ax) do begin
          putpixel(round(x),round(rea1[2,k]),white);
          if ax then x:=x+5 else x:=x-5;
        end;
      end;
      setcolor(getbkcolor);setwritemode(normalput);
      outtextxy(round(mfit.xmi-40),round(rea1[2,k]),#219+#219+#219+#219+#219+#219+#219+#219+#219+#219);
      setwritemode(putstyle);setcolor(colordraw);
      outtextxy(round(mfit.xmi-40),round(rea1[2,k]),myform(rea2[2,k],5));
    end;
    setmyLineStyle(LS);
  End;
*/
