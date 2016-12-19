#pragma once

#include "fre_fac.h"
//#include "mat\NdimMin.h"
//#include "mat\fun_fac.h"
#include "fre_ion.h"
#include "lib/ref/str_manip.h"



namespace ClcIonisation {



// ===========================================================================================================
// ===========================================================================================================
// ===========================================================================================================


// denc = mol/cm3
    struct MathAnisIon{
        MathAnisIon();
        static Str::OptionSet CalcType;//"DebyAnis Deby NoDeby"
        int CalcMethod;
        double MolVeight;
        void Init(const vector<double> &ionpot, const vector<double> &gfactor, const vector<int> &n_num, const vector<int> &l_num, Ref<ElectronStub> electron, int calcmethod,
            double molveight){
            IonPot = ( VecCl(ionpot)*M_Rconst ).Copy2Vector();
            Gfactor = gfactor;
            N_num = n_num;
            L_num = l_num;
            Electron = electron;
            CalcType.Check(calcmethod, "Bat ionization calculation type");
            CalcMethod = calcmethod;
            DeltaElDenc = (0.5);
            MolVeight = molveight;
        }

        double ClcFreeE(double denc, double temp){
            SetClcConst(denc, temp);
            vector<double> IonNum = ClcIonNum();
            return FreeE(IonNum);
        }


        vector<double> ClcIonNumSimple(){
            VecCl ionnum((int)N_num.size());
            //ionnum = ionnum*0;
            //ionnum[1] = 1;
            ionnum = ionnum*0 + 1;
            ionnum[1] = 0;
            ionnum = ionnum/(ionnum*(ionnum*0+1));
            VecCl newnum(ionnum);
            int it = 0, MaxIter = 1000;
            do {
                ionnum = newnum*0.2+ionnum*0.8;
                ClcIonNum(ionnum, newnum);
cout<<" mis "<<VecCl::ModMax(ionnum-newnum)<<" ionnum "<<ionnum;
            } while ((VecCl::ModMax(ionnum-newnum)>StndErr) && it++ < MaxIter);
            return newnum.Copy2Vector();
        }
        vector<double> ClcIonNum(){
            VecCl ionnum;
            DeltaIonNum.SetDim((int)N_num.size());
            DeltaIonNum = DeltaIonNum*0 + 1;
            DeltaIonNum = DeltaIonNum/(DeltaIonNum*(DeltaIonNum*0+1));
            int it = 0, MaxIter = 0;
            //DeltaElDenc = 0.5;
            do {
                ionnum = DeltaIonNum;
                double logDenc = log(DeltaElDenc);
                MathAnisIon *OldPtr = CurPtr;
                double Err = 1e-12;
                CurPtr = this;
                if (!Fzero(DeltaNumEStatic, -50, log(ionnum.Dim()-1.), logDenc, Err, Err, 1000))
                    cout<<"Fzero bad - result is x<"<<DeltaElDenc<<"> y(0)="<<DeltaNumEStatic(DeltaElDenc)<<"\n";
                CurPtr = OldPtr;
                DeltaElDenc = exp(logDenc);
                DeltaNumE(logDenc); // Setting DeltaIonNum
                ClcElNum(DeltaIonNum.Copy2Vector(), DeltaElDenc, DeltaEl2Denc);                

//cout<<" mis "<<VecCl::ModMax(ionnum-DeltaIonNum)<<" ionnum "<<ionnum<<" dDenc "<<exp(logDenc)-DeltaElDenc<<"\n";
            } while ((VecCl::ModMax(ionnum-DeltaIonNum)>StndErr) && it++ < MaxIter);
            return DeltaIonNum.Copy2Vector();
        }
        const map<Stroka, double>& GetInternalParams(){
            return InternalParams;
        }
    private:
        vector<double> IonPot, Gfactor;
        vector<int> N_num, L_num;
        Ref<ElectronStub> Electron;

//        double Denc;//, Temp;
        map<Stroka, double> InternalParams;

