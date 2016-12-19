#include <lib/precompiled/Std.h>


#define FALSE 0
#define TRUE 1
//#include <string.h>

#include "util.h"
#include <fcntl.h>

#ifndef LINUX
#include <sys/stat.h>
#endif

double sqr(double a){return a*a;};// a*=a - was very wrong !!!!!!!!!!!!!!!!111
//double max(double a,double b) {if (a>b) return a;else return b;};
//double min(double a,double b) {if (a<b) return a;else return b;};

#ifdef WCPP
#include <malloc.h>
#endif

long int Coreleft()
 {
#ifdef MSDOS
   return coreleft();
#else
#ifdef WCPP
   long int Used=0;
   _HEAPINFO hinfo;
   int heapstatus;   hinfo._pentry = NULL;
   while( ( heapstatus = _heapwalk( &hinfo ) ) == _HEAPOK )
     if (hinfo._useflag == _USEDENTRY) Used+=hinfo._size;
//     Used+=hinfo._size;
   if ( (heapstatus!=_HEAPEND) && (heapstatus!=_HEAPEMPTY) ) return -1;
   return Used;
#else
   return 0;
#endif
#endif
 }
/*
{
#ifndef WCPP
   return coreleft();
#else
#ifdef DEBUG_MODE
   _CrtMemState state;_CrtMemCheckpoint(&state );
	return state.lTotalCount;
#else
//	MEMORYSTATUS *mem;GlobalMemoryStatus(mem)
//	return mem->dwAvailVirtual;
	return 0;
#endif
#endif
};
*/
#if ( (defined(WCPP) || defined(WCPP_NET)) && defined(_DEBUG) )

//	_CrtMemState CheckLeakStatusSet;
#include <stack>
	stack<_CrtMemState*> CheckLeakStatusSet;
	void SetLeakTest()
	{
_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT );

		CheckLeakStatusSet.push(new _CrtMemState());
		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE ); _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
		//   _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE ); _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
		//   _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE ); _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );
	//	_CrtMemCheckpoint( &CheckLeakStatusSet );
		_CrtMemCheckpoint( CheckLeakStatusSet.top() );
	};
	void LeakTest()
	{
		_CrtMemState s2, s3;_CrtMemCheckpoint( &s2 );
		if (CheckLeakStatusSet.empty()) {cout<<" Bad leak test, double after init?\n";return;}
//		if ( _CrtMemDifference( &s3, &CheckLeakStatusSet, &s2 ) )
		if ( _CrtMemDifference( &s3, CheckLeakStatusSet.top(), &s2 ) )
			{ cout<<" Memory difference registered, looks like memory leak\n";
			  cout<<" UnAlloc normall blocks "<<(unsigned int)s3.lCounts[1]<<" memory there "<<(unsigned int)s3.lSizes[1]<<"\n";
			  cout.flush();
			_CrtMemDumpStatistics( &s3 );
			}
		else cout<<" No Memory difference registered. OK\n";cout.flush();
		delete CheckLeakStatusSet.top();
		CheckLeakStatusSet.pop();
	}

	int LeakTest(std::string &Err)
	{
	_CrtMemState s2, s3;_CrtMemCheckpoint( &s2 );
	char tmp[256],tmp1[256];
	int ret = 0;
	if (CheckLeakStatusSet.empty()) {cout<<" Bad leak test, double LeakTest after SetLeakTest?";return 1;}
//	if ( _CrtMemDifference( &s3, &CheckLeakStatusSet, &s2 ) )
	
	if ( _CrtMemDifference( &s3, CheckLeakStatusSet.top(), &s2 ) )
		{Err="";Err.append(" Memory difference registered\n").append(" UnAlloc normall blocks ").append(itoa((int)s3.lCounts[1],&tmp[0],10)).append(" memory there ").append(itoa((int)s3.lSizes[1],tmp1,10));ret=1;}
	else Err="OK";
	delete CheckLeakStatusSet.top();
	CheckLeakStatusSet.pop();
	return ret;
	}

