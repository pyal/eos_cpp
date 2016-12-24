#include <lib/precompiled/lex_anal_lib.h>
#include "lex_anal.h"

const char *Lex_Result::type_names[] = {"NumberInt",
                                        "NumberDouble",
                                        "String",
                                        "Char",
                                        "Token",
                                        "Identifier",
                                        "Error",
                                        "End"};

string Lex_Result::ToString() {
    char tmp[256];
    string res;
    res.append(" res_type ")
        .append(max<double>(13 - strlen(type_names[res_type]), 0), ' ')
        .append(type_names[res_type]);
    res.append(" res_int ").append(Itoa(res_int, tmp, 10));
    res.append(" res_val ").append(Gcvt(res_val, 12, tmp));
    res.append(" res_str ").append(res_str).append("\n");
    return res;
}
int Lex_Result::Equals(Lex_Result *fst, Lex_Result *sec) {
    if(fst == NULL) {
        if(sec != NULL)
            return 0;
        else
            return 1;
    }
    if(sec == NULL)
        return 0;
    if(fst->res_type == Lex_Result::Error) {
        if(sec->res_type == Lex_Result::Error)
            return 1;
        else
            return 0;
    }
    if(sec->res_type == Lex_Result::Error)
        return 0;
    if(fst->ToString() != sec->ToString())
        return 0;
    return 1;
}
ErrInf::ErrInf(const char *str, StreamManip &manip) {
    char tmp[256];
    reason = string(str).append("\nIt occured in line ");
    reason.append(Itoa(manip.GetLine(), &tmp[0], 10));
    reason.append(" position ");
    reason.append(Itoa(manip.GetPos(), &tmp[0], 10));
    reason.append("\n");
};


StateResults StateSkipSpace::CheckState(LexStateData &dat) {
    while(1) {
        if(!dat.manip.ReadWhileChars(" \t\f\v\b\r\n", 7)) {
            dat.res.ErrRes("End of file found");
            return Break;
        }
        return Continue;
    }
}

StateResults StateComment::CheckState(LexStateData &dat) {
    try {
        if(dat.manip.Guess("//", 2)) {
            dat.manip.SearchChar('\n');
            return Start;
        } else if(dat.manip.Guess("/*", 2)) {
            if(!dat.manip.SearchString("*/", 2))
                throw ErrInf("End of file found before closing comment", dat.manip);
            else
                return Start;
        }
        return Continue;
    } catch(ErrInf err) {
        dat.res.ErrRes(err.ToStr().c_str());
        return Break;
    }
}

StateResults StateNumber::CheckState(LexStateData &dat) {

    if(!dat.manip.GuessInChars(".0123456789", 11))
        return Continue;
    dat.manip.unget();
    int base = 10, ForceDouble = 0;
    if(dat.manip.GuessInChars("0", 1)) {
        if(dat.manip.GuessInChars("xX", 2))
            return ReadBase16(dat);   //{base=16;manip.UnfreezePos();manip.FreezePos();}
        if(dat.manip.GuessInChars("01234567", 8)) {
            dat.manip.unget();
            base = 8;
        }
    }
    dat.manip.FreezePos();
    while(dat.manip.GuessInChars("0123456789", 10))
        ;
    if(dat.manip.GuessInChars("uU", 2))
        return ConstructUnsigned(dat, base);
    if(!dat.manip.GuessInChars(".eE", 3))
        return ConstructSigned(dat, base);
    else
        dat.manip.unget();
    if(dat.manip.GuessInChars(".", 1)) {
        while(dat.manip.GuessInChars("0123456789", 10))
            ;
        ForceDouble = 1;
    }
    string str = dat.manip.UnfreezeToStr();
    dat.manip.FreezePos();
    if(dat.manip.GuessInChars("eE", 2)) {
        dat.manip.GuessInChars("+-", 2);
        if(!dat.manip.GuessInChars("0123456789", 10)) {
            dat.manip.RestorePos();
            if(ForceDouble)
                return ConstructDouble(dat, str);
            else
                return ConstructSigned(dat, str, base);
        }
        while(dat.manip.GuessInChars("0123456789", 10))
            ;
    }
    str = str + dat.manip.UnfreezeToStr();
    return ConstructDouble(dat, str);
}
StateResults StateNumber::ReadBase16(LexStateData &dat) {
    dat.manip.FreezePos();
    while(dat.manip.GuessInChars("0123456789abcdefABCDEF", 22))
        ;
    return ConstructSigned(dat, 16);
}
StateResults StateNumber::ConstructUnsigned(LexStateData &dat, int base) {
    char *tmp_end;
    string str = dat.manip.UnfreezeToStr();
    str.erase(--str.end());   // Getting rid of filnal symbol: uU
    dat.res.GenerateResult(
        strtoul(str.c_str(), &tmp_end, base), 0, "", Lex_Result::NumberInt);
    return End;
}
StateResults StateNumber::ConstructSigned(LexStateData &dat, int base) {
    dat.res.GenerateResult(dat.manip.UnfreezeToL(base), 0, "", Lex_Result::NumberInt);
    return End;
}
StateResults StateNumber::ConstructDouble(LexStateData &dat) {
    dat.res.GenerateResult(0, dat.manip.UnfreezeToD(), "", Lex_Result::NumberDouble);
    return End;
}
StateResults StateNumber::ConstructSigned(LexStateData &dat, string &str, int base) {
    char *tmp_end;
    dat.res.GenerateResult(
        strtol(str.c_str(), &tmp_end, base), 0, "", Lex_Result::NumberInt);
    return End;
}
StateResults StateNumber::ConstructDouble(LexStateData &dat, string &str) {
    char *tmp_end;
    dat.res.GenerateResult(
        0, strtod(str.c_str(), &tmp_end), "", Lex_Result::NumberDouble);
    return End;
}

