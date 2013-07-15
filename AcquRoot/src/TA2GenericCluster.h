//--Author	JRM Annand   30th Sep 2003  Read MC data
//--Rev 	JRM Annand...15th Sep 2003  Generalise methods  
//--Rev 	JRM Annand....9th Mar 2004  New OR variables
//--Rev 	JRM Annand....9th Mar 2005  protected instead of private vars
//--Rev 	JRM Annand...13th Jul 2005  split offs, time OR
//--Rev 	JRM Annand...25th Jul 2005  SetConfig hierarchy
//--Rev 	JRM Annand...20th Oct 2005  Iterative neighbour find (TAPS)
//--Rev 	JRM Annand... 9th Dec 2005  Change ambigous split-off thresh
//--Rev 	JRM Annand... 6th Feb 2006  Bug in SplitSearch
//--Rev 	JRM Annand...21st Apr 2006  Command-key enum to .h
//--Rev 	JRM Annand...22nd Jan 2007  4v0 update
//--Rev 	JRM Annand...12th May 2007  Central-frac and radius
//--Rev 	JRM Annand...18th Jan 2009  TMath::Sort (Root v5.22)
//--Rev 	JRM Annand...27th May 2009  Adapt from TA2ClusterDetector
//--Rev 	JRM Annand... 8th Oct 2009  MC error reconstructed pos.
//--Rev 	JRM Annand...20th Nov 2009  Adjustable energy weighting.
//--Rev 	JRM Annand...13th Apr 2010  MC position variables.
//--Update	JRM Annand   16th Sep 2011  log energy weighting
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2GenericCluster
//
// Decoding and calibration methods for EM calorimeters or similar systems
// where a shower or showers of secondary particles fire a cluster or
// clusters neighbouring calorimeter elements, 
// e.g. Crystal Ball NaI(Tl) array
//

#ifndef __TA2GenericCluster_h__
#define __TA2GenericCluster_h__

#include "TA2GenericDetector.h"
#include "GenHitCluster_t.h"       // hit cluster determination

// constants for command-line maps
enum { 
  EGenClustMaxCluster = 100, EGenClustNeighbour,  EGenClustAllNeighbour,
  EGenClustSplitOff, EGenClustIterate, EGenClustEnergyWeight,
  EGenClustEnergy, EGenClustTime, EGenClustCentFrac, EGenClustRadius,
  EGenClustHits, EGenClustMulti,
};

