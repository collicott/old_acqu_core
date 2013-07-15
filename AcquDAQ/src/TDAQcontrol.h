//--Author	JRM Annand   30th Sep 2005
//--Rev 	JRM Annand...
//--Update	JRM Annand...29th Apr 2009  Memory map listing
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TDAQcontrol
// Characteristics of a control module
// Adds on to standard TDAQmodule
//

#ifndef __TDAQcontrol_h__
#define __TDAQcontrol_h__

//#include "TDAQmodule.h"

#include "TList.h"
#include "DAQMemMap_t.h"

class TDAQmodule;

class TDAQcontrol {
 protected:
  TDAQmodule* fMod;                  // -> standard module part of controller
  TList* fSlave;                     // list of slave modules
  TList* fSlaveADC;                  // list of slave ADC modules
  TList* fMapList;
  void** fOutBuff;                   // output buffer
  void* fSlaveBase;                  // base address offset for slaves
  Int_t fNSlave;                     // total # slaves
  Int_t fNSlaveADC;                  // total # slave ADCs
  Int_t fNmap;                       // total # virtual memory mappings
 public:
  TDAQcontrol( TDAQmodule* );
  virtual ~TDAQcontrol( );
  virtual void AddSlave( TDAQmodule* );
  TDAQmodule* GetSlave( Char_t* );
  virtual void SenseIRQ(){}
  virtual void ResetIRQ(){}
  virtual void* SlaveADCRead( void** );
  virtual Int_t AddMap(DAQMemMap_t*);
  //
  void* GetSlaveBase(){ return fSlaveBase; }
  Int_t GetNSlave(){ return fNSlave; }
  Int_t GetNSlaveADC(){ return fNSlaveADC; }
  Int_t GetNmap(){ return fNmap; }
  //
  ClassDef(TDAQcontrol,1)   
    };

#endif
