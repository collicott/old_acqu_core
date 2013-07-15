//--Author	JRM Annand	12th Jan 2004  For Acqu-Root DataServer
//--Rev 	JRM Annand       6th Feb 2007  Add Update(Char_t*, Int_t)
//--Rev 	JRM Annand      26th Mar 2007  Add buffer empty flag
//--Rev 	JRM Annand      29th May 2007  Add ResetBuffPtr()
//--Rev 	JRM Annand      12th Sep 2010  Update...consider trailer word
//--Update	JRM Annand       7th Jan 2013  SetHeader function
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
//  TA2RingBuffer
//  I/O buffers for Acqu-Root data transfer.
//  The buffers are arranged in a doubly linked list where the last
//  wraps round to the 1st for "seamless" operation
//
//---------------------------------------------------------------------------

#ifndef _TA2RingBuffer_h_
#define _TA2RingBuffer_h_

#include "Rtypes.h"
#include "EnumConst.h"
#include "TCondition.h"                 // thread synch
#include "TMutex.h"

struct BuffLink_t
{
  void* fStore;                         // ptr to memory store
  void* fCurPos;                        // current position in memory
  BuffLink_t* fPrevious;                // ->previous buffer
  BuffLink_t* fNext;                    // ->next buffer
  Bool_t fIsFull;                       // memory store full flag
  Bool_t fIsEmpty;                      // memory store empty flag
};

class TA2RingBuffer{
protected:
  BuffLink_t* fBuffer;                  // array of buffer structs
  BuffLink_t* fCurrent;                 // pointer to current buffer
  TMutex* fSourceMutex;                 // synchronisation mechanism
  TMutex* fServerMutex;                 // ditto
  TCondition* fSourceCond;              // data source signal cond
  TCondition* fServerCond;              // data server signal cond
  UInt_t fLenBuff;                      // length of each buffer
  UInt_t fNbuff;                        // no. of buffers
	
public:
  // Create & setup constructor
  TA2RingBuffer( UInt_t, UInt_t );
  TA2RingBuffer( TA2RingBuffer* );
  virtual ~TA2RingBuffer();
  virtual void Initialise();
  virtual void Next(){ fCurrent = fCurrent->fNext; }
  virtual void Previous(){ fCurrent = fCurrent->fPrevious; }
  virtual void SetFull(){ fCurrent->fIsFull = ETrue; }
  virtual void Clear(){ fCurrent->fIsFull = EFalse; }
  virtual void* Update(UInt_t);
  virtual void* Update(Char_t*, UInt_t);
  virtual void* Update(Char_t*);
  virtual void SetHeader( UInt_t );
  virtual void Header( UInt_t );
  virtual void Trailer( );
  virtual void ResetBuffPtr( );
  virtual Bool_t ScanAvailable();
  virtual void FNext(){
    fCurrent->fIsFull = ETrue;
    fCurrent->fIsEmpty = EFalse;
    fCurrent = fCurrent->fNext;
    //    if( !fCurrent->fPrevious->fIsFull ) fSourceCond->Signal();
    fSourceCond->Signal();
  }
  virtual void CNext(){
    fCurrent->fIsFull = EFalse;
    fCurrent->fIsEmpty = ETrue;
    fCurrent = fCurrent->fNext;
    //if( fCurrent->fPrevious->fIsFull ) fServerCond->Signal();
    fServerCond->Signal();
  }
  virtual void WaitEmpty(){ if( !fCurrent->fIsEmpty ) fServerCond->Wait(); }
  virtual void WaitFull(){ if( !fCurrent->fIsFull ) fSourceCond->Wait(); }
  // Read-only parameters
  BuffLink_t* GetCurrent( ){ return fCurrent; }    // ->current buff struct
  void* GetStore(){ return fCurrent->fStore; }     // ->current memory store
  void* GetCurPos(){ return fCurrent->fCurPos; }   // current pos in memory
  BuffLink_t* GetNext(){ return fCurrent->fNext; } // ->next buffer in ring
  BuffLink_t* GetPrevious(){ return fCurrent->fPrevious; }
                                                   // ->previous buffer in ring
  TMutex* GetSourceMutex(){ return fSourceMutex; }
  TMutex* GetServerMutex(){ return fServerMutex; }
  TCondition* GetSourceCond(){ return fSourceCond; }
  TCondition* GetServerCond(){ return fServerCond; }
  Bool_t IsFull(){ return fCurrent->fIsFull; }    // memory store full flag
  Bool_t IsEmpty(){ return fCurrent->fIsEmpty; }  // memory store empty
  UInt_t GetLenBuff(){ return fLenBuff; }         // buffer length
  UInt_t GetNbuff(){ return fNbuff; }             // number internal buffers
};

