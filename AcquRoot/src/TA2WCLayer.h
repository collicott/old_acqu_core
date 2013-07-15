//--Author	V Lisin      28th Jun 2004  original DAPHNE fortran -> C
//--Update	JRM Annand... 8th Jul 2004  AcquRoot C++ class
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2WCLayer
//
// General methods for single layer wire chamber
//

#ifndef __TA2WCLayer_h__
#define __TA2WCLayer_h__

#include "TA2System.h"
#include "TVector3.h"

enum { EMaxTracks = 5, EMaxHitW = 25, EMaxHitS = 25 };

class TA2WCLayer : public TA2System {
 protected:
  Int_t fNElement;             // # wires or strips in layer
  Int_t fIstart;               // start index of layer within detector
  Int_t fIend;                 // end index of layer within detector
  Int_t fMaxClust;             // max # clusters in layer
  Int_t fMaxClustSize;         // max # clusters in layer
  Int_t fNHits;                // # hits in layer
  Int_t fNClust;               // # clusters in layer 
  Int_t* fClust;               // cluster start index
  Int_t* fLenClust;            // # elements spanned by cluster
  Double_t* fCGClust;          // center of gravity of cluster
  Int_t fIdist;                // max distance between cluster elements  
  Int_t* fHits;                // total detector elements which fired in event
  Int_t* fLayerHits;           // layer elements which fired in event   
  Double_t* fEnergy;           // pulse amplitude array
  Double_t* fTime;             // hit time array
  TVector3** fPos;             // hit-element positioning array

 public:
  TA2WCLayer( const char*, Int_t, Int_t, Int_t, void*  ); // Normal use
  virtual ~TA2WCLayer();
  virtual void SetConfig( char*, int ){} // decode and load setup info
  virtual void DecodeCluster( Int_t&, Int_t** );
  virtual void CGCluster( Int_t );
  virtual void CGClusters(  );
  virtual Bool_t IsInside( Double_t ){ return EFalse; }
  //  virtual Double_t CorPhiWC( Int_t, Double_t );
  virtual Int_t GetNElement(){ return fNElement; }
  virtual Int_t* GetLayerHits(){ return fLayerHits; }
  virtual Int_t* GetClust(){ return fClust; }
  virtual Int_t GetClust(Int_t i){ return fClust[i]; }
  virtual Int_t* GetLenClust(){ return fLenClust; }
  virtual Int_t GetLenClust(Int_t i){ return fLenClust[i]; }
  virtual Int_t GetNClust(){ return fNClust; }
  virtual Int_t* GetNClustPtr(){ return &fNClust; }     // for display
  virtual Int_t* GetNHitsPtr(){ return &fNHits; }       // for display
  virtual Double_t* GetCGClust(){ return fCGClust; }
  virtual Double_t GetCGClust(Int_t i){ return fCGClust[i]; }


  ClassDef(TA2WCLayer,1)
};

//---------------------------------------------------------------------------
inline void TA2WCLayer::DecodeCluster( Int_t& nHit, Int_t** phit )
{
  //
  // Seach clusters in the list of hit elements 'hit' with length 'nHit'
  // and returns the number of clusters.
  // The elements are represented by their numbers, n0,n0+1,n0+2,...
  // n0+(nTot-1), 
  // so 'nTot' is the number of all elements. 
  // The elements are placed on the circle, i.e. elements with lowest
  // and highest numbers are neigbors. 'idist' is the maximum distance 
  // between neigbors (example: idist=2 -> number 2 and 4 are neigbors).
  // 'maxClu' is the maximum allowed number of clusters (array dimension).
  // 'listClu(i)' - first i-cluster element, 
  // 'lenClu - the length of i-cluster.
  // If number of clusters is more then 'maxClu' then return -1. 
  // If there is the cluster with big and small elements then
  // it we be placed at first position in the lists with big start number
  // Example: array of 10 elemnts 1...10 has hit elements 1, 9, 10, then
  // listClu[0]=9, lenClu[0]=3 and the number of clusters =1 will return.
  // - V.Lisin, 25-Jun-04.
  //

  Int_t nh;
  Int_t* hit = *phit;

  fNClust = 0;
  
  // Get number hits nh in particular layer
  for( nh=0; nh<nHit; nh++ ){
    if( hit[nh] >= fIend ) break;;
    fLayerHits[nh] = hit[nh] - fIstart;
  }
  fLayerHits[nh] = EBufferEnd;  // mark end of local hits buffer
  fNHits = nh;                  // store local # hits

  if(nh==0) return;
  nHit -= nh;                // subtract hits in current layer
  *phit += nh;               // point past hits in current layer

  if( nh==1 ) {              // trivial case...1 hit only in layer
    fNClust = 1;
    fLenClust[0] = 1;
    fClust[0] = fLayerHits[0];
    return;
  }

  // More than 1 hit in layer...look for clusters
  Int_t cluSta = fLayerHits[0];
  Int_t cluEnd = cluSta;
  Int_t nc = 0;
  // Loop round hits...add to cluster if current hit <= fIdist separated in
  // index from previous hit index  
  for(Int_t i=1; i<nh; i++) {
    if( fLayerHits[i] <= (cluEnd+fIdist) ) 
      cluEnd = fLayerHits[i];               // the element is in the cluster;
    else {                                  // the cluster is ended, so
      fClust[nc] = cluSta;                  // put it in the cluster start..
      fLenClust[nc++] = cluEnd - cluSta + 1;// and cluster length lists and...
      if( nc >= fMaxClust ){                // exceeding cluster storage space?
	fNClust = nc;                       // stop here
	return;
      }
      cluSta = fLayerHits[i];               // start new cluster;
      cluEnd = cluSta;
    }
  }
  // Check last cluster - may be it connects with the first one
  if( (fLayerHits[0] - cluEnd + fNElement) <= fIdist) { // - yes, connect them;
    fLenClust[0] = fLayerHits[0] + fLenClust[0] - cluSta + fNElement;
    fClust[0] = cluSta;                 // now 1st element > last element! 
  }
  else if( cluEnd != cluSta ){          // no - add last cluster to the list. 
    if( nc < fMaxClust ){               // ....if there is space
      fClust[nc] = cluSta;
      fLenClust[nc++] = cluEnd - cluSta + 1;
    }    
  }
  fNClust = nc;
}

//---------------------------------------------------------------------------
inline void TA2WCLayer::CGCluster( Int_t ic )
{
  // Center of gravity of cluster (index ic) of adjacent hit wires or strips
  // default no pulse height weighting

  fCGClust[ic] = fClust[ic] + 0.5*(fLenClust[ic] - 1);
}

//---------------------------------------------------------------------------
inline void TA2WCLayer::CGClusters()
{
  // Center of gravity of all clusters
  // For multi-value variables add end-marker to storage buffer
  Int_t i;
  for( i=0; i<fNClust; i++ )CGCluster( i );
  fClust[i] = EBufferEnd;
  fLenClust[i] = EBufferEnd;
  fCGClust[i] = EBufferEnd;
}

#endif
