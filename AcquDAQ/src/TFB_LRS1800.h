//--Author	B. Oussena     5th Feb 2010  Start updates for KpHBoard
//--Rev         B. Oussena     10th Mar 2010 futher development
//--Rev 	JRM Annand     27th Jul 2010 Generalise 1800 module class
//--Update	JRM Annand      9th Sep 2010 Update Decode()
//
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TFB_LRS1800
// Class to decode data read out from LeCroy 1800 series of QDCs and TDCs.
// These should all employ the same data format
//

#ifndef __TFB_LRS1800_h__
#define __TFB_LRS1800_h__

#include "TFBmodule.h"

class TFB_LRS1800 : public TFBmodule {
 protected:
 public:
  TFB_LRS1800( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TFB_LRS1800();
  virtual void Decode( UInt_t**, void**, Int_t*);
  ClassDef(TFB_LRS1800,1)   
    };

#endif
