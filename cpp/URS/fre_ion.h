#pragma once

#include "fre_fac.h"
//#include "mat\NdimMin.h"
//#include "mat\fun_fac.h"
#include "fre_dis.h"
#include "lib\ref\data_manip.h"

namespace ClcIonisation {

    struct ElectronStub : RefCount {
// denc = mol/cm3
        ElectronStub(): MolVeight(1), FreeEl(new FreeEIdeal) {
        }
        void Init(Ref<InterfaceFreeEIO> fel, double molvei){
            MolVeight = molvei;
            FreeEl = fel;
            Dencity = MathZer;
            Temperature = -1;
            DeriveConst = M_Eps2;
        }
        InterfaceFreeEIO *GetElectronMatter(){
            return FreeEl;
        }
        virtual double ClcFreeE(double denc, double temp){
            CheckParams(denc, temp);
            return FreeE;
        }
        virtual double ClcFreeEDeriv(double denc, double temp){
            CheckParams(denc, temp);
            return FreeEder;
        }
        double MolVeight, DeriveConst;
    private:
        void CheckParams(double denc, double temp){
            if (denc<2*MathZer)
                denc = 2*MathZer;
            if (fabs(log(denc / Dencity)) + fabs(temp-Temperature) < 0.1 * StndErr)
                return ;
            Dencity = denc;
            Temperature = temp;
            double d = max(Dencity, MathZer) * MolVeight, t = max(Temperature, MathZer);
            FreeE = FreeEl->FreeE(d, t) * MolVeight;
            FreeEder = (FreeEl->FreeE(d * (1 + 0.5 * DeriveConst), t) - FreeEl->FreeE(d * (1 - 0.5 * DeriveConst), t)) / (d * DeriveConst);
            FreeEder *= sqr(MolVeight);
        }
        Ref<InterfaceFreeEIO> FreeEl;
        double Dencity, Temperature;
        double FreeE, FreeEder;
    };

    struct FreeEIonStdIO : FreeEIOFind {
        FreeEIonStdIO() : FreeEIOFind(), MolVeight(1){
            IonPot.push_back(0);IonPot.push_back(10);
            Gfactor.push_back(1); Gfactor.push_back(4);
            SetInternalNames();
        }
        //                                   mol/cm3         K
        virtual vector<double> GetIonNums(double denc, double temp) = 0;
        //                            mol/cm3         K
        virtual double FreeEMol(double denc, double temp) = 0;
        //                           g/cm3         K
        virtual vector<double> GetIonPots(double denc, double temp) {
            return IonPot;
        }
        //                           g/cm3         K
        virtual double FreeE(double denc, double temp){
            return FreeEMol(denc/MolVeight, temp) / MolVeight;
        }
        
	    virtual void SetInternalNames(){
		    FreeEIOFind::SetInternalNames();
            vector<double> ions = IonPot; // not GetIonNum(); - because during construction function is undef
            for (size_t k = 0; k < ions.size(); k++)
                OutputNames[Stroka("IonNum_")+Stroka::Int2Str(int(k))] = NULL;
            OutputNames["ElectronNum"] = NULL;
  	    }

	    virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output) {
            int numVar;
            if (Str::GetNumberedVar(Output, "IonNum_", numVar, 0, (int)IonPot.size())) {
                vector<double> ions = GetIonNums(CheckPresence(Input, "Dencity") / MolVeight, CheckPresence(Input, "Temperature"));
                return ions[numVar];
            }

      //      Stroka Test1 = Stroka(Output,0, 7);
      //      if (Test1==Stroka("IonNum_")){
      //          vector<double> ions = GetIonNums(CheckPresence(Input, "Dencity") / MolVeight, CheckPresence(Input, "Temperature"));
		    //    int k = atoi(Stroka(Output, 7, Output.length()-7).c_str());
      //          assert(k>=0 && k<(int)ions.size());
      //          return ions[k];
		    //}
            if (Output == "ElectronNum") {
                vector<double> ions = GetIonNums(CheckPresence(Input, "Dencity") / MolVeight, CheckPresence(Input, "Temperature"));
                double elN = 0;
                for (size_t k = 1; k < ions.size(); k++)
                    elN += k * ions[k];
                return elN;
            }
		    return FreeEIOFind::OutputValClc(Input, Output);
	    }

        Stroka MakeHelp(){
            Stroka res = "\nClass for calculation addition to the free energy due to ionization. Ion levels are given in Electron volts\nIon_fractions are calculated for the internal vars: \nElectronNum and IonNum_i i=0..MaxIonLev , 0 - no ionization - fractions of particles \n";
            res += FreeEIOFind::MakeHelp();
            return res;
        }
        void ReadSubstancePar(istream &in) {
            char tmp[256];
            in>>tmp>>MolVeight>>tmp;
            IonPot.clear();Gfactor.clear();
            TestNextWord(in, "{");
            while(!TestNextChar(in,'}',1) && !(!in) ){
                double d,g;in>>tmp>>d>>tmp>>g;
                IonPot.push_back(d * M_eV_K * M_Rconst);
                Gfactor.push_back(g);
            }
            SavableClass *ptr;
            in>>tmp>>ptr;
            Ref<InterfaceFreeEIO> el;
            if (!(el<<ptr))
                throw info_except("Bad electron matter, not of the InterfaceFreeEIO type\n");
            Electron.Init(el, MolVeight);
            if (!in)
                throw info_except("Could not read ClcSimpleIon...\n");
	    }
        void SaveSubstancePar(ostream &out) {
            out<<EOLN();
            out<<" MolVeight "<<MolVeight<<" IonLevels { ";
            IncIdent();out<<EOLN();
            for(size_t k = 0; k < IonPot.size();k++)
                out<<" L_"<<k<<" "<<IonPot[k] / M_eV_K / M_Rconst<<" Degenration "<<Gfactor[k];
            DecIdent();
            out<<EOLN();
            out<<" } ";
            out<<EOLN();
            out<<" ElectronMatter "<<Electron.GetElectronMatter();
	    }
    protected:
        double MolVeight;
        vector<double> Gfactor;
        ElectronStub Electron;
        vector<double> IonPot;
    private:
    };

//  ====================================================================================
//  ====================            FreeESimpleIon     =================================
//  ====================================================================================
    struct FreeEIonSimple : FreeEIonStdIO {
        
        FreeEIonSimple() : FreeEIonStdIO(), ElNumErr(1e-10), LastElNum(0.5){}
        Stroka MakeHelp(){
            Stroka res = FreeEIonStdIO::MakeHelp() +  
                "\nSimple ionization (fixed ion potentials, no lowering).\n";
            return res;
        }

        double FreeEMol(double denc, double temp){
            SetClcConst(denc, temp);
            FindSystemPar();
            return FreeE();// FreeE(ClcIonNum());
        }

        virtual vector<double> GetIonNums(double denc, double temp) {
            SetClcConst(denc, temp);
            FindSystemPar();
            return FoundIons;//ClcIonNums(ClcElRelations(ClcIonNum()));
        }
        void ReadSubstancePar(istream &in) {
            FreeEIonStdIO::ReadSubstancePar(in);
            char tmp[256];
            in>>tmp>>ElNumErr;
            LastElNum = 0.5;
        }
        void SaveSubstancePar(ostream &out) {
            FreeEIonStdIO::SaveSubstancePar(out);
            out<<" ElNumError "<<ElNumErr;
        }
        virtual vector<double> GetClcIonPots(){
            return clcPot;
        }
    private:
        vector<double> clcPot;
    protected:
        double LastElNum, ElNumErr; // ====================== Set them in the init ===========================

        double clcCoef, clcDenc, clcTemp;

        double FoundElNum;
        vector<double> FoundIons;

        virtual void SetClcConst(double denc, double temp){
            clcDenc = denc;
            clcTemp = temp;
            clcCoef = M_Rconst * temp;
            vector<double> ionpots = GetIonPots(denc, temp);
            clcPot.resize(ionpots.size());
            for(size_t i = 0; i < clcPot.size(); i++) 
                clcPot[i] = ionpots[i] - clcCoef * log(Gfactor[i]);
        }
        virtual void FindSystemPar(){
            FoundElNum = ClcIonNum();
            FoundIons = ClcIonNums(ClcElRelations(FoundElNum));
        }

        virtual double FreeE(){
            double res = FoundElNum * Electron.ClcFreeE(FoundElNum * clcDenc, clcTemp);
            //vector<double> elNums = ClcIonNums(ClcElRelations(elNum));
            vector<double> clcpot = GetClcIonPots();
            for(size_t i = 0; i < clcpot.size(); i++)
                res += FoundIons[i] * (clcpot[i] + clcCoef * log(FoundIons[i]));
            return res;
        }





        vector<double> ClcElRelations(double elNum){
            vector<double> ret(clcPot);
            double m = Electron.MolVeight;
            double fElMul = Electron.ClcFreeE(elNum * clcDenc, clcTemp) + 
                            elNum * clcDenc * Electron.ClcFreeEDeriv(elNum * clcDenc, clcTemp);
            for(size_t i = 0; i < clcPot.size(); i++){
                ret[i] = (clcPot[i] - clcPot[0] + i * fElMul )/(-clcCoef);
            }
            return ret;

        }
        vector<double> ClcIonNums(vector<double> elRelations){
            VecCl elV = elRelations;
            double max = VecCl::Max(elV);
            elV = elV - max;
            for(int k=1;k<=int(elV.Dim());k++){
                 if (elV[k]>-200)
                     elV[k] = exp(elV[k]);
                 else 
                     elV[k] = exp(-200.);
            }
            elV = elV / (elV * (elV * 0 + 1));
            return elV.Copy2Vector();
        }

        double ClcIonNum(){
            double resNum = log(LastElNum), ErrorAbs = ElNumErr, ErrorRel = ElNumErr;
            int err, MaxIter = 1000;
            FreeEIonSimple* OldPtr = CurPtr;
            CurPtr = this;
            if (3<(err=Fzero(DencMisfitStatic, -100, log(GetIonPots(clcDenc, clcTemp).size()-1.), resNum, ErrorAbs, ErrorRel, MaxIter))){
                if ( fabs(resNum+60)>1 || DencMisfitStatic(resNum)<0 ){
			    cout<<" Bad FZero GetElDenc in FreeEIonOneMat \nfound Denc "
				    <<exp(resNum)<<" Mis "<<DencMisfitStatic(resNum)<<" err "<<err<<" logD "<<resNum<<"\n";
                }
            }
            resNum = exp(resNum);
            LastElNum = resNum;
            CurPtr = OldPtr;
            return resNum;
        }
        static FreeEIonSimple* CurPtr;
        static double DencMisfitStatic(double logElNum){
            return CurPtr->DencMisfit(logElNum);
        }
        double DencMisfit(double logElNum){
            double elNum = exp(logElNum);
            vector<double> ionNum = ClcIonNums(ClcElRelations(elNum));
            double ClcN = MathZer;
            for(size_t i = 1; i < ionNum.size(); i++)
                ClcN += i * ionNum[i];
            return log(elNum/ClcN);
        }

    };

