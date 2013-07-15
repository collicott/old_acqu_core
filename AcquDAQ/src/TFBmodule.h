//--Author	JRM Annand     2nd Sep 2009  Convert from acqu 3v7
//--Rev 	JRM Annand  
//--Rev         B. Oussena   15th Jan 2010 Start updates for FASTBUS
//--Rev         B. Oussena    1st feb 2010 Add cmd EFB_RDMode, SetRDMode()
//--Rev         B. Oussena   10th Mar 2010 Add decode Scaler and ADC methods
//--Rev  	JRM Annand   27th Jul 2010 Generalise 1800 module class
//--Rev         B. Oussena    3rd Aug 2010 Fix bugs in SetConfig() & PostInit()
//--Rev         JRM Annand    4th Sep 2010 Chk Mk1/Mk2 data, fBaseIndex setting
//--Rev         JRM Annand    9th Sep 2010 Ordering of CSR config
//--Update	JRM Annand    6th Jul 2011 gcc4.6-x86_64 warning fixes
//
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TFBmodule
// Basic class for FASTBUS slave modules
// General-purpose FASTBUS hardware with no specialist read/write procedures
// may use this class
//

#ifndef __TFBmodule_h__
#define __TFBmodule_h__

#include "TDAQmodule.h"
class TFB_1821SMI;

 
class TFBmodule : public TDAQmodule {
 protected:
  TFB_1821SMI* fSMIctrl;
  Int_t fSMI;        // smi No on branch 0-15 (Switch on SMI)
  Int_t fN;          // station of Fastbus crate
  Char_t fPanel;     //readout connector FRONT or REAR panel
  Char_t fP_SMI;     //concatenation of smi and panel for smi sel
  Char_t fReadMode;  // readout mode (m/b/s) megablock,block,single word subadr
  Char_t fTest;      // execute module self test (y/n)
  Char_t fGeog;      // geog addressing (y/n) std adr mode
  UInt_t fC0_Reset;  // c0 bit pattern for master register reset */
  UInt_t fC0_Config; // c0 bit pattern for enabling module setup */
  UInt_t fC0_Test;   // c0 bit pattern to initiate module test */
  UInt_t fC0_Acqu;   // c0 bit pattern for general acquistion */
  UInt_t fC0_Acqu_M; // c0 bit pattern for megablock master */
  UInt_t fC0_Acqu_S; // c0 bit pattern for megablock simple link */
  UInt_t fC0_Acqu_E; // c0 bit pattern for megablock end link */
  UShort_t fD_MBlk;  // megablock readout reg. */
  UShort_t fD_Blk;   // block mode readout reg. */
  UShort_t fD_Read;  // contains station and readout reg. */
  //Bool_t* fIsWrt;    // Write initialisation data?
 public:
  TFBmodule( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TFBmodule();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module
  virtual void PostInit( );
  virtual void InitReg( Char_t* );            // initialise and store register
  virtual void InitReg( UInt_t, Int_t, Int_t, Int_t, UInt_t=0 ){}
  virtual Bool_t CheckHardID();
  virtual void SetRDMode(); //<<-----     Baya 
  virtual void DecodeScaler( UInt_t**, void**, Int_t*);
  virtual void Decode( UInt_t**, void**, Int_t*){}
  //Bool_t* GetIsWrt(){ return fIsWrt; }
  Char_t GetReadMode(){ return fReadMode; }
  UShort_t GetD_Read(){ return fD_Read; }

  ClassDef(TFBmodule,1)   

    };

#endif
