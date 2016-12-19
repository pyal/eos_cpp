#ifndef __UTIL_H
#define __UTIL_H

#include "const.h"
#include <stdio.h>
#ifdef WCPP_NET
  #include <istream>
  #include <ostream>
  #include <iostream>
  #include <fstream>
  #include <iomanip>
  #include <strstream>
  using namespace std;
  #pragma warning( disable : 4244 ) // conversion double to int
  #pragma warning( disable : 4996 ) // aaa was depricated
  #pragma warning( disable : 4800 ) // 'int' : forcing value to bool 'true' or 'false' 
  #pragma warning( disable : 4786 )

  #include <io.h>
  #include <conio.h>
  #include <crtdbg.h>
  #include <typeinfo.h>
#else
	#ifndef MAC
	  #include <strstrea.h>
	  #include <iostream.h>
	  #include <fstream.h>
	  #include <iomanip.h>
	#endif
#endif
#ifdef MAC
	#include <istream>
	#include <ostream>
	#include <iostream>
	#include <fstream>
	#include <iomanip>
	#include <strstream>
	#include <typeinfo>
#include <zconf.h>
	using namespace std;
//	#include <io.h>
//	#include <conio.h>
//	#include <crtdbg.h>
#endif
#ifdef WCPP
  #include <string>
  #pragma warning( disable : 4244 )
  #include <io.h>
  #include <conio.h>
  #include <crtdbg.h>
#endif
#ifdef MSDOS
  #include <bios.h>
  #include <malloc.h>
  #include <dir.h>
  #include <io.h>
#endif
#ifdef LINUX
  #include <unistd.h>
  #include <dirent.h>
  #include <fcntl.h>
#endif
//#include <stdio.h>


#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include <time.h>

//#include <values.h>

#define NullNumber 1e300

// ================  SIMPLE USEFULL FUNCTIONS  ==================

void ExchVar(void *v1,void *v2,int size);
size_t MemoryMove(const void *src,void *dest,size_t size);

//void RandomInit();
//#ifndef MSDOS
//	struct RandomizerInit{protected:RandomizerInit(){srand( (unsigned)time( NULL ) );}
//    static RandomizerInit RandomizerInit_Object;};
//#endif

struct RandomClass{
	static double Rnd(double MaxVal);

	struct RandomInit{
		enum RandomType{PseudoRandom,StdRandom} ;
		friend RandomClass;
	protected:
		RandomInit(){InitSeed(StdRandom);};
		void InitSeed(RandomType rndtype);
		double Seed[5];
		RandomType RndType;
	};

	static RandomInit::RandomType InitSeed(int rndType=-1){
		if (rndType!=-1) 
			Rnd_Buf.InitSeed(RandomInit::RandomType(rndType));
		return Rnd_Buf.RndType;
	}
protected:
	static RandomInit Rnd_Buf;
};


int In_Lim(double Val,double from,double to,int BndInclude=1);

void InputArr(istream &f,double *Ar,
                   int &GetPnt,int &GetCol,int MaxCol,int MaxPnt );
int SqPolynom(double a,double b,double c,double &res1,double &res2);
// res1 - upper root; if not sqr polynom - returns false && res1 - solution of the linear polynom

// #include "grlib.h"       ======  Some strange errors
// ==== GrScr *AxGraph(GrArr *ar,char *Xstr,char *Ystr); defined now in grlib

int ChRead();
int seek_eof(istream &i);
int seek_eoln(istream &i);
void line_feed(istream &i);
int seek_str(istream &in,char *Str);

double sqr(double a);
//#ifndef max
//	#define min(a,b) ((a) <= (b) ? (a) : (b))
//	#define max(a,b) ((a) >= (b) ? (a) : (b))
//#endif
void SetExt(const char *src,char *dst,const char *setext);
void SetPathExt(char *src,char *dst,char *setpath,char *setext);
void SetPath(char *src,char *dst,char *setpath);
void SetLength(char *name,int length);

char *GetCmd(const char *key,int argc, char *argv[]);
void TrimSpaces(char *&input);
//int IsDouble(char *(&input),double &ans,int MayBeNeg);
int IsDouble(const char *input,double &ans,int MayBeNeg);
// if '' or "" returns : double-true ; ans=NullNumber
int IsInt(char *(&input),int &ans,int MayBeNag);
int EnterStr(istream &in,double *pnt,int MaxPnt,int break_=0);
void WriteStr(ostream &out,double *ar,int NumCol);
int OpenOutFileControl(char *file_name,fstream &out,int control_existence);
int CopyFile(const char *src,const char *dst);
long int Coreleft();
long int StackAvail();
int Stricmp(const char *str1,const char *str2);
int Strnicmp(const char *str1,const char *str2,int len);
char *Strupr(char *src);
char *Strlor(char *src);
void MergePath(char *FullName,const char *drive,const char *dir,const char *name,const char *ext);
void SplitPath(const char *FullName,char *drive,char *dir,char *name,char *ext);
char* Itoa(int i, char *buf, int base);
bool IsNan(double x);
bool Finite(double x);
char* Gcvt(double x, int ndigit, char *tmp);
void SetLeakTest();
void LeakTest();
int LeakTest(std::string &Err);

#ifdef WINDOWS_HEAD
	#include <windows.h>
	std::string GetLastMessage();
	int TestLastError(void *res);
#endif
#if	(defined(_DEBUG) || defined(_SHOWMATHERR)) && ( defined(WCPP_NET) || defined(WCPP) )
int _matherr( struct _exception *except );
#endif

void TestNextWord(istream &in,const char *wrd, const char* errMes = NULL);
// Depricateed - defined in SavableClass - better
//int GuessNextChar(istream &in, char ch);


#endif
