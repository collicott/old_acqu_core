//--Author	JRM Annand    8th Feb 2008
//--Rev         JRM Annand    5th Jun 2009  Prototype polarimetry	
//--Rev         JRM Annand   18th Feb 2010  Asimuthal scattering angle	
//--Update      JRM Annand   24th Mar 2010  Clean up, add angle limits	
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2TOFApparatus
//
// Apparatus class to service TOF arrays.
// Take information from TA2LongScint, and TA2Ladder detector classes
//

#include "TA2TOFApparatus.h"
#include "TAcquRoot.h"

// Default list of detector classes that the TA2TOFApparatus 
// apparatus may contain
enum { EGeneric, ELongScint, ELadder, EGenericClust };
static Map_t kValidDetectors[] = {
  {"TA2LongScint",          ELongScint},
  {"TA2Ladder",             ELadder},
  {"TA2GenericDetector",    EGeneric},
  {"TA2GenericCluster",     EGenericClust},
  {NULL, 		-1}
};
// Valid Keywords for command-line setup of CB apparatus
enum { ETOFTimeZero = 1000, ETOFLadder, ETOFPolarimeter };
static const Map_t kTOFKeys[] = {
  {"Time-Zero:",    ETOFTimeZero},
  {"Ladder:",       ETOFLadder},
  {"Polarimeter:",  ETOFPolarimeter},
  {NULL,            -1}
};

ClassImp(TA2TOFApparatus)

//-----------------------------------------------------------------------------
TA2TOFApparatus::TA2TOFApparatus( const char* name, TA2System* analysis )
  :TA2Apparatus( name, analysis, kValidDetectors )
{
  // Pointers to detectors not initialised
  fDet = NULL;
  fTOF = NULL;
  fLadder = NULL;
  fNDet = fNTOF = fNTOFbar = 0;
  fBeta = fFlightTime = fScatTheta = fScatPhi = fDiffTheta = NULL;
  fNPolScatter = 0;
  fTimeZero = 0;
  fPolAnal = fPolAsym = NULL;
  fPolTdiff = fPolEsum = NULL;
  AddCmdList( kTOFKeys );                  // for SetConfig()

}


//-----------------------------------------------------------------------------
TA2TOFApparatus::~TA2TOFApparatus()
{
  // Delete any owned "new" memory here
  DeleteArrays();
}

//-----------------------------------------------------------------------------
void TA2TOFApparatus::DeleteArrays()
{
  // Delete any owned "new" memory here
  TA2Apparatus::DeleteArrays();
  if( fDet ){
    for(Int_t i=0; i<fNDet; i++) delete fDet[i];
    delete fDet;
  }
  if( fBeta ){
    delete fBeta;
    delete fFlightTime;
  }
}

//-----------------------------------------------------------------------------
void TA2TOFApparatus::SetConfig(Char_t* line, Int_t key)
{
  // Any special command-line input for Crystal Ball apparatus
  Char_t nameAn[64];
  Char_t nameAsy[64];
  Int_t nParm;
  switch (key){
  case ETOFTimeZero:
    // Zero time point
    if( sscanf(line,"%lf",&fTimeZero) != 1 ){
      PrintError(line,"<TOF Zero Time>");
      return;
    }
    break;
  case ETOFLadder:
    // Use if ladder is specified elsewhere
    //    fLadder = new TA2Ladder( name, this );
    //    fDet[fNDet] = fLadder;
    break;
  case ETOFPolarimeter:
    // Basic polarimetry setup
    nParm = sscanf(line,"%s%s%lf%lf%lf%lf%lf%lf",
		   nameAn,nameAsy,&fTmin,&fTmax,&fEmin,&fEmax,
		   &fThetaMin,&fThetaMax);
    switch( nParm ){
    default:
      PrintError(line,"<TOF Polarimeter>");
      break;
    case 8:
      break;
    case 6:
      fThetaMax = 180;
      fThetaMin = 0;
      break;
    }
    if( !(fPolAnal =
	  (TA2GenericCluster*)GetChild( nameAn,"TA2GenericCluster" )) ) break;
    if( !(fPolAsym =
	  (TA2GenericCluster*)GetChild( nameAsy,"TA2GenericCluster" )) ){
      fPolAnal = NULL;
      break;
    }
    fScatTheta = new Double_t[EMaxPolAngle + 1];
    fScatPhi = new Double_t[EMaxPolAngle + 1];
    fDiffTheta = new Double_t[EMaxPolAngle + 1];
    fPolTdiff = new Double_t[EMaxPolAngle + 1];
    fPolEsum = new Double_t[EMaxPolAngle + 1];
    break;
  default:
    // default main apparatus SetConfig()
    TA2Apparatus::SetConfig( line, key );
    break;
  }
}

