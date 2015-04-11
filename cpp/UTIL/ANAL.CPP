
#include "anal.h"

#define FAIL -1
#define FALSE 0
#define TRUE 1
#define PLUS 0
#define MINUS 1
#define TIMES 2
#define DIVIDE 3
#define POWER 4
#define COLON 5
#define OPAREN 6
#define CPAREN 7
#define NUM 8
#define CELL 9
#define OPER 10
#define FUNC 11
#define BAD 12
#define SET 13
#define NOVAL 14
#define SIN 15
#define COS 16
#define EXP 17
#define LOG 18
#define RND 19
#define TETA 20
#define INT  21
#define ABS 22




#define INVOPER 0x100
//#define MAXACTIONS 200

//extern unsigned _stklen=40000;

ostream& operator<<(ostream &output,CellAct &act)
  { output<<"Action:    type "<<act.Do.Type<<" act "<<act.Do.act<<" Imp "<<act.Do.Imp
        <<"\nNumber:    Val  "<<act.Var.Val<<" act "<<act.Var.act
        <<" ind "<<act.Var.Num<<" Num or Cell "<<act.Var.Type;
   output<<"\n";
   return output;};


int DefOper(char *(&input),int &what)
  {
   TrimSpaces(input);
   switch(*(input++))
   {
    case '+' : what = PLUS;  break;
    case '-' : what = MINUS; break;
    case '*' : what = TIMES; break;
    case '/' : what = DIVIDE;break;
    case '^' : what = POWER;   break;
//    case ':' : what = COLON; break;
    case '(' : what = OPAREN;break;
    case '=' : what = SET;   break;
    case ')' : what = CPAREN;break;
    default  : input--;      return 0;
   } /* switch */
  return 1;
  };
int DefFunc(char *(&input),int &what)
  {
   TrimSpaces(input);
   int len=0;
   if (Strnicmp("sin",input,3)==0) {len=3;what = SIN;goto good;}
   if (Strnicmp("cos",input,3)==0) {len=3;what = COS;goto good;}
   if (Strnicmp("exp",input,3)==0) {len=3;what = EXP;goto good;}
   if (Strnicmp("log",input,3)==0) {len=3;what = LOG;goto good;}
   if (Strnicmp("rnd",input,3)==0) {len=3;what = RND;goto good;}
   if (Strnicmp("teta",input,3)==0) {len=4;what = TETA;goto good;}
   if (Strnicmp("int",input,3)==0) {len=3;what = INT;goto good;}
   if (Strnicmp("abs",input,3)==0) {len=3;what = ABS;goto good;}
   return 0;
   good:input+=len;return 1;
  };
int StrBeg(char *&input,char *s,int Trim)
// Returns TRUE if the string is a legal function, FALSE otherwise.
  {
   int len = strlen(s);
   if (Trim) TrimSpaces(input);
   if (Strnicmp(s, input, len) == 0)
   {
    input += len;
    return(TRUE);
   }
   return(FALSE);
  } // isfunc
int GetRow(char *(&input), int &col)
// Returns TRUE if the string is the start of a formula, FALSE otherwise.
// Also returns the column and row of the formula.

   {
    int ret=FALSE;
    if ( StrBeg(input,"C(") )
       {
	ret=TRUE;
  if (!IsInt(input,col),FALSE) ret =  FALSE;
        if (!StrBeg(input,")") ) ret =  FALSE;
       }
    return ret;
   }; // GetRow










void push(Action *act,int &a,Action cur)
  {  if ((a<0) || (a>MaxOperation)) {cout<<"Illegal use push"<<a<<"\n";exit(1);}
     act[++a] = cur;
  }

Action pop(Action *act,int &a)
  {if ((a<1) || (a>MaxOperation)) {cout<<"Illegal use pop "<<a<<"\n";
                                    a=0;Action ac;ac.Type=BAD;return ac;}
   return(act[a--]);}

Action nexttoken(char *(&input))
  {
   Action ret;ret.Type=BAD;
   TrimSpaces(input);
   if (*input == 0)      return ret;
   if (IsDouble(input,ret.Val,FALSE)) {ret.Type=NUM;return ret;}
   if (GetRow  (input,ret.Num)) {ret.Type=CELL;return ret;}
   if (DefOper (input,ret.act)) {ret.Type=OPER;return ret;}
   if (DefFunc (input,ret.act)) {ret.Type=FUNC;return ret;}
   ret.Type=FAIL;
   return ret;
  };
