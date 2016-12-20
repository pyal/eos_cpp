#ifndef Eno_Main_h
#define Eno_Main_h 1

#include "lib/data/DataVec.h"
#include "lib/data/DataTyp.h"
#include "lib/data/DataArr.h"

#include "Gasd/Topology/grid_operator.h"



struct DivOper:OperatorOneD
 {
  enum { ClcSpline,ClcDeriv,ClcFlux } OperationType;

  GridVectorField<double> Dif;
  GridField<int> StartLev;
  GridField<double> Pos;
  int FixedStp;
  int Oper;
  int MinGlobal;


  DivOper():Pos(NULL,NULL,1),StartLev(NULL,NULL,1),Dif(0,NULL,NULL,1),MinGlobal(0){};
  virtual void Set(OperatorMask *shift,int ax,int lev,int min_glob=0) 
   {OperatorOneD::Set(shift,ax,lev);Dif.SetDim(Level+1);Oper=ClcSpline;MinGlobal=min_glob;
    if (Level>=Shift->MaxShift) {fcout<<" DivOper, MaxShift>Level - condition violated.\n";fcout.flush();abort();}
    shift->Grid->AddVar(&Dif);shift->Grid->AddVar(&StartLev);shift->Grid->AddVar(&Pos); };

// u_func==NULL - initPos else InitDeriv (pos already stored)
  virtual void Init(GridVectorField<double> *pos,GridField<double> *u_func=NULL)
   {
//     {fcout<<" DivOper::Init u_func - zero. BAD.\n";fcout.flush();abort();}
//    GridField<double> *cel=NULL;
//    if (pos!=NULL) cel=pos->GetX(Axis);
    if (u_func==NULL) InitPos(pos);
    else ClcDerivative(u_func);
   };
  virtual void SetAction(OperatorOneD &oper){Oper=((DivOper*)(&oper))->Oper;};
  inline int SetOperation(int oper=-1){if (oper!=-1) Oper=oper;return Oper;}

  virtual void Operate(GridMask &ClcMask,GridField<double> &dat,GridField<double> &res)
   {
    switch(Oper)
     {
      case ClcSpline:  ClcPolynom(ClcMask,dat,res);break;
      default:fcout<<" Unknown operation in DivOper::Operate.\n";fcout.flush();abort();
     }
   };





  void InitPos(GridVectorField<double> *pos)
   {
    if (pos==NULL) FixedStp=1;
    else
     {
      GridField<double> *cel=pos->GetX(Axis);
      int Zero=Shift->MaxShift;
      GridMask &m=Shift->Shift[Axis][Zero],&m_min=Shift->Shift[Axis][Zero-1];
      Pos(m)=( (*cel)(m)+(*cel)(m_min) )*0.5;
      FixedStp=0;
     }
   }
/*
  void FindMin()
   {
    int N=Shift->WorkMask.SetDim(), Zero=Shift->MaxShift,k;
    GridMaskVector &mv=Shift->Shift[Axis];
    for (k=0;k<N;k++) 
     {
      int lev=-1;
      GridIndex i;
      double min,tmp;
      if ((i=mv[Zero-Level][k])>=0) 
       {
        i=mv[Zero][k];
        lev=0;min=fabs(Dif.GetX(Level)->GetPnt(i));
        for (int pos=1;pos<Level;pos++) 
         if ( (tmp=fabs(Dif.GetX(Level)->GetPnt(mv[Zero-pos][k])))<min ) {min=tmp;lev=pos;}
       }
      StartLev.GetPnt(mv[Zero][k])=lev;
//      StartLev.GetPnt(mv[Zero][k])=0;
//fcout<<" Lev "<<lev<<" 0 "<<Dif.GetX(Level)->GetPnt(mv[Zero][k])<<Dif.GetX(Level)->GetPnt(mv[Zero-1][k])<<"\n";
     }
   };
*/
  void FindMin()
   {
    int N=Shift->WorkMask.SetDim(), Zero=Shift->MaxShift,k,pos;
    GridMaskVector &mv=Shift->Shift[Axis];
    for (k=0;k<N;k++) 
     {
//fcout<<" k "<<k<<" one_est ";
      int lev=-1;
      GridIndex i;
      double min,tmp;
      if ((i=mv[Zero-Level][k])>=0) 
       {
        if (MinGlobal)
         {
          i=mv[Zero][k];
          lev=0;min=fabs(Dif.GetX(Level-1)->GetPnt(i));
          for (pos=1;pos<Level;pos++) 
           if ( (tmp=fabs(Dif.GetX(Level-1)->GetPnt(mv[Zero-pos][k])))<min ) {min=tmp;lev=pos;}
         }
        else
         {
//fcout<<lev;
//for (pos=0;pos<Level;pos++) fcout<<fabs(Dif.GetX(Level)->GetPnt(mv[Zero-pos][k]));
          lev=0;
          for (pos=1;pos<Level;pos++) 
           if ( fabs(Dif.GetX(pos)->GetPnt(mv[Zero-lev-1][k]))<
                fabs(Dif.GetX(pos)->GetPnt(mv[Zero-lev][k]))) lev++;
//fcout<<"\n k "<<k<<" second_est "<<lev;
//for (pos=0;pos<Level;pos++) fcout<<fabs(Dif.GetX(pos)->GetPnt(mv[Zero-lev][k]));
         }

       }
//fcout<<"\n";
      StartLev.GetPnt(mv[Zero][k])=lev;
//      StartLev.GetPnt(mv[Zero][k])=0;
//fcout<<" Lev "<<lev<<" 0 "<<Dif.GetX(Level)->GetPnt(mv[Zero][k])<<Dif.GetX(Level)->GetPnt(mv[Zero-1][k])<<"\n";
     }
   };

  void ClcDerivative(GridField<double> *get)//,GridField<double> *cell_center)
   { 
    int Zero=Shift->MaxShift,or;
    GridMask &m=Shift->Shift[Axis][Zero],&m_pls=Shift->Shift[Axis][Zero+1];//,&m_min=Shift->Shift[Axis][Zero-1]
    GridMaskVector &mv=Shift->Shift[Axis];

    (*Dif.GetX(0))(m)=(*get)(m);
    if (!FixedStp)
        for (or=1;or<=Level;or++) 
          (*Dif.GetX(or))(m)=( (*Dif.GetX(or-1))(m_pls)-(*Dif.GetX(or-1))(m) )
                                                /( Pos(mv[or+1+Zero])-Pos(m) );
      else
        for (or=1;or<=Level;or++) 
         (*Dif.GetX(or))(m)=((*Dif.GetX(or-1))(m_pls)-(*Dif.GetX(or-1))(m))/(or+1);

//fcout<<" Start=========================================\n";
//for (or=0;or<=Level;or++)
//fcout<<Dif.GetX(or)->data<<"\n";
//fcout<<" End=========================================\n";

    FindMin();
   }
  void ClcPolynom(GridMask &ClcMask,GridField<double> &dat,GridField<double> &res)
   {
    int Zero=Shift->MaxShift,k,L=Level,N=ClcMask.SetDim(),k0;
    DataVector<int> StdK=Shift->CvtFrom(ClcMask);
    GridMaskVector &mv=Shift->Shift[Axis];
    for (k=0;k<N;k++)
     {
      k0=StdK[k];
      if ( (mv[Zero-L][k0]<0) || (mv[Zero+L][k0]<0) ) continue;
      double sum=0;
      int r=StartLev.GetPnt(mv[Zero][k0]);
      GridIndex i=ClcMask[k];
      for (int j=1;j<=L+1;j++) 
       {
        double s=0;
        for (int mi=0;mi<j;mi++) 
         {
          double mul=1;
          for (int l=0;l<j;l++) 
           {
            if (l==mi) continue;
            double x=dat.GetPnt(i);
            if (FixedStp) mul*=(x-(-r+l-0.5));//x-(i0-r+l-0.5)
            else mul*=(x-Pos.GetPnt(mv[Zero-r+l][k0]));
           }
          s+=mul;
         }

        s*=Dif.GetX(j-1)->GetPnt(mv[Zero-r][k0]);
        sum+=s;
       }
      res.GetPnt(i)=sum;
     }
   };
 };

