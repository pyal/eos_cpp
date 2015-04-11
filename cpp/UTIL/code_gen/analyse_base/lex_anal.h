#ifndef _ANLEX_ANAL_H_
#define _ANLEX_ANAL_H_

#include "lib\std\util.h"
#include <string>
//#include <list>
//#include <stack>
//#pragma warning( disable : 4267 )
//#include "lib\ref\Stroka.h"

#include "stream_buf.h"
struct Lex_Result
{
	enum ResType{NumberInt,NumberDouble,String,Char,Token,Identifier,Error,End};
	static const char *type_names[];//={"NumberInt","NumberDouble","String","Char","Token","Identifier","Error","End";
	ResType res_type;
	long res_int;
	double res_val;
//	Stroka res_str;
	string res_str;
	Lex_Result(){ErrRes(" Lex_Result is not inited ");}
	Lex_Result(long n,double d,const char *ch,ResType t){GenerateResult(n,d,ch,t);}
	Lex_Result(const char *ch){ErrRes(ch);}
	Lex_Result(const Lex_Result &res){GenerateResult(res.res_int,res.res_val,res.res_str.c_str(),res.res_type);}

	void GenerateResult(long n,double d,const char *ch,ResType t){
        res_int=n;
        res_val=d;
//        res_str = string(ch);
        res_str.assign(ch);
        res_type=t;	
    }
    void ErrRes(const char *ch)	{res_int=0;res_val=0;res_str.assign(ch);res_type=Error;	}

	string ToString();
	static int Equals(Lex_Result *fst,Lex_Result *sec);
	friend ostream& operator<<(ostream &out,Lex_Result &lex){out<<lex.ToString().c_str();return out;}
};
struct ErrInf
{
	string ToStr(){return reason;}
	ErrInf(const char *str,StreamManip &manip);
	ErrInf(const char *str):reason(str){};
private:
	string reason;
};
// Is used by StateBrowser -all states have to be childrens of this class
template <class state_data>
struct StateInterface
{
// Is used by StateBrowser - to determine what to do
// Continue - continue the check, Break - to stop the check and return
// Start - to return to the start check point, End - to make the last check 
	enum StateResults {Continue,Break,Start,End} ;//UpLevel, DownLevel
	virtual StateResults CheckState(state_data &data)=0;
	virtual ~StateInterface(){};
};
// Main class to browse through states
// for each state - calls CheckState - accroding to the result value
// makes desision - to break check, to return to the begining, to continue
// or to make the last check 
// stores all States, deletes them before exiting
template <class state_data>
struct StateBrowser:StateInterface<state_data>
{
	StateBrowser(){};
// Main point
	//StateResults StateBrowser::CheckState(state_data &data)
	StateResults CheckState(state_data &data)
	{
		list<StateInterface<state_data>*>::iterator it=sub_states.begin();
		while (it!=sub_states.end())
		{
			StateResults res=(*it)->CheckState(data);
			switch(res)
			{
				case Continue:	it++;break;
				case Start:	it=sub_states.begin();break;
//				case End:	it=sub_states.end();it--;break;
				case End:	it=sub_states.end();break;
				case Break:	return Break;break;
				default:cout<<" error StateBrowser::CheckState\n";
			}

		}
		return Continue;
	}
	void PushState(StateInterface<state_data> *st){sub_states.push_back(st);}
	StateInterface<state_data>* TopState(){if (sub_states.empty()) return NULL;return sub_states.back();}
	void PopState(){delete TopState();sub_states.pop_back();}
	list<StateInterface<state_data>*> sub_states;
	void clear(){while (!sub_states.empty()) PopState();}
	~StateBrowser(){clear();}

};

struct LexStateData
{
	LexStateData(istream *in=NULL):manip(in){};
    void ResetStream(istream *in){
        manip.ResetStream(in);
        res = Lex_Result();
    }
	StreamManip manip;
	Lex_Result res;
};


struct StateSkipSpace:StateInterface<LexStateData>
{	StateResults CheckState(LexStateData &dat);};

