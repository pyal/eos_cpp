#ifndef _STREAM_BUF_H_
#define _STREAM_BUF_H_

#include "lib/std/util.h"
#include "lib/std/stdexception.h"
#include "lib/ref/ref.h"
//#include <string>
#undef min
#undef max
#include <list>
#include <stack>
//#pragma warning( disable : 4267 )
#include "lib/Ref/Stroka.h"

struct StreamUngetStorage {
    int ToPut;
    int get(int &ch) {
        int ret = (ToPut != EOF);
        if(ret)
            ch = ToPut;
        ToPut = EOF;
        return ret;
    }
    void put(int ch) {
        if(ToPut != EOF)
            cout << "Double unget, possible eroor!!\n";
        ToPut = ch;
    };
    int active() {
        return ToPut != EOF;
    }
    void clear() {
        ToPut = EOF;
    }
    StreamUngetStorage(const StreamUngetStorage &stor) : ToPut(stor.ToPut){};
    StreamUngetStorage() : ToPut(EOF){};
    StreamUngetStorage &operator=(StreamUngetStorage &src) {
        ToPut = src.ToPut;
        return *this;
    }
};

// Mark stored by StreamMemStorage
struct StreamPosition {
    list<int>::iterator it;
    int Line, Pos, Ch;
    StreamUngetStorage undo;
    StreamPosition(
        list<int>::iterator &it_,
        int Line_,
        int Pos_,
        int Ch_,
        StreamUngetStorage &undo_)
        : it(it_), Line(Line_), Pos(Pos_), Ch(Ch_), undo(undo_){};
    StreamPosition(const StreamPosition &src)
        : it(src.it), Line(src.Line), Pos(src.Pos), Ch(src.Ch), undo(src.undo){};
    //	StreamPosition &operator=(StreamPosition src)	{Line=src.Line;Pos=src.Pos;undo=src.undo; return *this;}
};

// Class used for buffering read stream data
// Can store several marks to restore reading from marked position
// instead of read stream
struct StreamMemStorage {
#define ITER list<int>::iterator
    StreamMemStorage() : ReadingStorage(0){};
    void clear() {
        while(!pos.empty())
            pos.pop();
        buf.erase(buf.begin(), buf.end());
        ReadingStorage = 0;
    };
    // Pushes a mark to skack of restore positions
    // to return there in future
    void PushMark(int line, int pos_, int Ch, StreamUngetStorage &undo);
    // Pops last mark from stack
    // fixing changes done to buffer, no undo in future
    void PopMark();
    // Starts reading from last mark position ( undo changes )
    // Pops last mark from buffer
    void ReturnToMark(int &line, int &pos_, int &Ch, StreamUngetStorage &undo);
    //EOF - if not reading
    int ReadNext(int &ch);
    // Write char to storage if it is necessary, to be called each time after reading
    // the input stream
    void Write(int ch);
    // Returns current reading buffer position
    ITER GetCurrentPos();
    Stroka ToString() {
        Stroka ret("StreamMemStorage { buf<");
        list<int>::iterator i;
        for(i = buf.begin(); i != buf.end(); i++) ret += *i;
        ret += Stroka("> Input level is ") + (int)pos.size() + " ";
        if(ReadingStorage) {
            ret += "Storage is reading, Reading pos ";
            int k = 1;
            i = buf.begin();
            while(1) {
                if(i == ReadingPos)
                    break;
                i++;
                k++;
            }
            if(ReadingPos == buf.end())
                ret += " is at the end of buffer ";
            else
                ret += Stroka("") + k + " char<" + (unsigned char)*i + ">" + " char_num " + (int)(*i);
        } else
            ret += "Not reading it";

        ret +=  " } ";
        return ret;
    }

private:
    list<int> buf;
    stack<StreamPosition> pos;
    ITER ReadingPos;
    int ReadingStorage;
    void TestWrite() {
        if((ReadingStorage) && (ReadingPos == buf.end())) {
            ReadingStorage = 0;
            if(pos.empty())
                clear();
        }
    }
};
// Base stream manipulation class
// Makes it possible to undo last read char in stream
// Return to marked stream position - so can mark position and
// afterwards - to undo readings, or to accept them
// Stores current line and current file position
struct StreamManip_Base : RefCount {