//-----------------------------------------------------------------------------
TA2DataManager*  TA2TOFApparatus::CreateChild(const char* name, Int_t dclass)
{
  // Create a TA2Detector class for use with the TA2TOFApparatus
  // Valid detector types in the following switch
  // Create detector list if not already done.
  // Chech if the max number detectors exceeded

  if( !fDet ){
    fDet = new TA2Detector*[EMaxTOFDetectors];
    fTOF = new TA2LongScint*[EMaxTOFDetectors];
  }
  if( fNDet >= EMaxTOFDetectors ){
    PrintError( "<Too many detectors in TOF apparatus>");
    return NULL;
  }

  if( !name ) name = Map2String( dclass );
  switch (dclass){
  default:
    return NULL;
  case EGeneric:
    // Basic detector array
    fDet[fNDet] = new TA2GenericDetector( name, this );
    break;
  case EGenericClust:
    // Basic detector array
    fDet[fNDet] = new TA2GenericCluster( name, this );
    break;
  case ELongScint:
    // Long scintillator arrays...standard TOF counters
    fDet[fNDet] = fTOF[fNTOF] = new TA2LongScint( name, this );
    fNTOFbar += fTOF[fNTOF]->GetNBar();
    fNTOF++;
    break;
    /*  case ELadder:
    // Use only if ladder not specified elsewhere
    fLadder = new TA2Ladder( name, this );
    fDet[fNDet] = fLadder;
    break;
    */
  }
  fNDet++;
  return fDet[fNDet-1];
}

//-----------------------------------------------------------------------------
void TA2TOFApparatus::LoadVariable( )
{
  // Input name - variable pointer associations for any subsequent
  // cut or histogram setup.
  // LoadVariable( "name", pointer-to-variable, type-spec );
  // NB scaler variable pointers need the preceeding &
  //    array variable pointers do not.
  // type-spec ED prefix for a Double_t variable
  //           EI prefix for an Int_t variable
  // type-spec SingleX for a single-valued variable
  //           MultiX  for a multi-valued variable

  //                            name        pointer          type-spec
  TA2DataManager::LoadVariable("Beta",        fBeta,           EDMultiX);
  TA2DataManager::LoadVariable("FlightTime",  fFlightTime,     EDMultiX);
  TA2DataManager::LoadVariable("ScatTheta",   fScatTheta,      EDMultiX);
  TA2DataManager::LoadVariable("ScatPhi",     fScatPhi,        EDMultiX);
  TA2DataManager::LoadVariable("DiffTheta",   fDiffTheta,      EDMultiX);
  TA2DataManager::LoadVariable("PolTdiff",    fPolTdiff,       EDMultiX);
  TA2DataManager::LoadVariable("PolEsum",     fPolEsum,        EDMultiX);
  TA2DataManager::LoadVariable("NPolScatter", &fNPolScatter,   EISingleX);
  //
  TA2Apparatus::LoadVariable();
}

//-----------------------------------------------------------------------------
void TA2TOFApparatus::PostInit( )
{
  // Some further initialisation after all setup parameters read in
  // default Cut setup
  //
  fMaxParticle = EMaxTOFDetectors;
  fBeta = new Double_t[fMaxParticle];
  fFlightTime = new Double_t[fMaxParticle];
  TA2Apparatus::PostInit();
}


