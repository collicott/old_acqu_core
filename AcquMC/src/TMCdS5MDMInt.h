//--Author	JRM Annand   17th Mar 2007   Separate integrand class
//--Rev         JRM Annand
//--Rev         JRM Annand    5th May 2008   Debug YComp()
//--Rev         JRM Annand   25th Jun 2008   General weighting density fn.
//--Update      JRM Annand   30th Jun 2008   Extra dimension for pion phi

//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TMCdS5MDMInt
//
// This provides a density function for the TFoam N-dimensional
// Monte Carlo generator.
// The default density evaluation method is by linear interpolation of
// an N-dimension function (e.g an N-fold differential cross section)
// which is evaluated on a regular grid of points
// Interpolation method:
// Multidimensional Interpolation Methods
// InterpMethods.pdf version 04/09/2006, K.C. Johnson

#ifndef __TMCdS5MDMInt_h__
#define __TMCdS5MDMInt_h__

#include "TMCFoamInt.h"

//-----------------------------------------------------------------------------
class TMCdS5MDMInt : public TMCFoamInt {
 protected:
  Int_t fIJKoffset;                     // to access different data base
  Int_t fDataSize;
  Double_t *fPSO;                       // Array of Evaluated observables
  virtual Int_t IJK( Int_t* );
  virtual void GenPSO();
 public:
  TMCdS5MDMInt(Int_t, Double_t*, TA2System*, Int_t = EDensLinear);
  virtual ~TMCdS5MDMInt();
  virtual void ReadData( Char_t* );
  virtual Double_t Density( Int_t, Double_t* );
  ClassDef(TMCdS5MDMInt,1)   
};

//-----------------------------------------------------------------------------
inline Double_t TMCdS5MDMInt::Density(Int_t nDim, Double_t *Xarg)
{
  // Density distribution for TFoam
  // Default if linear interpolation on a regular N-dim grid
  Scale( Xarg, fXscaled );
  GenPSO();
  Double_t phi = fXscaled[5];
  Double_t cos2phi = TMath::Cos( 2*phi );
  Double_t result = fPSO[0]*(1 - fPolB->X()*fPSO[1]*cos2phi);

  // weighting of output
  return WgtDensity( result );
}

//-----------------------------------------------------------------------------
inline Int_t TMCdS5MDMInt::IJK(Int_t* mj)
{
  // Convert N-dimensional i,j,k.... to an equivalent 1-D index
  Int_t coeff = 1;
  Int_t j1 = mj[fNDim-1];
  for( Int_t j=fNDim-1; j>0; j-- ){
    coeff *= fSN[j];
    j1 += mj[j-1]*coeff;
  }
  return j1 + fIJKoffset;
}

//-----------------------------------------------------------------------------
inline void TMCdS5MDMInt::GenPSO()
{
  // Interpolate values of cross section + polarisation observables for given
  // Energies and Angles. Interpolation always in 5D
  Int_t nd = fNDim;
  fNDim = 5;
  for(Int_t i=0; i<fNInterp; i++){
    fIJKoffset = fDataSize * i;
    switch( fDensOpt ){
    case EDensLinear:
    default:
      // multi-linear interpolation
      fPSO[i] =  LinInterpN(fXscaled);
      break;
    case EDensLinearC:
      // multi-linear interpolation with compensation
      fPSO[i] = LinInterpN(fXscaled,kTRUE);
      break;
    }
  }
  fNDim = nd;
}


#endif