#else
	void SetLeakTest(){};
	void LeakTest(){};
	int LeakTest(std::string &Err){return 0;};
#endif


long int StackAvail()
{
#ifdef MSDOS
   return stackavail();
#else
	return 0;
#endif
};
int SqPolynom(double a,double b,double c,double &res1,double &res2)
  {
   double sqroot=b*b-4*a*c;
			res1=0;res2=0;
   if (sqroot<0) return 0;
			if (fabs(a)<MathZer)
			  {
//      cout<<" SqPolynom - not square but linear polynom\n";
					 if (fabs(b)>MathZer) res1=-c/b;
						return 0;
					}
   res1=(-b+sqrt(sqroot))/(2*a);
   res2=(-b-sqrt(sqroot))/(2*a);
   return 1;
  };

int ChRead()
  {
   int u;
#ifdef MSDOS
   u=bioskey(0);
#endif
#ifdef WINDOWS_HEAD
   while (!_kbhit());u=_getch();
#endif
#ifdef MAC
    cout << "Not implemented ChRead under mac\n";
    exit(0);
#endif

   return u;
  };

int seek_eof(istream &i)
  {
   char ch=' ';
   int ret=0;
   while ( (i) && ((ch==' ') || (ch=='\n')) ) i.get(ch);
   if (i) { i.putback(ch);}
   if (!i) ret=1;
   return ret;
  };
int seek_eoln(istream &i)
  {
   char ch=' ';
   int ret=0;
   while ( (i) && (ch==' ') ) i.get(ch);
   if (ch=='\n') ret=1;
   else if (i) i.putback(ch);
   if (!i) ret=1;
   return ret;
  };

int Stricmp(const char *str1,const char *str2)
  {
#ifdef MAC
    return strcasecmp(str1,str2);
#else
    #ifndef LINUX
       return stricmp(str1,str2);
    #else
       return strcasecmp(str1,str2);
    #endif
#endif
  };

int Strnicmp(const char *str1,const char *str2,int len)
  {
#ifdef MAC
    cout<<" mac Strnicmp. Abort.\n";exit(0);
    return strcasecmp(str1,str2);
#else
    #ifndef LINUX
       return strnicmp(str1,str2,len);
    #else
       cout<<" linux Strnicmp. Abort.\n";abort(0);
       return strcasecmp(str1,str2);
    #endif
#endif
  };
char *Strupr(char *src)
  {
#ifdef MAC
    cout<<" mac Strnicmp. Abort.\n";exit(0);
    return src;
#else
    #ifndef LINUX
       return strupr(src);
    #else
       cout<<" linux Strupr. Abort.\n";abort(0);
       return src;
    #endif
#endif
  };


char* Itoa(int i, char *buf, int base) {
#ifdef MAC
    if(!buf) return buf;
    static int len = sizeof(int) * 8;
    if (snprintf(buf, len, "%d", i) == -1)
        buf[0] = 0;
        return buf;
#else
    return itoa(i, buf, base);
#endif
}

bool IsNan(double x) {
#ifdef MAC
    return isnan(x);
#else
    return _isnan(x);
#endif
}

bool Finite(double x) {
#ifdef MAC
    return finite(x);
#else
    return _finite(x);
#endif
}

char* Gcvt(double x, int ndigit, char *tmp) {
#ifdef MAC
    return gcvt(x, ndigit, tmp);
#else
    return _gcvt(x, ndigit, tmp);
#endif
}

void line_feed(istream &i)
  { char ch=' ';while ((i) && (ch!='\n')) i.get(ch); };

int seek_str(istream &in,char *Str)
  {
   char tmp[256];
   while (!seek_eof(in))
	{in>>tmp;if (Stricmp(tmp,Str)==0)
		  return 1;}
   return 0;
  };

