//--Author	JRM Annand   25th Jan 2006
//--Rev 	JRM Annand
//--Rev 	JRM Annand    8th Jan 2008 No write to hardware in InitRAM
//--Rev 	B.Oussena    26th Oct 2009 Bug fix reg offset InitRAM
//--Update	JRM Annand    5th Dec 2010 F9,F0's then RAM, no CNAF command 
//
//--Description
//
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TCAMAC_4508
// Dual,  8-bit input, 8-bit output Memory Lookup Unit (MLU)
// Latched input patterns may be read out via F0-A0,A1
// Setup of readout now hard wired.
// Setup of memory via "RAM:" commands, CAMAC "CNAF:" commands not recognised


#ifndef __TCAMAC_4508_h__
#define __TCAMAC_4508_h__

#include "TCAMACmodule.h"

enum { E4508_RAM0, E4508_RAM1 };
enum { E4508_MEM = 256, E4508_INIT = 4, E4508_RD = 2 };

class TCAMAC_4508 : public TCAMACmodule {
 protected:
  UShort_t* fInit[4];          // to reset everything
  UShort_t* fRam0;             // -> memory registers module 0
  UShort_t* fRam1;             // -> memory registers module 1
 public:
  TCAMAC_4508( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TCAMAC_4508();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module
  virtual void Init( );
  virtual void InitRAM( Char_t* );            // initialise and store register
  virtual UShort_t ConvAddr( UShort_t addr, UShort_t datum ){
    //	convert 4508 random access format W1-8 data, W9-16 address	
    return ((addr & 0xff)<<8) | (datum & 0xff); }

  ClassDef(TCAMAC_4508,1)   

    };

#endif