//-----------------------------------------------------------------------------
inline Bool_t TA2RingBuffer::ScanAvailable()
{
  // Search for buffer where the full flag isn't set.
  // If nothing set return to original position

  for( UInt_t i=0; i<=fNbuff; i++ ){
    Next();
    if( !fCurrent->fIsFull ) return ETrue;
  }
  return EFalse;
}

//-----------------------------------------------------------------------------
inline void* TA2RingBuffer::Update(UInt_t size)
{
  // Search for buffer where the full flag isn't set.
  // If nothing set return to original position
  // 12/09/10 consider trailer word when calc if event fits

  char* pos = (char*)(fCurrent->fCurPos) + size;
  if( pos >= (char*)(fCurrent->fStore) + fLenBuff - sizeof(UInt_t) )
    return NULL;
  fCurrent->fCurPos = pos;
  fCurrent->fIsEmpty = EFalse;
  return pos;
}

//-----------------------------------------------------------------------------
inline void* TA2RingBuffer::Update(Char_t* buff, UInt_t size)
{
  // Write size bytes from buff to current buffer.
  // Check if there is space available

  char* pos = (Char_t*)(fCurrent->fCurPos) + size;
  if( pos > (Char_t*)(fCurrent->fStore) + fLenBuff - sizeof(UInt_t) )
    return NULL;
  memcpy( fCurrent->fCurPos, buff, size );
  fCurrent->fCurPos = pos;
  fCurrent->fIsEmpty = EFalse;
  Trailer();
  return pos;
}

//-----------------------------------------------------------------------------
inline void* TA2RingBuffer::Update(Char_t* buff)
{
  // Write size bytes from buff to current buffer.
  // Check if there is space available

  char* pos = (Char_t*)(fCurrent->fStore) + fLenBuff;
  memcpy( fCurrent->fStore, buff, fLenBuff );
  fCurrent->fCurPos = pos;
  fCurrent->fIsEmpty = EFalse;
  return pos;
}

//-----------------------------------------------------------------------------
inline void TA2RingBuffer::SetHeader(UInt_t header)
{
  // Write new buffer header...e.g. change data buff to end buff
  UInt_t* h = (UInt_t*)fCurrent->fStore;
  *h = header;
}

//-----------------------------------------------------------------------------
inline void TA2RingBuffer::Header(UInt_t header)
{
  // Write buffer header and default end markers to buffer
  // Set buffer position pointer to just after header

  UInt_t* h = (UInt_t*)fCurrent->fStore;
  *h++ = header;
  fCurrent->fCurPos = h;          // place buffer pointer after header
  *h++ = EEndEvent;               // write end event
  *h = EBufferEnd;                // and end buffer marker just in case
}

//-----------------------------------------------------------------------------
inline void TA2RingBuffer::Trailer()
{
  // Write end marker at current buffer position
  // Do not update position pointer

  UInt_t* h = (UInt_t*)fCurrent->fCurPos;
  *h = EBufferEnd;
}  

//-----------------------------------------------------------------------------
inline void TA2RingBuffer::ResetBuffPtr()
{
  // reset the buffer pointer
  //  fCurrent->fCurPos = (UInt_t*)(fCurrent->fStore) + 1;
  fCurrent->fCurPos = fCurrent->fStore;
}
#endif
