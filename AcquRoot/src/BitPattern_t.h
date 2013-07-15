//--Author	JRM Annand   22nd Jun 2004
//--Rev 	JRM Annand...
//--Update	JRM Annand...29th sep 2012 Getter for fNHits
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2BitPattern
// Decode bit-pattern-unit output to create a hit pattern
//

#ifndef __TA2BitPattern_h__
#define __TA2BitPattern_h__

#include "TA2System.h"

class TA2BitPattern : public TA2System{
private:
  UShort_t** fADClist;                  // element Q/VDC  & conversion gain
  UShort_t* fADC;
  UShort_t* fPatternList;
  Int_t* fHits;
  Int_t fNADC;
  Int_t fNadc;
  Int_t fNelement;
  Int_t fNelem;
  Int_t fNHits;
public:
  TA2BitPattern( char* );
  virtual ~TA2BitPattern();
  virtual Bool_t Decode();
  UShort_t** GetADClist(){ return fADClist; }
  Int_t* GetHits(){ return fHits; }
  Int_t GetNHits(){ return fNHits; }
  ClassDef(TA2BitPattern,1)
};

//---------------------------------------------------------------------------
inline Bool_t TA2BitPattern::Decode( )
{
  // Build pattern from pattern ADCs
  // Apply window cuts and return true/false to cut decision

  UShort_t datum;
  Int_t chan;
  for( Int_t i=0; i<fNadc; i++ ){
    datum = *fADClist[i];
    if( datum == (UShort_t)ENullADC ) continue;
    k = i << 4
    for( Int_t j=0; j<16; j++ ){
      chan = fPatternList[k++];
      if( (datum & 1) )
      fHits[fNHits++] = p;
    }
      
}
#endif
