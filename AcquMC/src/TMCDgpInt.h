//--Author	JRM Annand   10th Jul 2008   New class
//--Rev         JRM Annand
//--Update      JRM Annand

//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TMCDgpInt
//
// This provides a 2D density function for the TFoam N-dimensional
// Monte Carlo generator.
// Calculate 2-Body CMS d(g,p) differential cross section.
// Use parametrisation of Rossi et al, PR C40(1989),2412
// or Jenkins et al, PR C50(1994),74
// Foam vector elements are:
// 0  Egamma
// 1  Theta proton
// 2  Phi proton (polarisation not yet implemented

#ifndef __TMCDgpInt_h__
#define __TMCDgpInt_h__

#include "TMCFoamInt.h"

enum { EMaxLegPol = 8 };
enum { EDgpJenkins, EDgpRossi };
enum { EDgpThProton, EDgpThNeutron };

//-----------------------------------------------------------------------------
class TMCDgpInt : public TMCFoamInt {
 protected:
 public:
  TMCDgpInt(Int_t, Double_t*, TA2System*, Int_t = EDensLinear);
  virtual ~TMCDgpInt();
  virtual Double_t Dgp(Double_t, Double_t, Int_t = EDgpJenkins);
  virtual Double_t Density( Int_t, Double_t* );
  virtual void ReadData( Char_t* );
  ClassDef(TMCDgpInt,1)   
};

//-----------------------------------------------------------------------------
inline Double_t TMCDgpInt::Density(Int_t nDim, Double_t *Xarg)
{
  // Density distribution for TFoam
  // Default if linear interpolation on a regular N-dim grid
  Scale( Xarg, fXscaled );
  Double_t result = Dgp( fXscaled[0], fXscaled[1] );
 // weighting of output
  return WgtDensity( result );
}

#endif
