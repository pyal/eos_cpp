#define MaxRows 512

#include <iostream.h>
#include <string.h>
#include <strstream.h>
#include "util.h"

void GetRows(char *name,int argc,char *argv[],int *RowRead,int *RowWrite)
  {
   fstream inp=fstream(name,ios::in);
   int len;
   double ar[MaxRows];
   while ( (!seek_eof(inp)) && (!(len=EnterStr(inp,&ar[0],MaxRows))) );
   if (len<1) len=1;
   RowRead[0]=len-1;RowWrite[0]=len-1;
   for (int k=2;k<=len;k++)
     {
      RowRead[k-1]=k;
      RowWrite[k-1]=k+len-1;
     }
   inp.close();
  };

void PutBuf(fstream &in,char *BStr,char *buf,int len)
  {

   char tmp[256];
   int  ch;
   strstream buf_stream(buf,len,ios::out);
   strcpy(tmp,"");
   while ( (Stricmp(tmp,BStr)!=0) && (!in.eof()) )
     {
      buf_stream<<tmp;
      while (( (ch=in.get())==' ' ) || ( ch=='\n' )) buf_stream.put(ch);
      in.putback(ch);
//      int pos=0;
//      while (( (ch=in.get())!=' ' ) && ( ch!='\n' ))
//        { tmp[pos++]=ch;} tmp[pos]=0;
      in>>tmp;
     }
   buf_stream.put(0);
//cout<<" Buf \n"<<buf;
   buf=buf_stream.str();
  }
int ControlPar( int argc, char *argv[] )
  {
   if ((argc<3) || (GetCmd("/h",argc,argv)!=NULL))
     {
      cout<<"usage:\n"
          <<argv[0]<<"  method_to_extend    data_file_to_work_with\n"<<
           "      Extends method to all columns.  \n"<<
           "      Writes the result file with the same name \n"<<
           "      and [cfg]|[cf1] extension\n"; 
      return 0;
     }
   return 1;
  }; 
void main( int argc, char *argv[] )
  {
   if (!ControlPar(argc,argv)) exit(1);
   char tmp[128],name[128];
   fstream in=fstream(argv[1],ios::in);
   SetExt(argv[2],name,"cfg");
   if (Stricmp(name,argv[1])==0) SetExt(argv[2],name,"cf1");
   fstream out=fstream(name,ios::out);

   while ( (Stricmp(tmp,"GeneralBegin")!=0) && (!seek_eof(in)) )
     { in>>tmp;out<<tmp<<"   ";if (seek_eoln(in)) out<<"\n";}
   in>>tmp>>tmp;
   out<<argv[2]<<"     "<<tmp;//<<"\n";

   int RowRead[MaxRows],RowWrite[MaxRows];
   GetRows(argv[2],argc,argv,&RowRead[0],&RowWrite[0]);

   char *buf=new char[10000];
   PutBuf(in,"SetBegin",&buf[0],10000);
   out<<buf;if (in.eof()) exit(1);
   while ( !(seek_eof(in)) )
     {
/*
      if (seek_eoln(in)) out<<"\n";in>>tmp;
      while ( (Stricmp(tmp,"SetBegin")!=0) && (!seek_eof(in)) )
        { out<<tmp<<"   ";if (seek_eoln(in)) out<<"\n";in>>tmp;}
      if (seek_eof(in)) break;
*/
//                Analize SetBegin String
//cout<< "Main Circle\n ";
      int k;
      in>>k>>k;
      PutBuf(in,"SetBegin",&buf[0],10000);
      for ( k=1;k<=RowRead[0];k++)
        { out<<"SetBegin      "<<RowRead[k]<<"   "<<RowWrite[k]<<" "<<buf<<".\n";}
     }
   delete buf;out.close();in.close();
  };
