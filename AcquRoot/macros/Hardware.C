// D.Watts 24th Jan 2004
// J.R.M. Annand 24th Jan 2004...generalise for single & multi-value
//#include "AcquDAQ/src/ModuleIndex.h"
void Hardware( )
{
  // Print details of DAQ hardware
  TA2DataFormat* df = gDS->GetDataFormat(0);
  Int_t n = df->GetNModule();
  Int_t imod, nmod;
  printf("DAQ system comprised of %d hardware modules\n",nmod);
  for( Int_t i=0; i<n; i++ ){
    imod = df->GetModAmin(i);
    nmod = df->GetModNChan(i);
    printf(" ModIndex = %d  # Channels = %d\n", imod, nmod );
  }
  return;
}                                      
