//--Author	JRM Annand   13th Feb 2007  Adapt from TMCParicle	
//--Rev         JRM Annand
//--Rev         JRM Annand   16th Apr 2008  Special 3-body decay
//--Rev         JRM Annand   30th Jun 2008  Bug fix pion asimuthal angle
//--Update      JRM Annand   25th Jan 2008  Redo constructers

//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TMCdS5MDMParticle
//
// Generate 4-momenta according to "foam method"
// Specialist class to handle radiative PS-meson photoproduction.
// 5-fold differential cross section calculated according to
// Unitary Model or Effective-field-theory Model, tabulated and used
// to generate foam.

#ifndef __TMCdS5MDMParticle_h__
#define __TMCdS5MDMParticle_h__

#include "TMCFoamParticle.h"

//-----------------------------------------------------------------------------
class TMCdS5MDMParticle : public TMCFoamParticle {
 protected:
 public:
  TMCdS5MDMParticle( const Char_t*, TRandom*, TParticlePDG*,
		   Int_t, Char_t*, TA2System* = NULL );
  TMCdS5MDMParticle( const Char_t*, TMCParticle*, Double_t*,
		   Double_t*, Int_t* );
  virtual ~TMCdS5MDMParticle();
  virtual void Flush();
  virtual void Init( );
  virtual Bool_t GenNDecay( );

  ClassDef(TMCdS5MDMParticle,1)

};

#endif
