
#ifndef __MTRND_H
#define __MTRND_H

//#include "matt_reg.h"
//#include "dat_mani.h"
#include "spl_fac.h"
#include "matrics.h"
#include "quad.h"
#include "lib/ref/ref.h"
struct RndFunc : RefCount {
    // Generate random function distributed according to the given law
    virtual double Rnd() = 0;
    virtual ~RndFunc(){};
};
// Generate a random integer number from MinNum to MaxNum
struct RndNumber : RndFunc {
    int MinNum, MaxNum;
    RndNumber(int minnum, int maxnum) : MinNum(minnum), MaxNum(maxnum) {}
    double Rnd() {
        int x = RandomClass::Rnd(MaxNum - MinNum) + 0.5;
        //	  int x=int(double(rand())/RAND_MAX*(MaxNum-MinNum)+0.5);
        return x + MinNum;
    }
};


struct RndIntegral : RndFunc {
    //  double Low,Up;
    static RndIntegral *CurRndStruct;
    double YNorm, Misfit, Norm;
    CurveSpline Int;
    RndIntegral(int SplOrder = 3) : Int(SplOrder){};
    static double xFunc(double x) {
        if(CurRndStruct)
            return CurRndStruct->Clc(x) * CurRndStruct->YNorm;
        return 0;
    }
    virtual RndIntegral *Init(double x_low, double x_up, int NumInt, double Misf);
    //  virtual double Rnd() { return X_from_Prob(double(rand())/RAND_MAX);};
    virtual double Rnd() {
        return X_from_Prob(RandomClass::Rnd(1));
    };

    virtual double X_from_Prob(double x) {
        return Int.Evaluate(x * Norm);
    };

    // Give the density of state with coordinate x
    virtual double Clc(double x) = 0;
    virtual ~RndIntegral(){};
};

struct RndGaus : RndIntegral {
    double GausDistribSigma, GausDistribZero;
    RndGaus() {
        Set(0, 0);
    };
    RndGaus(double zero, double sigma) {
        Set(zero, sigma);
    };
    void Set(double zero, double sigma) {
        GausDistribSigma = sigma;
        GausDistribZero = zero;
    }
    virtual double Clc(double x) {
        //cout<<x<<FMT<<exp(-sqr((x-GausDistribZero)/GausDistribSigma)/2)/sqrt(2*M_PI)/GausDistribSigma<<"\n";
        double dE = -sqr((x - GausDistribZero) / GausDistribSigma) / 2;
        if(dE > -40)
            return exp(dE) / sqrt(2 * M_PI) / GausDistribSigma;
        return 0;
    }
};

struct RndBoltsmanSinPower : RndIntegral {
    double BoltsmanDistribCoef, BoltsmanSinPower, BoltsmanDistribCoef2;
    //  RndBoltsmanSinPower(){Set(0,1);};
    RndBoltsmanSinPower(double BoltCoef = 0, double SinPower = 1, double BoltCoef2 = 1)
        : RndIntegral(1) {
        Set(BoltCoef, SinPower, BoltCoef2);
    };
    void Set(double BoltCoef, double SinPower, double BoltCoef2) {
        BoltsmanDistribCoef = BoltCoef;
        BoltsmanSinPower = SinPower;
        BoltsmanDistribCoef2 = BoltCoef2;
    };
    virtual double Clc(double x) {
        double r;
        x += BoltsmanDistribCoef2;
        if(BoltsmanSinPower < 0) {
            if(BoltsmanSinPower == (int)BoltsmanSinPower)
                //r=BoltsmanDistribCoef*(1+pow(cos(x*M_PI/180),-BoltsmanSinPower));
                r = BoltsmanDistribCoef * (pow(cos(x * M_PI / 180), -BoltsmanSinPower));
            else
                r = BoltsmanDistribCoef *
                    pow(fabs(cos(x * M_PI / 180)), -BoltsmanSinPower);
        } else {
            if(BoltsmanSinPower == (int)BoltsmanSinPower)
                r = BoltsmanDistribCoef * pow(sin(x * M_PI / 180), BoltsmanSinPower);
            else
                r = BoltsmanDistribCoef *
                    pow(fabs(sin(x * M_PI / 180)), BoltsmanSinPower);
        }
        //   if ( r>40) return 0;
        return exp(-r);
    }
};

//#include <vector>
struct RndBlume : RndFunc {
    VecCl ParamVal, StateTime, NormTime;   //Prob - probability of state//Prob,
    double TimeDisp, RelaxTime;
    int CurrentState;
    int TimesGenerated;
    RndGaus time;
    //  vector<RndGaus*> times;