//  ====================================================================================
//  ====================            FreeEGrigIon       =================================
//  ====================================================================================
    struct FreeEIonGrig : FreeEIonStdIO {
        FreeEIonGrig() : FreeEIonStdIO() {
        }
        virtual vector<double> GetIonNums(double denc, double temp) {
            double kT = M_Rconst * temp;
            vector<double> ionpots = GetIonPots(denc, temp);
            VecCl ret(int(ionpots.size()));
            for(int k = 1; k <= (int)ionpots.size(); k++)
                 if (-ionpots[k] / kT > -200)
                    ret[k] = Gfactor[k] * exp(-ionpots[k] / kT);
                 else 
                     ret[k] = 0;
            ret = ret / (ret * (ret * 0 + 1));
            return ret.Copy2Vector();
        }

        virtual double FreeEMol(double denc, double temp) {
            if ((temp<MathZer) || (denc<MathZer)) { 
                cout<<"FreeE Bad. Denc "<<denc<<" T "<<temp<<"\n";
                return 0;
            }
            double kT = M_Rconst * temp;
            double add = MathZer;
            vector<double> ionpots = GetIonPots(denc, temp);
            for(size_t k = 0; k < ionpots.size(); k++)
            if (-ionpots[k] / kT > -200)
                add += Gfactor[k] * exp(-ionpots[k] / kT);
            double ret = -kT * log(add);
            return ret;
        }

        Stroka MakeHelp(){
            Stroka res = "\nClass for finding simple ionization - freeE + ion_fraction . Simple Z+=Sum(Exp(-Ei/T); Ion levels are given in Electron volts\nIon_fractions are calculated for the internal vars IonNum_i i=0..MaxIonLev, 0 - no ionization. Estimation of ionization by grigoriev.\n";
            res += FreeEIOFind::MakeHelp();
            return res;
        }
    };

    struct OneVarRosLowering : OneVarFunction {
        OneVarRosLowering() : OneVarFunction(), V0(1), A0(0), A1(1) {}
        virtual double Calculate(double x){
            if (x >= V0)
                return A0;
            else
                return A0 + A1 * sqr(x - V0);
        }

       virtual int save_data_state(FilterOut&so){
           so<<" V0 "<<V0<<" A0 "<<A0<<" A1 "<<A1;
           return 1; 
       };
       virtual int read_data_state(FilterIn&si) {
           char tmp[256];
           si>>tmp>>V0>>tmp>>A0>>tmp>>A1;
           return 1;
       };

        Stroka MakeHelp(){
            return "Lowering of energy levels Pot = F(V) \n    V>V0: Pot = A0 \n     V<V0: Pot = A0 - A1*(V-V0)^2\n";
        }
    private:
        double V0, A0, A1;
    };

    struct FreeEIonLowVolDepend : FreeEIonSimple {
        FreeEIonLowVolDepend(): FreeEIonSimple() {
            VolFuncs.push_back(new OneVarRosLowering); 
        }
        void ReadSubstancePar(istream &in) {
            FreeEIonSimple::ReadSubstancePar(in);
            char tmp[256];
            SavableClass *ptr;
            in>>tmp>>tmp;
            if (stricmp("{", tmp) != 0)
                throw info_except("Bad input. Had to read <{> and read <%s>\n", tmp);
            VolFuncs.clear();
            while( !(!in) && !SavableClass::TestNextChar(in, '}') ){
            //while(!GuessNextChar(in, '}')) {
                in>>ptr;
                Ref<OneVarFunction> ptrF; ptrF<<ptr;
                VolFuncs.push_back(ptrF);
            }
        }
        void SaveSubstancePar(ostream &out) {
            FreeEIonSimple::SaveSubstancePar(out);
            out<<" VolDependentFunctions { ";
            for(size_t i = 0; i < VolFuncs.size(); i++)
                out<<VolFuncs[i];
            out<<" } ";
        }
        Stroka MakeHelp() {
            return FreeEIonSimple::MakeHelp() +  "\nAnd taking into account volume dependence of electron ionization levels. Volume in mol/cm3\n";
        }
        virtual vector<double> GetIonPots(double denc, double temp) {
            vector<double> ionLev; ionLev = IonPot;
            double V = 1. / max(MathZer, denc);
            for(size_t i = 0; i < VolFuncs.size(); i++)
                if (VolFuncs[i].nonnull())
                    ionLev[i] += VolFuncs[i]->Calculate(V);
    //cout<<" D "<<FixData.FixMatD<<" V "<<V<<" lev "<<ionLev;
            return ionLev;
        }

    private:
        vector<Ref<OneVarFunction> > VolFuncs;
    };















