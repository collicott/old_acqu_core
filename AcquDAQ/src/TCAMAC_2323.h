//--Author	JRM Annand    10th Feb 2007
//--Rev 	JRM Annand
//--Update	Baya Oussena  24th Nov 2011 left shift instead of right InitDelay
//
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TCAMAC_2323
// Dual,  programable gate and delay generator
// F9A0-1 Stop channels         F25A0-1 Start channels
// F1A0-1 Read delay registers  F17A0-1 Write delay registers

#ifndef __TCAMAC_2323_h__
#define __TCAMAC_2323_h__

#include "TCAMACmodule.h"

class TCAMAC_2323 : public TCAMACmodule {
 protected:
 public:
  TCAMAC_2323( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TCAMAC_2323();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module
  virtual void InitDelay( Char_t* );          // initialise delay registers

  ClassDef(TCAMAC_2323,1)   

    };

#endif
