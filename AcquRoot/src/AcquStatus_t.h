//----------------------------------------------------------------------------
//			Acqu++
//		Data Acquisition and Analysis
//			for
//		Nuclear Physics Experiments
//
//		C++ UNIX workstation-analysis
//		supported on PC/linux, Digital Unix, Solaris
//		Preferred compiler g++
//
//		    *** AcquStatus_t.h ***

//--Description
//	AcquStatus_t
//	Struct specifying Status information area

//--Author	JRM Annand	10th Sep 2001
//--Update	JRM Annand
//
//---------------------------------------------------------------------------

#ifndef _AcquStatus_t_h_
#define _AcquStatus_t_h_

struct AcquStatus_t{
  unsigned n_event;		// event number...read from input data buffer
  unsigned n_ev_sort;		// no. events sorted to date
  unsigned n_ev_cond;		// no. events past applied conds
  Int_t n_adc;			// no. adc spectra
  Int_t n_uds;			// no. user defined spectra
  Int_t n_1ds;			// no. 1 dimensional spectra
  Int_t n_2ds;			// no. 2 dimensional spectra
  Int_t n_udp;			// no. 1 dimensional physical
  Int_t n_2dp;			// no. 2 dimensional physical
  Int_t n_cond;			// no. applied conditions
  Int_t n_scaler;               // no. of scalers
  Int_t vme_flag;		// flags online input from vme-bus
  Int_t tape_flag;		// flags offline input from tape or disk
  Int_t store_flag;		// flags storeage of reduced data
  Int_t hist_flag;		// flags accumulation of spectra
  Int_t phys_flag;		// flags physical data storage
  Int_t phys_input;	       	// flags physical data input
  Int_t n_camac_err;		// no. CAMAC adc read errors
  Int_t n_fb_err;      		// no. FASTBUS adc read errors
  Int_t n_vme_err;	       	// no. VMEbus adc read errors
  Int_t n_fera_err;	       	// no. FERA adc read errors
  Int_t n_other_err;		// no. OTHER adc read errors
  Int_t n_scaler_err;		// no. scaler read errors
};

#endif
