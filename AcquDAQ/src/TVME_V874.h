//--Author	JRM Annand   9th Jan 2013  Prototype version
//--Rev 	
//--Update  
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TVME_V874B
// CAEN VMEbus 4-Channel TAPS readout module
// Output channels
//  8 LGS1   12 LGS2   16 TDC1   20 LGS3   24 LGS4   28 BP1
//  9 SGS1   13 SGS2   17 TDC2   21 SGS3   25 SGS4   29 BP2
// 10 LG1    14 LG2    18 TDC3   22 LG3    26 LG4
// 11 SG1    15 SG2    19 TDC4   23 SG3    27 SG4
// Hardware channels 0-7,30,31 are not used
// AcquDAQ offsets channel numbers by 8 so that LGS1 is stored as 0 + base index
// ie the module is assumed to have 22 output channels
//
// Coding of this class based on CAEN Technical Information Manual, Mod. V874B
// Rev. n.0, 15 Nov. 2004

#ifndef __TVME_V874_h__
#define __TVME_V874_h__

#include "TVMEmodule.h"

// Sizes of register arrays
enum {
  EV874_NOutBuff = 512,
  EV874_NThresh = 32,
  EV874_NAux = 128
};

// Address indices for main-board registers
enum {
  EV874_IFirmware,                                 // Firmware version
  EV874_IBitSet1,                                  // Bit set register 1
  EV874_IBitClr1,                                  // Bit clear register 1
  EV874_IStatus1,                                  // Status register 1
  EV874_IControl1,                                 // Control register 1
  EV874_IStatus2,                                  // Status register 2
  EV874_IEvCntLSB,                                 // Event counter LSB
  EV874_IEvCntMSB,                                 // Event counter MSB
  EV874_IFCW,                                      // Fast Clear window
  EV874_IBitSet2,                                  // Bit set register 2
  EV874_IBitClr2,                                  // Bit clear register 2
  EV874_ICrateSelect,                              // Crate select
  EV874_IEvCntReset,                               // Event counter reset
  EV874_IVset,                                     // TDC conversion gain
  EV874_IVoff,                                     // TDC time offset
  EV874_IClearTime,                                // clear time
  EV874_IThresh,                                   // pedestal threshold reg's
  EV874_IAux = EV874_IThresh + EV874_NThresh,      // aux bus for piggy back
  EV874_IOutBuff = EV874_IAux + EV874_NAux         // data buffer
};

class TVME_V874 : public TVMEmodule {
 protected:
  Int_t fThresh[32];                                // QDC data threshold values
  Int_t fFCWindow;                                  // Fast Clear time window
  Int_t fClrTime;                                   // Clear time
  Int_t fVset;                                      // TDC conversion gain
  Int_t fVoff;                                      // TDC time offset value
  Int_t fPedLG[4];                                  // LG pedestal values
  Int_t fPedLGS[4];                                 // LGS pedestal values
  Int_t fPedSG[4];                                  // SG pedestal values
  Int_t fPedSGS[4];                                 // SGS pedestal values
  Int_t fPedBP[2];                                  // Bit pattern "pedestals"
  Int_t fThrCFD[4];                                 // CFD threshold values
  Int_t fThrLED1[4];                                // LED1 threshold values
  Int_t fThrLED2[4];                                // LED2 threshold values
  Bool_t fIsEnThresh;                               // Enable pedestal suppress?
  Bool_t fIsEnOvFlow;                               // Enable overflow suppress?
  Bool_t fIsComStop;                                // Common stop mode?
 public:
  TVME_V874( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TVME_V874();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module
  virtual void PostInit( );
  virtual void ReadIRQ( void** );
  virtual Bool_t CheckHardID();
  virtual Bool_t IsEnThresh(){ return fIsEnThresh; }
  virtual Bool_t IsEnOvFlow(){ return fIsEnOvFlow; }
  virtual Bool_t IsComStop(){ return fIsComStop; }
  // Reset data buffer and event counter
  virtual void ResetData(){Write(EV874_IBitSet2,0x4);Write(EV874_IBitClr2,0x4);}
  // Full software reset of module
  virtual void Reset(){ Write(EV874_IBitSet1,0x8); Write(EV874_IBitClr1,0x8); }
  virtual void SetPiggy( Int_t, Int_t );
  virtual void InitDAC();
  virtual Int_t DACconv( Int_t );
  ClassDef(TVME_V874,1)   

    };

//---------------------------------------------------------------------------
inline Int_t TVME_V874::DACconv( Int_t voltage )
{
  // Convert a threshold voltage (in mV) to the equivalent DAQ value
  // Force to be in range 10 - 1100 mV
  if( voltage < 10 ) voltage = 10;
  if( voltage > 1100 ) voltage = 1100;
  Double_t dac = 8191 - (voltage * 8191/1100);
  return( (Int_t)dac );
}

#endif

