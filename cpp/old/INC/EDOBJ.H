#include grlib.h

#define BS 5592
#define FORMFEED 12
#define CR 7181
#define ESC 283
#define HOMEKEY 18176
#define ENDKEY 20224
#define UPKEY 18432
#define DOWNKEY 20480
#define PGUPKEY 18688
#define PGDNKEY 20736
#define LEFTKEY 19200
#define INSKEY 20992
#define RIGHTKEY 19712
#define DELKEY 21248
#define CTRLLEFTKEY 29440
#define CTRLRIGHTKEY 29696
#define F1 15104
#define F2 15360
#define F3 15616
#define F4 15872
#define F5 16128
#define F6 16384
#define F7 16640
#define F8 16896
#define F9 17152
#define F10 17408

struct Position { int rt,lt;}
struct Coordinate { Position dn,up;}
struct edel:Coordinate
  {
   int Active;
   edel(){Active=0;} 
   virtual void Draw()=0;
   virtual void Hide(){Active=0;}
   virtual void SetActive(int act=1){Active=act;Draw();}
   virtual int  HandEv(int event)=0;
   virtual ~edel();
  }
struct edpage:edel,LstCl  {}
struct edrun:edel{ virtual int run()=0;}

struct grstr:edel,ScrText
  {
   int ActCol;
   char *txt;
   grstr(char *buf,int pos_x, int pos_y,int leng)
     {
      
      TextStrc Meth;
      Meth.Font=DEFAULT_FONT;Meth.Direc=HORIZ_DIR;Meth.Just=CENTER_TEXT;
      ScrText(buf,pos_x,pos_y,Meth);
      ActCol=RED_COLOR;
     }