//  ====================================================================================
//  ====================            FreeEIonDeby       =================================
//  ====================================================================================

    struct FreeEIonDeby : FreeEIonSimple { //FreeEIonStdIO { //FreeEIonSimple?
        
        FreeEIonDeby() : FreeEIonSimple(), ElNum2Err(1e-15), LastElNum2(0.5), FoundEl2Num(0.5), Coef1Mul(1){}
        Stroka MakeHelp(){
            Stroka res = FreeEIonSimple::MakeHelp() +  
                "\nTaking into account deby lowering of potential\n";
            return res;
        }

        void ReadSubstancePar(istream &in) {
            FreeEIonSimple::ReadSubstancePar(in);
            char tmp[256];
            in>>tmp>>ElNum2Err;//>>tmp>>Coef1Mul;
            LastElNum2 = 0.5;
        }
        void SaveSubstancePar(ostream &out) {
            FreeEIonSimple::SaveSubstancePar(out);
            out<<" ElNum2Error "<<ElNum2Err;//<<" Coef1Mul "<<Coef1Mul;
        }
    protected:

        double iterElNum;
        double ElNum2Err, LastElNum2;
        double clcCoef1;
        double Coef1Mul;

        double FoundEl2Num;

        void SetClcConst(double denc, double temp){
            FreeEIonSimple::SetClcConst(denc, temp);
            clcCoef1 = -2. / 3 * pow(M_CGSCharge * M_ElectronCGSCharge, 3) * 
                sqrt(M_PI * denc / (M_Rconst * temp)) * pow(M_Na, 2);
        }

        virtual void FindSystemPar() {
            ClcIonNum(FoundElNum, FoundEl2Num);
            FoundIons = ClcIonNums(ClcElRelations(FoundElNum, FoundEl2Num));
        }

        virtual double FreeE(){
            double res = FreeEIonSimple::FreeE();
            res += clcCoef1 * pow(FoundEl2Num, 1.5);
            return res;
        }

        virtual vector<double> ClcElRelations(double elNum, double elNum2) {
            vector<double> ret(FreeEIonSimple::ClcElRelations(elNum));
            double c = - 1.5 * sqrt(elNum2) * clcCoef1 / clcCoef;
            for(size_t i = 1; i < GetClcIonPots().size(); i++){
                ret[i] += i * (i + 1) * c;
            }
            return ret;
        }

        double LastCoef;
        void ClcIonNum(double &elNum, double &elNum2){
            double resCoef = LastCoef, ErrorAbs = ElNumErr, ErrorRel = ElNumErr;
            int err, MaxIter = 1000;

            FreeEIonDeby* OldPtr = CurD;
            CurD = this;
            if (3<(err=Fzero(Num2CoefMisfitStaticD1, 2, GetIonPots(clcDenc, clcTemp).size(), resCoef, ErrorAbs, ErrorRel, MaxIter)))
                cout<<" Bad FZero GetElDenc in FreeEIonDebyHuckel \nfound Coef "<<resCoef<<" Mis "<<DencMisfitStaticD1(resCoef)<<" err "<<err<<"\n";
            elNum = ClcIonNum(resCoef);
            elNum2 = elNum * resCoef;

            LastElNum = elNum;
            LastCoef = resCoef;
            CurD = OldPtr;
        }
        static FreeEIonDeby* CurD;
        static double Num2CoefMisfitStaticD1(double num2coef){
            return CurD->Num2CoefMisfitD1(num2coef);
        }
        double Num2CoefMisfitD1(double num2coef){
            double elNum = ClcIonNum(num2coef);
            double elNum2 = elNum * num2coef;
            vector<double> ionNum = ClcIonNums(ClcElRelations(elNum, elNum2));
            double ClcN = MathZer, ClcN2 = MathZer;
            for(size_t i = 1; i < ionNum.size(); i++){
                ClcN2 += i * i * ionNum[i];
                ClcN += i * ionNum[i];
            }
            return log((ClcN2 + ClcN) / ClcN / num2coef);
        }
        double ClcIonNum(double num2coef) {
            iterNum2Coef = num2coef;
            double resNum = log(LastElNum), ErrorAbs = ElNum2Err, ErrorRel = ElNum2Err;
            int err, MaxIter = 1000;
            FreeEIonDeby* OldPtr = CurD;
            CurD = this;
            if (3<(err=Fzero(DencMisfitStaticD1, -100, log((double)GetIonPots(clcDenc, clcTemp).size()), resNum, ErrorAbs, ErrorRel, MaxIter)))
                cout<<" Bad FZero DencMisfitStaticD1 in FreeEIonDebyHuckel \nfound Denc "<<exp(resNum)<<" Mis "<<DencMisfitStaticD1(resNum)<<" err "<<err<<" logD "<<resNum<<"\n";
            resNum = exp(resNum);
            LastElNum = resNum;
            CurD = OldPtr;
            return resNum;
        }
        static double DencMisfitStaticD1(double logElNum){
            return CurD->DencMisfitD1(logElNum);
        }
        double iterNum2Coef;
        double DencMisfitD1(double logElNum){
            double elNum = exp(logElNum);
            double elNum2 = elNum * iterNum2Coef;
            vector<double> ionNum = ClcIonNums(ClcElRelations(elNum, elNum2));
            double ClcN = MathZer;
            for(size_t i = 1; i < ionNum.size(); i++)
                ClcN += i * ionNum[i];
            return log(elNum/ClcN);
        }

    };




//  ====================================================================================
//  ====================     FreeEIonDebyHuckel        =================================
//  ====================================================================================

    struct FreeEIonDebyHuckel : FreeEIonDeby { //FreeEIonStdIO { //FreeEIonSimple?
        
        FreeEIonDebyHuckel() : FreeEIonDeby(), CorrectName("Deby") {
            CorrectFuncs["Deby"] = new LoweringDeby;
            CorrectFuncs["OCP"] = new LoweringOCP;
            CorrectFuncs["OCPtest"] = new LoweringOCPTest;
            CorrectFuncs["Zer"] = new LoweringZer;
        }
        struct LoweringFunc : RefCount {
            virtual double Func(double elNum2) = 0;
            virtual double Deriv(double elNum2){
                return (Func(elNum2 * (1 + 0.5 * M_Eps2)) - Func(elNum2 * (1 - 0.5 * M_Eps2))) / M_Eps2 / elNum2;
            }
            virtual void Init(double denc, double temp) {};
        };
        
        struct LoweringDeby : LoweringFunc {
            virtual double Func(double elNum2) {
                return clcCoef1 * pow(elNum2, 3. / 2);
            }
            virtual double Deriv(double elNum2){
                return 3. / 2 * clcCoef1 * sqrt(elNum2);
            }
            virtual void Init(double denc, double temp) {
                clcCoef1 = -2. / 3 * pow(M_CGSCharge * M_ElectronCGSCharge, 3) * 
                    sqrt(M_PI * denc / (M_Rconst * temp)) * pow(M_Na, 2);
            };
        private:
            double clcCoef1;
        };
        struct LoweringOCPTest : LoweringFunc {
            virtual double Func(double elNum2) {
                return - pow(giCoef * elNum2, 3. / 2) / sqrt(3.);
            }
            virtual double Deriv(double elNum2){
                return - sqrt(giCoef * elNum2) * giCoef * sqrt(3.) / 2;
            }
            virtual void Init(double denc, double temp) {
                giCoef = pow(2., 2. / 3) * pow(M_CGSCharge * M_ElectronCGSCharge, 2) * 
                    pow(M_PI * denc / (3 * M_Rconst * temp), 1. / 3) * pow(M_Na, 1.333333333);
            };
        private:
            double giCoef;
        };
        struct LoweringOCP : LoweringFunc {
            virtual double Func(double elNum2) {
                double gi = giCoef * elNum2;
                double ret = sqrt(gi) * (2 * A3 + A1 * sqrt(A2 + gi));
                ret -= A1*A2*log(sqrt(gi/A2) + sqrt(1+gi/A2));
                ret -= 2 * A3 * atan(sqrt(gi));
                return ret;
            }
            virtual double Deriv(double elNum2){
                double gi = giCoef * elNum2;
                double a2s = sqrt(A2 + gi);
                return sqrt(gi) * (A1 + A3 * a2s + A1 * gi) * giCoef / a2s / (1 + gi);
            }
            virtual void Init(double denc, double temp) {
                giCoef = pow(2., 2. / 3) + pow(M_CGSCharge * M_ElectronCGSCharge, 2) * 
                    pow(M_PI * denc / (3 * M_Rconst * temp), 1. / 3) * pow(M_Na, 1.333333333);
                A1 = -0.9052;
                A2 = 0.6322;
                A3 = -sqrt(3.) / 2 - A1 / sqrt(A2);
            };
        private:
            double giCoef, A1, A2, A3;
        };

        struct LoweringZer : LoweringFunc { virtual double Func(double elNum2) { return 0;} };
        map<Stroka, Ref<LoweringFunc> > CorrectFuncs;
        Stroka CorrectName;
        Stroka MakeHelp(){
            Stroka res = FreeEIonSimple::MakeHelp() +  
                "\nTaking into account Deby OCP(Phys Rev E v58 n4 1998 p 4941 )  Zer(=simple ion) OCPtest(=deby test) or GVK(not implemented) lowering of potentials\n";
            return res;
        }

        void ReadSubstancePar(istream &in) {
            FreeEIonDeby::ReadSubstancePar(in);
            char tmp[256];
            in>>tmp>>CorrectName;
        }
        void SaveSubstancePar(ostream &out) {
            FreeEIonDeby::SaveSubstancePar(out);
            out<<" CorrectName "<<CorrectName;
        }
    protected:

        void SetClcConst(double denc, double temp){
            FreeEIonDeby::SetClcConst(denc, temp);
            CorrectFuncs[CorrectName]->Init(denc, temp);
        }

        virtual double FreeE(){
            double res = FreeEIonSimple::FreeE();
            res += CorrectFuncs[CorrectName]->Func(FoundEl2Num);
            return res;
        }

        virtual vector<double> ClcElRelations(double elNum, double elNum2) {
            vector<double> ret(FreeEIonSimple::ClcElRelations(elNum));
            double c = - CorrectFuncs[CorrectName]->Deriv(elNum2) / clcCoef;
            for(size_t i = 1; i < GetClcIonPots().size(); i++){
                ret[i] += i * (i + 1) * c;
            }
            return ret;
        }

    };





