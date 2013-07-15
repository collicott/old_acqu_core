//--Author	JRM Annand   24th Nov 2008
//--Rev
//--Update
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2GenericPhysics
//
// Framework for reaction reconstruction (physics) classes
// 

#ifndef __TA2GenericPhysics_h__
#define __TA2GenericPhysics_h__

#include "TA2Physics.h"

enum { EMaxGenApp = 8 };

class TA2Particle;

class TA2GenericPhysics : public TA2Physics {
 protected:
  TA2Apparatus* fTAGG;                      // Glasgow photon tagger
  TA2Particle* fTAGGinfo;                   // Tagger info store
  TA2Apparatus* fCB;                        // Crystal Ball
  TA2Apparatus* fTAPS;                      // TAPS
  TA2Apparatus* fTOF;                       // TOF
  TA2Apparatus** fGenApp;                   // generic list of apparati
  TA2Particle** fGenAppInfo;                // lists of particles
  TLorentzVector** fP4photon;               // sorted list of gamma 4 momenta
  TLorentzVector** fP4proton;               // sorted list of proton 4 momenta
  TLorentzVector** fP4piplus;               // sorted list of pi+ 4 momenta
  TLorentzVector** fP4neutron;              // sorted list of neutron 4 momenta
  TLorentzVector** fP4pi0;                  // sorted list of pi0 4 momenta
  TLorentzVector** fP4gprime;               // sorted list of gamma' 4 momenta
  TLorentzVector** fP4eta;                  // sorted list of eta 4 momenta
  TLorentzVector** fP4rootino;              // sorted list of unknown 4 momenta
  Int_t fNGenApp;                           // # connected generic apparati
  Int_t fNParticle;                         // total # particles in event
  Int_t fNphoton;                           // # photon
  Int_t fNproton;                           // # proton
  Int_t fNpiplus;                           // # pi+
  Int_t fNneutron;                          // # neutron
  Int_t fNpi0;                              // # pi0
  Int_t fNgprime;                           // # gamma'
  Int_t fNeta;                              // # eta
  Int_t fNrootino;                          // # unknowns
  Int_t fNprompt;                           // tagger prompts
  Int_t fNrandom;                           // tagger randoms
  Int_t fNparaP;                            // prompts lin-pol para
  Int_t fNparaR;                            // randoms lin-pol para
  Int_t fNperpP;                            // prompts lin-pol perp
  Int_t fNperpR;                            // randoms lin-pol perp
  Int_t fMaxTagg;                           // max # tagger particles
  Int_t fMaxParticle;                       // max # particles
  //
  Double_t* fEm;                  // overall missing mass
  Double_t* fEmP;
  Double_t* fEmR;
 public:
  TA2GenericPhysics( const char*, TA2Analysis* );
  virtual ~TA2GenericPhysics();
  virtual void LoadVariable();            // variables for display/cuts
  virtual void PostInit( );               // init after parameter input
  virtual void SetConfig(Char_t*, Int_t);

  virtual void Reconstruct();             // reconstruct detector info
  virtual TA2DataManager* CreateChild( const char*, Int_t ){ return NULL;}
  virtual void MarkUndefined( Int_t );
  virtual void MarkEndBuffer( );

  ClassDef(TA2GenericPhysics,1)
};

//-----------------------------------------------------------------------------
inline void TA2GenericPhysics::MarkUndefined( Int_t jtagg )
{
  // Initialise undefined
  fEmP[jtagg] = EPhysUndefined;
}

//-----------------------------------------------------------------------------
inline void TA2GenericPhysics::MarkEndBuffer()
{
  // Ensure the multi-data buffers are marked as ended
  fEmP[fNprompt] = EBufferEnd;
  fEmR[fNrandom] = EBufferEnd;
}

#endif
