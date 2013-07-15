//--Author	JRM Annand   24th May 2005	
//--Rev         JRM Annand   14th Sep 2005  re-do phase-space generators
//--Rev         JRM Annand   24th Apr 2006  non-uniform decay particle dist.
//--Rev         JRM Annand   14th Feb 2007  Gen2Decay mods
//--Rev         JRM Annand   16th Jul 2008  QF from TMCGenerator to Gen2Decay
//--Rev         JRM Annand   25th Jan 2009  Redo constructers for PDG ID
//--Rev         JRM Annand   27th Jan 2009  Feedback QF momentum to beam energy
//--Rev         JRM Annand    5th Feb 2009  Bug fix SetDist( Int_t, Char_t*....
//--Rev         JRM Annand    5th Mar 2009  Bug fix ResetT()
//--Rev 	JRM Annand...10th May 2009  QF without beam-energy compensation
//--Rev 	JRM Annand... 5th Jun 2009  Add GetFirstDecay() Gen1Decay()
//--Rev 	JRM Annand...10th Jun 2009  Fix for BoostLab()
//--Rev 	JRM Annand...27th Jan 2010  Gen2Decay() resonant mass distr.
//--Rev 	JRM Annand...25th Feb 2012  Out-of-plane angle
//--Update 	JRM Annand...27th Feb 2012  Use degrees option

//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
// 
// TMCParticle
//
// Particle specification and kinematics generation for the Monte Carlo
// event generator TMCGenerator

#ifndef __TMCParticle_h__
#define __TMCParticle_h__

#include "TF1.h"
#include "TH1D.h"
#include "TParticlePDG.h"
#include "TList.h"
#include "TLorentzVector.h"
#include "TLorentzRotation.h"
#include "TRandom.h"

