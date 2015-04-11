
#define _WIN32_WINNT 0x0500

#include <Windows.h>

#include "lib\ref\commandline.h"

int main(int argc, const char *argv[]){
    SetLeakTest();

	struct MemoryFormat{
		MemoryFormat():OutT(Used),OutS(Physical)
		{
			DivFactor[0] = 1;DivFactor[1] = 1024;DivFactor[2] = 1048576;
			DivName[0] = 'b';DivName[1] = 'k';   DivName[2] = 'm';
			OutTypeName[0] = "Total";OutTypeName[1] = "Avail"; OutTypeName[2] = "Used";
			OutSourceName[0] = "Physical"; OutSourceName[1] = "PageFile";OutSourceName[2] = "Virtual";
			SetDiv('m');
		};
		int DivFactor[3];// = (1,1024,1048576);
		char DivName[3];//  = ('b','k','m');
		int Div;

		MemoryFormat& SetDiv(char name){
			for (int k=0;k<sizeof(DivName)/sizeof(char);k++) 
				if (DivName[k]==name)
					Div = k;
			return *this;
		}
		int PercentUsed;

		enum OutType  { Total,Avail,Used };
		char *OutTypeName[3];// = ( "Total","Avail","Used" );
		enum OutSource  { Physical,PageFile,Virtual };
		char *OutSourceName[3];// = ( "Physical","PageFile","Virtual" );
		OutType OutT;
		OutSource OutS;
		MemoryFormat& SetFormat(OutType out_t,OutSource out_s,int percent){
			OutT=out_t;OutS = out_s;PercentUsed = percent;return *this;
		}
		void Out(ostream &out){
			MEMORYSTATUSEX stat;
			stat.dwLength = sizeof (stat);
			GlobalMemoryStatusEx(&stat);
			if (PercentUsed){
				out<<"Memory used percent "<<stat.dwMemoryLoad<<"\n";
				return ;
			}
			DWORDLONG *ptr = &stat.ullTotalPhys;
			int posR = OutS*2;
			int posA = OutT;
			out<<OutSourceName[OutS]<<" memory "<<OutTypeName[OutT]<<": ";
			if (posA!=2)
				out<<int(ptr[posR+posA]/DivFactor[Div]);
			else
				out<<int((ptr[posR]-ptr[posR+1])/DivFactor[Div]);
			out<<" "<<DivName[Div]<<"\n";
			return ;
		}
	};
    try {
        CommandLineExe Cmd("Usage: memory [Params_Key_Spec] [/][-]key  \n without parameters - standart memory\n");
        Cmd.Add('h',"PrintHelp"," - print this help \n");
        Cmd.Add('p',"Percent"," - print percent of phys mem used \n");
		Cmd.Add('t',"Type"," - Type to output(0,1,2): Total,Avail,Used \n");
		Cmd.Add('s',"Source"," - Source to output(0,1,2):Physical,PageFile,Virtual\n");
		Cmd.Add('f',"Format"," - Format to output(b,k,m):Byte,Kbyte,Mbyte\n");
//        Cmd.Add('a',"AllResults"," - not to delete good results \n");
//        Cmd.MainHelp += GenerateDetailedHelp();

        Cmd.Analize(argc, argv);
        if ( Cmd.Get("PrintHelp").Exist){
            cout<<Cmd.MakeHelp();
            return 0;
        } 
		MemoryFormat fmt;
		MemoryFormat::OutType OutT = MemoryFormat::Used;
		MemoryFormat::OutSource OutS = MemoryFormat::Physical;
		int Percent=0;
		if (Cmd.Get("Format").Exist) fmt.SetDiv(Cmd.Get("Format").Val.c_str()[0]);
		if (Cmd.Get("Type").Exist) OutT = (MemoryFormat::OutType)atoi(Cmd.Get("Format").Val.c_str());
		if (Cmd.Get("Source").Exist) OutS = (MemoryFormat::OutSource)atoi(Cmd.Get("Source").Val.c_str());
		if (Cmd.Get("Percent").Exist) Percent = 1;
		fmt.SetFormat(OutT,OutS,Percent).Out(cout);




    }
    CATCHEXCEPTION(" Memory failed ?????");
    //LeakTest();
    return 0;
}