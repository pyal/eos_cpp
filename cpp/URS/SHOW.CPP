 //NumCurve 5 XCurve  1
//x_min  0  x_max  5 y_min  -1  y_max  15  
//Name1 Pos  Color1 10 Show? 1
#define FMT "      "<<setw(13)<<setprecision(6)
#define FM  setw(13)<<setprecision(9)

#include "lib\std\util.h"
#include "old\inc\grlib.h"
//#include "memf.h"
#define ESC 27

#define MaxCurve    10
//#define MaxCurvePnt 2101
#define MaxCurvePnt 8000
                                 
//double pnt[6][512];//=new double [10000];
double *pnt[MaxCurve];
int Color[MaxCurve];
int Show[MaxCurve];
char Name[MaxCurve][20];
int NumCurve;
int Row;
int GetNext(int k,int XCurve)
  {
//cout<<" k "<<k<<" XCurve "<<XCurve<<" NumCurve "<<NumCurve;
   for(int i=k+1;i<NumCurve;i++) if ( (Show[i]) && (i!=XCurve) ) return i;
   return -1;
  };
int FindPnt(int Direction,int NumWave,int Deptth,int SubPnt,double MinDelta)
  {
   for (int k=Row-1;k>1+Deptth+SubPnt;k--) 
     if (fabs(pnt[NumWave][k]-pnt[NumWave][k-Deptth])>MinDelta) break;
   int ret=k-Deptth-SubPnt;
//cout<<ret<<" ";
   return ret;
  };
   

