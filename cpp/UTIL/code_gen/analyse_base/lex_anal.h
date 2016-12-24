#ifndef _ANLEX_ANAL_H_
#define _ANLEX_ANAL_H_

#include "lib/std/util.h"
#include <string>
//#include <list>
//#include <stack>
//#pragma warning( disable : 4267 )
//#include "lib/ref/Stroka.h"

#include "stream_buf.h"
struct Lex_Result {
    enum ResType { NumberInt, NumberDouble, String, Char, Token, Identifier, Error, End };
    static const char *type_names
        [];   //={"NumberInt","NumberDouble","String","Char","Token","Identifier","Error","End";
    ResType res_type;
    long res_int;
    double res_val;
    //	Stroka res_str;
    string res_str;
    Lex_Result() {
        ErrRes(" Lex_Result is not inited ");
    }
    Lex_Result(long n, double d, const char *ch, ResType t) {
        GenerateResult(n, d, ch, t);
    }
    Lex_Result(const char *ch) {
        ErrRes(ch);
    }
    Lex_Result(const Lex_Result &res) {
        GenerateResult(res.res_int, res.res_val, res.res_str.c_str(), res.res_type);
    }

    void GenerateResult(long n, double d, const char *ch, ResType t) {
        res_int = n;
        res_val = d;
        //        res_str = string(ch);
        res_str.assign(ch);
        res_type = t;
    }
    void ErrRes(const char *ch) {
        res_int = 0;
        res_val = 0;
        res_str.assign(ch);
        res_type = Error;
    }

    string ToString();
    static int Equals(Lex_Result *fst, Lex_Result *sec);
    friend ostream &operator<<(ostream &out, Lex_Result &lex) {
        out << lex.ToString().c_str();
        return out;
    }
};
struct ErrInf {
    string ToStr() {
        return reason;
    }
    ErrInf(const char *str, StreamManip &manip);
    ErrInf(const char *str) : reason(str){};

private:
    string reason;
};
// Is used by StateBrowser - to determine what to do
// Continue - continue the check, Break - to stop the check and return
// Start - to return to the start check point, End - to make the last check
enum StateResults { Continue, Break, Start, End };   //UpLevel, DownLevel
// Is used by StateBrowser -all states have to be childrens of this class
template <class state_data>
struct StateInterface {
    virtual StateResults CheckState(state_data &data) = 0;
    virtual ~StateInterface(){};
};
// Main class to browse through states
// for each state - calls CheckState - accroding to the result value
// makes desision - to break check, to return to the begining, to continue
// or to make the last check
// stores all States, deletes them before exiting
template <class state_data>
struct StateBrowser : StateInterface<state_data> {
    StateBrowser(){};
    // Main point
    //StateResults StateBrowser::CheckState(state_data &data)
    StateResults CheckState(state_data &data) {
        typename list<StateInterface<state_data> *>::iterator it = sub_states.begin();
        while(it != sub_states.end()) {
            StateResults res = (*it)->CheckState(data);
            switch(res) {
            case Continue:
                it++;
                break;
            case Start:
                it = sub_states.begin();
                break;
            //				case End:	it=sub_states.end();it--;break;
            case End:
                it = sub_states.end();
                break;
            case Break:
                return Break;
                break;
            default:
                cout << " error StateBrowser::CheckState\n";
            }
        }
        return Continue;
    }
    void PushState(StateInterface<state_data> *st) {
        sub_states.push_back(st);
    }
    StateInterface<state_data> *TopState() {
        if(sub_states.empty())
            return NULL;
        return sub_states.back();
    }
    void PopState() {
        delete TopState();
        sub_states.pop_back();
    }
    list<StateInterface<state_data> *> sub_states;
    void clear() {
        while(!sub_states.empty())
            PopState();
    }
    ~StateBrowser() {
        clear();
    }
};

struct LexStateData {
    LexStateData(istream *in = NULL) : manip(in){};
    void ResetStream(istream *in) {
        manip.ResetStream(in);
        res = Lex_Result();
    }
    StreamManip manip;
    Lex_Result res;
};


struct StateSkipSpace : StateInterface<LexStateData> {
    StateResults CheckState(LexStateData &dat);
};

