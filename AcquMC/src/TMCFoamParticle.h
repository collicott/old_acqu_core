//--Author	JRM Annand   13th Feb 2007  Adapt from TMCParicle	
//--Rev         JRM Annand
//--Update      JRM Annand   25th Jan 2009  Redo constructers

//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TMCFoamParticle
//
// Generate 4-momenta according to "foam method"
// Root class TFoam
//   S. Jadach and P.Sawicki
//   Institute of Nuclear Physics, Cracow, Poland
//   Stanislaw. Jadach@ifj.edu.pl, Pawel.Sawicki@ifj.edu.pl
// Beam + Target -> Reaction -> Products
// 4 momenta generated accoring to N-way differential cross section
// Sampled (in N dimensions) by the TFoam class
// Input function returning value of sigma(X1,X2,...XN) i=1,N
// for a given set of Xi
// Store
// 1) Interaction vertex in target (x,y,z)
// 2) Beam 4-momentum
// 3) 4-momenta of reaction products
// Output as ROOT tree (decodable by macro MCReplay.C)
// OR as HBOOK ntuple, in identical format to mkin for input
// to CB/TAPS GEANT-3 model cbsim

#ifndef __TMCFoamParticle_h__
#define __TMCFoamParticle_h__

#include "TMCParticle.h"
#include "TFoam.h"
#include "TMCFoamInt.h"

class TA2System;

// ordering at which options supplied....
// PDG index then indices of foam vector for kinematic variables
enum{ EITheta, EICosTheta, EIPhi, EIEnergy, EIMomentum };

//-----------------------------------------------------------------------------
class TMCFoamParticle : public TMCParticle {
 protected:
  Double_t* fXscale;                // scaled random generated vector
  Double_t* fScale;                 // Xi linear scale factors
  Int_t fITheta;                    // Theta index of foam vector
  Int_t fICosTheta;                 // Cos(theta) index of foam vector
  Int_t fIPhi;                      // Phi index of foam vector
  Int_t fIEnergy;                   // Energy index of foam vector
  Int_t fIMomentum;                 // Momentum index of foam vector
 public:
  TMCFoamParticle( const Char_t*, TRandom*, TParticlePDG*,
		   Int_t, Char_t*, TA2System* = NULL );
  TMCFoamParticle( const Char_t*, TMCParticle*, Double_t*,
		   Double_t*, Int_t* );
  virtual ~TMCFoamParticle();
  virtual void Flush();
  virtual void Init( );
  virtual Double_t* GetXscale(){ return fXscale; }
  virtual Double_t* GetScale(){ return fScale; }
  virtual Int_t GetITheta(){ return fITheta; }
  virtual Int_t GetICosTheta(){ return fICosTheta; }
  virtual Int_t GetIPhi(){ return fIPhi; }
  virtual Int_t GetIEnergy(){ return fIEnergy; }
  virtual Int_t GetIMomentum(){ return fIMomentum; }
  virtual void SetFoamLink( Int_t* );

  virtual Double_t Theta(){ 
    // return non-uniform random theta
    if( fITheta != -1 ) return fXscale[fITheta];
    else return TMCParticle::Theta();
  }
  virtual Double_t CosTheta(){ 
    // return non-uniform random theta
    if( fICosTheta != -1 ) return fXscale[fICosTheta];
    else if( fITheta != -1 ) return TMath::Cos(fXscale[fITheta]);
    else return TMCParticle::CosTheta();
  }
  virtual Double_t Phi(){
    // return non-uniform random phi
    if( fIPhi != -1 ) return fXscale[fIPhi];
    else return TMCParticle::Phi();
  }
  virtual Double_t Energy(){
    // return non-uniform random energy
    if( fIEnergy != -1 ) return fXscale[fIEnergy];
    else return TMCParticle::Energy();
  }
  virtual Double_t Momentum(){
    // return non-uniform random energy
    if( fIMomentum != -1 ) return fXscale[fIMomentum];
    else return TMCParticle::Momentum();
  }
  virtual Double_t Dist( Int_t i ){ return fXscale[i]; }

  ClassDef(TMCFoamParticle,1)

};

#endif
