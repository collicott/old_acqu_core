//--Author	JRM Annand	12th Jan 2004   Convert Acqu++ -> AcquRoot
//--Rev 	JRM Annand       5th Mar 2004   Use standard data buffer decode
//--Rev 	JRM Annand      22nd Apr 2004   TAPS MergeHeader
//--Rev 	JRM Annand       5th Nov 2006   nsrc
//--Update	JRM Annand...    3rd Jun 2008...const Char_t*...gcc 4.3
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2TAPSFormat
// Data format specification for TAPS @ CB-MAMI 2004
//
//---------------------------------------------------------------------------

#ifndef _TA2TAPSFormat_h_
#define _TA2TAPSFormat_h_

#include "TA2DataFormat.h"

class TA2TAPSFormat : public TA2DataFormat {
 protected:
  Bool_t fIsPartEvent;          // event buffer overflow flag
 public:
  TA2TAPSFormat(Char_t*, Int_t, Int_t=0);
  ~TA2TAPSFormat(){}
  
  virtual void Initialise( void*, TA2RingBuffer*, TA2RingBuffer*, UInt_t );
  //  virtual void SendData();			   // process data buffer
  virtual void SaveMk1Event( );		           // construct ACQU event
  virtual void MarkTAPSEvent();                    // mark single TAPS event
  virtual void SavePartialEvent();                 // save incomplete event
  virtual void MergeHeader(void*, void*, Int_t, Int_t);// ACQU header merging
  virtual Int_t GetEventLength( void* );

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

  ClassDef(TA2TAPSFormat,1)       	// for ROOT
};

//---------------------------------------------------------------------------
inline void TA2TAPSFormat::SaveMk1Event( )
{
  // Convert TAPS event to standard (Mk1) Acqu Event and save in output
  // buffer
}

//---------------------------------------------------------------------------
inline void TA2TAPSFormat::SavePartialEvent( )
{
  // Where TAPS event overflows buffer, save the portion in the current buffer
  // to add to the tail from the next event
}

//---------------------------------------------------------------------------
inline void TA2TAPSFormat::MarkTAPSEvent( )
{
  // Mark the start and stop of a TAPS event
}

//-----------------------------------------------------------------------------
inline Int_t TA2TAPSFormat::GetEventLength( void* evbuff )
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
