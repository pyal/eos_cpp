#include <lib/precompiled/std.h>


#include "ex_out.h"

int InputPar(fstream &in,double &from,double &min,double &max,
                                                 double &stp,char *write) {
    int ret=0;
    in>>write>>from>>write>>min>>write>>max>>write>>stp;
    in.getline(write,255);
    in>>write>>ret>>write;
    return ret;
}



Stroka Time_struct::Print() {
    Finish = millisec();
    double elapsed_time = Finish - Start;
    Start = Finish;
    return Stroka((double)elapsed_time/1000) + " s";
}

std::ostream &operator<<(std::ostream &output, Time_struct &time) {
    output<<time.Print();
    return output;
}

int PrintNumIterTime::PrintNow() {
    if (CurNum % IterCircle == 0)
        return 1;
    return 0;
}

int PrintNumIterTime::PrintNow(Stroka &str) {
    str = "";
    if (CurNum % IterCircle == 0) {
        if (time.ElapsedTime() > 1) {
            str = Descr + "iters " + Stroka::Int2Str(IterCircle) + " / " +
                  Stroka::Int2Str(CurNum) + " [" + time.Print() + "]";
        } else {
            IterCircle += IncByNum;
        }
    }
    CurNum++;
    return str.size() > 0;
}

std::ostream &operator<<(std::ostream &out, PrintNumIterTime &t) {
    Stroka str;
    if (t.PrintNow(str)) out << str;
    return out;
}

long PrintNumIterTime::getIterCircle() const {
    return IterCircle;
}