//  ====================================================================================
//  ====================     FreeEIonDebyHuckel        =================================
//  =========================      END             =====================================












    struct AnisimovLowering {
        double clcCoef2, clcCoef1;
        vector<double> clcLc0, clcLc1, clcLc2, clcLc3;
        vector<double> N_num, L_num, clcCoef3;
        AnisimovLowering() {
            N_num = VecCl::MakeStepVector(2, 0, 1, 0).Copy2Vector();
            L_num = VecCl::MakeStepVector(2, 0, 1, 0).Copy2Vector();
            clcCoef3 = VecCl::MakeStepVector(2, 0, 0, 0).Copy2Vector();
        }

        void ReadSubstancePar(istream &in) {
            char tmp[256];
            in>>tmp;
            TestNextWord(in, "{");
            N_num.clear(); L_num.clear();clcCoef3.clear();
            while(!SavableClass::TestNextChar(in,'}',1) && !(!in) ){
                double n,l;in>>tmp>>n>>tmp>>l;
                N_num.push_back(n);
                L_num.push_back(l);
                clcCoef3.push_back((sqr(n) - l * (l - 1)) / (2 * n));
            }
            clcLc0.resize(N_num.size());
            clcLc1.resize(N_num.size());
            clcLc2.resize(N_num.size());
            clcLc3.resize(N_num.size());
	    }
        void SaveSubstancePar(ostream &out) {
            //out<<" ElNum2Error "<<ElNum2Err<<" CorrectName "<<CorrectName;
            out<<" Nnum_Lnum { ";
            SavableClass::IncIdent();out<<SavableClass::EOLN();
            for(size_t i = 0; i < L_num.size(); i++)
                out<<" N "<<N_num[i]<<" L "<<L_num[i]<<"   ";
            SavableClass::DecIdent();out<<SavableClass::EOLN();
            out<<" } ";
            out<<SavableClass::EOLN();
        }
                        //     mol/cm3       K                             kJ(original)
        void SetClcConst(double denc, double temp, const vector<double> &ionPots){
            //clcCoef1 = -2. / 3 * pow(M_CGSCharge * M_ElectronCGSCharge, 3) * 
            //    sqrt(M_PI * denc / clcCoef) * pow(M_Na, 2);
            double clcCoef = M_Rconst * temp;
            clcCoef2 = 2 * M_CGSCharge * M_ElectronCGSCharge *
                sqrt(M_PI * denc / clcCoef) * M_Na; // clcCoef2 * sqrt(sum2el) = Cappa (cm)
            clcCoef2 *= M_Bor * 1e-8; //Cappa (1/BorRad)
            vector<double> ionpot = (VecCl(ionPots) * (1000 / (M_Na * M_Eau_J))).Copy2Vector();

            for(size_t i = 0;i<clcCoef3.size();i++){
                //clcLc0[i] = IonPot[i];
                if (ionpot[i]==0)
                    clcLc1[i] = clcLc2[i] = clcLc3[i] = 0;
                else {
                    clcLc1[i] = -(1+2*clcCoef3[i])*sqrt(ionpot[i]*0.5);
                    clcLc2[i] = clcCoef3[i]*(1+0.5*clcCoef3[i]);
                    clcLc3[i] = -sqr(clcCoef3[i])/(2*sqrt(ionpot[i]*2));
                }
            }
        }

        double GetLambda(double num2el) {
            return 1 / clcCoef2 * sqrt(num2el);
        }
        double GetILambda(double num2el) {
            return (clcCoef2 * sqrt(num2el));
        }
        vector<double> ClcPotentialAddition(double num2el) {
            double lambdaI = (clcCoef2 * sqrt(num2el));
            double lambdaI2 = sqr(lambdaI);
            vector<double> ionAdd(clcLc1.size());
            ionAdd[0] = 0;
            for(size_t i = 1; i < ionAdd.size();i++)
                ionAdd[i] = lambdaI * clcLc1[i] + lambdaI2 * clcLc2[i] + lambdaI * lambdaI2 * clcLc3[i];
            //return ionAdd;
            return (VecCl(ionAdd) * (M_Na * M_Eau_J / 1000)).Copy2Vector();
        }
        vector<double> ClcDerivAdd(vector<double> &el) {
            double n2el = MathZer, nel = MathZer, lc1s = MathZer, lc2s = MathZer, lc3s = MathZer;
            for(size_t i = 1; i < el.size(); i++){
                nel += i * el[i];
                n2el += sqr(i) * el[i];
                lc1s += el[i] * clcLc1[i];
                lc2s += el[i] * clcLc2[i];
                lc3s += el[i] * clcLc3[i];
            }
            n2el += nel;
            vector<double> derAdd(el.size());
            for(size_t i = 0; i < el.size(); i++){
                double ionNmul = i * (i + 1);
                derAdd[i] = ionNmul * (clcCoef2 * lc1s / (2 * sqrt(n2el)) + sqr(clcCoef2) * lc2s - 1.5 * sqrt(n2el) * pow(clcCoef2, 3) * lc3s);
                derAdd[i] += sqrt(n2el) * clcCoef2 * (clcLc1[i] - clcLc1[0]) + n2el * sqr(clcCoef2) * (clcLc2[i] - clcLc2[0]);
                derAdd[i] += pow(n2el, 1.5) * pow(clcCoef2, 3) * (clcLc3[i] - clcLc3[0]);
            }
            //return derAdd; // derAdd/-kT = log[el[i]/el[0]]
            return (VecCl(derAdd) * (M_Na * M_Eau_J / 1000)).Copy2Vector();
            
        }

    };

    struct AnisimovLoweringCurve : NamedOneVarFunction {
        AnisimovLoweringCurve() : NamedOneVarFunction(Stroka("IonPot_0 IonPotAU_0 PotDer_0 Lambda X_sum2el")) {
        }
        virtual double Calculate(double sum2el, const Stroka &What){
            KnownClcName(What,1);
            int numVar;
            Anis.SetClcConst(Dencity, Temperature, Energy);
            if (What == "Lambda")
                return Anis.GetLambda(sum2el); //clcCoef2 * sqrt(sum2el);
            if (What == "ILambda")
                return Anis.GetILambda(sum2el); // return 1 / (Anis.clcCoef2 * sqrt(sum2el));
            if (Str::GetNumberedVar(What, "IonPot_", numVar, 0, (int)Energy.size()))
                return Anis.ClcPotentialAddition(sum2el)[numVar] / max(MathZer, Energy[numVar]);// * (1000 / (M_Na * M_Eau_J));
            if (Str::GetNumberedVar(What, "IonPotAU_", numVar, 0, (int)Energy.size()))
                return Anis.ClcPotentialAddition(sum2el)[numVar] / max(MathZer, Energy[numVar]);
            if (Str::GetNumberedVar(What, "PotDer_", numVar, 0, (int)Energy.size()))
                return Anis.ClcDerivAdd(MakeElectronVector(sum2el))[numVar];
            throw info_except("Name <%s> is not defined.\nDefined vars: %s\n", What.c_str(), MakeNames().c_str());
        }
        vector<double> MakeElectronVector(double sum2el) {
            double stdNum2El = MathZer;
            for(size_t i = 1; i < Energy.size(); i++)
                stdNum2El += i * (i + 1);
            VecCl ret(int(Energy.size()));ret = ret * 0 + sum2el / stdNum2El;
            ret[0] = max(MathZer, 1 - sum2el / stdNum2El * (Energy.size() - 1));
            return ret.Copy2Vector();
        }
        Stroka MakeNames() {
            Stroka ret("X_sum2el Lambda ILambda");

            if (Energy.size() != Anis.N_num.size())
                throw info_except(" Dims E<%i> Anis_M<%i> are different\n", Energy.size(), Anis.N_num.size());
            for(size_t i = 0; i < Energy.size(); i++) {
                ret += " IonPot_" + Stroka::Int2Str(int(i));
                ret += " PotDer_" + Stroka::Int2Str(int(i));
                ret += " IonPotAU_" + Stroka::Int2Str(int(i));
            }
            return ret;
        }
        virtual int save_data_state(FilterOut&out) {
            Anis.SaveSubstancePar(out);
            out<<" Ek { ";
            SavableClass::IncIdent();out<<SavableClass::EOLN();
            for(size_t i = 0; i < Energy.size(); i++)
                out<<" E "<<Energy[i] / (M_eV_K * M_Rconst)<<"   ";
            SavableClass::DecIdent();out<<SavableClass::EOLN();
            out<<" } ";
            out<<SavableClass::EOLN();
            out<<" Dencity "<<Dencity<<" Temperature "<<Temperature;
            return !(!out);
        }
        virtual int read_data_state(FilterIn &in){
            Stroka tmp;
            Anis.ReadSubstancePar(in);
            in>>tmp;
            TestNextWord(in, "{");
            Energy.clear(); 
            while(!SavableClass::TestNextChar(in,'}',1) && !(!in) ){
                double e;in>>tmp>>e;
                Energy.push_back(e * M_eV_K * M_Rconst);
            }
            in>>tmp>>Dencity>>tmp>>Temperature;
            SetClcList(MakeNames());
            return !(!in);
        }
        Stroka MakeHelp(){
            Stroka res = "Class for calculation of potential lowering in Anisimovs appr. Lambda in 1 / Bhor, ILambda - in Bhor IonPotAdd - in IonPot IonDer - too, \n"+
                NamedOneVarFunction::MakeHelp();
            return res;
        }
    protected:
        AnisimovLowering Anis;
        vector<double> Energy;
        double Dencity, Temperature;

    };





