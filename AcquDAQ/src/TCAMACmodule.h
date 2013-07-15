//--Author	JRM Annand	25th Jan 2006
//--Rev 	JRM Annand
//--Rev 	JRM Annand       6th Dec 2007 PostInit check fData
//--Rev 	JRM Annand      11th Jan 2008 Add ReadIRQ
//--Rev 	JRM Annand       5th Sep 2010 Datum in hex CNAF input
//--Update	JRM Annand      16th Sep 2010 ReadIRQScaler...24bit read
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TCAMACmodule
// Basis of all CAMAC modules.
// This contains general-purpose procedures to setup and readout of CAMAC
// General-purpose CAMAC may be instantiated using this class

#ifndef __TCAMACmodule_h__
#define __TCAMACmodule_h__

#include "TDAQmodule.h"
//#include "TDAQCtrlMod.h"

class TCAMACmodule : public TDAQmodule {
 protected:
  Int_t fC;            // Crate number (1-7 parallel branch)
  Int_t fN;            // station number (1-23).
 public:
  TCAMACmodule( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TCAMACmodule();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module
  virtual void PostInit();
  virtual void Init( );
  virtual void InitReg( Char_t* );            // initialise and store register
  //  virtual void Read(void*, void*, Int_t, Int_t);
  //  virtual void Write(void*, void*, Int_t, Int_t);
  virtual void Read(void*, void*);
  virtual void Write(void*, void*);
  virtual UInt_t Read(Int_t);
  virtual void Write(Int_t);
  virtual void ReadIRQ(void**);       // interrupt driven readout procedure
  virtual void ReadIRQScaler(void**); // scaler readout procedure
  virtual UShort_t* CNAF( UInt_t c, UInt_t n, UInt_t a, UInt_t f );
  virtual UInt_t* CNAFL( UInt_t c, UInt_t n, UInt_t a, UInt_t f );
  //

  ClassDef(TCAMACmodule,1)   

    };

//-----------------------------------------------------------------------------
inline void TCAMACmodule::Read( void* addr, void* data )
{
  // Controller read
  fCtrlMod->Read( addr, data, 0x39, 0x02  );
}

//-----------------------------------------------------------------------------
inline void TCAMACmodule::Write( void* addr, void* data )
{
  // Controller write
  fCtrlMod->Write( addr, data, 0x39, 0x02  );
}

//-----------------------------------------------------------------------------
inline UInt_t TCAMACmodule::Read( Int_t i )
{
  // Controller read
  fPCtrlMod->Read( fReg[i], fData+i, 0x39, 0x02 );
  return fData[i];
}

//-----------------------------------------------------------------------------
inline void TCAMACmodule::Write( Int_t i )
{
  // Controller write
  fPCtrlMod->Write( fReg[i], fData+i, 0x39, 0x02 );
}

//-----------------------------------------------------------------------------
inline UShort_t* TCAMACmodule::CNAF( UInt_t c, UInt_t n, UInt_t a,
				     UInt_t f )
{
  // Call the controller's CNAF convertion
  // 16-bit data transfer assumed at that address
  return fCtrlMod->AddrS( c, n, a, f );
}

//-----------------------------------------------------------------------------
inline UInt_t* TCAMACmodule::CNAFL( UInt_t c, UInt_t n, UInt_t a,
				    UInt_t f )
{
  // Call the controller's CNAF convertion
  // 24-bit data transfer assumed at that address
  return fCtrlMod->AddrL( c, n, a, f );
}

//-----------------------------------------------------------------------------
inline void TCAMACmodule::ReadIRQ( void** outBuffer )
{
  // Default basic read of series of CNAF's identified as "ADC" reads
  // These "IRQ" CNAFs come after Initialisation and Test CNAF lists
  //
  Int_t i,j;
  for(i=0,j=fNInitReg+fNTestReg; i<fNIRQReg; i++, j++){
    Read(j);
    ADCStore( outBuffer, fData[j], fBaseIndex+i );  // store read from module
  }
}

//-----------------------------------------------------------------------------
inline void TCAMACmodule::ReadIRQScaler( void** outBuffer )
{
  // Default basic read of series of CNAF's identified as "Scaler" reads
  // These "IRQ" CNAFs come after Initialisation and Test CNAF lists
  // For now limit to 16 bit...problem with msb (bits 16-23) CAMAC read
  // using KPh i686-based VMEbus master
  //
  Int_t i,j;
  // UInt_t datum;
  // void* addr;
  for(i=0,j=fNInitReg+fNTestReg; i<fNIRQReg; i++, j++){
    //    addr = (UShort_t*)((Char_t*)fReg[j] - 2);// for most significant bits
    //    Read( addr, (void*)fData[j] );             // 8 msb
    //    datum = fData[j] << 16;                    // bits 16-23
    Read(j);                                   // 16 lsb
    //    datum |= fData[j];                         // bits 0-15
    // Store 16 significant bits
    ScalerStore( outBuffer, (fData[j] & 0xffff), fBaseIndex+i );
  }
}


#endif
