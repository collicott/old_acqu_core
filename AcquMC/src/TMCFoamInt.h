//--Author	JRM Annand   17th Mar 2007   Separate integrand class
//--Rev         JRM Annand
//--Rev         JRM Annand    5th May 2008   Debug YComp()
//--Rev         JRM Annand   25th Jun 2008   General weighting density fn.
//--Rev         JRM Annand   30th Jun 2008   ReadData move from TMCFoamGenerator
//--Update      JRM Annand   17th Sep 2008   SetWgt() extended
//
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TMCFoamInt
//
// This provides a density function for the TFoam N-dimensional
// Monte Carlo generator.
// The default density evaluation method is by linear interpolation of
// an N-dimension function (e.g an N-fold differential cross section)
// which is evaluated on a regular grid of points
// Interpolation method:
// Multidimensional Interpolation Methods
// InterpMethods.pdf version 04/09/2006, K.C. Johnson

#ifndef __TMCFoamInt_h__
#define __TMCFoamInt_h__

#include "TFoamIntegrand.h"
#include "TA2System.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TLorentzRotation.h"

// For switching to higher-order interpolation schemes
enum { EDensLinear, EDensLinearC, EDensCubic, EDensCubicC };
// Polarisation switches
enum { EBeamPol, ETargetPol, ERecoilPol};
// Switches for weighting of density fn.
// default no weight, bremsstrahlung (1/E), phase space sin(theta)
enum { EWgtUnity, EWgtBrem, EWgtSinTh, EWgtBremRes };

//-----------------------------------------------------------------------------
class TMCFoamInt : public TFoamIntegrand {
 protected:
  TA2System* fSys;
  Double_t* fXarg;     // foam variables [0,1] interval
  Double_t* fScale;    // scaling factors for variables
  Double_t* fXscaled;  // scaled variables
  Double_t* fYN;       // evaluated points on grid
  Double_t** fXN;      // grid point values for each dimension
  Double_t* fUj;       // Linear "Cardinal" functions
  Double_t* fXd;       // x[j] - x[fM0[j]]
  TVector3* fPolB;     // Beam pol. Stokes vect. lin 0->90deg,lin -45->45, circ
  TVector3* fPolT;     // Target pol.
  TVector3* fPolR;     // Recoil components should only be set to 1/0
                       // depending on if they should be calculated or not
  TLorentzVector* flRecPol;             // Cal. recoil pol.
  TLorentzRotation* fCMtoLAB;           // CM -> lab for RecPol
  Int_t* fSN;                           // # grid values for each dimension
  Int_t* fM0;                           // nearest-&-less-than grid indices
  Int_t* fM1;                           // fM1[j] = fM0[j] + s, s=0,1
  Int_t* fM2;                           // for linear interp compensation
  Int_t* fIwgt;                         // for weighting interp result
  Int_t fNDim;                          // # dimensions
  Int_t fNInterp;                       // # foam results
  Int_t fDensOpt;                       // switch for density fn evaluation
  Double_t fWgt0;                       // optional parms weighting fn.
  Double_t fWgt1;
  Double_t fWgt2;

  // N-dim interpolation...called by Density()
  virtual void LinIntpAccum( Int_t, Double_t&, Bool_t = kFALSE ); 
  virtual Int_t IJK( Int_t* );
  Double_t GenFn(Double_t, Double_t, Double_t, Double_t, Double_t);
  Double_t YComp();
 public:
  TMCFoamInt(Int_t, Double_t*, TA2System*, Int_t = EDensLinear);
  virtual ~TMCFoamInt();
  virtual void ReadData( Char_t* );
  virtual Double_t LinInterpN( Double_t*, Bool_t = kFALSE );
  virtual Double_t Density( Int_t, Double_t* );
  virtual Double_t WgtDensity( Double_t );
  virtual void Scale( Double_t*, Double_t* );
  virtual void SetPol(Double_t, Double_t, Double_t, Int_t);
  virtual void SetPolB(Double_t x,Double_t y,Double_t z)
  { SetPol(x,y,z,EBeamPol); }
  virtual void SetPolT(Double_t x,Double_t y,Double_t z)
  { SetPol(x,y,z,ETargetPol); }
  virtual void SetPolR(Double_t x,Double_t y,Double_t z)
  { SetPol(x,y,z,ERecoilPol); }
  virtual void SetWgt( Int_t, Int_t, Double_t* = NULL );
  virtual Double_t* GetXarg(){ return fXarg; }
  virtual Double_t* GetScale(){ return fScale; }
  virtual Double_t* GetXscaled(){ return fXscaled; }
  virtual Double_t* GetYN(){ return fYN; }
  virtual Double_t** GetXN(){ return fXN; }
  virtual TVector3* GetPolB(){return fPolB;}
  virtual TVector3* GetPolT(){return fPolT;}
  virtual TVector3* GetPolR(){return fPolR;}
  virtual TLorentzVector* GetlRecPol(){ return flRecPol; }
  virtual TVector3 GetRecPol(){return flRecPol->Vect();}
  virtual TLorentzRotation* GetCMtoLAB(){ return fCMtoLAB; }
  virtual Int_t GetNDim(){ return fNDim; }
  virtual Int_t GetNInterp(){ return fNInterp; }
  virtual Int_t* GetIwgt(){ return fIwgt; }
  virtual Int_t GetDensOpt(){ return fDensOpt; }

  virtual void Test();

  ClassDef(TMCFoamInt,1)   
};

