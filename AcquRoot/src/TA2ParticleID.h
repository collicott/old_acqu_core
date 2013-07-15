//--Author	JRM Annand   27th Sep 2004
//--Rev 	JRM Annand...
//--Rev 	JRM Annand...23rd Oct 2004 Integrate to apparatus
//--Rev 	JRM Annand...17th Mar 2005 Store particle types, SetMassP4
//--Rev 	JRM Annand...15th Apr 2005 SetMassP4 mass in MeV bugfix
//--Rev 	JRM Annand...20th Apr 2005 Remove local 4-mom array
//--Rev 	JRM Annand...22nd Jan 2007 4v0 update
//--Rev 	JRM Annand... 6th Feb 2008 P4 from TOF
//--Update	JRM Annand... 1st Sep 2009 delete[], zap fP4
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2ParticleID
// Particle attributes using PDG numbering scheme....
// As used in GEANT-4, but NOT GEANT-3 
//

#ifndef __TA2ParticleID_h__
#define __TA2ParticleID_h__

#include "TA2System.h"
#include "TDatabasePDG.h"
#include "TPDGCode.h"
#include "TLorentzVector.h"

enum{ kEta = 221 };         // this one is missing from TPDGcode.h

struct ParticleID_t{
  Int_t* fPDGtype;   // list of PDG type numbers $ROOTSYS/include/TPDGCode.h
  Double_t* fPDGconfidence; // confidence "weight" in ID. 1.0 = certain
  Double_t fPx;             // maybe this would be safer if storage
  Double_t fPy;             // required
  Double_t fPz;
  Double_t fE;
};

class TA2ParticleID : public TA2System{
private:
  //  Int_t fNParticle;         // # particles handled in event
  //  Int_t fMaxParticle;       // max # particles handled in event
  Int_t fMaxType;           // # different particle species to handle
  Int_t* fPDGtype;          // considered PDG ID's of particles
  Double_t* fMass;          // Masses of event's particles
  Double_t* fCharge;        // Charges of event's particles
  TDatabasePDG* fPDG;       // PDG data base
public:
  TA2ParticleID( char* );
  virtual ~TA2ParticleID( );
  virtual void SetConfig(char*, Int_t){}
  //  virtual void Reset(){ fNParticle = 0; }
  //  Int_t GetNParticle(){ return fNParticle; }
  //  void SetNParticle( Int_t );
  //  Int_t GetMaxParticle(){ return fMaxParticle; }
  Int_t GetMaxType(){ return fMaxType; }
  TDatabasePDG* GetPDG(){ return fPDG; }
  TParticlePDG* GetPDG( Int_t i ){ return fPDG->GetParticle(i); }
  Double_t Mass( Int_t i ){ return (fPDG->GetParticle(i))->Mass(); }
  Double_t Charge( Int_t i ){ return (fPDG->GetParticle(i))->Charge(); }
  Int_t* GetPDGtype(){ return fPDGtype; }
  Int_t GetPDGtype( Int_t i ){ return fPDGtype[i]; }
  Double_t* GetMass(){ return fMass; }
  Double_t GetMass( Int_t i ){ return fMass[i]; }
  Double_t* GetCharge(){ return fCharge; }
  Double_t GetCharge( Int_t i ){ return fCharge[i]; }
  void SetP4( TLorentzVector*, Int_t, Double_t, TVector3* );
  void SetP4tof( TLorentzVector*, Int_t, Double_t, TVector3* );
  void SetMassP4( TLorentzVector*, Int_t );
  Double_t GetMassMeV( Int_t ipdg )
  { return fPDG->GetParticle(ipdg)->Mass() * 1000; }

  ClassDef(TA2ParticleID,1)
};

//----------------------------------------------------------------------------
inline void TA2ParticleID::SetP4( TLorentzVector* p4, Int_t ipdg, Double_t T,
					   TVector3* pos )
{
  // return pointer to 4 vector for particle i in event
  // particle id ipdg, kinetic energy T, direction pos
  // 1st check there's a particle there

  Double_t m = GetMassMeV( ipdg )                    ; // particle mass MeV
  Double_t E = T + m                        ;          // particle total energy
  p4->SetE( E );
  TVector3 p = pos->Unit() * TMath::Sqrt( E*E - m*m ); // 3 momentum
  p4->SetVect( p );
}

//----------------------------------------------------------------------------
inline void TA2ParticleID::SetP4tof( TLorentzVector* p4, Int_t ipdg,
				     Double_t b, TVector3* pos )
{
  // return pointer to 4 vector for particle i in event
  // particle id ipdg, velocity b, direction pos
  // assume b < 1 checked already
  
  Double_t m = GetMassMeV( ipdg );            // particle mass MeV
  Double_t p2 = b*b*m*m/(1. - b*b);
  Double_t p = TMath::Sqrt(p2);
  Double_t E = sqrt( p2 + m*m );              // particle total energy
  p4->SetE( E );
  TVector3 pm = pos->Unit() * p;              // 3 momentum
  p4->SetVect( pm );
}
  
//----------------------------------------------------------------------------
inline void TA2ParticleID::SetMassP4( TLorentzVector* p4, Int_t ipdg )
{
  // Modify an existing 4 vector for a new particle mass
  // Direction not modifed

  Double_t Mnew = GetMassMeV( ipdg );
  Double_t Enew = p4->E() - p4->M() + Mnew;
  p4->SetE( Enew );
  TVector3 p = p4->Vect().Unit() * TMath::Sqrt( Enew*Enew - Mnew*Mnew );
  p4->SetVect( p );
}
  

#endif
