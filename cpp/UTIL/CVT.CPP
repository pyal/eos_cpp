

#include "cvt.h"

#include <dos.h>
#include <malloc.h>

//#define MaxArrPnt      1500

//#define MaxCodeNumber 100
//#define MaxFoundCode 1000
//#define MaxCodeLength 30
//#define MaxInputBuf  1000

extern unsigned _stklen=60000;
double StndErr=1e-6;


//void find_code() // procedure which finds all codes (different) in the input
//void order_code() //procedure which set all codes in oder deleting extra ones
//void code_page() // procedure which codes page by table
//void code_subst()// procedure which substitutes combinations, saves output
                 //  to the fstream.
//void unlimited_change()

code_struct::~code_struct()
  { for (int k=1;k<=NumCodes;k++) delete code[k];}

void delete_code(char *buf,code_struct &code,code_config *Res,int &buf_len,
                 int &Sub_buf)
  {
   int Sub=0,k=0,tmp,tmp1;
   int cur_page=0;
   Sub_buf=0;
   while ( (k<=buf_len) && (cur_page<Res[0].position) )
     {
      cur_page++;
      Res[cur_page].position-=Sub;
      k=Res[cur_page].position;
      if (k>buf_len) return;
      int Add=code.Buf_Len[Res[cur_page].code_n];
//cout<<" delete code   Add "<<Add<<"\n";
      buf_len-=Add;
      Sub+=Add;
      tmp =buf_len;
      if (cur_page+1<Res[0].position)
          if ((tmp1=Res[cur_page+1].position-Sub)<buf_len) tmp=tmp1;
      MemoryMove(&buf[k+Add],&buf[k],sizeof(char)*(tmp-k));
//      movmem(&buf[k+Add],&buf[k],sizeof(char)*(buf_len-k-Add));
//      buf_len-=Add;
//      Sub+=Add;
     }
   if (k>buf_len) Sub_buf=k-buf_len;
//cout<<" delete_code ; stack  "<<stackavail()<<" mem  "<<coreleft()<<"\n";
  };


int find_match(char *buf,int len,code_struct &code,int &code_n,int &Add,int Begin)
  {
//cout<<" find_match name ";
//for (int i=0;i<len1;i++) cout<<buf1[i];cout<<"\n";
//for (    i=0;i<len2;i++) cout<<buf2[i];cout<<"\n";
   int tmp,len1;
   char *name;
   for (int k=Begin;k<len;k++)
     {
      for (int i=1;i<=code.NumCodes;i++)
         if ( len-k>=(len1=code.Buf_Len[i]) )
           {
            tmp=1;name=code.code[i];
            for (int k1=k;k1<k+len1;k1++) if (buf[k1]!=name[k1-k]) {tmp=0;break;}
            if (tmp) {code_n=i; Add=len1; return k; }
           }
     }   
   return -1;
  };

void find_code(char *buf,code_struct &code,code_config *Res,int buf_len)
   // procedure which finds all codes (different) in the input
 {
  int k=0,Num=0,code_n,Add;
  char *name;   
  while ( ((k=find_match(buf,buf_len,code,code_n,Add,k))>=0) && 
             (Num<MaxFoundCode) )
        {
//cout<<" find_code  found "<<k<<" code_n "<<code_n<<" Add "<<Add<<"\n";
         Num++;Res[Num].position=k;Res[Num].code_n=code_n;k+=Add;}
  Res[0].position=Num;

 };


void code_p(char *buf0,code_struct &code,code_config *Res,int buf_len,
            int &Last_Page)
