

#include "lib/ref/commandline.h"
#include "lib/std/ex_out.h"
#include "qtplot.h"

void Show(map<Stroka, Stroka> par) {
    log_always("Show");
    vector<vector<double>> pnts;
    vector<string> names;
    QtData::IDataSource *dataSource = (atoi(~par["gasd"]) == 0) ?
                                      (QtData::IDataSource *)new MultiFile(par["file"]) :
                                      (QtData::IDataSource *)new GasdData(par["file"], "CurTime", par["CurTimeFirst"] != "0");
    QtPlot plot(dataSource);
    plot.Plot();
}

int main(int argc, const char *argv[]) {
    Time_struct Time;
    try {
        NRef::TCommandParse Cmd(
                "Usage: poly_test  - without parameters - help\n");
        Cmd.Add(Show, "show", "plot file", "file in.dat plot file\ngasd 0 Gasd clc file\nCurTimeFirst 0  0 - wilkins format, 1 - kru format)", 1);
        Cmd.SimpleRun(argc, argv);
    }
    CATCHMAINEXCEPTION(string(argv[0]) + " failed ");

    LeakTest();
    cout << "Done in " << Time << "\n";
    return 0;
}