//-----------------------------------------------------------------------------
inline Double_t TMCFoamInt::Density(Int_t nDim, Double_t *Xarg)
{
  // Density distribution for TFoam
  // Default if linear interpolation on a regular N-dim grid
  Scale( Xarg, fXscaled );
  Double_t result;
  switch( fDensOpt ){
  case EDensLinear:
  default:
    // multi-linear interpolation
    result =  LinInterpN(fXscaled);
    break;
  case EDensLinearC:
    // multi-linear interpolation with compensation
    result = LinInterpN(fXscaled,kTRUE);
    break;
  }
  // weighting of output
  return WgtDensity( result );
}

//-----------------------------------------------------------------------------
inline Double_t TMCFoamInt::WgtDensity( Double_t density )
{
  // Apply weighting to result of density output
  // 1 approx bremsstrahlung
  // 2 sin(theta) phase-space factor
  // 3 add isolated resonance to excitation function
  //
  for( Int_t i=0; i<fNDim; i++ ){
    Double_t x = fXscaled[i];
    switch( fIwgt[i] ){
    default:
    case EWgtUnity:                         // default no weighting
      break;
    case EWgtBrem:
      density *= 1/x;                       // "bremsstrahlung" 1/E weighting
      break;
    case EWgtBremRes:
      density += fWgt0* TMath::BreitWigner(x, fWgt1, fWgt2); // Resonance
      break;
    case EWgtSinTh:
      density *= TMath::Sin( x );           // sin(Theta) phase space weighting
      break;
    }
  }
  //
  return density;
}

//-----------------------------------------------------------------------------
inline void TMCFoamInt::Scale( Double_t* in, Double_t* out )
{
  // scale vector in by factors in scale array
  for( Int_t i=0; i<fNDim; i++ ){
    Int_t j = 2*i;
    fXarg[i] = in[i];
    out[i] = in[i] * (fScale[j+1] - fScale[j]) + fScale[j];
  }
}

//-----------------------------------------------------------------------------
inline void TMCFoamInt::LinIntpAccum( Int_t j, Double_t& yFit, Bool_t comp )
{
  // Evaluate cardinal functions recursively for each dimension j
  // and for sj = 0,1
  // Optionally apply compenstion for error in multi-linear interpolation
  for( Int_t i=0; i<=1; i++ ){
    fUj[j] = (1-i) + (2*i-1)*fXd[j];
    fM1[j] = fM0[j] + i;
    if( j < (fNDim-1) ){
      LinIntpAccum( j+1, yFit, comp );
    }
    else{
      Int_t jtot = IJK(fM1);
      Double_t u = fYN[jtot];
      if( comp ) u -= YComp();
      for( Int_t n=0; n<fNDim; n++ ) u *= fUj[n];
      yFit += u;
    }
  }
}

//-----------------------------------------------------------------------------
inline Double_t TMCFoamInt::YComp()
{
  // 1st order compensation for error in linear interpolation
  // via "double differential" of grid points...check for off range

  Double_t d2Y = 0.0;
  Double_t Y[3];
  Int_t j;
  Int_t iy[3];
  for( j=0; j<fNDim; j++ )fM2[j] = fM1[j];
  for( j=0; j<fNDim; j++ ){
    if( fM1[j] == 0 ) iy[0] = 0;                       // off range low
    else if( fM1[j] == fSN[j] - 1) iy[0] = fSN[j] - 3; // off range high
    else iy[0] = fM1[j] - 1;                           // OK
    iy[1] = iy[0] + 1;
    iy[2] = iy[0] + 2;
    for( Int_t i=0; i<3; i++ ){
      fM2[j] = iy[i];
      Y[i] = fYN[IJK(fM2)];
    }
    d2Y += Y[0] + Y[2] - 2*Y[1];
    fM2[j] = fM1[j];
  }
  return d2Y/12.0;
}

//-----------------------------------------------------------------------------
inline Int_t TMCFoamInt::IJK(Int_t* mj)
{
  // Convert N-dimensional i,j,k.... to an equivalent 1-D index
  Int_t coeff = 1;
  Int_t j1 = mj[fNDim-1];
  for( Int_t j=fNDim-1; j>0; j-- ){
    coeff *= fSN[j];
    j1 += mj[j-1]*coeff;
  }
  return j1;
}

//-----------------------------------------------------------------------------
inline Double_t TMCFoamInt::GenFn(Double_t E, Double_t Eg, Double_t Thg,
				Double_t Phig, Double_t Thpi0)
{
  Double_t Efac = TMath::BreitWigner(E, 0.3, 0.15);
  Double_t Egfac = TMath::BreitWigner(Eg, 0.1, 0.1) * TMath::Sqrt(E)/Eg;
  Double_t sinth = TMath::Sin(Thg);
  Double_t cosphi = TMath::Cos(2*Phig);
  Double_t sinpi = TMath::Sin(Thpi0);
  Double_t Thgfac = TMath::Sqrt(E*Eg)*(1 + sinth*sinth);
  Double_t Phigfac = 1.0 + cosphi;
  Double_t Thpi0fac = 1.0 + sinpi*sinpi;
  return Efac * Egfac * Thgfac * Phigfac * Thpi0fac;
}

//-----------------------------------------------------------------------------
inline void TMCFoamInt::SetWgt( Int_t iwgt, Int_t i, Double_t* opt  )
{
  // Initialise optional weighting for foam generator
  //
  if( i < fNDim ) fIwgt[i] = iwgt;
  if( opt ) {fWgt0 = opt[0]; fWgt1 = opt[1]; fWgt2 = opt[2];}
}

#endif
