
#include "lib/data/DataVec.h"
//#include "IndBase.h"
#include "lib/data/DataArr.h"

#include "lib/data/DataTyp.h"
#include "lib/data/VecRes.h"

#define GridIndex int


struct GridCenterPnt : public SavableClass  
{
  public:

   GridCenterPnt():IndNum(0),Coord(0),Neibous(0),Volume(0){};
   GridCenterPnt(const SpacePoint &Spnt,int Npnt=0):IndNum(Npnt),Coord(Spnt),Volume(0),
			                                                             Neibous(0){};
   GridCenterPnt(const GridCenterPnt &pnt)
    {IndNum=pnt.IndNum;Coord=pnt.Coord;Volume=pnt.Volume;Neibous=pnt.Neibous;}
   GridCenterPnt &operator=(const GridCenterPnt &pnt)
    {IndNum=pnt.IndNum;Coord=pnt.Coord;Volume=pnt.Volume;Neibous=pnt.Neibous;
     return *this;}
   virtual int save_data_state (FilterOut &so)
    {
	    so<<KeyDesc("IndNum")<<IndNum<<KeyDesc("Coord")<<Coord
       <<KeyDesc("Volume")<<Volume<<KeyDesc("Neibous")<<Neibous<<KeyDesc("EndPnt\n");
     return 1;
    }
   virtual int read_data_state (FilterIn &si)
    {
	    si>>StdKey>>IndNum>>StdKey>>Coord>>StdKey>>Volume>>StdKey>>Neibous>>StdKey;
     return 1;
    }

//  protected://  private:

    GridIndex IndNum;
    SpacePoint Coord;
    double Volume;
    DataVector<GridIndex> Neibous;

    GridCenterPnt& Set(const SpacePoint &Spnt,int ind,double vol,
                        DataVector<GridIndex> &Neib)
     {Coord=Spnt;Volume=vol;IndNum=ind;Neibous=Neib;return *this;}
};


struct Grid : public SavableClass
 {
   virtual int save_data_state (FilterOut &so){so<<KeyDesc("data")<<data;return 1;};
   virtual int read_data_state (FilterIn &si){si>>StdKey>>data;return 1;};
   ResizeVector<GridCenterPnt, CopyStructSlow<GridCenterPnt> > data;
//   ResizeVector<GridCenterPnt > data;
 };

static ClassDesc GridCenterPnt_cd(typeid(GridCenterPnt),"GridCenterPnt");
static ClassDesc Grid_cd(typeid(Grid),"Grid");

void CreateGrid(Grid &stor,SpaceBound &bound,IndBound &index)
 {
   GridCenterPnt Cent;
   int k,k1,N=index.NumPnt(),N0=index.Low.SetDim();
   IndexPoint dim=index.Size(),ind,tmp_ind;
   SpacePoint add=bound.Low,mul=bound.Size(),pnt(N0);
   for (k=0;k<N0;k++) mul[k]/=dim[k];
   DataVector<GridIndex> neib(2*N0);
   double vol=mul[0];
   for (k=1;k<N0;k++) vol*=mul[k];
   for (k=0;k<N;k++) 
    {
     ind=index.Int2Pnt(k);
//fcout<<"\nind"<<ind;
     for (k1=0;k1<N0;k1++) pnt[k1]=add[k1]+(ind[k1]-index.Low[k1])*mul[k1];
     for (k1=0;k1<N0;k1++) 
      {tmp_ind=ind;tmp_ind[k1]=ind[k1]-1;neib[2*k1]=index.Pnt2Int(tmp_ind);
       tmp_ind[k1]=ind[k1]+1;neib[2*k1+1]=index.Pnt2Int(tmp_ind);}

//fcout<<"\nPnt"<<pnt<<"k"<<k<<"neib"<<neib<<"\n";
     stor.data[k]=Cent.Set(pnt,k,vol,neib);
//fcout<<stor.data[k]<<"\n";
    }
 };

   
//typedef   
void TestGrid()
 {
  Grid stor;
  SpacePoint low(3),up(3);  low=1.;up=2.;
  IndexPoint lowi(3),upi(3);lowi=(int)1;upi=(int)11;
  SpaceBound bound(low,up);
  IndBound index(lowi,upi);
//fcout<<"Bound\n"<<bound<<"\nindex\n"<<index;
  CreateGrid(stor,bound,index);
  Grid stor1;stor1.SavableClass::operator=(stor);
  fcout<<stor1;
  
 }

 int test_sav(int , const char *[])
{
//fcout<<" Test";fcout.flush();
//  TestVector();
//  IndRegBase reg;
//  TestArray();
  TestGrid();

};

