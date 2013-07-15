//--Author	JRM Annand     29th Jul 2010 Start
//--Rev
//--Update
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TFB_Phil10C
// Class to decode data read out from Phillips 10C QDCs and TDCs
// These should all employ the same data format
//

#ifndef __TFB_Phil10C_h__
#define __TFB_Phil10C_h__

#include "TFBmodule.h"

class TFB_Phil10C : public TFBmodule {
 protected:
 public:
  TFB_Phil10C( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TFB_Phil10C();
  virtual void Decode( UInt_t**, void**, Int_t*);
  ClassDef(TFB_Phil10C,1)   
    };

#endif
