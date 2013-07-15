//--Author	JRM Annand	22nd Feb 2003
//--Rev 	JRM Annand       4th Feb 2004
//--Rev         Sven Schumann   25th Mar 2004  SetBinContent() fix
//--Rev 	JRM Annand      31st Mar 2004  Scaler plot
//--Rev 	JRM Annand      24th Jun 2004  Scaler plot upper range bug
//--Rev 	JRM Annand      27th Jun 2004  Rate or multi-scaler plot
//--Rev 	JRM Annand      10th Feb 2005  Template issues gcc 3.4
//--Rev 	JRM Annand      10th May 2005  Complete weight installation
//--Update	JRM Annand      22nd Jul 2005  fix errors gcc 4.0
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data.
//
// TA2H1
// Wrapper class for TH1F 1D histogram class
// adds pointer to x variable to be histogramed and a pointer to a
// weight variable. Able to handle single or multi value
// variables through child classes of an abstract  base class

#ifndef __TA2H1_h__
#define __TA2H1_h__

#include "TH1F.h"
#include "TA2H.h"

template< typename T > class TA2H1 : public TA2H, public TH1F {
 protected:
  T* fHistBuff;
 public:
  TA2H1(){}
  TA2H1( char* n, const char* t, Int_t ch, Axis_t low, Axis_t high, T* data,
	 TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H(cut,wgt),
    TH1F(n,t,ch,low,high) { fHistBuff = data; }
  TA2H1( char* n, const char* t, Int_t ch, Double_t* xch, T* data,
	 TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H(cut,wgt),
    TH1F(n,t,ch,xch) { fHistBuff = data; }

  virtual ~TA2H1(){}
  T* GetHistBuff(){ return fHistBuff; }     // return data buffer ptr
  //  ClassDef(TA2H1,1)   
};

// Single-value per event histogram
template< typename T > class TA2H1S: public TA2H1<T>{
 public:
  TA2H1S(){};
  TA2H1S( char* n, const char* t, Int_t ch, Axis_t low, Axis_t high, T* data,
	  TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H1<T>(n,t,ch,low,high,data,cut,wgt){}
  TA2H1S( char* n, const char* t, Int_t ch, Double_t* xch, T* data,
	  TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H1<T>(n,t,ch,xch,data,cut,wgt){}
  virtual ~TA2H1S(){}
  using TA2H1<T>::fHistBuff;   // gcc 3.4
  using TA2H1<T>::fWeight;     // gcc 3.4
  using TA2H1<T>::fCut;        // gcc 3.4
  void Fill()
    { 
      // Fill histogram with single value held in buffer
      // No action if "null" marker found in buffer
      // Check condition 1st (if any)
      if( fCut ){ if( !fCut->Test() ) return; }
      T* hb = fHistBuff;
      if( *hb != ((T)ENullHit) )
	TH1F::Fill( (Float_t)(*hb), *fWeight );
      return;
    }
};

// Multi-value per event histogram  
template< typename T > class TA2H1M: public TA2H1<T>
{
 public:
  TA2H1M(){};
  TA2H1M( char* n, const char* t, Int_t ch, Axis_t low, Axis_t high, T* data,
	  TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H1<T>(n,t,ch,low,high,data,cut,wgt){}
  TA2H1M( char* n, const char* t, Int_t ch, Double_t* xch, T* data,
	  TA2Cut* cut = NULL, Stat_t* wgt = NULL ):
    TA2H1<T>(n,t,ch,xch,data,cut,wgt){}
  virtual ~TA2H1M(){}
  using TA2H1<T>::fHistBuff;   // gcc 3.4
  using TA2H1<T>::fWeight;     // gcc 3.4
  using TA2H1<T>::fCut;        // gcc 3.4
  void Fill()
    {
      // Fill histogram until end-marker found in buffer
      // Check condition 1st (if any)
      if( fCut ){ if( !fCut->Test() ) return; }
      T* hb = fHistBuff;
      while( *hb != (T)EBufferEnd ){
	TH1F::Fill( (Float_t)(*hb), *fWeight );
	hb++;
      }
    }
};

// Flash ADC pulse form plot
template< typename T > class TA2H1MF: public TA2H1<T>
{
 private:
  Int_t fChan;
 public:
  TA2H1MF(){};
  TA2H1MF( char* n, const char* t, Int_t ch, T* data ):
    TA2H1<T>(n,t,ch,(Axis_t)0,(Axis_t)(ch-1),data){ fChan = ch; }
  virtual ~TA2H1MF(){}
  using TA2H1<T>::fHistBuff;   // gcc 3.4
  using TA2H1<T>::fCut;        // gcc 3.4
  void Fill()
    {
      // transfer flash ADC samples into histogram internal data array
      // note SetBinContent(0,...) doesn't work
      T* hb = fHistBuff;
      for( int i=0; i<fChan; i++ ){
	TH1F::SetBinContent( i+1, (Float_t)(hb[i]) );
      }
    }
};

// Scaler plot
template< typename T > class TA2H1MS: public TA2H1<T>
{
 private:
  Int_t fChan;
  Bool_t* fIsScalerRead;
 public:
  TA2H1MS(){};
  TA2H1MS( char* n, const char* t, Int_t ch, T* data, Bool_t* scflag ):
    TA2H1<T>(n,t,ch,(Axis_t)0,(Axis_t)(ch),data)
    { fChan = ch; fIsScalerRead = scflag; }
  virtual ~TA2H1MS(){}
  using TA2H1<T>::fHistBuff;   // gcc 3.4
  using TA2H1<T>::fCut;        // gcc 3.4
  void Fill()
    {
      // transfer scaler counts into histogram internal data array
      // note SetBinContent(0,...) doesn't work.
      // do only if scaler read flag set
      if( ! *fIsScalerRead ) return;
      T* hb = fHistBuff;
      for( int i=0; i<fChan; i++ ){
	TH1F::SetBinContent( i+1, (Float_t)(hb[i]) );
      }
    }
};

// Scaler Rate monitor plot
template< typename T > class TA2H1SR: public TA2H1<T>{
 private:
  Int_t fChan;
  Int_t fNChan;
  Int_t fFrequency;
  Int_t fFreqCounter;
  T fMean;
  Bool_t* fIsScalerRead;
 public:
  TA2H1SR(){};
  TA2H1SR( char* n, const char* t, Int_t ch, T* data, Int_t freq,
	   Bool_t* scflag ):
    TA2H1<T>(n,t,ch,(Axis_t)0,(Axis_t)ch,data)
    {
      fFrequency = freq; fIsScalerRead = scflag; fNChan = ch;
      fChan = 0; fFreqCounter = 0; fMean = 0;
    }
  virtual ~TA2H1SR(){}
  using TA2H1<T>::fHistBuff;   // gcc 3.4
  using TA2H1<T>::fCut;        // gcc 3.4
  void Fill()
    { 
      // transfer scaler-related info e.g. rates ratios
      // into histogram internal data array
      // note SetBinContent(0,...) doesn't work.
      // do only if scaler read flag set
      if( ! *fIsScalerRead ) return;
      fFreqCounter++;
      fMean += *fHistBuff;
      if( fFreqCounter < fFrequency ) return;
      fFreqCounter = 0;
      TH1F::SetBinContent( fChan, fMean/fFrequency );
      fMean = 0;
      fChan++;
      if( fChan >= fNChan ){
	fChan = 0;
	TH1F::Reset();
      }
      return;
    }
};

#endif

