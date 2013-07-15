//--Author	JRM Annand  2nd Apr 1998 Adapt from lund_def.h (3v2)
//--Rev 	JRM Annand 22nd May 1998 Root typedefs/name conventions
//--Rev 	JRM Annand 18th Feb 1999 Different TAPS formats
//--Rev 	JRM Annand 25th Jul 2002 emun to EnumCont.h
//--Update	JRM Annand 22nd Jan 2007 4v0 update
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// DataFormats
//
// Structs giving the structure of various data formats
// Adapted from earlier versions of ACQU and AcquRoot
//

#ifndef _DataFormats_h_
#define _DataFormats_h_

//--------------------------------------------------------------------------
//
//	ACQU MkI data format 1st
//      Most of these structs are adapted form those in
//	include/c/dmem_module.h
//
//--------------------------------------------------------------------------

//	ACQU experimental header struct
//	formerly called m_dev_header

struct AcquExptHeader_t {
  Char_t time[26];		// time in ascii  
  Char_t exp_desc[133];		// experiment header 
  Char_t run_note[133];		// particular run note 
  Char_t out_file[40];		// output file 
  UShort_t run;			// run number 
  UShort_t n_slave;		// no. of slave VME crates 
  UShort_t n_module;		// total no. modules 
  UShort_t n_vme;	      	// act. no. VME modules 
  UShort_t n_camac;		// act. no. CAMAC modules 
  UShort_t n_fastb;		// act. no. FASTBUS modules 
  UShort_t n_spect;		// act. no readout subaddrs 
  UShort_t n_scaler;		// no. of scalers readout 
  UShort_t C_IRQ;		// no. CAMAC IRQ readout addresses 
  UShort_t C_IRQ_S;		// no. CAMAC scaler readout addresses 
  UShort_t F_IRQ;		// no. FASTBUS modules for IRQ readout 
  UShort_t F_IRQ_S;		// no. FASTBUS modules for scaler readout 
  UShort_t len_buff;		// length of data buff(byte)
};

struct AcquExptInfo_t {	       	// 1st part of header buffer
  Char_t fTime[26];		// run start time (ascii)
  Char_t fDescription[133];	// description of experiment
  Char_t fRunNote[133];		// particular run note
  Char_t fOutFile[40];		// output file
  UShort_t fRun;		// run number
  UShort_t fNslave;		// no. of slave VME crates
  UShort_t fNmodule;		// total no. modules
  UShort_t fNvme;	       	// no. VME modules
  UShort_t fNcamac;		// no. CAMAC modules
  UShort_t fNfastb;		// no. FASTBUS modules
  UShort_t fNspect;		// no. ADC's read out
  UShort_t fNscaler;		// no. scalers readout
  UShort_t fCIrq;	       	// no. CAMAC ADC module readouts
  UShort_t fCIrqS;		// no. CAMAC scaler module readouts 
  UShort_t fFIrq;	       	// no. FASTBUS ADC module readouts
  UShort_t fFIrqS;		// no. FASTBUS scaler module readouts
  UShort_t fRecLen;		// maximum buffer length = record len
};

//	This struct specifies the module from which
//	an ADC or scaler has been read
//	formerly called P_spect

struct ADCInfo_t {
  UShort_t fModIndex;		// index in ModHeader array
  UShort_t fModSubAddr;		// subaddress in module
};
struct Pspect_t {
  UShort_t sp_index;		// index of module ID structure
  UShort_t a;			// module subaddress
};

//	Header struct of a hardware module...
//	Information about a particular piece of hardware
//	formerly called mod_header

struct ModHeader_t{
  Char_t name[20];		// name of module 
  UShort_t vic_crate;		// VME crate no. on VIC bus 
  UShort_t bus_type;		// index vme, camac, fastbus etc. 
  UShort_t mod_type;		// type of module, ADC, latch etc. 
  UShort_t branch;		// branch address 
  UShort_t crate;	       	// crate address 
  UShort_t station;		// crate station address 
  UShort_t a_min;	       	// min subaddress 
  UShort_t a_max;	       	// max subaddress 
  UShort_t bits;	       	// max no. bits from output 
};

struct ModuleInfo_t {
  Char_t fName[20];		// name of module
  UShort_t fVicCrate;		// VME crate no. on VIC bus
  UShort_t fBusType;		// index vme, camac, fastbus etc
  UShort_t fModType;		// type of module, ADC, latch etc.
  UShort_t fBranch;		// branch address (geographical)
  UShort_t fCrate;		// crate address (geographical)
  UShort_t fStation;		// crate station address (geographical)
  UShort_t fAmin;	       	// 1st subaddress
  UShort_t fAmax;	       	// last subaddress
  UShort_t fBits;	       	// max no. bits from output word
};

//	Structure of an error block...written if the front-end readout
//	detects an error
//	formerly called read_error

struct ReadError_t {
  UInt_t fHeader;	       	// error block header
  UShort_t fBus;	       	// bus type
  UShort_t fCrate;		// crate no. (geographical)
  UShort_t fStation;		// station no. (geographical)
  UShort_t fSubAddr;		// subaddress (geographical)
  UShort_t fCode;	       	// error code returned
  UShort_t fAlign;		// to keep long word boundary alignment
};

