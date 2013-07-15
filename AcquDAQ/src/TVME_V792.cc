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

#include "TVME_V792.h"
#include "TDAQexperiment.h"
//#include "TDAQmemmap.h"

ClassImp(TVME_V792)

enum { ECAEN_Threshold=200, ECAEN_NoThreshold };
static Map_t kCAENKeys[] = {
  {"Threshold:",          ECAEN_Threshold},
  {"No-threshold:",       ECAEN_NoThreshold},
  {NULL,                  -1}
};

//-----------------------------------------------------------------------------
TVME_V792::TVME_V792( Char_t* name, Char_t* file, FILE* log,
		      Char_t* line ):
  TVMEmodule( name, file, log, line )
{
  // Basic initialisation 
  fCtrl = NULL;                            // no control functions
  fType = EDAQ_ADC;                        // analogue to digital converter
  fNreg = fMaxReg = EV7XX_IID2 + 1;        // Last "hard-wired" register
  fHardID = 792;                           // ID read from hardware
  fNBits = 12;                             // 12-bit ADC
  fIsThreshold = kTRUE;                    // default suppress pedestals
  AddCmdList( kCAENKeys );                 // CAEN-specific setup commands
}

//-----------------------------------------------------------------------------
TVME_V792::~TVME_V792( )
{
  // Clean up
}

//-----------------------------------------------------------------------------
void TVME_V792::SetConfig( Char_t* line, Int_t key )
{
  // Configuration from file 
  Int_t ithr, thr, i;
  switch( key ){
  case ECAEN_Threshold:
    // Specific setting of data threshold register
    if( sscanf(line,"%d%d",&ithr,&thr) != 2 ){
      PrintError(line,"<Parse threshold read>");
      break;
    }
    if( !fReg ){
      PrintError(line,"<Threshold setting, registers not yet initialised>");
      break;
    }
    if( ithr > fNChannel ){
      PrintError(line,"<Threshold index > # channels in module>");
      break;
    }
    i = EV7XX_IThresholds + ithr;
    fData[i] = thr;
    break;
  case ECAEN_NoThreshold:
    // Turn off pedestal suppress
    fIsThreshold = kFALSE;
    break;
  default:
    // default try commands of TDAQmodule
    TVMEmodule::SetConfig(line, key);
    break;
  }
}

//---------------------------------------------------------------------------
void TVME_V792::ReadIRQ( void** outBuffer )
{
  // Read and decode the memory of the V792
  // Store the decoded ADC index and value in *outBuffer
  // Errors in contents of header word considered fatal
  // This version decodes a single event and then resets the data buffer
  //
  UShort_t status1 = Read(EV7XX_IStatus1);    // read status reg. for data avail
  if( !(status1 & 0x1) || (status1 & 0x4) ){  // is it OK
    fprintf(fLogStream,"<V7XX QDC/TDC no data or busy, Status = %x (hex)>\n",
	    status1);
    ErrorStore(outBuffer, EErrDataFormat);
    ResetData();
    return;
  }
  Int_t i = EV7XX_IOutBuff;
  UInt_t datum = Read(i++);                 // data header
  if( (datum & 0x7000000) != 0x2000000 ){   // check its a header word
    ErrorStore(outBuffer, EErrDataFormat);
    ResetData();
    return;
  }
  Int_t nword = (datum & 0x3f00) >> 8;      // # data words
  //
  UShort_t adcVal, adcIndex;                // adc value and index
  for( i=1; i<=nword; i++ ){
    datum = Read(i);
    adcVal = datum & 0xfff;                 // ADC value
    adcIndex = (datum & 0x1f0000) >> 16;    // ADC subaddress
    adcIndex += fBaseIndex;                 // index offset
    ADCStore( outBuffer, adcVal, adcIndex );// store values
  }
  datum = Read(i); 
  if( (datum & 0x7000000) != 0x4000000 ){   // Check trailer word
    ErrorStore(outBuffer, EErrDataFormat);
    ResetData();
    return;
  }
  Reset();
  ResetData();
}

//-------------------------------------------------------------------------
void TVME_V792::PostInit( )
{
  // Check if any general register initialisation has been performed
  // Setup data memory, threshold registers and status registers.
  // Check if ID read from internal ROM matches the expected value
  // If OK carry out default init to perform write initialisation of registers
  if( fIsInit ) return;            // Init already done?
  //
  if( !fReg )
    PrintError("","<Register Initialisation not yet performed>", EErrFatal);
  Int_t i,j;
  // Data memory addresses
  for( i=0; i<EV7XX_NMem; i++ ) InitReg( EV7XX_OutBuff+(4*i), 4, 0x09, i );
  // Threshold registers
  for( j=0; j<EV7XX_NThresh; j++ ){
    fIsWrt[i+j] = kTRUE;              // ensure threshold datum is written
    InitReg(EV7XX_Thresholds+(2*j),2,0x09, i+j, fData[i+j]);
  }
  // Status Registers
  InitReg( EV7XX_Status1, 2, 0x09, EV7XX_IStatus1);
  InitReg( EV7XX_Status2, 2, 0x09, EV7XX_IStatus2);
  // Bit-set and clear registers
  InitReg( EV7XX_BitSet1, 2, 0x09, EV7XX_IBitSet1);
  fData[EV7XX_IBitSet1] = 0x80;                      // data memory reset
  InitReg( EV7XX_BitClr1, 2, 0x09, EV7XX_IBitClr1); 
  fData[EV7XX_IBitClr1] = 0x80;                      // clear data memory reset
  InitReg( EV7XX_BitSet2, 2, 0x09, EV7XX_IBitSet2);
  fData[EV7XX_IBitSet2] = 0x4;                       // data memory reset
  // If no pedestal suppression set the "Low threshold enable bit
  if( !fIsThreshold )  fData[EV7XX_IBitSet2] |= 0x10;
  InitReg( EV7XX_BitClr2, 2, 0x09, EV7XX_IBitClr2); 
  fData[EV7XX_IBitClr2] = 0x4;                       // clear data reset
  // Module ID ROM
  InitReg( EV7XX_ID0, 2, 0x09, EV7XX_IID0);
  InitReg( EV7XX_ID1, 2, 0x09, EV7XX_IID1);
  InitReg( EV7XX_ID2, 2, 0x09, EV7XX_IID2);
  // Software reset
  TVMEmodule::PostInit();
  // Software reset. This will reset many registers which potentially one
  // would want to program. The programming then has to be re-done after this
  // reset. Note that pedestal registers are not affected by a soft reset
  Reset();
  ResetData();
  return;
}

//-------------------------------------------------------------------------
Bool_t TVME_V792::CheckHardID( )
{

  // Read module ID from internal ROM
  Int_t id = Read(EV7XX_IID2) & 0xff;
  id |= (Read(EV7XX_IID1) & 0xff) << 8;
  id |= (Read(EV7XX_IID0) & 0xff) << 16;
  if( id == fHardID ) return kTRUE;
  else
    PrintError("","<CAEN V7** ADC/TDC hardware ID read error>",EErrFatal);


  return kFALSE;
}
