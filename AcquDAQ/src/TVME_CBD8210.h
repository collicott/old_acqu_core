//--Author	JRM Annand	21st Dec 2005
//--Rev 	JRM Annand
//--Rev 	JRM Annand..28th Apr 2009..remove TDAQmemmap.h
//--Rev 	B. Oussena       May 2009..fix a bug in PostInit()
//--Update	JRM Annand  17th May 2010..It should be A24 addressing
//
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TVME_CBD8210
// CES CBD 8210 CAMAC Parallel Branch Driver
//

#ifndef __TVME_CBD8210_h__
#define __TVME_CBD8210_h__

#include "TVMEmodule.h"

class TVME_CBD8210 : public TVMEmodule {
 protected:
  UShort_t* fCSR;       // control/status register
  UShort_t* fIFR;       // interrupt flag register
  Bool_t    fIsIRQEnabled;
 public:
  TVME_CBD8210( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TVME_CBD8210();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module
  virtual void PostInit( );
  virtual UShort_t* AddrS( UInt_t, UInt_t, UInt_t, UInt_t );
  virtual UInt_t* AddrL( UInt_t, UInt_t, UInt_t, UInt_t );
  virtual void WaitIRQ();
  virtual void ResetIRQ();
  virtual void EnableIRQ();
  virtual void DisableIRQ(){ fIsIRQEnabled = kFALSE; }
  //  virtual void Read(UInt_t, void*, Int_t= fAM, Int_t= fDW);
  //  virtual void Write(UInt_t, void*, Int_t= fAM, Int_t= fDW);
  //
  //  Int_t GetAM(){ return fAM; }
  //  Int_t GetDW(){ return fDW; }
  UShort_t* GetCSR(){ return fCSR; }
  UShort_t* GetIFR(){ return fIFR; }
  Bool_t IsIRQEnabled(){ return fIsIRQEnabled; }

  ClassDef(TVME_CBD8210,1)   

    };

//-----------------------------------------------------------------------------
inline UShort_t* TVME_CBD8210::AddrS( UInt_t c, UInt_t n, UInt_t a,
				      UInt_t f )
{
  // For a given CAMAC CNAF Calculate the equivalent VMEbus address
  // 16-bit data transfer assumed at that address
  return  (UShort_t*)((Char_t*)fBaseAddr + ((c & 0xf)<<16) + ((n & 0xff)<<11) +
		      ((a & 0xff)<<7) + ((f & 0xff)<<2) + 2 );
}

//-----------------------------------------------------------------------------
inline UInt_t* TVME_CBD8210::AddrL( UInt_t c, UInt_t n, UInt_t a,
				    UInt_t f )
{
  // For a given CAMAC CNAF Calculate the equivalent VMEbus address
  // 24-bit data transfer assumed at that address
  return  (UInt_t*)((Char_t*)fBaseAddr + ((c & 0xf)<<16) + ((n & 0xff)<<11) +
		    ((a & 0xff)<<7) + ((f & 0xff)<<2) );
}

//-----------------------------------------------------------------------------
inline void TVME_CBD8210::WaitIRQ( )
{
  // Poll the state of the IRQ-4 flag, bit 1 of the CSR register
  // Set IRQ ACK-2 NIM output if IRQ-4 received (acquisition busy)
  Int_t csr;
  Int_t ifr = 0x2;
  for(;;){
    if( fIsIRQEnabled ){       // "interrupt" enabled?
      Read(fCSR,&csr);         // poll CSR register
      if( csr & 0x1 )break;    // exit when Int-4 sensed
    }
  }
  Write(fIFR, &ifr);       // set IRQ ACK-2 flag (NIM output)
  csr &= ~0x4;             // enable IRQ bits in CSR
  Write(fCSR, &csr);

}

//-----------------------------------------------------------------------------
inline void TVME_CBD8210::ResetIRQ( )
{
  // Reset IRQ ACK-2 (busy) flag;
  //
  Int_t ifr = 0;
  Write(fIFR, &ifr);
}

//-----------------------------------------------------------------------------
inline void TVME_CBD8210::EnableIRQ( )
{
  // Enable triggers via INT-4 input and ACK-2 output
  //
  fIsIRQEnabled = kTRUE;
  Int_t csr;
  Read(fCSR,&csr);
  csr &= ~0x4;
  Int_t ifr = 0x2;
  Write(fIFR, &ifr);             // set Ack2
  Write(fCSR, &csr);             // clear disable bits
  ifr = 0;
  Write(fIFR, &ifr);             // set Ack2
}

#endif