StateResults StateString::CheckState(LexStateData &dat) {
    string str;
    try {
        if(ReadStrChar(dat.manip, '\'', str)) {

            if(str.length() != 1) {
                dat.res.ErrRes(
                    string("Malformed char is found:").append(str).append("\n").c_str());
                return Break;
            }
            dat.res.GenerateResult(0, 0, str.c_str(), Lex_Result::Char);
            return End;
        }
        if(!ReadStrChar(dat.manip, '\"', str))
            return Continue;
        dat.res.GenerateResult(0, 0, str.c_str(), Lex_Result::String);
        return End;
    } catch(ErrInf err) {
        dat.res.ErrRes(err.ToStr().c_str());
        return Break;
    }
}
int StateString::ReadStrChar(StreamManip &manip, char ch, string &res) {
    if(!manip.Guess(&ch, 1))
        return 0;
    res = "";
    char tmp[2] = {ch, '\\'};
    do {
        manip.FreezePos();
        if(!manip.SearchForChars(&tmp[0], 2))
            throw(ErrInf(" Not found closing chars for string or symbol", manip));
        if(manip.LastChar() == '\\') {
            res = res + manip.UnfreezeToStr();
            res.erase(--res.end());
            res = res + decode(manip);
        }
    } while(manip.LastChar() != ch);
    res = res + manip.UnfreezeToStr();
    res.erase(--res.end());   //Getting rid of final comma
    return 1;
}
string StateString::decode(StreamManip &manip) {
    if(manip.Guess("x", 1)) {
        manip.FreezePos();
        if(!manip.GuessInChars("1234567890abcdefABCDEF", 22))
            throw(ErrInf("Escape symbol - without following digits", manip));
        while(manip.GuessInChars("1234567890abcdefABCDEF", 22))
            ;
        return CheckChar(manip, manip.UnfreezeToL(16));
    }
    if(manip.GuessInChars("01234567", 8)) {
        manip.unget();
        int OK;
        manip.FreezePos();
        if(manip.GuessInChars("01234567", 8))
            if(manip.GuessInChars("01234567", 8))
                OK = 1;
        return CheckChar(manip, manip.UnfreezeToL(8));
    }
    if(manip.GuessInChars("\'\"\\", 3))
        return string(1, (char)manip.LastChar());
    //		if (manip.GuessInChars("ntrfbav",7)) return string("\\").append(1,(char)manip.LastChar());
    if(manip.GuessInChars("n", 1))
        return string(1, '\n');
    if(manip.GuessInChars("t", 1))
        return string(1, '\t');
    if(manip.GuessInChars("r", 1))
        return string(1, '\r');
    if(manip.GuessInChars("f", 1))
        return string(1, '\f');
    if(manip.GuessInChars("b", 1))
        return string(1, '\b');
    if(manip.GuessInChars("a", 1))
        return string(1, '\a');
    if(manip.GuessInChars("v", 1))
        return string(1, '\v');
    if(manip.GuessInChars("\n", 1))
        return string("");
    return string(1, (char)manip.get());
}

string StateString::CheckChar(StreamManip &manip, int val) {
    if((val < 0) || (val > 256))
        throw(ErrInf("Escape symbol - too large symbol - reading chars", manip));
    return string(1, (char)val);
}


StateResults StateIdentifier::CheckState(LexStateData &dat) {
    if(!dat.manip.GuessInChars(alpha.c_str(), (int)alpha.length()))
        return Continue;
    dat.manip.unget();
    dat.manip.FreezePos();
    while(dat.manip.GuessInChars(alpha.c_str(), (int)alpha.length()))
        ;
    dat.res.GenerateResult(
        0, 0, dat.manip.UnfreezeToStr().c_str(), Lex_Result::Identifier);
    return End;
}
StateResults StateDefIsError::CheckState(LexStateData &dat) {
    ErrInf err("Cannot assign lexema. Error", dat.manip);
    dat.res.ErrRes(err.ToStr().c_str());
    dat.manip.SearchChar('\n');
    return End;
}
StateResults StateTest::CheckState(LexStateData &dat) {
    //	if (WriteResults) cout<<dat.res;
    if(WriteResults)
        cout << dat.res.res_str.c_str() << "\n";
    return Continue;
}
StateResults StateTokenSingle::CheckState(LexStateData &dat) {
    for(list<TokenData>::iterator it = tokens.begin(); it != tokens.end(); it++)
        if(dat.manip.Guess(it->str.c_str(), it->len)) {
            dat.res.GenerateResult(it->val, 0, it->str.c_str(), Lex_Result::Token);
            return Break;   // Here break - to immediately return, not to do last token check
        }
    return Continue;
}
void StateTokenSingle::Add(const char *tok, int val) {
    TokenData cur(string(tok), val);
    int l = cur.len;
    list<TokenData>::iterator it = tokens.begin();
    while((it != tokens.end()) && (it->len > l))
        it++;
    tokens.insert(it, cur);
}

StateResults StateToken::CheckState(LexStateData &data) {
    StateResults res = StateBrowser<LexStateData>::CheckState(data);
    if(res == Break)
        res = End;
    return res;
}

void StateToken::Add(const char *tok, int val) {
    try {
        StateTokenSingle *cur = ReverceTopState();
        if(!cur)
            throw ErrInf("trying to add token to NULL token state list\n");
        cur->Add(tok, val);
    } catch(ErrInf &e) {
        cout << e.ToStr() << "\n";
    }
}