class TA2GenericCluster : public TA2GenericDetector {
 protected:
  GenHitCluster_t** fCluster;           // Clusters of hits
  UInt_t* fClustHit;                    // Cluster indices
  Bool_t* fIsSplit;                     // Indices split-off clusters
  UInt_t* fTempHits;                    // Element-Hit store
  UInt_t fNCluster;                     // # of clusters
  Int_t fClustSizeFactor;               // enlarge factor, hit cluster buffers
  UInt_t fNSplit;                       // # low energy clusters
  UInt_t fNSplitMerged;                 // # split-offs merged
  UInt_t fMaxCluster;                   // Max # of clusters
  UInt_t* fNClustHitOR;                 // OR of #hits in individuyal clusters
  Double_t* fTheta;                     // theta of cluster hit
  Double_t* fPhi;                       // phi of cluster hit
  Double_t* fClEnergyOR;                // OR of cluster energies
  Double_t* fClTimeOR;                  // OR of cluster times
  Double_t* fClCentFracOR;              // OR of energy ratios in central elem.
  Double_t* fClRadiusOR;                // OR E-weighted cluster radii
  Double_t* fCldTheta;                  // error in theta
  Double_t* fCldPhi;                    // error in phi
  Double_t* fCldX;                      // error in X
  Double_t* fCldY;                      // error in Y
  Double_t* fCldZ;                      // error in Z
  Double_t* fXmc;                       // actual particle X
  Double_t* fYmc;                       // actual particle Y
  Double_t* fZmc;                       // actual particle Z
  Double_t fClEthresh;                  // threshold energy for main cluster
  Double_t fEthresh;                    // generic threshold energy for cluster
  Double_t fEthreshSplit;               // threshold energy for split-off
  Double_t fMaxThetaSplitOff;           // max split-off opening angle 
  Double_t fMinPosDiff;                 // min acceptable neighbour pos diff. 
  Double_t fMaxPosDiff;                 // max acceptable neighbour pos diff. 
  Double_t* fSplitAngle;                // opening angles detween clusters
  Double_t fEWgt;                       // energy weighting factor
  Int_t fLEWgt;                        // energy weighting factor switch
  Int_t* fISplit;                       // for sorting cluster opening angles
  Int_t* fIJSplit;                      // for sorting cluster opening angles
  Int_t fMaxSplitPerm;                  // for sorting cluster opening angles
  Bool_t fIsIterate;                    // cluster member find iteration ON/OFF
 public:
  TA2GenericCluster( const char*, TA2System* );// Normal use
  virtual ~TA2GenericCluster();
  virtual void SetConfig( char*, int );// decode and load setup info
  virtual void ParseDisplay( char* );  // decode histogram setup lines
  virtual void LoadVariable(  );       // name-variable association
  virtual void PostInit( );            // initialise using setup info
  virtual void Decode( );              // hits -> energy procedure
  virtual void DecodeCluster( );       // determine clusters
  virtual void DecodeSaved( );         // decode previously written data
  virtual void Cleanup( );             // end-of-event cleanup
  //  virtual void SaveDecoded( ) = 0;     // specialist
  //  virtual void ReadDecoded( ) = 0;     // specialist
  virtual void DeleteClusterArrays();  // flush cluster-specific arrays
  virtual void SplitSearch();          // look for split-off clusters

  GenHitCluster_t** GetCluster(){ return fCluster; }
  GenHitCluster_t* GetCluster( UInt_t i ){ return fCluster[i]; }
  UInt_t* GetClustHit(){ return fClustHit; }
  UInt_t* GetClustHit( UInt_t i ){ return fClustHit + i; }
  Bool_t* GetIsSplit(){ return fIsSplit; }
  Bool_t* GetIsSplit( UInt_t i ){ return fIsSplit + i; }
  UInt_t* GetTempHits(){ return fTempHits; }
  UInt_t GetNCluster(){ return fNCluster; }
  Int_t GetClustSizeFactor(){ return fClustSizeFactor; }
  UInt_t GetNSplit(){ return fNSplit; }
  UInt_t GetNSplitMerged(){ return fNSplitMerged; } 
  UInt_t GetMaxCluster(){ return fMaxCluster; }
  UInt_t* GetNClustHitOR(){ return fNClustHitOR; }
  Double_t* GetTheta(){ return fTheta; }
  Double_t* GetPhi(){ return fPhi; }
  Double_t* GetClEnergyOR(){ return fClEnergyOR; }
  Double_t* GetClTimeOR(){ return fClTimeOR; }
  Double_t* GetClCentFracOR(){ return fClCentFracOR; }
  Double_t* GetClRadiusOR(){ return fClRadiusOR; }
  Double_t* GetCldTheta(){ return fCldTheta; }
  Double_t* GetCldPhi(){ return fCldPhi; }
  Double_t* GetCldX(){ return fCldX; }
  Double_t* GetCldY(){ return fCldY; }
  Double_t* GetCldZ(){ return fCldZ; }
  Double_t* GetXmc(){ return fXmc; }
  Double_t* GetYmc(){ return fYmc; }
  Double_t* GetZmc(){ return fZmc; }
  Double_t GetClEthresh(){ return fClEthresh; }
  Double_t GetEthresh(){ return fEthresh; }
  Double_t GetEthreshSplit(){ return fEthreshSplit; }
  Double_t GetMaxThetaSplitOff(){ return fMaxThetaSplitOff; }
  Double_t GetMinPosDiff(){ return fMinPosDiff; } 
  Double_t GetMaxPosDiff(){ return fMaxPosDiff; }
  Double_t* GetSplitAngle(){ return fSplitAngle; }
  Double_t GetEWgt(){ return fEWgt; }
  Int_t GetLEWgt(){ return fLEWgt; }
  Int_t* GetISplit(){ return fISplit; }
  Int_t* GetIJSplit(){ return fIJSplit; }
  Int_t GetMaxSplitPerm(){ return fMaxSplitPerm; }
  Bool_t IsIterate(){ return fIsIterate; }
 
