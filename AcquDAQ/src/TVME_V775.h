//--Author	JRM Annand	11th Jan 2008
//--Rev 	JRM Annand
//--Update	JRM Annand..28th Apr 2009..remove TDAQmemmap.h
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TVME_V775
// CAEN VMEbus 32-channel TDC (analogue TAC front end)
//

#ifndef __TVME_V775_h__
#define __TVME_V775_h__

#include "TVME_V792.h"

class TVME_V775 : public TVME_V792 {
 protected:
 public:
  TVME_V775( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TVME_V775();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module

  ClassDef(TVME_V775,1)   

    };

#endif

