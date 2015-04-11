#include "util.h"

#ifndef LINUX
#include "process.h"
#endif
#ifndef WCPP
#include <strstream.h>
#else
#include <strstrea.h>
#endif
#include <stdlib.h>
#define MaxCommand 1520

//typedef (char *)[MaxCommand] charptr;
char *BegStr[MaxCommand];

int LongNumberFormat=2;

struct CommandType
  {
   int NumCommand;
   int FoundCommand;
   char *name[MaxCommand];
   char *found[MaxCommand];
  };
char *SpecSum(int argc,char *argv[],int *EnterStr,
                       int BegPos)
  {
   char *tmp,*cur,*tmp1,*bufstr=new char[256];cur=bufstr;
   char num[5];num[LongNumberFormat-1]=0;
   strstream out(bufstr,256,ios::out);
	 int k;
   for (k=BegPos+1;k<=argc;k++) out<<argv[k]<<" ";
   out<<'\0';
//cout<<bufstr<<"\n";

   k=0;
#ifdef LINUX
   while ((tmp=strchr(cur,'\#'))!=NULL)
#else
   while ((tmp=strchr(cur,'#'))!=NULL)
#endif
     {
	  int Add=0;num[1]=0;  // Added
#ifdef LINUX
	  if (tmp[1]=='\#')
#else
	  if (tmp[1]=='#')
#endif
		{ for (int o=0;o<LongNumberFormat;o++) num[o]=tmp[2+o];Add=LongNumberFormat;} 
   else  // Added
      num[0]=tmp[1];
   k++;BegStr[k]=cur;cur[tmp-cur]=0;
//cout<<cur<<" "<<&tmp[1]<<"\n";
      if ((EnterStr[k]=atoi(num))==0)
         {cout<<"Cannot find number "<<num<<"\n";exit(1);}
      cur=&tmp[2+Add];

/*
      if (tmp1=strchr(tmp,' '))
        {k++;BegStr[k]=cur;cur[tmp-cur]=0;tmp[tmp1-tmp]=0;
//cout<<cur<<" "<<&tmp[1]<<"\n";
         if ((EnterStr[k]=atoi(&tmp[1]))==0)
           {cout<<"Cannot find number "<<&tmp[1]<<"\n";exit(1);}
        }
      else {cout<<"Cannot find number "<<&tmp[1]<<"\n";exit(1);}
//cout<<BegStr[k]<<" "<<EnterStr[k]<<"\n";
      cur=&tmp1[1];
*/
     }
   BegStr[k+1]=cur;
//cout<<BegStr[k+1]<<"\n";
   EnterStr[0]=k;
   return bufstr;
  };
void ExtractCommand(int argc,char *argv[],CommandType &Work,int &BegPos)
  {
   int Found;
   Work.FoundCommand=0;
   for (int k=1;k<=argc;k++)
     {
      Found=0;
//     if (strncmp(argv[k],"\\" , len) == 0) break;
      for (int k1=1;k1<=Work.NumCommand;k1++)
        {
         int len = strlen(Work.name[k1]);
//         Work.found[k1]=NULL;
         if (strncmp(argv[k], Work.name[k1], len) == 0)
           { Work.found[k1] = argv[k]+len;Work.FoundCommand++;Found=1;}
        }
     if (!Found) break;
     }
   BegPos=Work.FoundCommand+1;
  };

