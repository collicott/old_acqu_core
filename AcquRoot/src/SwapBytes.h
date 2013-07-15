//--Author	JRM Annand	21st Jan 2004   Convert Acqu++ -> AcquRoot
//--Rev 	JRM Annand
//--Update	JRM Annand
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
//	SwapBytes
//	Byte swapping for data transfer between Big-Little-Endian systems
//
//---------------------------------------------------------------------------

#ifndef _SwapBytes_h_
#define _SwapBytes_h_

//-----------------------------------------------------------------------------
inline UShort_t Swap2Byte( UShort_t word )
{
// Swap byte order of 2 byte (16-bit) integer

  UChar_t* cw = (UChar_t*)&word;
  UChar_t temp = cw[0];
  cw[0] = cw[1];
  cw[1] = temp;
  return *(UShort_t*)cw;
}

//-----------------------------------------------------------------------------
inline UInt_t Swap4Byte( UInt_t word )
{
// Swap byte order of 4 byte (32-bit) integer

  UChar_t* cw = (UChar_t*)&word;
  UChar_t temp = cw[0];
  cw[0] = cw[3];
  cw[3] = temp;
  temp = cw[1];
  cw[1] = cw[2];
  cw[2] = temp;
  return *(UInt_t*)cw;
}

//-----------------------------------------------------------------------------
inline void Swap4ByteBuff( UInt_t* P, Int_t n)
{
// Swap n bytes in data buffer P..assume its composed of 4-byte words

  n>>=2;                        // bytes to 4-byte words
  Int_t i;			// loop counter
  for(i=0; i<n; i++){
    *P = Swap4Byte(*P);	        // do the 4-byte swap
    P++;			// next word
  }
}

//-----------------------------------------------------------------------------
inline void Swap2ByteBuff( UShort_t* P, Int_t n )
{
// Swap n bytes in data buffer P..assume its composed of 2-byte words

  n>>=1;                        // bytes to 2-byte words
  Int_t i;			// loop counter
  for(i=0; i<n; i++){
    *P = Swap2Byte(*P);	        // do the 2-byte swap
    P++;			// next word
  }
}

//-----------------------------------------------------------------------------
inline void Swap2ShortBuff( UShort_t* P, Int_t n )
{
// Swap (16bit) shorts in data buffer..assume its composed of 4-byte words

  Int_t i;			// loop counter
  UShort_t q;			// temporary variable
  for(i=0; i<n; i++){
    q = *P;
    *P = *(P+1);
    P++;
    *P = q;
    P++;
  }
}

#endif
