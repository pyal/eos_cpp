
#pragma once


#include "urs_curve.h"
#include "util/code_gen/calculator/calculator_std.h"

//
////==============================================================================================
////============================      UrsCurve_FileReader      ===================================
////==============================================================================================
struct UrsCurve_Calculator:URS_Curve::ClcVar{

    UrsCurve_Calculator();//:URS_Curve::ClcVar(),DefaultValue(0){};
    virtual Ref<SavableClass> GetRes(const Stroka &SubName){
		CheckStp(SubName, this);
        for (int k=0;k<OutputCurveNames.SetNumEl();k++)
            if (SubName==OutputCurveNames[k])
                return new SavableDouble(
                *(double*)DataSimpleStorage::GetData( prg.GetVar(OutputClcNames[k])->dat ));
		throw info_except("Cannot calculate variable %s\n",SubName.c_str());
	}
    virtual void ClcStart(URS_Curve* Data){
        URS_Curve::ClcVar::ClcStart(Data);
        strstream ins((char*)Program.c_str(),(streamsize)Program.length(),ios::in);
        prg.ResetProgram(new Lex_Calcul_Analyser(&ins));
        CalculatorProgam::GetCurPtr(&prg);
        for(int k=0;k<InputCurveNames.SetNumEl();k++)
            prg.GenerateDouble(DefaultValue, InputClcNames[k].c_str());
        for(int k=0;k<OutputCurveNames.SetNumEl();k++)
            if (prg.GetVar(OutputClcNames[k])==0)
                prg.GenerateDouble(DefaultValue, OutputClcNames[k].c_str());
        prg.Read();
        CalculatorProgam::RestorePtr();
    };
    virtual void ClcEnd(){};

    virtual int save_data_state( FilterOut&so) { 
        so<<"Input_CurveName_ClcName { ";
        for(int k=0;k<InputCurveNames.SetNumEl();k++)
            so<<InputCurveNames[k].c_str()<<" "<<InputClcNames[k].c_str()<<" ; ";
        so<<" } Output_CurveName_ClcName { ";
        for(int k=0;k<OutputCurveNames.SetNumEl();k++)
            so<<OutputCurveNames[k].c_str()<<" "<<OutputClcNames[k].c_str()<<" ; ";
        so<<" } DefaultValue "<<DefaultValue<<" Program { "<<Program.c_str()<<" } ";


        return 1; 
    };
	virtual int read_data_state(FilterIn&si){ 
        char tmp[256];
        si>>tmp>>tmp;
        InputCurveNames.Release(); InputClcNames.Release();
        OutputCurveNames.Release();OutputClcNames.Release();
        si>>tmp;
        while (stricmp(tmp,"}")!=0){
            InputCurveNames.push(Stroka(tmp));si>>tmp;
            InputClcNames.push(Stroka(tmp));si>>tmp;
            if (stricmp(tmp,";")!=0)
                throw info_except(" Could not read name, wrong delimiter %s\n",tmp);
            si>>tmp;
        }
        si>>tmp>>tmp;
        if (stricmp(tmp,"{")!=0)
            throw info_except(" Could not read name, wrong delimiter %s - have to be { \n",tmp);
        si>>tmp;
        while (stricmp(tmp,"}")!=0){
            OutputCurveNames.push(Stroka(tmp));si>>tmp;
            OutputClcNames.push(Stroka(tmp));si>>tmp;
            if (stricmp(tmp,";")!=0)
                throw info_except(" Could not read name, wrong delimiter %s - have to be ; \n",tmp);
            si>>tmp;
        }
        si>>tmp>>DefaultValue>>tmp>>tmp;
        si>>tmp;
        while (stricmp(tmp,"}")!=0){
            Program +=Stroka(tmp)+Stroka(" ");
            si>>tmp;
        }
        return 1; 
    };
	virtual Stroka MakeHelp(){
        Stroka res = "Class for calculation by formula\n";
		Ref<UrsCurve_Calculator> form = new UrsCurve_Calculator();
        form->InputCurveNames.push(Stroka("X_curve"));
        form->InputClcNames.push(Stroka("X"));
        form->OutputCurveNames.push(Stroka("Y_curve"));
        form->OutputClcNames.push(Stroka("Y"));
        form->DefaultValue = 0;
        form->Program = Stroka(" Y=X; ");
        res += SavableClass::object2string(form);
        return res;
	}
protected:
	//virtual void MakeHelp(FilterTextOut &out){
	//	out<<"Class for calculation by formula\n";
	//	Ref<UrsCurve_Calculator> form = new UrsCurve_Calculator();
 //       form->InputCurveNames.push(Stroka("X_curve"));
 //       form->InputClcNames.push(Stroka("X"));
 //       form->OutputCurveNames.push(Stroka("Y_curve"));
 //       form->OutputClcNames.push(Stroka("Y"));
 //       form->DefaultValue = 0;
 //       form->Program = Stroka(" Y=X; ");
 //       out<<form;
	//}
	virtual void ClcNewStp(int CurStp, const Stroka &SubName){
        if (!FirstVarStpClc(this))
            return;
        CalculatorProgam::GetCurPtr(&prg);
        for (int k=0;k<InputClcNames.SetNumEl();k++)
            *(double*)DataSimpleStorage::GetData( prg.GetVar(InputClcNames[k])->dat ) = 
            GetVar(InputCurveNames[k]);
        prg.Execute();
        CalculatorProgam::RestorePtr();
    };

private:

    ResizeVector<Stroka> InputCurveNames,InputClcNames;
    ResizeVector<Stroka> OutputCurveNames,OutputClcNames;
    Stroka Program;
//    ResizeVector<double> CurLineIn, CurLineOut;
    double DefaultValue;
    CalculatorProgam prg;
};