struct FluxOper: OperatorOneD
 {
  Ref<DivOper> Div;
  DataArray<double> BoundRCoef,BoundLCoef;
  int AutoInitDiv;
  FluxOper():Div(NULL){};

  virtual void Set(OperatorMask *shift,int ax,int lev,DivOper *div,int autoInitDiv=1) 
   {
    OperatorOneD::Set(shift,ax,lev);Div=div;autoInitDiv=AutoInitDiv;
    if (AutoInitDiv) Div->Set(shift,ax,lev);
//    Level++;
    BoundRCoef.SetDim(Level,Level);BoundLCoef.SetDim(Level,Level);
//    BoundRCoef.SetDim(Level+1,Level+1);BoundLCoef.SetDim(Level+1,Level+1);
   };


  virtual void Init(GridVectorField<double> *pos,GridField<double> *u_func=NULL)
   {
    if (AutoInitDiv) Div->Init(pos,u_func);
    if (u_func==NULL) ConstructCoef(pos);
   };
//  virtual void SetAction(OperatorOneD &oper){Oper=((DivOper*)(&oper))->Oper;};
//  inline int SetOperation(int oper=-1){if (oper!=-1) Oper=oper;return Oper;}

  virtual void Operate(GridMask &ClcMask,GridField<double> &dat,GridVectorField<double> &res)
   {
    ClcFlux(ClcMask,res);
   };



  double ClcCoefJRX(int j,int r,double i_sh,int k)
   {
    int m,l,q;
    double sm,sl,mq,pl;
    sm=0;
    for (m=j+1;m<=k;m++) 
     {
      sl=0;
      for (l=0;l<=k;l++)
       {
        mq=1;
        if (l==m) continue;
        for (q=0;q<=k;q++) if ((q==m) || (q==l)) continue;else mq*=(i_sh+r-q+0.5);
        sl+=mq;
       }
      pl=1;
      for (l=0;l<=k;l++)  if (l==m) continue;else pl*=(m-l);
      sm+=(sl/pl);
     }
    return sm;
   }
    
  void ConstructCoef(GridVectorField<double> *pos)
   {
    int k=Level,j,r;
//    if (pos==NULL) //FixedStp=1; {
    for (j=0;j<k;j++)  
     {
      for (r=0;r<k;r++) // for (r=-1;r<k;r++) 
       {
        BoundRCoef(r,j)=ClcCoefJRX(j,r,0.5,k);
        BoundLCoef(r,j)=ClcCoefJRX(j,r,-0.5,k);
       }
     }
   }
  void ClcFlux(GridMask &ClcMask,GridVectorField<double> &res)
   {
    int Zero=Shift->MaxShift,k,N=ClcMask.SetDim(),k0;
    DataVector<int> StdK=Shift->CvtFrom(ClcMask);
    GridMaskVector &mv=Shift->Shift[Axis];
    for (k=0;k<N;k++)
     {
      k0=StdK[k];
      if ( (mv[Zero-Level][k0]<0) || (mv[Zero+Level][k0]<0) ) continue;
      double Fr=0,Fl=0;
      int r=Div->StartLev.GetPnt(mv[Zero][k0]);
      GridIndex i=ClcMask[k];
//      if (FixedStp)
       {
        for (int j=0;j<Level;j++) 
         {
          Fr+=BoundRCoef(r,j)*Div->Dif.GetX(0)->GetPnt(mv[Zero+j-r][k0]);
          Fl+=BoundLCoef(r,j)*Div->Dif.GetX(0)->GetPnt(mv[Zero+j-r][k0]);
         }
       }
//      else
      res.GetX(0)->GetPnt(i)=Fl;
      res.GetX(1)->GetPnt(i)=Fr;
     }
//fcout<<res.GetX(0)->data;fcout.flush();abort();
   };

 };

