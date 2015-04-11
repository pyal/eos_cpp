//uses util;
#ifndef __MEMF_H
#define __MEMF_H

#include <fstream.h>
#include <strstream.h>
#include <string.h>
#include <mem.h>
#include <conio.h>
#include <stdio.h>
#include <alloc.h>
#include <dir.h>
#include <bios.h>
#include "util.h"
#define MAXMESS 60
static int maxbuf=512;
static int draw_ask=0;
static char CurPath[MAXDRIVE+MAXDIR]="";

//  ======================== MESSAGE =============================

struct message
  {
   char *buf;
   message(){buf=NULL;};
   message(const char* p)
     {buf=strdup(p);
      printf("%s\n",buf);
     };
   virtual void mess_draw(const char* mes=NULL)
     { if (mes!=NULL) { delete buf;buf=strdup(mes);}
      printf("%s\n",buf);
     };
   void mess_hide(){ };
  ~message() { delete buf; };
  };

//  ======================== USER_ASK ============================

struct user_ask:message
  {
   enum ask_mode{INF,YN,NY,ER};
   enum ask_draw { draw,virt };
   enum answer{Yes,No,Abort};
   ask_draw dr;

   user_ask(){ dr=(ask_draw)draw_ask;};
   user_ask(ask_draw draw){ dr=draw;};

   answer ask(const char* p,ask_mode ask=INF);

   virtual void mess_draw(const char* mes=NULL)
     { if (dr==draw) message::mess_draw(mes);};

   int in_ar(int *ptr,int num,int val,int &nval);
  };

//  ========================  FNAME   ============================

struct fname:user_ask
  {
   char *buf;
   fname( ){ buf=new char[MAXDRIVE+MAXDIR+MAXFILE+MAXEXT];};
   fname(ask_draw draw ):user_ask(draw)
       { buf=new char[MAXDRIVE+MAXDIR+MAXFILE+MAXEXT];};
   char *path(const char *p);
   char *name(const char *n);
   char *jname(const char *jn);
   char *ext(const char *e);
   char *sum(const char *path,const char* name);
   char *sum(const char *path,char* jname,char* ext);
   char *askname(const char *name=NULL,int next=1,
                 const char *mess=NULL,
                 const char *path=NULL);
   ~fname(){delete buf;};
  };

//  ========================  MEMF    ============================

struct memf
  {
   enum work_with {zero,file,mem};
   work_with work;
   iostream *mfbuf;
   memf(){ work=zero;};
   memf(const char *cname,int mode,const char *mess=NULL,int read_name=1);
   memf(char *buf,int n,int mode)
     {
      mfbuf=(iostream*)new strstream(buf,n,mode);
      work=mem;
     };
    ~memf() { if (work==mem)
             delete ((strstream*)mfbuf);
        else delete (  fstream*)mfbuf;};
  };


#endif













































//
