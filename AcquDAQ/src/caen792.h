//--Author      JRM Annand    19th June 2003
//--Rev         JRM Annand...
//--Rev         JRM Annand...
//--Update      B. Oussena   13th Aug 2010  more clean up
//--Description
//                *** Acqu++  ***
// Data Acquisition for Sub-Atomic Physics Experiments.
//
// CAEN792
// Procedures for CAEN792 VMEbus QDC
//

#ifndef _caen792_h_
#define _caen792_h_

#include "dmem_module.h"

#define N_CAENV792 32

int init_CAENV792( unsigned short, int,  char** );
int CAENV792_read( struct m_dev_table*, struct sw_buff*, int i );

//-----------------------------------------------------------------------
// Register Structure of CAEN V792 QDC's
//----------------------------------------------------------------------
struct CAENV792_dev{
  UInt_t  *Data;                // data buffer
  UShort_t *FWrev;         // firmware revision
  UShort_t *GeoAddr;       // to set up geographic address
  UShort_t *MCSTaddr;
  UShort_t *SetBits1;      // basic initialisation
  UShort_t *ClrBits1;      // to clear any set bits
  UShort_t *IRQlevel;      // VMEbus IRQ level
  UShort_t *IRQvector;     // IRQ vector
  UShort_t *Status1;       // 1st status register
  UShort_t *Ctrl1;         // 1st control register
  UShort_t *AddrMSB;
  UShort_t *AddrLSB;
  UShort_t *SingleShot;
  UShort_t *MCSTctrl;
  UShort_t *EventTrig;
  UShort_t *Status2;
  UShort_t *EvCntLSB;
  UShort_t *EvCntMSB;
  UShort_t *EvIncr;
  UShort_t *IncrOffset;    // readout pointer control
  UShort_t *FCwindow;      // set fast clear window
  UShort_t *SetBits2;
  UShort_t *ClrBits2;
  UShort_t *MemTestAddr;
  UShort_t *MemTestWhigh;
  UShort_t *MemTestWlow;
  UShort_t *CrateSelect;
  UShort_t *EvTestW;
  UShort_t *EvCntReset;
  UShort_t *Ipedestal;     // pedestal current control
  UShort_t *MemTestR;
  UShort_t *SWcomm;
  UShort_t *SlideConst;
  UShort_t *AADreg;
  UShort_t *BADreg;
  UShort_t *Thresh;       // data storage thresholds
};

struct CAENV792_dev *P_CAENV792[MAX_VME_ADC];

#endif