struct StateComment:StateInterface<LexStateData>
{	StateResults CheckState(LexStateData &dat);};
struct StateNumber:StateInterface<LexStateData>
{
	StateResults CheckState(LexStateData &dat);
	
private:
	StateResults ReadBase16(LexStateData &dat);
	StateResults ConstructUnsigned(LexStateData &dat,int base);
	StateResults ConstructSigned(LexStateData &dat,int base);
	StateResults ConstructDouble(LexStateData &dat);
	StateResults ConstructSigned(LexStateData &dat,string &str,int base);
	StateResults ConstructDouble(LexStateData &dat,string &str);
};
struct StateString:StateInterface<LexStateData>
{
	StateResults CheckState(LexStateData &dat);
private:
	int ReadStrChar(StreamManip &manip,char ch,string &res);
	string decode(StreamManip &manip);
	string CheckChar(StreamManip &manip,int val);

};





struct StateIdentifier:StateInterface<LexStateData>
{
	string alpha;
	StateIdentifier(){alpha=GenerateAlphaChar();};
	string GenerateAlphaChar()
	{string ret;for (int k=0;k<256;k++) if (isalnum(k)) ret+=(char)k;ret+='_';ret+='$';return ret;}
	StateResults CheckState(LexStateData &dat);
};
// Default - error state - xannot resolve current sequence - to skip up to new line
struct StateDefIsError:StateInterface<LexStateData>
{	StateResults CheckState(LexStateData &dat);};
// Istead of test to output current state
struct StateTest:StateInterface<LexStateData>
{
	int WriteResults;
	StateTest(int Write=1):WriteResults(Write){};
	StateResults CheckState(LexStateData &dat);
};

struct TokenData{string str;int val,len;TokenData(string &s,int v):str(s),val(v){len=(int)str.length();};TokenData(const TokenData &t):str(t.str),val(t.val),len(t.len){};string ToString(){char tmp[256];string res("Token is <");res.append(str).append("> and its value is <").append(itoa(val,tmp,10)).append("> ");return res;}};
struct StateTokenSingle:StateInterface<LexStateData>
{
	StateResults CheckState(LexStateData &dat);
	void Add(const char *tok,int val);
	list<TokenData> tokens;
};

struct StateToken:StateBrowser<LexStateData>
{
	StateResults CheckState(LexStateData &data);

	void Add(const char *tok,int val);
	// To search tokens in reverce order - first to search last ones
	void RevercePushState(){sub_states.push_front(new StateTokenSingle);}
	void RevercePopState(){delete ReverceTopState();sub_states.pop_front();}
	StateTokenSingle* ReverceTopState(){if (sub_states.empty()) return NULL;return dynamic_cast<StateTokenSingle*>(sub_states.front());}
};

struct Lex_Analyser:RefCount
{
	virtual Lex_Result* Analyse()
	{
		if (browse.CheckState(dat)==StateBrowser<LexStateData>::Break) return 0;
		return &dat.res;
	}
	//Lex_Analyser(istream &in,int writeresults=1)
 //       :dat(in),WriteResults(writeresults)	{Init_Analyser();};
	Lex_Analyser(istream *in=NULL,int writeresults=1)
        :dat(in),WriteResults(writeresults)	{Init_Analyser();};
    void ResetStream(istream *in){dat.ResetStream(in);}
	void PushTokenLevel(){token->RevercePushState();}
	void PopTokenLevel(){token->RevercePopState();}
	void AddToken(const char *str,int val){token->Add(str,val);}
	virtual ~Lex_Analyser(){};
    virtual void Init_Analyser(){
		browse.PushState(new StateSkipSpace);
		browse.PushState(new StateComment);
		browse.PushState(new StateNumber);
		browse.PushState(new StateString);
		browse.PushState(token=new StateToken);
		browse.PushState(new StateIdentifier);
		browse.PushState(new StateDefIsError);
		browse.PushState(new StateTest(WriteResults));
    }
    void GetBuferReadingPos(int &Line, int &Pos){
        Line = dat.manip.GetLine();
        Pos  = dat.manip.GetPos();
    }
	int IsGood(){return dat.manip.IsGood();}
protected:
	StateBrowser<LexStateData> browse;
	StateToken *token;
	LexStateData dat;
    int WriteResults;

};