// procedure which codes page by table
  {
   int cur_page=0,k=0,bound=0;
//cout<<" code_p GetCurName\n";
   unsigned char *page=(unsigned char *)code.code[Last_Page],
	             *buf=(unsigned char *)buf0;
//cout<<" Page "<<tmp<<"  ";
//for (int i=1;i<=256;i++) cout<<int((unsigned char)(page[i]))<<" ";cout<<"\n";
   while ( (k<=buf_len) && (cur_page<Res[0].position) )
     {
      cur_page++;
      page=(unsigned char *)code.code[Res[cur_page].code_n];
      if ( (bound=Res[cur_page].position)>buf_len ) bound=buf_len;
      while (bound>k)  {buf[k]=page[buf[k]];k++;}
     }
   while (buf_len>k)  {buf[k]=page[buf[k]];k++;}
   if (cur_page!=0) Last_Page=Res[cur_page].code_n;
//cout<<" code_p ; stack  "<<stackavail()<<" mem  "<<coreleft()<<"\n";
  };

void code_ch(char *buf,code_struct &code,code_struct &code_prev,
             code_config *Res,int buf_len,fstream &out,int &Sub)
// procedure which substitutes combinations, saves output
//  to the fstream.
  {
   int cur_page=0,cur_pos=0,new_pos=0,page_len,code_len,order_n;
   char *page;
   while ( (cur_pos<=buf_len) && (cur_page<Res[0].position) )
     {
      cur_page++;
      order_n=Res[cur_page].code_n;
      new_pos=Res[cur_page].position;
      if (new_pos>=buf_len) break;
      out.write(&buf[cur_pos],new_pos-cur_pos);                
      page=code.code[order_n];
      page_len=code.Buf_Len[order_n];
      code_len=code_prev.Buf_Len[order_n];
//cout<<" code_ch GetCurName "<<" delta "<<new_pos-cur_pos<<" page len "<<page_len<<"\n";
      out.write(page,page_len);
      cur_pos=new_pos+code_len;
     }
    Sub=max<int>(cur_pos-buf_len,0);
    if (Sub==0) 
     out.write(&buf[cur_pos],buf_len-cur_pos);
//cout<<" code_ch ; stack  "<<stackavail()<<" mem  "<<coreleft()<<"\n";
//cout<<" code ch Sub "<<Sub<<" cur_pos "<<cur_pos<<" buf_len "<<buf_len<<"\n";
  };




