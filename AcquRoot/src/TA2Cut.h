//--Author	JRM Annand    5th Feb 2004
//--Update	JRM Annand    6th Apr 2005 add ECutMisc, ECutCMisc
//--Update      JRM Annand   14th Apr 2005 Wild-card options for cut
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TTA2Cut
//
// Abstract base class for any condition
// Specify set of procedures which user-defined derivatives
// must implement
// User-defined condition classes must inherit from this one
//
#ifndef __TA2Cut_h__
#define __TA2Cut_h__

#include "TNamed.h"
#include "EnumConst.h"                              // Acqu emumerations

// Determine scope of cut..applied to raw data, partially analysed data
// or to histograms
enum { ECutRawData, ECutData, ECutHistogram, ECutMisc, ECutCHist, ECutCData,
       ECutCMisc, ECutElement,
       ECutOR, ECutAND, ECutNOR, ECutNAND};
// Type of variable to which cut condition applied
enum { ECutSingleX, ECutMultiX, ECutSingleY, ECutMultiY, ECutMultiXY };
// cut linkage for chain of cuts

//template<class T>
struct Name2Variable_t{
  // Associate address of variable  and variable type
  // with character string
  char* fName;                // name of variable
  //  T* fVariable;
  Double_t* fVariable;
  Int_t fSingle;              // single/multi value variable
};

// True, False negate
const Bool_t kNOT[] = { ETrue, EFalse };


class TA2Cut : public TNamed {
 private:
 protected:
  Bool_t fIsTrue;                               // condition met?
  Bool_t fIsNOT;                                // logical netation?
  UInt_t fLink;                                 // for logical combination
  Double_t* fOpt;                               // wild-card options
  Int_t fNOpt;                                  // # option parameters
 public:
  TA2Cut( const char*, UInt_t = ECutOR, Double_t* = NULL );
  virtual ~TA2Cut();
  virtual Bool_t Test( ) = 0;
  virtual void Cleanup( ){ fIsTrue = EFalse; }
  Bool_t NOT( Bool_t result ){ return kNOT[result]; }
  Bool_t IsTrue(){ return fIsTrue; }
  UInt_t GetLink(){ return fLink; }
  Double_t* GetOpt(){ return fOpt; }
  Double_t GetOpt(Int_t i){ return fOpt[i]; }
  Int_t GetNOpt(){ return fNOpt; }
  ClassDef(TA2Cut,1)
};

#endif