void InputArr(istream &f,double *Ar,
		   int &GetPnt,int &GetCol,int MaxCol,int MaxPnt )
  {
   double d;
   GetCol=0;
   GetPnt=1;
//   char ch=' ';
   line_feed(f);
//   while ((f) && (ch!='\n')) f.get(ch);
   while (!seek_eoln(f))
      {
       if (MaxCol<=GetCol) {line_feed(f);break;}
       GetCol++;
       f>>d;Ar[(GetCol-1)*MaxPnt+GetPnt]=d;
      }
   while (!seek_eof(f) && (GetPnt<MaxPnt-1) )
     {
      GetPnt++;int k=0;
      while (!seek_eoln(f))
	{
	 if (GetCol<=k) {line_feed(f);break;}
	 k++;f>>d;Ar[(k-1)*MaxPnt+GetPnt]=d;
	}
     }
  };

void ExchVar(void *v1,void *v2,int size)
  {
   char *a=new char[size];
#ifdef MSDOS
   movmem(v1,a,size);movmem(v2,v1,size);
   movmem(a,v2,size);
#else
   memmove(a,v1,size);memmove(v1,v2,size);
   memmove(v2,a,size);
#endif
   delete[] a;
  };
size_t MemoryMove(const void *src,void *dest,size_t size)
  {
#ifdef MSDOS
   return movmem(src,dest,size)==NULL?0:size;
#else
   return memmove(dest,src,size)==NULL?0:size;
#endif
  };
//
//void RandomInit()
//{
//#ifdef MSDOS
//   randomize();
//#else
//   //srand( (unsigned)time( NULL ) );
//#endif
//};
//#ifndef MSDOS
//RandomizerInit RandomizerInit::RandomizerInit_Object;
//#endif


void RandomClass::RandomInit::InitSeed(RandomType rndtype){
	RndType = rndtype;
	if (RndType==PseudoRandom){
		unsigned int s = 6543;//uint32
		// make random numbers and put them into the buffer
		for (int i=0; i<5; i++) {
			s = s * 29943829 - 1;
			Seed[i] = s * (1./(65536.*65536.));}
	} else {
#ifndef MSDOS
		srand( (unsigned)time( NULL ) );
#else
		randomize();
#endif
	}
};



double RandomClass::Rnd(double MaxVal){
	RandomInit::RandomType rndType = InitSeed();
	if (rndType==RandomInit::StdRandom){
		return ((double)rand())/(double)RAND_MAX*MaxVal;
	} else {
// returns a random number between 0 and 1:
		long double c;
		c = (long double)2111111111.0 * Rnd_Buf.Seed[3] +
			1492.0 * (Rnd_Buf.Seed[3] = Rnd_Buf.Seed[2]) +
			1776.0 * (Rnd_Buf.Seed[2] = Rnd_Buf.Seed[1]) +
			5115.0 * (Rnd_Buf.Seed[1] = Rnd_Buf.Seed[0]) +
			Rnd_Buf.Seed[4];
		Rnd_Buf.Seed[4] = floorl(c);
		Rnd_Buf.Seed[0] = c - Rnd_Buf.Seed[4];
		Rnd_Buf.Seed[4] = Rnd_Buf.Seed[4] * (1./(65536.*65536.));
		return Rnd_Buf.Seed[0]*MaxVal;
	}
}
RandomClass::RandomInit RandomClass::Rnd_Buf;










//int In_Lim(double Val,double from,double to)
int In_Lim(double Val,double from,double to,int BndInclude)
  {
   if ( from>to) ExchVar(&from,&to,sizeof(to));
   if (BndInclude) if ((Val==from) || (Val==to)) return 1;
//   {
    if ( (Val>=to) || (Val<=from) ) return 0;
    return 1;
//   }
//    if ( (Val>to) || (Val<from) ) return 0;
//    return 1;

  };