int main(int argc,char *argv[])
  {
   argc--;
   CommandType Work={4,0,NULL,"/n","/h","/d","/l",NULL,NULL,NULL,NULL,NULL
                              ,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL
                              ,NULL,NULL,
                          NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL
                              ,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL
                              ,NULL,NULL,NULL};
   int BegPos=1,EnterStr[MaxCommand],NumStrToRead=0,k;
   int Delimiter=0;
   ExtractCommand(argc,argv,Work,BegPos);
//cout<<Work.found[1]<<"\n";
   if ((Work.found[2]!=0) || (argc==0))
     {
      cout<<"usage: "<<argv[0]<<"\n"
          <<" /key file_to_read_from string_to_do(#i - i-th param from file( or ##34 )\n"
          <<"  key : h - this help\n"
          <<"        d - delimiter in the input file to use\n"
          <<"        n - number param to read from file in one iteration\n"
          <<"        if param=0  - read by line                         \n"
          <<"        if it is negative read all                         \n"
          <<"        l - LongNumberLength=2   by def                    \n";
      exit (0);
     }
   k=1;while ((k<=Work.NumCommand) && (Stricmp(Work.name[k],"/n")!=0) ) k++;
   if ((k<=Work.NumCommand) && (Work.found[k]!=NULL)) NumStrToRead=atoi(Work.found[k]);
   k=1;while ((k<=Work.NumCommand) && (Stricmp(Work.name[k],"/d")!=0) ) k++;
   if ((k<=Work.NumCommand) && (Work.found[k]!=NULL)) Delimiter=Work.found[k][0];
   k=1;while ((k<=Work.NumCommand) && (Stricmp(Work.name[k],"/l")!=0) ) k++;
   if ((k<=Work.NumCommand) && (Work.found[k]!=NULL)) LongNumberFormat=atoi(Work.found[k]);
//   NumStrToRead=atoi(Work.found[1]);
//   if (NumStrToRead==0) NumStrToRead=1;
   int Seek_eoln=0,Seek_eof=0;
   if (NumStrToRead==0) Seek_eoln=1;
   if (NumStrToRead<0) Seek_eof=1;

   char *buf=SpecSum(argc,argv,EnterStr,BegPos);
//   NumStrToRead=1;//EnterStr[0];
   fstream in(argv[BegPos],ios::in);
   char bufstr[256],inbuf[MaxCommand][128],*tmp[MaxCommand];
   char newbuf1[256],newbuf2[256];
//   strstream out(bufstr,256,ios::out);
#ifdef WCPP
   if (Delimiter!=0) {_pctype[32]=16;_pctype[Delimiter]=72;}
#else
#ifdef MSDOS
   if (Delimiter!=0) {_ctype[32]=16;_ctype[Delimiter]=72;}
#else
#ifdef LINUX
   if (Delimiter!=0);// {_ctype[32]=16;_ctype[Delimiter]=72;}
#endif
#endif
#endif

   while (!seek_eof(in))
     {
      strstream out(bufstr,256,ios::out);
//      out.init(bufstr[0],256);
      if (Seek_eoln)
        {NumStrToRead=0;k=1;

         while (!seek_eoln(in))
           {
			 if (NumStrToRead==MaxCommand) {k--;NumStrToRead--;}
		     {in>>inbuf[k++];NumStrToRead++;}
			}
		}
      if (Seek_eof)
        {NumStrToRead=0;k=1;
         while (!seek_eof(in))
           {if (NumStrToRead==MaxCommand) {k--;NumStrToRead--;}
            {in>>inbuf[k++];NumStrToRead++;}
			}
        }
      if ((!Seek_eof) && (!Seek_eoln))
         {for (k=1;k<=NumStrToRead;k++)
            in>>inbuf[k];
		}
      for (k=1;k<=EnterStr[0];k++) out<<BegStr[k]<<inbuf[EnterStr[k]];
      if (Stricmp(inbuf[1],"//")!=0)
        {
//         cout<<"\\\\"<<"  !!!!!!\n";
         out<<BegStr[EnterStr[0]+1]<<'\0';
/*
cout<<bufstr<<"\n";
      cur=strchr(bufstr,' ');k=1;
      do
        {
         k++;tmp[k]=&cur[1];bufstr[cur-bufstr]=0;
cout<<tmp[k]<<"  "<<k<<"  ";
        }
      while (cur=strchr(&cur[1],' '));
      tmp[k]=NULL;
      tmp[0]=bufstr;
cout<<bufstr<<"\n";
cout<<tmp[0]<<"\n";
      strcpy(newbuf1,"command");
      strcpy(newbuf2,"/c");strcat(newbuf2,tmp[0]);tmp[0]=newbuf1;tmp[1]=newbuf2;
      k=spawnvp(P_WAIT,tmp[0],tmp);
cout<<k<<"\n";
//      perror("exec error");
//      execvp("help.com", argv);
//      perror("exec error");
*/
         system(bufstr);
        }
//cout<<_doserrno<<"\n";
     }
   delete buf;
   return 0;
  };
