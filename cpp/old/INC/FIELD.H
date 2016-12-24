
#define MAX_ITEM 256
#include <bwindow.h>
#include <bkeybrd.h>
#include <bedit.h>
#include <bkeys.h>
#include <string.h>
#include <bstrings.h>
#include <mem.h>
#include "wind.h"
#include <conio.h>
#include <alloc.h>
#include <stdio.h>
#define NIL NULL
int in_border(int crr_n,int fst_n,int lst_n)
  { 
    if (crr_n<fst_n) crr_n=fst_n;
    if (crr_n>lst_n) crr_n=lst_n;
    return crr_n;
  };

struct data_to_item
  {  
   int typ;
   int num;
   LOC ed_loc,h_loc;
   DIM ed_dim,h_dim;
   int ed_atr,h_atr,ed_hatr;
   char *help;
   void *ptr;
  };
typedef void      (*DRAW_FUNC) (data_to_item data,int high=-1);
typedef void      (*PRE_ED   ) (data_to_item data);
typedef ED_ACTION (*MAIN_ED  ) (data_to_item data,KEY_SEQUENCE &key);
typedef void      (*POST_ED  ) (data_to_item data,ED_ACTION action);
typedef void *    (*STORE_IN ) (data_to_item data,void *ptr);
typedef void      (*DEL_IN   ) (data_to_item data,void *ptr);

struct edit_item
  {
   void *in_par;
   STORE_IN s;
   DRAW_FUNC d;
   PRE_ED f;
   MAIN_ED m;
   POST_ED l;
   DEL_IN del;
  };


struct data_item:data_to_item
  {
   edit_item ed;
   data_item(int xed,int yed,int led,int aed,int ahed,
             int xh ,int yh ,int lh ,int ah ,
             void *p,const char *h,int n,int t,edit_item e);
   void draw(){(*ed.d)(*this);};
   ED_ACTION edit(KEY_SEQUENCE &key)
     {
      ED_ACTION action=ED_NULL;
      (*ed.f)(*this);
      action=(*ed.m)(*this,key);
      (*ed.l)(*this,action);
      return action;
     };
   ~data_item()
     {
       (*ed.del)(*this,ed.in_par);
       delete help;
     };
  };

struct grp_item:wind_blk
  {
   int num_it,curr_it;
   int blk_ed_attr,blk_ed_hattr,blk_h_attr;
   data_item *it[MAX_ITEM];

   grp_item(int xs,int ys,int attrib,int ed_attr,int ed_hattr,int h_attr):
     wind_blk((int)wind_blk::real,xs,ys,attrib)
      {num_it=0;blk_ed_attr=ed_attr;blk_ed_hattr=ed_hattr;blk_h_attr=h_attr;};

   void add_item(int xed,int yed,int led,int xh ,int yh ,int lh ,
                            void *p,const char *h,int n,int t,edit_item e)
     {
      if (MAX_ITEM>num_it) {num_it++;it[num_it]=new data_item
       (xed,yed,led,blk_ed_attr,blk_ed_hattr,xh,yh,lh,blk_h_attr,p,h,n,t,e);}
     };
   void draw();
   ED_ACTION edit(KEY_SEQUENCE &key,int &choice);
   ~grp_item();


   int set_it(int curr_it,ED_ACTION action);  // internal calc where to go
  };


void *    store_int (data_to_item data,void *ptr){return NULL;};
void      del_int (data_to_item data,void *ptr){};

//void      dr_int(data_to_item data,int high=-1){};
//void      pre_int(data_to_item data){};
//ED_ACTION main_int (data_to_item data,KEY_SEQUENCE &key)
//    {ED_ACTION ed=ED_ABORT;return ed;};
//void      post_int (data_to_item data,ED_ACTION action){};

void      dr_int(data_to_item data,int high=-1)
  {
   char buf [256];
   int *a=(int*)data.ptr;
   Itoa(*a,buf,10);
   stpjust(buf,buf,' ',data.ed_dim.w,JUST_LEFT);

   wncurmov(data.h_loc.row,data.h_loc.col);
   wnwrstr(data.help,data.h_atr,data.h_atr);

   int atr;
   if (high==-1) atr=data.ed_atr;else atr=data.ed_hatr;
   wncurmov(data.ed_loc.row,data.ed_loc.col);
   wnwrstr((char*)&buf,atr,atr);
  };
void      pre_int(data_to_item data)
  {
   dr_int(data,0);
  };
void      post_int (data_to_item data,ED_ACTION action)
  {

   if (action!=ED_ABORT)

     {
      int *a=(int*)data.ptr;
      char *buf=new char[data.ed_dim.w+1];buf[data.ed_dim.w]=0;
      wnrdbuf(data.ed_loc.row,data.ed_loc.col,data.ed_dim.w,buf,0);
      *a=atoi(buf);
      delete buf;
     }
  };

void null_validation(KB_DATA *pkey_data){};

ED_ACTION main_int (data_to_item data,KEY_SEQUENCE &key)
  {
   printf("Free %u beg main_int   \n",coreleft());
   ED_CONTROL ctrl;
   ctrl.ul_corner=data.ed_loc;
   ctrl.dimensions=data.ed_dim;
   ctrl.attribute=data.ed_atr;
   ctrl.replace_cursor.high=13;
   ctrl.replace_cursor.low=12;
   ctrl.insert_cursor.high=13;
   ctrl.insert_cursor.low=0;
   ctrl.control_function=null_validation;
   ctrl.control_flags=ED_AUTOSKIP|ED_LEFTJUST;

   char *buf=new char[data.ed_dim.w+1];buf[data.ed_dim.w]=0;
   char *ret=new char[data.ed_dim.w+1];
   wnrdbuf(data.ed_loc.row,data.ed_loc.col,data.ed_dim.w,buf,0);
   int ercode=wnfield( b_pcurwin,buf,ret,data.ed_dim.w+1,&ctrl,&key);
//   if (ercode!=WN_NO_ERROR) printf("Error %d editing window",ercode);

   delete buf;  delete ret;
   printf("Free %u end main_int   \n",coreleft());

//   return get_action(key);
   
   ED_ACTION ed=ED_ABORT;return ed;
  };











/*
struct wind_blk {
  enum wind_mode {real,virt};
  int mode;
  int xl,yl,attr;
  BWINDOW *bwind;
  BORDER bord;
  WHERE where;
  wind_blk(int cmode,int xs=-1,int ys=-1,int attrib=-1);
  wind_blk();
  void wind_mode(int cmode=-1,int xs=-1,int ys=-1,int attrib=-1);
  void wind_bord(){};
  void wind_where(){};
  void wind_new_zer(){};

  void wind_init();
  void wind_draw();
  void wind_delete();
  void wind_hide();
  void Err(int Fun,int Num=-1);
   ~wind_blk();
  };







ED_ACTION get_action(KEY_SEQUENCE key)
  {
//   printf("\nFree %u beg get_action \n",coreleft());
   ED_KEY *ed=edretkey(&key);
//   ED_ACTION ret=*(ed->edit_actions.pactions);
//   delete ed;
//   return ret;
//   printf("Free %u end get_action \n",coreleft());
   return *(ed->edit_actions.pactions);
  }



*/

