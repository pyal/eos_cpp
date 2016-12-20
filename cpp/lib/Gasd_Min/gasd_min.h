

#ifndef __GASD_MIN_H
#define __GASD_MIN_H

#include "messb\fastd.h"
#include "urs\fre_ros.h"
#include "urs\fre_multi.h"

//=====================================================================
//=======================  FAZerColdExp6Curve   =======================
//=====================================================================


struct FAZerColdExp6Curve:FAZerBase{
	FAZerColdExp6Curve();
   void   ClcBase(double *par,double *Xval,double *Res);
   void OutputInternPar(ostream &output){
	   if (!SavableClass::Save(output,FreeE) ) 
           throw info_except(" Could not save matter\n");
       output<<"\n";
   };
   void InputInternPar(istream &input){
	   MatterIO *Matter=dynamic_cast<MatterIO*>(SavableClass::Read(input));
       if ( !Matter ) 
           throw info_except(" Could not register matter\n");
//       delete FreeE;
        FreeE = dynamic_cast<MatterFreeE*>(Matter);
       if ( !FreeE ) throw info_except(" Could not convert matter to freeE\n");
       //if ( !dynamic_cast<FreeERossH2_Minim*>(FreeE->FreeEPtr)) 
       //    throw info_except(" Could not get FreeERossH2_Minim from FreeEMatter\n");;
       FreeESumMatter *Check;
       if ( !(Check=dynamic_cast<FreeESumMatter*>((InterfaceFreeEIO*)FreeE->FreeEPtr)) ) 
           throw info_except(" Could not get FreeESumMatter from FreeEMatter\n");
	   if ( !(dynamic_cast<FreeERossH2_Minim*>((InterfaceFreeEIO*)Check->FreeVect[0])) )
           throw info_except(" First matter in FreeESumMatter is not FreeERossH2_Minim\n");
   };
   //FreeERossH2 *Mat;
   Ref<MatterFreeE> FreeE;
};
// Cannot to be used directly, for usage under FAZerCurveMain only....
struct FAZerCurveBase:FAZerBase{
	FAZerCurveBase():FAZerBase(){};
   ~FAZerCurveBase(){};
   virtual Ref<SavableClass> GenerateVar(double *par)=0;
   void   ClcBase(double *par,double *Xval,double *Res){};// Not used...
   virtual Stroka MakeHelp() {
	   Stroka str = DataSource::GenerateUnicName("tmp");
	   FilterTextOut tmp(str.c_str(),DataSource::Memory);
       tmp.SetRefMask(SavableClass::SimpleEdit);
	   DrawHelp(tmp);
	   return Stroka( DataSource::GetStr(str.c_str()).c_str() );
   };
   //void OutputInternPar(ostream &output){};
   //void InputInternPar(istream &input){};
};
struct FAZerCurveMatterSum_0_H2Minim:FAZerCurveBase{
	FAZerCurveMatterSum_0_H2Minim();
   virtual Ref<SavableClass> GenerateVar(double *par);
   void OutputInternPar(ostream &output);
   void InputInternPar(istream &input);
//   ~FAZerCurveMatterSum_0_H2Minim(){};
protected: 
	Ref<MatterFreeE> FreeE;
};
struct FAZerCurveMatterSum_FreeERoss:FAZerCurveBase{
	FAZerCurveMatterSum_FreeERoss();
   virtual Ref<SavableClass> GenerateVar(double *par);
   void OutputInternPar(ostream &output);
   void InputInternPar(istream &input);
protected: 
	Ref<MatterFreeE> FreeE;
};

#include "urs\test\urs_curve.h"
//#include "lib/ref/class_sav.h"


