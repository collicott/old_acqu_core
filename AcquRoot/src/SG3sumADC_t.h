//--Author	JRM Annand   9th March 2003
//--Update	JRM Annand  24th Jan 2004 Exclude multiple entries during event
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
// SG3sumADC_t
// 3-Sum SG ADC info handler. All functions inlined in this header file

#ifndef __SG3sumADC_h__
#define __SG3sumADC_h__

#include "EnumConst.h"                    // general constants
#include "DataFormats.h"                  // Acqu data formats

class SG3sumADC_t : public MultiADC_t
{
 protected:
  Double_t fSumFactor;
  Double_t fTailFactor;
public:
  SG3sumADC_t( Int_t index, Int_t pchan, Int_t schan, Int_t tchan ):
    MultiADC_t(index, 3){
    // store ADC index and set max # of hist (chan) to 3
    // which defaults to 3, create hit storage array
    fSumFactor = (Double_t)schan/pchan;
    fTailFactor = (Double_t)tchan/pchan;
  }
  virtual ~SG3sumADC_t(){
    // delete storage arrays...previously "newed" at construct
    if(fStore) delete fStore;
  }
  virtual void Fill( AcquBlock_t* d ){
    // Fill the SG 3-Sum array until it is completely filled
    // or until the ADC index on the input stream doesn't match fIadc.
    // The multi-ADC values are assumed to come consecutively.
    // If there are fewer than fChan values on the input stream
    // zero the remainder of the storage array
    MultiADC_t::Fill(d);
    // Assume signal falls below pedestal, store results as 
    //    fStore[1] = (UShort_t)(((Double_t)fStore[0] * fSumFactor) -
    //			   (Double_t)fStore[1]);
    //    fStore[2] = (UShort_t)(((Double_t)fStore[0] * fTailFactor) -
    //			   (Double_t)fStore[2]);
    fStore[1] = (Short_t)( (Double_t)fStore[1] -
			   ((Double_t)fStore[0] * fSumFactor) );
    fStore[2] = (Short_t)( (Double_t)fStore[2] -
			   ((Double_t)fStore[0] * fTailFactor) );
  }
  // 
};

#endif
