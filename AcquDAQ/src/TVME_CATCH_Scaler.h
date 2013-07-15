//--Author	JRM Annand   7th Jun 2007
//--Rev 	JRM Annand
//--Rev 	JRM Annand..  28th Apr 2009..remove TDAQmemmap.h
//--Rev 	JRM Annand..  25th Oct 2009..further development
//--Rev         B. Oussena    24 Jul 2010 have changed PostInit()
//--Rev         B. Oussena  13th Aug 2010  more cleaning 
//--Update      JRM Annand   8th Sep 2010  ReadIRQScaler, TCS eventID moved
//
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TVME_CATCH_Scaler
// COMPASS CATCH hardware
//

#ifndef __TVME_CATCH_Scaler_h__
#define __TVME_CATCH_Scaler_h__

#include "TVME_CATCH.h"

class TVME_CATCH_Scaler : public TVME_CATCH {
 protected:
 public:
  TVME_CATCH_Scaler( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TVME_CATCH_Scaler();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module
  virtual void PostInit( );
  virtual void ReadIRQScaler( void** );
  ClassDef(TVME_CATCH_Scaler,1)   

    };

#endif
