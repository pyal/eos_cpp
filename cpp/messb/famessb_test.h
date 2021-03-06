#ifndef _FAMESSB_TEST_H_
#define _FAMESSB_TEST_H_

#include "lib\test_fact\test_case.h"
#include "famessb_test_resource.h"
#include "lib\test_fact\resource_reader.h"
//#include <vector>
extern int CopyFile(const char *src, const char *dst);


struct ResourcesStruct {
    int data;
    int config;
    double err;
    const char *Name;
};

struct TestCase_Famessb_Static : TestCase_Std   //TestCase_Buffered
{
    static const char *TestExeOutput;
    static int DeleteGoodResults;
    static int PrintDiscrepencyEstimations;
    TestCase_Famessb_Static(){};   //:manip(NULL),in(NULL),buf(NULL){};
    ~TestCase_Famessb_Static() {
        Finish();
    }
    TestSuite *CreateTest() {
        TestSuite_Std *suite = new TestSuite_Std(class_name());
        suite->AddTest(Test_Func, "Test_Func");
        return suite;
    }
    void Init() {
        Finish();
    }
    void
    Finish(){};   //{delete in;in=NULL;delete buf;buf=NULL;delete manip;manip=NULL;	}
    void Text_Func_Out(ResourcesStruct *res, int ResLen, const char *name);
    int GetHi(string &name, double &Hi, string &Err);
    int Test_External_Func(ResourcesStruct *resources, int Length, string &Err);
    static TestCase_Famessb_Static *Restore(DescribedClass *ptr) {
        TestCase_Famessb_Static *cur = dynamic_cast<TestCase_Famessb_Static *>(ptr);
        if(!cur)
            throw(info_except("ptr is not TestCase_Famessb_Static"));
        return cur;
    }
    static void Test_Func(TestCase *ptr) {
        TestCase_Famessb_Static *cur = Restore(ptr);
        unlink(TestExeOutput);
        string err, ResErr, Message;
        ResourcesStruct HQ_GAM_resources[] = {
            {T_HQ_GAM_Q4_IS_1_DAT, T_HQ_GAM_Q4_IS_1_CFG, 1e-6, "T_HQ_GAM_Q4_IS_1"},
            {T_HQ_GAM_Q4_T0_G0_E0_P0_DAT,
             T_HQ_GAM_Q4_T0_G0_E0_P0_CFG,
             1e-6,
             "T_HQ_GAM_Q4_T0_G0_E0_P0"},
            {T_HQ_GAM_Q4_T0_G90_E0_P0_DAT,
             T_HQ_GAM_Q4_T0_G90_E0_P0_CFG,
             1e-6,
             "T_HQ_GAM_Q4_T0_G90_E0_P0"},
            {T_HQ_GAM_Q4_T30_G30_E1_P50_DAT,
             T_HQ_GAM_Q4_T30_G30_E1_P50_CFG,
             1e-6,
             "T_HQ_GAM_Q4_T30_G30_E1_P50"},
            {T_HQ_GAM_Q4_T90_G90_E1_P_0_DAT,
             T_HQ_GAM_Q4_T90_G90_E1_P_0_CFG,
             1e-6,
             "T_HQ_GAM_Q4_T90_G90_E1_P_0"},
            {T_HQ_GAM_Q4_T90_G90_E1_P_50_DAT,
             T_HQ_GAM_Q4_T90_G90_E1_P_50_CFG,
             1e-6,
             "T_HQ_GAM_Q4_T90_G90_E1_P_50"}};
        ResourcesStruct HQ_resources[] = {
            {T_HQ_Q0_DAT, T_HQ_Q0_CFG, 1e-6, "T_HQ_Q0"},
            {T_HQ_Q4_DAT, T_HQ_Q4_CFG, 1e-6, "T_HQ_Q4"},
            {T_HQ_Q4_E1_DAT, T_HQ_Q4_E1_CFG, 1e-6, "T_HQ_Q4_E1"},
            {T_HQ_Q4_T90_DAT, T_HQ_Q4_T90_CFG, 1e-6, "T_HQ_Q4_T90"},
            {T_HQ_Q4_T90_E1_DAT, T_HQ_Q4_T90_E1_CFG, 1e-6, "T_HQ_Q4_T90_E1"},
            {T_HQ_Q4_T90_P90_E1_DAT, T_HQ_Q4_T90_P90_E1_CFG, 1e-6, "T_HQ_Q4_T90_P90_E1"}};
        ResourcesStruct MONO_resources[] = {
            {T_MONO_Q0_T0_P0_DAT, T_MONO_Q0_T0_P0_CFG, 1e-6, "T_MONO_Q0_T0_P0"},
            {T_MONO_Q0_T54_DAT, T_MONO_Q0_T54_CFG, 1e-6, "T_MONO_Q0_T54"},
            {T_MONO_Q0_T90_P0_DAT, T_MONO_Q0_T90_P0_CFG, 1e-6, "T_MONO_Q0_T90_P0"},
            {T_MONO_Q4_T0_P0_DAT, T_MONO_Q4_T0_P0_CFG, 1e-6, "T_MONO_Q4_T0_P0"},
            {T_MONO_Q4_T54_P0_DAT, T_MONO_Q4_T54_P0_CFG, 1e-6, "T_MONO_Q4_T54_P0"},
            {T_MONO_Q4_T90_P0_DAT, T_MONO_Q4_T90_P0_CFG, 1e-6, "T_MONO_Q4_T90_P0"},
            {T_MONO_Q4_TQ_30_GQ30_QH_0_E1_T90_P0_DAT,
             T_MONO_Q4_TQ_30_GQ30_QH_0_E1_T90_P0_CFG,
             1e-6,
             "T_MONO_Q4_TQ_30_GQ30_QH_0_E1_T90_P0"},
            {T_MONO_Q4_TQ_30_GQ30_QH_60_E1_T90_P0_DAT,
             T_MONO_Q4_TQ_30_GQ30_QH_60_E1_T90_P0_CFG,
             1e-6,
             "T_MONO_Q4_TQ_30_GQ30_QH_60_E1_T90_P0"}};
        ResourcesStruct QH_resources[] = {
            {T_QH_E0_T30_P0_DAT, T_QH_E0_T30_P0_CFG, 1e-6, "T_QH_E0_T30_P0"},
            {T_QH_E0_T30_P30_DAT, T_QH_E0_T30_P30_CFG, 1e-6, "T_QH_E0_T30_P30"},
            {T_QH_E1_T30_P0_DAT, T_QH_E1_T30_P0_CFG, 1e-6, "T_QH_E1_T30_P0"},
            {T_QH_E1_T30_P30_DAT, T_QH_E1_T30_P30_CFG, 1e-6, "T_QH_E1_T30_P30"},
            {T_QH_Q0_IS_0_DAT, T_QH_Q0_IS_0_CFG, 1e-6, "T_QH_Q0_IS_0"},
            {T_QH_Q4_IS_0_DAT, T_QH_Q4_IS_0_CFG, 1e-6, "T_QH_Q4_IS_0"},
            {T_QH_Q4_IS_1_DAT, T_QH_Q4_IS_1_CFG, 1e-6, "T_QH_Q4_IS_1"}};

        ResourcesStruct AVER_resources[] = {
            {T_AVER_H100_Q4_IS1_E1_T30_P60_IX50_IY50_IZ100_HC400_TV45_A18_DAT,
             T_AVER_H100_Q4_IS1_E1_T30_P60_IX50_IY50_IZ100_HC400_TV45_A18_CFG,
             1e-6,
             "T_AVER_H100_Q4_IS1_E1_T30_P60_IX50_IY50_IZ100_HC400_TV45_A18"},
            {T_AVER_H1_Q0_IS0_E0_T0_P0_IX0_IY0_IZ0_HC400_TV0_A18_DAT,
             T_AVER_H1_Q0_IS0_E0_T0_P0_IX0_IY0_IZ0_HC400_TV0_A18_CFG,
             1e-6,
             "T_AVER_H1_Q0_IS0_E0_T0_P0_IX0_IY0_IZ0_HC400_TV0_A18"},
            {T_AVER_H1_Q0_IS0_E0_T0_P0_IX0_IY0_IZ0_HC400_TV90_A18_DAT,
             T_AVER_H1_Q0_IS0_E0_T0_P0_IX0_IY0_IZ0_HC400_TV90_A18_CFG,
             1e-6,
             "T_AVER_H1_Q0_IS0_E0_T0_P0_IX0_IY0_IZ0_HC400_TV90_A18"},
            {T_AVER_H1_Q0_IS0_E0_T0_P0_IX0_IY0_IZ100_HC400_TV0_A18_DAT,
             T_AVER_H1_Q0_IS0_E0_T0_P0_IX0_IY0_IZ100_HC400_TV0_A18_CFG,
             1e-6,
             "T_AVER_H1_Q0_IS0_E0_T0_P0_IX0_IY0_IZ100_HC400_TV0_A18"},
            {T_AVER_H1_Q0_IS0_E0_T0_P0_IX0_IY100_IZ0_HC400_TV0_A18_DAT,
             T_AVER_H1_Q0_IS0_E0_T0_P0_IX0_IY100_IZ0_HC400_TV0_A18_CFG,
             1e-6,
             "T_AVER_H1_Q0_IS0_E0_T0_P0_IX0_IY100_IZ0_HC400_TV0_A18"},
            {T_AVER_H1_Q4_IS0_E1_T30_P60_IX0_IY0_IZ100_HC400_TV0_A18_DAT,
             T_AVER_H1_Q4_IS0_E1_T30_P60_IX0_IY0_IZ100_HC400_TV0_A18_CFG,
             1e-6,
             "T_AVER_H1_Q4_IS0_E1_T30_P60_IX0_IY0_IZ100_HC400_TV0_A18"},
            {T_AVER_H1_Q4_IS1_E1_T30_P60_IX0_IY0_IZ100_HC400_TV0_A18_DAT,
             T_AVER_H1_Q4_IS1_E1_T30_P60_IX0_IY0_IZ100_HC400_TV0_A18_CFG,
             1e-6,
             "T_AVER_H1_Q4_IS1_E1_T30_P60_IX0_IY0_IZ100_HC400_TV0_A18"}};
        ResourcesStruct RAND_JUMP_resources[] = {
            {T_RAND_JUMP_H400_Q0_IS0_E0_T0_P0_NA1_NT1_TT100_AT0_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0_DAT,
             T_RAND_JUMP_H400_Q0_IS0_E0_T0_P0_NA1_NT1_TT100_AT0_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0_CFG,
             1e-6,
             "T_RAND_JUMP_H400_Q0_IS0_E0_T0_P0_NA1_NT1_TT100_AT0_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0"},
            {T_RAND_JUMP_H400_Q4_IS0_E1_T60_P0_NA1_NT1_TT100_AT100_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0_DAT,
             T_RAND_JUMP_H400_Q4_IS0_E1_T60_P0_NA1_NT1_TT100_AT100_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0_CFG,
             1e-6,
             "T_RAND_JUMP_H400_Q4_IS0_E1_T60_P0_NA1_NT1_TT100_AT100_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0"},
            {T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0_DAT,
             T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0_CFG,
             1e-6,
             "T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0"},
            {T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX2_TDISP5_TA0_PA60_PRA1_TB0_PB240_PRB1_DAT,
             T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX2_TDISP5_TA0_PA60_PRA1_TB0_PB240_PRB1_CFG,
             1e-6,
             "T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX2_TDISP5_TA0_PA60_PRA1_TB0_PB240_PRB1"},
            {T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX2_TDISP5_TA0_PA60_PRA30_TB0_PB240_PRB30_DAT,
             T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX2_TDISP5_TA0_PA60_PRA30_TB0_PB240_PRB30_CFG,
             1e-6,
             "T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX2_TDISP5_TA0_PA60_PRA30_TB0_PB240_PRB30"},
            {T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX2_TDISP5_TA60_PA0_PRA1_TB240_PB0_PRB1_DAT,
             T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX2_TDISP5_TA60_PA0_PRA1_TB240_PB0_PRB1_CFG,
             1e-6,
             "T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX2_TDISP5_TA60_PA0_PRA1_TB240_PB0_PRB1"},
            {T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX2_TDISP5_TA60_PA0_PRA30_TB240_PB0_PRB30_DAT,
             T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX2_TDISP5_TA60_PA0_PRA30_TB240_PB0_PRB30_CFG,
             1e-6,
             "T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_NAX2_TDISP5_TA60_PA0_PRA30_TB240_PB0_PRB30"},
            {T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV1_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0_DAT,
             T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV1_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0_CFG,
             1e-6,
             "T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV1_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0"},
            {T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP1_AP1_NOP0_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0_DAT,
             T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP1_AP1_NOP0_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0_CFG,
             1e-6,
             "T_RAND_JUMP_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP1_AP1_NOP0_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0"},
            {T_RAND_JUMP_H400_Q4_IS1_E1_T60_P0_NA1_NT1_TT100_AT100_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0_DAT,
             T_RAND_JUMP_H400_Q4_IS1_E1_T60_P0_NA1_NT1_TT100_AT100_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0_CFG,
             1e-6,
             "T_RAND_JUMP_H400_Q4_IS1_E1_T60_P0_NA1_NT1_TT100_AT100_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA0_PA0_PRA1_TB0_PB0_PRB0"},
            {T_RAND_JUMP_H400_Q4_IS1_E1_T60_P0_NA1_NT1_TT100_AT100_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA60_PA90_PRA1_TB0_PB0_PRB0_DAT,
             T_RAND_JUMP_H400_Q4_IS1_E1_T60_P0_NA1_NT1_TT100_AT100_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA60_PA90_PRA1_TB0_PB0_PRB0_CFG,
             1e-6,
             "T_RAND_JUMP_H400_Q4_IS1_E1_T60_P0_NA1_NT1_TT100_AT100_TP0_AP0_NOP1_HV0_NAX1_TDISP5_TA60_PA90_PRA1_TB0_PB0_PRB0"}};
        ResourcesStruct RAND_WALK_resources[] = {
            {T_RAND_WALK_H400_Q4_IS0_E1_T0_P0_NA1_NT1_TT100_AT100_TP0_AP0_NOP1_HV0_DAT,
             T_RAND_WALK_H400_Q4_IS0_E1_T0_P0_NA1_NT1_TT100_AT100_TP0_AP0_NOP1_HV0_CFG,
             1e-6,
             "T_RAND_WALK_H400_Q4_IS0_E1_T0_P0_NA1_NT1_TT100_AT100_TP0_AP0_NOP1_HV0"},
            {T_RAND_WALK_H400_Q4_IS0_E1_T0_P0_NA5_NT5_TT1_AT100_TP0_AP0_NOP1_HV0_DAT,
             T_RAND_WALK_H400_Q4_IS0_E1_T0_P0_NA5_NT5_TT1_AT100_TP0_AP0_NOP1_HV0_CFG,
             1e-6,
             "T_RAND_WALK_H400_Q4_IS0_E1_T0_P0_NA5_NT5_TT1_AT100_TP0_AP0_NOP1_HV0"},
            {T_RAND_WALK_H400_Q4_IS1_E1_T0_P0_NA30_NT10_TT1_AT1_TP0_AP0_NOP1_HV1_DAT,
             T_RAND_WALK_H400_Q4_IS1_E1_T0_P0_NA30_NT10_TT1_AT1_TP0_AP0_NOP1_HV1_CFG,
             1e-6,
             "T_RAND_WALK_H400_Q4_IS1_E1_T0_P0_NA30_NT10_TT1_AT1_TP0_AP0_NOP1_HV1"},
            {T_RAND_WALK_H400_Q4_IS1_E1_T0_P0_NA30_NT10_TT1_AT1_TP1_AP1_NOP0_HV0_DAT,
             T_RAND_WALK_H400_Q4_IS1_E1_T0_P0_NA30_NT10_TT1_AT1_TP1_AP1_NOP0_HV0_CFG,
             1e-6,
             "T_RAND_WALK_H400_Q4_IS1_E1_T0_P0_NA30_NT10_TT1_AT1_TP1_AP1_NOP0_HV0"},
            {T_RAND_WALK_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_DAT,
             T_RAND_WALK_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0_CFG,
             1e-6,
             "T_RAND_WALK_H400_Q4_IS1_E1_T0_P0_NA30_NT5_TT1_AT1_TP0_AP0_NOP1_HV0"},
            {T_RAND_WALK_H400_Q4_IS1_E1_T0_P0_NA5_NT5_TT1_AT100_TP0_AP0_NOP1_HV0_DAT,
             T_RAND_WALK_H400_Q4_IS1_E1_T0_P0_NA5_NT5_TT1_AT100_TP0_AP0_NOP1_HV0_CFG,
             1e-6,
             "T_RAND_WALK_H400_Q4_IS1_E1_T0_P0_NA5_NT5_TT1_AT100_TP0_AP0_NOP1_HV0"}};
        ResourcesStruct RELAX2_resources[] = {
            {T_RELAX2_H1_Q0_IS0_E0_T0_P0_HT0_HP0_H400_Q0_IS0_E0_T0_P0_HT0_HP0_T11000_T21_DAT,
             T_RELAX2_H1_Q0_IS0_E0_T0_P0_HT0_HP0_H400_Q0_IS0_E0_T0_P0_HT0_HP0_T11000_T21_CFG,
             1e-6,
             "T_RELAX2_H1_Q0_IS0_E0_T0_P0_HT0_HP0_H400_Q0_IS0_E0_T0_P0_HT0_HP0_T11000_T21"},
            {T_RELAX2_H1_Q0_IS0_E0_T0_P0_HT0_HP0_H400_Q4_IS0_E1_T30_P60_HT0_HP0_T11000_T21_DAT,
             T_RELAX2_H1_Q0_IS0_E0_T0_P0_HT0_HP0_H400_Q4_IS0_E1_T30_P60_HT0_HP0_T11000_T21_CFG,
             1e-6,
             "T_RELAX2_H1_Q0_IS0_E0_T0_P0_HT0_HP0_H400_Q4_IS0_E1_T30_P60_HT0_HP0_T11000_T21"},
            {T_RELAX2_H1_Q0_IS0_E0_T0_P0_HT0_HP0_H400_Q4_IS0_E1_T30_P60_HT15_HP75_T11000_T21_DAT,
             T_RELAX2_H1_Q0_IS0_E0_T0_P0_HT0_HP0_H400_Q4_IS0_E1_T30_P60_HT15_HP75_T11000_T21_CFG,
             1e-6,
             "T_RELAX2_H1_Q0_IS0_E0_T0_P0_HT0_HP0_H400_Q4_IS0_E1_T30_P60_HT15_HP75_T11000_T21"},
            {T_RELAX2_H1_Q0_IS0_E0_T0_P0_HT0_HP0_H400_Q4_IS1_E1_T30_P60_HT15_HP75_T11000_T21_DAT,
             T_RELAX2_H1_Q0_IS0_E0_T0_P0_HT0_HP0_H400_Q4_IS1_E1_T30_P60_HT15_HP75_T11000_T21_CFG,
             1e-6,
             "T_RELAX2_H1_Q0_IS0_E0_T0_P0_HT0_HP0_H400_Q4_IS1_E1_T30_P60_HT15_HP75_T11000_T21"},
            {T_RELAX2_H400_Q0_IS0_E0_T0_P0_HT0_HP0_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T110000000_T210000000_DAT,
             T_RELAX2_H400_Q0_IS0_E0_T0_P0_HT0_HP0_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T110000000_T210000000_CFG,
             1e-6,
             "T_RELAX2_H400_Q0_IS0_E0_T0_P0_HT0_HP0_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T110000000_T210000000"},
            {T_RELAX2_H400_Q0_IS0_E0_T0_P0_HT0_HP0_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21_DAT,
             T_RELAX2_H400_Q0_IS0_E0_T0_P0_HT0_HP0_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21_CFG,
             1e-6,
             "T_RELAX2_H400_Q0_IS0_E0_T0_P0_HT0_HP0_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21"},
            {T_RELAX2_H400_Q0_IS0_E0_T0_P0_HT0_HP0_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21000_DAT,
             T_RELAX2_H400_Q0_IS0_E0_T0_P0_HT0_HP0_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21000_CFG,
             1e-6,
             "T_RELAX2_H400_Q0_IS0_E0_T0_P0_HT0_HP0_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21000"},
            {T_RELAX2_H400_Q4_IS0_E1_T30_P60_HT0_HP0_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21000_DAT,
             T_RELAX2_H400_Q4_IS0_E1_T30_P60_HT0_HP0_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21000_CFG,
             1e-6,
             "T_RELAX2_H400_Q4_IS0_E1_T30_P60_HT0_HP0_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21000"},
            {T_RELAX2_H400_Q4_IS0_E1_T30_P60_HT15_HP75_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21000_DAT,
             T_RELAX2_H400_Q4_IS0_E1_T30_P60_HT15_HP75_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21000_CFG,
             1e-6,
             "T_RELAX2_H400_Q4_IS0_E1_T30_P60_HT15_HP75_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21000"},
            {T_RELAX2_H400_Q4_IS1_E1_T30_P60_HT15_HP75_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21000_DAT,
             T_RELAX2_H400_Q4_IS1_E1_T30_P60_HT15_HP75_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21000_CFG,
             1e-6,
             "T_RELAX2_H400_Q4_IS1_E1_T30_P60_HT15_HP75_H1_Q0_IS0_E0_T0_P0_HT0_HP0_T11_T21000"}};
        ResourcesStruct RELAXHN_resources[] = {
            {T_RELAXHN_H0_Q0_IS0_E0_T0_P0_N1_TA1_HA400_TA0_PA0_TB0_HB0_TB0_PB0_TC0_HC0_TC0_PC0_DAT,
             T_RELAXHN_H0_Q0_IS0_E0_T0_P0_N1_TA1_HA400_TA0_PA0_TB0_HB0_TB0_PB0_TC0_HC0_TC0_PC0_CFG,
             1e-6,
             "T_RELAXHN_H0_Q0_IS0_E0_T0_P0_N1_TA1_HA400_TA0_PA0_TB0_HB0_TB0_PB0_TC0_HC0_TC0_PC0"},
            {T_RELAXHN_H0_Q4_IS0_E1_T60_P30_N1_TA1_HA400_TA0_PA0_TB0_HB0_TB0_PB0_TC0_HC0_TC0_PC0_DAT,
             T_RELAXHN_H0_Q4_IS0_E1_T60_P30_N1_TA1_HA400_TA0_PA0_TB0_HB0_TB0_PB0_TC0_HC0_TC0_PC0_CFG,
             1e-6,
             "T_RELAXHN_H0_Q4_IS0_E1_T60_P30_N1_TA1_HA400_TA0_PA0_TB0_HB0_TB0_PB0_TC0_HC0_TC0_PC0"},
            {T_RELAXHN_H0_Q4_IS0_E1_T60_P30_N1_TA1_HA400_TA75_PA15_TB0_HB0_TB0_PB0_TC0_HC0_TC0_PC0_DAT,
             T_RELAXHN_H0_Q4_IS0_E1_T60_P30_N1_TA1_HA400_TA75_PA15_TB0_HB0_TB0_PB0_TC0_HC0_TC0_PC0_CFG,
             1e-6,
             "T_RELAXHN_H0_Q4_IS0_E1_T60_P30_N1_TA1_HA400_TA75_PA15_TB0_HB0_TB0_PB0_TC0_HC0_TC0_PC0"},
            {T_RELAXHN_H0_Q4_IS1_E1_T60_P30_N1_TA1_HA400_TA75_PA15_TB0_HB0_TB0_PB0_TC0_HC0_TC0_PC0_DAT,
             T_RELAXHN_H0_Q4_IS1_E1_T60_P30_N1_TA1_HA400_TA75_PA15_TB0_HB0_TB0_PB0_TC0_HC0_TC0_PC0_CFG,
             1e-6,
             "T_RELAXHN_H0_Q4_IS1_E1_T60_P30_N1_TA1_HA400_TA75_PA15_TB0_HB0_TB0_PB0_TC0_HC0_TC0_PC0"},
            {T_RELAXHN_H0_Q4_IS1_E1_T60_P30_N2_TA10000_HA400_TA75_PA15_TB10000_HB_400_TB75_PB15_TC0_HC0_TC0_PC0_DAT,
             T_RELAXHN_H0_Q4_IS1_E1_T60_P30_N2_TA10000_HA400_TA75_PA15_TB10000_HB_400_TB75_PB15_TC0_HC0_TC0_PC0_CFG,
             1e-6,
             "T_RELAXHN_H0_Q4_IS1_E1_T60_P30_N2_TA10000_HA400_TA75_PA15_TB10000_HB_400_TB75_PB15_TC0_HC0_TC0_PC0"},
            {T_RELAXHN_H0_Q4_IS1_E1_T60_P30_N2_TA1_HA400_TA75_PA15_TB1_HB_400_TB75_PB15_TC0_HC0_TC0_PC0_DAT,
             T_RELAXHN_H0_Q4_IS1_E1_T60_P30_N2_TA1_HA400_TA75_PA15_TB1_HB_400_TB75_PB15_TC0_HC0_TC0_PC0_CFG,
             1e-6,
             "T_RELAXHN_H0_Q4_IS1_E1_T60_P30_N2_TA1_HA400_TA75_PA15_TB1_HB_400_TB75_PB15_TC0_HC0_TC0_PC0"},
            {T_RELAXHN_H0_Q4_IS1_E1_T60_P30_N3_TA1_HA400_TA75_PA15_TB1_HB_400_TB75_PB15_TC1_HC200_TC35_PC80_DAT,
             T_RELAXHN_H0_Q4_IS1_E1_T60_P30_N3_TA1_HA400_TA75_PA15_TB1_HB_400_TB75_PB15_TC1_HC200_TC35_PC80_CFG,
             1e-6,
             "T_RELAXHN_H0_Q4_IS1_E1_T60_P30_N3_TA1_HA400_TA75_PA15_TB1_HB_400_TB75_PB15_TC1_HC200_TC35_PC80"}};
        ResourcesStruct RELAX_MEAN_resources[] = {
            {T_RELAX_MEAN_H400_Q0_IS0_E0_T0_P0_NH30_C2D1_AT100_DAT,
             T_RELAX_MEAN_H400_Q0_IS0_E0_T0_P0_NH30_C2D1_AT100_CFG,
             1e-6,
             "T_RELAX_MEAN_H400_Q0_IS0_E0_T0_P0_NH30_C2D1_AT100"},
            {T_RELAX_MEAN_H400_Q4_IS0_E1_T0_P0_NH30_C2D1_AT100_DAT,
             T_RELAX_MEAN_H400_Q4_IS0_E1_T0_P0_NH30_C2D1_AT100_CFG,
             1e-6,
             "T_RELAX_MEAN_H400_Q4_IS0_E1_T0_P0_NH30_C2D1_AT100"},
            {T_RELAX_MEAN_H400_Q4_IS1_E1_T0_P0_NH30_C2D1_AT100_DAT,
             T_RELAX_MEAN_H400_Q4_IS1_E1_T0_P0_NH30_C2D1_AT100_CFG,
             1e-6,
             "T_RELAX_MEAN_H400_Q4_IS1_E1_T0_P0_NH30_C2D1_AT100"},
            {T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH160_C2D0_AT1_DAT,
             T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH160_C2D0_AT1_CFG,
             1e-6,
             "T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH160_C2D0_AT1"},
            {T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH160_C2D1_AT1_DAT,
             T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH160_C2D1_AT1_CFG,
             1e-6,
             "T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH160_C2D1_AT1"},
            {T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH30_C2D0_AT10_DAT,
             T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH30_C2D0_AT10_CFG,
             1e-6,
             "T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH30_C2D0_AT10"},
            {T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH30_C2D0_AT100_DAT,
             T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH30_C2D0_AT100_CFG,
             1e-6,
             "T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH30_C2D0_AT100"},
            {T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH30_C2D1_AT10_DAT,
             T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH30_C2D1_AT10_CFG,
             1e-6,
             "T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH30_C2D1_AT10"},
            {T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH30_C2D1_AT100_DAT,
             T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH30_C2D1_AT100_CFG,
             1e-6,
             "T_RELAX_MEAN_H400_Q4_IS1_E1_T60_P30_NH30_C2D1_AT100"}};

        ResourcesStruct KASH_EST_resources[] = {
            {T_KASH_EST_H400_Q0_IS0_E0_T0_P0_T20_TB40_A1_A1_W_DAT,
             T_KASH_EST_H400_Q0_IS0_E0_T0_P0_T20_TB40_A1_A1_W_CFG,
             1e-6,
             "T_KASH_EST_H400_Q0_IS0_E0_T0_P0_T20_TB40_A1_A1_W"},
            {T_KASH_EST_H400_Q0_IS1_E1_T60_P30_T20_TB40_A10_A30_W_DAT,
             T_KASH_EST_H400_Q0_IS1_E1_T60_P30_T20_TB40_A10_A30_W_CFG,
             1e-6,
             "T_KASH_EST_H400_Q0_IS1_E1_T60_P30_T20_TB40_A10_A30_W"},
            {T_KASH_EST_H400_Q2_IS0_E1_T60_P30_T20_TB40_A1_A1_W_DAT,
             T_KASH_EST_H400_Q2_IS0_E1_T60_P30_T20_TB40_A1_A1_W_CFG,
             1e-6,
             "T_KASH_EST_H400_Q2_IS0_E1_T60_P30_T20_TB40_A1_A1_W"},
            {T_KASH_EST_H400_Q2_IS1_E1_T60_P30_T20_TB40_A10_A10_W_DAT,
             T_KASH_EST_H400_Q2_IS1_E1_T60_P30_T20_TB40_A10_A10_W_CFG,
             1e-6,
             "T_KASH_EST_H400_Q2_IS1_E1_T60_P30_T20_TB40_A10_A10_W"},
            {T_KASH_EST_H400_Q2_IS1_E1_T60_P30_T20_TB40_A10_A30_W_DAT,
             T_KASH_EST_H400_Q2_IS1_E1_T60_P30_T20_TB40_A10_A30_W_CFG,
             1e-6,
             "T_KASH_EST_H400_Q2_IS1_E1_T60_P30_T20_TB40_A10_A30_W"},
            {T_KASH_EST_H400_Q4_IS0_E0_T0_P0_T20_TB40_A1_A1_W_DAT,
             T_KASH_EST_H400_Q4_IS0_E0_T0_P0_T20_TB40_A1_A1_W_CFG,
             1e-6,
             "T_KASH_EST_H400_Q4_IS0_E0_T0_P0_T20_TB40_A1_A1_W"},
            {T_KASH_EST_H400_Q4_IS0_E1_T90_P0_T20_TB40_A1_A1_W_DAT,
             T_KASH_EST_H400_Q4_IS0_E1_T90_P0_T20_TB40_A1_A1_W_CFG,
             1e-6,
             "T_KASH_EST_H400_Q4_IS0_E1_T90_P0_T20_TB40_A1_A1_W"}};
        ResourcesStruct KASH_RELAX_resources[] = {
            {T_KASH_RELAX_H400_Q0_IS0_E0_T0_P0_NT1000_RT150_FD1000_SS0_P0_T90_DAT,
             T_KASH_RELAX_H400_Q0_IS0_E0_T0_P0_NT1000_RT150_FD1000_SS0_P0_T90_CFG,
             1e-6,
             "T_KASH_RELAX_H400_Q0_IS0_E0_T0_P0_NT1000_RT150_FD1000_SS0_P0_T90"},
            {T_KASH_RELAX_H400_Q0_IS0_E0_T0_P0_NT1000_RT150_FD1000_SS1_P0_T90_DAT,
             T_KASH_RELAX_H400_Q0_IS0_E0_T0_P0_NT1000_RT150_FD1000_SS1_P0_T90_CFG,
             1e-6,
             "T_KASH_RELAX_H400_Q0_IS0_E0_T0_P0_NT1000_RT150_FD1000_SS1_P0_T90"},
            {T_KASH_RELAX_H400_Q0_IS0_E0_T0_P0_NT100_RT50_FD10_SS10_P0_T90_DAT,
             T_KASH_RELAX_H400_Q0_IS0_E0_T0_P0_NT100_RT50_FD10_SS10_P0_T90_CFG,
             1e-6,
             "T_KASH_RELAX_H400_Q0_IS0_E0_T0_P0_NT100_RT50_FD10_SS10_P0_T90"},
            {T_KASH_RELAX_H400_Q0_IS0_E0_T0_P0_NT2000_RT150_FD2000_SS0_P0_T90_DAT,
             T_KASH_RELAX_H400_Q0_IS0_E0_T0_P0_NT2000_RT150_FD2000_SS0_P0_T90_CFG,
             1e-6,
             "T_KASH_RELAX_H400_Q0_IS0_E0_T0_P0_NT2000_RT150_FD2000_SS0_P0_T90"},
            {T_KASH_RELAX_H400_Q2_IS1_E0_T0_P0_NT100_RT500_FD100_SS0_P0_T90_DAT,
             T_KASH_RELAX_H400_Q2_IS1_E0_T0_P0_NT100_RT500_FD100_SS0_P0_T90_CFG,
             1e-6,
             "T_KASH_RELAX_H400_Q2_IS1_E0_T0_P0_NT100_RT500_FD100_SS0_P0_T90"},
            {T_KASH_RELAX_H400_Q2_IS1_E0_T90_P90_NT100_RT500_FD100_SS0_P0_T90_DAT,
             T_KASH_RELAX_H400_Q2_IS1_E0_T90_P90_NT100_RT500_FD100_SS0_P0_T90_CFG,
             1e-6,
             "T_KASH_RELAX_H400_Q2_IS1_E0_T90_P90_NT100_RT500_FD100_SS0_P0_T90"},
            {T_KASH_RELAX_H400_Q2_IS1_E1_T60_P30_NT100_RT500_FD100_SS0_P0_T90_DAT,
             T_KASH_RELAX_H400_Q2_IS1_E1_T60_P30_NT100_RT500_FD100_SS0_P0_T90_CFG,
             1e-6,
             "T_KASH_RELAX_H400_Q2_IS1_E1_T60_P30_NT100_RT500_FD100_SS0_P0_T90"}};
        ResourcesStruct EXTERNAL_H_resources[] = {
            {T_EXTERNAL_H_H0_1_Q0_IS0_E0_T0_G0_P0_IX0_IY0_IZ0_HX10_HY0_HZ0_HC400_TV0_N8S1_DAT,
             T_EXTERNAL_H_H0_1_Q0_IS0_E0_T0_G0_P0_IX0_IY0_IZ0_HX10_HY0_HZ0_HC400_TV0_N8S1_CFG,
             1e-6,
             "T_EXTERNAL_H_H0_1_Q0_IS0_E0_T0_G0_P0_IX0_IY0_IZ0_HX10_HY0_HZ0_HC400_TV0_N8S1"},
            {T_EXTERNAL_H_H0_1_Q4_IS0_E0_T0_G0_P0_IX0_IY0_IZ0_HX10_HY0_HZ0_HC400_TV0_N8S1_DAT,
             T_EXTERNAL_H_H0_1_Q4_IS0_E0_T0_G0_P0_IX0_IY0_IZ0_HX10_HY0_HZ0_HC400_TV0_N8S1_CFG,
             1e-6,
             "T_EXTERNAL_H_H0_1_Q4_IS0_E0_T0_G0_P0_IX0_IY0_IZ0_HX10_HY0_HZ0_HC400_TV0_N8S1"},
            {T_EXTERNAL_H_H0_1_Q4_IS0_E0_T0_G0_P0_IX10_IY10_IZ0_HX0_HY0_HZ0_HC400_TV0_N8S1_DAT,
             T_EXTERNAL_H_H0_1_Q4_IS0_E0_T0_G0_P0_IX10_IY10_IZ0_HX0_HY0_HZ0_HC400_TV0_N8S1_CFG,
             1e-6,
             "T_EXTERNAL_H_H0_1_Q4_IS0_E0_T0_G0_P0_IX10_IY10_IZ0_HX0_HY0_HZ0_HC400_TV0_N8S1"},
            {T_EXTERNAL_H_H0_1_Q4_IS0_E1_T30_G25_P30_IX0_IY0_IZ0_HX10_HY0_HZ0_HC400_TV0_N8S1_DAT,
             T_EXTERNAL_H_H0_1_Q4_IS0_E1_T30_G25_P30_IX0_IY0_IZ0_HX10_HY0_HZ0_HC400_TV0_N8S1_CFG,
             1e-6,
             "T_EXTERNAL_H_H0_1_Q4_IS0_E1_T30_G25_P30_IX0_IY0_IZ0_HX10_HY0_HZ0_HC400_TV0_N8S1"},
            {T_EXTERNAL_H_H0_1_Q4_IS1_E1_T30_G25_P30_IX10_IY20_IZ30_HX10_HY20_HZ30_HC400_TV0_N8S1_DAT,
             T_EXTERNAL_H_H0_1_Q4_IS1_E1_T30_G25_P30_IX10_IY20_IZ30_HX10_HY20_HZ30_HC400_TV0_N8S1_CFG,
             1e-6,
             "T_EXTERNAL_H_H0_1_Q4_IS1_E1_T30_G25_P30_IX10_IY20_IZ30_HX10_HY20_HZ30_HC400_TV0_N8S1"},
            {T_EXTERNAL_H_H0_1_Q4_IS1_E1_T30_G25_P30_IX10_IY20_IZ30_HX10_HY20_HZ30_HC400_TV90_N8S1_DAT,
             T_EXTERNAL_H_H0_1_Q4_IS1_E1_T30_G25_P30_IX10_IY20_IZ30_HX10_HY20_HZ30_HC400_TV90_N8S1_CFG,
             1e-6,
             "T_EXTERNAL_H_H0_1_Q4_IS1_E1_T30_G25_P30_IX10_IY20_IZ30_HX10_HY20_HZ30_HC400_TV90_N8S1"},
            {T_EXTERNAL_H_H200_Q4_IS1_E1_T30_G25_P30_IX10_IY20_IZ30_HX10_HY20_HZ30_HC200_TV90_N8S0_DAT,
             T_EXTERNAL_H_H200_Q4_IS1_E1_T30_G25_P30_IX10_IY20_IZ30_HX10_HY20_HZ30_HC200_TV90_N8S0_CFG,
             1e-6,
             "T_EXTERNAL_H_H200_Q4_IS1_E1_T30_G25_P30_IX10_IY20_IZ30_HX10_HY20_HZ30_HC200_TV90_N8S0"}};


        //    cur->Text_Func_Out(&HQ_EIL_resources[0],sizeof(HQ_EIL_resources)/sizeof(ResourcesStruct),"HQ_EIL");

        cur->Text_Func_Out(
            &HQ_GAM_resources[0],
            sizeof(HQ_GAM_resources) / sizeof(ResourcesStruct),
            "HQ_GAM");
        cur->Text_Func_Out(
            &HQ_resources[0], sizeof(HQ_resources) / sizeof(ResourcesStruct), "HQ");
        cur->Text_Func_Out(
            &MONO_resources[0], sizeof(MONO_resources) / sizeof(ResourcesStruct), "MONO");
        cur->Text_Func_Out(
            &QH_resources[0], sizeof(QH_resources) / sizeof(ResourcesStruct), "QH");
        cur->Text_Func_Out(
            &RELAX2_resources[0],
            sizeof(RELAX2_resources) / sizeof(ResourcesStruct),
            "RELAX2");
        cur->Text_Func_Out(
            &RELAXHN_resources[0],
            sizeof(RELAXHN_resources) / sizeof(ResourcesStruct),
            "RELAXHN");
        cur->Text_Func_Out(
            &RELAX_MEAN_resources[0],
            sizeof(RELAX_MEAN_resources) / sizeof(ResourcesStruct),
            "RELAX_MEAN");
        // now is excluded function ....
        //cur->Text_Func_Out(&AVER_resources[0],sizeof(AVER_resources)/sizeof(ResourcesStruct),"AVER");
        cur->Text_Func_Out(
            &EXTERNAL_H_resources[0],
            sizeof(EXTERNAL_H_resources) / sizeof(ResourcesStruct),
            "EXTERNAL_H");
        cur->Text_Func_Out(
            &KASH_EST_resources[0],
            sizeof(KASH_EST_resources) / sizeof(ResourcesStruct),
            "KASH_EST");
        cur->Text_Func_Out(
            &RAND_JUMP_resources[0],
            sizeof(RAND_JUMP_resources) / sizeof(ResourcesStruct),
            "RAND_JUMP");
        cur->Text_Func_Out(
            &RAND_WALK_resources[0],
            sizeof(RAND_WALK_resources) / sizeof(ResourcesStruct),
            "RAND_WALK");
        cur->Text_Func_Out(
            &KASH_RELAX_resources[0],
            sizeof(KASH_RELAX_resources) / sizeof(ResourcesStruct),
            "KASH_RELAX");
    }
    //istream *in;
    //char *buf;
    //StreamManip_Base *manip;
};