void main(int argc,char *argv[])                             
  {         
   fstream in(argv[1],ios::in);                              
   fstream data(argv[2],ios::in);
   fstream *output=NULL,*outtim=NULL,*outsho=NULL;
   fstream *input=NULL;
   char *tmpptr,*ShockCfg;
   int GetPnt=0,Shock=0;
   int Direction,NumWave,SubPnt,Deptth;// for shock
   double MinDelta;
   if ((argc<2) || (GetCmd("/h",argc,argv)!=NULL))
     { cout<<"usage:\n"<<argv[0]<<"  file_to_show [file_cfg] [options]\n"
           <<" options: /gNum   - Get point number Num store it in show_pnt.dat\n"
           <<"          /sFile_Shock    - Get_Shock_Wave_Front Parameters in show_pnt.sho\n"
           <<"          /tTime  - Get parameters for time in show_pnt.tim\n"
           <<"          file_cfg like:  \n"
           <<"          File_Shock like: \n"
           "      /h - display help\n"; exit(1);  }
   double GetTim=0;
   if ( (tmpptr=GetCmd("/t",argc-1,argv))!=NULL) GetTim=atof(tmpptr);
   if ( (tmpptr=GetCmd("/g",argc-1,argv))!=NULL) GetPnt=atoi(tmpptr);
   if ( (ShockCfg=GetCmd("/s",argc-1,argv))!=NULL) Shock=1;
   char tmp[512],time_str[50];
   char getch=0;                                
   int Fst=1,k,XCurve,i;Row=0;
   double d[MaxCurve];

   Rect Gr;
   ScrText TextArr[MaxCurve],Head;
   TextStrc meth={DEFAULT_FONT,HORIZ_DIR,CENTER_TEXT,BLACK};

   data>>tmp>>NumCurve>>tmp>>XCurve;
   XCurve--;NumCurve=min(NumCurve,MaxCurve);
   data>>tmp>>Gr.dn.x>>tmp>>Gr.up.x>>tmp>>Gr.dn.y>>tmp>>Gr.up.y;
   double x_pos=120,y_pos=10;
   for (k=0;k<NumCurve;k++) 
     {
      data>>tmp>>Name[k]>>tmp>>Color[k]>>tmp>>Show[k];
         pnt[k]=new double [MaxCurvePnt];  // extracted from Show circle
      if (Show[k]) 
        { 
         TextArr[k].SetScrText(Name[k],x_pos,y_pos,meth,Color[k]);y_pos+=12;
        }  
//cout<<" Name "<<Name[k]<<"   ";
     }
//cout<<"   XCurve "<<XCurve<<"\n";ChRead();
//   meth.BkColor=BLUE;
   GrArr *ar[MaxCurve];
   GrScr *Axis;
   GrScr Graphs;
   if ((!GetPnt) && (!GetTim) && (!Shock)) InitGrph(); 

   while ((in)  && (getch!=ESC))                             
     {
      Row=0;   
      in>>tmp>>time_str;in.getline(tmp,511);
      while  ( (EnterStr(in,&d[0],MaxCurve,1)!=NumCurve) && (in) ){};
       do
        {                                                    
         Row++;
         for (k=0;k<NumCurve;k++) 
            if (Show[k]) pnt[k][Row]=d[k+1];
        }                                                   
      while  ( (EnterStr(in,&d[0],MaxCurve,1)==NumCurve) && (Row<MaxCurvePnt-1) && (in));
      if ((Row>1) && (!GetPnt) && (!GetTim) && (!Shock))
        {
//cout<<" Row "<<Row<<"\n";
         if (Fst) 
           {
            k=-1;i=0;
            if ( (k=GetNext(k,XCurve))!=-1)
              {
//cout<<" Fst i "<<i<<" k "<<k;ChRead();
               ar[i]=new GrArr(pnt[XCurve],pnt[k],Row);ar[i]->PixColor(Color[k]);
               Axis=AxGraph(ar[i],Name[XCurve],"");Axis->GrDim(Gr);    
               Axis->Show();
               Graphs.LstAdd(ar[i]);i++;
               TextArr[k].Show();
              }
            while ( (k=GetNext(k,XCurve))!=-1 )
              {
//cout<<" Fst i "<<i<<" k "<<k;ChRead();
               ar[i]=new GrArr(pnt[XCurve],pnt[k],Row);ar[i]->PixColor(Color[k]);
               Graphs.LstAdd(ar[i]);i++;
               TextArr[k].Show();
              }
//Gr=Axis->GrDim();
//cout<<Gr.dn.x<<"   "<<Gr.up.x<<"   "<<Gr.dn.y<<"   "<<Gr.up.y<<"\n";
//Gr=Axis->ScrDim();
//cout<<Gr.dn.x<<"   "<<Gr.up.x<<"   "<<Gr.dn.y<<"   "<<Gr.up.y<<"\n";
//ChRead();
            Graphs.GrDim(Axis->GrDim());
            Graphs.ScrDim(Axis->ScrDim());
//            Graphs.GrDim(Gr);
//            Graphs.ScrDim(ar[0]->CellConv::SetScr());
           }
         else  
           {
            k=-1;i=0;while ( (k=GetNext(k,XCurve))!=-1 )
              {
//cout<<" Stn i "<<i<<" k "<<k;ChRead();
               ar[i]->ArrReset(pnt[XCurve],pnt[k],Row);i++;
              }
           }   
         Graphs.Show();
//cout<<tmp;
         Head.Hide(); Head.SetScrText(time_str,400,10,meth,WHITE);Head.Show();
//cout<<"\n";ChRead();

         getch=ChRead();
         Graphs.Hide();
         Fst=0;
        }
      if ((GetPnt) && (!seek_eof(in)) )
        {
         if (Fst)
           {
            output=new fstream ("show_pnt.dat",ios::out);
            Fst=0;
            k=-1;i=0;
            *output<<"      Time          ";
            while ( (k=GetNext(k,-1))!=-1)  *output<<Name[k]<<"               ";
            *output<<"\n";
           }
         k=-1;i=0;
cout<<" out";
         *output<<FM<<atof(time_str);
         while ( (k=GetNext(k,-1))!=-1)  *output<<FMT<<pnt[k][GetPnt];
         *output<<"\n";
        } 

      if ((GetTim) && (!seek_eof(in)) )
        {
         if (Fst)
           {
            if (atof(time_str)>=GetTim)
              {
               outtim=new fstream ("show_pnt.tim",ios::out);
               Fst=0;
               k=-1;i=0;
               *outtim<<"      Time          "<<FM<<atof(time_str)<<"\n";
               while ( (k=GetNext(k,-1))!=-1)  
                  *outtim<<Name[k]<<"               ";
               *outtim<<"\n";
               k=-1;i=0;
cout<<" out";
               for (int i=1;i<=Row;i++) 
                 { while ( (k=GetNext(k,-1))!=-1) *outtim<<FMT<<pnt[k][i];
                   *outtim<<"\n";
                 }  
              } 
           }     
        } 

      if ((Shock) && (!seek_eof(in)) )
        {
         if (Fst)
           {
            outsho=new fstream ("show_pnt.sho",ios::out);
            input=new fstream (ShockCfg,ios::in);
            *input>>tmp>>Direction>>tmp>>NumWave>>tmp>>Deptth>>tmp>>
                         SubPnt>>tmp>>MinDelta;
            delete input;input=NULL;
            Fst=0;
            k=-1;i=0;
            *outsho<<"      Time          ";
            while ( (k=GetNext(k,-1))!=-1)  *outsho<<Name[k]<<"               ";
            *outsho<<"\n";
            *outsho<<" NumWave "<<NumWave<<" MinDelta "<<MinDelta <<"\n";

           }
         int Pnt=FindPnt(Direction,NumWave,Deptth,SubPnt,MinDelta);
         k=-1;i=0;
//cout<<"\nTime "<<time_str<<" float "<<FM<<atof(time_str)<<"\n";
cout<<" out"<<Pnt<<" ";
         *outsho<<FM<<atof(time_str);
         while ( (k=GetNext(k,-1))!=-1)  *outsho<<FMT<<pnt[k][Pnt];
         *outsho<<FMT<<GetPnt<<"\n";
        } 
     }         
//cout<<" That is All "<<GetPnt<<" "<<GetTim<<" "<<Shock<<" "<<output<<" "<<outtim<<" "<<outsho<<"\n";ChRead();
  if ((!GetPnt) && (!GetTim) && (!Shock))  {delete Axis;closegraph();}
//cout<<" That is All close\n";ChRead();
  if (GetPnt) output->close();if (GetTim) outtim->close();if (Shock) outsho->close();
//cout<<" That is All delete\n";ChRead();
  delete output;delete outtim;delete outsho;
               
cout<<" end";
  };                                                        








