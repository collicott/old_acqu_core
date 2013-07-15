//--Author	JRM Annand    5th Dec 2007 
//--Rev 	JRM Annand...
//--Rev 	JRM Annand... 3rd Jun 2008...const Char_t*...gcc 4.3
//--Update	JRM Annand    6th Jul 2011  gcc4.6-x86_64 warning fixes
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TGUIsupervise
// Network link GUI control
//

#ifndef __TGUIsupervise_h__
#define __TGUIsupervise_h__

#include "TDAQsupervise.h"

R__EXTERN class TA2Control*  gCTRL;  // global ptr to AcquRoot application
class TDAQguiCtrl;

class TGUIsupervise : public TDAQsupervise {
 protected:
 public:
  TGUIsupervise( Char_t*, TDAQexperiment*, Char_t* = NULL );
  virtual ~TGUIsupervise( );
  virtual void ExecRemote( const Char_t* );
  virtual Char_t* GetString( const Char_t* );
  virtual void PutString( Char_t* );
  virtual void ExecStart( ){ ExecRemote("Start:"); }
  virtual void ExecRun( ){ ExecRemote("Run:"); }
  virtual void ExecPause(  ){ ExecRemote("Pause:"); }
  virtual void ExecEnd(  ){ ExecRemote("End:"); }
  virtual void ExecKill(  ){ ExecRemote("Kill:"); }
  virtual void ExecBufferDump(){ ExecRemote("Dump:"); }
  Char_t* GetRunDesc(){ return GetString("Experiment:"); }
  Char_t* GetTime(){ return GetString("Time:"); }
  Char_t* GetComment(){ return GetString("Comment:"); }
  Char_t* GetFileName(){ return GetString("File:"); }
  Int_t GetRunNumber();
  void SetRunNumber(Int_t);
  virtual void SetAuto( Bool_t );
  virtual Bool_t IsRunInit();
  virtual Bool_t IsDataReady();
  //
  ClassDef(TGUIsupervise,1)   
    };


#endif