//  ====================================================================================
//  ====================     FreeEIonDebyAnisimov      =================================
//  ====================================================================================

    struct FreeEIonDebyAnisimov : FreeEIonDebyHuckel { //FreeEIonStdIO { //FreeEIonSimple?
        
        FreeEIonDebyAnisimov() : FreeEIonDebyHuckel(), LogVectorErr(1e-8) {
            LastIons = (VecCl(int(IonPot.size())) * 0).Copy2Vector();
            if (LastIons.size() > 0)
                LastIons[0] = 1;
        }
        Stroka MakeHelp(){
            Stroka res = FreeEIonDebyHuckel::MakeHelp() +  
                "\nTaking into account Anisimov lowering of potentials.\nAnisimov Petrov Snizhenie Potentsialov ionizatsii v neidealnoi plasme ZhTPh 1998 t68 n6 p 45-50.\nCan show DebyL IonLevel_i IonLowering_i IonNum_i ElectronNum\n";
            return res;
        }

	    virtual void SetInternalNames(){
		    //FreeEIonStdIO::SetInternalNames();
		    FreeEIonDebyHuckel::SetInternalNames();
            vector<double> ions = IonPot; // not GetIonNum(); - because during construction function is undef
            for (size_t k = 0; k < ions.size(); k++) {
                OutputNames[Stroka("IonLevel_") + Stroka::Int2Str(int(k))] = NULL;
                OutputNames[Stroka("IonLowering_") + Stroka::Int2Str(int(k))] = NULL;
            }
            OutputNames["DebyL"] = NULL;
  	    }

	    virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
                //return Anis.clcCoef2 * sqrt(sum2el);
            //if (What == "ILambda")
            //    return 1 / (Anis.clcCoef2 * sqrt(sum2el));
            int numVar;
            if (Str::GetNumberedVar(Output, "IonLevel_", numVar, 0, (int)IonPot.size())) {
                vector<double> ions = GetIonNums(CheckPresence(Input, "Dencity") / MolVeight, CheckPresence(Input, "Temperature"));
                vector<double> pots = FreeEIonDebyHuckel::GetClcIonPots();
                return pots[numVar];
		    }
            if (Str::GetNumberedVar(Output, "IonLowering_", numVar, 0, (int)IonPot.size())) {
                vector<double> ions = GetIonNums(CheckPresence(Input, "Dencity") / MolVeight, CheckPresence(Input, "Temperature"));
                vector<double> pots = FreeEIonDebyHuckel::GetClcIonPots();
                vector<double> anisAdd = Anis.ClcPotentialAddition(FoundEl2Num);
                return anisAdd[numVar] / pots[numVar];
		    }
            if (Output == "DebyL") {
                vector<double> ions = GetIonNums(CheckPresence(Input, "Dencity") / MolVeight, CheckPresence(Input, "Temperature"));
                return Anis.GetLambda(FoundEl2Num);
            }
		    return FreeEIonDebyHuckel::OutputValClc(Input, Output);
	    }


        //map<Stroka, Ref<LoweringFunc> > CorrectFuncs;
        //Stroka CorrectName;
        //double FreeEMol(double denc, double temp){
        //    SetClcConst(denc, temp);
        //    double elNum, elNum2;
        //    ClcIonNum(elNum, elNum2);
        //    return FreeE_(elNum, elNum2, ions);
        //}

        //virtual vector<double> GetIonNums(double denc, double temp) {
        //    SetClcConst(denc, temp);
        //    double elNum, elNum2;
        //    vector<double> ions;
        //    ClcIonNum(elNum, elNum2, ions);
        //    return ions;//ClcIonNums(ClcElRelationsAnis(elNum, elNum2));
        //}
        void ReadSubstancePar(istream &in) {
            FreeEIonDebyHuckel::ReadSubstancePar(in);
            Anis.ReadSubstancePar(in);

            LastIons = (VecCl(int(IonPot.size())) * 0).Copy2Vector();
            if (LastIons.size() > 0)
                LastIons[0] = 1;
            char tmp[256];
            in>>tmp>>LogVectorErr;
            //LastElNum2 = 0.5;
        }
        void SaveSubstancePar(ostream &out) {
            FreeEIonDebyHuckel::SaveSubstancePar(out);
            Anis.SaveSubstancePar(out);
            out<<" LogVectorErr "<<LogVectorErr;
        }
        virtual vector<double> GetClcIonPots(){
            return (VecCl(FreeEIonDebyHuckel::GetClcIonPots()) + 
                VecCl(Anis.ClcPotentialAddition(FoundEl2Num))).Copy2Vector();
        }
        
    protected:
        AnisimovLowering Anis;

        vector<double> FoundIons, LastIons /*For ClcElRelations*/;
        double LogVectorErr;

        void SetClcConst(double denc, double temp){
            FreeEIonDebyHuckel::SetClcConst(denc, temp);
            Anis.SetClcConst(denc, temp, FreeEIonDebyHuckel::GetIonPots(denc, temp));
        }

        virtual void FindSystemPar() {
            FreeEIonDebyHuckel::FindSystemPar();
            FoundIons = ClcIonNums(ClcElRelations(FoundElNum, FoundEl2Num));
        }

        virtual vector<double> ClcElRelations(double elNum, double elNum2) {
            VecCl huck(FreeEIonDebyHuckel::ClcElRelations(elNum, elNum2));
            vector<double> ions(LastIons);
            for(size_t i = 0; i < 100; i++) {
                vector<double> iplus = ClcIonNums((huck + VecCl(Anis.ClcDerivAdd(ions))).Copy2Vector());
                double err = MathZer;
                for(size_t i1 = 0; i1 < iplus.size(); i1++)
                    err += fabs(log(max(MathZer, iplus[i1]) / max(MathZer, ions[i1])));
                ions = ((VecCl(ions) + VecCl(iplus)) / 2).Copy2Vector();
                if (err < LogVectorErr)
                    break;
            }
            LastIons = ions;
            return (huck + VecCl(Anis.ClcDerivAdd(LastIons))).Copy2Vector();
        }

    };



 