struct StateComment : StateInterface<LexStateData> {
    StateResults CheckState(LexStateData &dat);
};
struct StateNumber : StateInterface<LexStateData> {
    StateResults CheckState(LexStateData &dat);

private:
    StateResults ReadBase16(LexStateData &dat);
    StateResults ConstructUnsigned(LexStateData &dat, int base);
    StateResults ConstructSigned(LexStateData &dat, int base);
    StateResults ConstructDouble(LexStateData &dat);
    StateResults ConstructSigned(LexStateData &dat, string &str, int base);
    StateResults ConstructDouble(LexStateData &dat, string &str);
};
struct StateString : StateInterface<LexStateData> {
    StateResults CheckState(LexStateData &dat);

private:
    int ReadStrChar(StreamManip &manip, char ch, string &res);
    string decode(StreamManip &manip);
    string CheckChar(StreamManip &manip, int val);
};


struct StateIdentifier : StateInterface<LexStateData> {
    string alpha;
    StateIdentifier() {
        alpha = GenerateAlphaChar();
    };
    string GenerateAlphaChar() {
        string ret;
        for(int k = 0; k < 256; k++)
            if(isalnum(k))
                ret += (char)k;
        ret += '_';
        ret += '$';
        return ret;
    }
    StateResults CheckState(LexStateData &dat);
};
// Default - error state - xannot resolve current sequence - to skip up to new line
struct StateDefIsError : StateInterface<LexStateData> {
    StateResults CheckState(LexStateData &dat);
};
// Istead of test to output current state
struct StateTest : StateInterface<LexStateData> {
    int WriteResults;
    StateTest(int Write = 1) : WriteResults(Write){};
    StateResults CheckState(LexStateData &dat);
};

struct TokenData {
    string str;
    int val, len;
    TokenData(const string &s, int v) : str(s), val(v) {
        len = (int)str.length();
    };
    TokenData(const TokenData &t) : str(t.str), val(t.val), len(t.len){};
    string ToString() {
        char tmp[256];
        string res("Token is <");
        res.append(str)
            .append("> and its value is <")
            .append(Itoa(val, tmp, 10))
            .append("> ");
        return res;
    }
};
struct StateTokenSingle : StateInterface<LexStateData> {
    StateResults CheckState(LexStateData &dat);
    void Add(const char *tok, int val);
    list<TokenData> tokens;
};

struct StateToken : StateBrowser<LexStateData> {
    StateResults CheckState(LexStateData &data);

    void Add(const char *tok, int val);
    // To search tokens in reverce order - first to search last ones
    void RevercePushState() {
        sub_states.push_front(new StateTokenSingle);
    }
    void RevercePopState() {
        delete ReverceTopState();
        sub_states.pop_front();
    }
    StateTokenSingle *ReverceTopState() {
        if(sub_states.empty())
            return NULL;
        return dynamic_cast<StateTokenSingle *>(sub_states.front());
    }
};

struct Lex_Analyser : RefCount {
    virtual Lex_Result *Analyse() {
        if(browse.CheckState(dat) == StateResults::Break)
            return 0;
        return &dat.res;
    }
    //Lex_Analyser(istream &in,int writeresults=1)
    //       :dat(in),WriteResults(writeresults)	{Init_Analyser();};
    Lex_Analyser(istream *in = NULL, int writeresults = 1)
        : dat(in), WriteResults(writeresults) {
        Init_Analyser();
    };
    void ResetStream(istream *in) {
        dat.ResetStream(in);
    }
    void PushTokenLevel() {
        token->RevercePushState();
    }
    void PopTokenLevel() {
        token->RevercePopState();
    }
    void AddToken(const char *str, int val) {
        token->Add(str, val);
    }
    virtual ~Lex_Analyser(){};
    virtual void Init_Analyser() {
        browse.PushState(new StateSkipSpace);
        browse.PushState(new StateComment);
        browse.PushState(new StateNumber);
        browse.PushState(new StateString);
        browse.PushState(token = new StateToken);
        browse.PushState(new StateIdentifier);
        browse.PushState(new StateDefIsError);
        browse.PushState(new StateTest(WriteResults));
    }
    void GetBuferReadingPos(int &Line, int &Pos) {
        Line = dat.manip.GetLine();
        Pos = dat.manip.GetPos();
    }
    int IsGood() {
        return dat.manip.IsGood();
    }

protected:
    StateBrowser<LexStateData> browse;
    StateToken *token;
    LexStateData dat;
    int WriteResults;
};


#endif   // _ANLEX_ANAL_H_
