//----------------------------------------------------------------------------
//			Acqu-Root++
//		Data Acquisition and Analysis
//			for
//		Nuclear Physics Experiments
//
//		    *** TD2A.h ***

//--Description
//	TD2A
//	Digital-to-analogue conversion
//
//--Author	JRM Annand	17th Feb 2003
//--Update	JRM Annand      18th Feb 2004  D2AM_t for multi-hit ADCs
//
//---------------------------------------------------------------------------
#ifndef __TD2A_h__
#define __TD2A_h__

#include "EnumConst.h"
#include "TNamed.h"

class D2A_t{
 protected:
  Double_t fA0;                           // pedestal
  Double_t fA1;                           // conversion gain
  Double_t fA2;                           // non-linear?
  UShort_t* fADC;                         // -> stored ADC value
 public:
  D2A_t( UShort_t* adc, Double_t a0, Double_t a1, Double_t a2 = 0.0 ){
    // store input parameters
    fADC = adc; fA0 = a0; fA1 = a1; fA2 = a2;
  }
  virtual ~D2A_t(){};
  virtual Double_t D2A(){
    if( *fADC == (UShort_t)ENullADC ) return (Double_t)ENullADC;
    // digital to analogue
    Double_t value = (Double_t)(*fADC);
    value -= fA0;
    if( !fA2 )return fA1*value;
    else return fA1*value + fA2*value*value;
  }
  UShort_t* GetADC(){ return fADC; }
  UShort_t GetADCValue(){ return *fADC; }
  Double_t GetA0(){ return fA0; }
  Double_t GetA1(){ return fA1; }
  Double_t GetA2(){ return fA2; }
};

class D2AM_t : public D2A_t
{
 private:
 public:
  D2AM_t( UShort_t* adc, Double_t a0, Double_t a1, Double_t a2 = 0.0 ):
    D2A_t( adc, a0, a1, a2 ){
    // store input parameters
  }
  virtual ~D2AM_t(){};
  virtual Double_t D2A(){
    Short_t* madc = (Short_t*)fADC;
    if( *madc == (Short_t)ENullStore ) return (Double_t)ENullADC;
    // digital to analogue
    Double_t value = (Double_t)(*madc);
    value -= fA0;
    if( !fA2 )return fA1*value;
    else return fA1*value + fA2*value*value;
  }
};

#endif
