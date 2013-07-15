//--Author	JRM Annand    2nd Oct 2003
//--Rev 	JRM Annand...12th Dec 2003 Multi-hit ADC's
//--Rev 	JRM Annand...18th Feb 2004 D2AM_t for Multi-hit ADC's
//--Rev 	JRM Annand...27th May 2004 Time walk
//--Rev 	JRM Annand...21st Oct 2005 Getter for cuts, global energy scale
//--Rev 	JRM Annand....8th Sep 2006 SetWalk check mem leak
//--Rev 	JRM Annand...30th Oct 2008 Multihit TDCs
//--Rev 	JRM Annand...29th Nov 2008 Remove D2A_t and Cut_t
//--Rev 	JRM Annand... 3rd Dec 2008 Time walk options
//--Update	JRM Annand... 1st Sep 2009 constructer no incr nelem
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// HitD2A_t
// Digital-to-analogue conversion for a single detector element
// The element may have amplitude (ADC) and time (TDC) digitised
// readout. Window cuts are applied
//

#ifndef __THitD2A_h__
#define __THitD2A_h__

#include "EnumConst.h"
#include "TimeWalk_t.h"

class TA2Detector;

class HitD2A_t{
private:
  // ADC stuff
  Double_t fA0;                     // pedestal
  Double_t fA1;                     // conversion gain
  Double_t fA2;                     // quadratic term?
  UShort_t* fADC;                   // -> stored ADC value
  Bool_t fIsMultiADC;               // multi-hit or not
  Double_t fEnergyLowThr;           // valid energy window
  Double_t fEnergyHighThr;
  // TDC stuff
  Double_t fT0;                     // Time-zero offset
  Double_t fT1;                     // conversion gain
  Double_t fT2;                     // quadratic term?
  UShort_t* fTDC;                   // -> stored TDC value
  UShort_t** fTDCM;                 // -> stored multihit TDC value
  Bool_t fIsMultiTDC;               // multi-hit or not
  Double_t fTimeLowThr;             // valis time window
  Double_t fTimeHighThr;
  //
  TimeWalk_t* fWalk;            // time walk correction
  Double_t* fEnergy;            // energy MeV
  Double_t* fTime;              // time ns
  Double_t** fTimeM;            // multihit time ns
  Double_t fEnergyScale;        // global energy scale factor
  Int_t fMode;                  // ADC, TDC, ADC+TDC
  Int_t fNMultihit;             // analysis of multiple hits in TDC
  Int_t fNhit;                  // # TDC hits
  Int_t fIadc;                  // ADC index
  Int_t fItdc;                  // TDC index
public:
  HitD2A_t( char*, UInt_t, TA2Detector* );
  virtual ~HitD2A_t();
  Bool_t Calibrate();
  Bool_t CheckEnergy();
  Bool_t CheckTime();
  UShort_t* GetRawADC(){ return fADC; }
  UShort_t* GetRawTDC(){ return fTDC; }
  UShort_t GetRawADCValue(){ return *fADC; }
  UShort_t GetRawTDCValue(){ return *fTDC; }
  Double_t GetEnergy(){ return *fEnergy; }
  Double_t GetTime(){ return *fTime; }
  Double_t** GetTimeM(){ return fTimeM; }
  Double_t* GetTimeM(Int_t m){ return fTimeM[m]; }
  Double_t GetEnergyLowThr(){ return fEnergyLowThr; }
  Double_t GetEnergyHighThr(){ return fEnergyHighThr; }
  Double_t GetTimeLowThr(){ return fTimeLowThr; }
  Double_t GetTimeHighThr(){ return fTimeHighThr; }
  void SetWalk( char* );
  void SetWalk( Double_t, Double_t );
  Int_t GetMode(){ return fMode; }
  Int_t GetNMultihit(){ return fNMultihit; }
  Int_t GetNhit(){ return fNhit; }
  Int_t GetIadc(){ return fIadc; }
  Int_t GetItdc(){ return fItdc; }
};