    //	StreamManip_Base(istream &inp):in(inp){LastGetChar=-2;CurPos=1;CurLine=1;}
    StreamManip_Base(istream *inp = NULL) {
        ResetStream(inp);
    }
    void ResetStream(istream *inp) {
        in_ptr = inp;
        LastGetChar = -2;
        CurPos = 1;
        CurLine = 1;
        undo.clear();
        mem.clear();
    }
    int GetLine() {
        return CurLine;
    }   //to be changed for frozen values
    int GetPos() {
        return CurPos;
    }
    // Unget last read char ( return it to the stream )
    void unget() {
        unget(LastChar());
    };
    // Get next char
    int get() {
        //cout<<" EOF "<<EOF<<" LastGetChar "<<LastGetChar<<"\n"<<flush;
        if(LastGetChar != EOF) {
            ReadStream();
            IncrementCurPos();
        }
        log_debug(Stroka("LastChar: ") + LastGetChar + " IsEOF? " + (LastGetChar == EOF));
        return LastGetChar;
    };
    //Reading from buf will return to stacked mark
    void RestorePos() {
        mem.ReturnToMark(CurLine, CurPos, LastGetChar, undo);
    };
    //Stacks a mark.
    // If reading from file - stored mark = buf tail, if reading from buf - stores current buf read position
    void FreezePos() {
        mem.PushMark(CurLine, CurPos, LastGetChar, undo);
    }
    // Remove last mark
    void UnfreezePos() {
        mem.PopMark();
    };   //Skips all to current pos
         // returns last read char
    int LastChar() {
        return LastGetChar;
    }
    int EndIsRead() {
        return LastGetChar == EOF;
    }

    //Generates from frozen part (from last stored mark)- string, Unfreezes position
    string UnfreezeToStr();
    int IsGood() {
        if(!in_ptr)
            return 0;
        return !(!(*in_ptr));
    }
    Stroka ToString() {
        Stroka ret("StreamBufOK: ");
        ret = ret + IsGood() + " MemBufData " + mem.ToString();
        return ret;
    }

    //	char Look(int pos){ return;} //looks several pos back in frozen bufer

private:
    // Return to stream symbol ch - UnfreezeToStr will not work correctly
    // if this symbol will not be the one, previously read
    void unget(const int ch);

    // Increment current file posion counter
    void IncrementCurPos();
    // Decrement current file posion counter
    void DecrementCurPos(const int ch);
    // Read a char from stream, put it into LastGetChar
    void ReadStream();

private:
    int CurPos, CurLine, LastPos;
    istream *in_ptr;
    int LastGetChar;   //ReadBuf,,Reading

    StreamUngetStorage undo;
    StreamMemStorage mem;
};

// Usefull procedures, to use for StreamManip_Base
struct StreamManip : StreamManip_Base {
    StreamManip(istream *inp = NULL) : StreamManip_Base(inp){};
    //  return signed int value constructed from stored buf (from last mark position)
    // Unfreeze buffer after that
    long UnfreezeToL(int base) {
        char *tmp_end;
        return strtol(UnfreezeToStr().c_str(), &tmp_end, base);
    }
    //  return unsigned value constructed from stored buf (from last mark position)
    // Unfreeze buffer after that
    unsigned long UnfreezeToUl(int base) {
        char *tmp_end;
        return strtoul(UnfreezeToStr().c_str(), &tmp_end, base);
    }
    //  return double value constructed from stored buf (from last mark position)
    // Unfreeze buffer after that
    double UnfreezeToD() {
        char *tmp_end;
        return strtod(UnfreezeToStr().c_str(), &tmp_end);
    }

    // Read stream while read symbols are one of the given ones
    // Return 0 - EOF is found first
    int ReadWhileChars(const char *ch, int len);

    // Try to find a given char in the stream
    // return 0 - if EOF is found first, 1 - char is found and read
    int SearchChar(const char ch) {
        int c;
        while(((c = get()) != ch) && (c != EOF))
            ;
        return (c == ch);
    }
    // Try to find one of given chars in the stream
    // return 0 - if EOF is found first, 1 - char is found and read
    int SearchForChars(const char *str, int len);
    // Find a string in the stream, return 0 - EOF is found first, return 1 - string is
    // found and read from the stream
    int SearchString(const char *str, int len);
    // Guess that next is going a given string - if not - stream is unchanged
    int Guess(const char *str, int len);
    // Guess that next char is one of string if not - return last char to stream
    int GuessInChars(const char *ch, int len);
};

#endif


