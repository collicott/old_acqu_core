//--Author	JRM Annand    2nd Sep 2009  Convert from acqu 3v7
//--Rev         B. Oussena   15th Jan 2010  Start updates
//--Rev         B. Oussena   15th Jan 2010  Add Cmd E1821SMI_INIT & TFB_1821SMI::Init_SMI1821()
//--Rev         B. Oussena   30th Jan 2010  fix seg fault: set access to virt. addresses
//--Rev         B. Oussena    1st Feb 2010  Add  TFB_1821SMI::Add_FbADC()
//--Rev         B. Oussena   24th Feb 2010  fix bug : set status=0x20
//--Rev         B. Oussena   28th Feb 2010  Add  TFB_1821SMI::Add_FbSCA()
//--Rev 	JRM Annand   27th Jul 2010  Some cleaning up
//--Rev 	B. Oussena   2nd  Aug 2010  fixed a bug in the constructor.
//--Rev 	B. Oussena   3rd  Aug 2010  fixed a bug in TFB_1821SMI::HM_1821: set W=0.
//--Rev 	JRM Annand    9th Sep 2010  ESMIStatusTry reset to 1000
//--Rev 	JRM Annand   14th Sep 2010  PedAddr() n<<8
//--Rev  	JRM Annand    6th Jul 2011  gcc4.6-x86_64 warning fixes
//--Rev 	B. Oussena    7th Nov 2011  have re-organised the parameters lines from the methods. Have seemed before not to be working
//--Update	B. Oussena    7th Nov 2011  added one printf in method  Exec1821

//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TFB_1821SMI
// LeCroy 1821 FASTBUS Segment Interface Manager (SMI)
// FASTBUS crate controller
//
#ifndef __TFB_1821SMI_h__
#define __TFB_1821SMI_h__

#include "TDAQmodule.h" 
#include "TVME_KPhSMI.h"

enum { ESMIStatusTry = 3,        // was equal to  1000 baya
       ESMIClrFB = 0x1c,	// start addr clear fastbus macro 
       ESMIBusMaster = 0x2a,	// start addr bus mastership macro 
       ESMIBlkRd = 0x33,	// start addr block read macro 
       ESMIRealTm = 0x03,	// start addr real time loop 
       ESMIScalerRd = 0x0b,   	// start of scaler list readout 
       ESMIADCRd = 0x0d,	// start of ADC list readout 
       ESMIReset = 0x8000,	// reset the SMI 
       ESMIPedSub = 0x100,	// enable pedestal subtract 
       ESMICompress = 0x1000,	// enable data compression 
       ESMISuppNeg = 0x800,	// suppress negatives(after ped sub)
       ESMIPipeDisable = 0x840c,// no pipeline operations 
       ESMIPrimAddr = 0x21,	// prim addr err 
       ESMIDefault = 0x28,	// hard block unknown error
       ESMIFront = 0x20,        // front-panel communication
       ESMIFrontFast = 0x60,    // fast front-panel communication
       ESMIRear = 0x30,         // rear-panel communication
       ESMIEndRear = 0xac,      // sequencer return rear panel
       ESMIEndFront = 0x00,     // sequencer return front panel
       ESMIbufflen = 8192,      // raw readout buffer length
};

class TFB_1821SMI : public TDAQmodule{
  friend class TVME_KPhSMI;
 protected:
  //------------------ Baya
  Int_t fSMI;               // SMI no 0-15
  Int_t fRAM;	            // RAM no to load 0-7 */
  Int_t fSlots;             // no slots filled in crate */
  Int_t fIPanel;            // indicates type of readout */
  Int_t fPipe;              // configure SMI pipeline */
  Int_t fThresh;            // SMI data compression thresh */
  Int_t fVMEInterface;      // for >1 VME-SMI interface */
  TVME_KPhSMI* fFBCtrlMod;  // controller has extra specialist stuff
  
  // Adds from Baya
  
  TList *fADCList;
  TList *fScalerList;
  Int_t fNADC; 
  Int_t fNScaler;
  
  //------------------------- 
  
