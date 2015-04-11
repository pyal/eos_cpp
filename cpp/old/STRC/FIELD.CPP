// uses tct_t2s,nw_t2s,wind;
#include "field.h"

data_item::data_item(int xed,int yed,int led,int aed,int ahed,
                     int xh ,int yh ,int lh ,int ah ,
                     void *p,const char *h,int n,int t,edit_item e)
  {
   ed_loc.row=yed;ed_loc.col=xed;ed_dim.h=1;ed_dim.w=led;ed_atr=aed;
   ed_hatr=ahed;
   h_loc.row=yh;h_loc.col=xh;h_dim.h=1;h_dim.w=lh;h_atr=ah;
   ptr=p;num=n;typ=t;ed=e;
   help=new char[h_dim.w+1];
   stpjust(help,h,' ',h_dim.w,JUST_LEFT);
   ed.in_par=ed.s((data_to_item)(*this),ptr);
  };

void grp_item::draw()
  {
//   printf("Free %u beg draw\n",coreleft());
   wind_draw();
   for (int k=1;k<=num_it;k++) it[k]->draw();
//   printf("Free %u end draw\n",coreleft());
  };
int grp_item::set_it(int curr_it,ED_ACTION action)
  {
   if ( (action==ED_UP) || (action==ED_RIGHT) ) curr_it++;
   if ( (action==ED_DOWN) || (action==ED_LEFT) ) curr_it--;
   curr_it=in_border(curr_it,1,num_it);
   return curr_it;
  };

ED_ACTION grp_item::edit(KEY_SEQUENCE &key,int &choice)
  {
   printf("Free %u beg edit                   QQQ\n",coreleft());
   draw();
   ED_ACTION action=ED_NULL;
   key.character_code=0;key.key_code=0;choice=0;
   if (num_it>0)
    {
     curr_it=in_border(curr_it,1,num_it);

     do
       {
        action=it[curr_it]->edit(key);
        curr_it=set_it(curr_it,action);
       }
     while ( (action!=ED_NULL) &&  (action!=ED_TRANSMIT) && (action!=ED_ABORT) );
     choice=it[curr_it]->num;
    }
   printf("Free %u end edit                   QQQ\n",coreleft());
   getch();getch();
   return action;
  };
grp_item::~grp_item()
  {
//   wind_delete();
   printf("Free %u beg destructing\n",coreleft());
   for (int k=1;k<=num_it;k++)
     {delete it[k] ;}
// ( (wind_blk)(*this) ).~wind_blk();
   printf("Free %u end destructing            QQQ\n",coreleft());
  };





void main()
  {
   clrscr();
   printf("Free %u beg \n",coreleft());
   int i1=100;
   int i2=100;
   edit_item eint;
   eint.s=&store_int;
   eint.d=&dr_int;
   eint.f=&pre_int;
   eint.m=&main_int;
   eint.l=&post_int;
   eint.del=&del_int;

   grp_item *grp=new grp_item(50,10,SC_BLUE,SC_RED,SC_WHITE,SC_GREEN);

//   printf("Free %u after new grp \n",coreleft());
//   delete grp;
//   printf("Free %u after delete grp \n",coreleft());

//   grp=new grp_item(50,10,SC_BLUE,SC_RED,SC_WHITE,SC_GREEN);
//   printf("Free %u beg \n",coreleft());
//   grp->add_item(20,1,10, 1,1,10,&i1,"integer 1",1,-1,eint);
//   printf("Free %u first new\n",coreleft());
//   grp->add_item(20,2,10, 1,2,10,&i2,"integer 2",2,-1,eint);
//   printf("Free %u second new\n",coreleft());
//   delete grp;
//   printf("Free %u after delete grp \n",coreleft());

//   grp=new grp_item(50,10,SC_BLUE,SC_RED,SC_WHITE,SC_GREEN);

   printf("Free %u beg \n",coreleft());
   grp->add_item(20,1,10, 1,1,10,&i1,"integer 1",1,-1,eint);
   printf("Free %u first new\n",coreleft());
   grp->add_item(20,2,10, 1,2,10,&i2,"integer 2",2,-1,eint);
   printf("Free %u second new before edit            EEEE\n",coreleft());

   KEY_SEQUENCE key;
   int choice;
   grp->edit(key,choice);
   printf("Free %u after edit1              EEEE1\n",coreleft());
   grp->edit(key,choice);
   printf("Free %u after edit2              EEEE2\n",coreleft());
   delete grp;
   printf("Free %u delete grp                     zZZZZ\n",coreleft());
  };























/*
void      pre_int(data_item data)
  {
   dr_int(data,0); 
  };
ED_ACTION get_action(KEY_SEQUENCE key)
  {
   ED_KEY *ed=edretkey(&key);
   return *(ed->edit_actions.pactions);
  }    
ED_ACTION main_int (data_to_item data,KEY_SEQUENCE &key)
  {
   ED_CONTROL ctrl;
   ctrl.ul_corner=data.ed_loc; 
   ctrl.dimensions=data.ed_dim; 
   ctrl.attribute=data.ed_atr; 
   ctrl.replace_cursor.high=13; 
   ctrl.replace_cursor.low=12; 
   ctrl.insert_cursor.high=13; 
   ctrl.insert_cursor.low=0; 
   ctrl.control_function=NULL; 
   ctrl.control_flags=ED_AUTOSKIP|ED_LEFTJUST;

   char *buf=new char[data.ed_dim.w+1];
   char *ret=new char[data.ed_dim.w+1];
   wnrdbuf(data.ed_loc.row,data.ed_loc.col,data.ed_dim.w,buf,0);  
   edfield( buf,ret,data.ed_dim.w,&ctrl,&key);

   delete buf;  delete ret;
   return get_action(key);
  };

void      post_int (data_to_item data,ED_ACTION action)
  {
   if (action!=ED_ABORT)
     {   
      int *a=(int*)data.ptr;
      char *buf=new char[data.ed_dim.w+1];
      wnrdbuf(data.ed_loc.row,data.ed_loc.col,data.ed_dim.w,buf,0);  
      *a=atoi(buf);
      delete buf;
     }
  };
    
void *    store_int (data_to_item data,void *ptr){return NULL;};
void      del_int (data_to_item data,void *ptr){};
void      dr_int(data_to_item data,int high=-1)
  {
   char buf [256];
   int *a=(int*)data.ptr;
   itoa(*a,buf,10);
   stpjust(buf,buf,' ',data.ed_dim.w,JUST_LEFT);

   wncurmov(data.h_loc.row,data.h_loc.col);
   wnwrstr(data.help,data.h_atr,data.h_atr);

   int atr;
   if (high==-1) atr=data.ed_atr;else atr=data.ed_hatr;
   wncurmov(data.ed_loc.row,data.ed_loc.col);
   wnwrstr((char*)&buf,atr,atr);
  };










  */
