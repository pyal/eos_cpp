

#pragma once

#include "lib\ref\class_sav.h"
#include "lib\ref\stroka.h"



// Not a real savable class....
struct FindSubstanceIO:virtual SavableClass{
	FindSubstanceIO():FileName("material.cfg"),Substance("Steel-Tern"){};
	FindSubstanceIO(const char *file,const char *subst):FileName(file),Substance(subst){};
    Stroka FileName,Substance;
	FilterTextIn ReadBuf;
	FilterTextOut WriteBuf;
	int  SetInput(FilterIn *in) {
		in->OpenBuf(DataSource::GenerateUnicName(FileName.c_str()).c_str(),1,DataSource::Disk,0,FileName.c_str());
		char tmp[1024];
		while (!(!(*in)) ){
			Stroka Sub;
			(*in)>>Sub;
  			if (Sub==Substance) 
				break;
			in->getline(tmp,1024);
		}
		return !(!(*in));
	}
	void SetOutput(FilterOut *out) {
		fstream O(FileName.c_str(),ios::in|ios::out);
		char tmp[1024];
		while (!seek_eof(O)){
			Stroka Sub;
			O>>Sub;
  			if (Sub==Substance) 
				throw info_except("Substance %s already present in file %s",Substance.c_str(),FileName.c_str());
			O.getline(tmp,1024);
		}
		out->SetNewBuf(O.rdbuf(),1);
		O<<Substance<<" ";
	}

	FilterIn* GetReadStream(){
      if (!SetInput(&ReadBuf))
		  throw info_except("Bad Substance-Matter;\nSubstance:%s FileName:%s\n", Substance.c_str(),FileName.c_str());
      return &ReadBuf;
	}
	FilterOut* GetOutStream(){
      SetOutput(&WriteBuf);
      return &WriteBuf;
	}
	virtual int read_data_state(FilterIn &input){
		input>>FileName>>FileName;
		input>>Substance>>Substance;
		return !(!input);
	};
	virtual int save_data_state( FilterOut &output){output<<" FileName "<<FileName<<" Substance "<<Substance<<" ";return !(!output);}


};

