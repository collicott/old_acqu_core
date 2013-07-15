//--Author	JRM Annand   24th May 2005	       
//--Rev 	JRM Annand... 3rd Jun 2008...const Char_t*...gcc 4.3 
//--Rev 	JRM Annand...25th Jan 2009...redo constructer
//--Update	JRM Annand...27th Jan 2010...copy constructer option

//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TMCResonance
//
// Hadronic resonance specification for Monte Carlo
// Inherits from TMCParticle and in addition has a Breit-Wigner
// distribution of mass.

#ifndef __TMCResonance_h__
#define __TMCResonance_h__


#include "TMCParticle.h"

class TMCResonance : public TMCParticle {
 protected:
  Double_t fGamma;                           // Breit-wigner width
  TH1D* fMassDist;                           // mass distribution
  Double_t fMLow;                            // low mass limit
  Double_t fMHigh;                           // high mass limit
 public:
  TMCResonance( Char_t*, TMCParticle*, Double_t=0, Double_t=0, Double_t=0 );
  virtual ~TMCResonance();
  virtual TH1D** FindDist( Int_t, Char_t*, Double_t = 0, Double_t = 0);
  virtual Double_t Mass(){ return fMassDist->GetRandom(); }
  Double_t GetGamma(){ return fGamma; }
  TH1D* GetMassDist(){ return fMassDist; }
  Double_t GetMLow(){ return fMLow; }
  Double_t GetMHigh(){ return fMHigh; }

  ClassDef(TMCResonance,1)
};


#endif


