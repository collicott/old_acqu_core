//--Author	JRM Annand    9th Sep 2004
//--Rev
//--Update
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// ROOT dictionary specification
// All classes linked in libUserRoot.so must included in the ROOT
// dictionary so that they are recognised by the CINT command processor

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// Monte Carlo classes
//#pragma link C++ class TA2System+;
#pragma link C++ class TMCGenerator+;
#pragma link C++ class TMCFoamGenerator+;
#pragma link C++ class TMCPhotoPSGenerator+;
#pragma link C++ class TMCParticle+;
#pragma link C++ class TMCResonance+;
#pragma link C++ class TMCFoamParticle+;
#pragma link C++ class TMCdS5MDMParticle+;
#pragma link C++ class TMCFoamInt+;
#pragma link C++ class TMCPhotoPSInt+;
#pragma link C++ class TMCdS5MDMInt+;
#pragma link C++ class TMCDgpInt+;

#endif