//// Do not understand ...
//struct StateNotIdentifier:StateIdentifier
//{
//	StateNotIdentifier():StateIdentifier(){alpha+="\n \t,.?";};
//	StateResults CheckState(LexStateData &dat)
//	{
//		while ((!dat.manip.GuessInChars(alpha.c_str(),(int)alpha.length()))
//			&& (!dat.manip.EndIsRead()) ) dat.manip.get();
//		if (dat.manip.EndIsRead()) return Break;
//		dat.manip.unget();	return Continue;
//	}
//};
//struct StatePhrase:StateNotIdentifier
//{
//	StatePhrase():StateNotIdentifier(){};
//	StateResults CheckState(LexStateData &dat)
//	{
//		if (!dat.manip.GuessInChars(alpha.c_str(),(int)alpha.length())) return Start;
//		dat.manip.unget();dat.manip.FreezePos();
//		while(dat.manip.GuessInChars(alpha.c_str(),(int)alpha.length()));
//		string res1=dat.manip.UnfreezeToStr(),res;
//// What is it????????????????
//		if (res1.length()<40) return Start;
//		char tmp[256];
//		res=string("\nLine ").append(itoa(dat.manip.GetLine(),&tmp[0],10));
//		res.append(" Pos ").append(itoa(dat.manip.GetPos(),&tmp[0],10));
//		res.append(" string \n").append(res1);
//		dat.res.GenerateResult(0,0,res1.c_str(),
//			Lex_Result::ResType::Identifier);
////		dat.res.GenerateResult(0,0,res.c_str(),	Lex_Result::ResType::Identifier);
//		return End;
//	};
//};

#endif

