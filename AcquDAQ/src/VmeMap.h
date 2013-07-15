//----------------------------------------------------------------------------
//			ACQU
//		Data Acquisition and Analysis
//			for
//		Nuclear Physics Experiments
//
//		UNIX workstation analysis module
//		supported on PC/linux, Digital Unix, Solaris
//		Preferred compiler g++
//
//		    *** VmeMap.h ***
//
//	Define VMEbus mapping to physical address
//
//	JRM Annand	21st Oct 1998	Adapt from 3v3 (gcc)	
//---------------------------------------------------------------------------

#ifndef _VmeMap_h_
#define _VmeMap_h_

//	Eltec 68000 based CPU's

#ifdef E7
//	Physical addresses of VMEbus	
#define		VMEA32D32_P	0x00000000	// A32 D32 extended access 
#define		VMEA24D16_P	0xff000000	// A24 D16 standard access 
#define		VMEA16D16_P	0xffff0000	// A16 D16 access 
#define		VMEA16D16_PE	0xffffffff	// end of A16 D16 access 
//	Kernel virtual addresses of VMEbus	
#define		VMEA32D32_V	0x00000000	// A32 D32 extended access 
#define		VMEA24D16_V	0xff000000	// A24 D16 standard access 
#define		VMEA16D16_V	0xffff0000	// A16 D16 access 
//	VSB access..not normally used	
#define		VSBSTART	0xF0000000	// start VSB access 
#define		VSBEND		0xFCFFFFEF	// end VSB access 
//	Factor 4 which must be applied to IRQ vect for 68040's 
#define		IRQ_VFACT	4
#endif

//	Motorola MVME1603, 1604, 2603, 2604 PowerPC VME platforms

#ifdef MVME160X
//	Physical addresses of VMEbus	
#define		VMEA32D32_P	0xd0000000	// A32 D32 extended access 
#define		VMEA24D16_P	0xe0000000	// A24 D16 standard access 
#define		VMEA16D16_P	0xefff0000	// A16 D16 access 
#define		VMEA16D16_PE	0xefffffff	// end of A16 D16 access 
//	Kernel virtual addresses of VMEbus	
#define		VMEA32D32_V	0xd0000000	// A32 D32 extended access 
#define		VMEA24D16_V	0xc0000000	// A24 D16 standard access 
#define		VMEA16D16_V	0xcfff0000	// A16 D16 access 
//	VSB access..none for MVME1604	
#define		VSBSTART	0		// no VSB 
#define		VSBEND		0
#define		IRQ_VFACT	1		// normal factor 1 
//	PCI->VMEbus hardware Kernel Virtual	
#define		VME2PCI		0xe0802000	// start VME2PCI registers 
//	Normal IRQ vector when load interrupt service	
#define		IRQ_VFACT	1
#endif

//	CES RIO 806X PowerPC-based machines

#ifdef CESRIO806X
//      Physical addresses of VMEbus     
#define          VMEA32D32_P     0xe0000000     // A32 D32 extended access 
#define          VMEA24D16_P     0xee000000     // A24 D16 standard access 
#define          VMEA16D16_P     0xeffe0000     // A16 D16 access 
#define          VMEA16D16_PE    0xeffeffff     // end of A16 D16 access 
//     Kernel virtual addresses of VMEbus       
#define          VMEA32D32_V     0xd0000000     // A32 D32 extended access 
#define          VMEA24D16_V     0xde000000     // A24 D16 standard access 
#define          VMEA16D16_V     0xdffe0000     // A16 D16 access 
//      VSB access...not normally used          
#define          VSBSTART        0xc0000000     // start VSB access        
#define          VSBEND          0x00000000     // end VSB access          
// IRQ_FACT definition for RIO 806x             
#define          IRQ_VFACT       1
#endif

#endif
