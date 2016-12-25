#include "deffunc.h"
#include "mem.h"

static double MinInfinFunc= 1.0;
static double InitStep = 0.1;

double InFinQunc10(X_func Func,double A,double AbsErr,double InFinZero,
                   int NumZero,long MaxFunc,int &Flag)
 
// Normal   - use X20,Y20 -> X0,Y0; Xi -> Xi+Hm
// Div step - use X0іX10,Y0іY10; mem X11іX20,Y11іY20
// Mult step - use X20,Y20 -> X0,Y0
// Normal after Div - use mem X11іX20,Y11іY20
// Last interval - use X20,Y20 -> X0,Y0
  {
   double w0=5.0/299376,w1=16067*w0,w2=106300*w0,w3=-48525*w0;
   double w4=272400*w0,w5=-260550*w0,w6=427368*w0;
   int HalfArraySize=10*sizeof(double);

   double X0,Y0,ValueH,Value2H,Sum,Toler,Hmain,h;
   double X[21],Y[21],Xr[11],Yr[11];
   int i,j,AfterDiv;
   long NumFunc;
    Sum=0.0;Hmain=InitStep;h=0.05*Hmain;X0=A;
    Y0=Func(X0);
    for (i=1;i<=20;i++) { X[i]=X0+h*i;Y[i]=Func(X[i]);}
    NumFunc=21;AfterDiv=0;j=0;
    do
      {
      Value2H=2*h*(w1*(Y0+Y[20])+w2*(Y[2]+Y[18])+w3*(Y[4]+Y[16])+
        w4*(Y[6]+Y[14])+w5*(Y[8]+Y[12])+w6*Y[10]);
      ValueH=h*(w1*(Y0+2*Y[10]+Y[20])+w2*(Y[1]+Y[9]+Y[11]+Y[19])+
        w3*(Y[2]+Y[8]+Y[12]+Y[18])+w4*(Y[3]+Y[7]+Y[13]+Y[17])+
        w5*(Y[4]+Y[6]+Y[14]+Y[16])+w6*(Y[5]+Y[15]));
      Toler=fabs(ValueH-Value2H);
      if  (Toler<=AbsErr) 
        {
          Sum=Sum+ValueH;
          if (Sum!=0.0)  if (fabs(ValueH/Sum)<=InFinZero)  
            {
              j++;
              if ((j>=NumZero) && (X0>MinInfinFunc))
                {
                  Flag=0.0;
                  return Sum;
                };
            } 
          else j=0;
          X0=X[20];
          Y0=Y[20];
          if (Toler*100<AbsErr) 
            {
              Hmain=2.0*Hmain;
              h=2.0*h;
              if (AfterDiv) 
                {
                  AfterDiv=0;
                  movmem(&Xr[1],&X[1],HalfArraySize);
                  movmem(&Yr[1],&Y[1],HalfArraySize);
                  for (i=11;i<=20;i++)
                    {
                      X[i]=X0+h*i;
                      Y[i]=Func(X[i]);
                    };
                  NumFunc+=10;
                } 
              else
                {  
                  for (i=1;i<=20;i++)
                    {
                      X[i]=X0+h*i;
                      Y[i]=Func(X[i]);
                    };
                  NumFunc+=20;
                };
            } else 
            {
              if (AfterDiv) 
                {
                  AfterDiv=0;
                  for (i=1;i<20;i++)
                    {
                      X[i]=X[i]+Hmain;
                      Y[i]=Func(X[i]);i++;
                      X[i]=Xr[i/2];
                      Y[i]=Yr[i/2];
//cout<<" i "<<i<<" i/2 "<<i/2;
                    };    
                  NumFunc+=10;
                } else
                {  
                  for (i=1;i<=20;i++)
                    {
                      X[i]=X[i]+Hmain;
                      Y[i]=Func(X[i]);
                    };
                  NumFunc+=20;
                };
            };
        } 
      else
        {
          Hmain=0.5*Hmain;
          h=0.5*h;
          AfterDiv=1;
          movmem(&X[11],&Xr[1],HalfArraySize);
          movmem(&Y[11],&Yr[1],HalfArraySize);
          for (i=10;i>=1;i--)
            {
              X[2*i]=X[i];
              Y[2*i]=Y[i];
            };
          for (i=1;i<=19;i+=2)
            {
              X[i]=X0+h*i;
              Y[i]=Func(X[i]);
            };
          NumFunc+=10;
        }
cout<<" Num "<<NumFunc;
      }
    while (NumFunc<MaxFunc);
    Flag=X[20];
    return Sum;
  };    


