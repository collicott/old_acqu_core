//--Author	JRM Annand    5th Feb 2004
//--rev 	JRM Annand   31st Mar 2004 Scaler type enum
//--Update	JRM Annand   27th Jun 2004 Rate type enum
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2_N2V
//
// Simple container to associate a name (string) with the pointer
// to a variable. Info on the type of variable is stored
//
#ifndef __TA2_N2V_h__
#define __TA2_N2V_h__

#include "TObject.h"

// Variable type designators
// Single/Multi value X/Y variables, Shape = pulse form
enum { ESingleX, EMultiX, ESingleY, EMultiY, EMultiXY,
       EShapeX, EScalerX, ERateX};
// Double versions of above
enum { EDSingleX=ESingleX, EDMultiX=EMultiX, EDSingleY=ESingleY, 
       EDMultiY=EMultiY, EDMultiXY=EMultiXY, EDShapeX=EShapeX,
       EDScalerX = EScalerX, EDRateX = ERateX};
// Integer versions of above
enum { EIntOffset = 1000 };
enum { EISingleX=EDSingleX+EIntOffset, EIMultiX=EDMultiX+EIntOffset, 
       EISingleY=EDSingleY+EIntOffset, EIMultiY=EDMultiY+EIntOffset,
       EIMultiXY=EDMultiXY+EIntOffset, EIShapeX=EDShapeX+EIntOffset,
       EIScalerX=EDScalerX+EIntOffset, EIRateX=EDRateX+EIntOffset};
template<class T> class TA2_N2V : public TObject
{
  // Associate address of variable  and variable type
  // with character string
 protected:
  const char* fName;                // name of variable
  T* fVar;                          // address of variable
  Int_t fType;                      // single/multi value variable
  Bool_t fIsInt;                    // is it an integer
 public:
  TA2_N2V( const char* name, T* var, Int_t type )
  {
    // Store name, pointer, and type. Determine if integer or double
    fName = name; fVar = var;
    if( type >= EIntOffset ){
      type -= EIntOffset;
      fIsInt = ETrue;
    }
    else fIsInt = EFalse;
    fType = type; 
  }
  const char* GetName(){ return fName; }
  T* GetVar(){ return fVar; }
  Int_t GetType(){ return fType; }
  Bool_t IsInt(){ return fIsInt; }
};


#endif