void unlimited_change(code_struct &code_page,code_struct &page,
                      code_struct &code_change,code_struct &change,
                      fstream &in,fstream &out)
  {
//   struct time t;
   code_config *Res=new code_config[MaxFoundCode+1];
   int Last_Page=1;
//   code_config *Res_change=new code_config[MaxFoundCode+1];
   int buf_len,Sub_buf,Buf_work,Add_len;
   char *buf=new char[MaxInputBuf+1];
   in.read(buf,MaxInputBuf);buf_len=in.gcount();
//cout<<" buf read ";for (int k=0;k<buf_len;k++) cout<<buf[k];cout<<"\n";
   while (!in.eof())
     {
      Buf_work=buf_len-MaxCodeLength;
//gettime(&t);cout<<"           "<<(int)t.ti_min<<"   "<<(int)t.ti_sec<<"   "<<(int)t.ti_hund<<"\n";
      find_code(buf,code_page,Res,buf_len);
//gettime(&t);cout<<" find code "<<(int)t.ti_min<<"   "<<(int)t.ti_sec<<"   "<<(int)t.ti_hund<<"\n";
      delete_code(buf,code_page,Res,Buf_work,Sub_buf);
//gettime(&t);cout<<" del  code "<<(int)t.ti_min<<"   "<<(int)t.ti_sec<<"   "<<(int)t.ti_hund<<"\n";
      code_p(buf,page,Res,Buf_work,Last_Page);
//gettime(&t);cout<<" page code "<<(int)t.ti_min<<"   "<<(int)t.ti_sec<<"   "<<(int)t.ti_hund<<"\n";

      Add_len=MaxCodeLength-Sub_buf;
      MemoryMove(&buf[buf_len- Add_len],&buf[Buf_work],sizeof(char)*Add_len);
      buf_len=Buf_work+Add_len;
//cout<<" unlimited Sub_buf page "<<Sub_buf<<" Add len "<<Add_len<<"\n";
//cout<<" unlimited buf_len "<<buf_len<<"\n";
//cout<<" buf page \n";for ( int k=0;k<buf_len;k++) cout<<buf[k];cout<<"\n";

//gettime(&t);cout<<"           "<<(int)t.ti_min<<"   "<<(int)t.ti_sec<<"   "<<(int)t.ti_hund<<"\n";
      if (Sub_buf!=0) find_code(buf,code_change,Res,Buf_work);
      else find_code(buf,code_change,Res,buf_len);
//gettime(&t);cout<<" find code "<<(int)t.ti_min<<"   "<<(int)t.ti_sec<<"   "<<(int)t.ti_hund<<"\n";
//cout<<" find code \n";for (k=0;k<buf_len;k++) cout<<buf[k];cout<<"\n";
      code_ch(buf,change,code_change,Res,Buf_work,out,Sub_buf);
//gettime(&t);cout<<" chan code "<<(int)t.ti_min<<"   "<<(int)t.ti_sec<<"   "<<(int)t.ti_hund<<"\n";
//cout<<" code ch \n";for ( k=0;k<Buf_work;k++) cout<<buf[k];cout<<"\n";

      Add_len-=Sub_buf;
      MemoryMove(&buf[buf_len- Add_len],&buf[0],sizeof(char)*Add_len);
//cout<<" unlimited Sub_buf change"<<Sub_buf<<" Add len "<<Add_len<<"\n";
//cout<<" unlimited buf_work "<<Buf_work<<"\n";
      in.read(&buf[Add_len],MaxInputBuf-Add_len);buf_len=in.gcount();
      buf_len+=Add_len;
//cout<<" buf read \n";for ( k=0;k<buf_len;k++) cout<<buf[k];cout<<"\n";
     }

   find_code(buf,code_page,Res,buf_len);
//cout<<" Unlimit ; find "<<"\n";
   delete_code(buf,code_page,Res,buf_len,Sub_buf);
//cout<<" Unlimit ; delete code"<<"\n";
   code_p(buf,page,Res,buf_len,Last_Page);
//cout<<" Unlimit ; page_cvt"<<"\n";
   find_code(buf,code_change,Res,buf_len);
//cout<<" Unlimit ; find change"<<"\n";
   code_ch(buf,change,code_change,Res,buf_len,out,Sub_buf);
//cout<<" Unlimit ; stack  "<<stackavail()<<" mem  "<<coreleft()<<"\n";
//cout<<" Unlimit ; change_cvt Sub_buf "<<Sub_buf<<"\n";
   delete Res;delete buf;
  };
void  form_page(char *tmp1,char *tmp2)
  {
   char tmp[257];
//cout<<" Form Page  strlen "<<strlen(tmp1)<<"\n";
   for ( int i=1;i<=256;i++ )
    {
     tmp[i]=i;
     for ( int k=0;k<strlen(tmp1);k++ )
       if (int((unsigned char)tmp1[k])==i) {tmp[i]=tmp2[k];break;}
    }
  for ( int k=1;k<=256;k++ ) tmp1[k]=tmp[k];tmp1[0]=0;
//cout<<" Page ";
//for ( i=1;i<=256;i++) cout<<int(tmp1[i])<<" ";cout<<"\n";

  };
int test_zer(char *tmp)
{
 int i=0,ret=1;
 while (tmp[i]!=0)
  {if (tmp[i]!=48) return 1;
   else 
    {cout<<tmp<<"\n";ret=0;}i++;}
 return ret;
}
void get_num(fstream &in,char *tmp1,char *tmp2,int &len1,int &len2)
  {
   char tmp[257];
   int i,k=0;
   in>>tmp;
   while (( (i=test_zer(tmp))==0) || ((i=atoi(tmp))!=0 )) 
     {
//cout<<"getting num "<<tmp<<"\n";
      tmp1[k]=i;k++;in>>tmp;};len1=k;
   k=0;
   while  (!seek_eoln(in)) {in>>tmp;tmp2[k]=atoi(tmp);k++;} ;len2=k;
//   seek_eoln(in);
  };