double InFinQulb9(X_func Func,double A,double AbsErr,double InFinZero,
                   int NumZero,long MaxFunc,int &Flag)
  {
   double H[23]={ 22,
      0.0250605011, 0.0501210023, 0.0807034301, 0.1592206341, 0.1614068602,
      0.2500000000, 0.3184412681, 0.3407793659, 0.4192965699, 0.4749394989,
      0.5000000000, 0.5250605011, 0.5807034301, 0.6592206341, 0.6815587319,
      0.7500000000, 0.8385931398, 0.8407793659, 0.9192965699, 0.9498789977,
      0.9749394989, 1.0000000000};
    double w0=0.0277777778,w1=0.1654953616,w2=0.2745387126;
    double w3=0.3464285110,w4=0.3715192744;
    int Xmove[12]=
//{       1, 2, 3, 4, 5,  6, 7,  8,  9, 10, 11 }
    { 11, 2, 0, 5, 7, 0, 11, 0, 15, 17, 20, 22 };
    int Xcalc[22]={21,
//{    1 2 3 4 5 6 7 8 9 0 }
     1,0,1,1,0,1,0,1,1,1,
     0,1,1,1,0,1,0,1,1,0,1};

    double X0,Y0,ValueH,Value2H,Sum,Toler,Hmain;
    double X[23],Y[23];
    int i,j;
    long NumFunc;

    Sum=0.0;
    Hmain=InitStep;
    X0=A;
    Y0=Func(X0);
    for (i=1;i<=22;i++) {X[i]=X0+Hmain*H[i];Y[i]=Func(X[i]);}  
    NumFunc=23;
    do
      {
      Value2H=w0*(Y0+Y[22])+w1*(Y[2]+Y[20])+w2*(Y[5]+Y[17])+
        w3*(Y[7]+Y[15])+w4*Y[11];
      Value2H=0.5*Hmain*Value2H;
      ValueH=w0*(Y0+2*Y[11]+Y[22])+w1*(Y[1]+Y[10]+Y[12]+Y[21])+
        w2*(Y[3]+Y[9]+Y[13]+Y[19])+w3*(Y[4]+Y[8]+Y[14]+Y[18])+
        w4*(Y[6]+Y[16]);
      ValueH=0.25*Hmain*ValueH;
      Toler=fabs(ValueH-Value2H);
      if  (Toler<=AbsErr) 
        {
          Sum=Sum+ValueH;
          if (Sum!=0.0) if (fabs(ValueH/Sum)<=InFinZero) 
            {
              j++;
              if ((j>=NumZero) && (X0>MinInfinFunc))
                { Flag=0.0;return Sum;  }
            } 
          else j=0;
          X0=X[22];
          Y0=Y[22];
          if (Toler*100<AbsErr) 
            {
              Hmain=2.0*Hmain;
              for (i=1;i<=22;i++)
                { X[i]=X0+Hmain*H[i];Y[i]=Func(X[i]); }
              NumFunc+=22;
            } 
          else 
            {
              for (i=1;i<=22;i++)
                { X[i]=X[i]+Hmain;Y[i]=Func(X[i]); }
              NumFunc+=22;
            }
        } 
      else
        {
          Hmain=0.5*Hmain;
          for (i=11;i>=1;i--) if (Xmove[i]!=0) 
            { X[Xmove[i]]=X[i];Y[Xmove[i]]=Y[i]; }  
          for (i=1;i<=21;i++) if (Xcalc[i]==1) 
            { X[i]=X0+Hmain*H[i];Y[i]=Func(X[i]); }
          NumFunc+=14;
        }
cout<<" While NumFunc "<<NumFunc;
      }
    while (NumFunc<MaxFunc);
    Flag=X[22];
    return Sum;
  };













double Exp(double x) {return exp(-pow(x,0.1));}
//double Exp(double x) {return exp(-(x));}

void main()
  {
   int Flag;
   double res=
          InFinQulb9(Exp,0,1e-12,1e-15,10,100000,Flag);
cout<<"\nres "<<res<<" Flag "<<Flag<<"\n";
          res=InFinQunc10(Exp,0,1e-12,1e-15,10,1000000,Flag);
cout<<" \nFlag "<<Flag<<" Result "<<res<<"\n";
  };