//struct :OperatorOneD

struct ReconstrFlux: OperatorOneD
 {
  enum { Godunov, LaxFriedrich } FluxType;
  int flux,MonotoneIncreaseFlux;
  double Alpha;
  ReconstrFlux():flux(LaxFriedrich),Alpha(10),MonotoneIncreaseFlux(1){};

  virtual void Set(OperatorMask *shift,int ax,int lev,int MonotInc=1,int flux_type=LaxFriedrich,double alpha=10) 
   { OperatorOneD::Set(shift,ax,lev);flux=flux_type;Alpha=alpha;MonotoneIncreaseFlux=MonotInc; };


//  virtual void SetAction(OperatorOneD &oper){Oper=((DivOper*)(&oper))->Oper;};
  inline int SetOperation(int oper=-1){if (oper!=-1) flux=oper;return flux;}

  virtual void Operate(GridMask &ClcMask,GridVectorField<double> &dat,GridField<double> &res)
   {
    int Zero=Shift->MaxShift,k,N=ClcMask.SetDim(),k0;
    DataVector<int> StdK=Shift->CvtFrom(ClcMask);
    GridMask m=Shift->Shift[Axis][Zero],m_pls=Shift->Shift[Axis][Zero+1],
                                              m_min=Shift->Shift[Axis][Zero-1];
    for (k=0;k<N;k++)
     {
      k0=StdK[k];
      GridIndex i=ClcMask[k];
      if ( (m_min[k0]<0) || (m[k0]<0) )  continue;
      double fa=dat.GetX(3)->GetPnt(m_min[k0]),fb=dat.GetX(2)->GetPnt(m[k0]);
      if (flux==Godunov)
       {
        if (MonotoneIncreaseFlux) res.GetPnt(i)=fa;
        else res.GetPnt(i)=fb;
       }
      else
       {
        res.GetPnt(i)=0.5*( fa+fb-Alpha*
         ( dat.GetX(0)->GetPnt(m[k0])-dat.GetX(1)->GetPnt(m_min[k0]) ) );
       }
     }
//fcout<<res.data<<"\n";fcout.flush();//abort();
   };
 };

