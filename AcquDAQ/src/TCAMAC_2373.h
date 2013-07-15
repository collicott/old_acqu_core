//--Author	JRM Annand	25th Jan 2006
//--Rev 	JRM Annand
//--Update	JRM Annand 
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TCAMAC_2373
// Dual,  8-bit input, 8-bit output Memory Lookup Unit (MLU)
// Latched input patterns may be read out

#ifndef __TCAMAC_2373_h__
#define __TCAMAC_2373_h__

#include "TCAMACmodule.h"

enum { E2373_RAM0, E2373_RAM1, E2373_RD0, E2373_RD1 };
enum {E2373_MEM=65536 };

class TCAMAC_2373 : public TCAMACmodule {
 protected:
  UShort_t* fCCR;
  UShort_t* fAddr;
  UShort_t* fData;
  UShort_t fCCRDatum;
  UShort_t addr,datum; // <<-- BAYA

 public:
  TCAMAC_2373( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TCAMAC_2373();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module
  virtual void PostInit( );                   // 2nd phase initialisation
  virtual void InitRAM(Char_t*);            // initialise and store register 

  virtual void InitMEM2373();   //<<-------- BAYA

  ClassDef(TCAMAC_2373,1)   

    };

#endif