/*
{============================================================================
 Procedures for integrating of one-dimensional function
=============================================================================
 Input and Output variables:
  Func - function under integral
  A    - left bound
  B    - right bound
  AbsErr - absolute error of integral
  MaxFunc - maximum number of function calculation
   ( when MaxFunc reached, then integrating interrupted )
  Flag - error variable, if Flag=0 then integral is O'k, else 
    Flag point to calculated part of integral, where integrating interrupted 
 Infinity integrals:
  InFinZero - minimum value of integral at interval,while integrating not interrupted 
  NumZero - number of intervals with integral < InFinZero  
 Abbreviation of quadrature formules:
 nc* - Newton-Cotes quadrature of order *
 lb* - Lobbatto quadrature of order *
=============================================================================}

interface
uses MtType;
{$I MathDef.}

function InfinQunc8(Func:FunType;A,AbsErr,InFinZero:RealType;NumZero:byte;
  MaxFunc:longint;var Flag:RealType):RealType;
{ Adaptive infinit quadrature (Newton-Cotes 8 order) }

function InfinQunc10(Func:FunType;A,AbsErr,InFinZero:RealType;NumZero:byte;
  MaxFunc:longint;var Flag:RealType):RealType;
{ Adaptive infinit quadrature (Newton-Cotes 10 order) }

function InfinQulb9(Func:FunType;A,AbsErr,InFinZero:RealType;NumZero:byte;
  MaxFunc:longint;var Flag:RealType):RealType;
{ Adaptive infinit quadrature (Lobbato 9 order) }

function StepQunc8(Func:FunType;A,B,AbsErr:RealType;MaxFunc:longint;var Flag:RealType):RealType;
{ Adaptive quadrature at interval [A,B] (Newton-Cotes 8 order) }
  
function StepQunc10(Func:FunType;A,B,AbsErr:RealType;MaxFunc:longint;var Flag:RealType):RealType;
{ Adaptive quadrature at interval [A,B] (Newton-Cotes 10 order) }
  
function StepQulb9(Func:FunType;A,B,AbsErr:RealType;MaxFunc:longint;var Flag:RealType):RealType;
{ Adaptive quadrature at interval [A,B] (Lobbato 9 order) }

function RecInt(Func:FunType;A,B,AbsErr:RealType;IntFunc:IntFuncType):RealType;
{ Recursive quadrature at interval [A,B] (various quadrature) }
  
{ Quadrature formules of Newton-Cotes of various orders }
function QuNC2(Func:FunType;A,B:RealType):RealType;
function QuNC4(Func:FunType;A,B:RealType):RealType;
function QuNC6(Func:FunType;A,B:RealType):RealType;
function QuNC8(Func:FunType;A,B:RealType):RealType;
function QuNC10(Func:FunType;A,B:RealType):RealType;
  
const
  MinInfinFunc:RealType = 1.0;  { Value of X when calculating not interrupted }
  InitStep:RealType = 0.1;      { Initial step of integrating }

{---------------------------------------------------------------------------}
implementation

function InfinQunc8(Func:FunType;A,AbsErr,InFinZero:RealType;NumZero:byte;
  MaxFunc:longint;var Flag:RealType):RealType;
{
 Normal   - use X16,Y16 -> X0,Y0; Xi -> Xi+Hm
 Div step - use X0іX8,Y0іY8; mem X9іX16,Y9іY16
 Mult step - use X16,Y16 -> X0,Y0
 Normal after Div - use mem X9іX16,Y9іY16
 Last interval - use X16,Y16 -> X0,Y0
}   
 const
   w0=4.0/14175.0;
   w1=989*w0;
   w2=5888*w0;
   w3=-928*w0;
   w4=10496*w0;
   w5=-4540*w0;
   HalfArraySize=8*SizeOf(RealType);
  var
    X0,Y0,ValueH,Value2H,Sum,Toler,Hmain,h:RealType;
    X,Y: array [1..16] of RealType;
    Xr,Yr:array [1..8] of RealType;
    i,j:byte;
    AfterDiv:boolean;
    NumFunc:longint;
  begin
    Sum:=0.0;
    Hmain:=InitStep;
    h:=0.0625*Hmain;
    X0:=A;
    Y0:=Func(X0);
    for i:=1 to 16 do
      begin
        X[i]:=X0+h*i;
        Y[i]:=Func(X[i]);
      end;  
    NumFunc:=17;
    AfterDiv:=false;
    j:=0;
    repeat
      Value2H:=2*h*(w1*(Y0+Y[16])+w2*(Y[2]+Y[14])+w3*(Y[4]+Y[12])+
        w4*(Y[6]+Y[10])+w5*Y[8]);
      ValueH:=h*(w1*(Y0+2*Y[8]+Y[16])+w2*(Y[1]+Y[7]+Y[9]+Y[15])+
        w3*(Y[2]+Y[6]+Y[10]+Y[14])+w4*(Y[3]+Y[5]+Y[11]+Y[13])+
        w5*(Y[4]+Y[12]));
      Toler:=abs(ValueH-Value2H);
      if  (Toler<=AbsErr) then
        begin
          Sum:=Sum+ValueH;
          if (Sum<>0.0) then if (abs(ValueH/Sum)<=InFinZero) then 
            begin
              inc(j);
              if (j>=NumZero) and (X0>MinInfinFunc) then
                begin
{$IFDEF TRACE}    write('[Q',X[16]:10,' ',NumFunc,']'); {$ENDIF}              
                  Flag:=0.0;
                  InFinQunc8:=Sum;
                  exit;
                end;
            end else j:=0;
          X0:=X[16];
          Y0:=Y[16];
          if (Toler*100<AbsErr) then
            begin
              Hmain:=2.0*Hmain;
              h:=2.0*h;
              if (AfterDiv) then
                begin
                  AfterDiv:=false;
                  Move(Xr,X[1],HalfArraySize);
                  Move(Yr,Y[1],HalfArraySize);
                  for i:=9 to 16 do 
                    begin
                      X[i]:=X0+h*i;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,8);
                end else
                begin  
                  for i:=1 to 16 do 
                    begin
                      X[i]:=X0+h*i;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,16);
                end;
            end else 
            begin
              if (AfterDiv) then
                begin
                  AfterDiv:=false;
                  for i:=1 to 16 do if (odd(i)) then
                    begin
                      X[i]:=X[i]+Hmain;
                      Y[i]:=Func(X[i]);
                    end else
                    begin
                      X[i]:=Xr[i div 2];
                      Y[i]:=Yr[i div 2];
                    end;    
                  inc(NumFunc,8);
                end else
                begin  
                  for i:=1 to 16 do 
                    begin
                      X[i]:=X[i]+Hmain;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,16);
                end;
            end;
        end else
        begin
          Hmain:=0.5*Hmain;
          h:=0.5*h;
          AfterDiv:=true;
          Move(X[9],Xr,HalfArraySize);
          Move(Y[9],Yr,HalfArraySize);
          for i:=8 downto 1 do
            begin
              X[2*i]:=X[i];
              Y[2*i]:=Y[i];
            end;
          for i:=1 to 15 do if (odd(i)) then
            begin
              X[i]:=X0+h*i;
              Y[i]:=Func(X[i]);
            end;
          inc(NumFunc,8);
        end;
    until(NumFunc>=MaxFunc);
{$IFDEF TRACE}    write('[Q',X[16]:10,']'); {$ENDIF}              
{$IFDEF ERROR}    write('[QFover]'); {$ENDIF}
{$IFDEF ERRHALT}  halt(1); {$ENDIF}
    Flag:=X[16];
    InfinQunc8:=Sum;
  end;

function InFinQunc10(Func:FunType;A,AbsErr,InFinZero:RealType;NumZero:byte;
  MaxFunc:longint;var Flag:RealType):RealType;
{
 Normal   - use X20,Y20 -> X0,Y0; Xi -> Xi+Hm
 Div step - use X0іX10,Y0іY10; mem X11іX20,Y11іY20
 Mult step - use X20,Y20 -> X0,Y0
 Normal after Div - use mem X11іX20,Y11іY20
 Last interval - use X20,Y20 -> X0,Y0
}   
 const
   w0=5.0/299376;
   w1=16067*w0;
   w2=106300*w0;
   w3=-48525*w0;
   w4=272400*w0;
   w5=-260550*w0;
   w6=427368*w0;
   HalfArraySize=10*SizeOf(RealType);
  var
    X0,Y0,ValueH,Value2H,Sum,Toler,Hmain,h:RealType;
    X,Y: array [1..20] of RealType;
    Xr,Yr:array [1..10] of RealType;
    i,j:byte;
    AfterDiv:boolean;
    NumFunc:longint;
  begin
    Sum:=0.0;
    Hmain:=InitStep;
    h:=0.05*Hmain;
    X0:=A;
    Y0:=Func(X0);
    for i:=1 to 20 do
      begin
        X[i]:=X0+h*i;
        Y[i]:=Func(X[i]);
      end;  
    NumFunc:=21;
    AfterDiv:=false;
    j:=0;
    repeat
      Value2H:=2*h*(w1*(Y0+Y[20])+w2*(Y[2]+Y[18])+w3*(Y[4]+Y[16])+
        w4*(Y[6]+Y[14])+w5*(Y[8]+Y[12])+w6*Y[10]);
      ValueH:=h*(w1*(Y0+2*Y[10]+Y[20])+w2*(Y[1]+Y[9]+Y[11]+Y[19])+
        w3*(Y[2]+Y[8]+Y[12]+Y[18])+w4*(Y[3]+Y[7]+Y[13]+Y[17])+
        w5*(Y[4]+Y[6]+Y[14]+Y[16])+w6*(Y[5]+Y[15]));
      Toler:=abs(ValueH-Value2H);
      if  (Toler<=AbsErr) then
        begin
          Sum:=Sum+ValueH;
          if (Sum<>0.0) then if (abs(ValueH/Sum)<=InFinZero) then 
            begin
              inc(j);
              if (j>=NumZero) and (X0>MinInfinFunc) then
                begin
{$IFDEF TRACE}    write('[Q',X[20]:10,' ',NumFunc,']'); {$ENDIF}              
                  Flag:=0.0;
                  InFinQunc10:=Sum;
                  exit;
                end;
            end else j:=0;
          X0:=X[20];
          Y0:=Y[20];
          if (Toler*100<AbsErr) then
            begin
              Hmain:=2.0*Hmain;
              h:=2.0*h;
              if (AfterDiv) then
                begin
                  AfterDiv:=false;
                  Move(Xr,X[1],HalfArraySize);
                  Move(Yr,Y[1],HalfArraySize);
                  for i:=11 to 20 do 
                    begin
                      X[i]:=X0+h*i;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,10);
                end else
                begin  
                  for i:=1 to 20 do 
                    begin
                      X[i]:=X0+h*i;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,20);
                end;
            end else 
            begin
              if (AfterDiv) then
                begin
                  AfterDiv:=false;
                  for i:=1 to 20 do if (odd(i)) then
                    begin
                      X[i]:=X[i]+Hmain;
                      Y[i]:=Func(X[i]);
                    end else
                    begin
                      X[i]:=Xr[i div 2];
                      Y[i]:=Yr[i div 2];
                    end;    
                  inc(NumFunc,10);
                end else
                begin  
                  for i:=1 to 20 do 
                    begin
                      X[i]:=X[i]+Hmain;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,20);
                end;
            end;
        end else
        begin
          Hmain:=0.5*Hmain;
          h:=0.5*h;
          AfterDiv:=true;
          Move(X[11],Xr,HalfArraySize);
          Move(Y[11],Yr,HalfArraySize);
          for i:=10 downto 1 do
            begin
              X[2*i]:=X[i];
              Y[2*i]:=Y[i];
            end;
          for i:=1 to 19 do if (odd(i)) then
            begin
              X[i]:=X0+h*i;
              Y[i]:=Func(X[i]);
            end;
          inc(NumFunc,10);
        end;
    until(NumFunc>=MaxFunc);
{$IFDEF TRACE}    write('[Q',X[20]:10,']'); {$ENDIF}              
{$IFDEF ERROR}    write('[QFover]'); {$ENDIF}
{$IFDEF ERRHALT}  halt(1); {$ENDIF}
    Flag:=X[20];
    InfinQunc10:=Sum;
  end;

function StepQunc8(Func:FunType;A,B,AbsErr:RealType;MaxFunc:longint;var Flag:RealType):RealType;
{
 Normal   - use X16,Y16 -> X0,Y0; Xi -> Xi+Hm
 Div step - use X0іX8,Y0іY8; mem X9іX16,Y9іY16
 Mult step - use X16,Y16 -> X0,Y0
 Normal after Div - use mem X9іX16,Y9іY16
 Last interval - use X16,Y16 -> X0,Y0
}   
 const
   w0=4.0/14175.0;
   w1=989*w0;
   w2=5888*w0;
   w3=-928*w0;
   w4=10496*w0;
   w5=-4540*w0;
   HalfArraySize=8*SizeOf(RealType);
  var
    X0,Y0,ValueH,Value2H,Sum,Toler,Hmain,h:RealType;
    X,Y: array [1..16] of RealType;
    Xr,Yr:array [1..8] of RealType;
    i:byte;
    AfterDiv:boolean;
    NumFunc:longint;
  begin
    Sum:=0.0;
    Hmain:=InitStep*(B-A);
    h:=0.0625*Hmain;
    X0:=A;
    Y0:=Func(X0);
    for i:=1 to 16 do
      begin
        X[i]:=X0+h*i;
        Y[i]:=Func(X[i]);
      end;  
    NumFunc:=17;
    AfterDiv:=false;
    repeat
      Value2H:=2*h*(w1*(Y0+Y[16])+w2*(Y[2]+Y[14])+w3*(Y[4]+Y[12])+
        w4*(Y[6]+Y[10])+w5*Y[8]);
      ValueH:=h*(w1*(Y0+2*Y[8]+Y[16])+w2*(Y[1]+Y[7]+Y[9]+Y[15])+
        w3*(Y[2]+Y[6]+Y[10]+Y[14])+w4*(Y[3]+Y[5]+Y[11]+Y[13])+
        w5*(Y[4]+Y[12]));
      Toler:=abs(ValueH-Value2H);
      if  (Toler<=AbsErr) then
        begin
          Sum:=Sum+ValueH;
          X0:=X[16];
          Y0:=Y[16];
          if (X0>=B) then
            begin
{$IFDEF TRACE}    write('[Q',X[16]:10,' ',NumFunc,']'); {$ENDIF}              
              Flag:=0.0;
              StepQunc8:=Sum;
              exit;
            end;  
          if (Toler*100<AbsErr) then
            begin
              Hmain:=2.0*Hmain;
              h:=2.0*h;
              if ((X0+HMain)>B) then
                begin
                  Hmain:=B-X0;
                  h:=0.0625*Hmain;
                  AfterDiv:=false;
                end;  
              if (AfterDiv) then
                begin
                  AfterDiv:=false;
                  Move(Xr,X[1],HalfArraySize);
                  Move(Yr,Y[1],HalfArraySize);
                  for i:=9 to 16 do 
                    begin
                      X[i]:=X0+h*i;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,8);
                end else
                begin  
                  for i:=1 to 16 do 
                    begin
                      X[i]:=X0+h*i;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,16);
                end;
            end else 
            begin
              if ((X0+HMain)>B) then
                begin
                  Hmain:=B-X0;
                  h:=0.0625*Hmain;
                  for i:=1 to 16 do 
                    begin
                      X[i]:=X0+h*i;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,16);
                  AfterDiv:=false;
                end else  
              if (AfterDiv) then
                begin
                  AfterDiv:=false;
                  for i:=1 to 16 do if (odd(i)) then
                    begin
                      X[i]:=X[i]+Hmain;
                      Y[i]:=Func(X[i]);
                    end else
                    begin
                      X[i]:=Xr[i div 2];
                      Y[i]:=Yr[i div 2];
                    end;    
                  inc(NumFunc,8);
                end else
                begin  
                  for i:=1 to 16 do 
                    begin
                      X[i]:=X[i]+Hmain;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,16);
                end;
            end;
        end else
        begin
          Hmain:=0.5*Hmain;
          h:=0.5*h;
          AfterDiv:=true;
          Move(X[9],Xr,HalfArraySize);
          Move(Y[9],Yr,HalfArraySize);
          for i:=8 downto 1 do
            begin
              X[2*i]:=X[i];
              Y[2*i]:=Y[i];
            end;
          for i:=1 to 15 do if (odd(i)) then
            begin
              X[i]:=X0+h*i;
              Y[i]:=Func(X[i]);
            end;
          inc(NumFunc,8);
        end;
    until(NumFunc>=MaxFunc);
{$IFDEF TRACE}    write('[Q',X[16]:10,']'); {$ENDIF}              
{$IFDEF ERROR}    write('[QFover]'); {$ENDIF}
{$IFDEF ERRHALT}  halt(1); {$ENDIF}
    Flag:=X[16];
    StepQunc8:=Sum;
  end;

function StepQunc10(Func:FunType;A,B,AbsErr:RealType;MaxFunc:longint;var Flag:RealType):RealType;
 const
   w0=5.0/299376;
   w1=16067*w0;
   w2=106300*w0;
   w3=-48525*w0;
   w4=272400*w0;
   w5=-260550*w0;
   w6=427368*w0;
   HalfArraySize=10*SizeOf(RealType);
  var
    X0,Y0,ValueH,Value2H,Sum,Toler,Hmain,h:RealType;
    X,Y: array [1..20] of RealType;
    Xr,Yr:array [1..10] of RealType;
    i:byte;
    AfterDiv:boolean;
    NumFunc:longint;
  begin
    Sum:=0.0;
    Hmain:=InitStep*(B-A);
    h:=0.05*Hmain;
    X0:=A;
    Y0:=Func(X0);
    for i:=1 to 20 do
      begin
        X[i]:=X0+h*i;
        Y[i]:=Func(X[i]);
      end;  
    NumFunc:=21;
    AfterDiv:=false;
    repeat
      Value2H:=2*h*(w1*(Y0+Y[20])+w2*(Y[2]+Y[18])+w3*(Y[4]+Y[16])+
        w4*(Y[6]+Y[14])+w5*(Y[8]+Y[12])+w6*Y[10]);
      ValueH:=h*(w1*(Y0+2*Y[10]+Y[20])+w2*(Y[1]+Y[9]+Y[11]+Y[19])+
        w3*(Y[2]+Y[8]+Y[12]+Y[18])+w4*(Y[3]+Y[7]+Y[13]+Y[17])+
        w5*(Y[4]+Y[6]+Y[14]+Y[16])+w6*(Y[5]+Y[15]));
      Toler:=abs(ValueH-Value2H);
      if  (Toler<=AbsErr) then
        begin
          Sum:=Sum+ValueH;
          X0:=X[20];
          Y0:=Y[20];
          if (X0>=B) then
            begin
{$IFDEF TRACE}    write('[Q',X[20]:10,' ',NumFunc,']'); {$ENDIF}              
              Flag:=0.0;
              StepQunc10:=Sum;
              exit;
            end;  
          if (Toler*100<AbsErr) then
            begin
              Hmain:=2.0*Hmain;
              h:=2.0*h;
              if ((X0+HMain)>B) then
                begin
                  Hmain:=B-X0;
                  h:=0.05*Hmain;
                  AfterDiv:=false;
                end;  
              if (AfterDiv) then
                begin
                  AfterDiv:=false;
                  Move(Xr,X[1],HalfArraySize);
                  Move(Yr,Y[1],HalfArraySize);
                  for i:=11 to 20 do 
                    begin
                      X[i]:=X0+h*i;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,10);
                end else
                begin  
                  for i:=1 to 20 do 
                    begin
                      X[i]:=X0+h*i;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,20);
                end;
            end else 
            begin
              if ((X0+HMain)>B) then
                begin
                  Hmain:=B-X0;
                  h:=0.05*Hmain;
                  for i:=1 to 20 do 
                    begin
                      X[i]:=X0+h*i;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,20);
                  AfterDiv:=false;
                end else  
              if (AfterDiv) then
                begin
                  AfterDiv:=false;
                  for i:=1 to 20 do if (odd(i)) then
                    begin
                      X[i]:=X[i]+Hmain;
                      Y[i]:=Func(X[i]);
                    end else
                    begin
                      X[i]:=Xr[i div 2];
                      Y[i]:=Yr[i div 2];
                    end;    
                  inc(NumFunc,10);
                end else
                begin  
                  for i:=1 to 20 do 
                    begin
                      X[i]:=X[i]+Hmain;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,20);
                end;
            end;
        end else
        begin
          Hmain:=0.5*Hmain;
          h:=0.5*h;
          AfterDiv:=true;
          Move(X[11],Xr,HalfArraySize);
          Move(Y[11],Yr,HalfArraySize);
          for i:=10 downto 1 do
            begin
              X[2*i]:=X[i];
              Y[2*i]:=Y[i];
            end;
          for i:=1 to 19 do if (odd(i)) then
            begin
              X[i]:=X0+h*i;
              Y[i]:=Func(X[i]);
            end;
          inc(NumFunc,10);
        end;
    until(NumFunc>=MaxFunc);
{$IFDEF TRACE}    write('[Q',X[20]:10,']'); {$ENDIF}              
{$IFDEF ERROR}    write('[QFover]'); {$ENDIF}
{$IFDEF ERRHALT}  halt(1); {$ENDIF}
    Flag:=X[20];
    StepQunc10:=Sum;
  end;

function QuNC2(Func:FunType;A,B:RealType):RealType;
 const
   P=2;
   w0=1.0/3.0;
   w1=1*w0;
   w2=4*w0;
 var
   h:RealType;
  begin
    h:=(B-A)/P;
    QuNc2:=h*(w1*(Func(A)+Func(B))+w2*Func(A+h));
  end;

function QuNC4(Func:FunType;A,B:RealType):RealType;
 const
   P=4;
   w0=2.0/45.0;
   w1=7*w0;
   w2=32*w0;
   w3=12*w0;
 var
   h:RealType;
  begin
    h:=(B-A)/P;
    QuNc4:=h*(w1*(Func(A)+Func(B))+w2*(Func(A+h)+Func(A+3*h))+
     w3*Func(A+2*h));
  end;

function QuNC6(Func:FunType;A,B:RealType):RealType;
 const
   P=6;
   w0=1.0/140.0;
   w1=41*w0;
   w2=216*w0;
   w3=27*w0;
   w4=272*w0;
 var
   h:RealType;
  begin
    h:=(B-A)/P;
    QuNc6:=h*(w1*(Func(A)+Func(B))+w2*(Func(A+h)+Func(A+5*h))+
     w3*(Func(A+2*h)+Func(A+4*h))+w4*Func(A+3*h));
  end;

function QuNC8(Func:FunType;A,B:RealType):RealType;
 const
   P=8;
   w0=4.0/14175.0;
   w1=989*w0;
   w2=5888*w0;
   w3=-928*w0;
   w4=10496*w0;
   w5=-4540*w0;
 var
   h:RealType;
  begin
    h:=(B-A)/P;
    QuNc8:=h*(w1*(Func(A)+Func(B))+w2*(Func(A+h)+Func(A+7*h))+
     w3*(Func(A+2*h)+Func(A+6*h))+w4*(Func(A+3*h)+Func(A+5*h))+
     w5*Func(A+4*h));
  end;

function QuNC10(Func:FunType;A,B:RealType):RealType;
 const
   P=10;
   w0=5.0/299376;
   w1=16067*w0;
   w2=106300*w0;
   w3=-48525*w0;
   w4=272400*w0;
   w5=-260550*w0;
   w6=427368*w0;
 var
   h:RealType;
  begin
    h:=(B-A)/P;
    QuNc10:=h*(w1*(Func(A)+Func(B))+w2*(Func(A+h)+Func(A+9*h))+
     w3*(Func(A+2*h)+Func(A+8*h))+w4*(Func(A+3*h)+Func(A+7*h))+
     w5*(Func(A+4*h)+Func(A+6*h))+w6*Func(A+5*h));
  end;

function RecInt(Func:FunType;A,B,AbsErr:RealType;IntFunc:IntFuncType):RealType;
  var
    Int0,Int1,Int2,Mid,Toler:RealType;
  begin
    Mid:=0.5*(A+B);
    Int0:=IntFunc(Func,A,B);
    Int1:=IntFunc(Func,A,Mid);
    Int2:=IntFunc(Func,Mid,B);
    Toler:=(Int1+Int2)-Int0;
    if (abs(Toler)<=AbsErr) then RecInt:=Int1+Int2 else  
      begin
        Int1:=RecInt(Func,A,Mid,AbsErr,IntFunc);
        Int2:=RecInt(Func,Mid,B,AbsErr,IntFunc);
        RecInt:=Int1+Int2;
      end;
  end;
    
function StepQulb9(Func:FunType;A,B,AbsErr:RealType;MaxFunc:longint;var Flag:RealType):RealType;
  const
    H: array [1..22] of RealType = 
    ( 0.0250605011, 0.0501210023, 0.0807034301, 0.1592206341, 0.1614068602,
      0.2500000000, 0.3184412681, 0.3407793659, 0.4192965699, 0.4749394989,
      0.5000000000, 0.5250605011, 0.5807034301, 0.6592206341, 0.6815587319,
      0.7500000000, 0.8385931398, 0.8407793659, 0.9192965699, 0.9498789977,
      0.9749394989, 1.0000000000);
    w0=0.0277777778;
    w1=0.1654953616;
    w2=0.2745387126;
    w3=0.3464285110;
    w4=0.3715192744;
    Xmove: array[1..11] of byte = (
{    1, 2, 3, 4, 5,  6, 7,  8,  9, 10, 11 }
     2, 0, 5, 7, 0, 11, 0, 15, 17, 20, 22 );
    Xcalc: array[1..21] of byte =
{    1 2 3 4 5 6 7 8 9 0 }
    (1,0,1,1,0,1,0,1,1,1,
     0,1,1,1,0,1,0,1,1,0,1);
  var
    X0,Y0,ValueH,Value2H,Sum,Toler,Hmain:RealType;
    X,Y: array [1..22] of RealType;
    i:byte;
    NumFunc:longint;
  begin
    Sum:=0.0;
    Hmain:=InitStep*(B-A);
    X0:=A;
    Y0:=Func(X0);
    for i:=1 to 22 do
      begin
        X[i]:=X0+Hmain*H[i];
        Y[i]:=Func(X[i]);
      end;  
    NumFunc:=23;
    repeat
      Value2H:=w0*(Y0+Y[22])+w1*(Y[2]+Y[20])+w2*(Y[5]+Y[17])+
        w3*(Y[7]+Y[15])+w4*Y[11];
      Value2H:=0.5*Hmain*Value2H;
      ValueH:=w0*(Y0+2*Y[11]+Y[22])+w1*(Y[1]+Y[10]+Y[12]+Y[21])+
        w2*(Y[3]+Y[9]+Y[13]+Y[19])+w3*(Y[4]+Y[8]+Y[14]+Y[18])+
        w4*(Y[6]+Y[16]);
      ValueH:=0.25*Hmain*ValueH;
      Toler:=abs(ValueH-Value2H);
      if  (Toler<=AbsErr) then
        begin
          Sum:=Sum+ValueH;
          X0:=X[22];
          Y0:=Y[22];
          if (X0>=B) then
            begin
{$IFDEF TRACE}    write('[Q',X[22]:10,' ',NumFunc,']'); {$ENDIF}              
              Flag:=0.0;
              StepQulb9:=Sum;
              exit;
            end;  
          if (Toler*100<AbsErr) then
            begin
              Hmain:=2.0*Hmain;
              if ((X0+HMain)>B) then Hmain:=B-X0;
              for i:=1 to 22 do 
                begin
                  X[i]:=X0+Hmain*H[i];
                  Y[i]:=Func(X[i]);
                end;
              inc(NumFunc,22);
            end else 
            begin
              if ((X0+HMain)>B) then
                begin
                  Hmain:=B-X0;
                  for i:=1 to 22 do 
                    begin
                      X[i]:=X0+Hmain*H[i];
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,22);
                end else  
                begin  
                  for i:=1 to 22 do 
                    begin
                      X[i]:=X[i]+Hmain;
                      Y[i]:=Func(X[i]);
                    end;
                  inc(NumFunc,22);
                end;
            end;
        end else
        begin
          Hmain:=0.5*Hmain;
          for i:=11 downto 1 do if (Xmove[i]<>0) then
            begin
              X[Xmove[i]]:=X[i];
              Y[Xmove[i]]:=Y[i];
            end;  
          for i:=1 to 21 do if (Xcalc[i]=1) then
            begin
              X[i]:=X0+Hmain*H[i];
              Y[i]:=Func(X[i]);
            end;
          inc(NumFunc,14);
        end;
    until(NumFunc>=MaxFunc);
{$IFDEF TRACE}    write('[Q',X[22]:10,']'); {$ENDIF}              
{$IFDEF ERROR}    write('[QFover]'); {$ENDIF}
{$IFDEF ERRHALT}  halt(1); {$ENDIF}
    Flag:=X[22];
    StepQulb9:=Sum;
  end;

function InfinQulb9(Func:FunType;A,AbsErr,InFinZero:RealType;NumZero:byte;
  MaxFunc:longint;var Flag:RealType):RealType;
  const
    H: array [1..22] of RealType = 
    ( 0.0250605011, 0.0501210023, 0.0807034301, 0.1592206341, 0.1614068602,
      0.2500000000, 0.3184412681, 0.3407793659, 0.4192965699, 0.4749394989,
      0.5000000000, 0.5250605011, 0.5807034301, 0.6592206341, 0.6815587319,
      0.7500000000, 0.8385931398, 0.8407793659, 0.9192965699, 0.9498789977,
      0.9749394989, 1.0000000000);
    w0=0.0277777778;
    w1=0.1654953616;
    w2=0.2745387126;
    w3=0.3464285110;
    w4=0.3715192744;
    Xmove: array[1..11] of byte = (
{    1, 2, 3, 4, 5,  6, 7,  8,  9, 10, 11 }
     2, 0, 5, 7, 0, 11, 0, 15, 17, 20, 22 );
    Xcalc: array[1..21] of byte =
{    1 2 3 4 5 6 7 8 9 0 }
    (1,0,1,1,0,1,0,1,1,1,
     0,1,1,1,0,1,0,1,1,0,1);
  var
    X0,Y0,ValueH,Value2H,Sum,Toler,Hmain:RealType;
    X,Y: array [1..22] of RealType;
    i,j:byte;
    NumFunc:longint;
  begin
    Sum:=0.0;
    Hmain:=InitStep;
    X0:=A;
    Y0:=Func(X0);
    for i:=1 to 22 do
      begin
        X[i]:=X0+Hmain*H[i];
        Y[i]:=Func(X[i]);
      end;  
    NumFunc:=23;
    repeat
      Value2H:=w0*(Y0+Y[22])+w1*(Y[2]+Y[20])+w2*(Y[5]+Y[17])+
        w3*(Y[7]+Y[15])+w4*Y[11];
      Value2H:=0.5*Hmain*Value2H;
      ValueH:=w0*(Y0+2*Y[11]+Y[22])+w1*(Y[1]+Y[10]+Y[12]+Y[21])+
        w2*(Y[3]+Y[9]+Y[13]+Y[19])+w3*(Y[4]+Y[8]+Y[14]+Y[18])+
        w4*(Y[6]+Y[16]);
      ValueH:=0.25*Hmain*ValueH;
      Toler:=abs(ValueH-Value2H);
      if  (Toler<=AbsErr) then
        begin
          Sum:=Sum+ValueH;
          if (Sum<>0.0) then if (abs(ValueH/Sum)<=InFinZero) then 
            begin
              inc(j);
              if (j>=NumZero) and (X0>MinInfinFunc) then
                begin
{$IFDEF TRACE}    write('[Q',X[16]:10,' ',NumFunc,']'); {$ENDIF}              
                  Flag:=0.0;
                  InfinQulb9:=Sum;
                  exit;
                end;
            end else j:=0;
          X0:=X[22];
          Y0:=Y[22];
          if (Toler*100<AbsErr) then
            begin
              Hmain:=2.0*Hmain;
              for i:=1 to 22 do 
                begin
                  X[i]:=X0+Hmain*H[i];
                  Y[i]:=Func(X[i]);
                end;
              inc(NumFunc,22);
            end else 
            begin
              for i:=1 to 22 do 
                begin
                  X[i]:=X[i]+Hmain;
                  Y[i]:=Func(X[i]);
                end;
              inc(NumFunc,22);
            end;
        end else
        begin
          Hmain:=0.5*Hmain;
          for i:=11 downto 1 do if (Xmove[i]<>0) then
            begin
              X[Xmove[i]]:=X[i];
              Y[Xmove[i]]:=Y[i];
            end;  
          for i:=1 to 21 do if (Xcalc[i]=1) then
            begin
              X[i]:=X0+Hmain*H[i];
              Y[i]:=Func(X[i]);
            end;
          inc(NumFunc,14);
        end;
    until(NumFunc>=MaxFunc);
{$IFDEF TRACE}    write('[Q',X[22]:10,']'); {$ENDIF}              
{$IFDEF ERROR}    write('[QFover]'); {$ENDIF}
{$IFDEF ERRHALT}  halt(1); {$ENDIF}
    Flag:=X[22];
    InfinQulb9:=Sum;
  end;

end.




*/