////  ====================================================================================
////  ====================       FreeEIonAnisimovDeby    =================================
////  ====================================================================================
//
//    struct FreeEIonAnisimovDeby : FreeEIonDeby { //FreeEIonStdIO { //FreeEIonSimple?
//        
//        FreeEIonAnisimovDeby() : FreeEIonDeby(){}
//        Stroka MakeHelp(){
//            Stroka res = FreeEIonDeby::MakeHelp() +  
//                "\nTaking into account anisimov lowering of potential\n";
//            return res;
//        }
//
//        void ReadSubstancePar(istream &in) {
//            FreeEIonDeby::ReadSubstancePar(in);
//            char tmp[256];
//            in>>tmp>>ElNum2Err>>tmp>>Coef1Mul;
//            LastElNum2 = 0.5;
//        }
//        void SaveSubstancePar(ostream &out) {
//            FreeEIonDeby::SaveSubstancePar(out);
//            out<<" ElNum2Error "<<ElNum2Err<<" Coef1Mul "<<Coef1Mul;
//        }
//    protected:
//
//        double clcCoef2;
//        vector<double> clcLc0, clcLc1, clcLc2, clcLc3;
//        vector<double> N_num, L_num;
//
//        void SetClcConst(double denc, double temp){
//            FreeEIonDeby::SetClcConst(denc, temp);
//            //clcCoef1 = -2. / 3 * pow(M_CGSCharge_JouleCentimeterP05 * sqrt(1e-3) * M_ElectronCGSCharge, 3) * 
//            //    sqrt(M_PI * denc / clcCoef) * pow(M_Na, 2);
//            clcCoef2 = 2 * M_CGSCharge_JouleCentimeterP05 * sqrt(1e-3) * M_ElectronCGSCharge*
//                sqrt(M_PI * denc / clcCoef) * M_Na;
//
//            vector<double> clcCoef3;
//            clcCoef3.resize(N_num.size());
//            clcLc0.resize(N_num.size());
//            clcLc1.resize(N_num.size());
//            clcLc2.resize(N_num.size());
//            clcLc3.resize(N_num.size());
//            for(size_t i = 0;i<clcCoef3.size();i++){
//                clcCoef3[i] = (sqr(N_num[i])-L_num[i]*(L_num[i]-1))/(2*N_num[i]);
//                clcLc0[i] = IonPot[i]-clcCoef*log(Gfactor[i]);
//                if (IonPot[i]==0)
//                    clcLc1[i] = clcLc2[i] = clcLc3[i] = 0;
//                else {
//                    clcLc1[i] = -(1+2*clcCoef3[i])*sqrt(IonPot[i]*0.5);
//                    clcLc2[i] = clcCoef3[i]*(1+0.5*clcCoef3[i]);
//                    clcLc3[i] = -sqr(clcCoef3[i])/(2*sqrt(IonPot[i]*2));
//                }
//            }
//        }
//
//
//
//
//
//
//        void SetIterConst_(double ElNum, double El2Num){
//            double LambdaI = clcCoef2*iterEl2Num;
//            for(size_t i =0;i<iterPot.size();i++)
//                iterPot[i] = clcLc0[i];
//            if (CalcMethod==0){
//                for(size_t i =0;i<iterPot.size();i++)
//                    iterPot[i] += clcLc1[i]*LambdaI + clcLc2[i]*sqr(LambdaI) + clcLc3[i]*pow(LambdaI,3);
//            }
//            iterElFree = Electron->ClcFreeE(iterElNum*clcDenc, clcTemp);
//            iterElDeriv = Electron->ClcFreeEDeriv(iterElNum*clcDenc, clcTemp);
//cout<<" New freeE "<<iterElFree/MolVeight<<" Denc "<<iterElNum*clcDenc*MolVeight<<" Temp "<<clcTemp;//<<"\n";
//        }
//        double FreeE(const vector<double> &ionnum){
//            SetIterConst(ionnum);
//            double res = iterElNum*iterElFree;
//            for(size_t i =0;i<iterPot.size();i++)
//                res += iterNum[i]*(iterPot[i]+clcCoef*log(iterNum[i]));
//            if (CalcMethod<2)
//                res += clcCoef1*pow(iterEl2Num, 1.5);
//            return res;
//        }
//        double ClcLogRelat(int i1, int i2){
//        //double ClcLogRelat(int i2, int i1){
//            double IonNumMul = (i2 - i1)*(1+i1+i2);
//            double Lc1sum = 0, Lc2sum = 0, Lc3sum = 0; 
//            for(size_t i = 0;i<clcLc0.size();i++){
//                Lc1sum += clcLc1[i]*iterNum[i];
//                Lc2sum += clcLc2[i]*iterNum[i];
//                Lc3sum += clcLc3[i]*iterNum[i];
//            }
//            double res = (clcLc0[i1] - clcLc0[i2]) + ( i1 - i2 )*( iterElFree + iterElNum*clcDenc*iterElDeriv );
////cout<<" iterElFree "<<iterElFree<<" iterElNum "<<iterElNum<<" iterElDeriv "<<iterElDeriv<<" sum "<<iterElFree + iterElNum*clcDenc*iterElDeriv; 
////cout<<" IonLev "<<IonPot[i2]<<"\n";
//if (i1!=i2)
//cout<<" Delta "<<( i1 - i2 )*( iterElFree + iterElNum*clcDenc*iterElDeriv )/M_Rconst<<" IonLev "<<-IonPot[i2]/M_Rconst<<"\n";
//            if (CalcMethod<2){
//                res += ( -1.5*sqrt(iterEl2Num)*(clcCoef1 + pow(clcCoef2,3)) - 
//                          0.5*clcCoef2*Lc1sum/sqrt(iterEl2Num) - Lc2sum*sqr(clcCoef2) )*IonNumMul;
//                res += (clcLc1[i1] - clcLc1[i2])*sqrt(iterEl2Num)*clcCoef2 + 
//                       (clcLc2[i1] - clcLc2[i2])*iterEl2Num*sqr(clcCoef2) +
//                       (clcLc3[i1] - clcLc3[i2])*pow(iterEl2Num, 1.5)*pow(clcCoef2,3);
//            }
//            //return -res/clcCoef;
////cout<<" Lev "<<i1<<":"<<i2<<" "<<res<<" norm "<<res/clcCoef<<" ion "<<IonPot[i2]<<"\n";
//            return res/clcCoef;
//        }
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//        double FreeE_(double elNum, double elNum2){
//            double res = elNum * Electron.ClcFreeE(elNum * clcDenc, clcTemp);
//            vector<double> elNums = ClcIonNums(ClcElRelations(elNum, elNum2));
//            for(size_t i = 0; i < clcPot.size(); i++)
//                res += elNums[i] * (clcPot[i] + clcCoef * log(elNums[i]));
//            res += clcCoef1 * pow(elNum2, 2. / 3);
//            return res;
//        }
//
//        virtual vector<double> ClcElRelations(double elNum, double elNum2) {
//            vector<double> ret(FreeEIonSimple::ClcElRelations(elNum));
//            double c = 1.5 * sqrt(elNum2) * clcCoef1 / clcCoef;
//            for(size_t i = 0; i < clcPot.size(); i++){
//                ret[i] -= i * (i + 1) * c;
//            }
//            return ret;
//        }
//
//        void ClcIonNum(double &elNum, double &elNum2){
//            double ErrorAbs = ElNumErr, ErrorRel = ElNumErr;
//            elNum = FreeEIonSimple::ClcIonNum();
//            elNum2 = ClcIonNum2(elNum); // calculation of iterResElNum
//            double resNum = log(elNum);
//            int err, MaxIter = 1000;
//            FreeEIonDeby* OldPtr = CurD;
//            CurD = this;
//            if (3<(err=Fzero(DencMisfitStaticD1, resNum, log(iterResElNum), resNum, ErrorAbs, ErrorRel, MaxIter))){
//                cout<<" Bad FZero GetElDenc in FreeEIonDeby \nfound Denc "<<exp(resNum)<<" Mis "<<DencMisfitStaticD1(resNum)<<" err "<<err<<" logD "<<resNum<<"\n";
//            }
//            resNum = exp(resNum);
//            LastElNum = resNum;
//            elNum = resNum;
//            elNum2 = ClcIonNum2(elNum);
//            CurD = OldPtr;
//        }
//        static FreeEIonDeby* CurD;
//        static double DencMisfitStaticD1(double logElNum){
//            return CurD->DencMisfitD1(logElNum);
//        }
//        double DencMisfitD1(double logElNum){
//            double elNum = exp(logElNum);
//            double elNum2 = ClcIonNum2(elNum);
//            vector<double> ionNum = ClcIonNums(ClcElRelations(elNum, elNum2));
//            double ClcN = MathZer;
//            for(size_t i = 1; i < ionNum.size(); i++)
//                ClcN += i * ionNum[i];
//            return log(elNum / ClcN);
//        }
//        double ClcIonNum2(double elNum){
//            iterElNum = elNum;
//            double resNum = log(LastElNum2), ErrorAbs = ElNum2Err, ErrorRel = ElNum2Err;
//            resNum = log(EstimateElNum2(elNum));
//            int err, MaxIter = 1000;
//            FreeEIonDeby* OldPtr = CurD;
//            CurD = this;
//            if (3<(err=Fzero(DencMisfitStaticD2, resNum, log(elNum * (GetIonPots(clcDenc, clcTemp).size() - 1 + 1)), resNum, ErrorAbs, ErrorRel, MaxIter))){
//                cout<<" Bad FZero GetEl2Denc in FreeEIonDeby \nfound Denc "<<exp(resNum)<<" Mis "<<DencMisfitStaticD2(resNum)<<" err "<<err<<" logD "<<resNum<<"\n";
//            }
//            resNum = exp(resNum);
//            LastElNum2 = resNum;
//            CurD = OldPtr;
//            return resNum;
//        }
//        static double DencMisfitStaticD2(double logElNum){
//            return CurD->DencMisfitD2(logElNum);
//        }
//        double iterResElNum;
//        double DencMisfitD2(double logElNum){
//            double elNum2 = exp(logElNum);
//            double elNum = iterElNum;
//            vector<double> ionNum = ClcIonNums(ClcElRelations(elNum, elNum2));
//            //double ClcN = MathZer;
//            double ClcN = elNum;
//            ClcN = MathZer;
//            iterResElNum = MathZer;
//            for(size_t i = 1; i < ionNum.size(); i++){
//                ClcN += i * i * ionNum[i];
//                ClcN += i * ionNum[i];
//                iterResElNum += i * ionNum[i];
//            }
//            //iterResElNum = (iterResElNum + elNum) / 2.;
//            return log(elNum2/ClcN);
//        }
//        double EstimateElNum2(double elNum){
//            vector<double> ionNum = ClcIonNums(FreeEIonSimple::ClcElRelations(elNum));
//            double ClcN = elNum;
//            ClcN = MathZer;
//            for(size_t i = 1; i < ionNum.size(); i++){
//                ClcN += i * i * ionNum[i];
//                ClcN += i * ionNum[i];
//            }
//            return ClcN;
//        }
//
//    };
//























}; //ClcIonisation


