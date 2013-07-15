//--Author	JRM Annand   27th Jun 2004
//--Rev 	JRM Annand...
//--Update	JRM Annand...27th Mar 2007  Decode divide-by-zero check
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2RateMonitor
// Time evolution of scalers counts or ratios of scaler counts
// e.g. scaler/clock-scaler = rate
//

#ifndef __TA2RateMonitor_h__
#define __TA2RateMonitor_h__

#include "TA2System.h"

enum { ENRates, ERateData  };
// Command-line default detector key words which determine
// what information is read in
static const Map_t kRateDefaultKeys[] = {
  {"Number-Rates:",ENRates},
  {"RateData:",    ERateData},
  {NULL,          -1}
};

class TA2RateMonitor : public TA2System{
private:
  UShort_t* fADC;
  UInt_t* fScaler;
  Double_t* fScalerSum;
  Char_t** fRateName;
  Int_t fNRate;
  Int_t fNrate;
  Int_t* fScalerIndex;
  Int_t* fClockIndex;
  Int_t* fFrequency;
  Double_t* fNorm;
  Double_t* fRate;
  Bool_t* fIsScalerRead;
public:
  TA2RateMonitor( char*, UShort_t*, UInt_t*, Double_t*, Bool_t* );
  virtual ~TA2RateMonitor();
  virtual void SetConfig( char*, int );
  virtual void Decode();
  Char_t** GetRateName(){ return fRateName; }
  Char_t* GetRateName(Int_t i){ return fRateName[i]; }
  Int_t GetNRate(){ return fNRate; }
  Int_t* GetScalerIndex(){ return fScalerIndex; }
  Int_t GetScalerIndex(Int_t i){ return fScalerIndex[i]; }
  Int_t* GetClockIndex(){ return fClockIndex; }
  Int_t GetClockIndex(Int_t i){ return fClockIndex[i]; }
  Int_t* GetFrequency(){ return fFrequency; }
  Int_t GetFrequency(Int_t i){ return fFrequency[i]; }
  Double_t* GetNorm(){ return fNorm; }
  Double_t GetNorm(Int_t i){ return fNorm[i]; }
  Double_t* GetRate(){ return fRate; }
  Double_t GetRate(Int_t i){ return fRate[i]; }

  ClassDef(TA2RateMonitor,1)
};

//---------------------------------------------------------------------------
inline void TA2RateMonitor::Decode( )
{
  // Build set of patterns from pattern ADCs
  // Apply window cuts and return true/false to cut decision

  if( !(*fIsScalerRead) ) return;
  Int_t n,i,j;

  // loop round separate "rate" monitors
  for( n=0; n<fNRate; n++ ){
    i = fScalerIndex[n];
    j = fClockIndex[n];
    if( !fScaler[j] ) fRate[n] = 0;
    else fRate[n] = fNorm[n]*fScaler[i]/fScaler[j];
  }
}
#endif
