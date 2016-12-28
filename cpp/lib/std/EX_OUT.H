
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
        Stroka Print();
        friend std::ostream& operator<<(std::ostream &output, Time_struct &time);
    };

    class PrintNumIterTime {
    private:
        long IncByNum, IterCircle, CurNum;
    public:
        long getIterCircle() const;

    private:
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
        int PrintNow();
		int PrintNow(Stroka &str);
        friend std::ostream &operator<<(std::ostream &out, PrintNumIterTime &t);
        Stroka PrintLast() {
            return Descr + "numIter " + CurNum + " time " + beg.Print();
        }
        Time_struct &GetTimer() {
            return time;
        }
		void InitTimer() {
			beg.Mark();
		}


    };


#endif