//struct TestCase_Lex_Analyser:TestCase_Buffered
//{
//	TestCase_Lex_Analyser():lex_an(NULL),in(NULL),buf(NULL){};
//	~TestCase_Lex_Analyser(){Finish();}
//	TestSuite* CreateTest()
//	{
//		TestSuite_Std* suite=new TestSuite_Std(class_name());
//		suite->AddTest(Test_String,"Test_String");
//		suite->AddTest(Test_Number,"Test_Number");
//		suite->AddTest(Test_Token,"Test_Token");
//
//		return suite;
//	}
//	void Init()
//	{
//		Finish();//          10
//	}
//	void Finish()	{delete in;in=NULL;delete buf;buf=NULL;delete lex_an;lex_an=NULL;	}
//	istream *in;
//	char *buf;
//	Lex_Analyser *lex_an;
//	static TestCase_Lex_Analyser* Start(DescribedClass *ptr,const char *str)
//	{
//		TestCase_Lex_Analyser *cur=dynamic_cast<TestCase_Lex_Analyser*>(ptr);
//		if (!cur) throw(TestExeption("ptr is not TestCase_Lex_Analyser"));
//		cur->buf=strdup(str);//"\"test stream\"\n123456789\n1e23\n1e\n1.e"
//		cur->in=new strstream(cur->buf,strlen(cur->buf),ios::in);
//		cur->lex_an=new Lex_Analyser(*cur->in,0);
//		//cur->lex_an=new Lex_Analyser(*cur->in,1);// to draw all the results
//		return cur;
//	}
//	static void CheckRes(Lex_Result *fst,Lex_Result *sec)
//	{
//		if (Lex_Result::Equals(fst,sec))
////			throw(TestExeption("OK"));
//		return;
//		if (fst==NULL) throw(TestExeption(string("Lex result returned NULL and have to return \n").append(sec->ToString()).c_str()));
//		if (sec==NULL) throw(TestExeption(string("Lex result returned ").append(fst->ToString()).append(" and have to return NULL ").c_str()));
//		throw(TestExeption(string("Lex result returned ").append(fst->ToString()).append(" and have to return  ").append(sec->ToString()).c_str()));
//	};
//	static void Test_String(TestCase *ptr)
//	{
//		TestCase_Lex_Analyser *cur=Start(ptr,"\"Start\"\t\"Another\034\" \t\'\035\'\t\n\
//\"aa\
//a\naa\"");
//		Lex_Result *res,tobe(0,0,"Start",Lex_Result::String);
//		res=cur->lex_an->Analyse();
//		CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,0,"Another\034",Lex_Result::String);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,0,"\035",Lex_Result::Char);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,0,"aaa\naa",Lex_Result::String);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		CheckRes(res,NULL);
//	}
//	static void Test_Number(TestCase *ptr)
//	{
//		TestCase_Lex_Analyser *cur=Start(ptr,"123 0x3a 022 022.\t1e-1 1.\n 1e23 1.e1 1e 1.e");
//		Lex_Result *res,tobe;
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(123,0,"",Lex_Result::NumberInt);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0x3a,0,"",Lex_Result::NumberInt);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(022,0,"",Lex_Result::NumberInt);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,22,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,0.1,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,1,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,1e23,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,10,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(1,0,"",Lex_Result::NumberInt);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,0,"e",Lex_Result::Identifier);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,1,"",Lex_Result::NumberDouble);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(0,0,"e",Lex_Result::Identifier);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		CheckRes(res,NULL);
//	}
//	static void Test_Token(TestCase *ptr)
//	{
//		TestCase_Lex_Analyser *cur=Start(ptr,"+ - word wor\t word + word +");
//		Lex_Result *res,tobe;
//		cur->lex_an->PushTokenLevel();
//
//		cur->lex_an->AddToken("word",4);
//		cur->lex_an->AddToken("wor",3);
//		cur->lex_an->AddToken("+",1);
//		cur->lex_an->AddToken("-",2);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(1,0,"+",Lex_Result::Token);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(2,0,"-",Lex_Result::Token);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(4,0,"word",Lex_Result::Token);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(3,0,"wor",Lex_Result::Token);CheckRes(res,&tobe);
//
//		cur->lex_an->PushTokenLevel();
//		cur->lex_an->AddToken("word",5);
//
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(5,0,"word",Lex_Result::Token);CheckRes(res,&tobe);
//
//		cur->lex_an->PopTokenLevel();
//
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(1,0,"+",Lex_Result::Token);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(4,0,"word",Lex_Result::Token);CheckRes(res,&tobe);
//		res=cur->lex_an->Analyse();
//		tobe.GenerateResult(1,0,"+",Lex_Result::Token);CheckRes(res,&tobe);
//
//		res=cur->lex_an->Analyse();
//		CheckRes(res,NULL);
//	}
//};
//
#endif