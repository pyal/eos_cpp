

#include "Grid_bound.h"

DescribedClass *create_GridBound_cd() {
    return new GridBound;
}
static ClassDesc GridBound_cd(
    typeid(GridBound),
    "GridBound",
    1,
    "Grid",
    &create_GridBound_cd);

DescribedClass *create_GridVarBoundInd_cd() {
    return new GridVarBoundInd;
}
static ClassDesc GridVarBoundInd_cd(
    typeid(GridVarBoundInd),
    "GridVarBoundInd",
    1,
    "Grid",
    &create_GridVarBoundInd_cd);
//static ClassDesc Grid_GridBoundPnt_cd(typeid(Grid<GridBoundPnt>),"Grid_GridBoundPnt",1,"Grid");//,&create_GridVarBoundInd_cd);
//GridBoundManipulator

DescribedClass *create_GridBoundManipulator_cd() {
    return new GridBoundManipulator;
}
static ClassDesc GridBoundManipulator_cd(
    typeid(GridBoundManipulator),
    "GridBoundManipulator",
    1,
    "Grid",
    &create_GridBoundManipulator_cd);

// =====================================================================
// =================           GridBound           =====================
// =====================================================================
GridBound::GridBound(GridVarBoundInd *center)
    : Grid<GridBoundPnt>(), centerind_ptr(center) {}   //SetRefMask(SingleFileStorage);};

GridVarBoundInd *GridBound::SetCenterGrid(GridVarBoundInd *center) {
    if(center == NULL)
        return centerind_ptr;
    if(centerind_ptr != NULL) {
        ExEnv::err()
            << " Cannot change GridVarBoundInd object. Something wrong in GridBound::SetCenterGrid"
            << endl;
        abort();
    }
    centerind_ptr = center;
    return centerind_ptr;
};
int GridBound::read_data_state(FilterIn &si) {
    Grid<GridBoundPnt>::read_data_state(si);
    SavableClass *tmp;
    si >> StdKey >> tmp;
    centerind_ptr = dynamic_cast<GridVarBoundInd *>(tmp);
    return 1;
};

// =====================================================================
// =================          GridVarBoundInd      =====================
// =====================================================================
/*
struct BoundDesc
 {
  SpacePoint Center;
  SpacePoint NormalDir;//(GridIndex &k);
  double Surface;//(GridIndex &k);
 }
struct GridVarBoundInd : public GridField<GridBoundIndPnt>
 {
   Ref<GridBound> gridbound_ptr;
*/

void GridVarBoundInd::DeleteOldBoundary(GridIndex k, int kpos) {
    if(k < 0)
        return;
    GridIndex old = data[k][kpos], k1;
    if(old < 0)
        return;
    int k1pos;
    GridBoundPnt bnd = gridbound_ptr->GetPnt(old);
    k = bnd.First;
    kpos = bnd.FirstPos;
    k1 = bnd.Second;
    k1pos = bnd.SecondPos;
    if(k >= 0)
        data[k][kpos] = -2;
    if(k1 >= 0)
        data[k1][k1pos] = -2;
    gridbound_ptr->DelPnt(old);
}
void GridVarBoundInd::ResizeIndPnt(GridIndex k, int NewDim) {
    int OldDim = data[k].SetDim();
    if(NewDim == OldDim)
        return;
    data[k].Resize(NewDim);
    if(NewDim > OldDim)
        for(int k1 = OldDim; k1 < NewDim; k1++)
            data[k][k1] = -2;
};

