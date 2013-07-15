//--Author	JRM Annand   27th Apr 2003
//--Rev 	JRM Annand...30th Sep 2003  New Detector bases
//--Rev 	JRM Annand...15th Oct 2003  ReadDecoded...MC data
//--Rev 	JRM Annand... 5th Feb 2004  3v8 compatible
//--Rev 	JRM Annand...25th Jul 2005  SetConfig hierarchy
//--Rev 	JRM Annand...18th Jan 2007  Inherit from TA2Detector
//--Rev 	JRM Annand... 6th Feb 2008  Display direction, flight path
//--Rev 	JRM Annand...16th Sep 2008  Extend ReadDecoded() for MC
//--Rev 	JRM Annand... 4th Dec 2008  Inherit TA2GenericDetector
//--Update	JRM Annand...11th Oct 2012  Use TA2GenericDetector::Decode
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2LongScint
//
// Decoding and calibration methods for a long bar of plastic scintillator
// Assumes one PMT at each end of bar -> position sensitivity, mean time etc.
//

#ifndef __TA2LongScint_h__
#define __TA2LongScint_h__

#include "MCBranchID.h"
#include "TA2GenericDetector.h"
#include "LongBar_t.h"

class TA2LongScint : public TA2GenericDetector {
 private:
 protected:
  LongBar_t** fBar;                    // Array of scintillator bar info
  Double_t* fMeanEnergy;               // Mean pulse heights
  Double_t* fMeanEnergyOR;             // Mean pulse-height OR
  Double_t* fMeanTime;                 // Mean times
  Double_t* fMeanTimeOR;               // Mean-time OR
  Double_t* fTimeDiff;                 // Time difference
  Double_t* fTimeDiffOR;               // Time difference OR
  Double_t* fThetaOR;                  // theta of bar hit
  Double_t* fPhiOR;                    // phi of bar hit
  Double_t* fFlightOR;                 // flight path of bar hits
  Double_t* fTheta;                    // theta of bar hit
  Double_t* fPhi;                      // phi of bar hit
  Double_t* fFlight;                   // flight path of bar hits
  TVector3* fBarPos;                   // Hit position on Bar
  Int_t* fBarHits;                     // Scint-bar hits
  Int_t* fBarHitCount;                 // # elements fired for each bar
  Int_t* fElement2Bar;                 // element-bar correspondence
  Int_t* fSmearHits;                   // MC hits after smearing effects
  Int_t fNBarHits;                     // # scint-bar hits
  Int_t fNBar;                         // # scintillator bars
  Int_t fNbar;                         // # scintillator bars running counter
  Int_t fNFrontBar;                    // # forward scintillator bars
  UInt_t fNSmearHits;                  // # MC hits after smearing effects

 public:
  TA2LongScint( const char*, TA2System* );// Normal use
  virtual ~TA2LongScint();
  virtual void PostInit( );            // initialise using setup info
  virtual void ParseDisplay(char*);    // display setup
  virtual void SetConfig(Char_t*, Int_t);
  virtual void LoadVariable();
  virtual void Decode( );              // hits -> energy procedure
  virtual void DecodeBar();            // combine hits to bar
  virtual void Cleanup( );             // end-of-event cleanup
  virtual void SaveDecoded( );         // save local analysis
  virtual void ReadDecoded( );         // read back previous analysis
  virtual void TimeCorrect( Double_t );// apply start time correction
  virtual void SmearMCHit(Int_t,Double_t,Double_t,Double_t,Double_t,Double_t);
  //
  LongBar_t** GetBar(){ return fBar; }
  LongBar_t* GetBar(Int_t i){ return fBar[i]; };
  Double_t* GetMeanEnergy(){ return fMeanEnergy; }
  Double_t GetMeanEnergy(Int_t i){ return fMeanEnergy[i]; }
  Double_t* GetMeanEnergyOR(){ return fMeanEnergyOR; }
  Double_t* GetMeanTime(){ return fMeanTime; }
  Double_t GetMeanTime(Int_t i){ return fMeanTime[i]; }
  Double_t* GetMeanTimeOR(){ return fMeanTimeOR; }
  Double_t* GetTimeDiff(){ return fTimeDiff; }
  Double_t GetTimeDiff(Int_t i){ return fTimeDiff[i]; }
  Double_t* GetTimeDiffOR(){ return fTimeDiffOR; }
  Double_t* GetThetaOR(){ return fThetaOR; }
  Double_t* GetPhiOR(){ return fPhiOR; }
  Double_t* GetFlightOR(){ return fFlightOR; }
  Double_t* GetTheta(){ return fTheta; }
  Double_t* GetPhi(){ return fPhi; }
  Double_t* GetFlight(){ return fFlight; }
  Double_t GetTheta(Int_t i){ return fTheta[i]; }
  Double_t GetPhi(Int_t i){ return fPhi[i]; }
  Double_t GetFlight(Int_t i){ return fFlight[i]; }
  TVector3* GetBarPos(){ return fBarPos; }
  TVector3 GetBarPos(Int_t i){ return fBarPos[i]; }
  Int_t* GetBarHits(){ return fBarHits; }
  Int_t GetNBarHits(){ return fNBarHits; }
  Int_t GetNBar(){ return fNBar; }
  Int_t GetNbar(){ return fNbar; }

