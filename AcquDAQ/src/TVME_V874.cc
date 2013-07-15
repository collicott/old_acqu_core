//--Author	JRM Annand   9th Jan 2013  Prototype version
//--Rev 	
//--Update      B. Oussena   5th Feb 2013  Fix bug in ReadIRQ()
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

#include "TVME_V874.h"
#include "TDAQexperiment.h"
//#include "TDAQmemmap.h"
//
// Internal register address offsets
VMEreg_t V874reg[] = {
  {0x1000,  0x0, 'w', 0x0},             // Firmware version
  {0x1006,  0x0, 'w', 0x0},             // Bit Set 1
  {0x1008,  0x0, 'w', 0x0},             // Bit Clear 1
  {0x100e,  0x0, 'w', 0x0},             // Status reg 1
  {0x1010,  0x0, 'w', 0x0},             // Control reg 1
  {0x1022,  0x0, 'w', 0x0},             // Status reg 2
  {0x1024,  0x0, 'w', 0x0},             // Event counter lsb
  {0x1026,  0x0, 'w', 0x0},             // Event counter msb
  {0x102e,  0x0, 'w', 0x0},             // Fast clear window
  {0x1032,  0x0, 'w', 0x0},             // Bit Set 2
  {0x1034,  0x0, 'w', 0x0},             // Bit Clear 2
  {0x103c,  0x0, 'w', 0x0},             // Crate select
  {0x1040,  0x0, 'w', 0x0},             // Event counter reset
  {0x1060,  0x0, 'w', 0x0},             // Vset
  {0x1062,  0x0, 'w', 0x0},             // Voff
  {0x1066,  0x0, 'w', 0x0},             // Clear time
  {0x1080,  0x0, 'w', EV874_NThresh-1}, // ADC Threshold registers
  {0x1200,  0x0, 'w', EV874_NAux-1},    // for access to piggy back
  {0x0000,  0x0, 'l', EV874_NOutBuff-1},// data output buffer
  {0xffffffff,0x0,'w',0x0},             // terminating line
};

// DAC addresses on piggy back board
const Int_t DevPedLG[] =   { 0x0f, 0x0e, 0x09, 0x08 }; // DAC addr LG ped
const Int_t DevPedLGS[] =  { 0x10, 0x11, 0x16, 0x17 }; // DAC addr LGS ped
const Int_t DevPedSG[] =   { 0x1f, 0x1e, 0x19, 0x18 }; // DAC addr SG ped
const Int_t DevPedSGS[] =  { 0x20, 0x21, 0x26, 0x27 }; // DAC addr SGS ped
const Int_t DevThrCFD[] =  { 0x24, 0x1c, 0x14, 0x0c }; // DAC addr CFD thr
const Int_t DevThrLED1[] = { 0x22, 0x1a, 0x12, 0x0a }; // DAC addr LED1 thr
const Int_t DevThrLED2[] = { 0x23, 0x1b, 0x13, 0x0b }; // DAC addr LED2 thr


ClassImp(TVME_V874)

enum { EV874_ThrLG=200, EV874_ThrLGS, EV874_ThrSG, EV874_ThrSGS, EV874_ThrBP,
       EV874_FCWindow, EV874_CLRTime, EV874_Voff, EV874_Vset, EV874_EnThresh,
       EV874_EnOvFlow, EV874_ComStop, EV874_PedLG, EV874_PedLGS,
       EV874_PedSG, EV874_PedSGS, EV874_ThrCFD, EV874_ThrLED1, EV874_ThrLED2};
static Map_t k874Keys[] = {
  {"Thr-LG:",             EV874_ThrLG},
  {"Thr-LGS:",            EV874_ThrLGS},
  {"Thr-SG:",             EV874_ThrSG},
  {"Thr-SGS:",            EV874_ThrSGS},
  {"Thr-BP:",             EV874_ThrBP},
  {"FC-Window:",          EV874_FCWindow},
  {"Clear-Time:",         EV874_CLRTime},
  {"V-Set:",              EV874_Vset},
  {"V-Off:",              EV874_Voff},
  {"Suppress-Pedestal:",  EV874_EnThresh},
  {"Suppress-Overflow:",  EV874_EnOvFlow},
  {"Common-Stop:",        EV874_ComStop},
  {"Ped-LG:",             EV874_PedLG},
  {"Ped-LGS:",            EV874_PedLGS},
  {"Ped-SG:",             EV874_PedSG},
  {"Ped-SGS:",            EV874_PedSGS},
  {"Thr-CFD:",            EV874_ThrCFD},
  {"Thr-LED1:",           EV874_ThrLED1},
  {"Thr-LED2:",           EV874_ThrLED2},
  {NULL,                  -1}
};

