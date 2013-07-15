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

#include "TA2GenericCluster.h"

// Command-line key words which determine what to read in
static const Map_t kClustDetKeys[] = {
  {"Max-Cluster:",          EGenClustMaxCluster},
  {"Next-Neighbour:",       EGenClustNeighbour},
  {"All-Neighbour:",        EGenClustAllNeighbour},
  {"Split-Off:",            EGenClustSplitOff},
  {"Iterate-Neighbours:",   EGenClustIterate},
  {"Energy-Weight:",        EGenClustEnergyWeight},
  {NULL,          -1}
};


ClassImp(TA2GenericCluster)

//---------------------------------------------------------------------------
TA2GenericCluster::TA2GenericCluster( const char* name, 
					TA2System* apparatus )
  :TA2GenericDetector(name, apparatus)
{
  // Do not allocate any "new" memory here...Root will wipe
  // Set private variables to zero/false/undefined state
  // Add further commands to the default list given by TA2Detector
  AddCmdList( kClustDetKeys );
  fCluster = NULL;
  fClustHit = NULL;
  fIsSplit = NULL;
  fTempHits = NULL;
  fNCluster = fNSplit = fNSplitMerged = fMaxCluster = 0;
  fClustSizeFactor = 1;
  fNClustHitOR = NULL;
  fTheta = fPhi = fClEnergyOR = fClTimeOR = fClCentFracOR = fClRadiusOR = 
    fCldTheta = fCldPhi = fCldX = fCldY = fCldZ = fXmc = fYmc = fZmc = NULL;
  fClEthresh = fEthresh = fEthreshSplit = 0.0;
  fMaxThetaSplitOff = fMinPosDiff  = fMaxPosDiff = 0.0;
  fEWgt = 0.0;
  fLEWgt = 0.0;
  fSplitAngle = NULL;
  fISplit = fIJSplit = NULL;
  fMaxSplitPerm = 0;
  fIsIterate = kFALSE;
  fIsPos = ETrue;          // override standard detector, must have position
}

//---------------------------------------------------------------------------
TA2GenericCluster::~TA2GenericCluster()
{
  // Free up all allocated memory
  // ...arrays created at the initialisation stage
  // Start with common arrays of TA2Detector class
  DeleteClusterArrays();
}

//---------------------------------------------------------------------------
void TA2GenericCluster::DeleteClusterArrays( )
{
  // Free up allocated memory
  DeleteArrays();
  for( UInt_t i=0; i<fMaxCluster; i++ ){
    if( fCluster[i] ) delete fCluster[i];
  }
  delete fCluster;
}

