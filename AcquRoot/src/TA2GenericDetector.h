//--Author	JRM Annand    1st Dec 2004
//--Rev         JRM Annand    9th Oct 2008  Add ReadDecoded	
//--Rev         JRM Annand    9th Oct 2008  Generalise ReadDecoded	
//--Rev         JRM Annand   21st May 2009  Store MC incident particles	
//--Rev         JRM Annand   16th Feb 2010  MC Weighted or non-weighted energy
//--Rev         JRM Annand   27th Mar 2010  MC Time over threshold option
//--Rev         JRM Annand   30th May 2012  Check *fNPartMC not negative
//--Update      JRM Annand   10th Oct 2012  Time over threshold
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2GenericDetector
//
// A version of TA2Detector which can be instantiated
//

#ifndef __TA2GenericDetector_h__
#define __TA2GenericDetector_h__

#include "MCBranchID.h"
#include "TA2Detector.h"
#include "TLorentzVector.h"

enum {
  EGenericDetectorMC = 1000, EGenericDetectorComb, EGenericDetectorToThr,
  EGenericDetectorNPartMC = 16,
};

// Command-line generic detector key words which determine
// what information is read in
static const Map_t kGenericDetectorKeys[] = {
  {"MC-Decode:",          EGenericDetectorMC},
  {"MC-Combine:",         EGenericDetectorComb},
  {"Time-over-Thresh:",   EGenericDetectorToThr},
  {NULL,            -1},
};
 
class TA2GenericDetector : public TA2Detector {
 protected:
  Int_t fIminMC;
  Int_t fImaxMC;
  Int_t* fNPartMC;
  Int_t* fNHitsMC;
  Int_t* fHitsMC;
  Int_t* fIndex2Element;
  Float_t* fEnergyMC;
  Float_t* fTimeMC;
  Float_t* fX_MC;
  Float_t* fY_MC;
  Float_t* fZ_MC;
  Float_t* fVertexMC;
  TVector3* fHitPos;
  Double_t* fXMC;                      // MC hit X
  Double_t* fYMC;                      // MC hit Y
  Double_t* fZMC;                      // MC hit Z
  Double_t* fXMCOR;                    // MC hit X
  Double_t* fYMCOR;                    // MC hit Y
  Double_t* fZMCOR;                    // MC hit Z
  TVector3 fVertex;                    // MC production vertex
  //  TVector3 fDirCos;                // MC production vertex
  Float_t* fBeamMC;                    // MC Beam
  Float_t* fDirCosMC;                  // MC particle directions
  Float_t* fPlabMC;                    // MC particle total lab momenta
  Float_t* fElabMC;                    // MC particle lab energies
  TLorentzVector* fP4MC;               // MC incident particle 4-momenta
  TLorentzVector* fP4Comb;             // MC incident particle 4-momenta
  Double_t* fEMC;                      // MC incident particle theta lab
  Double_t* fPMC;                      // MC incident particle theta lab
  Double_t* fThMC;                     // MC incident particle theta lab
  Double_t* fPhiMC;                    // MC incident particle phi lab
  Double_t* fTimeOvThr;                // Time over threshold
  Int_t fNComb;                 // no. MC particles to combine
  Int_t* fIComb;                // indices MC particles to combi
 public:
  TA2GenericDetector( const char*, TA2System* );// Normal use
  virtual ~TA2GenericDetector();
  virtual void DeleteArrays();
  virtual void SetConfig( char*, Int_t ); // read in setup parameters
  virtual void SaveDecoded( );            // save local analysis
  virtual void ReadDecoded( );            // read back previous analysis
  virtual void PostInit();
  virtual void LoadVariable();
  virtual Int_t GetBranchIndex(Char_t*);
  virtual void Decode();
  virtual void Cleanup();
  TVector3 GetVertex(){ return fVertex; }
  TVector3 GetHitPos(Int_t i){ return fHitPos[i]; }
  Double_t GetXMC(Int_t i){ return fXMC[i]; }
  Double_t GetYMC(Int_t i){ return fYMC[i]; }
  Double_t GetZMC(Int_t i){ return fZMC[i]; }
  
  ClassDef(TA2GenericDetector,1)
};

