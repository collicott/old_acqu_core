//--Author	JRM Annand    1st Dec 2004
//--Update	
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2GenericApparatus
//
// Generic apparatus class which may be instantiated
//

#ifndef __TA2GenericApparatus_h__
#define __TA2GenericApparatus_h__

#include "TA2Apparatus.h"

enum{ EMaxGenericDetectors = 16 };

class TA2GenericApparatus : public TA2Apparatus {
 protected:
  TA2Detector** fDet;      // list of Pointers to the detector classes
  Int_t fNDet;             // number of detectors
 public:
  TA2GenericApparatus( const char*, TA2System* ); // construct called by TAcquRoot
  virtual ~TA2GenericApparatus();
  virtual TA2DataManager* CreateChild( const char*, Int_t );
  virtual void PostInit( );                // finish off initialisation
  virtual void DeleteArrays( );
  virtual void Reconstruct( );
  virtual void Cleanup();                  // reset at end of event
  
  ClassDef(TA2GenericApparatus,1)
};

//-----------------------------------------------------------------------------
inline void TA2GenericApparatus::Cleanup( )
{
  // Clear any event-by-event variables
 TA2DataManager::Cleanup();
}
    

#endif
