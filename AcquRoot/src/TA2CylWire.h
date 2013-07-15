//--Author	V Lisin      28th Jun 2004  original DAPHNE fortran -> C
//--Rev 	JRM Annand... 8th Jul 2004  AcquRoot C++ class
//--Update	JRM Annand...31st Mar 2005  fPhiCor = layerparm[]
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2CylWire
//
// Helical cathode strips on a cylindrical surface
//

#ifndef __TA2CylWire_h__
#define __TA2CylWire_h__

#include "TA2WCLayer.h"

class TA2CylWire : public TA2WCLayer {
protected:
  Double_t fRadius;                // radius mm
  Double_t fLength;                // length mm
  Double_t fPhiCor[3];               // z correction coeffs
  Double_t fPhiSpace;              // phi spacing of wires
public:
  TA2CylWire( const char*, Int_t, Int_t, Int_t, void*, Double_t*  );
  //  virtual void CGCluster( Int_t );
  virtual void CGCluster( Int_t );
  virtual Double_t GetRadius(){ return fRadius; }
  virtual Double_t GetLength(){ return fLength; }
  virtual Double_t* GetPhiCor(){ return fPhiCor; }
  virtual Double_t GetPhiCor(Int_t i){ return fPhiCor[i]; }

  ClassDef(TA2CylWire,1)
    
};

//---------------------------------------------------------------------------
inline void TA2CylWire::CGCluster( Int_t ic )
{
  // Center of gravity of cluster (index ic) of adjacent hit wires
  // No pulse height to weight the distribution.
  // Convert wire # to phi

  Double_t phiCG = fPhiSpace * ( fClust[ic] + 0.5*(fLenClust[ic] - 1) );
  Double_t twoPi = TMath::TwoPi();
  phiCG += fPhiCor[0];
  if( phiCG >= twoPi ) phiCG -= twoPi;
  else if( phiCG < 0 ) phiCG += twoPi;
  //  fCGClust[ic] = phiCG + fPhiCor[0];
  fCGClust[ic] = phiCG;
}


#endif
