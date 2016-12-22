//#include <alloc.h>

#include "lib/precompiled/ivl_cvt.h"

//#include "mat/dat_fac.h"
//#include "mat/spl_fac.h"
////#include "mat/spl_reg.h"
////#include "mat/dat_reg.h"
//
////#include "matlib.h"
//#include "mat/mtmin.h"
//#include "mat/zero.h"
//#include "lib/ref/data_manip.h"
//#include "urs/matt_caloric_spl.h"
//#include "lib/box_lib/box_forcelnk.h"
#include "lib/box_lib/box_face.h"
#include "lib/ref/file_manip.h"

double StndErr=1e-6;



Stroka GenerateDetailedHelp() {
    Stroka ret = Stroka("Program build time ") + __TIMESTAMP__ + "\n";
    ret += Stroka(" Registered BlackBox objects:\n\n\n") + SavableClass::HelpForCategory("BlackBox_category");
    return ret;
};


void PrintHelp(map<Stroka, Stroka> par) {
    cout<<GenerateDetailedHelp();
}



void BlackBox(map<Stroka, Stroka> par) {
    vector<Ref<SavableClass> > objVector = File::ReadConf(~par["ConfigFile"], 1, 1);
    Ref<IBlackBoxBase> blackBox = SavableClass::TestType<IBlackBoxBase>(objVector[0], "Have to define BlackBox class");
    blackBox->DoIt();
}
void Show(map<Stroka, Stroka> par) {
	vector<Stroka> points = Str::SplitLine(par["Pnt"], 0, ':');
	map<int,FILE*> pnt;
	for(size_t i = 0; i < points.size();i++)
		pnt[atoi(~points[i])] = File::open(~(par["Out"]+ "." + points[i]), "w", "can not open out file");
	FILE *in = File::open(~par["In"], "r", "no input file");
	vector<Stroka> lineV;
	Stroka timeName("CurTime"), line;
	if (par.find("TimeName") != par.end())
		timeName = par["TimeName"];
	int curPos = 0;
	double ans,  curTime = 0;
	while(File::GetLine(in, line)) {
		lineV = Str::SplitLine(line);
		if (lineV.size() < 2)
			continue;
		if (lineV[0] == timeName) {
			if (IsDouble(~lineV[1], ans, 1)) {
				curPos = 0;
				curTime = ans;
			} 
			continue;
		}
		int bad = 0;
		for(size_t i = 0; i < lineV.size(); i++)
			if (!IsDouble(~lineV[i], ans, 1))
				bad = 1;
		if (bad)
			continue;
		if (pnt.find(curPos) != pnt.end()) {
			Stroka l = Stroka(curTime, 15) + " " + line + "\n";
			fwrite(~l, sizeof(char), l.size(), pnt[curPos]);
		}
		curPos++;
	}
	File::close(in);
	for(map<int,FILE*>::iterator it = pnt.begin();it != pnt.end(); it++)
		File::close(it->second);
}


#include "lib\std\ex_out.h"
#include "lib\ref\commandline.h"


int main( int argc, const char *argv[] )
{
    SetLeakTest();
    Time_struct Time;
    try {
        NRef::TCommandParse Cmd("Usage: ivl_cvt [Params_Key_Spec] [/][-]key  \n without parameters - help\n");
        Cmd.MainHelp += GenerateDetailedHelp();
        Cmd.Add(PrintHelp, "help", "show help?", "");
        Cmd.Add(BlackBox, "blackbox", "Run black box object ", "ConfigFile par.cfg File format [ BlackBox BlackBoxObject ]");
		Cmd.Add(Show, "show", "get given points for all times from the gasd dat file;Can define TimeName - string with time definition(default is CurTime)", "In STDIN input(dat) file\nOut STDOUT base output name (point name is added)\nPnt 1:2:3 points to get");

        Cmd.SimpleRun(argc, argv);
    } CATCHMAINEXCEPTION(" ivl_cvt failed ");

    LeakTest();
    cout<<"Done in "<<Time<<"\n";
    return 0;
}



