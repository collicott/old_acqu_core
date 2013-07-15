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

#include "TA2RingBuffer.h"

//ClassImp(TA2RingBuffer)

//-----------------------------------------------------------------------------
TA2RingBuffer::TA2RingBuffer( UInt_t length, UInt_t nbuff )
{
  // Allocate memory for data buffers and setup circular linked list
  // to access them in sequence

  fLenBuff = length;                         // Buffer length
  fNbuff = nbuff;                            // number of buffers
  fBuffer = new BuffLink_t[nbuff];               // array of buffers
  for( UInt_t i=0; i<nbuff; i++ ){
    fBuffer[i].fStore = new char[length];   // memory for data buffer
  }
  // create synchronisation conditions
  fSourceMutex = new TMutex();
  fSourceCond = new TCondition(fSourceMutex);
  fServerMutex = new TMutex();
  fServerCond = new TCondition(fServerMutex);

  Initialise();                              // setup the linked list
}

//-----------------------------------------------------------------------------
TA2RingBuffer::TA2RingBuffer( TA2RingBuffer* rbuff )
{
  // Make a cloan of the buffer
  //
  *this = *rbuff;
}

//-----------------------------------------------------------------------------
TA2RingBuffer::~TA2RingBuffer()
{
  // Flush all allocated memory
  //
  for( UInt_t i=0; i<fNbuff; i++ ) delete (char*)(fBuffer[i].fStore);
  delete fBuffer;
}

//-----------------------------------------------------------------------------
void TA2RingBuffer::Initialise()
{
  // Reset the buffer (BuffLink_t) linked list

  UInt_t i,j;
  for( i=0; i<fNbuff; i++ ){
    if( i == fNbuff-1 ) j = 0;
    else j = i+1;
    fBuffer[i].fNext = fBuffer+j;             // pointer to next buffer
    if( i == 0 ) j = fNbuff-1;
    else j = i-1;
    fBuffer[i].fPrevious = fBuffer+j;         // pointer to previous buffer
    fBuffer[i].fCurPos = fBuffer[i].fStore;   // position at start
    fBuffer[i].fIsFull = EFalse;              // Flag as useable
    fBuffer[i].fIsEmpty = ETrue;              // Flag as useable
  }
  fCurrent = fBuffer;                         // Start at the 0th element

}