//---------------------------------------------------------------------------
void TA2GenericCluster::SetConfig( char* line, int key )
{
  // Load Ladder parameters from file or command line
  // Keywords which specify a type of command can be found in
  // the kLaddKeys array at the top of the source .cc file
  // The following are setup...
  //	1. # X-triggers, #elements in ladder array, global TDC offset
  //	2. X-trigger parameters if any, TDC, TDC calib, TDC cut
  //	3. Ladder parameters, TDC, calib, cut window, Eg calib, Scaler
  //	4. Any post initialisation
  //	5. Histograms...should be done after post-initialisation

  // Cluster specific configuration
  switch( key ){
  case EGenClustMaxCluster:
    // Max number of clusters
    if( sscanf( line, "%d%lf", &fMaxCluster, &fClEthresh ) < 1 ){
      PrintError(line,"<Parse # clusters>");
      break;
    }
    fEthresh = fClEthresh;
    fCluster = new GenHitCluster_t*[fNelement];
    fClustHit = new UInt_t[fMaxCluster];
    fTempHits = new UInt_t[fNelement];
    fNClustHitOR = new UInt_t[fNelement];
    fTheta = new Double_t[fNelement];
    fPhi      = new Double_t[fNelement];
    fClEnergyOR  = new Double_t[fNelement];
    fClTimeOR  = new Double_t[fNelement];
    fClCentFracOR  = new Double_t[fNelement];
    fClRadiusOR  = new Double_t[fNelement];
    fCldTheta  = new Double_t[fNelement];
    fCldPhi  = new Double_t[fNelement];
    fCldX  = new Double_t[fNelement];
    fCldY  = new Double_t[fNelement];
    fCldZ  = new Double_t[fNelement];
    fXmc  = new Double_t[fNelement];
    fYmc  = new Double_t[fNelement];
    fZmc  = new Double_t[fNelement];
    fNCluster = 0;
    break;
  case EGenClustIterate:
    // Enable iterative cluster-member determination
    // Must be done before any next-neighbour setup
    if( fNCluster ){
      PrintError(line,"<Enable iteration before cluster-neighbour setup>");
      break;
    }
    if( sscanf( line, "%d%lf%lf",
		&fClustSizeFactor, &fMinPosDiff, &fMaxPosDiff ) < 3 ){
      PrintError(line,"<Cluster-iterate parse>");
      break;
    }
    PrintMessage(" Iterative cluster neighbour seek enabled\n");
    fIsIterate = kTRUE;
    break;
  case EGenClustNeighbour:
    // Nearest neighbout input
    if( fNCluster < fNelement )
      fCluster[fNCluster] =
	new GenHitCluster_t(line,fNCluster,fClustSizeFactor,fEWgt,fLEWgt);
    fNCluster++;
    break;
  case EGenClustAllNeighbour:
    // All nearest neighbours (for diagnostics only)
    for(fNCluster=0; fNCluster < fNelement; fNCluster++)
      fCluster[fNCluster] = new GenHitCluster_t( line,fNCluster );
    break;
  case EGenClustSplitOff:
    // Enable split-off search
    if( sscanf( line, "%lf%lf", &fEthreshSplit, &fMaxThetaSplitOff ) < 1 ){
      PrintError(line,"<Parse split off threshold>");
      break;
    }
    fEthresh = fEthreshSplit;          // reset generic threshold
    for(UInt_t i=2; i<fMaxCluster; i++) fMaxSplitPerm += i;
    fSplitAngle = new Double_t[fMaxSplitPerm];
    fISplit = new Int_t[fMaxSplitPerm];
    fIJSplit = new Int_t[fMaxSplitPerm]; 
    fIsSplit = new Bool_t[fMaxCluster];
    break;
  case EGenClustEnergyWeight:
    // Set energy-weighting factor for position determination
    // default = sqrt(E)
    if( sscanf( line, "%lf%d", &fEWgt, &fLEWgt ) < 1 ){
      PrintError(line,"<Parse energy weighting factor>");
    }
    break;
  default:
    // Command not found...try standard detector
    TA2GenericDetector::SetConfig( line, key );
    break;;
  }
  return;
}

//---------------------------------------------------------------------------
void TA2GenericCluster::PostInit()
{
  // Some further initialisation after all setup parameters read in
  // Start with alignment offsets
  // Create space for various output arrays
  fNCluster = 0;    // make sure zeroed
  TA2GenericDetector::PostInit();
}

