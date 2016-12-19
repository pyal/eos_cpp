#include <lib/precompiled/lex_anal_lib.h>

#include "lex_anal_test.h"


static ClassDesc TestCase_Manip_ClassDesc(typeid(TestCase_Manip),"TestCase_Manip",1,"TestCase_Manip_category",create<TestCase_Manip>);
static ClassDesc TestCase_Lex_Analyser_ClassDesc(typeid(TestCase_Lex_Analyser),"TestCase_Lex_Analyser",1,"TestCase_Lex_Analyser_category",create<TestCase_Lex_Analyser>);



TestCase_Manip::TestCase_Manip():in(NULL),buf(NULL), manip(NULL){};
