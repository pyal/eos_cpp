#include <lib\precompiled\eos.h>

#include "matt_internal.h"


static ClassDesc Matter2State_ClassDesc(typeid(Matter2State),"Matter2State",1,"Matter_category",create<Matter2State>);
static ClassDesc MatterSum_ClassDesc(typeid(MatterSum),"MatterSum",1,"Matter_category",create<MatterSum>);
static ClassDesc Matter2StateProb_ClassDesc(typeid(Matter2StateProb),"Matter2StateProb",1,"Matter_category",create<Matter2StateProb>);









//
//void Matter2State::Pressure(double *P,double *Dencity,double *Energy,int Num)
//		{
//			double *P1=new double[Num+2];
//			double *P2=new double[Num+2];
//			int n=1,k;
//			double d1,d2,alpha;
//			M1->Pressure(P1,Dencity,Energy,Num);
//			M2->Pressure(P2,Dencity,Energy,Num);
//			for ( k=PosIntPar;k<Num+PosIntPar;k++)
//					{
//						alpha=IntParArr[k][0];
//						if (alpha==0) P[n]=P1[n];
//						else if (alpha==1) P[n]=P2[n];
//						     else { ClcDenc(Dencity[n],Energy[n],alpha,d1,d2);
//												      P[n]=(P1[n]+alpha*P2[n])/(1+alpha);}
//					 n++;
//					}
//			delete P1;delete P2;
//		};			
//void MatterSum::Sound(double *S,double *Dencity,double *Energy,int Num)
//		{
//			double *S1=new double[Num+2];
//			double *S2=new double[Num+2];
//			int n=1,k;
//			double d1,d2,alpha;
//			M1->Pressure(S1,Dencity,Energy,Num);
//			M2->Pressure(S2,Dencity,Energy,Num);
//			for ( k=PosIntPar;k<Num+PosIntPar;k++)
//					{
//						alpha=IntParArr[k][0];
//						if (alpha==0) S[n]=S1[n];
//						else if (alpha==1) S[n]=S2[n];
//						     else { ClcDenc(Dencity[n],Energy[n],alpha,d1,d2);
//												      S[n]=(S1[n]+alpha*S2[n])/(1+alpha);}
//					 n++;
//					}
//			delete S1;delete S2;
//		};	
Matter2State::Matter2State()
    :MatterIO(),M1(new MatterABu),M2(new MatterABu),TimeToWait(0.1),Pres_ElseDenc(0){
        SetInternalNames();
    };
		
double Matter2State::Pressure(double Dencity,double Energy){
    CheckInput();
	if (InputValStorage["TransTime"]>=0) 
        return M1->Pressure(Dencity,Energy);
    return M2->Pressure(Dencity,Energy);
};
double Matter2State::Sound(double Dencity,double Energy){
    CheckInput();
	if (InputValStorage["TransTime"]>=0) 
        return M1->Sound(Dencity,Energy);
    return M2->Sound(Dencity,Energy);
};