//// Base class for ionization calculations - reads data (Degeneration, MolWeight, IonLevels)
//// provides output ion number variables (overwrite GetIonNum function)
//struct FreeEIonAdd:FreeEIOFind{
//    FreeEIonAdd();
//    double FreeE(double Denc,double T) {
//     if ((T<MathZer) || (Denc<MathZer)) 
//        { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
//     double kt=M_Rconst/MolVeight*T;
//     double add = 0;
//     for(size_t k=0;k<IonLevels.size();k++)
//         if (-IonLevels[k]/T>-100)
//            add +=Degeneration[k]*exp(-IonLevels[k]/T);
//     if (add<MathZer)
//         add = StndErr;
//     double ret=-kt*log(add);
//     return ret;
//    }
//    void ReadSubstancePar(istream &in) {
//        char tmp[256];
//        in>>tmp>>MolVeight>>tmp;
//        IonLevels.clear();Degeneration.clear();
//        TestNextWord(in, "{");
//        while(!TestNextChar(in,'}',1) && !(!in) ){
//            double d,deg;in>>tmp>>d>>tmp>>deg;
//            IonLevels.push_back(d*M_eV_K);
//            Degeneration.push_back(deg);
//        }
//        if (!in)
//            throw info_except("Could not read matter...\n");
//    }
//    void SaveSubstancePar(ostream &out) {
//        out<<" MolVeight "<<MolVeight<<" IonLevels { ";
//        for(size_t k=0;k<IonLevels.size();k++)
//            out<<" L_"<<k<<" "<<IonLevels[k]/M_eV_K<<" Degenration "<<Degeneration[k];
//        out<<" } ";
//    }
//
//
//
//	virtual void SetInternalNames(){
//		FreeEIOFind::SetInternalNames();
//		for (size_t k=0;k<=IonLevels.size();k++)
//            OutputNames[Stroka("IonNum_")+Stroka::Int2Str(int(k))] = NULL;
//        OutputNames["ElectronNum"] = NULL;
//  	}
//	virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
//		Stroka Test1 = Stroka(Output,0, 6);
//        if (Test1==Stroka("IonNum")){
//		    int k = atoi(Stroka(Output, 7, Output.length()-7).c_str());
//            assert(k>=0 && k<(int)IonLevels.size());
//            return GetIonNum(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature"))[k+1];
//		}
//        if (Output == "ElectronNum"){
//            vector<double> ions = GetIonNum(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature")).Copy2Vector();
//            double elN = 0;
//            for (size_t k = 1; k < ions.size(); k++)
//                elN += k * ions[k];
//            return elN;
//        }
//		return FreeEIOFind::OutputValClc(Input, Output);
//	}
//
//
//    Stroka MakeHelp(){
//        Stroka res = "Class for finding simple ionization - freeE + ion_fraction . Simple Z+=Sum(Exp(-Ei/T); Ion levels are given in Electron volts\nIon_fractions are calculated for the internal vars IonNum_i i=0..MaxIonLev, 0 - no ionization. Estimation of ionization by grigoriev.\n"+
//        FreeEIOFind::MakeHelp();
//        return res;
//    }
//
//// To be overwrited by the children
//    virtual VecCl GetIonNum(double Denc, double T){
//        VecCl ret(int(IonLevels.size()));
//        for(int k=1;k<=int(IonLevels.size());k++)
//             if (-IonLevels[k-1]/T>-100)
//                ret[k]=Degeneration[k-1]*exp(-IonLevels[k-1]/T);
//             else 
//                 ret[k] = 0;
//        ret = ret/(ret*(ret*0+1));
//        return ret;
//    }
//protected:
//    vector<double> IonLevels, Degeneration;
//    double MolVeight;
//    
//    
//}; 
//
////Calculates FreeE, derivative of electron component
////Will be depricated...
//struct PureIon_StrictClc:RefCount{
//    PureIon_StrictClc(InterfaceFreeEIO *freeeelectron){
//        FreeEelectron = freeeelectron;
//        SubstVeights.SetDim(1); SubstVeights[1]=1;
//        DeriveConst = M_Eps2*10;
//    }
//    double SetElDenc(){
//        return ElDenc;
//    }
//    void SetElDenc(double temp, double eldenc){
//        ElDenc = max(eldenc,MathZer); 
//        ElE = FreeEelectron->FreeE(eldenc, temp);
//        ElEder = (FreeEelectron->FreeE(eldenc*(1+0.5*DeriveConst), temp)-FreeEelectron->FreeE(eldenc*(1-0.5*DeriveConst), temp))/(eldenc*DeriveConst);
//    }
//    double FreeENumDeriv(double IonNum, double M0=1){
//        double ret = -ElDenc*ElE*SubstVeights[1]/sqr(M0)+IonNum/M0*(ElE+ElDenc*ElEder);
//        return ret;
//    }
//    double ElFreeE(){
//        return ElE;
//    }
//    double ElFreeEderive(){
//        return ElEder;
//    }
//    double ElE, ElEder;
//private:
//    Ref<InterfaceFreeEIO> FreeEelectron;
//    VecCl IonNum, SubstVeights;
//    double DeriveConst;
//    double ElDenc;
//};
//
//
//
//
//
//struct FreeEIonOneMat : FreeEIonAdd {
//    FreeEIonOneMat() : FreeEIonAdd(), ElFreeE(new FreeEIdeal), MatFreeE(new FreeEIdeal) {
//        SetInternalNames();
//    };
//    double FreeE(double Denc,double T) {
//     if ((T<MathZer) || (Denc<MathZer)) 
//        { cout<<"FreeE Bad. Denc "<<Denc<<" T "<<T<<"\n";return 0;}
//     ClcDataStruct FixData = MakeStartData(Denc, T);
//     double ElDenc = ClcElDenc(FixData);
//     vector<double> lev = ClcIonLevels(FixData, ClcElDenc(FixData));
//     double fStrict = ClcFreeEStrict(FixData, lev, ElDenc);
//     return fStrict;
//
//    }
//    void ReadSubstancePar(istream &in) {
//        FreeEIonAdd::ReadSubstancePar(in);
//        char tmp[256];
//        SavableClass *ptrEl, *ptrMat;
//        in>>tmp>>ptrEl>>tmp>>ptrMat>>tmp>>ZeroTemp>>tmp>>ElDencErr>>tmp>>DeltaMulCoef;
//        if (!in || !(ElFreeE<<ptrEl) || !(MatFreeE<<ptrMat))
//            throw info_except("Could not read matter...\n");
//        Electron = new PureIon_StrictClc(ElFreeE);
//
////ClcDataStruct FixData = MakeStartData(1e-5, 15000);
////VecCl tmp1 = VecCl::MakeStepVector(11,-10,0,0);
////for(int i = 1; i<= tmp1.Dim(); i++)
////    ClcIonLevelsCout(FixData, pow(10,tmp1[i]));
//
//    }
//    void SaveSubstancePar(ostream &out) {
//        FreeEIonAdd::SaveSubstancePar(out);
//        out<<" ElectronFreeE "<<ElFreeE<<" MatterFreeE "<<MatFreeE<<" ZeroTemp "<<ZeroTemp<<" ElDencErr "<<ElDencErr<<" DeltaMulCoef_crack "<<DeltaMulCoef;
//    }
//
//
//    Stroka MakeHelp(){
//        Stroka res = FreeEIonAdd::MakeHelp() + "Calc FreeE taking into account ions. Aprox - interaction part for ions - the same. Uses EOS of Mat, Electrons...\n"+
//        FreeEIOFind::MakeHelp();
//        return res;
//    }
//    struct ClcDataStruct{
//        double FixMatE, FixMatT, FixMatD;
//    };
//    virtual vector<double> GetElectronLevels(const ClcDataStruct &FixData){
//        return IonLevels;
//    }
//
//
//
//	virtual void SetInternalNames(){
//		FreeEIonAdd::SetInternalNames();
//		int tmp = 1;
//		for (size_t k=0;k<=IonLevels.size();k++)
//            OutputNames[Stroka("IonLev_")+Stroka::Int2Str(int(k))] = &tmp;
//  	}
//	virtual double OutputValClc(const map<Stroka,double> &Input, const Stroka &Output){
//		Stroka Test1 = Stroka(Output,0, 6);
//        if (Test1==Stroka("IonLev")){
//		    int k = atoi(Stroka(Output, 7, Output.length()-7).c_str());
//            assert(k>=0 && k<(int)IonLevels.size());
//            ClcDataStruct FixData = MakeStartData(CheckPresence(Input, "Dencity"), CheckPresence(Input, "Temperature"));
//            double ElDenc = ClcElDenc(FixData);
//            return ClcIonLevels(FixData, ClcElDenc(FixData))[k];
//		}
//		return FreeEIonAdd::OutputValClc(Input, Output);
//	}
//
//
//protected:
//    VecCl GetIonNum(double Denc, double T){
//         ClcDataStruct FixData = MakeStartData(Denc, T);
//         double ElDenc = ClcElDenc(FixData);
//         return ClcIonNum(FixData, ClcIonLevels(FixData, ElDenc));
//    }
//private:
//    //double ClcFreeESimple1(const ClcDataStruct &FixDat, const vector<double> &IonLev, double ElDenc){
//    //    double kt=M_Rconst/MolVeight*FixDat.FixMatT;
//    //     double add = 0;
//    //     for(size_t k=0;k<IonLev.size();k++)
//    //         if (-IonLev[k]/FixDat.FixMatT>-100)
//    //            add +=Degeneration[k]*exp(-IonLev[k]/FixDat.FixMatT);
//    //     if (add<MathZer)
//    //         add = StndErr;
//    //     double ret=FixDat.FixMatE-kt*log(add);
//    //     return ret;
//    //}
//    //double ClcFreeESimple(const ClcDataStruct &FixDat, const vector<double> &IonLev, double ElDenc){
//    //    double kt=M_Rconst/MolVeight*FixDat.FixMatT;
//    //    double kt_T=M_Rconst/MolVeight;
//    //    VecCl LevNum = ClcIonNum(FixDat, IonLev);
//    //    double Sum = 0;
//    //    for(int k = 1;k<=LevNum.Dim();k++){
//    //        Sum += LevNum[k]*(kt_T*IonLev[k-1]-kt*(log(LevNum[k])+log(Degeneration[k-1])));
//    //    }
//    //    return FixDat.FixMatE+Sum;
//    //}
//    double ClcFreeEStrict(const ClcDataStruct &FixDat, const vector<double> &IonLev, double ElDenc){
//        double kt=M_Rconst/MolVeight*FixDat.FixMatT;
//        double kt_T=M_Rconst/MolVeight;
//        VecCl LevNum = ClcIonNum(FixDat, IonLev);
//        double Sum = 0;
//        vector<double> iLev = GetElectronLevels(FixDat);
//        for(int k = 1;k<=LevNum.Dim();k++){
//            Sum += LevNum[k]*(kt_T*iLev[k-1]-kt*(-log(LevNum[k])+log(Degeneration[k-1])));
//        }
//        Electron->SetElDenc(FixDat.FixMatT, ElDenc);
//        Sum += Electron->ElFreeE()*ElDenc/FixDat.FixMatD;
//        Sum += kt*log(Degeneration[0]); // It is  included already in Fmat...
//        return FixDat.FixMatE+Sum;
//    }
//    Ref<PureIon_StrictClc> Electron;
//
//    Ref<InterfaceFreeEIO> ElFreeE;
//    Ref<InterfaceFreeEIO> MatFreeE;
//    double ZeroTemp, ElDencErr, DeltaMulCoef;
//    ClcDataStruct MakeStartData(double Denc, double Temp){
//        ClcDataStruct ret;
//        ret.FixMatD = Denc;
//        ret.FixMatT = Temp;
//        ret.FixMatE = MatFreeE->FreeE(Denc, Temp);
//        return ret;
//    }
//    double ClcElDenc(const ClcDataStruct &FixDat){
//        double resDenc = LastElDencStatic, ErrorAbs = ElDencErr, ErrorRel = ElDencErr;
//        //return 0.5*FixDat.FixMatD;
//        int err, MaxIter = 1000;
//        FreeEIonOneMat* OldPtr = CurPtr;
//        CurPtr = this;
//        FixDatStore = FixDat;
//        if (3<(err=Fzero(DencMisfitStatic, -60 ,log(FixDat.FixMatD*IonLevels.size()) , resDenc, ErrorAbs, ErrorRel, MaxIter))){
//            if ( fabs(resDenc+60)>1 || DencMisfitStatic(resDenc)<0 ){
//			cout<<" Bad FZero GetElDenc in FreeEIonOneMat \nfound Denc "
//				<<exp(resDenc)<<" Mis "<<DencMisfitStatic(resDenc)<<" err "<<err<<" logD "<<resDenc<<"\n";
//            }
//        }
//        CurPtr = OldPtr;
//        LastElDencStatic = resDenc;
//        return exp(resDenc);
//    }
//    static FreeEIonOneMat* CurPtr;
//    static double LastElDencStatic;
//    static double DencMisfitStatic(double ElDenc){
//        return CurPtr->DencMisfit(ElDenc);
//    }
//    ClcDataStruct FixDatStore;
//    double DencMisfit(double logElDenc){
//        double ElDenc = exp(logElDenc);
//        double ClcD = ClcElDenc(FixDatStore, ElDenc);
//        return log(ElDenc/ClcD);
//    }
//    double ClcElDenc(const ClcDataStruct &FixDat, double ElDenc){
//        VecCl IonNum = ClcIonNum(FixDat, ClcIonLevels(FixDat, ElDenc));
//        double ElCoef = 0;
//        for(int k=2;k<=IonNum.Dim();k++)
//            ElCoef += (k-1)*IonNum[k];
//        return FixDat.FixMatD*ElCoef;
//    }
//    void ClcIonLevelsCout(const ClcDataStruct &FixDat, double ElDenc){
//        vector<double> lev(IonLevels.size());
//        Electron->SetElDenc(FixDat.FixMatT, ElDenc);
//cout<<"Electron "<<Electron->ElFreeE()<<" Denc "<<ElDenc<<" Temp "<<FixDat.FixMatT<<" ======================================\n";
//        double Shift = Electron->FreeENumDeriv(0);
//        double Coef = MolVeight/M_Rconst;
//        vector<double> iLev = GetElectronLevels(FixDat);
//        for(int k=0;k<(int)IonLevels.size();k++){
//            lev[k] = iLev[k]+(Electron->FreeENumDeriv(1)-Shift)*Coef*k;
//cout<<" k "<<k<<" ILev "<<iLev[k] <<" add "<< (Electron->FreeENumDeriv(1) - Shift) * Coef * k<<" res "
//<<lev[k]/(-FixDat.FixMatT)+ log(Degeneration[k]*FixDat.FixMatT)<<"\n";
//        }
//        VecCl ret(lev), l(lev);
//        double min = VecCl::Min(l);
//        l = l-min;
////cout<<"ElectronCoef "<<l*(-1)/FixDat.FixMatT;
//        for(int k=1;k<=int(lev.size());k++){
//             if (l[k]/FixDat.FixMatT<200)
//                 ret[k]=Degeneration[k-1]*exp(-l[k]/FixDat.FixMatT);
//             else 
//                 ret[k] = Degeneration[k-1]*exp(-200.);
//        }
//        ret = ret/(ret*(ret*0+1));
//cout<<" elV "<<ret;
//    }
//
//
//
//    vector<double> ClcIonLevels(const ClcDataStruct &FixDat, double ElDenc){
//        vector<double> lev(IonLevels.size());
//        Electron->SetElDenc(FixDat.FixMatT, ElDenc);
//        double Shift = Electron->FreeENumDeriv(0);
//        double Coef = MolVeight/M_Rconst;
//        vector<double> iLev = GetElectronLevels(FixDat);
//        for(int k=0;k<(int)IonLevels.size();k++){
//            //lev[k] = iLev[k]+(Electron->FreeENumDeriv(k)-Shift)*Coef*k;
//            lev[k] = iLev[k]+(Electron->FreeENumDeriv(1)-Shift)*Coef*k;
//        }
//        return lev;
//    }
//
//    VecCl ClcIonNum(const ClcDataStruct &FixDat, const vector<double> &lev){
//        VecCl ret(int(lev.size())), l(int(lev.size()));
//        for(int k=1;k<=int(lev.size());k++)
//            l[k] = lev[k-1];
//        double min = VecCl::Min(l);
//        l = l-min;
////cout<<"ElectronCoef "<<l*(-1)/FixDat.FixMatT;
//        for(int k=1;k<=int(lev.size());k++){
//             if (l[k]/FixDat.FixMatT<200)
//                 ret[k]=Degeneration[k-1]*exp(-l[k]/FixDat.FixMatT);
//             else 
//                 ret[k] = Degeneration[k-1]*exp(-200.);
//        }
//        ret = ret/(ret*(ret*0+1));
//        return ret;
//    }
//
//    
//};
//
//struct RosLowering : OneVarFunction {
//    RosLowering() : OneVarFunction(), V0(1), A0(0), A1(1) {}
//    virtual double Calculate(double x){
//        if (x>=V0)
//            return A0;
//        else
//            return A0 + A1 * sqr(x-V0);
//    }
//
//   virtual int save_data_state(FilterOut&so){
//       so<<" V0 "<<V0<<" A0 "<<A0<<" A1 "<<A1;
//       return 1; 
//   };
//   virtual int read_data_state(FilterIn&si) {
//       char tmp[256];
//       si>>tmp>>V0>>tmp>>A0>>tmp>>A1;
//       return 1;
//   };
//
//    Stroka MakeHelp(){
//        return "Lowering of energy levels Pot = F(V) \n    V>V0: Pot = A0 \n     V<V0: Pot = A0 - A1*(V-V0)^2\n";
//    }
//private:
//    double V0, A0, A1;
//};
//
//struct FreeEIonLowVolDepend : FreeEIonOneMat  {
//    FreeEIonLowVolDepend(): FreeEIonOneMat() {
//        VolFuncs.push_back(new RosLowering); 
//    }
//    void ReadSubstancePar(istream &in) {
//        FreeEIonOneMat::ReadSubstancePar(in);
//        char tmp[256];
//        SavableClass *ptr;
//        in>>tmp>>tmp;
//        if (stricmp("{", tmp) != 0)
//            throw info_except("Bad input. Had to read <{> and read <%s>\n", tmp);
//        VolFuncs.clear();
//        while(!GuessNextChar(in, '}')){
//            in>>ptr;
//            Ref<OneVarFunction> ptrF; ptrF<<ptr;
//            VolFuncs.push_back(ptrF);
//        }
//    }
//    void SaveSubstancePar(ostream &out) {
//        FreeEIonOneMat::SaveSubstancePar(out);
//        out<<" VolDependentFunctions { ";
//        for(size_t i = 0; i < VolFuncs.size(); i++)
//            out<<VolFuncs[i];
//        out<<" } ";
//    }
//    Stroka MakeHelp(){
//        return FreeEIonOneMat::MakeHelp() +  "taking into account volume dependence of electron ionization levels\n";
//    }
//    virtual vector<double> GetElectronLevels(const ClcDataStruct &FixData){
//        vector<double> ionLev; ionLev = IonLevels;
//        //DataManip::CopyVector(ionLev, IonLevels);
//        double V = 1. / max(MathZer, FixData.FixMatD);
//        for(size_t i = 0; i < VolFuncs.size(); i++)
//            if (VolFuncs[i].nonnull())
//                ionLev[i] += VolFuncs[i]->Calculate(V);
////cout<<" D "<<FixData.FixMatD<<" V "<<V<<" lev "<<ionLev;
//        return ionLev;
//    }
//
//private:
//    vector<Ref<OneVarFunction> > VolFuncs;
//};
