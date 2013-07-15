//--Author	JRM Annand    4th Feb 2003
//--Rev 	JRM Annand...26th Feb 2003...1st production version
//--Rev 	JRM Annand...30th Sep 2003...Incorporate TA2DataManager
//--Rev 	JRM Annand...18th Oct 2004...Periodic, data save, reconstr.
//--Rev 	JRM Annand...22nd Oct 2004...inherit TA2Apparatus
//--Rev 	JRM Annand...21st Jan 2007   4v0 update
//--Update	JRM Annand...25th Nov 2008   EPhysUndefined
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2Physics
//
// Base case for Physics analysis, ie generally starting with 4-vectors
// and assembling these into possible reaction kinematics and determining
// observables.
// "General-purpose" physics impractical, users are expected to provide
// "the beef". Various utilities defined here
// TA2Physics is a Super-Apparatus
//

#ifndef __TA2Physics_h__
#define __TA2Physics_h__

#include "TA2Apparatus.h"
enum { EPhysUndefined = 999999999 };

class TA2Analysis;

class TA2Physics : public TA2Apparatus {
 protected:
  TLorentzVector* fP4miss;              // missing momenta
  TLorentzVector* fP4target;            // target 4 vector (at rest)
  TVector3 fTargetPos;                  // target position
  Double_t* fMmiss;                     // missing mass
  Double_t* fPmiss;                     // missing momentum magnitude
  Double_t* fPtheta;                    // missing momentum theta
  Double_t* fPphi;                      // missing momentum phi
  Int_t fNtarget;                       // # target nuclei
  Int_t fNperm;                         // # general purpose
  Int_t* fNpermutation;                 // wild card stuff

public:
  TA2Physics( const char*, TA2Analysis* );
  virtual ~TA2Physics();
  virtual void LoadVariable();            // variables for display/cuts
  virtual void SetConfig( char*, int );   // setup decode in implement
  virtual void PostInit( );               // init after parameter input
  virtual void Reconstruct();             // reconstruct detector info
  virtual TA2DataManager* CreateChild( const char*, Int_t ){ return NULL;}
  virtual void Periodic();                // periodic task

  TLorentzVector* GetP4miss(){ return fP4miss; }// missing 4 momenta
  TLorentzVector* GetP4target(){ return fP4target;}// target 4 momenta
  TVector3 GetTargetPos(){ return fTargetPos; } // target position
  Double_t* GetMmiss(){ return fMmiss; }        // missing masses
  Double_t* GetPmiss(){ return fPmiss; }        // missing momenta
  Double_t* GetPtheta(){ return fPtheta; }      // recoil polar angles
  Double_t* GetPphi(){ return fPphi; }          // recoil asim angles
  Int_t GetNtarget(){ return fNtarget; }        // # target nuclei
  Int_t GetNperm(){ return fNperm; }            // # general purpose integers
  Int_t* GetNpermutation(){ return fNpermutation; }// general purpose integers
  
  ClassDef(TA2Physics,1)
};

//-----------------------------------------------------------------------------
inline void TA2Physics::Periodic( )
{
  // Trivial example of periodic execution procedure
  //
  if( !UpdatePeriod() ) return;
  // Anything else comes here
}
#endif