//-----------------------------------------------------------------------------
void TA2TOFApparatus::Reconstruct( )
{
  // Calculate flight-path corrected flight times, velocity and 
  // for non-relativistic particles calculate 4 momenta for TOF hits
  //
  TA2LongScint* ls;
  Int_t i,j,k,m,n;
  Int_t* hits;
  TVector3* dir;
  //  Double_t x,y,z;
  for( i=0,m=0; i<fNTOF; i++ ){
    ls = fTOF[i];
    n = ls->GetNBarHits();
    hits = ls->GetBarHits();
    for( j=0; j<n; j++ ){
      if( m >= fMaxParticle ) break;
      k = hits[j];
      fFlightTime[m] = 100 * (ls->GetMeanTime(k) - fTimeZero)/ls->GetFlight(k);
      fBeta[m] = kConvBeta * ls->GetFlight(k)/(ls->GetMeanTime(k) - fTimeZero);
      dir = ls->GetBarPos() + k;
      //      x = dir->X();
      //      y = dir->Y();
      //      z = dir->Z();
      fPDG_ID[i] = kGamma;
      if( fPCut ){                          // If cuts sorted
	Int_t mc = fPCutStart[i];           // Start cut index for particle i
	for( Int_t n=0; n<fNSectorCut[i]; n++ ){ // Loop over specified cuts
	  if( fPCut[mc]->Test() ){           // Condition met?
	    fPDG_ID[i] = GetCutPDGIndex(mc); // OK get associated PDG index
	    fParticleID->SetP4tof(fP4+m,kProton,fBeta[m],dir);
	    break;                          // cut OK so exit loop
	  }
	  mc++;                              // for next try
	}
      }
       m++;
    }
  }
  fBeta[m] = EBufferEnd;
  fFlightTime[m] = EBufferEnd;
  fNparticle = m;
  RecPol();
}

//-----------------------------------------------------------------------------
void TA2TOFApparatus::RecPol( )
{
  // Prototype double scattering polarimetry

  Int_t j = 0;
  if( !fPolAnal ) return;
  Int_t n1 = fPolAnal->GetNCluster();
  if( n1 ){
    TVector3 anp = fPolAnal->GetHitPos(fPolAnal->GetHits(0));
    TVector3 asp = fPolAsym->GetHitPos(fPolAsym->GetHits(0));
    Int_t n2 = fPolAsym->GetNCluster();
    if( n2 ){
      TVector3 v = fPolAnal->GetVertex();
      GenHitCluster_t** anCl = fPolAnal->GetCluster();
      UInt_t* idAn = fPolAnal->GetClustHit();     // Anal indices active clusts
      GenHitCluster_t** asyCl = fPolAsym->GetCluster();
      UInt_t* idAsy = fPolAsym->GetClustHit();    // Asym indices active clusts
      TVector3* anPos;
      TVector3* asyPos;
      TVector3 diffPos;
      Double_t tdiff,esum;
      GenHitCluster_t* anC;
      GenHitCluster_t* asyC;
      for( Int_t i1=0; i1<n1; i1++ ){
	anC = anCl[idAn[i1]];
	anPos = anC->GetMeanPosition();
	for( Int_t i2=0; i2<n2; i2++ ){
	  Double_t th,th1,ph;
	  //	  Double_t xx1,yy1,zz1,th1,ph1;
	  asyC = asyCl[idAsy[i2]];
	  asyPos = asyC->GetMeanPosition();
	  diffPos = *asyPos - *anPos;
	  tdiff = asyC->GetTime() - anC->GetTime();
	  esum = asyC->GetEnergy() + anC->GetEnergy();
	  if( (tdiff < fTmin) || (tdiff > fTmax) ) continue;
	  if( (esum < fEmin) || (esum > fEmax) ) continue;
	  th = anPos->Theta();
	  ph = anPos->Phi();
	  diffPos.RotateZ(-ph);
	  diffPos.RotateY(-th);
	  th1 = diffPos.Theta() * TMath::RadToDeg();
	  if( (th1 < fThetaMin) || (th1 > fThetaMax) ) continue;
	  fPolTdiff[j] = tdiff;
	  fPolEsum[j] = esum;
	  fScatPhi[j] = diffPos.Phi() * TMath::RadToDeg();
	  fScatTheta[j] = th1;
	  diffPos = diffPos - anp + asp;
	  fDiffTheta[j] = diffPos.Theta() * TMath::RadToDeg();
	  j++;
	  if( j >= EMaxPolAngle )break;
	} 
      }
    }
  }
  fScatTheta[j] = EBufferEnd;
  fScatPhi[j] = EBufferEnd;
  fDiffTheta[j] = EBufferEnd;
  fPolTdiff[j] = EBufferEnd;
  fPolEsum[j] = EBufferEnd;
  fNPolScatter = j;
}
    