  ClassDef(TA2LongScint,1)
};

//---------------------------------------------------------------------------
inline void TA2LongScint::Decode( )
{
  // Decode the NaI ADC and TDC information
  // Decode raw TDC and Scaler information into
  // Hit pattern, "Energy" pattern, aligned OR etc.

  TA2GenericDetector::Decode();
  DecodeBar();
}

//---------------------------------------------------------------------------
inline void TA2LongScint::DecodeBar()
{
  // Step through PMT hits array
  // back reference PMTs to bars and require 2 PMT hits for a bar hit
  // Calculate bar quantities....mean time, position etc.
  Int_t j;
  fNBarHits = 0;
  for( UInt_t i=0; i<fNhits; i++ ){   // step through decoded hits
    j = fElement2Bar[ fHits[i] ];     // ref bar to PMT
    fBarHitCount[j]++;
    if( fBarHitCount[j] == 2 ){       // require both PMTs fire
      if( fBar[j]->TDiffCheck() ){    // check & record time difference
	fBarHits[fNBarHits] = j;        // record bar hit
	fBar[j]->TMean();               // mean time
	fBar[j]->GMean();               // mean energy
	fBar[j]->Pos();                 // hit position
	// Save OR quanties for diagnostic spectra
	fMeanEnergyOR[fNBarHits] = fMeanEnergy[j];
	fMeanTimeOR[fNBarHits] = fMeanTime[j];
	fTimeDiffOR[fNBarHits] = fTimeDiff[j];
	fThetaOR[fNBarHits] = fTheta[j] = 
	  fBarPos[j].Theta() * TMath::RadToDeg();
	fPhiOR[fNBarHits] = fPhi[j] = 
	  fBarPos[j].Phi() * TMath::RadToDeg();
	fFlightOR[fNBarHits] = fFlight[j] = fBarPos[j].Mag();
	fNBarHits++;                    // hit counter
      }
    }
  }
  // mark end of variable length arrays
  fBarHits[fNBarHits] = EBufferEnd;
  fMeanEnergyOR[fNBarHits] = EBufferEnd;
  fMeanTimeOR[fNBarHits] = EBufferEnd;
  fTimeDiffOR[fNBarHits] = EBufferEnd;
  fThetaOR[fNBarHits] = EBufferEnd;
  fPhiOR[fNBarHits] = EBufferEnd;
  fFlightOR[fNBarHits] = EBufferEnd;
}

//---------------------------------------------------------------------------
inline void TA2LongScint::ReadDecoded( )
{
  // Read info from A2 Monte Carlo output...energy, time, position, index
  // Reconstruct individual PMT response on the basis of
  // 1) Dist from hit to PMT's 1 & 2
  // 2) Loss of scintillation signal due to attenuation
  // 3) Pulse height smearing (resolution effects)
  // 4) Time scintillation light to travel from hit to PMT's
  // 5) Time-pickoff resolution
  //
  Int_t j,k;
  TA2GenericDetector::ReadDecoded();
  fNSmearHits = 0;
  for( UInt_t i=0; i<fNhits; i++ ){
    j = fHits[i];
    k = fElement2Bar[j];
    SmearMCHit(k,fEnergy[j],fTime[j],fXMC[j],fYMC[j],fZMC[j] );
  }
  for( UInt_t i=0; i<fNSmearHits; i++ ) fHits[i] = fSmearHits[i];
  fNhits = fNSmearHits;
  fHits[fNhits] = EBufferEnd;
  if( fNbar )DecodeBar();
}

