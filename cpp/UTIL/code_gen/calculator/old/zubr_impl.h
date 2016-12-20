
#pragma once

extern "C" 
#include "zubr_import.h"
extern "C" 
#include "zubr_calculator_tab.h"

#include "util\code_gen\analyse_base\lex_anal.h"
#include "lib/ref/stroka.h"
#include "lib/data/VecRes.h"

struct Symbol:RefCount{
	Symbol(){sym_ptr = this;};
	Symbol(const Symbol& sym){sym_ptr = this;*this=sym;}
	Symbol(const char *name_, int type_, double val_, const char *str_,
		   void *ptr_,int    (*defn_)()):name(name_),type(type_),u(val_,ptr_,defn_,str_){sym_ptr=this;}

	Symbol &operator=(const Symbol& sym){name = sym.name;type = sym.type;u = sym.u;return *this;}

	SYMBOL sym_ptr;
	Stroka name;          /* имя переменной или функции */
	enum Type {Number=NUMBER,Var=VAR,Bltin=BLTIN,Undef=UNDEF,String=STRING};
	int type;
	static const char *type_names[];//={"NumberInt","NumberDouble","String","Char","Token","Identifier","Error","End"};
	static const char *GetTypeName(int t);
	struct InDat {
	    double  val;       /* значение для переменной( VAR )           */
		void  *ptr;  /* указатель на встроенную функцию( BLTIN ) */
		int     (*defn)();  /* FUNCTION, PROCEDURE */
		Stroka  str;       /* string */
		InDat():val(0),ptr(0),defn(0){};
		InDat(const InDat &in){*this = in;};
		InDat &operator=(const InDat &in){val=in.val;ptr=in.ptr;defn=in.defn;str=in.str;return *this;};
		InDat( double val_, void  *ptr_,int    (*defn_)(), const char *str_ ):val(val_),str(str_),ptr(ptr_),defn(defn_){};
	};
    InDat u;
	friend ostream& operator<<(ostream &out, const Symbol &sym){
		out<<" SymbolType "<<GetTypeName(sym.type)<<" Name< "<<sym.name<<" > U.Val "<<sym.u.val<<" U.Str< "<<sym.u.str<<" > ";
		return out;
	}
	friend istream& operator>>(istream &in, const Symbol &sym){
		return in;
	}

};

struct CalculatorVars:RefCount{
    map<Stroka,Ref<Symbol> > progr_vars;
	ResizeVector<Ref<Symbol> > progr_const;

	ZUBR_CALCULATOR_ZUBR_STYPE Pack_ZUBR(Ref<Symbol> &sym){
		ZUBR_CALCULATOR_ZUBR_STYPE ret;
		ret.sym = (Symbol*)sym;
//		ret.inst = &sym->u.defn;
//		ret.inst = 0;
		return ret;
	}
	ZUBR_CALCULATOR_ZUBR_STYPE AddConst(int type,double val,const char *str){
		Ref<Symbol> sym = new Symbol(0,type,val,str,0,0);
		progr_const.push(sym);
		return Pack_ZUBR(sym);
	}
    ZUBR_CALCULATOR_ZUBR_STYPE AddVar(const char *name, int type,double val,const char *str,
									  void  *ptr, int    (*defn)()){
		Ref<Symbol> sym = new Symbol(name,type,val,str,ptr,defn);
		progr_vars[Stroka(name)] = sym;
		return Pack_ZUBR(sym);
	}
    int GetVar(const char *name, ZUBR_CALCULATOR_ZUBR_STYPE &zvar){
		if (progr_vars.find(Stroka(name))==progr_vars.end())
			return 0;
		zvar = Pack_ZUBR(progr_vars[Stroka(name)]);
		return 1;
	};
};

struct BuildInFunc{
    enum {SIN,COS,EXP} Func;//,TETA,RND,INT,ABS} Func;
    static void *FPtr[];
};


struct Lex_Calcul_Analyser: Lex_Analyser
{
    Lex_Calcul_Analyser(istream *in=NULL):Lex_Analyser(in,0){Init_Analyser();};

    virtual void Init_Analyser(){
		browse.clear();
//		browse.PushTokenLevel();
		browse.PushState(new StateSkipSpace);
		browse.PushState(new StateComment);
    
		browse.PushState(new StateNumber);
		browse.PushState(new StateString);
		browse.PushState(token=new StateToken);
		browse.PushState(new StateIdentifier);
		browse.PushState(new StateDefIsError);
//		browse.PushState(new StateTest(WriteResults));
		PushTokenLevel();

        AddToken("sin",BuildInFunc::SIN+1500); 
        AddToken("cos",BuildInFunc::COS+1500); 
        AddToken("exp",BuildInFunc::EXP+1500); 

        AddToken("+=",ADDEQ);
        AddToken("-=",SUBEQ);
        AddToken("*=",MULEQ);
        AddToken("/=",DIVEQ);
        AddToken("print",PRINT);
        AddToken("read",READ);

        AddToken(",",int(','));
        AddToken("(",int('('));
        AddToken(")",int(')'));

        AddToken("+",int('+'));
        AddToken("-",int('-'));
        AddToken("=",int('='));
        AddToken("*",int('*'));
        AddToken("/",int('/'));
        AddToken("^",int('^'));
        AddToken(";",int('\n'));
	};
};
struct Datum{
	double  val;
	Ref<Symbol> sym;
	friend FilterOut& operator<<(FilterOut &out, const Datum &dat){
        out<<" Datum.Val "<<dat.val<<" Datum.sym ";*((ostream*)(&out))<<dat.sym<<" ";
		return out;
	}
	friend FilterIn& operator>>(FilterIn &in,const Datum &dat){return in;}
};
//FilterOut& operator<<(FilterOut &out, const Symbol &sym);
//FilterOut& operator<<(FilterOut &out, const Datum &dat);
//FilterIn& operator>>(FilterIn &in,const Datum &dat);
//FilterIn& operator>>(FilterIn &in,const Symbol &dat);

struct CalculatorProgam: RefCount{
    ResizeVector<Datum> data_stack;
    ResizeVector<INST>  progr;
    INST *progr_counter;
    int returning; // 1 if find operator
    Ref<Lex_Analyser> lex;
	CalculatorVars vars;

    CalculatorProgam(Lex_Analyser* Analyse):lex(Analyse){};
    CalculatorProgam &Execute();
    CalculatorProgam &Read();



    static inline CalculatorProgam *GetCurPtr(){return Ptr;};
    static inline Symbol *ExtractSmbFromPc(){
        Symbol *ret = (Symbol*)(*(Ptr->progr_counter));
        Ptr->progr_counter++;
        return ret;
    };
private:
    static CalculatorProgam* Ptr,*Ptr_Old;
};

//
//struct StaticFunc{
//    static Lex_Calcul_Analyser lex;
//};
//

extern int zubr_calculator_zubr_lex(void);
extern void zubr_calculator_zubr_error(char*);

//extern void error_void    ( void );
void assign    ( void );
void varpush    ( void );
void void_pop    ( void );
void constpush    ( void );
void eval    ( void );
INST *code     ( INST f );
void execute   ( INST *p );

void Yadd    ( void );
void Ysub    ( void );
void Ymul    ( void );
void Ydiv    ( void );
void power   ( void );
void print( void );

void negate    ( void );
void prstr     ( void );
void prexpr    ( void );
void bltin     ( void );
void bltin2    ( void );
void varread   ( void );

void Yaddeq    ( void );
void Ysubeq    ( void );
void Ymuleq    ( void );
void Ydiveq    ( void );

