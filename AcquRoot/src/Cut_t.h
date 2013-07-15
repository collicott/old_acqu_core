//----------------------------------------------------------------------------
//			Acqu-Root++
//		Data Acquisition and Analysis
//			for
//		Nuclear Physics Experiments
//
//		    *** TCuts.h ***

//--Description
//	TCuts
//	Simple data cuts
//
//--Author	JRM Annand	17th Feb 2003
//--Update	JRM Annand      21st Oct 2005  Getters for thresholds
//
//---------------------------------------------------------------------------
#ifndef __TCuts_h__
#define __TCuts_h__

#include "EnumConst.h"

class Cut_t {
private:
  Double_t fLowThr;                            // pedestal
  Double_t fHighThr;                           // conversion gain
public:
  Cut_t( Double_t low, Double_t high ){
    // store input parameters
    fLowThr = low; fHighThr = high;
  }
  virtual ~Cut_t(){};
  Bool_t Window( Double_t value ){
    if( (value >= fLowThr) && (value <= fHighThr) ) return ETrue;
    else return EFalse;
  }
  Bool_t Window( UShort_t* adc ){
    Double_t value = (Double_t)(*adc);
    if( (value >= fLowThr) && (value <= fHighThr) ) return ETrue;
    else return EFalse;
  }
  Double_t GetLowThr(){ return fLowThr; }
  Double_t GetHighThr(){ return fHighThr; }
};

#endif
