//--Author	JRM Annand    4th Apr 2006
//--Rev 	JRM Annand...
//--Rev 	JRM Annand... 9th Feb 2007  Integrate with AcquRoot
//--Rev 	JRM Annand... 7th Sep 2010  getenv("HOST") not "localhost"
//--Rev 	JRM Annand... 9th Sep 2010  add data no-store
//--Update	JRM Annand... 1st Oct 2012  print message when connecting
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TDAQstore
// Storeage of data read out from experimental hardware by DAQ
// Typically the data is stored on local disk or send over a network socket

#ifndef _TDAQstore_h_
#define _TDAQstore_h_

#include "TA2System.h"
#include "TA2RingBuffer.h"

class ARFile_t;                  // I/O to file
class ARSocket_t;                // I/O via net socket

// Type of data output channel
enum { EStoreDOUndef, EStoreDONet, EStoreDODisk, EStoreDOAcquRoot };
 
class TDAQstore : public TA2System {
 protected:
  TA2RingBuffer* fOutBuff;        // ring data buffer
  ARSocket_t* fSocket;		 // for TCP/IP communications
  ARFile_t* fDataOutFile;        // file to write local output
  Char_t* fFileName;             // file name spec
  Int_t fRecLen;	   	 // no. of bytes in record
  Int_t fPacLen;                 // network transfer packet length
  Int_t fPort;                   // network port
  Int_t fMode;                   // net-socket local(accept)/remote(connect)
  Int_t fNfile;			 // number files processed
  Int_t fNrecord;		 // number records processed
  //  Int_t fRingSize;               // # elements in ring buffer
  Bool_t fIsSwap;                // byte swapping?

 public:
  TDAQstore( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TDAQstore();
  virtual void SetConfig(char*, Int_t){}
  virtual void PostInit( );
  virtual void Process();	   // Process and send data
  virtual void Shutdown() { }	   // Shut down the data source
  virtual void Run();		   // contains all the work

  void SetOutBuff(TA2RingBuffer* b){
    fOutBuff = new TA2RingBuffer( b );
  }
  TA2RingBuffer* GetOutBuff(){ return fOutBuff; }    
  ARSocket_t* GetSocket(){ return fSocket; }
  ARFile_t* GetDataOutFile(){ return fDataOutFile; }
  Char_t* GetFileName(){ return fFileName; }
  void SetFileName(Char_t* name){ fFileName = name; }
  Int_t GetRecLen(){ return fRecLen; }
  Int_t GetPacLen(){ return fPacLen; }
  Int_t GetPort(){ return fPort; }
  Int_t GetMode(){ return fMode; }
  Int_t GetNfile(){ return fNfile; }
  Int_t GetNrecord(){ return fNrecord; }
  //  Int_t GetRingSize(){ return fRingSize; }
  Bool_t IsSwap(){ return fIsSwap; }
  Bool_t IsStore(){ 
    if( fMode ==  EStoreDOUndef )return kFALSE;
    else return kTRUE;
  }
  ClassDef(TDAQstore,1)       	// for ROOT
    };

#endif
