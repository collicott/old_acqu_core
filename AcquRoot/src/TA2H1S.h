//----------------------------------------------------------------------------
//			Acqu-Root++
//		Data Acquisition and Analysis
//			for
//		Nuclear Physics Experiments
//
//		    *** TA2H1S.h ***

//--Description
//	TA2H1S
// Wrap the data buffer containing values to be histogramed
// in an inherited class of TH1F. Different data buffer types
// handled by template.
// This one assumes single values to be histogramed. 
//
//--Author	JRM Annand	22nd Feb 2003
//--Update	JRM Annand
//
//---------------------------------------------------------------------------
#ifndef __TA2H1S_h__
#define __TA2H1S_h__

#include "TH1F.h"
#include "EnumConst.h"                              // Acqu emumerations

template< typename T > class TA2H1S : public TH1F {
private:
  T* fHistBuff;
public:
  TA2H1S(){}
  TA2H1S( char*, const char*, Int_t, Axis_t, Axis_t, T* );
  TA2H1S( char*, const char*, Int_t, Double_t*, T* );

  virtual ~TA2H1S();
  void Fill(){ TH1::Fill( (Float_t)(*fHistBuff) ); }
  T* GetHistBuff(){ return fHistBuff; }     // return data buffer ptr
   // 
  ClassDefT(TA2H1S,1)   
};

#endif