        double clcCoef, clcCoef1, clcCoef2, clcDenc, clcTemp;
        vector<double> clcLc0, clcLc1, clcLc2, clcLc3;

        void SetClcConst(double denc, double temp){
            clcDenc = denc;
            clcTemp = temp;
            clcCoef = M_Rconst*temp;
            double Charge = M_ElectronCGSCharge;
            double sq = sqrt(M_PI*clcDenc/clcCoef);
            clcCoef1 = -2*pow(Charge,3)/3*sq;
            clcCoef2 = 2*Charge*sq;

            vector<double> clcCoef3;
            clcCoef3.resize(N_num.size());
            clcLc0.resize(N_num.size());
            clcLc1.resize(N_num.size());
            clcLc2.resize(N_num.size());
            clcLc3.resize(N_num.size());
            for(size_t i = 0;i<clcCoef3.size();i++){
                clcCoef3[i] = (sqr(N_num[i])-L_num[i]*(L_num[i]-1))/(2*N_num[i]);
                clcLc0[i] = IonPot[i]-clcCoef*log(Gfactor[i]);
                if (IonPot[i]==0 || CalcMethod!=0)
                    clcLc1[i] = clcLc2[i] = clcLc3[i] = 0;
                else {
                    clcLc1[i] = -(1+2*clcCoef3[i])*sqrt(IonPot[i]*0.5);
                    clcLc2[i] = clcCoef3[i]*(1+0.5*clcCoef3[i]);
                    clcLc3[i] = -sqr(clcCoef3[i])/(2*sqrt(IonPot[i]*2));
                }
            }
        }

        double iterElNum, iterEl2Num;
        double iterElFree, iterElDeriv;
        vector<double> iterPot, iterNum;