void SetExt(const char *src,char *dst,const char *setext)
  {
#ifdef MSDOS
   char path_buffer[50],name[50],drive[50],dir[50],ext[5];
   strcpy(path_buffer,src);
   fnsplit(path_buffer,drive,name,dir,ext);
   fnmerge(path_buffer,drive,name,dir,setext);
   strcpy(dst,path_buffer);
#else
#if defined(WCPP) || defined(WCPP_NET)
   char path_buffer[_MAX_PATH];
   char drive[_MAX_DRIVE];
   char dir[_MAX_DIR];
   char fname[_MAX_FNAME];
   char ext[_MAX_EXT];
   strcpy(path_buffer,src);
   _splitpath( path_buffer, drive, dir, fname, ext );
   _makepath(  path_buffer, drive, dir, fname, setext );
   strcpy(dst,path_buffer);
#else
#ifdef LINUX_NO
   char dir[PC_PATH_MAX];
   char fname[PC_NAME_MAX];
   char ext[PC_NAME_MAX];
   strcpy(path_buffer,src);
   _splitpath( path_buffer, drive, dir, fname, ext );
   _makepath(  path_buffer, drive, dir, fname, setext );
#endif
#endif
#endif
  };
void SetPathExt(char *src,char *dst,char *setpath,char *setext)
  {
#ifdef MSDOS
   char tmp[50],name[50],drive[50],dir[50],ext[5];
   char         name1[50],drive1[50],dir1[50],ext1[5];
   strcpy(tmp,src);
   fnsplit(src,drive,dir,name,ext);
   int k=strlen(setpath);if (setpath[k-1]!='\\') {setpath[k]='\\';setpath[k+1]=0;}
   fnsplit(setpath,drive1,dir1,name1,ext1);
   fnmerge(tmp,drive1,dir1,name,setext);
   strcpy(dst,tmp);
#else
#ifdef WCPP
   char tmp[_MAX_PATH],drive[_MAX_DRIVE],dir[_MAX_DIR],name[_MAX_FNAME],ext[_MAX_EXT];
   char drive1[_MAX_DRIVE],dir1[_MAX_DIR],name1[_MAX_FNAME],ext1[_MAX_EXT];
   strcpy(tmp,src);
   _splitpath( tmp, drive, dir, name, ext );
   int k=strlen(setpath);if (setpath[k-1]!='\\') {setpath[k]='\\';setpath[k+1]=0;}
   _splitpath( setpath, drive1, dir1, name1, ext1 );
   _makepath( tmp, drive1, dir1, name, setext );
   strcpy(dst,tmp);
#else
#ifdef LINUX_NO
   char dir[PC_PATH_MAX];
   char fname[PC_NAME_MAX];
   char ext[PC_NAME_MAX];
   strcpy(path_buffer,src);
   _splitpath( path_buffer, drive, dir, fname, ext );
   _makepath(  path_buffer, drive, dir, fname, setext );
#endif
#endif
#endif
  };
void SetLength(char *name,int length)
  {
   int k=0,k1;
   while ((name[k]) && (k<length) ) k++;
   if (k!=length) for (k1=k;k1<length;k1++) name[k1]=' ';
   name[length]=0;
  };
void SetPath(char *src,char *dst,char *setpath)
  {
#ifdef MSDOS
   char tmp[50],name[50],drive[50],dir[50],ext[5];
   char         name1[50],drive1[50],dir1[50],ext1[5];
   strcpy(tmp,src);
   fnsplit(src,drive,dir,name,ext);
   int k=strlen(setpath);if (setpath[k-1]!='\\') {setpath[k]='\\';setpath[k+1]=0;}
   fnsplit(setpath,drive1,dir1,name1,ext1);
   fnmerge(tmp,drive1,dir1,name,ext);
   strcpy(dst,tmp);
#else
#ifdef WCPP
   char tmp[_MAX_PATH],drive[_MAX_DRIVE],dir[_MAX_DIR],name[_MAX_FNAME],ext[_MAX_EXT];
   char drive1[_MAX_DRIVE],dir1[_MAX_DIR],name1[_MAX_FNAME],ext1[_MAX_EXT];
   strcpy(tmp,src);
   _splitpath( tmp, drive, dir, name, ext );
   int k=strlen(setpath);if (setpath[k-1]!='\\') {setpath[k]='\\';setpath[k+1]=0;}
   _splitpath( setpath, drive1, dir1, name1, ext1 );
   _makepath( tmp, drive1, dir1, name, ext );
   strcpy(dst,tmp);
#else
#ifdef LINUX_NO
   char dir[PC_PATH_MAX];
   char fname[PC_NAME_MAX];
   char ext[PC_NAME_MAX];
   strcpy(path_buffer,src);
   _splitpath( path_buffer, drive, dir, fname, ext );
   _makepath(  path_buffer, drive, dir, fname, setext );
#endif
#endif
#endif
  };