int ControlMinus(Action curtoken,char *&input)
  {
   if (curtoken.Type==OPER)
     {
      if ((curtoken.act==SET) || (curtoken.act==OPAREN))
          return (StrBeg(input,"-"));
     }
   return FALSE;
  }
int parsestr(char *s,Action *act,int &k)
  {
//cout<<s<<"\n";Action firsttoken
   Action curtoken,Nag,Mul;
   Nag.Type=NUM;Nag.Val=-1;Mul.Type=OPER;Mul.act=TIMES;
   char *input;//=new char[256];
   int error = FALSE,all=FALSE;
// May be wrong
//   input=strdup(s);
   input=Strupr(strdup(s));

   curtoken = nexttoken(input);
   if (curtoken.Type!=CELL) error=TRUE;
   k=0;
   while ((!error) && (!all))
     {
      push(act,k,curtoken);
      if (ControlMinus(curtoken,input)) {push(act,k,Nag);push(act,k,Mul);}
      curtoken=nexttoken(input);

      if (curtoken.Type==BAD) all=TRUE;
      if (curtoken.Type==FAIL) error=TRUE;
     };
   if (error)
     {cout<<"do not understand identifier. Left str :"<<input<<"\n";exit(1);}
   curtoken.Type = BAD;
   act[++k]=curtoken;
//   delete input;
   return 1;
  };
Action Imp(Action act)
  {
   Action ret=act;ret.Imp=0;
   if (ret.Type==OPER)
     {
     switch (act.act)
       {
        case BAD   :ret.Imp=-1; break;
        case SET   :ret.Imp+=2; break;
        case PLUS  :
        case MINUS :ret.Imp+=4; break;
        case TIMES :
        case DIVIDE:ret.Imp+=6; break;
        case POWER   :ret.Imp+=8; break;
        default   :ret.Imp=0;
       }
     }
   if (ret.Type==FUNC) ret.Imp+=9;
   return ret;
  };
int Control(Action tmp)
  {
   if ( (tmp.act==OPAREN) || (tmp.act==CPAREN) ) return 1;
   return 0;
  };
void SetImp(Action *act)
  {
   int a=0,add=0,t=0;
   Action tmp[MaxOperation],cur;
   while (act[++a].Type!=BAD)
     {
       cur=act[a];
       if ((cur.Type==OPER) || (cur.Type==FUNC))
       {
        if (cur.act==OPAREN) add+=20;
        else  if (cur.act==CPAREN) add-=20;
          else {  cur.Imp=(Imp(cur)).Imp+add;push(tmp,t,cur);}
       }
       else push(tmp,t,cur);
     }
   push(tmp,t,act[a]);
   if (add!=0) {cout<<"you have entered not matched parents\n";exit(1);}
   MemoryMove(tmp,act,(a+1)*sizeof(Action));
  };
int FormCell(Action *in,CellAct *out)
  {
   int curin=1,curout=1;
   Action act=in[curin];
   while (in[++curin].Type!=BAD)
     {
      if (act.Type==FUNC)
        { out[curout].Do=act;out[curout].Var.Type=NOVAL;
         if (in[curin].Type==OPER)
           {cout<<"wrong order of operations. Operation after function\n";exit(1);}
        }
      else
        {
         if ( ((act.Type!=NUM) && (act.Type!=CELL)) ||
              (in[curin].Type!=OPER) )
           {cout<<"wrong order of operations.\n";exit(1);}
         out[curout].Var=act;out[curout].Do=in[curin++];
       }
      act=in[curin];
      curout++;
     }
   out[curout].Do.Type=BAD;out[curout].Do.Imp=-1;out[curout].Var=act;
   return 1;
  };
void ReForm(Action *act,int &aind,CellAct *tmp,int &tind,int imp)
  {
//cout<<tind<<" tind\n";
   int imp0=imp;
   while ((tind>0) && (imp0<=tmp[tind].Do.Imp))
     {
      imp=tmp[tind].Do.Imp;
      int maxtind=tind;
      while ((tind>0) && (imp<=tmp[tind].Do.Imp)) tind--;
      int mintind=tind;
      while (++tind<=maxtind)
        {
         if (tmp[tind].Var.Type!=NOVAL) push(act,aind,tmp[tind].Var);
         else   tmp[tind].Do.act=INVOPER|tmp[tind].Do.act;
         push(act,aind,tmp[tind].Do);
        }
      tind=mintind;
     }
  };