double Matter2State::Temperature(double Dencity,double Energy){
    CheckInput();
	if (InputValStorage["TransTime"]>=0) 
        return M1->Temperature(Dencity,Energy);
    return M2->Temperature(Dencity,Energy);
};
void Matter2State::ClcArray(double *P,double *Dencity,double *Energy,int Num, const Stroka &What){
    vector<double> D1,D2,E1,E2,P1,P2;
    D1.reserve(Num);D2.reserve(Num);E1.reserve(Num);E2.reserve(Num);
	for (int k=1;k<=Num;k++)
		if (InputVectorStorage["TransTime"][k]>=0){
		    D1.push_back(Dencity[k]);E1.push_back(Energy[k]);
		} else {
		    D2.push_back(Dencity[k]);E2.push_back(Energy[k]);
		}
	P1.resize(D1.size());P2.resize(D2.size());
	//double *p1 = &P1[0],*p2 = &P2[0], *d1 = &D1[0], *d2 = &D2[0], *e1 = &E1[0], *e2 = &E2[0];
	//p1--;p2--;e1--;e2--;d1--;d2--;
	double *p1 = NULL,*p2 = NULL, *d1 = NULL, *d2 = NULL, *e1 = NULL, *e2 = NULL;
    if (D1.size()!=0){
        p1 = &P1[0]; d1 = &D1[0];e1 = &E1[0];
        p1--;e1--;d1--;
    }
    if (D2.size()!=0){
        p2 = &P2[0];d2 = &D2[0];e2 = &E2[0];
        p2--;e2--;d2--;
    }
    if (What=="Pressure"){
	    M1->Pressure(p1,d1,e1,(int)D1.size());
	    M2->Pressure(p2,d2,e2,(int)D2.size());
    } else if (What=="Sound"){
	    M1->Sound(p1,d1,e1,(int)D1.size());
	    M2->Sound(p2,d2,e2,(int)D2.size());
    } else if (What=="Temperature"){
	    M1->Temperature(p1,d1,e1,(int)D1.size());
	    M2->Temperature(p2,d2,e2,(int)D2.size());
    } else
        throw info_except("Do not know how to calculate:<%s>\n",What.c_str());
	int i1=0,i2=0;
	for (int k=1;k<=Num;k++)
		if (InputVectorStorage["TransTime"][k]>=0){
		    P[k] = P1[i1];i1++;
		} else {
		    P[k] = P2[i2];i2++;
		}
};
void Matter2State::Pressure(double *P,double *Dencity,double *Energy,int Num){
 //   vector<double> D1,D2,E1,E2,P1,P2;
 //   D1.reserve(Num);D2.reserve(Num);E1.reserve(Num);E2.reserve(Num);
	//for (int k=1;k<=Num;k++)
	//	if (InputVectorStorage["TransTime"][k]>=0){
	//	    D1.push_back(Dencity[k]);E1.push_back(Energy[k]);
	//	} else {
	//	    D2.push_back(Dencity[k]);E2.push_back(Energy[k]);
	//	}
	//P1.resize(D1.size());P2.resize(D2.size());
	//double *p1 = &P1[0],*p2 = &P2[0], *d1 = &D1[0], *d2 = &D2[0], *e1 = &E1[0], *e2 = &E2[0];
	//p1--;p2--;e1--;e2--;d1--;d2--;
	//M1->Pressure(p1,d1,e1,(int)D1.size());
	//M2->Pressure(p2,d2,e2,(int)D2.size());
	//int i1=0,i2=0;
	//for (int k=1;k<=Num;k++)
	//	if (InputVectorStorage["TransTime"][k]>=0){
	//	    P[k] = P1[i1];i1++;
	//	} else {
	//	    P[k] = P2[i2];i2++;
	//	}
    ClcArray(P,Dencity,Energy,Num,"Pressure");
};			
void Matter2State::Sound(double *S,double *Dencity,double *Energy,int Num){
    ClcArray(S,Dencity,Energy,Num,"Sound");
};			
void Matter2State::Temperature(double *T,double *Dencity,double *Energy,int Num){
    ClcArray(T,Dencity,Energy,Num,"Temperature");
};			
void Matter2State::ChangeInputVal(double *Dencity,double *Energy,
                                           int Num,double Time){
	double *P=new double[Num+1];
//double a,a0;
//a0 = InputVectorStorage["TransTime"][1];
    if (Pres_ElseDenc) Pressure(P,Dencity,Energy,Num);
    else MemoryMove(Dencity,P,sizeof(double)*(Num+1));
	for (int k=1;k<=Num;k++){
        if (InputVectorStorage["TransTime"][k]>=0)
		    if (P[k]>MinP) InputVectorStorage["TransTime"][k]+=Time;
            else InputVectorStorage["TransTime"][k] = M_MinDouble;
        else
		    if (P[k]<MinP) InputVectorStorage["TransTime"][k]-=Time;
            else InputVectorStorage["TransTime"][k] = -M_MinDouble;

//a = InputVectorStorage["TransTime"][k];
        if (fabs(InputVectorStorage["TransTime"][k])>TimeToWait)
            InputVectorStorage["TransTime"][k]=MTSign(InputVectorStorage["TransTime"][k])*(-M_MinDouble);
//double b = InputVectorStorage["TransTime"][k];
//if (a!=b)
    //cout<<k<<"   "<<a<<" "<<b<<"\n";
    }
	delete P;
//cout<<a0<<" "<<InputVectorStorage["TransTime"][1]<<"\n";
};

// ============================================================
// =============      MatterSum       =========================
// ============================================================

MatterSum::MatterSum()
    :MatterIO(), M1(new MatterABu), M2(new MatterABu), VolProp12(1), MinP(0.5), Pres_Denc(1){
        SetInternalNames();
};
//    MatterSum():MatterIO(){VolProp12=1;SetInternalNames();}