char *GetCmd(const char *key,int argc, char *argv[])
   {
    size_t len = strlen(key);
    char *s;
    for (size_t k=1;k<=(size_t)argc;k++)
      {
       s=argv[k];
       if (strncmp(s, key, len) == 0)
         {
          s += len;
          return s;
         }
      }
    return NULL;
  }

void TrimSpaces(char *&input)
  {  while ((*input)==' ')  { input++;} }
//int IsDouble(char *(&input),double &ans,int MayBeNeg)
int IsDouble(const char *in,double &ans,int MayBeNeg) {
    char *input = (char*)in;
   char *start, numstring[80];
   int decimal, len, numlen;
   double coef=1;
   ans=0;
   TrimSpaces(input);
   if (*input=='"')
     {
      input++;
      if (*input=='"') {ans=NullNumber;return 1;}
      else return 0;
     }
   if (MayBeNeg) {
       if (*input=='-') {coef=-1;input++;}
   }
   if (*input=='+') 
       input++;
   if (strchr("0123456789.", *input)) {
    start = input;
    len = 0;
    decimal = FALSE;
    while ((isdigit(*input)) ||
      ((*input == '.') && (!decimal)))
    {
     if (*input == '.')
      decimal = TRUE;
     input++;
     len++;
    }
    if ((len == 1) && (start[0] == '.'))
     return 0;
    if ((*input == 'E') || (*input == 'e'))
    {
     input++;
     len++;
     if (strchr("+-", *input) != NULL)
     {
      input++;
      len++;
     }
     numlen = 0;
     while ((isdigit(*input)) && (++numlen <= 5))
     {
      input++;
      len++;
     }
    }
    strncpy(numstring, start, len);
    numstring[len] = 0;
    ans = atof(numstring);
    ans=ans*coef;
    return 1;
   }
   return 0;
  };
int IsInt(char *(&input),int &ans,int MayBeNag)
  {
   int len, maxlen = 10;
   char *start, numstring[10];
   int coef=1;
   int ret=FALSE;
   TrimSpaces(input);
   if (*input==0) return ret;
   if (MayBeNag) {if (*input=='-') {coef=-1;input++;}}
   start=input;
   for (len = 0; len < maxlen; len++)
    { if (!isdigit(*input))  break;input++;  }

    if (len == 0)  return FALSE;
    strncpy(numstring, start, len);
    numstring[len] = 0;
    ans = atoi(numstring);
    ans*=coef;
    return TRUE;
   };
int EnterStr(istream &in,double *pnt,int MaxPnt,int break_)
  {
   char s[256],*s1;s1=s;
   int ret=0;
//   double d;
   do
      {
       if (MaxPnt<=ret) {line_feed(in);break;}
       ret++;
       s1=s;in>>s;
       if (!IsDouble(s1,pnt[ret],1)) {line_feed(in);ret=0;if (break_) return ret;}
      }
   while (!seek_eoln(in));
   return ret;
  };
void WriteStr(ostream &out,double *ar,int NumCol)
  {
//   out.x_width=13;
   for (int k=1;k<=NumCol;k++)
     {
      if (ar[k]==NullNumber) out<<setw(15)<<"\"\""<<"  ";
      else out<<setw(15)<<setprecision(10)<<ar[k]  <<"  ";
     }
   out<<"\n";
  };
