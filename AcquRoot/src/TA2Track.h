//--Author	JRM Annand... 1st Sep 2004  AcquRoot C++ class
//--Update	JRM Annand...
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2Track
//
// Charged particle tracks, reconstructed from position sensitive detector
// information. Track consistes of 2, 3-vectors: an origin and a set of
// direction cosines.
//

#ifndef __TA2Track_h__
#define __TA2Track_h__

#include "TA2System.h"
#include "TVector3.h"

class TA2Track : public TA2System {
 protected:
  TVector3* fOrigin;             // Track origin
  TVector3* fDirCos;             // direction cosines
  TVector3* fVertex;             // track intersection vertex

 public:
  TA2Track( const char*  ); // Normal use
  virtual ~TA2Track();
  virtual void SetTrack( Double_t* );
  virtual void SetTrack( Double_t,Double_t,Double_t,
			 Double_t,Double_t,Double_t );
  virtual void SetTrackCyl( Double_t,Double_t,Double_t,
			    Double_t,Double_t,Double_t );
  virtual void SetConfig(char*, Int_t){}
  virtual void LoadVariable(){}
  virtual TVector3* PseudoZVertex( Double_t );
  virtual TVector3* TrackVertex( TA2Track* );
  virtual TVector3* GetOrigin(){ return fOrigin; }
  virtual TVector3* GetDirCos(){ return fDirCos; }
  virtual TVector3* GetVertex(){ return fVertex; }
  virtual Double_t GetOx(){ return fOrigin->X(); }  // origin x
  virtual Double_t GetOy(){ return fOrigin->Y(); }  // origin y
  virtual Double_t GetOz(){ return fOrigin->Z(); }  // origin z
  virtual Double_t GetDu(){ return fDirCos->X(); }  // dircos u
  virtual Double_t GetDv(){ return fDirCos->Y(); }  // dircos v
  virtual Double_t GetDw(){ return fDirCos->Z(); }  // dircos w
  virtual Double_t GetVx(){ return fVertex->X(); }  // Vertex x
  virtual Double_t GetVy(){ return fVertex->Y(); }  // Vertex y
  virtual Double_t GetVz(){ return fVertex->Z(); }  // Vertex z
  virtual Double_t GetTheta(){ return fDirCos->Theta(); }  // Polar angle
  virtual Double_t GetPhi(){ return fDirCos->Phi(); }      // Asym angle

  ClassDef(TA2Track,1)
};

//---------------------------------------------------------------------------
inline void TA2Track::SetTrack( Double_t* pos )
{
  // Set coordinates, assumed cartesian, of origin and direction cosines
  // from values provided in input array

  fOrigin->SetXYZ(pos[0],pos[1],pos[2]);
  fDirCos->SetXYZ(pos[3],pos[4],pos[5]);
}

//---------------------------------------------------------------------------
inline void TA2Track::SetTrack( Double_t x0, Double_t y0, Double_t z0,
				Double_t x1, Double_t y1, Double_t z1 )
{
  // Set coordinates, assumed cartesian, of origin and direction cosines
  // from 6 input variables

  fOrigin->SetXYZ(x0,y0,z0);
  fDirCos->SetXYZ(x1,y1,z1);
  *fDirCos -= *fOrigin;
  *fDirCos = fDirCos->Unit();
}

//---------------------------------------------------------------------------
inline void TA2Track::SetTrackCyl( Double_t r0, Double_t phi0, Double_t z0, 
				   Double_t r1, Double_t phi1, Double_t z1 )
{
  // Set track cooordinates from input cylindrical polars

  SetTrack( r0*cos(phi0), r0*sin(phi0), z0,
	    r1*cos(phi1), r1*sin(phi1), z1 );
}


//---------------------------------------------------------------------------
inline TVector3* TA2Track::TrackVertex( TA2Track* tr )
{
  // Cartesian coordinates of intersection point between this track
  // and other track tr

  TVector3* o = tr->GetOrigin();   // origin of other track
  TVector3* d = tr->GetDirCos();   // direction cosines of other track

  // Distance from the origin of the other track to the intersection point
  Double_t dd = ( fDirCos->Y() * (fOrigin->X() - o->X()) - 
		  fDirCos->X() * (fOrigin->Y() - o->Y()) )/
                 (fDirCos->X()*d->Y() - d->X()*fDirCos->Y());
  // The intersection point
  *fVertex = *o - (dd * *d);
  return fVertex;
}


//---------------------------------------------------------------------------
inline TVector3* TA2Track::PseudoZVertex( Double_t r )
{
  // Intersection points of a track defined by origin (hit point on
  // inner chamber) and direction cosines (determined by hit points on
  // inner and outer chambers) on a cylinder of radius r, axis = Z axis.
  // If there's no intersection return NULL. If there is return a
  // "pseudo vertex" the point where the track is closest to the Z axis

  Double_t a = fDirCos->X()*fDirCos->X() + fDirCos->Y()*fDirCos->Y();
  Double_t b = fOrigin->X()*fDirCos->X() + fOrigin->Y()*fDirCos->Y();
  Double_t c = fOrigin->X()*fOrigin->X() + fOrigin->Y()*fOrigin->Y() - r*r;
  Double_t d = b*b - a*c;
  // Check if the track crosses within dist r of z axis
  if( d <= 0 ) return NULL;
  // OK so calculate the distance from origin to closest approach
  d = b/a;
  // ...and trace back from the origin to the 3-vector point of closest app.
  *fVertex = *fOrigin - d * *fDirCos;
  return fVertex;
}

#endif