void MatterSum::ChangeInputVal(double *Dencity,double *Energy,
                                           int Num,double Time){
	double *P=new double[Num+1];
    if (Pres_Denc) Pressure(P,Dencity,Energy,Num);
    else MemoryMove(Dencity,P,sizeof(double)*(Num+1));
	for (int k=1;k<=Num;k++)
		if (InputVectorStorage["State"][k]==0) 
		    if (P[k]>MinP) InputVectorStorage["State"][k]=1;
	delete P;
			//double *P=new double[Num+1];
   //if (Pres_Denc) Pressure(P,Dencity,Energy,Num);
   //else MemoryMove(Dencity,P,sizeof(double)*(Num+1));
			//for (int k=PosIntPar;k<Num+PosIntPar;k++)
			//			if (IntParArr[k][0]==0) if (P[k+1-PosIntPar]>MinP) IntParArr[k][0]=1;
			//delete P;
};
void MatterSum::Pressure(double *P,double *Dencity,double *Energy,int Num){
	double *P1=new double[Num+2];
	double *P2=new double[Num+2];
	int n=1,k;
	double d1,d2,alpha;
	M1->Pressure(P1,Dencity,Energy,Num);
	M2->Pressure(P2,Dencity,Energy,Num);
	//for ( k=PosIntPar;k<Num+PosIntPar;k++){
	//	alpha=IntParArr[k][0];
	for ( k=1;k<=Num;k++){
		alpha=InputVectorStorage["State"][k];
		if (alpha==0) P[n]=P1[n];
		else if (alpha==1) P[n]=P2[n];
				else { ClcDenc(Dencity[n],Energy[n],alpha,d1,d2);
										P[n]=(P1[n]+alpha*P2[n])/(1+alpha);}
		n++;
	}
	delete P1;delete P2;
};			
void MatterSum::Sound(double *S,double *Dencity,double *Energy,int Num){
	double *S1=new double[Num+2];
	double *S2=new double[Num+2];
	int n=1,k;
	double d1,d2,alpha;
	M1->Pressure(S1,Dencity,Energy,Num);
	M2->Pressure(S2,Dencity,Energy,Num);
	//for ( k=PosIntPar;k<Num+PosIntPar;k++){
	//	alpha=IntParArr[k][0];
	for ( k=1;k<=Num;k++){
		alpha=InputVectorStorage["State"][k];
		if (alpha==0) S[n]=S1[n];
		else if (alpha==1) S[n]=S2[n];
				else { ClcDenc(Dencity[n],Energy[n],alpha,d1,d2);
										S[n]=(1-alpha)*S1[n]+alpha*S2[n];}
		n++;
	}
	delete S1;delete S2;
};			
double MatterSum::Pressure(double Dencity,double Energy){
	double alpha=InputValStorage["State"];
//cout<<"alpha "<<alpha<<" Dencity "<<Dencity<<" Energy  "<<Energy<<"\n";
	if (alpha==0) return M1->Pressure(Dencity,Energy);
	if (alpha==1) return M2->Pressure(Dencity,Energy);
	double d1,d2;
	ClcDenc(Dencity,Energy,alpha,d1,d2);
	return (M1->Pressure(d1,Energy)+
			alpha*M2->Pressure(d2,Energy))/(1+alpha);
};
double MatterSum::Sound(double Dencity,double Energy){
	double alpha=InputValStorage["State"];
//cout<<"alphaS "<<alpha<<" Dencity "<<Dencity<<" Energy  "<<Energy<<"\n";
	if (alpha==0) return M1->Sound(Dencity,Energy);
	if (alpha==1) return M2->Sound(Dencity,Energy);
	double d1,d2;
	ClcDenc(Dencity,Energy,alpha,d1,d2);
	return (M1->Sound(d1,Energy)+
			alpha*M2->Sound(d2,Energy))/(1+alpha);
};

double MatterSum::Temperature(double Dencity,double Energy){
	double alpha=InputValStorage["State"];
	if (alpha==0) return M1->Temperature(Dencity,Energy);
	if (alpha==1) return M2->Temperature(Dencity,Energy);
	double d1,d2;
	ClcDenc(Dencity,Energy,alpha,d1,d2);
	return (M1->Temperature(d1,Energy)+
			alpha*M2->Temperature(d2,Energy))/(1+alpha);
};

//=================================================================================
//=========================          Matter2StateProb         =====================
//=================================================================================


Matter2StateProb::Matter2StateProb()
    :MatterIO(),M1(new MatterABu),M2(new MatterABu),TransDenc(0.1),Prob0(0.1),Prob1(0.1){
        SetInternalNames();
    };
		