//-----------------------------------------------------------------------------
void TA2GenericCluster::LoadVariable( )
{
  // Input name - variable pointer associations for any subsequent
  // cut or histogram setup
  // LoadVariable( "name", pointer-to-variable, type-spec );
  // NB scaler variable pointers need the preceeding &
  //    array variable pointers do not.
  // type-spec ED prefix for a Double_t variable
  //           EI prefix for an Int_t variable
  // type-spec SingleX for a single-valued variable
  //           MultiX  for a multi-valued variable

  //  TA2DataManager::LoadVariable("ClEnergy",  &fClEnergy,   EDSingleX);
  //  TA2DataManager::LoadVariable("ClTimeOR",  f???,         EDSingleX);
  //  TA2DataManager::LoadVariable("ClHits",    fClHits,           EDSingleX);
  //  TA2DataManager::LoadVariable("ClMulti",   fClMulti,         EDMultiX);
  TA2DataManager::LoadVariable("ClNhits",       &fNCluster,      EISingleX);
  TA2DataManager::LoadVariable("ClNSplit",      &fNSplit,        EISingleX);
  TA2DataManager::LoadVariable("ClNSplitMerged",&fNSplitMerged,  EISingleX);
  TA2DataManager::LoadVariable("ClTotHits",     fClustHit,       EIMultiX);
  TA2DataManager::LoadVariable("ClTheta",       fTheta,          EDMultiX);
  TA2DataManager::LoadVariable("ClPhi",         fPhi,            EDMultiX);
  TA2DataManager::LoadVariable("ClNhitsOR",     fNClustHitOR,    EIMultiX);
  TA2DataManager::LoadVariable("ClEnergyOR",    fClEnergyOR,     EDMultiX);
  TA2DataManager::LoadVariable("ClTimeOR",      fClTimeOR,       EDMultiX);
  TA2DataManager::LoadVariable("ClCentFracOR",  fClCentFracOR,   EDMultiX);
  TA2DataManager::LoadVariable("ClRadiusOR",    fClRadiusOR,     EDMultiX);
  TA2DataManager::LoadVariable("CldTheta",      fCldTheta,       EDMultiX);
  TA2DataManager::LoadVariable("CldPhi",        fCldPhi,         EDMultiX);
  TA2DataManager::LoadVariable("CldX",          fCldX,           EDMultiX);
  TA2DataManager::LoadVariable("CldY",          fCldY,           EDMultiX);
  TA2DataManager::LoadVariable("CldZ",          fCldZ,           EDMultiX);
  TA2DataManager::LoadVariable("Xmc",           fXmc,            EDMultiX);
  TA2DataManager::LoadVariable("Ymc",           fYmc,            EDMultiX);
  TA2DataManager::LoadVariable("Zmc",           fZmc,            EDMultiX);
  TA2GenericDetector::LoadVariable();
}

//-----------------------------------------------------------------------------
void TA2GenericCluster::ParseDisplay( char* line )
{
  // Input private histogram spec to general purpose parse
  // and setup routine

  // List of local cluster histograms
  const Map_t kClustDetHist[] = {
    {"ClEnergy",         EGenClustEnergy},
    {"ClTime",           EGenClustTime},
    {"ClCentFrac",       EGenClustCentFrac},
    {"ClRadius",         EGenClustRadius},
    {"ClHits",           EGenClustHits},
    {"ClMulti",          EGenClustMulti},
    {NULL,                  -1}
  };

  UInt_t i,j,k,l,chan;
  Double_t low,high;
  Char_t name[EMaxName];
  Char_t histline[EMaxName];

  // Do the rather specialist cluster display setup.
  // This doesn't fit too well with the Name2Variable_t model as
  // the cluster info is stored in separate cluster classes
  // If not specialist cluster display call standard detector display

  if( sscanf(line, "%s%s", histline, name) != 2 ){
    PrintError(line,"<Cluster display - bad format>");
    return;
  }
  i = Map2Key( name, kClustDetHist );
  switch( i ){
  case EGenClustEnergy:
  case EGenClustHits:
  case EGenClustTime:
  case EGenClustMulti:
    // Cluster spectra
    // cluster  chan bins, range low--high, elements j--k,
    if( sscanf(line, "%*s%*s%d%lf%lf%d%d", &chan,&low,&high,&j,&k) != 5 ){
      PrintError(line,"<Cluster display - bad format>");
      return;
    }
    for( l=j; l<=k; l++ ){
      if( l >= fNCluster ){
	PrintError(line,"<Cluster display - element outwith range>");
	return;
      }
      sprintf( histline, "%s%d %d  %lf %lf",name,l,chan,low,high );
      switch( i ){
      case EGenClustEnergy:
	Setup1D( histline, fCluster[l]->GetEnergyPtr() );
	break;
      case EGenClustHits:
	Setup1D( histline, fCluster[l]->GetHits(), EHistMultiX );
	break;
      case EGenClustTime:
	Setup1D( histline, fCluster[l]->GetTimePtr() );
	break;
      case EGenClustMulti:
	Setup1D( histline, fCluster[l]->GetNhitsPtr() );
	break;
      }
    }
    break;
  default:
    // Try the standard display if not specialist cluster stuff
    TA2HistManager::ParseDisplay(line);
    break;
  }

  fIsDisplay = ETrue;
  return;
}