/*
      if ((GetPnt) && (!seek_eof(in)) )
        {
         if (Fst)
           {
            output=new fstream ("show_pnt.dat",ios::out);
            Fst=0;
            k=-1;i=0;
            *output<<"      Time          ";
            while ( (k=GetNext(k,-1))!=-1)  *output<<Name[k]<<"               ";
            *output<<"\n";
           }
         k=-1;i=0;
cout<<" out";
         *output<<FM<<atof(time_str);
         while ( (k=GetNext(k,-1))!=-1)  *output<<FMT<<pnt[k][GetPnt];
         *output<<"\n";
        } 


      if ((Shock) && (!seek_eof(in)) )
        {
         if (Fst)
           {
            output=new fstream ("show_pnt.dat",ios::out);
            input=new fstream (ShockCfg,ios::in);
            *input>>tmp>>Direction>>tmp>>NumWave>>tmp>>Deptth>>tmp>>
                         SubPnt>>tmp>>MinDelta;
            delete input;input=NULL;
            Fst=0;
            k=-1;i=0;
            *output<<"      Time          ";
            while ( (k=GetNext(k,-1))!=-1)  *output<<Name[k]<<"               ";
            *output<<"\n";
            *output<<" NumWave "<<NumWave<<" MinDelta "<<MinDelta <<"\n";

           }
         GetPnt=FindPnt(Direction,NumWave,Deptth,SubPnt,MinDelta);
         k=-1;i=0;
cout<<" out";
         *output<<FM<<atof(time_str);
         while ( (k=GetNext(k,-1))!=-1)  *output<<FMT<<pnt[k][GetPnt];
         *output<<FMT<<GetPnt<<"\n";
        } 
     }         


*/
