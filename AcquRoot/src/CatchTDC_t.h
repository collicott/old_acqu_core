//--Author	JRM Annand   9th March 2003
//--Rev 	JRM Annand
//--Update      D. Krambrich 5th April 2004 TDC overflow handler
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
// CatchTDC
// Catch multi-hit TDC info handler. All functions inlined in this header file

#ifndef __CatchTDC_h__
#define __CatchTDC_h__

#include "MultiADC_t.h"

enum { ETDCOverflow = 62054 };

class CatchTDC_t : public MultiADC_t
{
 protected:
  UShort_t* fRef;                         // -> to storage area
public:
  CatchTDC_t( Int_t index, Int_t chan, UShort_t* refaddr ):
    MultiADC_t(index, chan){
    fRef = refaddr;
  }
  virtual ~CatchTDC_t(){
    // delete storage arrays...previously "newed" at construct
    if(fStore) delete fStore;
  }
  virtual void Fill( AcquBlock_t* d ){
    // Fill the multi-TDC array until its full,
    // or until the ADC index on the input stream doesn't match fIadc.
    // The multi-ADC values are assumed to come consecutively.
    // If there are fewer than fChan values on the input stream
    // zero the remainder of the storage array
    // A simple algorithm is added to find the correct time difference
    // taking account of possible overflows in either the reference
    // or current channel (if neither or both overflow there is no problem)

    Int_t diff, diff_p, diff_m, min;  // channel - ref difference
    fNtry = 0;
    while( d->id == fIadc ){
      if( fNstore < fChan ) {
	diff   = d->adc - *fRef;
	diff_p = diff + ETDCOverflow;    // + overflow
	diff_m = diff - ETDCOverflow;    // - overflow
	min = (abs(diff) < abs(diff_p)) ? diff : diff_p;
	min = (abs(min)  < abs(diff_m)) ? min  : diff_m;
	fStore[fNstore] = Short_t( min );
	fNstore++;
      }
      fNtry++;
      d++;
    }
  }
};

#endif
