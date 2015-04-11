#include <lib\precompiled\lex_anal_lib.h>
#include "stream_buf.h"


//==================================================================
//===============  StreamMemStorage ================================
//==================================================================

void StreamMemStorage::PushMark(int line,int pos_,int Ch,StreamUngetStorage &undo)
{
    list<int>::iterator it=buf.end();
	TestWrite();
	if (ReadingStorage) {
		it=ReadingPos;
		if (it!=buf.begin())
			it--;
		else
			it = buf.end();
	}
	//assert(it>=0);
	StreamPosition p(it,line,pos_,Ch,undo);
	pos.push(p);
};
void StreamMemStorage::PopMark()	
{	
	if (pos.empty()) cout<<"Stoping to write empty mem\n";
	pos.pop();if ((pos.empty()) && (!ReadingStorage)) clear();
}
void StreamMemStorage::ReturnToMark(int &line,int &pos_,int &Ch,StreamUngetStorage &undo)
{
	if (pos.empty()) {cout<<"Starting read from empty storage\n";throw;}
	StreamPosition p=pos.top();pos.pop();
	ReadingPos=p.it;line=p.Line;pos_=p.Pos;undo=p.undo;Ch=p.Ch;
	ReadingStorage=1;
	if (ReadingPos!=buf.end())
		ReadingPos++;
	else 
		ReadingPos=buf.begin();
}
int StreamMemStorage::ReadNext(int &ch)
{
	int ret=0;
	TestWrite();
	if (ReadingStorage)	{	ch=*ReadingPos;ReadingPos++;ret=1;	}
	return ret;
}
void StreamMemStorage::Write(int ch) 
{
	if ((!ReadingStorage) && (!pos.empty())) 
        buf.push_back(ch);
	TestWrite();
}
ITER StreamMemStorage::GetCurrentPos()
{
	ITER ret=buf.end();
	if (ReadingStorage) {ret=ReadingPos;}
	return ret;
}
ostream& operator<<(ostream &out,StreamMemStorage &mem)
{
	out<<" buf<";
	list<int>::iterator i;
	for (i=mem.buf.begin();i!=mem.buf.end();i++) out<<*i;
	out<<">\n";
	out<<"Input level is "<<(int)mem.pos.size()<<"\n";
	if (mem.ReadingStorage) 
	{
		out<<"Storage is reading, Reading pos ";
		int k=1;i=mem.buf.begin();
		while (1) {if (i==mem.ReadingPos) break;i++;k++;}
		if (mem.ReadingPos==mem.buf.end()) out<<" is at the end of buffer \n";
		else out<<k<<" char<"<<(unsigned char)*i<<">"<<" char_num "<<(int)(*i)<<"\n";
	}
	else out<<"Not reading it\n";

	return out;
}
//==================================================================
//===============  StreamMemStorage ================================
//==================================================================
//==================================================================
//===============  StreamManip_Base ================================
//==================================================================

string StreamManip_Base::UnfreezeToStr()
{
	StreamUngetStorage was_undo=undo;
	ITER upper=mem.GetCurrentPos();
	mem.ReturnToMark(CurLine,CurPos,LastGetChar,undo);
	string ret="";
	while ((upper!=mem.GetCurrentPos()) ) {ret+=(char)get();}
	if (LastGetChar==EOF) ret.resize(max(ret.length(),1)-1);
//	if ((was_undo.active()) && (ret!=""))
	if ((was_undo.active()) && (ret.length()!=0))
	{undo.get(LastGetChar);unget();ret.resize(max(ret.length(),1)-1);}

	return ret;
};


void StreamManip_Base::unget(const int ch)
{
	DecrementCurPos(ch);
	LastGetChar=ch;
	undo.put(LastGetChar);
}

void StreamManip_Base::IncrementCurPos()
{
	if (LastGetChar=='\n') {CurLine++;LastPos=CurPos;CurPos=1;}
	else CurPos++;
}
void StreamManip_Base::DecrementCurPos(const int ch)
{
	if (ch=='\n') {CurLine--;CurPos=LastPos;}
	else CurPos--;
}
void StreamManip_Base::ReadStream()
{
	if (undo.get(LastGetChar)) return;
	if (mem.ReadNext(LastGetChar)) return;
    if (!in_ptr) throw info_except("Trying to read from NULL stream\n");
	LastGetChar=in_ptr->get();
	mem.Write(LastGetChar);
}

//==================================================================
//===============  StreamManip_Base ================================
//==================================================================
//==================================================================
//===============  StreamManip      ================================
//==================================================================

int StreamManip::ReadWhileChars(const char *ch,int len)
{
	int c=get(),k;
beg:
	for (k=0;k<len;k++)	if (ch[k]==c) {c=get();goto beg;}//after ++ will be 0
	if (c==EOF) return 0;
	unget();
	return 1;
}
int StreamManip::SearchForChars(const char* str,int len)
{
	if (len==0) return 0;
	int c;
	while (1)
	{
		c=get();
		if (c==EOF) return 0;
		for (int k=0;k<len;k++) if (c==str[k]) return 1;
	}
}
int StreamManip::SearchString(const char* str,int len)
{	if (len==0) return 1;
	while(1)
	{
		if (!SearchChar(str[0])) return 0;
		if (Guess(&str[1],len-1)) return 1;
	}
}
int StreamManip::Guess(const char* str,int len)
{
	FreezePos();
	for (int k=0;k<len;k++)
		if (get()!=str[k]) {RestorePos();return 0;}
	UnfreezePos();
	return 1;
}
int StreamManip::GuessInChars(const char *ch,int len)
{
	char c=get();
	for (int k=0;k<len;k++) if (c==ch[k]) return 1;
	unget();
	return 0;
}