/*
struct StreamUngetStorage
{
	int ToPut;
	int get(int &ch){int ret=(ToPut!=EOF);if (ret) ch=ToPut;ToPut=EOF;return ret;}
	void put(char ch){if (ToPut!=EOF) cout<<"Double unget, possible eroor!!\n";ToPut=ch;};
	int active(){return ToPut!=EOF;}
	StreamUngetStorage(const StreamUngetStorage& stor):ToPut(stor.ToPut){};
	StreamUngetStorage():ToPut(EOF){};
	StreamUngetStorage& operator=(StreamUngetStorage &src){ToPut=src.ToPut;return *this;}
};
struct StreamPosition
{
	list<char>::iterator it;
	int Line,Pos,Char,Reading;
	StreamUngetStorage undo;
	StreamPosition(list<char>::iterator &it_,int Line_,int Pos_,
		StreamUngetStorage &undo_):it(it_),Line(Line_),Pos(Pos_),undo(undo_){};
	StreamPosition(const StreamPosition &src):it(src.it),Line(src.Line),Pos(src.Pos),undo(src.undo){};
//	StreamPosition &operator=(StreamPosition src)	{Line=src.Line;Pos=src.Pos;undo=src.undo; return *this;}
};
// Class used for buffering read stream data
// Can store several marks to restore reading from marked position
// instead of read stream
struct StreamMemStorage
{
#define ITER list<char>::iterator
	StreamMemStorage():ReadingStorage(0){};
	void erase(){while (!pos.empty()) pos.pop();buf.erase(buf.begin(),buf.end());ReadingStorage=0;};
// Pushes a mark to skack of restore positions
// to return there in future
	void PushMark(int line,int pos_,int Ch,StreamUngetStorage &undo)
	{
        list<char>::iterator it=buf.end();
		TestWrite();
		if (ReadingStorage) {it=ReadingPos;it--;}
		StreamPosition p(it,line,pos_,undo);
		pos.push(p);
	};
// Pops last mark from stack
// fixing changes done to buffer, no undo in future
	void PopMark()	
	{	
		if (pos.empty()) cout<<"Stoping to write empty mem\n";
		pos.pop();if ((pos.empty()) && (!ReadingStorage)) erase();
	}
// Starts reading from last mark position ( undo changes )
// Pops last mark from buffer
	void ReturnToMark(int &line,int &pos_,StreamUngetStorage &undo)
	{
		if (pos.empty()) {cout<<"Starting read from empty storage\n";throw;}
		StreamPosition p=pos.top();pos.pop();
		ReadingPos=p.it;line=p.Line;pos_=p.Pos;undo=p.undo;
		ReadingStorage=1;
		ReadingPos++;
	}
//EOF - if not reading
	int ReadNext(int &ch)
	{
		int ret=0;
		TestWrite();
		if (ReadingStorage)	{	ch=*ReadingPos;ReadingPos++;ret=1;	}
		return ret;
	}
// Write char if it is necessary
	void Write(char ch) 
	{
		if ((!ReadingStorage) && (!pos.empty())) buf.push_back(ch);
		TestWrite();
	}
// Returns current reading buffer position
	ITER GetCurrentPos()
	{
//		if (buf.empty()) {cout<<" Trying to read from empty buffer!!\n";throw;}
		ITER ret=buf.end();
		if (ReadingStorage) {ret=ReadingPos;}
		return ret;
	}
	friend ostream& operator<<(ostream &out,StreamMemStorage &mem)
	{
		out<<" buf<";
		list<char>::iterator i;
		for (i=mem.buf.begin();i!=mem.buf.end();i++) out<<*i;
		out<<">\n";
		out<<"Input level is "<<mem.pos.size()<<"\n";
		if (mem.ReadingStorage) 
		{
			out<<"Storage is reading, Reading pos ";
			int k=1;i=mem.buf.begin();
			while (1) {if (i==mem.ReadingPos) break;i++;k++;}
			if (mem.ReadingPos==mem.buf.end()) out<<" is at the end of buffer \n";
			else out<<k<<" char<"<<*i<<">"<<" char_num "<<(int)(*i)<<"\n";
		}
		else out<<"Not reading it\n";

		return out;
	}
private:
	list<char> buf;
	stack<StreamPosition> pos;
	ITER ReadingPos;
	int ReadingStorage;
	void TestWrite(){if ((ReadingStorage) && (ReadingPos==buf.end())) {ReadingStorage=0;if (pos.empty()) erase();}}
};
// Base stream manipulation class
// Makes it possible to undo last read char in stream
// Return to marked stream position - so can mark position and
// afterwards - to undo readings, or to accept them
// Stores current line and current file position
struct StreamManip_Base
{

	StreamManip_Base(istream &inp):in(inp){LastGetChar=-2;CurPos=1;CurLine=1;}
	int GetLine(){return CurLine;}//to be changed for frozen values
	int GetPos(){return CurPos;}
// Unget last read char ( return it to the stream )
	void unget(){unget(LastChar());};
// Get next char
	int get(){if (LastGetChar!=EOF) {ReadStream();PushBuf();}return LastGetChar;};
//Reading from buf will return to stacked mark
	void RestorePos(){	mem.ReturnToMark(CurLine,CurPos,undo);	};
//Stacks a mark.
// If reading from file - stored mark = buf tail, if reading from buf - stores current buf read position
	void FreezePos()	{	mem.PushMark(CurLine,CurPos,LastGetChar,undo);	}
// Remove last mark
	void UnfreezePos(){mem.PopMark();};//Skips all to current pos
// returns last read char
	char LastChar(){return LastGetChar;} 

//Generates from frozen part (from last stored mark)- string, Unfreezes position
	string UnfreezeToStr()
	{
		StreamUngetStorage was_undo=undo;
		ITER upper=mem.GetCurrentPos();
		mem.ReturnToMark(CurLine,CurPos,undo);
		string ret="";
		while ((upper!=mem.GetCurrentPos()) ) {ret+=(char)get();}
		if ((was_undo.active()) && (ret!=""))
		{undo.get(LastGetChar);unget();ret.resize(max<double>(ret.length(),1)-1);}

//		string ret=mem.ReadLabel(undo);	mem.PopMark();
		return ret;
	};
	friend ostream& operator<<(ostream &out,StreamManip_Base &manip)
	{	out<<manip.mem;	return out;	}

//	char Look(int pos){ return;} //looks several pos back in frozen bufer

private:
	void unget(const char ch)
	{
		if (ch=='\n') {CurLine--;CurPos=LastPos;}
		else CurPos--;
		LastGetChar=ch;
		undo.put(LastGetChar);
	}

	void PushBuf()
	{
		if (LastGetChar=='\n') {CurLine++;LastPos=CurPos;CurPos=1;}
		else CurPos++;
	}
	void ReadStream()
	{
		if (undo.get(LastGetChar)) return;
		if (mem.ReadNext(LastGetChar)) return;
		LastGetChar=in.get();
		mem.Write(LastGetChar);
	}

	private:
	int CurPos,CurLine,LastPos;
	istream &in;
	int LastGetChar,ReadBuf,Reading;

	StreamUngetStorage undo;
	StreamMemStorage mem;
};
// Usefull procedures, to use for StreamManip_Base
struct StreamManip:StreamManip_Base
{
	StreamManip(istream &inp):StreamManip_Base(inp){};
	long UnfreezeToL(int base){char *tmp_end;return strtol( UnfreezeToStr().c_str(), &tmp_end, base );}
	unsigned long UnfreezeToUl(int base){char *tmp_end;return strtoul( UnfreezeToStr().c_str(), &tmp_end, base );}
	double UnfreezeToD(){char *tmp_end;return strtod( UnfreezeToStr().c_str(), &tmp_end );}


	int ReadWhileChars(const char *ch,int len)
	{
		int c=get(),k;
		for (k=0;k<len;k++)	if (ch[k]==c) {c=get();k=0;}
		if (c==EOF) return 0;
		unget();
		return 1;
	}
	int SearchChar(const char ch)
	{	int c;while ( ( (c=get())!=ch ) && (c!=EOF) ) ;	return (c==ch);	}
	int SearchForChars(const char* str,int len)
	{
		if (len==0) return 0;
		int c;
		while (1){c=get();if (c==EOF) return 0;for (int k=0;k<len;k++) if (c==str[k]) return 1;}
	}
	int SearchString(const char* str,int len)
	{	if (len==0) return 1;
		while(1)
		{
			if (!SearchChar(str[0])) return 0;
			if (Guess(&str[1],len-1)) return 1;
		}
	}
	int Guess(const char* str,int len)
	{
		FreezePos();
		for (int k=0;k<len;k++)
			if (get()!=str[k]) {RestorePos();return 0;}
		UnfreezePos();
		return 1;
	}
	int GuessInChars(const char *ch,int len)
	{
		char c=get();
		for (int k=0;k<len;k++) if (c==ch[k]) return 1;
		unget();
		return 0;
	}
	Lex_Result res;
};
*/