int OpenOutFileControl(char *file_name,fstream &out,int control_existence)
  {
   int ret=1;
#ifdef MAC
    int OpenMethod=ios::out;
#else
    #ifdef WCPP_NET
       int OpenMethod=ios::out;
    #else
       int OpenMethod=ios::out|ios::noreplace;
    #endif
#endif
   if (!control_existence) OpenMethod=ios::out;
   out.open(file_name,OpenMethod);
   if (out.fail())
     {
      cout<<"File: "<<file_name<<" already exists. Overwrite[y] ?";cout.flush();
      int  ch=(char)ChRead();cout<<(char)ch<<"\n";
      if (((char)ch=='Y')||((char)ch=='y')||(ch==13))
       {out.close();out.clear();
        out.open(file_name,ios::out);}
//      else {ret=0;}
      if (out.fail()) {ret=0;cout<<"File was not opened\n";}
      cout.flush();
     }
   return ret;
  };
int CopyFile(const char *src,const char *dst)
  {
   int Read,Write;
#ifdef MAC
#define BINMODE
#define S_IMODE
#else
    #define BINMODE O_BINARY |
    #define S_IMODE ,S_IREAD  | S_IWRITE
    #ifdef LINUX
        #define BINMODE
        #define S_IMODE
    #endif
#endif
   if ((Read = open(src, BINMODE O_RDONLY)) == -1)
     { cout<<"Error opening file "<<src<<" in CopyFile\n";return 0;}
   if ((Write = open(dst, BINMODE O_WRONLY | O_CREAT | O_TRUNC S_IMODE)) == -1)
     { cout<<"Error writing file "<<dst<<" in CopyFile\n";return 0;}
   int Size=4096,Get;
   char *ptr=new char[Size];
   do
     {Get=read(Read,ptr,Size);write(Write,ptr,Get);}
   while (Get==Size);
   delete[] ptr;
   close(Write);close(Read);
   return 1;
  };

#ifdef WINDOWS_HEAD
std::string GetLastMessage()
{
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
    FORMAT_MESSAGE_FROM_SYSTEM | 
    FORMAT_MESSAGE_IGNORE_INSERTS,NULL,
       GetLastError(),NULL, (LPTSTR) &lpMsgBuf,0,NULL);
    std::string ret((char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
    return ret;
};
int TestLastError(void *res)
  {
    if (res==NULL) {cout<<" Error caught; "<<GetLastMessage().c_str()<<"\n";return 0;}
    else return 1;
  };


#if	(defined(_DEBUG) || defined(_SHOWMATHERR)) && ( defined(WCPP_NET) || defined(WCPP) )
int _matherr( struct _exception *except ){
	char *descr[]={"","_DOMAIN","_SING","_OVERFLOW","_UNDERFLOW","_TLOSS","_PLOSS"};
	cout<<"MathErr occured. Error type:"<<descr[except->type]<<" func name "<<except->name<<
		" arg1 "<<except->arg1<<" arg2 "<<except->arg2<<" retval "<<except->retval<<"\n";
    //if( except->type == _DOMAIN ){
    //    if( strcmp( except->name, "log" ) == 0 ){
    //        except->retval = log( -(except->arg1) );
    //        printf( "Special: using absolute value: %s: _DOMAIN error\n", except->name );
    //        return 1;
    //    }
    //    else if( strcmp( except->name, "log10" ) == 0 ){
    //        except->retval = log10( -(except->arg1) );
    //        printf( "Special: using absolute value: %s: _DOMAIN error\n", except->name );
    //        return 1;
    //    }
    //}
    return 0;    
}
#endif


#endif

#include "stdexception.h"

void TestNextWord(istream &in, const char *wrd, const char* errMes){
    char tmp[102400];
    in>>tmp;
    if (Stricmp(tmp, wrd) != 0) {
        if (errMes)
            throw info_except("Expected <%s> and got <%s> - descr %s\n", wrd,
                              tmp, errMes);
        else
            throw info_except("Expected <%s> and got <%s>\n", wrd, tmp);
    }
}
// Defined in savableClass - better
//int GuessNextChar(istream &in, char ch){
//    char c;
//    while(!(!in) && isspace( c=in.peek() ))
//        in.get(c);
//    if (!in || c!=ch)
//        return 0;
//    return 1;
//}