    // prob - probability to leave the state accroding to Blume
    //        in width
    RndBlume(VecCl &values, VecCl &prob) {
        StateTime = prob;
        for(int k = 1; k <= StateTime[0]; k++)
            StateTime[k] = 1 / max<double>(StateTime[k], MathZer) / prob[0];
        ParamVal = values;

        NormTime = StateTime / (StateTime * (StateTime * 0 + 1));
        //	  Prob=StateTime/norm;// So probability to find the state accroding to Blume
        //Correct Blume normalization
        TimesGenerated = 0;
        //	  RelaxTime=1;
        RelaxTime = VecCl::ModMax(StateTime) / StateTime[0];
    }
    void SetParamVals(VecCl &values) {
        ParamVal = values;
    };
    void GenerateTimes(double gausDisp) {
        double RTime = 1;
        TimeDisp = RTime * gausDisp;
        time.Set(RTime, TimeDisp);
        time.Init(
            max<double>(RTime - 4 * TimeDisp, sqrt(MathZer)),
            RTime + 4 * TimeDisp,
            100,
            1e-5);
        //RelaxTime=StateTime*(StateTime*0+1);
        //TimeDisp=RelaxTime*gausDisp;
        //times.resize(StateTime[0]+1);
        //for (int k=1;k<=StateTime[0];k++)
        //{
        // times[k]=new RndGaus();
        // //times[k]->Set(StateTime[k],gausDisp*StateTime[k]);
        // //times[k]->Init(max<double>(StateTime[k]-4*gausDisp*StateTime[k],sqrt(MathZer)),
        // //          StateTime[k]+4*gausDisp*StateTime[k],100,1e-5);
        // times[k]->Set(StateTime[k],TimeDisp);
        // times[k]->Init(max<double>(StateTime[k]-4*TimeDisp,sqrt(MathZer)),
        //           StateTime[k]+4*TimeDisp,100,1e-5);
        //}
        TimesGenerated = 1;
    }


    double Rnd() {
        //	  double x=double(rand())/RAND_MAX*StateTime[0],s=0;
        double x = RandomClass::Rnd(StateTime[0]);
        CurrentState = (int)x + 1;
        //cout<<x<<" RndChange "<<CurrentState<<"  "<<ParamVal[CurrentState]<<"\n";
        return ParamVal[CurrentState];
    }
    double RndStart() {
        //	  double s=0,x=double(rand())/RAND_MAX;
        double s = 0, x = RandomClass::Rnd(1);
        int i = 0;
        while((x > s) && (i < NormTime[0])) {
            i++;
            s += NormTime[i];
        }
        CurrentState = i;
        //cout<<x<<" RndStart "<<CurrentState<<"  "<<ParamVal[CurrentState]<<"\n";
        return ParamVal[CurrentState];
    }

    double RndTstp() {
        if(!TimesGenerated)
            GenerateTimes(1);
        return time.Rnd() * StateTime[CurrentState];
        //	  return times[CurrentState]->Rnd();
    }

    //  ~RndBlume(){if (TimesGenerated) for (int k=1;k<=StateTime[k];k++) delete times[k];}
private:
    RndBlume(){};
};
/*
struct RndBlume
{
  VecCl ParamVal,Prob;//Prob - probability of state
  double RelaxTime,TimeDisp;
  int TimesGenerated;
  RndGaus time;


// prob - probability to leave the state accroding to Blume
//        in width 
  RndBlume(VecCl &values,VecCl &prob)
  {
	  VecCl uni=prob*0+1;
	  double norm=uni*prob;
	  ParamVal=values;Prob=uni-prob/norm;// So probability to find the state accroding to Blume
//	  if (prob[0]==1) Prob[1]=1;
//	  else {Prob=Prob/(prob[0]-1);RelaxTime=1/((prob[0]-1)*norm);}
	  Prob=Prob/(uni*Prob);
	  RelaxTime=prob[0]/norm;
//	  for (int k=1;k<=prob[0];k++) RelaxTime+=1/prob[k];RelaxTime=1/RelaxTime;
	  //Correct Blume normalization
	  TimesGenerated=0;
  }
  void GenerateTimes(double gausDisp)
  {TimeDisp=RelaxTime*gausDisp;
   time.Set(RelaxTime,TimeDisp);
   time.Init(max<double>(RelaxTime-4*TimeDisp,sqrt(MathZer)),
//	             RelaxTime+4*TimeDisp,100,1e-5*RelaxTime);
	             RelaxTime+4*TimeDisp,100,1e-5);
   TimesGenerated=1;}


  double Rnd()
  {
	  double x=double(rand())/RAND_MAX,s=0;
	  int i=0;
	  while ((x>s) && (i<Prob[0])) {i++;s+=Prob[i];}
//cout<<x<<" "<<i<<"  "<<ParamVal[i]<<"\n";
	  return ParamVal[i];
  }

  double RndTstp()
  {
	  if (!TimesGenerated) GenerateTimes(0.2);
	  return time.Rnd();
  }

private:
	RndBlume(){};
};

*/
#endif


/*
extern double GausDistribSigma;
extern double GausDistribZero;
extern double BoltsmanDistribCoef;

struct DistribStruct
 {
  CurveSpline Int;
  double Norm,YNorm;
 };



double GausDistrib(double x);
double BoltsmanDistrib(double x);


void SetRndDistrib(X_func Func,double x_low,double x_up,int NumInt,double Misf,
                   DistribStruct &Distrib);
double FindRndVal(DistribStruct &Distrib);

*/