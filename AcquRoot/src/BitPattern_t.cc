//--Author	JRM Annand   22nd Jun 2004
//--Rev 	JRM Annand...
//--Update	JRM Annand...
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2BitPattern
// Decode bit-pattern-unit output to create a hit pattern
//

#include "TA2BitPattern.h"
#include "TVector3.h"

ClassImp(TA2BitPattern)

//---------------------------------------------------------------------------
TA2BitPattern::TA2BitPattern( char* fname )
{
  // Setup the pattern from config file
  fNadc = fNADC = fNelement = fNelem = 0;
  fADClist = NULL;
  fPatternList = NULL;
  fHits = NULL;
  FileConfig(fname);
  return;
}

//---------------------------------------------------------------------------
TA2BitPattern::~TA2BitPattern( )
{
  // Free memory allocated to store calibration parameters

  if( fADC ){
    delete fADC;
    delete fADCcut;
  }
  if( fTDC ){
    delete fTDC;
    delete fTDCcut;
  }
  if( fWalk ){
    delete fWalk;
  }
}

//---------------------------------------------------------------------------
void TA2BitPattern::SetConfig( char* line, int key )
{
  // Load basic bit-pattern parameters from file or command line
  Int_t iadc;
  Int_t ebit[16];
  switch( key ){
  case EPatternSize:
    // # of elements in hit pattern
    if( sscanf( line,"%d%d",&fNADC,&fNelement ) != 2 )
      PrintError(line,"<Bit-pattern size setup>");
    fADClist = new UShort_t*[fNADC];
    fPatternList = new UShort_t[fNelement];
    fHits = new Int_t[fNelement];
    break;
  case EPatternADC:
    // parameters for each pattern ADC
    if( sscanf( line,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",&iadc,
		ebit,   ebit+1, ebit+2, ebit+3,
		ebit+4, ebit+5, ebit+6, ebit+7,
		ebit+8, ebit+9, ebit+10,ebit+11,
		ebit+12,ebit+13,ebit+14,ebit+16 ) != 17 )
      PrintError(line,"<Bit-pattern ADC setup>");
    fADCList[fNadc++] = fADC + iadc;
    for( Int_t i=0; i<16; i++ ) fPatternList[fNelem+i] = ebit[i];
    fNelem += 16;
    break;
  default:
    // Pass on any unrecognised line
    PrintError(line,"<Unrecognised command>"
    break;
  }
  return;
}

