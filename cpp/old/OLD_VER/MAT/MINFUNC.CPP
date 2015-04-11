#include "minfunc.h"


//=====================================================================
//===============         MinFunc           ===========================
//=====================================================================
   
//=====================================================================
//================       Simple functions   ===========================
//=====================================================================

double MinFunc(double x,double y,X_func Func,double Tol)
  {
   if (MinStrc==NULL) MinStrc=new ClcMinStrc;
   return MinStrc->MinGoldSqr(x,y,Func,Tol);
  };
double MinGold(double x,double y,X_func Func,double Tol)
  {
   if (MinStrc==NULL) MinStrc=new ClcMinStrc;
   return MinStrc->MinGold(x,y,Func,Tol);
  };
double MinSqr(double x,double y,X_func Func,double Tol)
  {
   if (MinStrc==NULL) MinStrc=new ClcMinStrc;
   return MinStrc->MinSqr(x,y,Func,Tol);
  };

void DelMinFunc() { if (MinStrc!=NULL) delete MinStrc;MinStrc=NULL;};


//=====================================================================
//================       Object module      ===========================
//=====================================================================

   ClcMinStrc::ClcMinStrc()
     {clcx=new double[3];Fclcx=new double[3];clcx[0]=2;Fclcx[0]=2;
      MachZer=1;while ( (MachZer+1)!=1 ) MachZer=MachZer/2;
      MachZer=sqrt(MachZer);
      GoldCoef=0.5*(3-sqrt(5));
     };

   double ClcMinStrc::CmpDelMin(double mainx,double newx,double Zer)
     {
      if (fabs(mainx-newx)>Zer) return newx;
      if (mainx>newx) return mainx-Zer;
      return mainx+Zer;
     };
   double ClcMinStrc::MinSqrAppr(double x1,double x2,double x3,
		     double y1,double y2,double y3)
     {
      double a,b,temp;
      temp=(y2-y1)/(x2-x1);
      a=((y3-y1)/(x3-x1)-temp)/(x3-x2);
      b=temp-(x2+x1)*a;
      if (fabs(a)<MachZer) return MathInf;
      return (-b/(2*a));
     };
   void ClcMinStrc::SetBnd(double x1,double x2,double x3,
		       double y1,double y2,double y3)
     { lft=x1;clcx[1]=x2;rgt=x3; Flft=y1;Fclcx[1]=y2;Frgt=y3;  };
   void ClcMinStrc::NextBound()
     {
      if (clcx[1]>clcx[2])
	{ExchVar(&clcx[1],&clcx[2],sizeof(clcx[1]));
	 ExchVar(&Fclcx[1],&Fclcx[2],sizeof(Fclcx[1]));}
      double minBl=min(Flft,Fclcx[1]);
      double minBr=min(Frgt,Fclcx[2]);
      if (minBl<minBr)
	   {SetBnd(lft,clcx[1],clcx[2],Flft,Fclcx[1],Fclcx[2]);MinCurr=minBl;}
      else {SetBnd(clcx[1],clcx[2],rgt,Fclcx[1],Fclcx[2],Frgt);MinCurr=minBr;}
       
     };

   void ClcMinStrc::GoldStp()
     {
      double delt=(rgt-lft);
      if (clcx[1]<lft+delt/2) clcx[2]=rgt-delt*GoldCoef;
      else                    clcx[2]=lft+delt*GoldCoef;
      clcx[2]=CmpDelMin(clcx[1],clcx[2],ZerStp);
      Fclcx[2]=func(clcx[2]);
      NextBound();
     };
   void ClcMinStrc::SqrStp()
     {
      clcx[2]=MinSqrAppr(lft,clcx[1],rgt,Flft,Fclcx[1],Frgt);
      if ( (!In_Lim(clcx[2],lft,rgt)) ||
		     ( fabs(clcx[1]-clcx[2]) <ZerStp ) )
		                                   GoldStp();
      else
	{
	 Fclcx[2]=func(clcx[2]);
	 NextBound();
	}
     };
   void ClcMinStrc::SetBeg(double A,double B,X_func yfunc,double Tol)
     {
      func=yfunc;
      lft=A;rgt=B;Flft=func(lft);Frgt=func(rgt);
      ClcZer=Tol;
      clcx[1]=lft+(rgt-lft)*GoldCoef;Fclcx[1]=func(clcx[1]);
      MinCurr=MathInf;
     };

   double ClcMinStrc::MinGoldSqr(double A,double B,X_func yfunc,double Tol)
     {
      double minold=MathInf;
      SetBeg(A,B,yfunc,Tol);
      while ( (rgt-lft)>2*ClcZer )
	{
//	 minold=MinCurr;
	 ZerStp=MachZer*fabs(clcx[1])+Tol/3;
	 if ( ((clcx[1]-lft)>ZerStp) && ((rgt-clcx[1])>ZerStp) ) SqrStp();
	 else GoldStp();
	};
      return clcx[1];
     };
   double ClcMinStrc::MinNumGoldSqr(double A,double B,X_func yfunc,double Tol)
     {
      double minold=MathInf;
      int NumIter=Tol;Tol=(B-A)*1e-6;
      SetBeg(A,B,yfunc,Tol);
      for (int k=1;k<=NumIter;k++)
	{
//	 minold=MinCurr;
	 ZerStp=MachZer*fabs(clcx[1])+Tol/3;
	 if ( ((clcx[1]-lft)>ZerStp) && ((rgt-clcx[1])>ZerStp) ) SqrStp();
	 else GoldStp();
	};
      return clcx[1];
     };
   double ClcMinStrc::MinGold(double A,double B,X_func yfunc,double Tol)
     {
      SetBeg(A,B,yfunc,Tol);
      while ( (rgt-lft)>2*ClcZer )
	{ ZerStp=MachZer*fabs(clcx[1])+Tol/3;SqrStp();};
      return clcx[1];
     };
   double ClcMinStrc::MinSqr(double A,double B,X_func yfunc,double Tol)
     {
      SetBeg(A,B,yfunc,Tol);
      while ( (rgt-lft)>2*ClcZer )
	{ ZerStp=MachZer*fabs(clcx[1])+Tol/3;SqrStp();  };
      return clcx[1];
     };


