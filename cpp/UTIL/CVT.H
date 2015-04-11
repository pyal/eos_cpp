
#include "util.h"

//#ifndef WCPP
#include "ex_out.h"
//#


#define MaxArrPnt      1500

#define MaxCodeNumber 100
#define MaxFoundCode 32000
//#define MaxFoundCode 200
#define MaxCodeLength 30
#define MaxInputBuf  32000
//#define MaxInputBuf  200


//void find_code() // procedure which finds all codes (different) in the input
//void order_code() //procedure which set all codes in oder deleting extra ones
//void code_page() // procedure which codes page by table
//void code_subst()// procedure which substitutes combinations, saves output
                 //  to the fstream.
//void unlimited_change()

struct code_struct
  {
   int NumCodes,CurCode;
   char *code[MaxCodeNumber+1];
   int  Priority[MaxCodeNumber+1];
   int  Buf_Len[MaxCodeNumber+1];
   code_struct(){NumCodes=0;CurCode=0;code[0]=NULL;Buf_Len[0]=0;Priority[0]=0;}
   void ControlCode(int &N)
     {
      if (NumCodes==0) N=0;
      else if ((N>NumCodes) || (N<=0)) N=1;
     };
   void AddCode(char *name,int buf_len,int prior=0)
     {
      if (NumCodes==MaxCodeNumber) return;
      NumCodes++;
      code[NumCodes]=new char[buf_len];MemoryMove(name,code[NumCodes],buf_len);
      Priority[NumCodes]=prior;
      Buf_Len [NumCodes]=buf_len;
//cout<<" AddCode "<<NumCodes<<" name "<<name<<" buf_len "<<buf_len<<"\n";
     };
//   int GetNumCodes();
   ~code_struct();
  };

struct code_config   
  { 
   int position,code_n;
   friend ostream& operator<<(ostream &output,code_config &config) 
     { 
      output<<" Position "<<config.position<<" Code_Number "<<config.code_n<<"\n";
      return output;
     }
  };

