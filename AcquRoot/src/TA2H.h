//--Author	JRM Annand   13th Apr 2005
//--Update	JRM Annand
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2H
// Base AcquRoot wrapper class for ROOT histograms.
// Contains further information on any condition which must be met
// before histogram is incremented and the histogram increment weight
// Default increment weight is 1.0

#ifndef __TA2H_h__
#define __TA2H_h__

#include "TA2Cut.h"                                 // AcquRoot conditions
#include "TH1.h"

class TA2H {
 protected:
  TA2Cut* fCut;
  Stat_t* fWeight;
  Bool_t fIsWeight;
 public:
  TA2H(){}
  TA2H( TA2Cut* = NULL, Stat_t* = NULL );
  virtual ~TA2H();
  virtual void Fill() = 0;                            // define derived class
  void SetCut( TA2Cut* cut ){ fCut = cut; }           // specify cut
  TA2Cut* GetCut( ){ return fCut; }                   // get cut ptr
  void SetWeight( Double_t wgt ){ *fWeight = wgt; }   // set weight
  void SetWeight( Double_t* wgt ){ fWeight = wgt; }   // set weight variable
  Stat_t GetWeightVal(){ return *fWeight; }           // get weight ptr
  Stat_t* GetWeight(){ return fWeight; }              // get weight value
  Bool_t IsWeight(){ return fIsWeight; }              // non-unity weight?
  ClassDef(TA2H,1)   
};

#endif

