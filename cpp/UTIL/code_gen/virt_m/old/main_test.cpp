#include "lex_anal.h"
#include "lib\test\lib\test_case.h"
#include "lex_anal_test.h"





void main( int argc, char *argv[] )
{
   char *progname = argv[0];
   if (argc < 2)
   {
      cout<<"usage: "<<progname<<" file.name\n";
      exit( 1 );
   }

   char *input_file_name = argv[1];
   fstream in(input_file_name,ios::in|ios::binary);

   if( !in )
   {  cout<<"Cannot open file: "<<input_file_name<<"\n"; exit( 1 );   }

   Lex_Analyser anal(in);
   //TestSuite::RunTest("TestCase_Manip");
   //TestSuite::RunTest("TestCase_Lex_Analyser");
  while (!(!in)) anal.Analyse();
}
