//--Author	JRM Annand   30th Sep 2003
//--Update	JRM Annand...29th Nov 2007  Add libGui and libFoam
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
// ROOT script to make html'ised copy of the Acqu-Root Classes
//
{
  gROOT->Reset();
  gSystem->Load( "libPhysics.so" );    	          // Vectors etc.
  gSystem->Load( "libFoam.so" );    	          // Foam MC stuff
  gSystem->Load( "libGui.so" );    	          // GUI stuff
  gSystem->Load( "$acqu_sys/lib/$ACQU_OS_NAME/libAcquRoot.so" );// AR lib
  gSystem->Load( "$acqu/lib/$ACQU_OS_NAME/libUserRoot.so" );    // User lib
  gSystem->Load( "$acqu_sys/lib/$ACQU_OS_NAME/libAcquMC.so" );  // MC lib
  gSystem->Load( "$acqu/lib/$ACQU_OS_NAME/libMCUserRoot.so" );  // User MC
  gSystem->Load( "$acqu_sys/lib/$ACQU_OS_NAME/libAcquDAQ.so" ); // DAQ library
  THtml h;
  h.MakeAll();
  return;
}
