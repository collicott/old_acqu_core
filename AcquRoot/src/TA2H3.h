//--Author	JRM Annand	13th Apr 2005
//--Rev 	
//--Update
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data.
//
// TA2H3
// AcquRoot 3D histogrammer based on TH3F
// This is an abstract class as the fill method is pure virtual
// Derived classes TA2H3S and TA2H3M handle single or multi-value variables
// Variables to histogram are templated, but x,y,z are of the same type.
// An optional Double_t non-unity increment weight variable is provided 

#ifndef __TA2H3_h__
#define __TA2H3_h__

#include "TH3F.h"
#include "TA2H.h"

template< typename Tx,typename Ty,typename Tz > class TA2H3 : 
public TA2H, public TH3F {
 protected:
  Tx* fHistX;
  Ty* fHistY;
  Tz* fHistZ;
 public:
  TA2H3(){}
  // Const bin width x, y and z
  TA2H3( char* n, const char* t, Int_t chx, Axis_t lowx, Axis_t highx,
	 Int_t chy, Axis_t lowy, Axis_t highy,
	 Int_t chz, Axis_t lowz, Axis_t highz,
	 Tx* xdata, Ty* ydata, Tz* zdata,
	 TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H(cut,wgt),
    TH3F(n,t,chx,lowx,highx,chy,lowy,highy,chz,lowz,highz)
      { fHistX = xdata; fHistY = ydata; fHistZ = zdata; }
    
  // Variable x, y and z bin width
  TA2H3( char* n, const char* t, Int_t chx, Double_t* xch,
	 Int_t chy, Double_t* ych, Int_t chz, Double_t* zch,
	 Tx* xdata, Ty* ydata, Tz* zdata,
	 TA2Cut* cut = NULL, Stat_t* wgt = NULL  ):
    TA2H(cut,wgt),
    TH3F(n,t,chx,xch,chy,ych,chz,zch)
      { fHistX = xdata; fHistY = ydata; fHistZ = zdata; }

  virtual ~TA2H3(){}
  Tx* GetHistX(){ return fHistX; }      // return x data buffer ptr
  Ty* GetHistY(){ return fHistY; }      // return y data buffer ptr
  Ty* GetHistZ(){ return fHistZ; }      // return z data buffer ptr

  //  ClassDefT(TA2H3,1)   
};

//-----------------------------------------------------------------------------
// Single-X, single-Y value per event histogram
template< typename Tx, typename Ty, typename Tz > class TA2H3S: 
public TA2H3<Tx,Ty,Tz>{
 public:
  TA2H3S(){};
  // Fixed x and y bin width
  TA2H3S( char* n, const char* t, Int_t chx, Axis_t lowx, Axis_t highx,
	  Int_t chy, Axis_t lowy, Axis_t highy,
	  Int_t chz, Axis_t lowz, Axis_t highz,
	  Tx* xdata, Ty* ydata, Tz* zdata,
	  TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H3<Tx,Ty,Tz>(n,t,chx,lowx,highx,chy,lowy,highy,chz,lowz,highz,
		    xdata,ydata,zdata,cut,wgt){}

  // Variable x and y bin width
  TA2H3S( char* n, const char* t, Int_t chx, Double_t* xch,
	  Int_t chy, Double_t* ych, Int_t chz, Double_t* zch,
	  Tx* xdata, Ty* ydata, Tz* zdata,
	  TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H3<Tx,Ty,Tz>(n,t,chx,xch,chy,ych,chz,zch,xdata,ydata,zdata,cut,wgt){}

  virtual ~TA2H3S(){}
  using TA2H3<Tx,Ty,Tz>::fHistX;    // gcc 3.4
  using TA2H3<Tx,Ty,Tz>::fHistY;    // gcc 3.4
  using TA2H3<Tx,Ty,Tz>::fHistZ;    // gcc 3.4
  using TA2H3<Tx,Ty,Tz>::fWeight;   // gcc 3.4
  using TA2H3<Tx,Ty,Tz>::fCut;      // gcc 3.4
  void Fill()
    { 
      // Fill histogram with single values held in x,y,z buffers
      // If weight variable has been supplied at construct fill with weight,
      // otherwise weight = 1.0
      // No action if "null" marker found in any buffer
      if( fCut ){ if( !fCut->Test() ) return; }
      if( (*fHistX == (Tx)ENullHit) || (*fHistY == (Ty)ENullHit) ||
	  (*fHistZ == (Tz)ENullHit) ) return;
      TH3F::Fill( (Float_t)(*fHistX), (Float_t)(*fHistY),
		  (Float_t)(*fHistZ), *fWeight );
      return;
    }
};

//-----------------------------------------------------------------------------
// Multi-X,Y,Z  value per event histogram
template< typename Tx,typename Ty,typename Tz > class TA2H3M: 
public TA2H3<Tx,Ty,Tz>{
 public:
  // Fixed x and y bin width
  TA2H3M( char* n, const char* t, Int_t chx, Axis_t lowx, Axis_t highx,
	  Int_t chy, Axis_t lowy, Axis_t highy, 
	  Int_t chz, Axis_t lowz, Axis_t highz,
	  Tx* xdata, Ty* ydata, Tz* zdata, 
	  TA2Cut* cut, Stat_t* wgt = NULL ):
    TA2H3<Tx,Ty,Tz>(n,t,chx,lowx,highx,chy,lowy,highy,chz,lowz,highz,
		    xdata,ydata,zdata,cut,wgt){}
  // Variable x and y bin width
  TA2H3M( char* n, const char* t, Int_t chx, Double_t* xch,
	  Int_t chy, Double_t* ych, Int_t chz, Double_t* zch,
	  Tx* xdata, Ty* ydata, Tz* zdata,
	  TA2Cut* cut, Stat_t* wgt ):
    TA2H3<Tx,Ty,Tz>(n,t,chx,xch,chy,ych,chz,zch,xdata,ydata,zdata,cut,wgt){}

  virtual ~TA2H3M(){}
  using TA2H3<Tx,Ty,Tz>::fHistX;    // gcc 3.4
  using TA2H3<Tx,Ty,Tz>::fHistY;    // gcc 3.4
  using TA2H3<Tx,Ty,Tz>::fHistZ;    // gcc 3.4
  using TA2H3<Tx,Ty,Tz>::fWeight;   // gcc 3.4
  using TA2H3<Tx,Ty,Tz>::fCut;      // gcc 3.4
  void Fill()
    { 
      // Fill histogram with multi values held in x y and z buffers
      // Assume one-to-one correspondence between x, y, z data
      // Terminate fill when any end-of-buffer marker found
      if( fCut ){ if( !fCut->Test() ) return; }
      Tx* hbx = fHistX;
      Ty* hby = fHistY;
      Tz* hbz = fHistZ;
      for(;;){
	if( (*hbx == (Tx)EBufferEnd) || (*hby == (Ty)EBufferEnd) ||
	    (*hbz == (Tz)EBufferEnd) ) return;
	TH3F::Fill( (Float_t)(*hbx), (Float_t)(*hby), (Float_t)(*hbz),
		    *fWeight );
	hbx++; hby++; hbz++;
      }
      return;
    }
};

#endif