class TMCParticle : public TNamed {
 protected:
  TRandom*        fRandom;            // random number generator
  TParticlePDG*   fPDG;               // PDG particle properties
  TMCParticle*    fParent;            // parent particle (if decay product)
  TMCParticle*    fBeam;              // beam particle (used in QF)
  TLorentzVector* fP4;                // 4 momentum in Lab frame
  TLorentzVector** fP4d;              // 4 momenta of decay particles
  //  TLorentzVector* fP4Beam;            // beam 4-momentum...only used for QF
  TList*          fDecayList;         // list of decay products
  TH1D*           fThetaDist;         // angular distribution
  Double_t        fThetaLow;          // low limit polar angle
  Double_t        fThetaHigh;         // high limit polar angle
  TH1D*           fCosThDist;         // angular distribution
  Double_t        fCosThLow;          // low limit polar angle
  Double_t        fCosThHigh;         // high limit polar angle
  TH1D*           fPhiDist;           // angular distribution
  Double_t        fPhiLow;            // low limit asimuthal angle
  Double_t        fPhiHigh;           // high limit asimuthal angle
  TH1D*           fTDist;             // kinetic energy distribution
  Double_t        fTLow;              // low limit kinetic energy
  Double_t        fTHigh;             // high limit kinetic energy
  TH1D*           fPDist;             // |momentum| distribution
  Double_t        fPLow;              // low limit momentum
  Double_t        fPHigh;             // high limit momentum
  TH1D*           fOOPDist;           // Out-of-plane distribution
  Double_t        fOOPLow;            // low limit out-of-plane angle
  Double_t        fOOPHigh;           // high limit out-of-plane angle
  Double_t        fMass;              // particle mass
  Double_t*       fMd;                // decay particle masses
  Double_t*       fMd2;               // decay mass squared
  Double_t        fTcm;               // kinetic energy in CM
  Double_t        fWt;                // weight for particular event, fNd > 3
  Double_t        fWtMax;             // maximum weight, fNd > 3
  Int_t           fPDGindex;          // PDG code
  Int_t           fNd;                // # decay particles
  Int_t           fDecayMode;         // mode of decay
  Bool_t          fIsDecay;           // does it decay?
  Bool_t          fIsTrack;           // track this particle?
  Bool_t          fIsResonance;       // true = unstable resonance
  Bool_t          fUseDegrees;        // input angle limits in degrees
  //
  Double_t Pdk(Double_t a, Double_t b, Double_t c);  
 public:
  TMCParticle( const Char_t*, TRandom*, Double_t, Int_t, Bool_t = kFALSE,
	       Bool_t = kTRUE, Bool_t = kFALSE );
  TMCParticle( const Char_t*, TRandom*, TParticlePDG*,
	       Bool_t = kFALSE, Bool_t = kTRUE, Bool_t = kFALSE );
  TMCParticle( const Char_t*, TMCParticle* );
  virtual ~TMCParticle();
  virtual void Flush();
  virtual void Init();
  virtual void PostInit();
  virtual Bool_t GenDecay( );
  virtual Bool_t Gen1Decay( );
  virtual Bool_t Gen2Decay( );  
  virtual Bool_t GenNDecay( );
  virtual void BoostLab( );
  virtual TH1D** FindDist( Int_t, Char_t*, Double_t, Double_t );
  virtual void SetDist( Int_t, Int_t, Double_t*, Double_t*,
			Double_t, Double_t );
  virtual void SetDist( Int_t, TF1*, Int_t, Double_t, Double_t );
  virtual void SetDist( Int_t, Char_t*, Double_t*,
			Int_t, Double_t, Double_t );
  virtual void AddDecay( TMCParticle* );
  virtual TMCParticle* ReplaceDecay( TMCParticle* decay );
  virtual void GenMCDist(){ }   // action only necessary for foam particle
  // Getters
  TRandom* GetRandom(){ return fRandom; }
  TParticlePDG* GetPDG(){ return fPDG; }
  void SetParent( TMCParticle* parent ){ fParent = parent; }
  TMCParticle* GetParent(){ return fParent; }
  TLorentzVector* GetP4(){ return fP4; }
  void SetP4( TLorentzVector* p4 ){ *fP4 = *p4; }
  void SetP4( TLorentzVector p4 ){ *fP4 = p4; }
  TLorentzVector** GetP4d(){ return fP4d; }
  TLorentzVector* GetP4d(Int_t i){ return fP4d[i]; }
  //  TLorentzVector* GetP4Beam(){ return fP4Beam; }
  void SetBeam(TMCParticle* beam){ fBeam = beam; }
  TList* GetDecayList(){ return fDecayList; }
  TH1D* GetThetaDist(){ return fThetaDist; }
  Double_t GetThetaLow(){ return fThetaLow; }
  Double_t GetThetaHigh(){ return fThetaHigh; }
  TH1D* GetCosThDist(){ return fCosThDist; }
  Double_t GetCosThLow(){ return fCosThLow; }
  Double_t GetCosThHigh(){ return fCosThHigh; }
  TH1D* GetPhiDist(){ return fPhiDist; }
  //  TH1D* GetPhiDist(){ return fThetaDist; }   // bug fix 25/02/12 JRMA
  Double_t GetPhiLow(){ return fPhiLow; }
  Double_t GetPhiHigh(){ return fPhiHigh; }
  TH1D* GetTDist(){ return fTDist; }
  Double_t GetTLow(){ return fTLow; }
  Double_t GetTHigh(){ return fTHigh; }
  TH1D* GetPDist(){ return fPDist; }
  Double_t GetPLow(){ return fPLow; }
  Double_t GetPHigh(){ return fPHigh; }
  TH1D* GetOOPDist(){ return fOOPDist; }        // out-of-plane angle
  Double_t GetOOPLow(){ return fOOPLow; }
  Double_t GetOOPHigh(){ return fOOPHigh; }
  Double_t GetMass(){ return fMass; }
  virtual Double_t Mass( ){ return fMass; }
  void SetMass( Double_t m ){ if( fIsResonance ) fMass = m; }
  Double_t* GetMd(){ return fMd; }
  Double_t* GetMd2(){ return fMd2; }
  Double_t GetTcm(){ return fTcm; }
  Double_t GetWt(){ return fWt; }
  Double_t GetWtMax(){ return fWtMax; }
  Int_t GetPDGindex(){ return fPDGindex; }
  Int_t GetNd() { return fNd;}
  Int_t GetDecayMode(){ return fDecayMode; }
  void SetDecayMode( Int_t mode ){ fDecayMode = mode; }
  Bool_t IsDecay(){ return fIsDecay; }
  Bool_t IsTrack(){ return fIsTrack; }
  Bool_t IsResonance(){ return fIsResonance; }
  void SetDecay(Int_t isdec){ fIsDecay = (Bool_t)isdec; }
  void SetTrack(Int_t istr){ fIsTrack = (Bool_t)istr; }
  //
  virtual TMCParticle* GetDecay( Char_t* name )
  // search by name decay list for particle
  { return (TMCParticle*)fDecayList->FindObject( name ); }

