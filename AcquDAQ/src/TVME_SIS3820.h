//--Author	JRM Annand   1st Sep 2012
//--Rev 	JRM Annand
//--Update	JRM Annand
//
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TVME_SIS3820
// I/O register and scaler
//

#ifndef __TVME_SIS3820_h__
#define __TVME_SIS3820_h__

enum{
  ESIS_CSR,		   // Control & status register
  ESIS_ModID,		   // Module & firmware ID
  ESIS_IRQConfig,	   // Interrupt configuration 
  ESIS_IRQ_CSR,		   // Iinterrupt control/status
  ESIS_DataDirect,         // Direct output data register
  ESIS_DataJK,             // J/K output data register
  ESIS_InDirect,           // Direct input register
  ESIS_InLatched,          // Latched input register
  ESIS_Counter,            // Counter 1-4, offset 0x30-3c
  ESIS_ClrCounter,         // Clear counters
  ESIS_Reset,              // Reset module
};


#include "TVMEmodule.h"

class TVME_SIS3820 : public TVMEmodule {
 protected:
 public:
  TVME_SIS3820( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TVME_SIS3820();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module
  virtual void PostInit( );
  virtual void SendEventID(UInt_t);
  virtual UInt_t GetEventID();
  virtual Bool_t CheckHardID();
  ClassDef(TVME_SIS3820,1)   

    };

//-----------------------------------------------------------------------------
inline void TVME_SIS3820::SendEventID(UInt_t ID)
{
  // Send Event ID to remote system
  //
  Write(ESIS_DataDirect, ID);
}

//-----------------------------------------------------------------------------
inline UInt_t TVME_SIS3820::GetEventID( )
{
  // Enable triggers via INT-4 input and ACK-2 output
  //
  UInt_t id = Read(ESIS_InLatched);
  return id;
}

#endif
