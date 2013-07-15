//--Author	JRM Annand	22nd Feb 2003
//--Rev 	JRM Annand       4th Feb 2004  Fill conditions
//--Rev 	JRM Annand      10th Feb 2005  Template issues gcc 3.4
//--Update	JRM Annand      10th May 2005  Complete weight installation
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data.
//
// TA2H2
// Wrapper class for TH2F 2D histogram class
// adds pointers to x,y variables to be histogramed and a pointer to a
// weight variable. Able to handle any combination of single/multi value
// variables through child classes of an abstract  base class

#ifndef __TA2H2_h__
#define __TA2H2_h__

#include "TH2F.h"
#include "TA2H.h"

template< typename Tx, typename Ty > class TA2H2 : 
public TA2H, public TH2F {
 protected:
  Tx* fHistX;
  Ty* fHistY;
 public:
  TA2H2(){}
  // Fixed bin width x and y
  TA2H2( char* n, const char* t, Int_t chx, Axis_t lowx, Axis_t highx,
	 Int_t chy, Axis_t lowy, Axis_t highy, Tx* xdata, Ty* ydata,
	 TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H(cut,wgt),
    TH2F(n,t,chx,lowx,highx,chy,lowy,highy)
    { fHistX = xdata; fHistY = ydata; }
  // Fixed x bin width, variable y bin width
  TA2H2( char* n, const char* t, Int_t chx, Axis_t lowx, Axis_t highx,
	 Int_t chy, Double_t* ych, Tx* xdata, Ty* ydata,
	 TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H(cut,wgt),
    TH2F(n,t,chx,lowx,highx,chy,ych)
    { fHistX = xdata; fHistY = ydata; }
  // Variable x bin width, Fixed y bin width
  TA2H2( char* n, const char* t, Int_t chx, Double_t* xch,
	 Int_t chy, Axis_t lowy, Axis_t highy, Tx* xdata, Ty* ydata,
	 TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H(cut,wgt),
    TH2F(n,t,chx,xch,chy,lowy,highy)
    { fHistX = xdata; fHistY = ydata; }
  // Variable x and y bin width
  TA2H2( char* n, const char* t, Int_t chx, Double_t* xch,
	 Int_t chy, Double_t* ych, Tx* xdata, Ty* ydata,
	 TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H(cut,wgt),
    TH2F(n,t,chx,xch,chy,ych)
    { fHistX = xdata; fHistY = ydata; }

  virtual ~TA2H2(){}
  Tx* GetHistX(){ return fHistX; }     // return x data buffer ptr
  Ty* GetHistY(){ return fHistY; }     // return y data buffer ptr

  //  ClassDef(TA2H2,1)   
};

//-----------------------------------------------------------------------------
// Single-X, single-Y value per event histogram
template< typename Tx, typename Ty > class TA2H2SS: public TA2H2<Tx,Ty>{
 public:
  TA2H2SS(){};
  // Fixed x and y bin width
  TA2H2SS( char* n, const char* t, Int_t chx, Axis_t lowx, Axis_t highx,
	   Int_t chy, Axis_t lowy, Axis_t highy, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,lowx,highx,chy,lowy,highy,xdata,ydata,cut,wgt){}
  // Variable x, fixed y bin width
  TA2H2SS( char* n, const char* t, Int_t chx, Double_t* xch,
	   Int_t chy, Axis_t lowy, Axis_t highy, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,xch,chy,lowy,highy,xdata,ydata,cut,wgt){}
  // Fixed x, variable y bin width
  TA2H2SS( char* n, const char* t, Int_t chx, Axis_t lowx, Axis_t highx,
	   Int_t chy, Double_t* ych, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,lowx,highx,chy,ych,xdata,ydata,cut,wgt){}
  // Variable x and y bin width
  TA2H2SS( char* n, const char* t, Int_t chx, Double_t* xch,
	   Int_t chy, Double_t* ych, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,xch,chy,ych,xdata,ydata,cut,wgt){}

  virtual ~TA2H2SS(){}
  using TA2H2<Tx,Ty>::fHistX;    // gcc 3.4
  using TA2H2<Tx,Ty>::fHistY;    // gcc 3.4
  using TA2H2<Tx,Ty>::fWeight;   // gcc 3.4
  using TA2H2<Tx,Ty>::fCut;      // gcc 3.4
  void Fill()
    { 
      // Fill histogram with single values held in x and y buffers
      // No action if "null" marker found in either buffer
      if( fCut ){ if( !fCut->Test() ) return; }
      Tx* hbx = fHistX;
      Ty* hby = fHistY;
      if( (*hbx != ((Tx)EBufferEnd)) && (*hby != ((Ty)EBufferEnd)) )
	TH2F::Fill( (Float_t)(*hbx), (Float_t)(*hby), *fWeight );
      return;
    }
};

//-----------------------------------------------------------------------------
// Single-X, Multi-Y value per event histogram
template< typename Tx, typename Ty > class TA2H2SM: public TA2H2<Tx,Ty>{
 public:
  TA2H2SM(){};
  // Fixed x and y bin width
  TA2H2SM( char* n, const char* t, Int_t chx, Axis_t lowx, Axis_t highx,
	   Int_t chy, Axis_t lowy, Axis_t highy, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,lowx,highx,chy,lowy,highy,xdata,ydata,cut,wgt){}
  // Variable x, fixed y bin width
  TA2H2SM( char* n, const char* t, Int_t chx, Double_t* xch,
	   Int_t chy, Axis_t lowy, Axis_t highy, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,xch,chy,lowy,highy,xdata,ydata,cut,wgt){}
  // Fixed x, variable y bin width
  TA2H2SM( char* n, const char* t, Int_t chx, Axis_t lowx, Axis_t highx,
	   Int_t chy, Double_t* ych, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,lowx,highx,chy,ych,xdata,ydata,cut,wgt){}
  // Variable x and y bin width
  TA2H2SM( char* n, const char* t, Int_t chx, Double_t* xch,
	   Int_t chy, Double_t* ych, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,xch,chy,ych,xdata,ydata,cut,wgt){}

  virtual ~TA2H2SM(){}
  using TA2H2<Tx,Ty>::fHistX;    // gcc 3.4
  using TA2H2<Tx,Ty>::fHistY;    // gcc 3.4
  using TA2H2<Tx,Ty>::fWeight;   // gcc 3.4
  using TA2H2<Tx,Ty>::fCut;      // gcc 3.4
  void Fill()
    { 
      // Fill histogram with values held in x and y buffers
      // No action if "null" marker found in either buffer
      if( fCut ){ if( !fCut->Test() ) return; }
      Tx x = *fHistX;
      if( x == (Tx)EBufferEnd )return;
      Ty* hby = fHistY;
      if( *hby != ((Ty)EBufferEnd) ){
	TH2F::Fill( (Float_t)x, (Float_t)(*hby), *fWeight );
	hby++;
      }
      return;
    }
};

//-----------------------------------------------------------------------------
// Multi-X, single-Y value per event histogram
template< typename Tx, typename Ty > class TA2H2MS: public TA2H2<Tx,Ty>{
 public:
  TA2H2MS(){};
  // Fixed x and y bin width
  TA2H2MS( char* n, const char* t, Int_t chx, Axis_t lowx, Axis_t highx,
	   Int_t chy, Axis_t lowy, Axis_t highy, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,lowx,highx,chy,lowy,highy,xdata,ydata,cut,wgt){}
  // Variable x, fixed y bin width
  TA2H2MS( char* n, const char* t, Int_t chx, Double_t* xch,
	   Int_t chy, Axis_t lowy, Axis_t highy, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,xch,chy,lowy,highy,xdata,ydata,cut,wgt){}
  // Fixed x, variable y bin width
  TA2H2MS( char* n, const char* t, Int_t chx, Axis_t lowx, Axis_t highx,
	   Int_t chy, Double_t* ych, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,lowx,highx,chy,ych,xdata,ydata,cut,wgt){}
  // Variable x and y bin width
  TA2H2MS( char* n, const char* t, Int_t chx, Double_t* xch,
	   Int_t chy, Double_t* ych, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,xch,chy,ych,xdata,ydata,cut,wgt){}

  virtual ~TA2H2MS(){}
  using TA2H2<Tx,Ty>::fHistX;    // gcc 3.4
  using TA2H2<Tx,Ty>::fHistY;    // gcc 3.4
  using TA2H2<Tx,Ty>::fWeight;   // gcc 3.4
  using TA2H2<Tx,Ty>::fCut;      // gcc 3.4
  void Fill()
    { 
      // Fill histogram with values held in x and y buffers
      // No action if "null" marker found in either buffer
      if( fCut ){ if( !fCut->Test() ) return; }
      Ty y = *fHistY;
      if( y == ((Ty)EBufferEnd) ) return;
      Tx* hbx = fHistX;
      while( *hbx != ((Tx)EBufferEnd) ){
	TH2F::Fill( (Float_t)(*hbx), (Float_t)y, *fWeight );
	hbx++;
      }
      return;
    }
};

//-----------------------------------------------------------------------------
// Multi-X, Multi-Y value per event histogram
template< typename Tx, typename Ty > class TA2H2MM: public TA2H2<Tx,Ty>{
 public:
  TA2H2MM(){};
  // Fixed x and y bin width
  TA2H2MM( char* n, const char* t, Int_t chx, Axis_t lowx, Axis_t highx,
	   Int_t chy, Axis_t lowy, Axis_t highy, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,lowx,highx,chy,lowy,highy,xdata,ydata,cut,wgt){}
  // Variable x, fixed y bin width
  TA2H2MM( char* n, const char* t, Int_t chx, Double_t* xch,
	   Int_t chy, Axis_t lowy, Axis_t highy, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,xch,chy,lowy,highy,xdata,ydata,cut,wgt){}
  // Fixed x, variable y bin width
  TA2H2MM( char* n, const char* t, Int_t chx, Axis_t lowx, Axis_t highx,
	   Int_t chy, Double_t* ych, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,lowx,highx,chy,ych,xdata,ydata,cut,wgt){}
  // Variable x and y bin width
  TA2H2MM( char* n, const char* t, Int_t chx, Double_t* xch,
	   Int_t chy, Double_t* ych, Tx* xdata, Ty* ydata,
	   TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H2<Tx,Ty>(n,t,chx,xch,chy,ych,xdata,ydata,cut,wgt){}

  virtual ~TA2H2MM(){}
  using TA2H2<Tx,Ty>::fHistX;    // gcc 3.4
  using TA2H2<Tx,Ty>::fHistY;    // gcc 3.4
  using TA2H2<Tx,Ty>::fWeight;   // gcc 3.4
  using TA2H2<Tx,Ty>::fCut;      // gcc 3.4
  void Fill()
    { 
      // Fill histogram with multi values held in x and y buffers
      // No action if "null" marker found in either buffer
      // Assume one-to-one correspondence between x and y data
      if( fCut ){ if( !fCut->Test() ) return; }
      Tx* hbx = fHistX;
      Ty* hby = fHistY;
      while( (*hbx != ((Tx)EBufferEnd)) && (*hby != ((Ty)EBufferEnd)) ){
	TH2F::Fill( (Float_t)(*hbx), (Float_t)(*hby), *fWeight );
	hbx++;
	hby++;
      }
      return;
    }
};

#endif

