//--Author	JRM Annand   19th Nov 2004
//--Rev         JRM Annand    7th Feb 2008  Add time zero		
//--Update      JRM Annand   16th Sep 2008  Add atten-len, time res.		
//
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// LongBar_t
// Utility stuff for long scintillator bar read out both ends by PMT
//

#ifndef __TLongBar_h__
#define __TLongBar_h__

#include "HitD2A_t.h"
#include "TMath.h"

class TA2LongScint;

class LongBar_t{
private:
  Int_t fEnd1;
  Int_t fEnd2;
  Double_t fCeff;         // effective scint light speed in bar
  Double_t fAttenLn;      // optical attenuation length of bar
  Double_t fTRes;         // time resolution each end
  Double_t fNscint;       // scint quanta per MeV
  Double_t fHalfLength;   // 1/2 length of bar
  Double_t fLongOffset;   // hit offset along long axis
  Double_t fYsize;        // Y dimension of bar
  Double_t fZsize;        // Z dimension of bar
  Double_t fT0;           // time zero point
  Double_t* fEnergy1;     // -> pulse height PMT 1
  Double_t* fEnergy2;     // -> pulse height PMT 2
  Double_t* fTime1;       // -> time PMT 1
  Double_t* fTime2;       // -> time PMT 2
  Double_t* fMeanEnergy;  // -> mean pulse height
  Double_t* fMeanTime;    // -> mean time
  Double_t* fTimeDiff;    // -> time difference
  TVector3* fPos;         // -> x,y,z of hit
  TVector3* fPos1;        // -> x,y,z PMT 1 
  TVector3* fPos2;        // -> x,y,z PMT 2
  TVector3 fMeanPos;      // position of center of bar
  TVector3 fAlignment;    // unit vector alignment of bar
public:
  LongBar_t( char*, Int_t, TA2LongScint* );
  virtual ~LongBar_t();
  virtual Double_t TMean(Double_t, Double_t); // mean time V1
  virtual Double_t GMean(Double_t, Double_t); // mean energy V1
  virtual Double_t TDiff(Double_t, Double_t); // time diff V1
  virtual void TMean();                       // mean time V2
  virtual void GMean();                       // mean energy V2
  virtual void TDiff();                       // time diff V2
  virtual Bool_t TDiffCheck();                // time diff V2
  virtual void Pos();                         // determine hit position
  virtual void Cleanup();                     // end-of-event flush
  //
  Int_t GetEnd1(){ return fEnd1; }            // end1 index
  Int_t GetEnd2(){ return fEnd2; }            // end2 index
  Double_t GetCeff(){ return fCeff; }         // effective light speed
  Double_t GetAttenLn(){ return fAttenLn; }   // attenuation length
  Double_t GetTRes(){ return fTRes; }         // time resolution
  Double_t GetNscint(){ return fNscint; }     // scint quanta/MeV
  Double_t GetHalfLength(){ return fHalfLength; }
  Double_t GetLongOffset(){ return fLongOffset; }
  Double_t GetYsize(){ return fYsize; }       // Y dimension
  Double_t GetZsize(){ return fZsize; }       // Z dimension
  Double_t GetT0(){ return fT0; }             // time zero
  TVector3* GetPos(){ return fPos; }          // Hit position
  TVector3* GetPos1(){ return fPos1; }        // End-1 position
  TVector3* GetPos2(){ return fPos2; }        // End-2 position
  TVector3 GetMeanPos(){ return fMeanPos; }   // Central position
  TVector3 GetAlignment(){ return fAlignment;}// Alignment direction cosines
};

//---------------------------------------------------------------------------
inline Double_t LongBar_t::TMean( Double_t t1, Double_t t2 )
{
  // Mean of 2 times
  return 0.5 * (t1 + t2) - fT0;
}

//---------------------------------------------------------------------------
inline Double_t LongBar_t::GMean( Double_t e1, Double_t e2 )
{
  // Geometric mean of 2 pulse heights
  return TMath::Sqrt(e1 * e2);
}

//---------------------------------------------------------------------------
inline Double_t LongBar_t::TDiff( Double_t t1, Double_t t2 )
{
  // Difference of 2 times
  return t1 - t2;
}

//---------------------------------------------------------------------------
inline void LongBar_t::TMean( )
{
  // Arithmetic mean of 2 times
  *fMeanTime = 0.5 * (*fTime1 + *fTime2) - fT0;
}

//---------------------------------------------------------------------------
inline void LongBar_t::GMean( )
{
  // Geometric mean of 2 pulse heights
  *fMeanEnergy = TMath::Sqrt(*fEnergy1 * *fEnergy2);
}

//---------------------------------------------------------------------------
inline void LongBar_t::TDiff( )
{
  // Difference of 2 times
  *fTimeDiff = *fTime2 - *fTime1;
}

//---------------------------------------------------------------------------
inline Bool_t LongBar_t::TDiffCheck( )
{
  // Difference of 2 times...check it yields a sensible position
  Double_t diff = *fTime2 - *fTime1;
  fLongOffset = diff * fCeff;
  if( fabs(fLongOffset) > fHalfLength ){
    fLongOffset = ENullFloat;
    return kFALSE;
  }
  *fTimeDiff = diff;
  return kTRUE;
}

//---------------------------------------------------------------------------
inline void LongBar_t::Pos()
{
  // Convert long-axis offset from time difference to position
  *fPos = fMeanPos + fLongOffset * fAlignment;  // hit position
}

//---------------------------------------------------------------------------
inline void LongBar_t::Cleanup( )
{
  // Zero local variables
  *fMeanTime = ENullFloat;
  *fMeanEnergy = ENullFloat;
  *fTimeDiff = ENullFloat;
  fLongOffset = ENullFloat;
  fPos->SetXYZ(ENullFloat,ENullFloat,ENullFloat);
}

#endif
