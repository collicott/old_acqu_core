//--Author	JRM Annand	12th Jan 2004   Convert Acqu++ -> AcquRoot
//--Rev 	JRM Annand
//--Rev 	JRM Annand      30th Apr 2003   InputList
//--Update	JRM Annand      22nd Jan 2007   4v0 update
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2NetSource
// Data comes from TCP/IP stream socket...normally online data acquisition
//
//---------------------------------------------------------------------------

#ifndef _TA2NetSource_h_
#define _TA2NetSource_h_

#include "TA2DataSource.h"

class ARSocket_t;

class TA2NetSource : public TA2DataSource
{					// TCP/IP network transfer
  ARSocket_t* fSocket;			// for TCP/IP communications
  Int_t fPacketLen;                     // network transfer packet length
public:
  TA2NetSource(Char_t*, Char_t*, Int_t, Int_t=EFalse, Int_t=1, Int_t=EFalse);
  ~TA2NetSource();
  virtual void Initialise();
  virtual void ReadCheck();		// read data buffer from remote node
  virtual void HandShakeSk(int);	// handshake buffer read
  virtual void Setup(){}
  virtual void Process();
  virtual void Shutdown();
  virtual void InputList( char*, UInt_t, UInt_t);

  ARSocket_t* GetSocket(){ return fSocket; }
  
  ClassDef(TA2NetSource,1)      	// for ROOT
};

#endif