//-----------------------------------------------------------------------------
TVME_V874::TVME_V874( Char_t* name, Char_t* file, FILE* log,
		      Char_t* line ):
  TVMEmodule( name, file, log, line )
{
  // Default initialisation of TAPS readout
  fCtrl = NULL;                            // no control functions
  fType = EDAQ_ADC;                        // analogue to digital converter
  // Last "hard-wired" register
  fNreg = EV874_IOutBuff + EV874_NOutBuff + 2;        
  fHardID = 0x0406;                        // Firmware ID read from hardware
  fNBits = 12;                             // 12-bit ADC
  fIsEnThresh = fIsEnOvFlow = kFALSE;      // default no suppress pedestal/oflow
  fIsComStop = kFALSE;                     // default common start
  fFCWindow = 0;                           // Fast clear window
  fClrTime = 0;                            // Time to clear analogue sections
  fVoff = 240;                             // TDC offset 300ns
  fVset = 140;                             // TDC conv gain 54ps/chan
  // default all readout channel not stored
  for(Int_t i=0; i<32; i++){ fThresh[i] = 0x1ff; }
  for(Int_t i=0; i<4; i++){
    fPedLG[i] = fPedLGS[i] = fPedSG[i] = fPedSGS[i] = 4000;
    // 50 mV discriminator threshold approx
    fThrCFD[i] = fThrLED1[i] = fThrLED2[i] = 7820;
  }
  AddCmdList( k874Keys );                  // CAEN-specific setup commands
}

//-----------------------------------------------------------------------------
TVME_V874::~TVME_V874( )
{
  // Clean up
}

