//--Author	V Lisin      28th Jun 2004  original DAPHNE fortran -> C
//--Rev 	JRM Annand... 8th Jul 2004  AcquRoot C++ class
//--Update	JRM Annand...31st Mar 2005  fPhiCor mod
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2CylStrip
//
// Helical cathode strips on a cylindrical surface
//

#ifndef __TA2CylStrip_h__
#define __TA2CylStrip_h__

#include "TA2WCLayer.h"

class TA2CylStrip : public TA2WCLayer {
protected:
  Double_t fRadius;                // radius mm
  Double_t fTgWC;                  // tan helix angle
  Double_t fZ0;                    // along beam axis
  Double_t fPitch;                 // strip pitch mm
  Double_t fLength;                // length mm
  Double_t fEffLength;             // effective 1/2 length mm
  Double_t fZCor[3];               // z correction coeffs
  Double_t fPhiCor[3];             // phi correction coeffs
public:
  TA2CylStrip( const char*, Int_t, Int_t, Int_t, void*, Double_t*  );
  virtual void CGCluster( Int_t );
  virtual Bool_t IsInside( Double_t );
  virtual Double_t GetRadius(){ return fRadius; }
  virtual Double_t GetTgWC(){ return fTgWC; }
  virtual Double_t GetZ0(){ return fZ0; }
  virtual Double_t GetPitch(){ return fPitch; }
  virtual Double_t GetLength(){ return fLength; }
  virtual Double_t GetEffLength(){ return fEffLength; }
  virtual Double_t* GetZCor(){ return fZCor; }
  virtual Double_t GetZCor(Int_t i){ return fZCor[i]; }
  virtual Double_t* GetPhiCor(){ return fPhiCor; }
  virtual Double_t GetPhiCor(Int_t i){ return fPhiCor[i]; }

  ClassDef(TA2CylStrip,1)
};

//---------------------------------------------------------------------------
inline void TA2CylStrip::CGCluster( Int_t ic )
{
  // Amplitude-weighted Center of gravity of a cluster of adjacent hit strips
  // where fEnergy records the induced amplitude on each strip. 
  // Returns number in range 0.0 - 1.0

  Double_t esum = 0.0;
  Double_t wesum = 0.0;
  Int_t istart = fClust[ic];                    // start cluster index
  Int_t iend = istart + fLenClust[ic];          // end cluster index
  Int_t j;

  for( Int_t i=istart; i<iend; i++ ){
    j = i + fIstart;
    if( fEnergy[j] == (Double_t)ENullHit ) continue;
    esum += fEnergy[j];
    wesum += i*fEnergy[j];
  }
  // Energy weighted "index"....check its within bounds and save
  wesum = wesum/(esum * fNElement) + fPhiCor[0];
  if( wesum >= 1.0 ) wesum -= 1.0;
  if( wesum < 0 ) wesum += 1.0;
  fCGClust[ic] = wesum;
}

//---------------------------------------------------------------------------
inline Bool_t TA2CylStrip::IsInside( Double_t z )
{
  // Is Z coordinate sensible for this layer
  if( z < -fEffLength ) return EFalse;
  if( z > fEffLength ) return EFalse;
  return ETrue;
}

#endif
