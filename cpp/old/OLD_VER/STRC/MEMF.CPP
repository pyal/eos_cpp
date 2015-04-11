#include "memf.h"

//  ======================== USER_ASK ============================


int user_ask::in_ar(int *ptr,int num,int val,int &nval)
   {
    int ret=0;
    for (int k=0;k<=num;k++) if ( ptr[k]==val ) { nval=k;ret=1;break;}
    return ret;
   };
user_ask::answer user_ask::ask(const char* p,ask_mode ask)
   {
    int n;  //"y"   "Y"  "n"    "N"   " "  "ent""esc"
    int ar[7]={5497,5465,12654,12622,7181,14624,283};
    const char *add_mess[4]=
         {""," [y] ? ","[n] ? ",""};
    answer ans=Yes;

    char s[256];strstream st((char *)s,256,ios::in);char ch=0;
    st<<p<<add_mess[(int)ask]<<ch;
    if (dr==draw)
      {
        mess_draw(s);
        if ( (ask==YN) || (ask==NY) )
          {
           do ;
           while ( !in_ar(&ar[0],6,ChRead(),n) ) ;
           switch (n) {
             case 0   :
             case 1   :ans=Yes;break;
             case 2   :
             case 3   :ans=No ;break;
             case 4   :
             case 5   :if (ask==YN) ans=Yes;else ans=No;break;
             case 6   :ans=Abort;break;
                    }
           }
        else ChRead();
        mess_hide();
       }
     return ans;
   };


//  ========================  FNAME   ============================

char *fname::path(const char *p)
  {
   char drive[MAXDRIVE];
   char dir  [MAXDIR];
   char file [MAXFILE];
   char ext  [MAXEXT];
   fnsplit (p,drive,dir,file,ext);
   strcpy(buf,drive);
   strcat(buf,dir);
   return buf;
  };
char *fname::name(const char *n)
  {
   char drive[MAXDRIVE];
   char dir  [MAXDIR];
   char file [MAXFILE];
   char ext  [MAXEXT];
   fnsplit (n,drive,dir,file,ext);
   strcpy(buf,file);
   strcat(buf,ext);
   return buf;
  };
char *fname::jname(const char *jn)
  {
   char drive[MAXDRIVE];
   char dir  [MAXDIR];
   char ext  [MAXEXT];
   fnsplit (jn,drive,dir,buf,ext);
   return buf;
  };
char *fname::ext(const char *e)
  {
   char drive[MAXDRIVE];
   char dir  [MAXDIR];
   char file [MAXFILE];
   fnsplit (e,drive,dir,file,buf);
   return buf;
  };
char *fname::sum(const char *path,const char* name)
  {
   strcpy(buf,path);
   strcat(buf,name);
   return buf;
  };
char *fname::askname(const char *name,int next,const char *mess,
                                                      const char *path)
  {
   if (path==NULL) path=(const char*)CurPath;
   if (name==NULL) name="";
   sum(path,name);
   char *ret=buf;
   if (!( (dr==virt) || ( (name!="") && (!next) ) ))
     {
       answer ans=Yes;
       if (mess!=NULL) mess_draw(mess);
       char s[256];strstream st(s,256,ios::in);char ch=0;
       st<<"Current name : "<<buf<<". Enter new one "<<ch;
       ans=ask((const char*)&s,YN);

       if (ans==Yes) { printf("Enter new name:");gets(buf);}
                                     // To be changed !!!!!!!!!!!!!!!!!!!!!
       if (ans==Abort) ret=NULL;
     }
   return ret;
  };

//  ========================  MEMF    ============================


memf::memf(const char *cname,int mode,const char *mess,int read_name)
  {
   if ( mode&ios::out ) mode=(mode|ios::noreplace );
   beg:
   {
    fname n;
    mfbuf=(iostream*)new fstream(n.askname(cname,read_name,mess),mode);
   }
   if (!(*mfbuf) )
     {
      delete mfbuf;
      if ( ( mode&ios::nocreate ) || (mode&ios::in) )
	{
	 user_ask ask;
	 if (user_ask::Yes==
	  ask.ask("File does not exist.Another try ",user_ask::YN)) goto beg;
	}
      else
	if ( mode&ios::noreplace )
	  {user_ask ask;
	   user_ask::answer ans=ask.ask(
	       "File with such name already exist.Ovewrite ? ",user_ask::NY);
	   if (ans==user_ask::Yes)
	     { mode=(mode^ios::noreplace);read_name=0;goto beg; }
	   if (ans==user_ask::No) goto beg;
	  }
      }
   work=file;
  };


/*

void main()
 {
  char buf[256];
  cin>>buf;
  memf f(buf,"Input file",ios::in);
  if (*(f.mfbuf))
   {
    f.mfbuf->get(buf,256);
    cout<<(char*)&buf<<"out";
   }
 }




*/