//---------------------------------------------------------------------------
inline Bool_t HitD2A_t::Calibrate( )
{
  // Convert digital ADC & TDC to energy, time
  // Apply window cuts and return true/false to cut decision
  // Apply global energy scale factor (default 1.0)
  // Return logical result of testing within lower/upper limits

  //  Double_t energy;

  switch( fMode ){
  case 2:
    // TDC's only
    return CheckTime();
  case 3:
    // ADC and TDC
    if( CheckEnergy() )
      return CheckTime();
    else return EFalse;
  case 1:
    // ADC's only
    return CheckEnergy();
  default:
    return EFalse;
  }
}

//---------------------------------------------------------------------------
inline Bool_t HitD2A_t::CheckEnergy( )
{
  // Convert ADC channel to energy
  // Check 1st if ADC has valid channel contents
  // Check 2nd if energy is within bounds
  // Return TRUE if OK
  //
  Double_t energy,chan;
  if (!fIsMultiADC) {
    if( *fADC == (UShort_t)ENullADC ) return EFalse;   // SADC
    chan = (Double_t)(*fADC);
  }
  else {
    Short_t* madc = (Short_t*)fADC;
    if( *madc == (Short_t)ENullStore ) return EFalse; // standard QDC
    chan = (Double_t)(*madc);
  }
  chan -= fA0;                                   // pedestal subtract
  energy = fA1*chan;                             // linear scale
  if( fA2 ) energy += fA2*chan*chan;             // quadratic correction
  energy *= fEnergyScale;                        // global scale factor
  // check ADC thr.
  if( (energy < fEnergyLowThr) || (energy > fEnergyHighThr) ) return EFalse;
  *fEnergy = energy;
  return ETrue;
}

//---------------------------------------------------------------------------
inline Bool_t HitD2A_t::CheckTime( )
{
  // Convert TDC channel to time
  // Check 1st if TDC has valid channel contents
  // Check 2nd if time is within bounds
  // Return TRUE if OK
  // This one will analyse multiple hits from a TDC if fNMulihit > 0
  fNhit = 0;
  Double_t chan, time;
  // Single hit TDC (or analyse only 1 hit from multihit TDC)
  if( !fNMultihit ){
    if( !fIsMultiTDC ){
      if( *fTDC ==  (UShort_t)ENullADC ) return EFalse; // channel not defined
      chan = (Double_t)(*fTDC);
    }
    else{
      Short_t* madc = (Short_t*)fTDC;
      if( *madc == (Short_t)ENullStore ) return EFalse;// channel not defined
      chan = (Double_t)(*madc);
    }
    chan -= fT0;                             // subtract offset
    time = fT1*chan;                         // linear scale
    if( fT2 ) time += fT2*chan*chan;         // quadratic correction
    if( fWalk ) time = fWalk->Tcorr( *fEnergy, time ); // walk correction
    if( (time < fTimeLowThr) || (time > fTimeHighThr) ){
      *fTime = (Double_t)ENullADC;          // not inside window
      return EFalse;
    }
    else{
      *fTime = time;
      return kTRUE;
    }
  }
  // Multi-hit TDC  
  for( Int_t m=0; m<fNMultihit; m++ ){
    Short_t* madc = (Short_t*)fTDCM[m];
    if( *madc == (Short_t)ENullStore ) break;  // channel not defined
    chan = (Double_t)(*madc);
    chan -= fT0;                               // subtract offset
    time = fT1*chan;                           // linear scale
    if( fT2 ) time += fT2*chan*chan;           // quadratic correction
    if( fWalk ) time = fWalk->Tcorr( *fEnergy, time ); // walk correction
    if( (time < fTimeLowThr) || (time > fTimeHighThr) )
      *fTimeM[m] = (Double_t)ENullADC;
    else{
      if( !fNhit ) *fTime = time;             // time from 1st hit
      *fTimeM[m] = time;                      // store all hit times
      fNhit++;                                // log # hits
    }
  }
  if( fNhit )
    return kTRUE;
  else{
    *fTime = (Double_t)ENullADC;
    return kFALSE;
  }  
}

#endif