int PackAct(Action *in)
  {
   SetImp(in);
   CellAct tmp[MaxOperation/2+1],now[MaxOperation/2+1];
   if (!FormCell(in,tmp)) {cout<<"could not form operations\n";exit(1);}
   MemoryMove(tmp,now,sizeof(now));
   Action act[MaxOperation];
   int ind=0,tind=0,aind=0;
   while (now[++ind].Do.Type!=BAD)
     {
//cout<<now[ind];
      if (now[ind].Do.Imp>=now[ind+1].Do.Imp)
        {
         push(act,aind,now[ind+1].Var);now[ind+1].Var.Type=NOVAL;
         if (now[ind].Var.Type!=NOVAL) push(act,aind,now[ind].Var);
         else  now[ind].Do.act=INVOPER|now[ind].Do.act;
         push(act,aind,now[ind].Do);
         ReForm(act,aind,tmp,tind,now[ind+1].Do.Imp);
        }
      else tmp[++tind]=now[ind];
     }
//cout<<now[ind];
   ReForm(act,aind,tmp,tind,-1);act[aind+1].Type=BAD;
   MemoryMove(act,in,sizeof(act));
   return 0;
  };
double Extract(Action act,double *Tab)
  {
   if (act.Type == CELL ) return Tab[act.Num];
   return act.Val;
  };
void funclc(Action *act,int &a,Action cur,double *Tab)
  {
   Action x1=pop(act,a);
   if (cur.act&INVOPER) cur.act-=INVOPER;
   double x=Extract(x1,Tab)  ;
   x1.Type=NUM;
//cout<<" x ";
   if (x==NullNumber) x1.Val=x;
   else
     {
      switch (cur.act)
       {
        case SIN    : x1.Val=sin(x) ;       //     cout<<"SIN    ";
                      break;
        case COS    : x1.Val=cos(x) ;       //     cout<<"COS    ";
                      break;
        case EXP    : x1.Val=exp(x) ;       //     cout<<"EXP    ";
                      break;
        case LOG    : x1.Val=log(x) ;       //     cout<<"LOG    ";
                      break;
//        case RND    : x1.Val=((double)rand())/(double)RAND_MAX*x;
		case RND    : x1.Val=RandomClass::Rnd(x);
                                           //     cout<<"RND    ";
                      break;
        case TETA   : if (x>=0) x1.Val=1;else x1.Val=0;
                                           //     cout<<"TETA   ";
                      break;
        case INT    : x1.Val=int(x);
                                           //     cout<<"INT    ";
                      break;
        case ABS    : x1.Val=fabs(x);
                                           //     cout<<"ABS   ";
                      break;
       }
     }
//cout<<" RES FUNC: "<<x1.Val<<"\n  ";
   push(act,a,x1);
  };

void reduce(Action *act,int &a,Action cur,double *Tab)
  {
   Action x1=pop(act,a),y1=pop(act,a);
   if (cur.act&INVOPER) {ExchVar(&x1,&y1,sizeof(x1));cur.act-=INVOPER;}
   double y=Extract(y1,Tab),x;
   if (cur.act!=SET) x=Extract(x1,Tab);
//cout<<" x "<<x<<" y "<<y<<" ";
   x1.Type=NUM;
   if ((x==NullNumber) || (y==NullNumber))
     { x1.Val=NullNumber;if (cur.act==SET) Tab[x1.Num]=NullNumber;}
   else
     {
      switch (cur.act)
       {
        case PLUS   : x1.Val=x+y ;           //   cout<<"PLUS   ";
                      break;
        case MINUS  : x1.Val=x-y ;           //  cout<<"MINUS  ";
                      break;
        case TIMES  : x1.Val=x*y ;           //  cout<<"TIMES  ";
                      break;
        case DIVIDE : x1.Val=x/y ;           //  cout<<"DIVIDE ";
                      break;
        case POWER  : x1.Val=pow(x,y);       //  cout<<"POWER    ";
                      break;
        case SET    : Tab[x1.Num]=y;x1.Val=y;//  cout<<"SET    ";
                      break;
       }
     }
//cout<<" RES: "<<x1.Val<<"\n  ";
   push(act,a,x1);
  };
void EvalAct(Action *in,double *Tab)
  {
   Action cur,act[MaxOperation];
//   randomize();
   int a=0,i=0;
   while (in[i+1].Type!=BAD)
     {
      cur=in[++i];
      switch (cur.Type)
        {
         case OPER:reduce(act,a,cur,Tab);break;
         case FUNC:funclc(act,a,cur,Tab);break;
         case NUM :
         case CELL:push(act,a,cur);

//      cout<<Extract(cur,Tab)<<"\n";
                                     break;
        }
     }
  };
