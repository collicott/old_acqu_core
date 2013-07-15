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

ifndef __TA2List_h__
#define __TA2List_h__

#include "TNamed.h"

templete <typename T> class TA2ListVar : public TObj {
 private:
  T fVar;
 public:
  TA2ListVar( T var ){ fVar = var; }
  virtual ~TA2ListVar(){}
};

templete <typename T> class TA2List : public TNamed {
 private:
  TList* fList;
  TIter fNextVar(
  T fVar;
 public:
  TA2List( const char* );
  virtual ~TA2List();
  virtual void Add( T );
  virtual T Next();
 
  ClassDef(TA2List,1)
};

#endif
