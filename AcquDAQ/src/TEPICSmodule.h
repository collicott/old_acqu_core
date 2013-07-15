//--Author	Ken Livingston    23nd Sep 2012  Added support for EPICS
//--Rev 	
//
//--Description
//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TEPICSModule
// Basic class for reading EPICS events


#ifndef __TEPICSmodule_h__
#define __TEPICSmodule_h__

#include "TDAQmodule.h"


class TEPICSmodule : public TDAQmodule {
 protected:
  Short_t  fEPICSID;      // EPICS event ID for the user 
  Int_t    fEpicsIndex;   // Counter for no of EPICS midules
  Char_t   fName[32];     // EPICS event Name
  Char_t  *fDataBuffer;   // Buffer to hold the epics data read in for the event
  Int_t    fBuffLen;      // Length of the data buffer
  Short_t  fNChannels;    // No of EPICS channels to be read
  Char_t **fPVName;       // Array of channel names
  Int_t  *fNelem;         // Array of no of elements for each channel
  Int_t   *fChannelType;  // Array of channel types
  void   **fChannelAddr;  // Array of the addresses of the channels info within the buffer for each channel
  void   **fChannelData;  // Array of the addresses of the data within the buffer for each channel
  Int_t    fPeriodMode;   // Timed or scaler counts
  Long_t   fPeriod;       // Period in ms or # scaler events, depending on fPeriodMode
  Long_t   fCounter;      // Count no of scaler events
  TTimer  *fTimer;        // To deal with timed periodic readout
  Bool_t   fVerbose;      // 
  Bool_t   fIsTimed;      // To flag if it's timed
  Bool_t   fIsCounted;    // To flag if it's periodic
  Bool_t   fIsTimedOut;   // To flag when periodic timed readout is due
  Bool_t   fIsCountedOut; // To flag when periodic #scaler count readout is due
  Bool_t   fReadOnly;     // For reading epics buffers in the datastream
  
  
 public:

  TEPICSmodule(Char_t* name = NULL, Char_t * file = NULL, FILE* log = NULL, Char_t *line = NULL);  
  //Constructor for writing epics into AcuqDAQ data steam (and reading buffs if reqd)

  //functions for decoding EPICS buffer 
  virtual void DumpBuffer(Char_t *buffer = NULL, Char_t *outfile=NULL);      //Dump the epics events from a buffer
  virtual Int_t GetBufferInfo(Short_t*, Short_t *, Short_t*, Char_t *, Short_t *, Char_t *, time_t *); //Get info from and EPICS buffer header
  virtual void *GetChannel(Char_t *, Int_t *, Char_t *, Float_t *, Int_t *); //Get data for a channel in the buffer

  //functions related to writing EPICs channels intot EPICS buffers etc.
  virtual void SetConfig( Char_t*, Int_t );          // configure EPICS module
  virtual void PostInit();                           // initialisation
  virtual void WriteEPICS(void **);                  // Write the epics events from channel acces to the data buffer
  virtual Bool_t HandleTimer(TTimer*);               //Called for timed periodic mode to flag timed out.
  virtual void SetVerbose(Bool_t mode){fVerbose=mode;}
  virtual void Start();                              //Start counting for periodic readout
  virtual Bool_t IsTimedOut(){return fIsTimedOut;}   //Check if timer has run out
  virtual Bool_t IsCountedOut();                     //Check if no of scaler counts is reached
  virtual Bool_t IsTimed(){return fIsTimed;}         //Check if timed periodic readout setup
  virtual Long_t Count(){return fCounter++;}         //Increment the periodic scaler counter
  virtual Bool_t IsCounted(){return fIsCounted;}     //Check if scaler counted periodic readout setup
  virtual void SetEpicsIndex(Int_t i){fEpicsIndex=i;}     //To store the EPICS index of module

  virtual ~TEPICSmodule();

  ClassDef(TEPICSmodule,1)
};


inline void TEPICSmodule::Start()
{ 
  if(fPeriod){                                  //if doing periodic reads 
    if(fTimer){                                 //if a timer is set
      fTimer->Reset();                          //reset the timer
      fTimer->Start(fPeriod, kTRUE);            //start in single shot mode
      fIsTimedOut=kFALSE;                       //flag that it has not timed out
    }  
    else{                                       //if scaler event counting 
      fCounter=0;                               //zero the counter
      fIsCountedOut=kFALSE;                     //flag that it has not counted out
    }
  }
}

inline Bool_t TEPICSmodule::IsCountedOut()
{ 
  if(fCounter < fPeriod) fIsCountedOut=kFALSE; //increment counter and return if not reached the period count
  else fIsCountedOut=kTRUE;
  return fIsCountedOut;
}

#endif