  ClassDef(TA2GenericCluster,1)
};

//---------------------------------------------------------------------------
inline void TA2GenericCluster::Decode( )
{
  // Run basic TA2Detector decode, then cluster decode
  // and then histogram

  DecodeBasic();                    
  DecodeCluster();
}

//---------------------------------------------------------------------------
inline void TA2GenericCluster::DecodeSaved( )
{
  // Run basic TA2Detector decode, then cluster decode
  // and then histogram

  ReadDecoded();                    
  DecodeCluster();
}

//---------------------------------------------------------------------------
inline void TA2GenericCluster::DecodeCluster( )
{
  // Determine clusters of hits
  // Search around peak energies absorbed in individual crystals
  // Make copy of hits array as the copy will be altered

  memcpy( fTempHits, fHits, sizeof(UInt_t)*fNhits );  // temp copy
  //  fNCluster = 0;
  Double_t maxenergy;
  UInt_t i,j,k,kmax,jmax;
  // Find hit with maximum energy
  for( i=0; i<fMaxCluster;  ){
    maxenergy = 0;
    for( j=0; j<fNhits; j++ ){
      if( (k = fTempHits[j]) == ENullHit ) continue;
      if( maxenergy < fEnergy[k] ){
	maxenergy = fEnergy[k];
	kmax = k;
	jmax = j;
      }
    }
    if( maxenergy == 0 ) break;              // no more isolated hits
    if( kmax < fNelement ){
      // Construct the cluster
      fCluster[kmax]->ClusterDetermine( (TA2GenericCluster*)this );
      if( fCluster[kmax]->GetEnergy() >= fEthresh ){
	fClustHit[i] = kmax;
	fTheta[i] = fCluster[kmax]->GetTheta();
	fPhi[i] = fCluster[kmax]->GetPhi();
	fNClustHitOR[i] = fCluster[kmax]->GetNhits();
	fClEnergyOR[i] = fCluster[kmax]->GetEnergy();
	fClTimeOR[i] = fCluster[kmax]->GetTime();
	fClCentFracOR[i] = fCluster[kmax]->GetCentralFrac();
	fClRadiusOR[i] = fCluster[kmax]->GetRadius();
	fCldTheta[i] = fTheta[i] - fThMC[0];
	fCldPhi[i] = fPhi[i] - fPhiMC[0];
	// This block is only for reconstruction of Monte Carlo data
	// where error in reconstructed position can be estimated
	if( fX_MC ){
	  TVector3* mp = fCluster[kmax]->GetMeanPosition();
	  TVector3 ump = fP4MC[0].Vect(); ump = ump.Unit();
	  Double_t dist = mp->Z()/ump.Z(); // estimate
	  fXmc[i] = dist*ump.X();
	  fYmc[i] = dist*ump.Y();
	  fZmc[i] = fHitPos[kmax].Z();
	  fCldX[i] = mp->X() - fXmc[i]; 
	  fCldY[i] = mp->Y() - fYmc[i];
	  fCldZ[i] = mp->Z() - fZmc[i];
	} 
	i++;
      }
    }
    // If you reach here then there is an error in the decode
    // possible bad detector ID
    else fTempHits[jmax] = ENullHit;
  }
  fNCluster = i;                   // save # clusters
  // Now search for possible split offs if this is enabled
  if( fMaxSplitPerm ) SplitSearch();
  fClustHit[fNCluster] = EBufferEnd;
  fTheta[fNCluster] = EBufferEnd;
  fPhi[fNCluster] = EBufferEnd;
  fNClustHitOR[fNCluster] = EBufferEnd;
  fClEnergyOR[fNCluster] = EBufferEnd;
  fClTimeOR[fNCluster] = EBufferEnd;
  fClCentFracOR[fNCluster] = EBufferEnd;
  fClRadiusOR[fNCluster] = EBufferEnd;
  fCldTheta[fNCluster] = EBufferEnd;
  fCldPhi[fNCluster] = EBufferEnd;
  fCldX[fNCluster] = EBufferEnd;
  fCldY[fNCluster] = EBufferEnd;
  fCldZ[fNCluster] = EBufferEnd;
  fXmc[fNCluster] = EBufferEnd;
  fYmc[fNCluster] = EBufferEnd;
  fZmc[fNCluster] = EBufferEnd;
}