#endif


/*
struct DivDif 
 {
  int Level,Dim;
  GridManipulator *Grid;
  MaskManipulatorStruct *Shift;
  GridVectorField<double>  *Pos;
  int FixedStp;

// SpaceDim Order - div_dif, Level+1 = MaxOrder
  DataVector<GridVectorField<double> * > Dif;
  GridVectorField<int> *StartLev;

//  DivDif():StartLev(NULL){};
  ~DivDif(){if (Dif.SetDim()==0) return;for (int k=0;k<Dif.SetDim();k++) Dif[k]->SetStore(NULL);StartLev->SetStore(NULL);Pos->SetStore(NULL);}
  virtual void Start(GridManipulator *grid,int level)
   {
    Level=level;Grid=grid;Shift=grid->mask_manip->shift;
    Dim=Shift->Dim;

    int k,N=Level+1;
    StartLev= new GridVectorField<int>(Dim,Grid);
    Pos= new GridVectorField<double>(Dim,Grid);
    Dif.SetDim(N);
    for (k=0;k<N;k++) Dif[k]=new GridVectorField<double>(Dim,Grid);
   };
  void FindMin()
   {
    GridMask mask=Shift->MaskMain;
    DataVector<GridMaskVector> m(Level+1);
    int N=mask.SetDim(),k,d;
    for (k=0;k<=Level;k++) m[k]=mask+GridMaskShiftVector(-k,Shift);
    for (d=0;d<Dim;d++) 
     {
      for (k=0;k<N;k++) 
       {
        int lev=-1;
        GridIndex i;
        double min,tmp;
        if ((i=m[Level][d][k])>=0) 
         {
          i=m[0][d][k];
          lev=0;min=fabs(Dif[Level]->GetX(d)->GetPnt(i));
          for (int pos=1;pos<Level;pos++) 
           if ( (tmp=fabs(Dif[Level]->GetX(d)->GetPnt(m[pos][d][k])))<min) {min=tmp;lev=pos;}
         }
        StartLev->GetX(d)->GetPnt(m[0][d][k])=lev;
//fcout<<" Lev "<<lev<<" 0 "<<Dif[Level]->GetX(d)->GetPnt(m[0][0][k])<<Dif[Level]->GetX(d)->GetPnt(m[1][d][k])<<"\n";
       }
     }
   };
    


  virtual void CalculateDiv(GridField<double> *get,
          GridVectorField<double > *cell_center=NULL)
   { 
//    GridVectorField<double> *cur;
    GridMask mask=Shift->MaskMain;
    GridMaskVector maskv=Shift->MaskMain+GridMaskShiftVector(0,Shift),m1,m_;
    FixedStp=1;
    if (cell_center!=NULL) 
     {(*Pos)(maskv)=((*cell_center)(maskv)+(*cell_center)(Shift->MaskMain+GridMaskShiftVector(-1,Shift)) )*0.5;
//      *Pos=*cell_center;
//      cur=new GridVectorField<double>(Dim,Grid);*cur=*cell_center;
      FixedStp=0;
     }
    DataVector<GridMaskVector> mv(Level+2);
    for (int k=0;k<=Level+1;k++) mv[k]=mask+GridMaskShiftVector(k,Shift);
    (*Dif[0])(maskv)=(*get)(maskv);
    int or;
    m_=mask+GridMaskShiftVector(0,Shift);
    m1=mask+GridMaskShiftVector(1,Shift);
    if (!FixedStp)
        for (or=1;or<=Level;or++) 
          (*Dif[or])(m_)=( (*Dif[or-1])(m1)-(*Dif[or-1])(m_) )/( (*Pos)(mv[or+1])-(*Pos)(m_) );
      else
        for (or=1;or<=Level;or++) (*Dif[or])(m_)=((*Dif[or-1])(m1)-(*Dif[or-1])(m_))/(or+1);
    FindMin();
//    if (cell_center!=NULL) cur->SetStore(NULL);
   }

  MaskedData<double> ClcPolynom(MaskedData<double> work,int Ax)
   {
    MaskedData<double> ret(work.vec,work.mask);
    int k,d,L=Level;
    DataVector<GridMask> m(2*L+1);
    for (k=-L;k<=L;k++) m[L+k]=(*work.mask)+GridMaskShift(Ax,k,Shift);
    for (k=0;k<work.mask->SetDim();k++)
     {
      if ( (m[0][k]<0) || (m[2*L][k]<0) ) continue;
      GridIndex i0=m[L][k];
      d=Ax;
       {
        double sum=0;
        int r=StartLev->GetX(d)->GetPnt(i0);
        for (int j=1;j<=L+1;j++) 
         {
          double s=0;
          for (int mi=0;mi<j;mi++) 
           {
            double mul=1;
            for (int l=0;l<j;l++) 
             {
              if (l==mi) continue;
              double x=work.vec->GetPnt(i0);
              if (FixedStp) mul*=(x-(-r+l-0.5));//x-(i0-r+l-0.5)
              else mul*=(x-Pos->GetX(d)->GetPnt(m[L-r+l][k]));
             }
            s+=mul;
           }

          s*=Dif[j-1]->GetX(d)->GetPnt(m[L-r][k]);
          sum+=s;
         }
        ret.vec->GetPnt(i0)=sum;
       }
     }
    return ret;
   };

  
};




  
    
  void SetDif (GridField<double> *get)
   {
    GridMask mask=Shift->MaskMain;
    GridMaskVector m0=Shift->MaskMain+GridMaskShiftVector(0,Shift);
    GridMaskVector mp=Shift->MaskMain+GridMaskShiftVector(1,Shift);
    GridMaskVector mm=Shift->MaskMain+GridMaskShiftVector(-1,Shift);
    int N=mask.SetDim(),k,d,D=mp.SetDim();
    GridMaskVector m1,m1p;
//    m1[0]->SetDim(1);m1p=m1;
    m1=mm;m1p=mm;
    int n;
//fcout<<mp<<"\n\n";
    for (d=0;d<D;d++) 
     { n=0;for (k=0;k<N;k++) if (mm[d][k]<0) {m1[d][n]=mask[k];n++;}m1[d].Resize(n);}

    m1p=m1+GridMaskShiftVector(1,Shift);
    GridVectorField<double> dx(D,Grid,NULL,1);
    if (!FixedStp) {dx(m0)=(dx(mp)+dx(m0))*0.5;dx(m0)=dx(m0)-dx(mm);}
    (*Dif[0])(m0)=(*get)(m0);
    for (k=1;k<N;k++) 
     {
//fcout<<Dif[0]->GetX(0)->GetPnt(m1p[0][0])<<"  "<<Dif[0]->GetX(0)->GetPnt(m1[0][0])<<" ";
      if (FixedStp) (*Dif[0])(m1p)=(*Dif[0])(m1p)+(*Dif[0])(m1);
//      else (*Dif[0])(m1p)=(*Dif[0])(m1p)*dx(m1p)+(*Dif[0])(m1);
      m1=m1p;m1p=m1p+GridMaskShiftVector(1,Shift);
//fcout<<"m1 "<<m1<<" m1p "<<m1p<<"\n";
//fcout<<Dif[0]->GetX(0)->GetPnt(m1[0][0])<<"\n";
     }
fcout<<" ====================  Ups \n";

   }    

  MaskedVector<double> ClcPolynom(MaskedVector<double> work)//int k,SpacePoint &x,SpacePoint &res)
   {
    MaskedVector<double> ret(work.vec,work.mask);
    int k,d,L=Level-1;
    DataVector<GridMaskVector> m(2*L+1);
    for (k=L;k>-L;k--) m[L+k]=(*work.mask)[0]+GridMaskShiftVector(k,Shift);

    for (k=0;k<(*work.mask)[0].SetDim();k++)
     {
      for (d=0;d<Dim;d++) if ( (m[0][d][k]<0) || (m[2*L][d][k]<0) ) break;
      GridIndex i0=m[L][0][k];
      for (d=0;d<Dim;d++)
       {
        double sum=0;
        int r=StartLev->GetX(d)->GetPnt(i0);
        for (int j=1;j<=L;j++) 
         {
          double s=0;
          for (int mi=0;mi<j;mi++) 
           {
            double mul=1;
            for (int l=0;l<j;l++) 
             {
              if (l==mi) continue;
              double x=work.vec->GetX(d)->GetPnt(i0);
              if (FixedStp) mul*=(x-(i0-r+l-0.5));
              else mul*=(x-Pos->GetX(d)->GetPnt(m[L-r+l][d][i0]));
             }
            s+=mul;
           }
          s*=Dif[j-1]->GetX(d)->GetPnt(m[L-j+r][d][i0]);
          sum+=s;
         }
        ret.vec->GetX(d)->GetPnt(i0)=sum;
       }
     }
    return ret;
   };

  virtual void CalculateDiv(GridField<SpacePoint> *get)
   {
    if (pos==NULL) return;
    DataVector<GridField<double>* > coord;SpacePoint2Double(coord);

    FreeDouble(coord);
   };
  void SpacePoint2Double(GridVectorField<double> &coord)
   {
    if (pos==NULL) return;
    coord.SetDim(Dim);
    GridIndex i;
    GridMask mask=Shift->Get(0,0);
    for (int d=0;d<Dim;d++) 
     {coord[d]=new GridField<double>(Grid);
      for (int k=0;k<mask.SetDim();k++) 
       if ((i=mask[k])>=0) coord[d]->GetPnt(i)= pos->GetPnt(i)[d];
     }
   };
  void FreeDouble(DataVector<GridField<double>* > &coord)
   {
    if (pos==NULL) return;
    for (int d=0;d<Dim;d++) {coord[d]->SetStore(NULL);coord[d]=NULL;}
   };
  void MakeSpline(GridField<double> *put)
   {
    for (int k=0;k<Shift->Get(0,0).SetDim();k++)
     {
      
      SpacePoint tmp(Dim);tmp=(double)(Shift->Get(0,0)[k]);
      ClcPolynom(k,tmp,tmp);
      put->GetPnt(k)=tmp[0];
     }
   };
    
struct ShiftMaskSpace
 {
  DataVector<GridMaskVector> shift_mask;
  int Order;
  ShiftMaskSpace(){};
  inline int AxShift2Int(const int &shift) {return shift+Order;}
  void Set(int order,MaskManipulatorGeneral *manip,GridMask &mask)
   {
    int k,k1,i;
    Order=order;
    int Dim=(manip->grid->GetGridPnt(mask[0])).SetDim();
    shift_mask.SetDim(2*Order+1);
//    shift_mask[0]=mask;
      shift_mask[AxShift2Int(0)]=mask;
      for (k1=1;k1<=Order;k1++) 
       {
        for (k=0;k<Dim;k++)
         {
          i=AxShift2Int( k1);
          shift_mask[i][k]=manip->ShiftMask(shift_mask[i-1][k],2*k+1,1);
          i=AxShift2Int(-k1);
          shift_mask[i][k]=manip->ShiftMask(shift_mask[i+1][k],2*k  ,1);
       }
     }
   };
  inline GridMaskVector& Get(int order) {return shift_mask[AxShift2Int(order)];}
 };

struct ShiftMask
 {
  DataVector<GridMask> shift_mask;
  int Order,Dim;
  ShiftMask(){};
  inline int AxShift2Int(const int &axis,const int &shift) {return ((2*Order)+1)*axis+shift+Order;}
  void Set(int order,MaskManipulatorGeneral *manip,GridMask &mask)
   {
    int k,k1,i;
    Order=order;
    Dim=(manip->grid->GetGridPnt(mask[0])).SetDim();
    shift_mask.SetDim(AxShift2Int(Dim-1,Order)+1);
    shift_mask[0]=mask;
    for (k=0;k<Dim;k++)
     {
      shift_mask[AxShift2Int(k,0)]=mask;
      for (k1=1;k1<=Order;k1++) 
       {
        i=AxShift2Int(k, k1);
        shift_mask[i]=manip->ShiftMask(shift_mask[i-1],2*k+1,1);
        i=AxShift2Int(k, -k1);
        shift_mask[i]=manip->ShiftMask(shift_mask[i+1],2*k  ,1);
       }
     }
   };
  inline GridMask& Get(int axis,int order) {return shift_mask[AxShift2Int(axis,order)];}
 };

  
struct DivDif 
 {
  int Level,Dim;
  GridManipulator *Grid;
  GridField<SpacePoint,CopyStructSlow<SpacePoint> >  *pos;
// SpaceDim Order - mask, 0 - start mask, Level+1 = MaxOrder
  ShiftMask *Shift;

// SpaceDim Order - div_dif, Level+1 = MaxOrder
  DataVector<GridField<double> *> dif;
  DataVector<GridField<int> *> StartLev;

  inline int AxShift2Int(const int &axis,const int &shift)
   {if (shift==0) return 0;return Level*axis+shift;}
  virtual void Start(GridManipulator *grid,int level,ShiftMask *shift)
   {
    Level=level;Grid=grid;Shift=shift;
    Dim=shift->Dim;

    int k,N=AxShift2Int(Dim-1,Level)+1;
    StartLev.SetDim(Dim);
    for (k=0;k<Dim;k++) StartLev[k]=new GridField<int>(Grid);
    dif.SetDim(N);
    for (k=0;k<N;k++) dif[k]=new GridField<double>(Grid);
   };

  void SpacePoint2Double(DataVector<GridField<double>* > &coord)
   {
    if (pos==NULL) return;
    coord.SetDim(Dim);
    GridIndex i;
    GridMask mask=Shift->Get(0,0);
    for (int d=0;d<Dim;d++) 
     {coord[d]=new GridField<double>(Grid);
      for (int k=0;k<mask.SetDim();k++) 
       if ((i=mask[k])>=0) coord[d]->GetPnt(i)= pos->GetPnt(i)[d];
     }
   };
  void FreeDouble(DataVector<GridField<double>* > &coord)
   {
    if (pos==NULL) return;
    for (int d=0;d<Dim;d++) {coord[d]->SetStore(NULL);coord[d]=NULL;}
   };
  void FindMin()
   {
    int N=Shift->Get(0,0).SetDim();
    for (int d=0;d<Dim;d++) 
     {
      int dif_ind=AxShift2Int(d,Level);
      for (int k=0;k<N;k++) 
       {
        int lev=-1;
        GridIndex i;
        if ((i=Shift->Get(d,-Level)[k])>=0) 
         {
          lev=Level;
          double min=fabs(dif[dif_ind]->GetPnt(i)),tmp;
          for (int pos=Level-1;pos>=1;pos--) 
           if ( (tmp=fabs(dif[dif_ind]->GetPnt(Shift->Get(d,-pos)[k])))<min) {min=tmp;lev=pos;}
         }
        StartLev[d]->GetPnt(Shift->Get(0,0)[k])=lev;
//fcout<<" Lev "<<lev<<"\n";
       }
     }
   };
    
  void ClcPolynom(int k,SpacePoint &x,SpacePoint &res)
   {
    GridIndex i0=Shift->Get(0,0)[k];
    int d;
    for (d=0;d<Dim;d++) if ((Shift->Get(d,-Level)[k]<0) || (Shift->Get(d,Level)[k]<0) ) return;
    res.SetDim(Dim);
    for (d=0;d<Dim;d++)
     {
      double sum=0;
      int r=Level-StartLev[d]->GetPnt(i0);
      for (int j=1;j<=Level;j++) 
       {
        double s=0;
        for (int m=0;m<j;m++) 
         {
          double mul=1;
          for (int l=0;l<j;l++) 
           {
            if (l==m) continue;
            mul*=(x[d]-(i0-r+l-0.5));
           }
          s+=mul;
         }
        s*=dif[j]->GetPnt(Shift->Get(d,-j+r)[k]);
        sum+=s;
       }
      res[d]=sum;
     }
   };

  void MakeSpline(GridField<double> *put)
   {
    for (int k=0;k<Shift->Get(0,0).SetDim();k++)
     {
      
      SpacePoint tmp(Dim);tmp=(double)(Shift->Get(0,0)[k]);
      ClcPolynom(k,tmp,tmp);
      put->GetPnt(k)=tmp[0];
     }
   };
    
    

  virtual void CalculateDiv(GridField<double> *get,
          GridField<SpacePoint,CopyStructSlow<SpacePoint> > *cell_center=NULL)
   { 
    pos=cell_center;
    DataVector<GridField<double>* > coord;SpacePoint2Double(coord);
    int d,or;
    (*dif[0])(Shift->Get(0,0))=(*get)(Shift->Get(0,0));
    for (d=0;d<Dim;d++) 
     {
      if (pos!=NULL)
       {
        for (or=1;or<=Level;or++) 
         {
          (*dif[AxShift2Int(d,or)])(Shift->Get(d,-or))=
           ((*dif[AxShift2Int(d,or-1)])(Shift->Get(d,-or+1))-(*dif[AxShift2Int(d,or-1)])(Shift->Get(d,-or)))/
               ((*coord[d])(Shift->Get(d,-or+1))-(*coord[d])(Shift->Get(d,-or)));
          (*coord[d])(Shift->Get(d,-or))=
           ((*coord[d])(Shift->Get(d,-or+1))+(*coord[d])(Shift->Get(d,-or)))*0.5;

         }
       }
      else
       {
        for (or=1;or<=Level;or++) 
         (*dif[AxShift2Int(d,or)])(Shift->Get(d,-or))=
           (*dif[AxShift2Int(d,or-1)])(Shift->Get(d,-or+1))-
           (*dif[AxShift2Int(d,or-1)])(Shift->Get(d,-or));
       }
     }
    FindMin();
    FreeDouble(coord);
   }
  virtual void CalculateDiv(GridField<SpacePoint> *get)
   {
    if (pos==NULL) return;
    DataVector<GridField<double>* > coord;SpacePoint2Double(coord);

    FreeDouble(coord);
   };
    
};
void MakeRightDivDif(GridField<double> *get,
                     DataVector<GridField<double>* > &dif,
                     DataVector<GridMask> &ShiftL,int Level,
                     GridField<SpacePoint,CopyStructSlow<SpacePoint> > *pos_var)
 {
  int N=ShiftL[0].SetDim(),FixedCell=(pos_var==NULL);
  GridManipulator *grid=get->SetStore();
  GridField<double> *curg,*curp;
  *dif[0]=*get;
  Ref<GridField<SpacePoint,CopyStructSlow<SpacePoint> > > cur_pos,new_pos;
  DataVector<double> pos;
  if (!FixedCell) 
   {
    cur_pos=new GridField<SpacePoint,CopyStructSlow<SpacePoint> >(grid);
    new_pos=new GridField<SpacePoint,CopyStructSlow<SpacePoint> >(grid);
    (*cur_pos)=(*pos_var);
   }
  GridIndex i,i0;
  for (int lev=0;lev<Level;lev++) 
   { 
    curg=dif[lev];curp=dif[lev+1];
    for (int k1=0;k1<N;k1++) 
     {
      i=ShiftL[lev+1][k1],i0=ShiftL[lev][k1];
      if (i<0) continue;
      curp->GetPnt(i)=curg->GetPnt(i0)-curg->GetPnt(i);
      if (FixedCell) continue;
      SpacePoint fst=cur_pos->GetPnt(i0),sec=cur_pos->GetPnt(i);
      curp->GetPnt(i)=curp->GetPnt(i)/VectorMod(fst-sec);
      new_pos->GetPnt(i)=(fst+sec)*0.5;
     }
    if (!FixedCell) *cur_pos=*new_pos;
   }    
 };

*/