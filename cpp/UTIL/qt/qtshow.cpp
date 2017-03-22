

#include "lib/ref/commandline.h"
#include "lib/std/ex_out.h"
#include "qtplot.h"

void Show(map<Stroka, Stroka> par) {
    log_always("Show");
    vector<vector<double>> pnts;
    vector<string> names;
    char fieldDelim(' ');
    if (par["FieldDelim"] != "SPACE") fieldDelim = par["FieldDelim"][0];
    bool oneLineToMany = (par["OneLineToMany"] == "1");
    log_debug(Stroka("Running args ") +
      Str::Vec2Str(Str::Map2Vec(par, [](const Stroka &key, const Stroka &val)->Stroka {return key + Stroka(":") + val;})));
    QtData::IDataSource *dataSource = (atoi(~par["gasd"]) == 0) ?
      (QtData::IDataSource *)new MultiFile(par["file"], fieldDelim, oneLineToMany) :
      (QtData::IDataSource *)new GasdData(par["file"], "CurTime", par["CurTimeFirst"] != "0");
    QtPlot plot(dataSource);
    plot.Plot();
}

int main(int argc, const char *argv[]) {
    Time_struct Time;
        NRef::TCommandParse Cmd(
                "Usage: poly_test  - without parameters - help\n");
        Cmd.Add(Show, "show", "plot file", "file in.dat plot file\ngasd 0 Gasd clc file\nCurTimeFirst 0  0 - wilkins format, 1 - kru format)\nFieldDelim SPACE\nOneLineToMany 1 If X point is less then previous - start new line ", 1);
        Cmd.SimpleRun(argc, argv);

    LeakTest();
    cout << "Done in " << Time << "\n";
    return 0;
}