void GridVarBoundInd::AddNewBoundary(GridIndex k, int kpos) {
    if(k < 0)
        return;
    GridManipulator *Center_manip = GetGridManip();
    DataVector<GridIndex, CopyStructSlow<GridIndex>> neib_new =
        Center_manip->GetNeibous(k);
    int k1pos, Nnew = neib_new.SetDim();
    if((kpos >= Nnew) || (kpos < 0)) {
        ExEnv::err() << " Error AddNewBoundary: Nnew " << Nnew << " kpos " << kpos
                     << endl;
        abort();
    }

    GridIndex emp = Center_manip->NextEmptyIndex(-1), k1;
    GridBoundPnt bnd;
    Center_manip->GetPntBound(k, kpos, bnd);
    bnd.Center.SetClassStored(0);
    bnd.NormalDir.SetClassStored(0);
    //  k=bnd.First;kpos=bnd.FirstPos;
    k1 = bnd.Second;
    k1pos = bnd.SecondPos;
    ResizeIndPnt(k, Nnew);
    data[k][kpos] = emp;
    if(k1 >= 0) {
        ResizeIndPnt(k1, Center_manip->GetNeibous(k1).SetDim());
        data[k1][k1pos] = emp;
    }
    gridbound_ptr->AddPnt(emp, bnd);
}

DataVector<GridIndex> GridVarBoundInd::GetCurNeib(GridIndex k) {
    DataVector<GridIndex> ret = data[k];
    GridIndex emp;
    for(int kpos = 0; kpos < ret.SetDim(); kpos++) {
        emp = data[k][kpos];
        if(emp < 0) {
            //       ExEnv::err()<<" GetCurNeib strange, below zero "<<endl;
            ret[kpos] = -2;
            continue;
        }
        ret[kpos] = (gridbound_ptr->GetPnt(emp)).GetNeib(k);
    }
    return ret;
};

void GridVarBoundInd::ResetOldPointGeometry(GridIndex k) {
    if(k < 0)
        return;
    //  GridManipulator *Center_manip=SetStore();
    //  GridBoundPnt bnd=gridbound_ptr->GetPnt(old);
    //  GridIndex k=bnd.GetNeib(old);

    DataVector<GridIndex, CopyStructSlow<GridIndex>> neib_new =
        GetGridManip()->GetNeibous(k);
    DataVector<GridIndex, CopyStructSlow<GridIndex>> neib_old = GetCurNeib(k);
    int kpos, Nnew = neib_new.SetDim(), Nold = neib_old.SetDim();

    for(kpos = 0; kpos < Nold; kpos++) {
        if((kpos < Nnew) && (neib_old[kpos] == neib_new[kpos]))
            continue;
        // Different neibous, delete old neibous
        DeleteOldBoundary(k, kpos);
    }
};

void GridVarBoundInd::ResetNewPointGeomtry(GridIndex k) {
    if(k == -1)
        return;
    //  GridManipulator *Center_manip=SetStore();
    DataVector<GridIndex, CopyStructSlow<GridIndex>> neib_new =
        GetGridManip()->GetNeibous(k);
    DataVector<GridIndex, CopyStructSlow<GridIndex>> neib_old = GetCurNeib(k);
    int kpos, Nnew = neib_new.SetDim(), Nold = neib_old.SetDim();
    for(kpos = 0; kpos < Nnew; kpos++) {
        if((kpos < Nold) && (neib_old[kpos] == neib_new[kpos]))
            continue;
        // Different neibous, add new neibous
        AddNewBoundary(k, kpos);
    }
    //neib_old=GetCurNeib(k);for (kpos=0;kpos<Nnew;kpos++)
    //if (neib_old[kpos]!=neib_new[kpos]) fcout<<"They are not the same: "<<data[k]<<"\n";fcout.flush();
};
void GridVarBoundInd::ResetPointNeibGeometry(GridIndex k, int PntExist) {
    GridManipulator *Center_manip = GetGridManip();
    //  GridIndex gridnewpnt=Bound_manip->StartEmptyIndex(),gridoldpnt,curNeib,emp=gridbound_ptr->StartEmptyIndex();
    DataVector<GridIndex, CopyStructSlow<GridIndex>> neib_new;
    DataVector<GridIndex, CopyStructSlow<GridIndex>> neib_old = GetCurNeib(k);
    int kpos, Nnew = 0, Nold = neib_old.SetDim();
    if(PntExist) {
        neib_new = Center_manip->GetNeibous(k);
        Nnew = neib_new.SetDim();
    }
    // First - deleting old not matching boundaries
    for(kpos = 0; kpos < Nold; kpos++) {
        if(neib_old[kpos] == -1) {
            if((kpos >= Nnew) || (neib_new[kpos] != -1))
                DeleteOldBoundary(k, kpos);
        } else
            ResetOldPointGeometry(neib_old[kpos]);
    }
    // Second - adding new not matching boundaries

    for(kpos = 0; kpos < Nnew; kpos++) {
        if(neib_new[kpos] == -1) {
            if((kpos >= Nold) || (neib_old[kpos] != -1))
                AddNewBoundary(k, kpos);
        } else
            ResetNewPointGeomtry(neib_new[kpos]);
    }
};

