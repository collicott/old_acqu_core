//--Author	JRM Annand    8th Feb 2008
//--Rev         JRM Annand    5th Jun 2009  Prototype polarimetry	
//--Rev         JRM Annand   18th Feb 2010  Asimuthal scattering angle	
//--Update      JRM Annand   24th Mar 2010  Clean up, add angle limits	
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2TOFApparatus
//
// Apparatus class to service TOF arrays.
// Take information from TA2LongScint, and TA2Ladder detector classes
//

#ifndef __TA2TOFApparatus_h__
#define __TA2TOFApparatus_h__

#include "TA2Apparatus.h"
#include "TA2LongScint.h"
#include "TA2GenericDetector.h"
#include "TA2GenericCluster.h"

class TA2Ladder;

enum{ EMaxTOFDetectors = 16, EMaxPolAngle = 16 };

const Double_t kConvBeta = 0.03335641;  // 1/c in cm/ns

class TA2TOFApparatus : public TA2Apparatus {
 protected:
  TA2Detector** fDet;      // list of Pointers to the detector classes
  TA2LongScint** fTOF;     // list of Pointers to the TOF detector classes
  TA2Ladder* fLadder;      // ladder for retiming
  TA2GenericCluster* fPolAnal; // analyser
  TA2GenericCluster* fPolAsym; // asymmetry 
  Double_t* fBeta;         // Velocity
  Double_t* fFlightTime;   // Inverse velocity ns/m
  Double_t* fScatTheta;    // Polarimeter scattering polar angle
  Double_t* fScatPhi;      // Polarimeter scattering asimuthal angle
  Double_t* fDiffTheta;    // Polarimeter angle
  Double_t* fPolTdiff;     // Polarimeter time difference
  Double_t* fPolEsum;      // Polarimeter energy sum
  Double_t fTimeZero;      // Particle production time
  Double_t fTmin;          // limits for polarimeter time difference
  Double_t fTmax;
  Double_t fEmin;          // limits for polarimeter energy
  Double_t fEmax;
  Double_t fThetaMin;      // limits for polarimeter angle
  Double_t fThetaMax;
  Int_t fNDet;             // number of detectors
  Int_t fNTOF;             // number of TOF arrays
  Int_t fNTOFbar;          // number of TOF bars
  Int_t fNPolScatter;      // number of double scatters in polarimeter
 public:
  TA2TOFApparatus( const char*, TA2System* ); // construct called by TAcquRoot
  virtual ~TA2TOFApparatus();
  virtual TA2DataManager* CreateChild( const char*, Int_t );
  virtual void PostInit( );                // finish off initialisation
  virtual void DeleteArrays( );
  virtual void Reconstruct( );
  virtual void RecPol();
  virtual void Cleanup();                  // reset at end of event
  virtual void SetConfig(Char_t*, Int_t);
  virtual void LoadVariable();
  //
  TA2Detector** GetDet(){ return fDet; }
  TA2LongScint** GetTOF(){ return fTOF; }
  TA2Ladder* GetLadder(){ return fLadder; }
  Double_t* GetBeta(){ return fBeta; }
  Double_t* GetFlightTime(){ return fFlightTime; }
  Double_t GetTimeZero(){ return fTimeZero; }
  Int_t GetNDet(){ return fNDet; }
  Int_t GetNTOF(){ return fNTOF; }
  Int_t GetNTOFbar(){ return fNTOFbar; }
  
  ClassDef(TA2TOFApparatus,1)
};

//-----------------------------------------------------------------------------
inline void TA2TOFApparatus::Cleanup( )
{
  // Clear any event-by-event variables
 TA2DataManager::Cleanup();
}
    

#endif
