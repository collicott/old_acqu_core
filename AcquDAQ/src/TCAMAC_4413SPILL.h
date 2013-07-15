//--Author	Baya Oussena 13th Jul 2010
//--Rev 	Baya Oussena
//--Rev         JRM Annand   20th Jul 2010 Implement Run/End/Reset/Enab/Disab	
//--Rev         JRM Annand    6th Sep 2010 TrigSet & TrigExe after mapping	
//--Rev         JRM Annand    8th Sep 2010 TCS reset, pause between stop-start	
//--Update      JRM Annand   10th Sep 2010 Add fEXP->PostReset()...temp measure	
//                                         Change fReg order F26 1st	
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TCAMAC_4413SPILL
// Implement programable pulse generator from LRS 4413 discriminator
// Used to set/reset the trigger-inhibit latch and start/stop the CERN optical
// trigger control system

#ifndef __TCAMAC_4413SPILL_h__
#define __TCAMAC_4413SPILL_h__

#include "TCAMACmodule.h"

class TCAMAC_4413SPILL : public TCAMACmodule {
 protected:
  UShort_t* fTrigSet;   // F16 A0 mask register
  UShort_t* fTrigExe;   // F25 A0 generate test pulse
  Int_t fTCSresetDelay; // Time to allow TCS to reset
 public:
  TCAMAC_4413SPILL( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TCAMAC_4413SPILL();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module
  virtual void PostInit();
  // All TDAQmodules have the following procedures, by default dummies
  // Here they provide control of trigger-related hardware
  virtual void RunTrigCtrl();     // Issue start to TCS and enable triggers
  virtual void EndTrigCtrl();     // Disable triggers and Issue stop to TCS
  virtual void ResetTrigCtrl();   // Stop/start the TCS, disable/enable triggers
  virtual void EnableTrigCtrl();  // Enable triggers
  virtual void DisableTrigCtrl(); // Disable triggers

  ClassDef(TCAMAC_4413SPILL,1)   

    };

#endif
