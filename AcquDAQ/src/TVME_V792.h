//--Author	JRM Annand   1st Feb 2007
//--Rev 	JRM Annand
//--Rev 	JRM Annand  11th Jan 2008  1st working version
//--Rev 	JRM Annand..28th Apr 2009..remove TDAQmemmap.h
//--Update	JRM Annand.. 2nd May 2011..no pedestal suppress option
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TVME_V792
// CAEN VMEbus 32-channel QDC
// Also forms basis for classes to handle similar CAEN ADC hardware
//

#ifndef __TVME_V792_h__
#define __TVME_V792_h__

#include "TVMEmodule.h"

enum { EV7XX_NMem = 512,
       EV7XX_NReg = 5,
       EV7XX_NThresh = 32,
       EV7XX_NROM = 9 };

// Offsets in indices to internal registers
enum {
  EV7XX_OutBuff = 0,
  EV7XX_Firmware = 0x1000,
  EV7XX_GeoAddr = 0x1002,
  EV7XX_MCST = 0x1004,
  EV7XX_BitSet1 = 0x1006,
  EV7XX_BitClr1 = 0x1008,
  EV7XX_IRQLevel = 0x100A,
  EV7XX_IRQVect = 0x100C,
  EV7XX_Status1 = 0x100E,
  EV7XX_Ctrl1 = 0x1010,
  EV7XX_ADERhigh = 0x1012,
  EV7XX_ADERlow = 0x1014,
  EV7XX_SSReset = 0x1016,
  EV7XX_MCSTCtrl = 0x101A,
  EV7XX_EvTrig = 0x1020,
  EV7XX_Status2 = 0x1022,
  EV7XX_EvCntL = 0x1024,
  EV7XX_EvCntH = 0x1026,
  EV7XX_IncEv = 0x1028,
  EV7XX_IncOff = 0x102A,
  EV7XX_LoadTest = 0x102C,
  EV7XX_FCLRWind = 0x102E,
  EV7XX_BitSet2 = 0x1032,
  EV7XX_BitClr2 = 0x1034,
  EV7XX_WMemTest = 0x1036,
  EV7XX_MemTestWH = 0x1038,
  EV7XX_MemTestWL = 0x103A,
  EV7XX_CrateSel = 0x103C,
  EV7XX_TestEvWrt = 0x103E,
  EV7XX_EvCntRes = 0x1040,
  EV792_Iped = 0x1060,
  EV775_FullScale = 0x1060,
  EV7XX_RTestAddr = 0x1064,
  EV7XX_SWComm = 0x1068,
  EV775_SlConst = 0x106A,
  EV7XX_AAD = 0x1070, 
  EV7XX_BAD = 0x1072,
  EV7XX_Thresholds = 0x1080,
  EV7XX_ID0 = 0x8036,
  EV7XX_ID1 = 0x803A,
  EV7XX_ID2 = 0x803E,
};

// Address indices for default registers
// IID0,IID1,IID2 must come last
enum {
  EV7XX_IOutBuff,
  EV7XX_IThresholds = EV7XX_NMem,
  EV7XX_IStatus1 = EV7XX_NMem + EV7XX_NThresh,
  EV7XX_IStatus2,
  EV7XX_IBitSet1,
  EV7XX_IBitClr1,
  EV7XX_IBitSet2,
  EV7XX_IBitClr2,
  EV7XX_IID0,
  EV7XX_IID1,
  EV7XX_IID2,
};

class TVME_V792 : public TVMEmodule {
 protected:
  Bool_t fIsThreshold;
 public:
  TVME_V792( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TVME_V792();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module
  virtual void PostInit( );
  virtual void ReadIRQ( void** );
  virtual Bool_t CheckHardID();
  virtual Bool_t IsThreshold(){ return fIsThreshold; }
  //
  virtual void ResetData(){ Write(EV7XX_IBitSet2); Write(EV7XX_IBitClr2); }
  virtual void Reset(){ Write(EV7XX_IBitSet1); Write(EV7XX_IBitClr1); }

  ClassDef(TVME_V792,1)   

    };

#endif

