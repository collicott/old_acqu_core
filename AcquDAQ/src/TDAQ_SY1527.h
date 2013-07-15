//--Author	JRM Annand   18th May 2008
//--Rev 	JRM Annand...
//--Rev 	JRM Annand... 3rd Jun 2008  Conditional compilation
//--Update	JRM Annand... 6th Jul 2011  gcc4.6-x86_64 warning fix
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TDAQ_SY1527
// CAEN SY1527 HV Crate
// Assumes ethernet connection
//

#ifndef __TDAQ_SY1527_h__
#define __TDAQ_SY1527_h__

#include "TDAQmodule.h"
//
#ifdef AD_CAENHV
// CAEN library of control procedures for the SY1527 HV
#define UNIX                   // so the CAEN stuff knows its not windows
extern "C" {
#include "CAENHVWrapper.h"
}
#endif
#include "HV_1932_t.h"

enum{ ENSlot = 16 };

class TDAQ_SY1527 : public TDAQmodule {
 private:
  Char_t* fUser;
  Char_t* fPasswd;
 protected:
  Char_t* fNm;
  Int_t fLink;
  Char_t* fIP;
  Char_t* fSoftwareRelease;
  UShort_t fNSlot;
  UShort_t* fSerial;
  UShort_t* fNChannel;
  Char_t* fModelList;
  Char_t* fDescList;
  Char_t* fInfo;
  UChar_t* fRelMin;
  UChar_t* fRelMax;;
  HV_1932_t* fHV_1932;
  Char_t* fCommandLine;
  Bool_t* fIsSlotOccupied;
  Bool_t* fIsSlotConfigured;
 public:
  TDAQ_SY1527( Char_t*, Char_t*, FILE* );
  virtual ~TDAQ_SY1527();
  void SetConfig( Char_t*, Int_t );         // configure v2718
  virtual void PostInit();
  virtual void Connect();
  virtual void Disconnect();
  virtual void SaveValues();
  virtual void TurnOnOff( Bool_t );
  virtual void TurnOn(){ TurnOnOff(kTRUE); }
  virtual void TurnOff(){ TurnOnOff(kFALSE); }
  virtual void CommandLoop();
  virtual Int_t Read(UShort_t, Char_t*, void*, UShort_t, UShort_t = 1);
  virtual Int_t Write(UShort_t, Char_t*, void*, UShort_t, UShort_t = 1);
  virtual Int_t ReadNm(UShort_t, Char_t*, UShort_t, UShort_t = 1);
  virtual Int_t WriteNm(UShort_t, Char_t*, UShort_t, UShort_t = 1);
  Int_t GetLink(){ return fLink; }
  Bool_t IsSlotOccupied(Int_t i){ return fIsSlotOccupied[i]; }
  Bool_t IsSlotConfigured(Int_t i){ return fIsSlotConfigured[i]; }

  //
  ClassDef(TDAQ_SY1527,1)   
    };

#endif