struct FAZerCurveMain:FAZerBase{
	FAZerCurveMain():FAZerBase(),XVarName("MinVar.X"),YVarName("MinVar.Y"),UrsVarName("UrsVar"){
		name = strdup("FAZerCurveMain");
		Curve=new URS_Curve();
		Stroka SubX, SubY, VarName = Curve->MakeNames(XVarName, SubX);
        Curve->MakeNames(YVarName, SubY);
        UrsCurve_VarStorage* varStorage = new UrsCurve_VarStorage();
        varStorage->SetVar(~SubX, NULL);
        varStorage->SetVar(~SubY, NULL);
		Curve->SetVar(VarName, varStorage);
        Curve->SetOutput(new UrsCurve_Output2Store(~XVarName, ~YVarName));
		VarConstructor = new FAZerCurveMatterSum_FreeERoss();
        NumPar = VarConstructor->GetNumPar();
		for (int k=1;k<=NumPar;k++)
			namepar[k] = strdup(VarConstructor->namepar[k]);
	};
   void   ClcBase(double *par,double *Xval,double *Res){
		Ref<SavableClass> cl_var = VarConstructor->GenerateVar(par);
		Curve->SetVar(UrsVarName.c_str(), cl_var);

		Ref<ResizeVector<double> > X = new ResizeVector<double>();

        Stroka SubName,VarName = Curve->MakeNames(XVarName, SubName);
		UrsCurve_VarStorage *stor = dynamic_cast<UrsCurve_VarStorage*>((SavableClass*)Curve->GetVar(VarName));
		if (!stor) 
			throw info_except("variable %s is not of UrsCurve_VarStorage type...\n",XVarName.c_str());
		X->SetArr(&Xval[1],0,Xval[0]);
		stor->SetVar(SubName.c_str(),X);
        Curve->SetNumIter(Xval[0]);
		Curve->Do();

		VarName = Curve->MakeNames(YVarName, SubName);
		stor = dynamic_cast<UrsCurve_VarStorage*>((SavableClass*)Curve->GetVar(VarName));
		if (!stor) 
			throw info_except("variable %s is not of UrsCurve_VarStorage type...\n",YVarName.c_str());
		X = stor->GetVar(SubName.c_str());
		X->GetArr(&Res[1],0,Xval[0]);
		Res[0] = Xval[0];
   };
   void OutputInternPar(ostream &output){
        FilterTextOut out(output.rdbuf(),0);
        out.SetRefMask(SavableClass::SimpleEdit);
        if (!out)
            throw info_except("Could not read input config \n");
		out<<" XVarName "<<XVarName<<" YVarName "<<YVarName<<" UrsVarName "<<UrsVarName<<" Curve \n";out<<Curve<<"\nInternalClassName "<<VarConstructor->class_name()<<"\n";
        VarConstructor->OutputInternPar(out);
   };
   void InputInternPar(istream &input){
        FilterTextIn in(input.rdbuf(),0);
        in.SetRefMask(SavableClass::SimpleEdit);
        if (!in)
            throw info_except("Could not read input config \n");

        Stroka ClassName;
		char tmp[256];
        in>>tmp>>XVarName>>tmp>>YVarName>>tmp>>UrsVarName>>tmp;
        in>>Curve>>tmp>>ClassName;
        ClassDesc *desc = ClassDesc::name_to_class_desc(ClassName.c_str());
        if (!desc)
            throw info_except("Could not find class named %s\n", ClassName.c_str());
        VarConstructor = dynamic_cast<FAZerCurveBase*>(desc->create());
        if (!VarConstructor)
            throw info_except("Could not create class named %s (may be not FAZerCurveBase base class?)\n", ClassName.c_str());
        VarConstructor->InputInternPar(input);
	    for(int k=1;k<=NumPar;k++)
			delete namepar[k];
        NumPar = VarConstructor->GetNumPar();
		for (int k=1;k<=NumPar;k++)
			namepar[k] = strdup(VarConstructor->namepar[k]);


        
        //fcout.SetRefMask(SavableClass::UnlinkedObjects);
        //fcout<<curve;
        //Curve->Do();

   };
   Ref<FAZerCurveBase> VarConstructor;
   Ref<URS_Curve> Curve;
   Stroka XVarName, YVarName, UrsVarName;
   virtual int HelpPresent(){return 0;}
   virtual void DrawHelp(ostream &out){
		out<<" Envelope for minimizing InternalClassName Functions. Functions - generate SavableClass object, which is "<<
			"passed to Urs_Curve object(by UrsVarName) as well as X data points(by XVar name). Urs_Curve generate Y curve"<<
			"which is minimized. Urs_Curve object have not to be defined in Urs_Curve, X Y - are defined. " <<
            "The output is formed by UrsCurve_Output2Store (not std URS_Curve::Output). Use Ground=0 Int=-1 - to get the result\n";
		out<<"In the system currently present InternalClassName Functions:\n";
		out<<SavableClass::HelpForCategory("FAZerCurveBase_category").c_str()<<"\n";
		//ExEnv::set_out(&out);
		//ClassDesc::list_all_classes("FAZerCurveBase_category");
		//FilterTextOut tmp1(out.rdbuf(),0);
		//tmp1.SetRefMask(SavableClass::UnlinkedObjects);
		//ClassDesc::WriteHelpOnCategory(tmp1, "FAZerCurveBase_category", ClassDesc::PrintHelp);
		//ExEnv::set_out(&cout);
   };

};



#endif


		//FAZer *func=dynamic_cast<FAZer*>(tmp);
		//DescribedClass* tmp=ClassDesc::list_category_start("FAZerCurveBase_category");
		//while (func) {
		//	func->DrawHelp(out);
		//	tmp=ClassDesc::list_category_next(func,"FAZerCurveBase_category",1);
		//	func=dynamic_cast<FAZer*>(tmp);
		//	out<<"\n\n";
		//}
