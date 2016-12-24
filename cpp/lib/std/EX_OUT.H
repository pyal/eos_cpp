
#ifndef __EX_OUT_H
#define __EX_OUT_H  


#include "util.h"
#include "lib/ref/stroka.h"

    class Time_struct {
    private:
        long Start, Finish;
    public:
        long millisec() {
            return clock() * 1000 / CLOCKS_PER_SEC;
        }
        Time_struct() {
            Start = millisec();
        }
        void Mark() {
            Start = millisec();
        }
        double ElapsedTime() {
            return (millisec()-Start)/1000.;
        }
        Stroka Print() {
            Finish = millisec();
            double elapsed_time = Finish - Start;
            Start = Finish;
            char buf[1024];
            sprintf(buf, " RunTime: %f in seconds %f ", elapsed_time, ((double)elapsed_time/1000));
            return Stroka(buf);
        }
        friend std::ostream& operator<<(std::ostream &output, Time_struct &time) {
            output<<time.Print().c_str();
            return output;
        };
    };
    class PrintNumIterTime {

    private:
        long IncByNum, IterCircle, CurNum;
        Time_struct time,beg;
        Stroka Descr;

    public:
        PrintNumIterTime(const char *descr, long incbynum=10)
            : IncByNum(incbynum)
            , IterCircle(incbynum)
            ,CurNum(0)
            ,Descr(descr) {
        };
        PrintNumIterTime(long incbynum=10)
            : IncByNum(incbynum)
            , IterCircle(incbynum)
            , CurNum(0)
            , Descr("") {
        };
        int PrintNow() {
            if (CurNum % IterCircle == 0)
                return 1;
            return 0;
        }
		int PrintNow(Stroka &str) {
			str = "";
            if (CurNum % IterCircle == 0) {
                if (time.ElapsedTime() > 1) {
                    str = Descr + " Iters " + Stroka::Int2Str(CurNum) + "\t" +
                          Stroka::Int2Str(IterCircle) + "\t done in " +
                          time.Print() + "\n";
                } else {
                    IterCircle += IncByNum;
                }
            }
            CurNum++;
			return str.size() > 0;
		}
        friend std::ostream &operator<<(std::ostream &out, PrintNumIterTime &t) {
			Stroka str;
			if (t.PrintNow(str))
				out << str.c_str();
			return out;
            //if (t.CurNum % t.IterCircle == 0)
            //    if (t.time.ElapsedTime() > 1) {
            //        out<<t.Descr.c_str()<<" Iters "<<t.CurNum<<"\t"<<t.IterCircle<<"\t done in "<<t.time<<"\n";
            //    } else
            //        t.IterCircle += t.IncByNum;
            //t.CurNum++;
            //return out;
        }
        Stroka PrintLast() {
            char buf[1024];
            sprintf(buf,"%s NumIter %i done in %s", Descr.c_str(), (int)CurNum, beg.Print().c_str());
            return Stroka(buf);
        }
        Time_struct &GetTimer() {
            return time;
        }
		void InitTimer() {
			beg.Mark();
		}


    };


//struct Time_struct
//  {
//#if defined WCPP || defined WCPP_NET
//// double elapsed_time;
//   clock_t start, finish;
//   Time_struct() {start=clock();}
//   void Mark(){start=clock();}
//   friend ostream& operator<<(ostream &output,Time_struct &time)
//     {
//      time.finish=clock();
//	  double elapsed_time=time.finish-time.start;
//      time.start=time.finish;
//      output<<" RunTime: "<<elapsed_time<<" in seconds: "<<((double)elapsed_time/ CLOCKS_PER_SEC) ;
//      return output;
//     };
//#else
//   Time_struct() {};
//   void Mark(){};
//   friend ostream& operator<<(ostream &output,Time_struct &time)
//     { output<<" RunTime 0 ";return output; };
//#endif
//  };
//
//int InputPar(fstream &in,double &from,double &min,double &max,
//                                                 double &stp,char *write);
//


#endif

