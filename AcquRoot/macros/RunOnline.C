{
// Basic script to run the AcquRoot system from standard Root
// JRM Annand 31/01/03

gROOT->Reset();
gROOT->ProcessLine(".L libAcquRoot.so");
//
TAcquRoot* ar=new TAcquRoot();
ar->LinkAcqu();
ar->FileConfig("ROOTsetup.dat");
ar->SetOnline();
TRootAnalysis* an = new TRootAnalysis( ar );            // base analyser
ar->FileConfig("ROOTanalysis.dat");
an->Initialise();
ar->SetAnalysis( an );

//ar->LoadTreeFiles();
//ar->OfflineLoop();
ar->Mk1OnlineLoop();
}
