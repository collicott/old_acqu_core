//--Author	JRM Annand   17th May 2007  Test pseudo data
//--Rev 	JRM Annand
//--Rev 	JRM Annand   28th Nov 2007  WaitIRQ sleep(1) if DAQ not enabled
//--Rev 	JRM Annand   16th May 2011  Data > fMaxRand, set to 0
//--Update	JRM Annand   26th Nov 2012  Virtual event ID
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TVirtualModule
// Generate randomly generated data to test data streams etc.
// OR read data from non-ACQU data bases into data stream
//

#ifndef __TVirtualModule_h__
#define __TVirtualModule_h__

#include "TDAQmodule.h"
#include "TRandom.h"

class TVirtualModule : public TDAQmodule {
 protected:
  Char_t* fRandFn;
  TRandom* fRandom;            // random number generator
  Double_t fChan;
  UInt_t fMinRand;
  UInt_t fMaxRand;
  UInt_t fEventID;
  Bool_t    fIsIRQEnabled;
 public:
  TVirtualModule( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TVirtualModule();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module
  virtual void PostInit( );
  virtual void Read(void*, void*, Int_t, Int_t);
  virtual void Write(void*, void*, Int_t, Int_t);
  virtual void ReadIRQ( void** );
  virtual void WaitIRQ();
  virtual void ResetIRQ();
  //
  virtual void EnableIRQ(){ fIsIRQEnabled = kTRUE; }
  virtual void DisableIRQ(){ fIsIRQEnabled = kFALSE; }
  virtual UInt_t GetEventID(){
    UInt_t ev = fEventID++;
    return ev; 
  }
  Bool_t IsIRQEnabled(){ return fIsIRQEnabled; }

  ClassDef(TVirtualModule,1)   

    };

//-----------------------------------------------------------------------------
inline void TVirtualModule::Read( void* addr, void* data, Int_t am, Int_t dw )
{
  // Controller read
  fCtrlMod->Read( addr, data, am, dw );
}

//-----------------------------------------------------------------------------
inline void TVirtualModule::Write( void* addr, void* data, Int_t am, Int_t dw )
{
  // Controller write
  fCtrlMod->Write( addr, data, am, dw );
}

#endif
