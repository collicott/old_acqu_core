//--Author	JRM Annand	 6th Mar 2013   New class for Mk2 data
//--Rev     
//--Update 
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
//	TA2TAPSMk2Format
//	TAPS data stream, 99 format for AcquRoot Mk2 data format
//
//---------------------------------------------------------------------------

#ifndef _TA2TAPSMk2Format_h_
#define _TA2TAPSMk2Format_h_

#include "TA2Mk2Format.h"

class TA2TAPSMk2Format : public TA2DataFormat {
 protected:
  Bool_t fIsPartEvent;          // event buffer overflow flag
  AcquMk2Info_t* fExpInfo;       // experimental info from header
  ModuleInfoMk2_t* fModInfo;     // experimental hardware modules
 public:
  TA2TAPSMk2Format(Char_t*, Int_t, Int_t=0);
  ~TA2TAPSMk2Format(){}
  
  virtual void Initialise( void*, TA2RingBuffer*, TA2RingBuffer*, UInt_t );
  //  virtual void SendData();			   // process data buffer
  virtual void SaveMk1Event( );		           // construct ACQU event
  virtual void MarkTAPSEvent();                    // mark single TAPS event
  virtual void SavePartialEvent();                 // save incomplete event
  virtual void MergeHeader(void*, void*, Int_t, Int_t);// ACQU header merging
  virtual Int_t GetEventLength( void* );
  virtual void UpdateInfo( ){
    fModInfo = (ModuleInfoMk2_t*)(fExpInfo + 1);
  }

  virtual Bool_t IsHeadBuff( void* b ){     // Header buffer ?
    if( *(UInt_t*)b == EHeadBuff ) return ETrue;
    else return EFalse;
  }
  virtual Bool_t IsDataBuff( void* b ){  // Data buffer ?
    if( *(UInt_t*)b == EDataBuff ) return ETrue;
    else return EFalse;
  }
  virtual Bool_t IsEndBuff( void* b ){   // Trailer buffer ?
    if( *(UInt_t*)b == EEndBuff ) return ETrue;
    else return EFalse;
  }
  virtual Bool_t IsTermBuff( void* b ){  // Terminator buffer ?
    if( *(UInt_t*)b == EKillBuff ) return ETrue;
    else return EFalse;
  }

  ClassDef(TA2TAPSMk2Format,1)       	// for ROOT
};

//---------------------------------------------------------------------------
inline void TA2TAPSMk2Format::SaveMk1Event( )
{
  // Convert TAPS event to standard (Mk1) Acqu Event and save in output
  // buffer
}

//---------------------------------------------------------------------------
inline void TA2TAPSMk2Format::SavePartialEvent( )
{
  // Where TAPS event overflows buffer, save the portion in the current buffer
  // to add to the tail from the next event
}

//---------------------------------------------------------------------------
inline void TA2TAPSMk2Format::MarkTAPSEvent( )
{
  // Mark the start and stop of a TAPS event
}

//-----------------------------------------------------------------------------
inline Int_t TA2TAPSMk2Format::GetEventLength( void* evbuff )
{
  // Identify event's - worth of data and return event length (# of bytes )
  Int_t length = 0;
  UInt_t* event = (UInt_t*)evbuff;
  if( fEventIndex == *(UShort_t*)(event) )
    fEventID = *(UShort_t*)( (UShort_t*)(event) + 1 );
  else fEventID = 0xffff;

  while( *event != EEndEvent ){
    length++;    event++;
  }
  return (length * sizeof(UInt_t));
}

#endif