        void ClcElNum(const vector<double> &ionnum, double &ElNum, double &El2Num){
            ElNum = MathZer;
            El2Num = MathZer;
            for(size_t i =0;i<iterPot.size();i++){
                ElNum += i*ionnum[i];
                El2Num += i*i*ionnum[i];
            }
            El2Num += ElNum;
        }
        void SetIterConst(const vector<double> &ionnum){
            iterNum = ionnum;
            iterPot.resize(iterNum.size());
            ClcElNum(ionnum, iterElNum, iterEl2Num);
            SetIterConst_(iterElNum, iterEl2Num);
        }
        void SetIterConst(double ElNum){
            int N = (int)L_num.size();
            iterPot.resize(N);
            VecCl tmp(N);
            tmp = tmp*0 + (1-ElNum/(N-1))/N;
            tmp[N] = ElNum/(N-1);
            //tmp = tmp/(tmp*(tmp*0+1));
            iterNum = tmp.Copy2Vector();
            iterElNum = ElNum;
            iterEl2Num = DeltaEl2Denc;
            SetIterConst_(iterElNum, iterEl2Num);
        }
        void SetIterConst_(double ElNum, double El2Num){
            double LambdaI = clcCoef2*iterEl2Num;
            for(size_t i =0;i<iterPot.size();i++)
                iterPot[i] = clcLc0[i];
            if (CalcMethod==0){
                for(size_t i =0;i<iterPot.size();i++)
                    iterPot[i] += clcLc1[i]*LambdaI + clcLc2[i]*sqr(LambdaI) + clcLc3[i]*pow(LambdaI,3);
            }
            iterElFree = Electron->ClcFreeE(iterElNum*clcDenc, clcTemp);
            iterElDeriv = Electron->ClcFreeEDeriv(iterElNum*clcDenc, clcTemp);
cout<<" New freeE "<<iterElFree/MolVeight<<" Denc "<<iterElNum*clcDenc*MolVeight<<" Temp "<<clcTemp;//<<"\n";
        }
        double FreeE(const vector<double> &ionnum){
            SetIterConst(ionnum);
            double res = iterElNum*iterElFree;
            for(size_t i =0;i<iterPot.size();i++)
                res += iterNum[i]*(iterPot[i]+clcCoef*log(iterNum[i]));
            if (CalcMethod<2)
                res += clcCoef1*pow(iterEl2Num, 1.5);
            return res;
        }
        void ClcIonNum(VecCl &ionnum, VecCl &newnum){
            SetIterConst(ionnum.Copy2Vector());
            newnum = ClcIonNum_();
        }
        VecCl ClcIonNum_(){
            VecCl newnum((int)L_num.size()), logRelat((int)L_num.size());
            for(int k = 1;k<=logRelat.Dim();k++)
                logRelat[k] = ClcLogRelat(0, k-1);
            double max = VecCl::Max(logRelat);
            logRelat = logRelat - max;
cout<<"CoefNew "<<logRelat;
            for(int k = 1;k<=logRelat.Dim();k++)
                if (logRelat[k]>-200)
                    newnum[k] = exp(logRelat[k]);
                else
                    newnum[k] = exp(-200.);
            newnum = newnum/(newnum*(newnum*0+1));
            return newnum;
        }
        double ClcLogRelat(int i1, int i2){
        //double ClcLogRelat(int i2, int i1){
            double IonNumMul = (i2 - i1)*(1+i1+i2);
            double Lc1sum = 0, Lc2sum = 0, Lc3sum = 0; 
            for(size_t i = 0;i<clcLc0.size();i++){
                Lc1sum += clcLc1[i]*iterNum[i];
                Lc2sum += clcLc2[i]*iterNum[i];
                Lc3sum += clcLc3[i]*iterNum[i];
            }
            double res = (clcLc0[i1] - clcLc0[i2]) + ( i1 - i2 )*( iterElFree + iterElNum*clcDenc*iterElDeriv );
//cout<<" iterElFree "<<iterElFree<<" iterElNum "<<iterElNum<<" iterElDeriv "<<iterElDeriv<<" sum "<<iterElFree + iterElNum*clcDenc*iterElDeriv; 
//cout<<" IonLev "<<IonPot[i2]<<"\n";
if (i1!=i2)
cout<<" Delta "<<( i1 - i2 )*( iterElFree + iterElNum*clcDenc*iterElDeriv )/M_Rconst<<" IonLev "<<-IonPot[i2]/M_Rconst<<"\n";
            if (CalcMethod<2){
                res += ( -1.5*sqrt(iterEl2Num)*(clcCoef1 + pow(clcCoef2,3)) - 
                          0.5*clcCoef2*Lc1sum/sqrt(iterEl2Num) - Lc2sum*sqr(clcCoef2) )*IonNumMul;
                res += (clcLc1[i1] - clcLc1[i2])*sqrt(iterEl2Num)*clcCoef2 + 
                       (clcLc2[i1] - clcLc2[i2])*iterEl2Num*sqr(clcCoef2) +
                       (clcLc3[i1] - clcLc3[i2])*pow(iterEl2Num, 1.5)*pow(clcCoef2,3);
            }
            //return -res/clcCoef;
//cout<<" Lev "<<i1<<":"<<i2<<" "<<res<<" norm "<<res/clcCoef<<" ion "<<IonPot[i2]<<"\n";
            return res/clcCoef;
        }


        static MathAnisIon *CurPtr;
        static double DeltaNumEStatic(double numel){
            return CurPtr->DeltaNumE(numel);
        }
        double DeltaNumE(double logElDenc){
            double ElDenc = exp(logElDenc);
            double ClcD = ClcElDenc(ElDenc);
            return log(ElDenc/ClcD);
        }
        double DeltaElDenc, DeltaEl2Denc;
        VecCl DeltaIonNum;
        double ClcElDenc(double ElDenc){
            SetIterConst(ElDenc);
            DeltaIonNum = ClcIonNum_();
            double res, res2;
            ClcElNum(DeltaIonNum.Copy2Vector(), res, res2);
            return res;
        }


    };


}; // ClcIonisation