 virtual TMCParticle* GetFirstDecay( )
  // get the 1st particle on the decay list
  { return (TMCParticle*)fDecayList->First( ); }

  virtual Double_t Theta(){ 
    // return non-uniform random theta
    if( fThetaDist )return fThetaDist->GetRandom();
    else return fRandom->Uniform( fThetaLow, fThetaHigh );
  }
  virtual Double_t CosTheta(){ 
    // return non-uniform random theta
    if( fCosThDist )return fCosThDist->GetRandom();
    else return fRandom->Uniform( fCosThLow, fCosThHigh );
  }
  virtual Double_t Phi(){
    // return non-uniform random phi
    if( fPhiDist ) return fPhiDist->GetRandom(); 
    else return fRandom->Uniform( fPhiLow, fPhiHigh );
  }
  virtual Double_t Energy(){ 
    // return non-uniform random energy
    if( fTDist ) return fTDist->GetRandom(); 
    else return fRandom->Uniform( fTLow, fTHigh );
  }
  virtual Double_t Momentum(){ 
    // return non-uniform random momentum
    if( fPDist ) return fPDist->GetRandom(); 
    else return fRandom->Uniform( fPLow, fPHigh );
  }
  virtual Double_t OOP(){ 
    // return uniform or non-uniform random out-of-plane angle
    if( fOOPDist ) return fOOPDist->GetRandom(); 
    else return fRandom->Uniform( fOOPLow, fOOPHigh );
  }
  virtual TLorentzVector P4( Double_t m = -1.0 ){
    // return random 4 momentum
    if( m < 0.0 ) m = fMass;
    Double_t costh = CosTheta();
    Double_t sinth = TMath::Sqrt( 1.0 - costh*costh );
    Double_t phi = Phi();
    Double_t p = Momentum();
    TLorentzVector p4( p * sinth * TMath::Cos( phi ),
		       p * sinth * TMath::Sin( phi ),
		       p * costh,
		       TMath::Sqrt( p*p + m*m ) );
    return p4;
  }
  virtual void ResetT( Double_t dT ){
    // apply new kinetic energy T' = T + dT
    // do not change invariant mass or direction
    //
    Double_t m = fP4->M();             // save inv mass
    TVector3 u = fP4->Vect().Unit();  // save direction
    Double_t newE = fP4->E() + dT;
    Double_t newP = TMath::Sqrt( newE*newE - m*m );
    fP4->SetVect( newP*u );
    fP4->SetE( newE );
  }
  ClassDef(TMCParticle,1)

};

//----------------------------------------------------------------------------
inline Bool_t TMCParticle::GenDecay( )
{
  // Decide which phase-space generator to use and call it
  // For 2-body use simple algorithm which allows non phase-space angular
  // distributions. For >2 body use general recursive algorithm which allows
  // phase space only

  switch( fNd ){
  case 1:
    // trival case...decay is parent
    return Gen1Decay( );
  case 2:
    // simple 2-body
    return Gen2Decay( ); 
  default:
    // any >=3-particle decay (based on genbod)
    return GenNDecay( );
  }
}

//----------------------------------------------------------------------------
inline Double_t TMCParticle::Pdk(Double_t a, Double_t b, Double_t c) 
{
  // Utility

  Double_t x = (a-b-c)*(a+b+c)*(a-b+c)*(a+b-c);
  x = TMath::Sqrt(x)/(2*a);
  return x;
}

//----------------------------------------------------------------------------
inline void TMCParticle::BoostLab( ) 
{
  // Final boost of all decay particles from the centre of mass of the parent
  // particle to the lab frame.
  // Fixed for cases where beam direction is not along z-axis

  Double_t thR = fP4->Theta();
  Double_t phiR = fP4->Phi();
  //  Double_t th = fP4->Theta()*TMath::RadToDeg();
  for ( Int_t n=0; n<fNd; n++ ){
    //    th = fP4d[n]->Theta()*TMath::RadToDeg();
    fP4d[n]->Boost(0,0,fP4->P()/fP4->E());
    fP4d[n]->RotateY(thR);
    fP4d[n]->RotateZ(phiR);
    //    th = fP4d[n]->Theta()*TMath::RadToDeg();
  }
}

//----------------------------------------------------------------------------
inline Bool_t TMCParticle::Gen1Decay( ) 
{
  *fP4d[0] = *fP4;
  return kTRUE;
}

#endif