void form_code(fstream &in,code_struct &code_page,code_struct &page,
                           code_struct &code_change,code_struct &change,int UseComma=0)
  {
   char tmp1[2048],tmp2[2048];
//   seek_str(in,"begin");
   do
     {in>>tmp1;}
   while ( (Stricmp(tmp1,"begin")!=0) && (!in.eof()) );
   if (in.eof()) { cout<<"Could not find begin, halted.\n";exit(1);}
   in>>tmp1;
   while ( (Stricmp(tmp1,"change")!=0) && (!seek_eof(in)) )
     {
      code_page.AddCode(tmp1,strlen(tmp1),1);
      in.getline(tmp1,511);
      in.getline(tmp2,511);

      form_page(tmp1,tmp2);page.AddCode(tmp1,257,1);
      in>>tmp1;
#ifdef _DEBUG
cout<<tmp1<<"\n";
#endif
     }
   if (!in) {cout<<"could not find begin. Exiting.\n";exit(1);}
   int len1,len2;
   while (!seek_eof(in))
     {
//	  if (UseComma) 
//		{in.getline(tmp1,sizeof(tmp1)-1,'"');
//	     in.getline(tmp1,sizeof(tmp1)-1,'"');
//         if (seek_eoln(in)) tmp2[0]=0;
//         else in.getline(tmp2,sizeof(tmp2)-1,'"');
//	     in.getline(tmp2,sizeof(tmp2)-1,'"');seek_eoln(in);
//	  }
        len1=strlen(tmp1);len2=strlen(tmp2);
        in>>tmp1;
        if (Stricmp(tmp1,"num")==0) get_num(in,tmp1,tmp2,len1,len2);
      else 
       {
        if (seek_eoln(in)) tmp2[0]=0;
        else in>>tmp2;seek_eoln(in);
        len1=strlen(tmp1);len2=strlen(tmp2);
       }
      if (len1>0)
        {
#ifdef _DEBUG
cout<<" Add code "<<len1<<"    "<<len2<<"\n";
for (int itmp=0;itmp<len1;itmp++) cout<<int(tmp1[itmp]);cout<<"\n";
for (    itmp=0;itmp<len2;itmp++) cout<<int(tmp2[itmp]);cout<<"\n";
#endif
         code_change.AddCode(tmp1,len1,len1);
         change.AddCode(tmp2,len2);
        }
     }
  };

void main( int argc, char *argv[] )
  {
   argc--;
   if (((argc!=3)&&(argc!=4)) || (GetCmd("/h",argc,argv)!=NULL))
     {
      cout<<"usage:\n"<<argv[0]<<"  file_to_code save_result code_method_file key\n"<<
           "      /h - display help\n  key  /c  - to use comma as single block";

      exit(1);
     }
//#ifdef _DEBUG
SetLeakTest();//cout<<Coreleft()<<" Mem avail Begin  \n";
//#endif
   int UseComma=0;
   if ((argc==4) && (Stricmp(argv[4],"/c")==0)) UseComma=1;
   code_struct *code_page=  new code_struct,
               *page=       new code_struct,
               *code_change=new code_struct,
               *change=     new code_struct;

   fstream *in=new fstream(argv[3],ios::in);
   form_code(*in,*code_page,*page,*code_change,*change,UseComma);
   delete in;

   in=new fstream(argv[1],ios::in|ios::binary);
   fstream *out=new fstream(argv[2],ios::out|ios::binary);
//cout<<" unlimited change \n";
   Time_struct time;
   unlimited_change(*code_page,*page,*code_change,*change,*in,*out);
#ifdef _DEBUG
cout<<" Run Time "<<time;
#endif
   out->close();in->close();delete in;delete out;
   delete code_page; delete page;   delete code_change;   delete change;
//cout<<Coreleft()<<" Mem avail End \n";
   LeakTest();
  };




//=======================================================
//=======================================================
//=======================================================

