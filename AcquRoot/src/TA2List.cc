//--Author	JRM Annand   16th Jan 2005
//--Update	JRM Annand
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TTA2List
// Wrapper class for linked lists of standard variables..Char_t*, Int_t,
// Double_t
// Under development
//

#include "TA2List.h"

ClassImp(TA2List)

//-----------------------------------------------------------------------------
TA2List::TA2List( const char* name )
  :TNamed(name, "AcquRoot-List")
{
  fList = new TList;
}

//-----------------------------------------------------------------------------
TA2List::~TA2List()
{
  // Free up allocated memory
}

template <typename T> void TA2List::Add( T var )
{
  TA2ListVar* v = new TA2ListVar( var );
  fList->AddLast( v );
}

template <typename T> T TA2List::Next()
{
}