void GridVarBoundInd::ResetPointBounds(GridIndex k) {
    GridManipulator *Center_manip = GetGridManip();
    int kpos, N = data[k].SetDim();
    // First - deleting old not matching boundaries

    for(kpos = 0; kpos < N; kpos++) {
        GridBoundPnt bnd;
        Center_manip->GetPntBound(k, kpos, bnd);
        gridbound_ptr->ModifyPnt(data[k][kpos], bnd);
    }
};

void GridVarBoundInd::AddPnt(GridIndex k, int ResetNeib) {
    ResetPointNeibGeometry(k, 1);
};


void GridVarBoundInd::DelPnt(GridIndex k) {
    ResetPointNeibGeometry(k, 0);
    data[k].SetDim(0);
};


void GridVarBoundInd::ModifyPnt(GridIndex k) {
    ResetPointNeibGeometry(k, 1);
    ResetPointBounds(k);
};

void GridVarBoundInd::MovePnt(GridIndex From, GridIndex To) {
    int N = data[From].SetDim();
    for(int k = 0; k < N; k++) {
        gridbound_ptr->GetPnt(data[From][k]).ChangeNeib(From, To);
    }
    data[To] = data[From];
};


void GridVarBoundInd::CreateGridData() {
    GridManipulator *Center_manip = GetGridManip();
    if(!Center_manip)
        return;
    GridIndex k = Center_manip->StartPnt();
    for(k; k >= 0; k = Center_manip->NextPnt(k))
        AddPnt(k, 0);
    //   { fcout<<(*Center_manip)<<endl;AddPnt(k,0);fcout<<(*Center_manip)<<endl;}
};


//void GridVarBoundInd::ResetNeibous(int TopologyChanged)
// Not to react on any command, just to reset afterwards
//int  GridVarBoundInd::SetFrozen(int froze)


// =====================================================================
// =================     GridBoundManipulator      =====================
// =====================================================================


DataVector<GridIndex, CopyStructSlow<GridIndex>> GridBoundManipulator::GetNeibous(
    GridIndex k)   //{return store->GetPnt(k).Neibous;}
{
    GridBoundPnt pnt = grid->GetPnt(k);
    GridBoundIndPnt fst, lst;
    DataVector<GridIndex, CopyStructSlow<GridIndex>> ret(2);
    int Nfst, i;   //,Nlst
    if(pnt.First < 0)
        ret[0] = -1;
    else {
        fst = grid->centerind_ptr->GetPnt(pnt.First);
        Nfst = fst.SetDim();
        for(i = 0; i < Nfst; i++)
            if(fst[i] == k)
                if(i / 2. == i) {
                    if(i == Nfst - 1) {
                        fcout << " Very bad in GridBoundManipulator::GetNeibous\n";
                        abort();
                    } else
                        ret[0] = fst[i + 1];
                    break;
                } else {
                    if(i == 0) {
                        fcout << " Very bad in GridBoundManipulator::GetNeibous\n";
                        abort();
                    } else
                        ret[0] = fst[i - 1];
                    break;
                }
    }
    if(pnt.Second < 0)
        ret[1] = -1;
    else {
        fst = grid->centerind_ptr->GetPnt(pnt.Second);
        Nfst = fst.SetDim();
        for(i = 0; i < Nfst; i++)
            if(fst[i] == k)
                if(i / 2. == i) {
                    if(i == Nfst - 1) {
                        fcout << " Very bad in GridBoundManipulator::GetNeibous\n";
                        abort();
                    } else
                        ret[1] = fst[i + 1];
                    break;
                } else {
                    if(i == 0) {
                        fcout << " Very bad in GridBoundManipulator::GetNeibous\n";
                        abort();
                    } else
                        ret[1] = fst[i - 1];
                    break;
                }
    }

    return ret;
};

