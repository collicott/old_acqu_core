//--Author	Baya Oussena     15th Mar 2010
//--Rev 	Baya Oussena
//--Update      JRM Annand       29th Jul 2010 Remove extraneous stuff
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TFB_STR200
// Struck Fastbus 32-channel, 32-bit scaler
//

#ifndef __TFB_STR200_h__
#define __TFB_STR200_h__

#include "TFBmodule.h"

class TFB_STR200 : public TFBmodule {
 protected:
 public:
  TFB_STR200( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TFB_STR200();
  ClassDef(TFB_STR200,1)   

    };

#endif