/*
struct StateSkipSpace:StateInterface<LexStateData>
{
	StateResults CheckState(LexStateData &dat)
	{
	 while(1)
	 {
		if (!dat.manip.ReadWhileChars(" \t\f\v\b\r\n",7)) 
		{
			dat.res.ErrRes("End of file found");return Break;
		}
			return Continue;
	 }
	}
};

struct StateComment:StateInterface<LexStateData>
{
	StateResults CheckState(LexStateData &dat)
	{
		try
		{
			if (dat.manip.Guess("//",2)) {dat.manip.SearchChar('\n');return Start;}
			else if (dat.manip.Guess("/*",2) )
				if (!dat.manip.SearchString("*""/",2)) throw ErrInf("End of file found before closing comment",dat.manip);
				else return Start;
			return Continue;
		}
		catch (ErrInf err)	{dat.res.ErrRes(err.ToStr().c_str());return Break;}
	}

};
struct StateNumber:StateInterface<LexStateData>
{
	StateResults CheckState(LexStateData &dat)
	{

		if (!dat.manip.GuessInChars(".0123456789",11)) return Continue;
		dat.manip.unget();
		int base=10,ForceDouble=0;
		if (dat.manip.GuessInChars("0",1))
		{
			if (dat.manip.GuessInChars("xX",2)) return ReadBase16(dat);//{base=16;manip.UnfreezePos();manip.FreezePos();}
			if (dat.manip.GuessInChars("01234567",8)) {dat.manip.unget();base=8;}
		}
		dat.manip.FreezePos();
			while (dat.manip.GuessInChars("0123456789",10));
			if (dat.manip.GuessInChars("uU",2)) return ConstructUnsigned(dat,base);
			if (!dat.manip.GuessInChars(".eE",3)) return ConstructSigned(dat,base);
				else dat.manip.unget();
			if (dat.manip.GuessInChars(".",1)) {while (dat.manip.GuessInChars("0123456789",10)) ;ForceDouble=1;}
		string str=dat.manip.UnfreezeToStr();
		dat.manip.FreezePos();
			if (dat.manip.GuessInChars("eE",2))
			{
				dat.manip.GuessInChars("+-",2);
				if (!dat.manip.GuessInChars("0123456789",10)) 
				{
					dat.manip.RestorePos();
					if (ForceDouble) return ConstructDouble(dat,str);
					else return ConstructSigned(dat,str,base);
				}
				while (dat.manip.GuessInChars("0123456789",10));
			}
		str=str+dat.manip.UnfreezeToStr();
		return ConstructDouble(dat,str);
	}
private:
	StateResults ReadBase16(LexStateData &dat)
	{
		dat.manip.FreezePos();
		while (dat.manip.GuessInChars("0123456789abcdefABCDEF",22));
		return ConstructSigned(dat,16);
	}
	StateResults ConstructUnsigned(LexStateData &dat,int base)
	{
		char *tmp_end;
		string str=dat.manip.UnfreezeToStr();
		str.erase(--str.end());// Getting rid of filnal symbol: uU
		dat.res.GenerateResult(strtoul( str.c_str(), &tmp_end, base ),0,
			                     "",Lex_Result::ResType::NumberInt);
		return End;
	}
	StateResults ConstructSigned(LexStateData &dat,int base)
	{
		dat.res.GenerateResult(dat.manip.UnfreezeToL(base),0,
			                     "",Lex_Result::ResType::NumberInt);
		return End;
	}
	StateResults ConstructDouble(LexStateData &dat)
	{
		dat.res.GenerateResult(0,dat.manip.UnfreezeToD(),
			                     "",Lex_Result::ResType::NumberDouble);
		return End;
	}
	StateResults ConstructSigned(LexStateData &dat,string &str,int base)
	{
		char *tmp_end;
		dat.res.GenerateResult(strtol(str.c_str(),&tmp_end,base),0,
			                     "",Lex_Result::ResType::NumberInt);
		return End;
	}
	StateResults ConstructDouble(LexStateData &dat,string &str)
	{
		char *tmp_end;
		dat.res.GenerateResult(0,strtod(str.c_str(),&tmp_end),
			                     "",Lex_Result::ResType::NumberDouble);
		return End;
	}
};
struct StateString:StateInterface<LexStateData>
{
	StateResults CheckState(LexStateData &dat)
	{
		string str;
		try
		{
			if (ReadStrChar(dat.manip,'\'',str))
			{

				if (str.length()!=1) {dat.res.ErrRes(string("Malformed char is found:").append(str).append("\n").c_str());return Break;}
				dat.res.GenerateResult(0,0,str.c_str(),Lex_Result::ResType::Char);
				return End;
			}
			if (!ReadStrChar(dat.manip,'\"',str)) return Continue;
			dat.res.GenerateResult(0,0,str.c_str(),Lex_Result::ResType::String);
			return End;
		}
		catch(ErrInf err)
		{dat.res.ErrRes(err.ToStr().c_str());return Break;}
		
	}
private:
	int ReadStrChar(StreamManip &manip,char ch,string &res)
	{
		if (!manip.Guess(&ch,1)) return 0;
		res="";
		char tmp[2]={ch,'\\'};
		do	
		{
			manip.FreezePos();
			if (!manip.SearchForChars(&tmp[0],2)) throw(ErrInf(" Not found closing chars for string or symbol",manip));
			if (manip.LastChar()=='\\') 
			{
				res=res+manip.UnfreezeToStr();res.erase(--res.end());
				res=res+decode(manip);
			}
		}
		while (manip.LastChar()!=ch);
		res=res+manip.UnfreezeToStr();res.erase(--res.end());//Getting rid of final comma
		return 1;
	}
	string decode(StreamManip &manip)
	{
		if (manip.Guess("x",1)) 
		{
			manip.FreezePos();
			if (!manip.GuessInChars("1234567890abcdefABCDEF",22)) 
				throw (ErrInf("Escape symbol - without following digits",manip));
			while (manip.GuessInChars("1234567890abcdefABCDEF",22)) ;
			return CheckChar(manip,manip.UnfreezeToL(16));
		}
		if (manip.GuessInChars("01234567",8))
		{
			manip.unget();
			manip.FreezePos();
			if (manip.GuessInChars("01234567",8)) if (manip.GuessInChars("01234567",8));
			return CheckChar(manip,manip.UnfreezeToL(8));
		}
		if (manip.GuessInChars("\'\"\\",3)) return string(1,(char)manip.LastChar());
//		if (manip.GuessInChars("ntrfbav",7)) return string("\\").append(1,(char)manip.LastChar());
		if (manip.GuessInChars("n",1)) return string(1,'\n');
		if (manip.GuessInChars("t",1)) return string(1,'\t');
		if (manip.GuessInChars("r",1)) return string(1,'\r');
		if (manip.GuessInChars("f",1)) return string(1,'\f');
		if (manip.GuessInChars("b",1)) return string(1,'\b');
		if (manip.GuessInChars("a",1)) return string(1,'\a');
		if (manip.GuessInChars("v",1)) return string(1,'\v');
		if (manip.GuessInChars("\n",1)) return string("");
		return string(1,(char)manip.get());
	}

	string CheckChar(StreamManip &manip,int val)
	{
		if ((val<0) || (val>256)) 
			throw (ErrInf("Escape symbol - too large symbol - reading chars",manip));
		return string(1,(char)val);
	}

};





struct StateIdentifier:StateInterface<LexStateData>
{
	string alpha;
	StateIdentifier(){alpha=GenerateAlphaChar();};
	string GenerateAlphaChar()
	{string ret;for (int k=0;k<256;k++) if (isalnum(k)) ret+=(char)k;ret+='_';ret+='$';return ret;}
	StateResults CheckState(LexStateData &dat)
	{
		if (!dat.manip.GuessInChars(alpha.c_str(),alpha.length())) return Continue;
		dat.manip.unget();dat.manip.FreezePos();
        while(dat.manip.GuessInChars(alpha.c_str(),alpha.length()));
		dat.res.GenerateResult(0,0,dat.manip.UnfreezeToStr().c_str(),
			Lex_Result::ResType::Identifier);
		return End;
	}
};
struct StateDefIsError:StateInterface<LexStateData>
{
	StateResults CheckState(LexStateData &dat)
	{
		ErrInf err("Cannot assign lexema. Error",dat.manip);
		dat.res.ErrRes(err.ToStr().c_str());
		dat.manip.SearchChar('\n');
		return End;
	}
};
struct StateTest:StateInterface<LexStateData>
{
	int WriteResults;
	StateTest(int Write=1):WriteResults(Write){};
	StateResults CheckState(LexStateData &dat)
	{
		if (WriteResults) cout<<dat.res;
		return Continue;
	}
};
struct TokenData{string str;int val,len;TokenData(string &s,int v):str(s),val(v){len=str.length();};TokenData(const TokenData &t):str(t.str),val(t.val),len(t.len){};string ToString(){char tmp[256];string res("Token is <");res.append(str).append("> and its value is <").append(itoa(val,tmp,10)).append("> ");return res;}};
struct StateTokenSingle:StateInterface<LexStateData>
{
	StateResults CheckState(LexStateData &dat)
	{
		for (list<TokenData>::iterator it=tokens.begin();it!=tokens.end();it++)
			if (dat.manip.Guess(it->str.c_str(),it->len)) 
			{
				dat.res.GenerateResult(it->val,0,it->str.c_str(),
					Lex_Result::ResType::Token);
				return Break;// Here break - to immediately return, not to do last token check
			}
		return Continue;
	}
	void Add(const char *tok,int val)
	{
		TokenData cur(string(tok),val);
		int l=cur.len;
		list<TokenData>::iterator it=tokens.begin();
		while ((it!=tokens.end()) && (it->len>l)) it++;
		tokens.insert(it,cur);
	}
	list<TokenData> tokens;
};

struct StateToken:StateBrowser<LexStateData>
{
	StateResults CheckState(LexStateData &data)
	{
		StateResults res=StateBrowser<LexStateData>::CheckState(data);
		if (res==Break) res=End;
		return res;
	}

	void Add(const char *tok,int val)
	{
		try
		{
			StateTokenSingle* cur=ReverceTopState();
			if (!cur) throw ErrInf("trying to add token to NULL token state list\n");
			cur->Add(tok,val);
		}
		catch (ErrInf &e){cout<<e.ToStr()<<"\n";}
	}
	// To search tokens in reverce order - first to search last ones
	void RevercePushState(){sub_states.push_front(new StateTokenSingle);}
	void RevercePopState(){delete ReverceTopState();sub_states.pop_front();}
	StateTokenSingle* ReverceTopState(){if (sub_states.empty()) return NULL;return dynamic_cast<StateTokenSingle*>(sub_states.front());}
};
struct Lex_Analyser
{
	Lex_Result* Analyse()
	{
		if (browse.CheckState(dat)==StateBrowser<LexStateData>::Break) return 0;
		return &dat.res;
	}
	StateBrowser<LexStateData> browse;
	StateToken *token;
	LexStateData dat;
	Lex_Analyser(istream &in,int WriteResults=1):dat(in)
	{
		browse.PushState(new StateSkipSpace);
		browse.PushState(new StateComment);
		browse.PushState(new StateNumber);
		browse.PushState(new StateString);
		browse.PushState(token=new StateToken);
		browse.PushState(new StateIdentifier);
		browse.PushState(new StateDefIsError);
		browse.PushState(new StateTest(WriteResults));
	};
	void PushTokenLevel(){token->RevercePushState();}
	void PopTokenLevel(){token->RevercePopState();}
	void AddToken(const char *str,int val){token->Add(str,val);}
	~Lex_Analyser(){};
};


*/