//---------------------------------------------------------------------------
inline void TA2LongScint::TimeCorrect( Double_t toff )
{
  // Apply correction to time arrays...
  // e.g. correction to common TDC start/stop time
  for(Int_t j=0; j<fNBarHits; j++) fMeanTime[ fBarHits[j] ] -= toff;
  for(UInt_t j=0; j<fNhits; j++) fTime[ fHits[j] ] -= toff;
}

//---------------------------------------------------------------------------
inline void TA2LongScint::Cleanup( )
{
  // end-of-event cleanup
  TA2GenericDetector::Cleanup();
  for(Int_t i=0; i<fNBar; i++){
    fBarHitCount[i] = 0;
    fTheta[i] = fPhi[i] = fFlight[i] = ENullFloat;
    if( fXMC ) fXMC[i] = fYMC[i] = fZMC[i] = ENullFloat;
    if(fBar[i])fBar[i]->Cleanup();
  }
}

//---------------------------------------------------------------------------
inline void TA2LongScint::SmearMCHit( Int_t j, Double_t e, Double_t t,
				      Double_t x, Double_t y, Double_t z )
{
  //
  LongBar_t* bar = fBar[j];
  TVector3 mp = fBar[j]->GetMeanPos();
  Double_t x3 = mp.X();
  Double_t y3 = mp.Y();
  Double_t z3 = mp.Z();

  Double_t xx = bar->GetHalfLength();      // X dimension
  Double_t yy = 0.5*bar->GetYsize();       // Y dimension
  Double_t zz = 0.5*bar->GetZsize();       // Z dimension
  if( (x > x3+xx) || (x < x3-xx) ) return; 
  if( (y > y3+yy) || (y < y3-yy) ) return; 
  if( (z > z3+zz) || (z < z3-zz) ) return;
  TVector3 hp(x,y,z);

  // 1st end
  Int_t k = bar->GetEnd1();
  TVector3 hp1 = *fPosition[k] - hp;
  Double_t d = hp1.Mag();
  // Pulse height droop and resolution smearing
  Double_t ph = e * TMath::Exp(-d/bar->GetAttenLn());
  ph += fRandom->Gaus(0.0,TMath::Sqrt(ph/bar->GetNscint()));
  // Add scintillation time and resolution smearing
  Double_t t1 = t + d/bar->GetCeff() + fRandom->Gaus(0.0, bar->GetTRes());
  HitD2A_t* elem = fElement[k];
  // does it make the cuts
  if( (ph >= elem->GetEnergyLowThr()) && (ph <= elem->GetEnergyHighThr())
      && (t1 >= elem->GetTimeLowThr()) && (t1 <= elem->GetTimeHighThr()) ) {
    fEnergy[k] = ph;
    fTime[k] = t1;
    fSmearHits[fNSmearHits] = k;
    fNSmearHits++;
  }
  // doesn't make cuts....mark NULL
  else{
    fEnergy[k] = (Double_t)ENullHit;
    fTime[k] = (Double_t)ENullHit;
  }
  // 2nd End
  Int_t l = bar->GetEnd2();
  hp1 = *fPosition[l] - hp;
  d = hp1.Mag();
  // Pulse height droop and resolution smearing
  ph = e * TMath::Exp(-d/bar->GetAttenLn());
  ph += fRandom->Gaus(0.0,TMath::Sqrt(ph/bar->GetNscint()));
  // Add scintillation time and resolution smearing
  t1 = t + d/bar->GetCeff() + fRandom->Gaus(0.0, bar->GetTRes());
  elem = fElement[l];
  // does it make the cuts
  if( (ph >= elem->GetEnergyLowThr()) && (ph <= elem->GetEnergyHighThr())
      && (t1 >= elem->GetTimeLowThr()) && (t1 <= elem->GetTimeHighThr()) ) {
    fEnergy[l] = ph;
    fTime[l] = t1;
    fSmearHits[fNSmearHits] = l;
    fNSmearHits++;
  }
  // this one should already be NULL if cuts not made
}


#endif
