//--Author	JRM Annand   20th Dec 2005
//--Rev 	JRM Annand...
//--Rev 	JRM Annand... 5th Dec 2007 Int_t fHandle for libCAENVME
//--Rev 	JRM Annand... 3rd Jun 2008  Conditional compilation
//--Update	JRM Annand... 6th Jul 2011  gcc4.6-x86_64 warning fix
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TDAQ_V2718
// CAEN A2818 PCI <--> V2718 VMEbus controller via optical fibre
// Assumes that a2818 linux driver is loaded
// Uses control functions from libCAENVME.so

#ifndef __TDAQ_V2718_h__
#define __TDAQ_V2718_h__

#include "TDAQmodule.h"
//
#ifdef AD_CAENVME
// CAEN library of control procedures for the A2818/V2718 VMEbus controller
#define LINUX                   // so the CAEN stuff knows its not windows
extern "C" {
#include "CAENVMElib.h"
}
#endif

class TDAQ_V2718 : public TDAQmodule {
 protected:
  Int_t fHandle;
  //  CVBoardTypes fBoard;
  Int_t fBoard;
  Short_t fLink;
  Short_t fBdNum;
  Short_t fAM;
 public:
  TDAQ_V2718( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TDAQ_V2718();
  void SetConfig( Char_t*, Int_t );         // configure v2718
  virtual void PostInit();
  //  virtual void Read(void*, void*, Int_t= cvA24_U_DATA, Int_t= cvD16_swapped);
  //  virtual void Write(void*, void*, Int_t= cvA24_U_DATA, Int_t= cvD16_swapped);
  virtual void Read(void*, void*, Int_t, Int_t);
  virtual void Write(void*, void*, Int_t, Int_t);
  //  virtual void Read(void*, void*, Int_t);
  //  virtual void Write(void*, void*, Int_t);
  virtual void Read(void*, void*);
  virtual void Write(void*, void*);
  virtual void BlkRead();
  virtual void BlkWrite();
  //
  Int_t GetHandle(){ return fHandle; }
  Short_t GetLink(){ return fLink; }
  Short_t GetBdNum(){ return fBdNum; }
  Short_t GetAM(){ return fAM; }
  void SetAM( Int_t i ){ fAM = (UShort_t)i; }

  //
  ClassDef(TDAQ_V2718,1)   
    };

//-----------------------------------------------------------------------------
inline void TDAQ_V2718::Read( void* addr, void* data, Int_t am, Int_t dw )
{
  //
#ifdef AD_CAENVME
  fErrorCode = CAENVME_ReadCycle( fHandle, (UInt_t)addr, data,
				  (CVAddressModifier)am, (CVDataWidth)dw );
#endif
}

//-----------------------------------------------------------------------------
inline void TDAQ_V2718::Write( void* addr, void* data, Int_t am, Int_t dw )
{
  //
#ifdef AD_CAENVME
  fErrorCode = CAENVME_WriteCycle(fHandle, (UInt_t)addr, data,
				  (CVAddressModifier)am, (CVDataWidth)dw );
#endif
}

//-----------------------------------------------------------------------------
inline void TDAQ_V2718::Read( void* addr, void* data )
{
  // Read with default AM code and data size
#ifdef AD_CAENVME
  fErrorCode = CAENVME_ReadCycle( fHandle, (UInt_t)addr, data,
				  (CVAddressModifier)fAM, (CVDataWidth)*fDW );
#endif
}

//-----------------------------------------------------------------------------
inline void TDAQ_V2718::Write( void* addr, void* data )
{
  // Write at default AM code and data size
  //
#ifdef AD_CAENVME
  fErrorCode = CAENVME_WriteCycle(fHandle, (UInt_t)addr, data,
				  (CVAddressModifier)fAM, (CVDataWidth)*fDW );
#endif
}



#endif
