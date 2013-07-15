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

#include "TA2LongScint.h"
#include "LongBar_t.h"
#include "TVector3.h"

//---------------------------------------------------------------------------
LongBar_t::LongBar_t( char* line, Int_t nbar, TA2LongScint* ls )
{
  // Long scintillor bar....
  // Read in indices (main element array) of each end, effective light speed
  // and Y, Z dimensions. X assumed the long dimension
  // Get pointers to relevant quantities in arrays held by TA2LongScint
  // Setup vectors for position determination
  //
  Int_t n;
  n = sscanf( line,"%d%d%lf%lf%lf%lf%lf%lf%lf",
	      &fEnd1,&fEnd2,&fCeff,&fYsize,&fZsize,&fT0,&fAttenLn,&fTRes,
	      &fNscint );
  if( n < 5 ){
    // Error in input line...ignore it
    printf(" Error ignore create LongBar_t from input line:\n %s\n",line);
    return;
  }
  if( n < 6 ) fT0 = 0.0;          // default time offset 0.0;
  if( n < 7 ) fAttenLn = 400.0;   // default arrenuation length 400cm
  if( n < 8 ) fTRes = 0.0;        // default no time smearing
  if( n < 9 ) fNscint = 200.0;    // default scintillation yield
  // -> pulse heights each end
  fEnergy1 = ls->GetEnergy() + fEnd1;
  fEnergy2 = ls->GetEnergy() + fEnd2;
  // -> times each end
  fTime1 = ls->GetTime() + fEnd1;
  fTime2 = ls->GetTime() + fEnd2;
  // mean pulse height and time
  fMeanEnergy = ls->GetMeanEnergy() + nbar;
  fMeanTime = ls->GetMeanTime() + nbar;
  // time difference
  fTimeDiff = ls->GetTimeDiff() + nbar;
  // Positions
  fPos = ls->GetBarPos() + nbar;        // hit position
  fPos1 = ls->GetPosition( fEnd1 );     // const end1-of-bar pos
  fPos2 = ls->GetPosition( fEnd2 );     // const end2-of-bar pos
  TVector3 temp = *fPos1 - *fPos2;
  fHalfLength = temp.Mag()/2;           // 1/2 length of bar
  fAlignment = temp.Unit();             // direction along long axis
  temp = *fPos1 + *fPos2;
  fMeanPos = 0.5 * temp;                // position of middle of bar
  fLongOffset = ENullFloat;

  return;
}

//---------------------------------------------------------------------------
LongBar_t::~LongBar_t( )
{
  // No local new store....do nothing
}

