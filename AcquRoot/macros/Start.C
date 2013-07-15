//void Start( char* mode = "offline" )
{
// Basic script to run the AcquRoot system
// JRM Annand 06/02/03
  gROOT->Reset();
  int online = 0;
  //  if(strcmp(mode,"online") == 0) online = 1;

  gSystem->Load( "libAcquRoot.so" );   	// Acqu-Root class library
  gSystem->Load( "libThread.so" );     	// TThread library

  TAcquRoot* ar=new TAcquRoot("Test");
  ar->LinkAcqu();
  ar->FileConfig("ROOTsetup.dat");	// setup Acqu-Root from parameter file

  if( online ) ar->SetOnline();
  else ar->SetOffline();

  TRootAnalysis* an = new TRootAnalysis("PiPTOF",ar );            // base analyser
  an->FileConfig("ROOTanalysis.dat");
  //  an->Initialise();
  ar->SetAnalysis( an );

  ar->Start();				// start the analysis as thread
}