//-----------------------------------------------------------------------------
void TVME_V874::SetConfig( Char_t* line, Int_t key )
{
  // Configuration from file 
  Int_t val[32];
  switch( key ){
  case EV874_ThrLG:
    // Settings of LG data suppression threshold registers
    if( sscanf(line,"%i%i%i%i",fThresh+10,fThresh+14,fThresh+22,fThresh+26)
	!= 4 ){ PrintError(line,"<Parse LG threshold read>"); }
    break;
  case EV874_ThrLGS:
    // Settings of LGS data suppression threshold registers
    if( sscanf(line,"%i%i%i%i",fThresh+8,fThresh+12,fThresh+20,fThresh+24)
	!= 4 ){ PrintError(line,"<Parse LGS threshold read>"); }
    break;
  case EV874_ThrSG:
    // Settings of SG data suppression threshold registers
    if( sscanf(line,"%i%i%i%i",fThresh+11,fThresh+15,fThresh+23,fThresh+27)
	!= 4 ){ PrintError(line,"<Parse SG threshold read>"); }
    break;
  case EV874_ThrSGS:
    // Settings of SGS data suppression threshold registers
    if( sscanf(line,"%i%i%i%i",fThresh+9,fThresh+13,fThresh+21,fThresh+25)
	!= 4 ){ PrintError(line,"<Parse SGS threshold read>"); }
    break;
  case EV874_ThrBP:
    // Settings of Bit Pattern data suppression threshold registers
    if( sscanf(line,"%i%i",fThresh+30,fThresh+31)
	!= 2 ){ PrintError(line,"<Parse Bit-Pattern suppress read>"); }
    break;
  case EV874_EnThresh:
    // Turn on pedestal suppress
    fIsEnThresh = kTRUE;
    break;
  case EV874_EnOvFlow:
    // Turn on overflow suppress
    fIsEnOvFlow = kTRUE;
    break;
  case EV874_FCWindow:
    // Set fast clear window...units of 31.25ns, max value 1008
    if( sscanf(line,"%i",&fFCWindow) != 1 ){
      PrintError(line,"<Parse FC window read>");
    }
    break;
  case EV874_CLRTime:
    // Time to clear ADCs
    if( sscanf(line,"%i",&fClrTime) != 1 ){
      PrintError(line,"<Parse clear time read>");
    }
    break;
  case EV874_Vset:
    // Vset TDC conversion gain. Value 140,43 -> 54,103 ps/chan
    if( sscanf(line,"%i",&fVset) != 1 ){
      PrintError(line,"<Parse TDC convertion gain read>");
    }
    break;
  case EV874_Voff:
    // Voff Time offset for TDC. Value 240,165,131 -> 300,400,500ns
    if( sscanf(line,"%i",&fVoff) != 1 ){
      PrintError(line,"<Parse TDC offset read>");
    }
    break;
  case EV874_ComStop:
    // Switch to common-stop mode
    fIsComStop = kTRUE;
    break;
  case EV874_PedLG:
    // 4 Long-gate QDC pedestal values
    if( sscanf(line,"%i%i%i%i",fPedLG,fPedLG+1,fPedLG+2,fPedLG+3) != 4 ){
      PrintError(line,"<Parse LG pedestal read>");
    }
    break;
  case EV874_PedLGS:
    // 4 Long-gate-S QDC pedestal values
    if( sscanf(line,"%i%i%i%i",fPedLGS,fPedLGS+1,fPedLGS+2,fPedLGS+3) != 4 ){
      PrintError(line,"<Parse LGS pedestal read>");
    }
    break;
  case EV874_PedSG:
    // 4 short-gate QDC pedestal values
    if( sscanf(line,"%i%i%i%i",fPedSG,fPedSG+1,fPedSG+2,fPedSG+3) != 4 ){
      PrintError(line,"<Parse SG pedestal read>");
    }
    break;
  case EV874_PedSGS:
    // 4 short-gate-S QDC pedestal values
    if( sscanf(line,"%i%i%i%i",fPedSGS,fPedSGS+1,fPedSGS+2,fPedSGS+3) != 4 ){
      PrintError(line,"<Parse SGS pedestal read>");
    }
    break;
  case EV874_ThrCFD:
    // 4 CFD threshold values, in mV (positive)
    if( sscanf(line,"%d%d%d%d",val,val+1,val+2,val+3) != 4 ){
      PrintError(line,"<Parse CFD threshold read>");
      break;
    }
    for(Int_t i=0; i<4; i++){ fThrCFD[i] = DACconv(val[i]); }
    break;
  case EV874_ThrLED1:
    // 4 LED1 threshold values, in mV (positive)
    if( sscanf(line,"%d%d%d%d",val,val+1,val+2,val+3) != 4 ){
      PrintError(line,"<Parse LED1 threshold read>");
      break;
    }
    for(Int_t i=0; i<4; i++){ fThrLED1[i] = DACconv(val[i]); }
    break;
  case EV874_ThrLED2:
    // 4 LED2 threshold values, in mV (positive)
    if( sscanf(line,"%d%d%d%d",val,val+1,val+2,val+3) != 4 ){
      PrintError(line,"<Parse LED2 threshold read>");
      break;
    }
    for(Int_t i=0; i<4; i++){ fThrLED2[i] = DACconv(val[i]); }
    break;
  default:
    // default try commands of TDAQmodule
    TVMEmodule::SetConfig(line, key);
    break;
  }
}

//---------------------------------------------------------------------------
void TVME_V874::ReadIRQ( void** outBuffer )
{
  // Read and decode the memory of the V874
  // Store the decoded ADC index and value in *outBuffer
  // Any detected errors in the data format are written into the data stream,
  // readout aborted and a software reset made
  // This version decodes a single event and then resets the data buffer
  //
  UShort_t status1 = Read(EV874_IStatus1);    // read status reg. for data avail
  if( !(status1 & 0x1) ) return;              // no data in buffer
  if( (status1 & 0x4) ){                      // check if busy
    fprintf(fLogStream,"<V874 busy, Status = %x (hex)>\n", status1);
    ErrorStore(outBuffer, EErrDataFormat);
    ResetData();
    return;
  }
  Int_t i = EV874_IOutBuff;
  UInt_t datum = Read(i++);                 // data header
  if( (datum & 0x7000000) != 0x2000000 ){   // check its a header word
    ErrorStore(outBuffer, EErrDataFormat);
    ResetData();
    return;
  }
  Int_t j;                                  // added BAYA
  Int_t nword = (datum & 0x3f00) >> 8;      // # data channels recorded
  //
  UShort_t adcVal, adcIndex;                // adc value and index
  for( j=1; j<=nword; j++ ){
    datum = Read(i);                        // i++ BAYA
    i++;
    // id bits should be zero
    if( (datum & 0x7000000) ){              // data word? should be zero
      ErrorStore(outBuffer, EErrDataFormat);
      ResetData();
      return;
    }
    if( (datum & 0x1000) ) adcVal = 0xfff;  // overflow
    else adcVal = datum & 0xfff;            // ADC value not overflowed
    adcIndex = ((datum & 0x3f0000) >> 16) - 8;    // ADC subaddress
    adcIndex += fBaseIndex;                 // index offset
    ADCStore( outBuffer, adcVal, adcIndex );// store values
  }
  datum = Read(i); 
  if( (datum & 0x7000000) != 0x4000000 ){   // Check trailer word
    ErrorStore(outBuffer, EErrDataFormat);
    ResetData();
    return;
  }
  ResetData();
}

