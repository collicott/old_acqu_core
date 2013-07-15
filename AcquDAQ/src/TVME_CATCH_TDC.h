//--Author	JRM Annand     7th Jun 2007
//--Rev 	JRM Annand
//--Rev 	JRM Annand..  28th Apr 2009..remove TDAQmemmap.h
//--Rev 	JRM Annand..  25th Oct 2009..further development
//--Rev         B. Oussena    24 Jul 2010 modified PostInit()
//--Rev         B. Oussena    24 Jul 2010 set access to virt adr in F1Setup()
//--Rev         B. Oussena  13th Aug 2010  more cleaning 
//--Update      JRM Annand   8th Sep 2010  TCS event ID to TVME_CATCH
//
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TVME_CATCH_TDC
// COMPASS CATCH motherboard with F1 TDC piggy backs
//

#ifndef __TVME_CATCH_TDC_h__
#define __TVME_CATCH_TDC_h__

#include "TVME_CATCH.h"

struct F1Data_t{
  unsigned board;
  unsigned Xcatch;
  unsigned tdc;              
  unsigned registers[16];  // data (in file-format) to send to f1
};

class TVME_CATCH_TDC : public TVME_CATCH {
 protected:
  F1Data_t fF1data;
 public:
  TVME_CATCH_TDC( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TVME_CATCH_TDC();
  virtual void SetConfig( Char_t*, Int_t );   // configure VME module
  virtual void PostInit( );
  virtual void ReadIRQ( void** );
  Int_t F1ReadFile(Char_t*, F1Data_t*);
  Int_t F1Setup( F1Data_t* );
  ClassDef(TVME_CATCH_TDC,1)   

    };

#endif