DataVector<SpacePoint, CopyStructSlow<SpacePoint>> GridBoundManipulator::GetGridMaskedPnt(
    DataVector<GridIndex> &mask) {
    DataVector<GridBoundPnt> pnt;
    pnt = grid->GetMaskedPnt(mask);
    int k, N = pnt.SetDim();
    DataVector<SpacePoint, CopyStructSlow<SpacePoint>> ret(N);
    for(k = 0; k < N; k++)
        ret[k] = pnt[k].Center;
    return ret;
};

void GridBoundManipulator::SetGridMaskedPnt(
    DataVector<SpacePoint, CopyStructSlow<SpacePoint>> &data,
    DataVector<GridIndex> &mask) {
    DataVector<GridBoundPnt, CopyStructSlow<GridBoundPnt>> pnt;
    pnt = grid->GetMaskedPnt(mask);
    int k, N = pnt.SetDim();
    DataVector<SpacePoint, CopyStructSlow<SpacePoint>> ret(N);
    for(k = 0; k < N; k++)
        pnt[k].Center = data[k];
    grid->SetMaskedPnt(pnt, mask);
};


// ====================================================================
// ====================================================================
// ====================================================================
// ====================================================================
// ====================================================================
// ====================================================================


/*

DataVector<GridIndex, CopyStructSlow<GridIndex> > GridBoundManipulator::GetNeibous(GridIndex k)//{return store->GetPnt(k).Neibous;}
 {
  GridBoundPnt pnt=store->GetPnt(k);
  GridBoundIndPnt fst,lst;
  if (pnt.First>=0) fst=store->centerind_ptr->GetPnt(pnt.First);
  if (pnt.Second>=0) lst=store->centerind_ptr->GetPnt(pnt.Second);
//  DataVector<GridIndex> = GridBoundIndPnt
  int Nfst=fst.SetDim(),Nlst=lst.SetDim(),i,i1,pos=0,tmp,N1fst;
  DataVector<GridIndex, CopyStructSlow<GridIndex> > ret(Nfst+Nlst-1);
  for (i=0;i<Nfst;i++) if ((tmp=fst[i])!=k) {ret[pos]=tmp;pos++;}
  N1fst=pos;
  for (i=0;i<Nlst;i++) 
   {
    if ((tmp=lst[i])!=k) 
     {
      int good=1;
      for (i1=0;i1<N1fst;i1++) if (ret[i1]==tmp) {good=0;break;}
      if (good) {ret[pos]=tmp;pos++;}
     }
   }
  ret.Resize(pos);
  return ret;
 };




void GridVarBoundInd::CreateGridData()
 {
  GridManipulator *Bound_manip=gridbound_ptr->SetGridManip(),*Center_manip=SetStore();
  GridIndex k,emp=Bound_manip->StartEmptyIndex(),o;
  SpacePoint cur_pnt,neib_pnt,delta_pnt;
  GridBoundPnt bound;
  DataVector<GridIndex> neib_ind,cur_ind;
  for (k=Center_manip->StartPnt();k>=0;k=Center_manip->NextPnt(k))
    data[k]=store->GetNeibous(k)*0-1;
// Step all center_points
  for (k=Center_manip->StartPnt();k>=0;k=Center_manip->NextPnt(k))
   {
    
    cur_pnt=Center_manip->GetGridPnt(k);
    cur_ind=Center_manip->GetNeibous(k);
// Step all neibous k1 for point k
    for (int k1=0;k1<cur_ind.SetDim();k1++)
     {
// If point is not setted yet
      if (data[k][k1]==-1)
        {
         o=cur_ind[k1];
// ==================== Attention to be changed ===========================
         if (o<0) ;//Center_manip->GetBoundPnt(bound,k,k1);
         else
           {
            neib_pnt=Center_manip->GetGridPnt(o);
            bound.Center=(neib_pnt+cur_pnt)*0.5;
            delta_pnt=cur_pnt-neib_pnt;double norm=delta_pnt*delta_pnt;
            if (norm>MathZer) bound.NormalDir=delta_pnt/sqrt(norm);
            else bound.NormalDir=delta_pnt*0;
            bound.Surface=0;
            bound.First=k;bound.Second=o;
           }
         data[k][k1]=emp;gridbound_ptr->AddPnt(emp,bound);emp=gridbound_ptr->NextEmptyIndex(emp);
         if (o>=0) 
          {
           neib_ind=Center_manip->GetNeibous(o);
           for (int k2=0;k2<neib_ind.SetDim();k2++)
            if (neib_ind[k2]==k) 
             {
//              bound.NormalDir=bound.NormalDir*(-1);bound.First=o;bound.Second=k;
//              data[o][k2]=emp;AddPnt(emp,bound);emp=NextEmptyIndex(emp);
              data[o][k2]=emp;
             }
          }
        }
      }
   }
 };

void GridVarBoundInd::CreatePointNeib(DataVector<GridBoundPnt> &bound_vect,GridIndex k)
 {
//  FieldInterface 
  GridManipulator *Center_manip=SetStore();
//  SpacePoint k_pnt,k1_pnt,delta_pnt;
//  k_pnt=Center_manip->GetGridPnt(k);
//  DataVector<GridIndex> k1_ind,k_ind;
  DataVector<GridIndex> k_ind;
  k_ind=Center_manip->GetNeibous(k);
  int kpos,N=k_ind.SetDim();
  bound_vect.SetDim(N);

  for (kpos=0;kpos<N;kpos++)
     {
//      Center_manip->GetPntBound(k,kpos,bound_vect[kpos]);

         GridIndex k1=k_ind[kpos];
// ==================== Attention to be changed ===========================
         if (k1<0) ;//Center_manip->GetBoundPnt(bound_vect[kpos],k,kpos);
         else
           {
            k1_ind=Center_manip->GetNeibous(k1);
            k1_pnt=Center_manip->GetGridPnt(k1);
            bound_vect[kpos].Center=(k1_pnt+k_pnt)*0.5;
            delta_pnt=k_pnt-k1_pnt;double norm=delta_pnt*delta_pnt;
            if (norm>MathZer) bound_vect[kpos].NormalDir=delta_pnt/sqrt(norm);
            else bound_vect[kpos].NormalDir=delta_pnt*0;
            bound_vect[kpos].Surface=0;
            bound_vect[kpos].First=k;bound_vect[kpos].FirstPos=kpos;
            bound_vect[k1].Second=k1;
            for (int k1pos=0;k1pos<k1_ind.SetDim();k1pos++)
             if (k1_ind[k1pos]==k) {bound_vect[kpos].SecondPos=k1pos;goto Good;}
            ExEnv::err()<<" CouldNot Find Linked Reference to point in  GridVarBoundInd::CreatePointNeib."<<endl;abort();
Good:;
           }

     }
 };

void GridVarBoundInd::ResetPointNeib(DataVector<GridBoundPnt> &bound_vect,GridIndex k)
 {
  GridManipulator *Bound_manip=gridbound_ptr->SetGridManip(),*Center_manip=SetStore();
  GridIndex gridnewpnt=Bound_manip->StartEmptyIndex(),gridoldpnt,curNeib,emp=gridbound_ptr->StartEmptyIndex();
  int N=bound_vect.SetDim(),kpos;
  for (kpos=0;kpos<N;kpos++)
   {
    GridBoundPnt bnd=bound_vect[kpos];
//    int k=bnd.FirstInd,
    int k1=bnd.Second,k1pos=bnd.SecondPos;
// if a bound pnt
    if (k1<0)
     {data[k][kpos]=emp;gridbound_ptr->AddPnt(emp,bound_vect[kpos]);emp=Bound_manip->NextEmptyIndex(emp);continue;}

    gridoldpnt=data[k1][k1pos];
    GridBoundPnt gridbound=gridbound_ptr->GetPnt(gridoldpnt);
    curNeib=gridbound.GetNeib(k1);
    if (curNeib==-2) { ExEnv::err()<<" In ResetPointNeib - very bad neibour. Abort"<<endl;abort();}
// Right neibour, reseting boundary value
    if (curNeib==k) gridbound_ptr->GetPnt(gridoldpnt)=bound_vect[kpos];
    else
     {
// There was no neibour
      if (curNeib==-1) gridbound_ptr->DelPnt(gridoldpnt);
// there was different neibour
      else gridbound.ChangeNeib(k1,-1);
      data[k][kpos]=emp;
      data[k1][k1pos]=emp;
      gridbound_ptr->AddPnt(emp,bound_vect[kpos]);
      emp=Bound_manip->NextEmptyIndex(emp);
     };
   }
 };

void GridVarBoundInd::AddPnt(GridIndex k,int ResetNeib)
 {
  DataVector<GridBoundPnt> bound_vect;
  CreatePointNeib(bound_vect,k);
  data[k].SetDim(bound_vect.SetDim());data[k]=-1;
//  if (ResetNeib) 
   ResetPointNeib(bound_vect,k);
 };
    

void GridVarBoundInd::DelPnt(GridIndex delpnt)
 {
  GridBoundIndPnt grid=data[delpnt];
  int N=grid.SetDim();
  for (int k=0;k<N;k++)
   { 
    GridIndex gridoldpnt=grid[k];
    GridIndex NeibPnt=gridbound_ptr->GetPnt(gridoldpnt).GetNeib(delpnt);
    if (NeibPnt==-2) { ExEnv::err()<<" In GridVarBoundInd::DelPnt - very bad neibour. Abort"<<endl;abort();}
    if (NeibPnt==-1) gridbound_ptr->DelPnt(gridoldpnt);
    else 
     {
      DataVector<GridBoundPnt> bound_vect;
      CreatePointNeib(bound_vect,NeibPnt);ResetPointNeib(bound_vect,NeibPnt);
     }
   };
 };


void GridVarBoundInd::ModifyPnt(GridIndex k)
 {
//  DelPnt(k);AddPnt(k);return;
    DataVector<GridBoundPnt> bound_vect;
    CreatePointNeib(bound_vect,k);
    int N=SetStore()->GetNeibous(k).SetDim(),N1=data[k].SetDim();
    if (N1!=N)
     {
      GridBoundIndPnt newind(N),oldind=data[k];newind=-1;
      int i;
      for (i=0;i<min(N,N1);i++) newind[k]=oldind[k];
      GridBoundPnt gridbound=gridbound_ptr->GetPnt(data[k][i]);
      for (i=N;i<N1;i++) 
       {
        gridbound.ChangeNeib(k,-1);
        if (gridbound.NoNeibous()) gridbound_ptr->DelPnt(data[k][i]);}
     }
    ResetPointNeib(bound_vect,k);
 };

void GridVarBoundInd::MovePnt(GridIndex From,GridIndex To)
 {
  int N=data[From].SetDim();
  for (int k=0;k<N;k++)
   {
    GridBoundPnt gridbound=gridbound_ptr->GetPnt(data[From][k]);
    gridbound.ChangeNeib(From,To);
   }
  data[To]=data[From];
 };
 void GridVarBoundInd::CreatePointNeib(DataVector<GridBoundPnt> &bound_vect,GridIndex k)
 {
  GridManipulator *Center_manip=SetStore();
  int kpos,N=Center_manip->GetNeibous(k).SetDim();
  bound_vect.SetDim(N);

  for (kpos=0;kpos<N;kpos++)  Center_manip->GetPntBound(k,kpos,bound_vect[kpos]);
 };


*/