//--Author	JRM Annand   11th Sep 2010 Cut-down version of spill
//--Rev 	
//--Update      	
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TCAMAC_4413DAQEnable
// Implement programable pulse generator from  LRS 4413 discriminator
// Used to set/reset the trigger-inhibit latch

#ifndef __TCAMAC_4413DAQEnable_h__
#define __TCAMAC_4413DAQEnable_h__

#include "TCAMAC_4413SPILL.h"

class TCAMAC_4413DAQEnable : public TCAMAC_4413SPILL {
 protected:
 public:
  TCAMAC_4413DAQEnable( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TCAMAC_4413DAQEnable();
  // All TDAQmodules have the following procedures, by default dummies
  // Here they provide control of trigger-related hardware
  virtual void RunTrigCtrl();     // Enable triggers
  virtual void EndTrigCtrl();     // Disable triggers
  virtual void ResetTrigCtrl(){}  // Does nothing...on purpose...no TCS

  ClassDef(TCAMAC_4413DAQEnable,1)   

    };

#endif
