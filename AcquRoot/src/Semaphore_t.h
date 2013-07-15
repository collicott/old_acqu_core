
//--Author	JRM Annand	14th Jan 2004
//--Rev
//--Update
//
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data
//
// Semaphore_t
// Wrapper class for handling POSIX semaphores
// POSIX 1003.1b implementation on Linux 2.4
// For inter-thread synchronisation
//---------------------------------------------------------------------------

#ifndef _Semaphore_t_h_
#define _Semaphore_t_h_

#include <semaphore.h>
#include "Rtypes.h"

enum { EServerSem, ESortSem, ESourceSem };

class Semaphore_t {
private:
  sem_t *fSem;                          // array of POSIX semaphores
  Int_t fNsem;                          // # semaphores in array
public:
  Semaphore_t( Int_t nsem ){  
      // Initialise array of POSIX semaphores
      fNsem = nsem;
      fSem = new sem_t[nsem];
      for( Int_t i=0; i<nsem; i++ ){
	if( sem_init( fSem+i, 0, 0 ) )
	  printf(" Error initialising POSIX semaphore %d \n\n", i);
      }
  }
  virtual ~Semaphore_t(){
  // Delete array of POSIX semaphores
    for( Int_t i=0; i<fNsem; i++ ){
      if( sem_destroy( fSem+i ) )
	printf(" Error deleting POSIX semaphore %d \n\n", i);
    }
    delete fSem;
  }
  void Signal( Int_t );		        // semaphore signal
  void Clear( Int_t );		        // clear the semaphore
  void Wait( Int_t );		        // wait for semaphore set
  void WaitClear( Int_t );		// wait for set and then clear
  void HandShake( Int_t, Int_t );	// signal and wait for response
  Bool_t Test( Int_t );		        // test state of semaphore
  Int_t GetNsem(){ return fNsem; }	// get number of semphores
};

//---------------------------------------------------------------------------
inline void Semaphore_t::Signal( Int_t nsem )
{
  // Increment semaphore count to signal operation done

  if( sem_post( fSem+nsem ) )
    printf(" Error posting POSIX semaphore %d \n\n", nsem);
}
//---------------------------------------------------------------------------
inline void Semaphore_t::Clear( Int_t nsem )
{
  // Clear (re-initialise) semaphore

  if( sem_init( fSem+nsem, 0, 0 ) )
    printf(" Error clearing POSIX semaphore %d \n\n", nsem);
}
//---------------------------------------------------------------------------
inline void Semaphore_t::Wait( Int_t nsem )
{
  // Wait for semphore to be signaled
  // sem_wait always decrements the semaphore count
  // so here its posted to reincrement
  sem_wait( fSem+nsem );
  if( sem_post( fSem+nsem ) )
    printf(" Error resetting POSIX semaphore %d \n\n", nsem);
}
//---------------------------------------------------------------------------
inline void Semaphore_t::WaitClear( Int_t nsem )
{
  // Wait for semphore to be signaled
  // sem_wait always decrements the semaphore count (to zero)
  sem_wait( fSem+nsem );
}
//---------------------------------------------------------------------------
inline void Semaphore_t::HandShake( Int_t nsem1, Int_t nsem2 )
{
//
// Signal with nsem1 and then
// wait for a response on nsem2 (auto clear nsem2 when done)

  if( sem_post( fSem+nsem1 ) )
    printf(" Error posting POSIX semaphore %d \n\n", nsem1);
  if( sem_wait( fSem+nsem2 ) )
    printf(" Error waiting for POSIX semaphore %d \n\n", nsem2);
}
//---------------------------------------------------------------------------
inline Bool_t Semaphore_t::Test( Int_t nsem )
{
//
// Test the state of nsem1 and return True/Flase if its 1/0

  Int_t value;
  sem_getvalue( fSem+nsem, &value );
  return (Bool_t)value;
}

#endif