//---------------------------------------------------------------------------
inline void TA2GenericCluster::SplitSearch( )
{
  // Search for candidate split-off clusters associated with a main cluster
  // and attempt to merge them
  GenHitCluster_t *cli, *clj;
  Int_t* pij = fIJSplit;
  UInt_t i,j,ij,k,n;
  Double_t openAngle;
  for( i=0,n=0; i<fNCluster; i++ ){
    cli = fCluster[ fClustHit[i] ];                        // ith cluster
    if( cli->GetEnergy() > fClEthresh ) fIsSplit[i] = EFalse;
    else fIsSplit[i] = ETrue;
    for( j=i+1; j<fNCluster; j++ ){
      clj = fCluster[ fClustHit[j] ];                      // jth cluster
      if( (!fIsSplit[i]) && (clj->GetEnergy() > fClEthresh) ) continue;
      if( ( fIsSplit[i]) && (clj->GetEnergy() < fClEthresh) ) continue;
      openAngle = cli->OpeningAngle( clj );
      if( openAngle > fMaxThetaSplitOff ) continue;
      fSplitAngle[n] = openAngle;
      *pij++ = i | (j<<16);                       // store j,k indices
      n++;      
    }
  }
  // Sort the opening angles
  TMath::Sort((Int_t)n, fSplitAngle, fISplit, kFALSE);  // sort ascending
  fNSplitMerged = 0;
  for( k=0; k<n; k++ ){
    ij = fIJSplit[fISplit[k]];
    i = ij & 0xffff;
    j = (ij>>16) & 0xffff;
    cli = fCluster[ fClustHit[i] ];
    clj = fCluster[ fClustHit[j] ];
    if( cli->GetEnergy() > clj->GetEnergy() ){
      cli->Merge( clj );
      fIsSplit[j] = ETrue;
    }
    else{
      clj->Merge( cli );
      fIsSplit[i] = ETrue;
    }
    fNSplitMerged++;                          // # split-offs merged
  }
  // weed out any clusters marked as split off from cluster lists
  for( i=0,n=0; i<fNCluster; i++ ){
    if( fIsSplit[i] ) continue;
    fClustHit[n] = fClustHit[i];
    fTheta[n] = fCluster[i]->GetTheta();
    fPhi[n] = fCluster[i]->GetPhi();
    fNClustHitOR[n] = fCluster[i]->GetNhits();
    fClEnergyOR[n] = fCluster[i]->GetEnergy();
    fClTimeOR[n] = fCluster[i]->GetTime();
    n++;
  }
  fNSplit = fNCluster - n;                     // total low energy split-off
  fNCluster = n;                               // total accepted clusters
}

//---------------------------------------------------------------------------
inline void TA2GenericCluster::Cleanup( )
{
  // end-of-event cleanup
  TA2GenericDetector::Cleanup();
  //  would also clean up any cluster info here
  for(UInt_t i=0; i<fNCluster; i++) fCluster[ fClustHit[i] ]->Cleanup();
}
#endif
