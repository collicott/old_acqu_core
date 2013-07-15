//--Author	JRM Annand   25th Jan 2012
//--Rev 	JRM Annand
//--Update	JRM Annand
//
//--Description
//
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TCAMAC_GSI4800
// 48 channel, 24 bit CAMAC scaler from GSI

#ifndef __TCAMAC_GSI4800_h__
#define __TCAMAC_GSI4800_h__

#include "TCAMACmodule.h"

enum { EGSI4800_Reset, EGSI4800_Enable, 
       EGSI4800_RdLSB, EGSI4800_RdMSB, EGSI4800_Load};
enum { EGSI4800_INIT = 2, EGSI4800_RD = 2, EGSI4800_NChan = 48 };

class TCAMAC_GSI4800 : public TCAMACmodule {
 protected:
  UShort_t* fInit[4];          // to reset everything
  UShort_t* fRam0;             // -> memory registers module 0
  UShort_t* fRam1;             // -> memory registers module 1
 public:
  TCAMAC_GSI4800( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TCAMAC_GSI4800();
  virtual void SetConfig( Char_t*, Int_t );   // configure CAMAC module
  virtual void ReadIRQScaler(void**);
  virtual void PostInit( );

  ClassDef(TCAMAC_GSI4800,1)   

    };

#endif
