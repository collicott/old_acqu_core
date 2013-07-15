//--Author	JRM Annand   27th May 2004
//--Rev
//--Rev         JRM Annand    8th Sep 2006  Add SetWalk()
//--Update      JRM Annand    3rd Dec 2008  TimeWalkSergey_t, TimeWalkSven_t
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TimeWalk_t
// Correct hit time obtained from TDC for leading-edge walk
// As of AcquRoot 4v3 3 versions provided
// Basic by JRM Annand
// TimeWalkSergey_t Extended by S Prakov
// TimeWalkSven_t Extended by S Schuman
//

#ifndef __EdgeFit_t_h__
#define __EdgeFit_t_h__

#include "TF1.h"

//-----------------------------------------------------------------------------
// Basic AcquRoot edge fitting class
//-----------------------------------------------------------------------------
class EdgeFit_t {
 protected:
  TH1F* fEdgeHist;
  TF1* fFitFn;
  Int_t fLowCh;
  Int_t fHighCh;
 public:
  virtual void SetFit( Char_t* name, Int_t l1, Int_t l2, TH1F* hist ){
    fEdgeHist = hist;
    fLowCh = l1;
    fHighCh = l2;
    fFitFn = new TF1("edge", name, 0, 10);
  }
  virtual Double_t Edge( ){
    // Find spectrum edge
    Double_t maxCh = fEdgeHist->GetMaximumBin();
    fEdgeHist->Fit( fFitFn, "ROQ", maxCh+fLowCh, maxCh+fHighCh );
    Double_t maxGrad = 0.0;
    Double_t fitEdge;
    for( Double_t f=maxCh; f<maxCh+10; f+=0.1 ){
      Double_t grad = -fFitFn->Derivative();
      if( grad > maxGrad ){
	maxGrad = grad;
	fitEdge=f;
      }
    }
    return fitEdge;
  }
};

//-----------------------------------------------------------------------------
// Modified edge fit class using Gaussian
//-----------------------------------------------------------------------------
class EdgeFitGauss_t : public EdgeFit_t {
 private:
 public:
  Double_t Edge( Double_t energy, Double_t time ){
    // correct the input time on the basis of the input energy
    return ( time  - ( fRiseTime + fThreshold/TMath::Power(energy+fShift,fPower)));
  }
};


#endif
