//--Author	JRM Annand	 3rd Feb 2004
//--Rev 	JRM Annand      10th Feb 2005 Templating issues gcc 3.4
//--Update	JRM Annand      14th Apr 2005 Pass opts to TA2Cut
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data.
//
// TA2Cut1D
// Check if a variable falls between low and high limits

#ifndef __TA2Cut1D_h__
#define __TA2Cut1D_h__

#include "TA2Cut.h"                              // Acqu emumerations

// Base 1D Cut abstract class
// Data variable is templated to type T
template< typename T > class TA2Cut1D : public TA2Cut {
 protected:
  T* fValue;                                 // variable to test
  T fLow;                                    // low value
  T fHigh;                                   // high value
 public:
  TA2Cut1D( const char* name, UInt_t link, 
	    T low, T high, T* data, Double_t* opt ): TA2Cut(name, link, opt)
  { fValue = data; fLow = low; fHigh = high; }
  virtual ~TA2Cut1D(){}
  virtual Bool_t Test() = 0;                // need to define in derived class
  T* GetValue(){ return fValue; }           // return ptr to tested variable
  T GetLow(){ return fLow; }                // return low cut value
  T GetHigh(){ return fHigh; }              // return high cut value
  // ClassDefT(TA2Cut1D,1)   
};

// Single-value variable to test
template< typename T > class TA2Cut1DS: public TA2Cut1D<T>{
 public:
  TA2Cut1DS( const char* name, UInt_t link, T low, T high, 
	     T* data, Double_t* opt):
    TA2Cut1D<T>(name, link, low, high, data, opt){}
  using TA2Cut1D<T>::fValue;  // gcc 3.4
  using TA2Cut1D<T>::fLow;    // gcc 3.4
  using TA2Cut1D<T>::fHigh;   // gcc 3.4
  using TA2Cut1D<T>::fIsTrue; // gcc 3.4
  using TA2Cut1D<T>::fIsNOT;  // gcc 3.4
  Bool_t Test()
    {
      // Compare the single value in the buffer with the low-high window
      if( (fLow <= *fValue) && (*fValue <= fHigh ) ) fIsTrue = ETrue;
      else fIsTrue = EFalse;
      // negate result if NOT flag set
      if( fIsNOT ) fIsTrue = kNOT[fIsTrue];
      return fIsTrue;
    }
};

// Multi-value variable to test
template< typename T > class TA2Cut1DM: public TA2Cut1D<T>{
 public:
  TA2Cut1DM( const char* name, UInt_t link, T low, T high, 
	     T* data, Double_t* opt):
    TA2Cut1D<T>(name, link, low, high, data, opt){}
  using TA2Cut1D<T>::fValue;  // gcc 3.4
  using TA2Cut1D<T>::fLow;    // gcc 3.4
  using TA2Cut1D<T>::fHigh;   // gcc 3.4
  using TA2Cut1D<T>::fIsTrue; // gcc 3.4
  using TA2Cut1D<T>::fIsNOT;  // gcc 3.4
  Bool_t Test()
    {
      // Compare values in the buffer with the low and high window values
      // If any value falls within the low-high window return True
      // ie. its an OR
      T* v = fValue;
      fIsTrue = EFalse;
      while( *v != (T)EBufferEnd ){
	if( (fLow <= *v) && (*v <= fHigh ) ){
	  fIsTrue = ETrue;
	  break;
	}
	v++;
      }
      // negate result if NOT flag set
      if( fIsNOT ) fIsTrue = kNOT[fIsTrue];
      return fIsTrue;
    }
};



#endif