//	For converting foreign data formats to ACQU

struct AcquBlock_t{
  UShort_t id;			// adc index
  UShort_t adc;			// adc value
};
struct InvAcquBlock_t{
  UShort_t adc;			// adc value
  UShort_t id;			// adc index
};

//---------------------------------------------------------------------------
//	Lund Data Format
//	H.Ruijter data acquisition system
//--------------------------------------------------------------------------

#define		LUND_REC_LEN	16384		// default record length
#define		ADC_BLOCK	0xFDFDFDFD	// ADC readout event header
#define		SCALER_BLOCK	0xFEFEFEFE	// Scaler readout event header
#define		MAX_LUND_ADC	1024		// Maximum no. ADC's read

//	Struct of header at start of each event in data buffer

struct LundEventHeader_t{
  UInt_t header;	        // FDFDFDFD for adc; FEFEFEFE for scaler
  UInt_t res0;		        // "reserved"
  UInt_t n;		        // no. adc's or scalers
  UInt_t res1;		        // "reserved"
  UInt_t event;		        // event number
};

//	Struct of start of header buffer
//	Differs slightly from standard ACQU

struct LundExptHeader_t{
  Char_t time[28];	       	// time in ascii
  Char_t exp_desc[128];	       	// experiment description
  Char_t run_note[128];	       	// run description
  Char_t out_file[40];	       	// data file name
  UInt_t run;		       	// run number
};

//	End of Lund-specific stuff

//---------------------------------------------------------------------------
//	TAPS data format specification
//---------------------------------------------------------------------------

//	Comes at the start of every 8k buffer
//	Note there is a difference between Experimental data
//	and Geant-simulation data

struct TapsExptHeader_t{
  UShort_t lbuffer;	       	// buffer length..exp data
  UShort_t nbuff;      		// buffer counter..exp data
  UShort_t id;	       		// ID = 4
  UShort_t polarisation; 	// beam polarisation info
  UShort_t lgbuffer;	       	// buffer length..geant data
  UInt_t nbuffer;	      	// buffer number
  UShort_t reserved[16];       	// 32 bytes presently unused	
};

//	At the start of every event in the buffer

struct TapsEventHeader_t{
  Short_t levent;	       	// event length
  UShort_t fragment;	       	// event fragmented?
  UShort_t id;		       	// ID = 4
  UShort_t info;				// scaler and polarisation info
};

//	Sub-event specification..at the start of every sub-event within
//	an event....differences between experimental and simulation data

struct TapsSubEvent_t{	       	// experimental header
  UShort_t levent;	       	// length in bytes incl header
  UShort_t id;		       	// subevent type id
};

struct GTapsSubEvent_t{	        // simulation header
  UShort_t id;		       	// subevent type id
  UShort_t levent;	       	// length shorts after header
};

//	Standard TAPS grouping of index and 3 ADC's

struct TapsBlock_t{	       	// 1 of these for each hit det.
  UShort_t id;		       	// id number
  UShort_t adc[3];	       	// TDC, QDC, QDC
};

//	FERA readout TAPS grouping of index and 2 ADC's

struct TapsFera_t{	       	// 1 of these for each hit det.
  UShort_t id;		       	// id number
  UShort_t adc[2];	       	// TDC, QDC
};

//	Veto detector grouping of index and ADC

struct TapsVeto_t{	       	// 1 for every hit veto counter
  UShort_t id;		       	// id number
  UShort_t adc;		       	// charge output
};

//	Sub-event id = 25. Mainz tagger stuff

struct TapsTagger_t{
  UShort_t nevent;	       	// tagger event number
  UShort_t levent;	       	// tagger event length
  UShort_t sevent;	       	// sub-sub event spec.
};
struct TapsTagger1_t{	       	// sevent = 1
  UShort_t id;		       	// channel id
  UShort_t time;	       	// tdc output
};
struct TapsTagger2_t{	       	// sevent = 2
  UShort_t taggeff;	       	// taggeff photon detector QDC
};
struct TapsTagger3_t{	       	// sevent = 3
  UInt_t scalers[352];	       	// tagger scalers
};
struct TapsTagger4_t{	       	// sevent = 4
  UInt_t scalers[12];	       	// tagger camac scalers
};
struct TapsTagger5_t{	       	// sevent = 5
  UShort_t latch[24];	       	// tagger latches
};


//---------------------------------------------------------------------------
//	Data format specification for Giessen test data acquisition
//	system. NB data is a 2-byte unsigned int...here referred to
//	as a word.
//	The header struct comes at the start of each buffer. Individual
//	events are fixed length and have no header
//---------------------------------------------------------------------------

struct GtestEventHeader_t{
  UShort_t id;		       	// buffer identification
  UShort_t nbuffer;	       	// buffer number
  UShort_t lbuffer;	       	// used no. of words in buffer
  UShort_t levent;	       	// no. words in an event
  UInt_t time;		       	// time (seconds since 1/1/70)
  UInt_t reserved;	       	// not used
};


#endif
