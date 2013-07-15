{
// Basic script to run the AcquRoot system
// JRM Annand 31/01/03

gROOT->Reset();
gROOT->ProcessLine(".L libAcquRoot.so");
//
TAcquRoot* ar=new TAcquRoot();
ar->LinkAcqu();
ar->FileConfig("ROOTsetup.dat");
TRootAnalysis* an = new TRootAnalysis( ar );            // base analyser
an->FileConfig("ROOTanalysis.dat");
ar->SetAnalysis( an );

ar->LoadTreeFiles();
ar->OfflineLoop();
}
