//--Author	JRM Annand    5th Feb 2004
//--Update	JRM Annand   14th Apr 2005 GetCutList
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2MultiCut
//
// Multiple-condition data cut....
// ie a container whereby a chain of single 1D or 2D conditions
// may be implemented and logically joined via NOT/AND/OR links
// 
#ifndef __TA2MultiCut_h__
#define __TA2MultiCut_h__

#include "TA2Cut.h"
#include "TList.h"

class TA2MultiCut : public TA2Cut {
 private:
 protected:
  TList* fCutList;
  
 public:
  TA2MultiCut( const char*, UInt_t = ECutOR );
  virtual ~TA2MultiCut();
  virtual void AddCut( TA2Cut* cut ){ fCutList->AddLast(cut); }
  virtual TA2Cut* FindCut( const char* name ){
    return (TA2Cut*)fCutList->FindObject( name );
  }
  virtual Bool_t Test();
  virtual void Evaluate();
  virtual void Cleanup();
  virtual TList* GetCutList(){ return fCutList; }

  ClassDef(TA2MultiCut,1)
};

//-----------------------------------------------------------------------------
inline Bool_t TA2MultiCut::Test( )
{
  // Step through list of cuts and call the condition
  // function to determine whether the datum passes the
  // cut or not.
  // Linkage of the cut conditions is determined by
  // GetLink() method

  TIter nextcut( fCutList );
  TA2Cut* cut;
  Bool_t result = ETrue;;

  while( ( cut = (TA2Cut*)nextcut() ) ){
    result = cut->Test();
    switch( cut->GetLink() ){
    case ECutOR:
      // OR..return true if result = true
      if( result ) goto breakmulti;
      break;
    case ECutAND:
      // AND..return false if result = false
      if( !result ) goto breakmulti;
      break;
    }
  }
 breakmulti:
  // negate result if NOT flag set
  if( fIsNOT ) result = kNOT[result];
  fIsTrue = result;
  return fIsTrue;;
}

//-----------------------------------------------------------------------------
inline void TA2MultiCut::Evaluate( )
{
  // Evaluate condition for each cut in list

  TIter nextcut( fCutList );
  TA2Cut* cut;
  //  Bool_t result;

  while( ( cut = (TA2Cut*)nextcut() ) ){
    //    result = cut->Test();
    cut->Test();
  }
}

//-----------------------------------------------------------------------------
inline void TA2MultiCut::Cleanup( )
{
  // Cleanup each cut in list and this multicut

  TIter nextcut( fCutList );
  TA2Cut* cut;

  while( ( cut = (TA2Cut*)nextcut() ) ) cut->Cleanup();
  fIsTrue = EFalse;
}

#endif
