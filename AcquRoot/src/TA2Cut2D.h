//--Author	JRM Annand	 3rd Feb 2004
//--Rev 	JRM Annand       6th Apr 2005 For 2D polygon cuts
//--Rev 	JRM Annand      14th Apr 2005 Pass opts to TA2Cut
//--Update	JRM Annand      15th Jul 200 TestXY
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data.
//
// TA2Cut2D
// Implement TCutG in AcquRoot conditions regime for polygon cuts in
// 2D space

#ifndef __TA2Cut2D_h__
#define __TA2Cut2D_h__

#include "TA2Cut.h"                              // Acqu emumerations
#include "TCutG.h"                               // Root polygon cuts

// Base 2D Cut abstract class
// Data variables are templated to type T
template< typename T > class TA2Cut2D : public TA2Cut {
 protected:
  T* fX;                                  // X variable to test
  T* fY;                                  // Y variable to test
  T* fXpoly;                              // X coords of polygon corners
  T* fYpoly;                              // Y coords of polygon corners
  Int_t fNpoly;                           // # polygon corners
 public:
  TA2Cut2D( const char* name, UInt_t link, T* xdata, T* ydata,
	    TCutG* cutg, Double_t* opt ): TA2Cut(name, link, opt)
  { 
    // Extract polygon info from TCutG
    fX = xdata; fY = ydata;            // x-y variable pointers
    fNpoly = cutg->GetN();             // # corners in polygon
    fXpoly = new T[fNpoly];            // polygon X coords
    fYpoly = new T[fNpoly];            // polygon Y coords
    Double_t* x = cutg->GetX();
    Double_t* y = cutg->GetY();
    for( Int_t i=0; i<fNpoly; i++ ){   // copy polygon coords from TCutG
      fXpoly[i] = (T)(*x); fYpoly[i] = (T)(*y);
      x++; y++;
    }
  }
  virtual ~TA2Cut2D(){}
  virtual Bool_t Test() = 0;                // need to define in derived class
  T* GetX(){ return fX; }                   // return ptr to tested X variable
  T* GetY(){ return fY; }                   // return ptr to tested Y variable 
  T* GetXpoly(){ return fXpoly; }           // ptr to polygon X coords
  T* GetYpoly(){ return fYpoly; }           // ptr to polygon Y coords
  Int_t GetNpoly(){ return fNpoly; }        // # corners in polygon
  Bool_t TestXY(T x, T y)
  {
    // Is X-Y (both doubles) inside the polygon
    if( TMath::IsInside( x,y,fNpoly,fXpoly,fYpoly ) ) fIsTrue = ETrue;
    else fIsTrue = EFalse;
    // negate result if NOT flag set
    if( fIsNOT ) fIsTrue = kNOT[fIsTrue];
    return fIsTrue;
  }
  // ClassDefT(TA2Cut2D,1)   
};

// Single-value variable to test
template< typename T > class TA2Cut2DS: public TA2Cut2D<T>{
 public:
  TA2Cut2DS( const char* name, UInt_t link, T* x, T* y,
	     TCutG* cutg, Double_t* opt):
    TA2Cut2D<T>(name, link, x, y, cutg, opt){}
  using TA2Cut2D<T>::fX;      // gcc 3.4
  using TA2Cut2D<T>::fY;      // gcc 3.4
  using TA2Cut2D<T>::fXpoly;  // gcc 3.4
  using TA2Cut2D<T>::fYpoly;  // gcc 3.4
  using TA2Cut2D<T>::fNpoly;  // gcc 3.4
  using TA2Cut2D<T>::fIsTrue; // gcc 3.4
  using TA2Cut2D<T>::fIsNOT;  // gcc 3.4
  Bool_t Test()
    {
      // Is X-Y inside the polygon
      if( TMath::IsInside( *fX,*fY,fNpoly,fXpoly,fYpoly ) ) fIsTrue = ETrue;
      else fIsTrue = EFalse;
      // negate result if NOT flag set
      if( fIsNOT ) fIsTrue = kNOT[fIsTrue];
      return fIsTrue;
    }
};

// Multi-value variable to test
template< typename T > class TA2Cut2DM: public TA2Cut2D<T>{
 public:
  TA2Cut2DM( const char* name, UInt_t link, T* x, T* y,
	     TCutG* cutg, Double_t* opt):
    TA2Cut2D<T>(name, link, x, y, cutg, opt){}
  using TA2Cut2D<T>::fX;      // gcc 3.4
  using TA2Cut2D<T>::fY;      // gcc 3.4
  using TA2Cut2D<T>::fXpoly;  // gcc 3.4
  using TA2Cut2D<T>::fYpoly;  // gcc 3.4
  using TA2Cut2D<T>::fNpoly;  // gcc 3.4
  using TA2Cut2D<T>::fIsTrue; // gcc 3.4
  using TA2Cut2D<T>::fIsNOT;  // gcc 3.4
  Bool_t Test()
    {
      // Is X-Y inside the polygon...check for all multi-values
      // until end of either X or Y data buffer sensed.
      // Result is true if any x-y pair is inside
      T* x = fX;
      T* y = fY;
      fIsTrue = EFalse;
      while( (*x != (T)EBufferEnd) && (*y != (T)EBufferEnd) ){
	if( TMath::IsInside( *fX,*fY,fNpoly,fXpoly,fYpoly ) ){
	  fIsTrue = ETrue;
	  break;
	}
	x++; y++;
      }
      // negate result if NOT flag set
      if( fIsNOT ) fIsTrue = kNOT[fIsTrue];
      return fIsTrue;
    }
};

#endif
