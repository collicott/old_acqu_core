//--Author	JRM Annand... 8th Jul 2004  AcquRoot C++ class
//--Rev 	JRM Annand... 7th Sep 2004  alpha-production version
//--Rev 	JRM Annand...15th Mar 2005  GetNTrack, GetMaxTrack
//--Rev 	JRM Annand...25th Jul 2005  SetConfig hierarchy
//--Update	JRM Annand... 1st Sep 2009  varname..BuildName, delete[]
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2WireChamber
//
// General purpose framework of decoding and calibration methods for 
// position sensitive multi-wire gas counters.
// The wire chamber typically consists of n separate chambers
// each chamber having l separate layers.
//

#ifndef __TA2WireChamber_h__
#define __TA2WireChamber_h__

#include "MCBranchID.h"
#include "TA2Detector.h"
#include "TA2WCLayer.h"
#include "TA2CylWire.h"
#include "TA2CylStrip.h"
#include "TA2Track.h"

class TA2WireChamber : public TA2Detector {

protected:
  Int_t fNLayer;                       // # planes (layers) of detector
  Int_t fNlayer;                       // ditto
  Int_t fNChamber;                     // # "separate" chambers
  Int_t fNchamber;                     // ditto
  Int_t fMaxIntersect;                 // max # intersect points
  Int_t fMaxTrack;                     // max # tracks
  Int_t fMaxVertex;                    // max # vertices
  Int_t** fChamberLayers;              // store layers in chambers
  Int_t fNTrack;                       // # tracks in event
  Int_t fNVertex;                      // # vertices in event
  TA2WCLayer** fWCLayer;               // array of plane classes;
  TA2Track** fTrack;                   // track info for event
  TVector3** fVertex;                  // arrays of vertices
  Double_t* fVertexLimits;             // outer limits of vertex coordinates
  Double_t* fTrackTheta;               // polar track angles
  Double_t* fTrackPhi;                 // asimuthal track angles
  Double_t fPi;
  Double_t f2Pi;
public:
  TA2WireChamber( const char*, TA2System* );
  virtual ~TA2WireChamber();
  virtual void DeleteArrays();                   // flush new storage
  virtual void Decode();                         // main decode method
  virtual void DecodeLayers();                   // separate layers decode
  virtual void IntersectLayers( Int_t ) = 0;     // intersection coords
  virtual void SetConfig( char*, Int_t );        // decode and load setup info
  virtual void LoadVariable();                   // load variables in list
  virtual void SetChamberParm(Int_t, Double_t*){}// save chamber param
  virtual void SaveDecoded( );                   // save local analysis
  virtual void ReadDecoded( );                   // read back previous analysis or MC
  // Getters for read-only variables
  Int_t GetNLayer(){ return fNLayer; }
  Int_t GetNlayer(){ return fNlayer; }
  Int_t GetNChamber(){ return fNChamber; }
  Int_t GetNchamber(){ return fNchamber; }
  Int_t GetMaxIntersect(){ return fMaxIntersect; }
  Int_t GetMaxTrack(){ return fMaxTrack; }
  Int_t** GetChamberLayers(){ return fChamberLayers; } 
  TA2WCLayer** GetWCLayer(){ return fWCLayer; } 
  TA2WCLayer* GetWCLayer(Int_t i){ return fWCLayer[i]; }
  TVector3** GetVertex(){ return fVertex; }
  TVector3* GetVertex(Int_t i){ return fVertex[i]; }
  TA2Track** GetTrack(){ return fTrack; }
  TA2Track* GetTrack(Int_t i){ return fTrack[i]; }
  Int_t GetNTrack(){ return fNTrack; }
  Double_t* GetVertexLimits(){ return fVertexLimits; }
  Double_t GetVertexLimits(Int_t i){ return fVertexLimits[i]; }
  Double_t* GetTrackTheta(){ return fTrackTheta; }
  Double_t* GetTrackPhi(){ return fTrackPhi; }
  // General utilities
  Double_t AbsDiffPhi( Double_t, Double_t );    // abs +ve phi diff
  Double_t DiffPhi( Double_t, Double_t );       // phi diff
  Double_t AbsPhi( Double_t );                  // phi 0 - 2*Pi

  ClassDef(TA2WireChamber,1)
};

//---------------------------------------------------------------------------
inline void TA2WireChamber::ReadDecoded( )
{
  // Read back...
  //   either previously analysed data from Root Tree file
  //   or MC simulation results, assumed to have the same data structure
  // No MC simulation of MWPC yet
}

//---------------------------------------------------------------------------
inline void TA2WireChamber::Decode( )
{
  // Do the basic decoding (e.g. ADC -> Energy)
  // and then sort out which hits belong to which layers

  DecodeBasic();
  DecodeLayers();
}

//---------------------------------------------------------------------------
inline void TA2WireChamber::DecodeLayers()
{
  // Loop round all active layers of the chamber
  // Identify clusters of hits and find "center of gravity (charge)"

  Int_t* hits = fHits;
  Int_t nhits = fNhits;
  // Loop round layers, continue even if nhits is zero as
  // methods initialise arrays which are plotted etc.
  for( Int_t i=0; i<fNLayer; i++ ){
    fWCLayer[i]->DecodeCluster( nhits,&hits );
    fWCLayer[i]->CGClusters();
  }
}

//---------------------------------------------------------------------------
inline Double_t TA2WireChamber::AbsPhi( Double_t phi )
{
  // Return phi angle in range 0 - 2*Pi
  // always returns +ve number
  
  if( phi < 0 ) phi += f2Pi;
  else if( phi > f2Pi ) phi -= f2Pi;
  return phi;
}

//---------------------------------------------------------------------------
inline Double_t TA2WireChamber::AbsDiffPhi( Double_t phi1, Double_t phi2 )
{
  // Utility to provide absolute difference in phi angle
  // always returns +ve number

  Double_t diff = phi1 - phi2;
  if( diff < 0 ) diff = -diff;
  if( diff > f2Pi ) diff = f2Pi - diff;
  return diff;
}

//---------------------------------------------------------------------------
inline Double_t TA2WireChamber::DiffPhi( Double_t phi1, Double_t phi2 )
{
  // Utility to provide difference in phi angle
  // always returns +ve number

  Double_t diff = phi1 - phi2;
  if( diff > fPi ) diff -= f2Pi;
  else if( diff < -fPi ) diff += f2Pi;
  return diff;
}

#endif