double Matter2StateProb::Pressure(double Dencity,double Energy){
    CheckInput();
	if (InputValStorage["TransTime"]>=0) 
        return M1->Pressure(Dencity,Energy);
    return M2->Pressure(Dencity,Energy);
};
double Matter2StateProb::Sound(double Dencity,double Energy){
    CheckInput();
	if (InputValStorage["TransTime"]>=0) 
        return M1->Sound(Dencity,Energy);
    return M2->Sound(Dencity,Energy);
};

double Matter2StateProb::Temperature(double Dencity,double Energy){
    CheckInput();
	if (InputValStorage["TransTime"]>=0) 
        return M1->Temperature(Dencity,Energy);
    return M2->Temperature(Dencity,Energy);
};
void Matter2StateProb::ClcArray(double *P,double *Dencity,double *Energy,int Num, const Stroka &What){
    vector<double> D1,D2,E1,E2,P1,P2;
    D1.reserve(Num);D2.reserve(Num);E1.reserve(Num);E2.reserve(Num);
	for (int k=1;k<=Num;k++)
		if (InputVectorStorage["TransTime"][k]>=0){
		    D1.push_back(Dencity[k]);E1.push_back(Energy[k]);
		} else {
		    D2.push_back(Dencity[k]);E2.push_back(Energy[k]);
		}
	P1.resize(D1.size());P2.resize(D2.size());
	double *p1 = &P1[0],*p2 = &P2[0], *d1 = &D1[0], *d2 = &D2[0], *e1 = &E1[0], *e2 = &E2[0];
	p1--;p2--;e1--;e2--;d1--;d2--;
    if (What=="Pressure"){
	    M1->Pressure(p1,d1,e1,(int)D1.size());
	    M2->Pressure(p2,d2,e2,(int)D2.size());
    } else if (What=="Sound"){
	    M1->Sound(p1,d1,e1,(int)D1.size());
	    M2->Sound(p2,d2,e2,(int)D2.size());
    } else if (What=="Temperature"){
	    M1->Temperature(p1,d1,e1,(int)D1.size());
	    M2->Temperature(p2,d2,e2,(int)D2.size());
    } else
        throw info_except("Do not know how to calculate:<%s>\n",What.c_str());
	int i1=0,i2=0;
	for (int k=1;k<=Num;k++)
		if (InputVectorStorage["TransTime"][k]>=0){
		    P[k] = P1[i1];i1++;
		} else {
		    P[k] = P2[i2];i2++;
		}
};
double Matter2StateProb::ClcDissCoef(double *Dencity,int Num){
    if (Num==0)
        return 0.5;
    int NumMin=0, NumPl=0;
	for (int k=1;k<=Num;k++)
        if (InputValStorage["TransTime"]>=0)
            NumPl++;
        else
            NumMin++;
    return NumPl/(NumMin+NumPl);
}
void Matter2StateProb::ChangeInputVal(double Dencity,double Energy,double Time){ 
    CheckInput(); 
    if (InputValStorage["TransTime"]>=0)
        if (Dencity>TransDenc) InputValStorage["TransTime"]+=Time;
        else InputValStorage["TransTime"] = M_MinDouble;
    else
        if (Dencity<TransDenc) InputValStorage["TransTime"]-=Time;
        else InputValStorage["TransTime"] = -M_MinDouble;
    if (fabs(InputValStorage["TransTime"])>M_MinDouble*2)
        if (RandomClass::Rnd(1)<Time*Prob0)
            InputValStorage["TransTime"]=MTSign(InputValStorage["TransTime"])*(-M_MinDouble);
}; 
void Matter2StateProb::ChangeInputVal(double *Dencity,double *Energy,
                                           int Num,double Time){
	double *P=new double[Num+1];
    double Dis = ClcDissCoef(Dencity,Num);
	for (int k=1;k<=Num;k++){
        if (InputVectorStorage["TransTime"][k]>=0)
            if (Dencity[k]>TransDenc) InputVectorStorage["TransTime"][k]+=Time;
            else InputVectorStorage["TransTime"][k] = M_MinDouble;
        else
            if (Dencity[k]<TransDenc) InputVectorStorage["TransTime"][k]-=Time;
            else InputVectorStorage["TransTime"][k] = -M_MinDouble;

       if (fabs(InputVectorStorage["TransTime"][k])>M_MinDouble*2){
            double d = InputVectorStorage["TransTime"][k]>0?Dis:1-Dis;
            if (RandomClass::Rnd(1)<Time*(Prob0+d*Prob1))
                InputVectorStorage["TransTime"][k]=MTSign(InputVectorStorage["TransTime"][k])*(-M_MinDouble);
       }
    }
	delete P;
};