//---------------------------------------------------------------------------
inline void TA2GenericDetector::Decode( )
{
  // Do the basic decoding (e.g. ADC -> Energy)
  // and histogram basic decoded parameters
  DecodeBasic();
  if( fTimeOvThr ){
    for(UInt_t i=0; i<fNelem; i++)fTimeOvThr[i] = (fElement[i])->GetTimeOvThr();
  }
}
//---------------------------------------------------------------------------
inline void TA2GenericDetector::ReadDecoded( )
{
  // Read info from A2 Monte Carlo output...energy, time, index
  // and optionally hit position, beam energy/direction and vertex
  //
  if( fVertexMC )                    // optional vertex
    fVertex.SetXYZ(fVertexMC[0],fVertexMC[1],fVertexMC[2]);
  if( fNPartMC ){                    // optional incident particles
    Int_t np = *fNPartMC;
    Double_t px,py,pz;
    Float_t* pd = fDirCosMC;
    for( Int_t i=0; i<np; i++ ){
      px = *pd++ * fPlabMC[i] * 1000;
      py = *pd++ * fPlabMC[i] * 1000;
      pz = *pd++ * fPlabMC[i] * 1000;
      fP4MC[i].SetXYZT( px,py,pz,(Double_t)fElabMC[i]*1000 );
      fEMC[i] = fP4MC[i].E();
      fPMC[i] = fP4MC[i].P();
      fThMC[i] = fP4MC[i].Theta() * TMath::RadToDeg();
      fPhiMC[i] = fP4MC[i].Phi() * TMath::RadToDeg();
    }
    if( fP4Comb ){
      fP4Comb->SetXYZT(0,0,0,0);
      for(Int_t i=0; i<fNComb; i++){
	Int_t j = fIComb[i];
	*fP4Comb = *fP4Comb + fP4MC[j];
      }
      fEMC[np] = fP4Comb->E();
      fPMC[np] = fP4Comb->P();
      fThMC[np] = fP4Comb->Theta() * TMath::RadToDeg();
      fPhiMC[np] = fP4Comb->Phi() * TMath::RadToDeg();
    }
  }
  Int_t nHits;
  if( fNHitsMC ) nHits = *fNHitsMC;             // # MC hits
  else nHits = 0;
  Float_t* energy = fEnergyMC;         // Energy array
  Float_t* time = fTimeMC;             // Time array
  Int_t* index = fHitsMC;              // Hit indices array
  Float_t* xpos;                       // Hit coordinates
  Float_t* ypos;
  Float_t* zpos;
  if( fX_MC ){                        // optional position
    xpos = fX_MC;                     // arrays of hit coordinates
    ypos = fY_MC;
    zpos = fZ_MC;
  }
  Int_t i,j,k;
  Double_t T,t,x,y,z;
  fTotalEnergy = 0.0;
  for( i=0,k=0; i<nHits; i++ ){
    j = *index++;               // bar index
    j = fIndex2Element[j];
    // Not part of the present detector
    // Shift array pointers to next
    if( j == ENullADC ){
      energy++; time++;
      if( fX_MC ){ xpos++; ypos++; zpos++; }
      continue;
    }
    // Store variables
    T = 1000 * *energy++;              // energy deposit in bar in MeV
    t = *time++;                       // hit time in bar
    if( fX_MC ){                       // position info recorded
      x = *xpos++;                     // x coord hit position
      y = *ypos++;                     // y coord hit position
      z = *zpos++;                     // z coord hit position
    }
    HitD2A_t* elem = fElement[j];
    if( (T < elem->GetEnergyLowThr()) || (T > elem->GetEnergyHighThr()) )
      continue;
    if( (t < elem->GetTimeLowThr()) || (t > elem->GetTimeHighThr()) )
      continue;
    if( fX_MC ){
      fHitPos[j].SetXYZ(x,y,z);
      fXMC[j] = x;
      fYMC[j] = y;
      fZMC[j] = z;
      fXMCOR[k] = x;
      fYMCOR[k] = y;
      fZMCOR[k] = z;
    }
    fEnergy[j] = T;
    fTime[j] = t;
    fTotalEnergy += T;
    fEnergyOR[k] = T;
    fTimeOR[k] = t;
    fHits[k] = j;
    k++;
  }
  fNhits = k;
  fHits[k] = EBufferEnd;
  fEnergyOR[k] = EBufferEnd;
  fTimeOR[k] = EBufferEnd;
  if(fXMCOR) fXMCOR[k] = fYMCOR[k] = fZMCOR[k] = EBufferEnd;
}

//---------------------------------------------------------------------------
inline void TA2GenericDetector::Cleanup( )
{
  // end-of-event cleanup
  TA2Detector::Cleanup();
  if( fX_MC ){
    for(UInt_t i=0; i<fNelement; i++){
      fXMC[i] = fYMC[i] = fZMC[i] = ENullFloat;
      fHitPos[i].SetXYZ( ENullFloat, ENullFloat, ENullFloat );
    }
  }
}


#endif