 public:
  TFB_1821SMI( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TFB_1821SMI();
  void SetConfig( Char_t*, Int_t );         
  virtual void PostInit();
  virtual void Read(void*, void*, Int_t, Int_t);
  virtual void Write(void*, void*, Int_t, Int_t);
  virtual void Read(void*, void*);
  virtual void Write(void*, void*);
  virtual void SelectSMI(UShort_t); //<<--------- Baya
  virtual void BlkRead();
  virtual void BlkWrite();
  //
  // SMI I/O routines
  Int_t Exec1821(UShort_t);	// execute 1821 code at given address 
  Int_t ExecRtime(UShort_t);	// boot 1821 SMI into real time loop 
  Int_t StopRtime();	       // retrieve the SMI sequ from the real time loop
  Int_t FB_read(unsigned*);		// read from attached slave 
  Int_t FB_wrt(unsigned);			// write to attached slave 
  Int_t IO_1821(UShort_t,UInt_t,UInt_t*, char, char);
  // general purpose address, r/w to slave module 
  Int_t R_1821(UShort_t,UInt_t,UInt_t*, char);
  // read from slave 
  Int_t W_1821(UShort_t,UInt_t,UInt_t, char);
  // write to slave 
  Int_t R_Block(UShort_t,UInt_t*, Int_t*, UShort_t);
  // address and block read slave 
  Int_t R_Block_f(unsigned*, Int_t*, UShort_t, UShort_t);
  // list mode block read 
  Int_t R_Block_L(UShort_t,UInt_t,UInt_t*, Int_t*);
  // diagnostic block read 
  Int_t R_Block_Tpin(unsigned,UInt_t*, Int_t*, UShort_t);
  //
  //
  Int_t MS_1821(UShort_t);   // download code from 1821 ROM/RAM to smi
  Int_t HS_1821(UShort_t);   // download code from Host to Sequencer 
  Int_t SH_1821();		   // upload SMI code from smi to host file
  Int_t HM_1821(UShort_t);   // download SMI code from host to 1821 RAM 
  Int_t SMI_code_gen(short, short, short);

  // generate SMI code to read list of FASTBUS addrs
  Int_t ch_format(UShort_t*, UShort_t*, UShort_t);

  // change SMI code line format 
  Int_t W_line(UShort_t, UShort_t*, UShort_t);

  // write single line of SMI code 
  Int_t W_ped(UShort_t, UShort_t, UShort_t);

  // write SMI pedestal memory 
  Int_t W_thr(UShort_t);     // write SMI thresh (data compression) mem
  Int_t R_ped(UShort_t, UShort_t, UShort_t*);
  Int_t smi_err_decode(UShort_t, UShort_t);
  // decode diagnostic number returned by SMI 
  Int_t smi_error(UInt_t**, void**, Int_t*);// get and decode error
  //
  //  Int_t PedAddr(Int_t n,Int_t a){ return (n << 11) | a; }
  Int_t PedAddr(Int_t n,Int_t a){ return (n << 8) | a; }
  //
  UShort_t GetSmi(){ return fSMI; }
  UShort_t GetRam(){ return fRAM; }
  UShort_t GetIpanel(){ return fIPanel; }
  virtual void Init_SMI1821(Char_t*);//<-----   Baya
  Int_t Add_FbADC(TDAQmodule*); // Baya Add FB ADC to list
  Int_t Add_FbSCA(TDAQmodule*);  // Baya Add Sca to list

  ClassDef(TFB_1821SMI,1)   
    };

//-----------------------------------------------------------------------------
inline void TFB_1821SMI::Read( void* addr, void* data, Int_t am, Int_t dw )
{
  //
}

//-----------------------------------------------------------------------------
inline void TFB_1821SMI::Write( void* addr, void* data, Int_t am, Int_t dw )
{
  //
}

//-----------------------------------------------------------------------------
inline void TFB_1821SMI::Read( void* addr, void* data )
{
  // Read with default AM code and data size
}

//-----------------------------------------------------------------------------
inline void TFB_1821SMI::Write( void* addr, void* data )
{
  // Write at default AM code and data size
  //
}
//-----------------------------------------------------------------------------
inline void TFB_1821SMI::SelectSMI( UShort_t smi)
{
  // Write at default AM code and data size
  //fCtrlMod->S_KPH(smi);/* select smi by front panel*/

  //
}



#endif
