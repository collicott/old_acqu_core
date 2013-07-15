//--Author	JRM Annand   9th March 2003
//--Update	JRM Annand
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
// FlashADC
// Flash ADC info handler...
// Store sample for each event, and also accumulate the samples.
// Calculate and store pedestal, pedestal-subtracted
// sum and pedestal subtracted tail (for pileup effects etc.)
// All functions inlined

#ifndef __FlashADC_h__
#define __FlashADC_h__

#include "MultiADC_t.h"                   // Inherits from multi-value ADC

class FlashADC_t : public MultiADC_t
{
 protected:
  Double_t* fAccum;                       // accumulated adc values
  Double_t fSum;                          // Signal pedestal-subtracted sum
  Double_t fPed;                          // Pedestal sum
  Double_t fTail;                         // Tail sum
  Double_t fSumFactor;                    // ratio total to pedestal channels
  Double_t fTailFactor;                   // ratio tail to pedestal channels
  UInt_t fPedLow;                         // lower limit pedestal window
  UInt_t fPedHigh;                        // upper limit pedestal window
  UInt_t fSumLow;                      // lower limit signal window
  UInt_t fSumHigh;                     // upper limit signal window
  UInt_t fTailLow;                        // lower limit tail window
  UInt_t fTailHigh;                       // upper limit tail window
public:
  FlashADC_t( Int_t i, Int_t ch, Int_t pl, Int_t ph, Int_t sl, Int_t sh,
	      Int_t tl = 0, Int_t th = 0 ) :
    MultiADC_t( i, ch ) {
    // store input parameters...MultiADC_t stores ADC index, # channels
    // and creates the fStore array
    fPedLow = pl;
    fPedHigh = ph;
    fSumLow = sl;
    fSumHigh = sh;
    fTailLow = tl;
    fTailHigh = th;
    fAccum = new Double_t[ch];
    fSumFactor = (fSumHigh - fSumLow)/(fPedHigh - fPedLow);
    fTailFactor = (fTailHigh - fTailLow)/(fPedHigh - fPedLow);
  }
  virtual ~FlashADC_t(){
    // delete storage arrays...previously "newed" at construct
    if(fStore) delete fStore;
    if(fAccum) delete fAccum;
  }
  virtual void Fill( AcquBlock_t* d ){
    // Fill the flash-ADC array until it is completely filled
    // or until the ADC index on the input stream doesn't match fIadc.
    // The flash-ADC values are assumed to come consecutively.
    // Compute the signal sum of the samples, the pedestal sum
    // and the tail sum. 
    fNstore = 0;
    fSum = fPed = fTail = 0;
    for( UInt_t i=0; i<fChan; i++){
      if( d->id == fIadc ){
	fStore[i] = d->adc;
	fAccum[i] += d->adc;
	if( (i >= fPedLow) && (i <= fPedHigh) ) fPed += d->adc;
	if( (i >= fSumLow) && (i <= fSumHigh) ) fSum += d->adc;
	if( (i <= fTailHigh) && (i >= fTailLow) ) fTail += d->adc;
	fNstore++;
	d++;
      }
      else fStore[i] = 0;
    }
    // Assume signal falls below pedestal
    fSum = (fPed * fSumFactor) - fSum;
    fTail = (fPed * fTailFactor) - fTail;
  }
  // Getters to access read-only private/protected variables
  Double_t GetSum(){ return fSum; }
  Double_t GetPed(){ return fPed; }
  Double_t GetTail(){ return fTail; }
  Double_t* GetSumPtr(){ return &fSum; }     // address required for histograms
  Double_t* GetPedPtr(){ return &fPed; }
  Double_t* GetTailPtr(){ return &fTail; }
  Double_t* GetAccum(){ return fAccum; }     // to display the shape
};

#endif