//-------------------------------------------------------------------------
void TVME_V874::PostInit( )
{
  // Check if any general register initialisation has been performed
  // Call standard VME PostInit to map register addresses to virtual
  // memory system and read and check the firmware ID...fatal error if non-match
  if( fIsInit ) return;            // Init already done?
  InitReg( V874reg );              // Store register addesses
  TVMEmodule::PostInit();          // standard memory mapping & id check
  // Specialist V874
  Reset();                         // software reset
  Write(EV874_IBitSet2, 0x2);      // ADC offline
  Write(EV874_IBitClr2, 0x100);    // setup mode
  // set operating mode
  Write(EV874_IBitSet2, 0x800);                  // Auto increment data ptr
  if( !fIsEnOvFlow ) Write(EV874_IBitSet2, 0x8); // Overflow suppress
  if( fIsEnThresh ) Write(EV874_IBitSet2, 0x10); // Below threshold suppress
  if( fIsComStop ) Write(EV874_IBitSet2, 0x400); // Common stop mode
  Write(EV874_IFCW, fFCWindow);                  // Set Fast Clear Window
  Write(EV874_IClearTime,fClrTime);              // set Clear Time
  Write(EV874_IVset,fVset);                      // set TDC gain
  Write(EV874_IVoff,fVoff);                      // set TDC offset
  Write(EV874_IControl1, (UInt_t)0);             // ??
  // Data threshold (pedestal suppression) settings
  for(Int_t i=0,j=EV874_IThresh; i<32; i++,j++) { Write(j,fThresh[i]); }
  InitDAC();                      // Setup DACs on piggy back board
  Write(EV874_IBitSet2,0x100);    // Readout mode
  Write(EV874_IBitClr2, 0x2);     // ADC online
  ResetData();
  return;
}

//-------------------------------------------------------------------------
Bool_t TVME_V874::CheckHardID( )
{
  // Read module firmware ID from internal ROM
  Int_t id = Read(EV874_IFirmware) & 0xffff;
  if( id == fHardID ){
    fprintf(fLogStream,"V874 firmware version Read: %x  Expected: %x\n",
	    id,fHardID);
    return kTRUE;
  }
  else
    fprintf(fLogStream,"V874 firmware version Read: %x  Expected: %x\n",
	    id,fHardID);
    PrintError("","<CAEN V874 hardware ID read error>",EErrFatal);
  return kFALSE;
}

//-------------------------------------------------------------------------
void TVME_V874::SetPiggy(Int_t device, Int_t value)
{
  // Write 13-bit value to DAC at address device on piggy back board
  Write(EV874_IBitClr2, 0x100);                   // setup mode
  Write(EV874_IAux, value & 0xff);                // 8 lsb transfer
  Write(EV874_IAux + 0x38, (value & 0x1f00)>>8);  // 5 msb transfer
  Write(EV874_IAux + (0x40 | device), (UInt_t)0); // int PLD->ext pins PLD
  Write(EV874_IAux + (0x40 | device), (UInt_t)0); // write to DAC
  Write(EV874_IBitSet2, 0x100);                   // readout mode
}

//-------------------------------------------------------------------------
void TVME_V874::InitDAC()
{
  // Load values into 13-bit DACs located on piggy-back board
  // values loaded via 8-bit aux bus.
  for(Int_t i=0; i<4; i++){
    SetPiggy( DevPedLG[i], fPedLG[i] );     // load LG pedestals
    SetPiggy( DevPedLGS[i], fPedLGS[i] );   // load LGS pedestals
    SetPiggy( DevPedSG[i], fPedSG[i] );     // load SG pedestals
    SetPiggy( DevPedSGS[i], fPedSGS[i] );   // load SGS pedestals
    SetPiggy( DevThrCFD[i], fThrCFD[i] );   // load CFD thresholds
    SetPiggy( DevThrLED1[i], fThrLED1[i] ); // load LED1 thresholds
    SetPiggy( DevThrLED2[i], fThrLED2[i] ); // load LED2 thresholds
  }
}
