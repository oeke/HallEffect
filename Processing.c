// Header files                                        //
//-----------------------------------------------------//
#include <cdefBF533.h>
// Header File for using Fractional Data Format in C and
// its special functions 
#include <fract.h>
#include <fract2float_conv.h>
#include "DefConst.h"
#include <math.h>
//-----------------------------------------------------//


//-----------------------------------------------------//
// Externe Variables                                    //
//-----------------------------------------------------//
extern unsigned char  ChipVer;        // Chip-Realize
extern unsigned int   LEDs;           // LED-Pattern
//-------------------------------
extern fract16        fInp_1;
extern fract16        fInp_2;
extern fract16        fInp_3;
extern fract16        fInp_4;
//-------------------------------
extern fract16        fOut_1;
extern fract16        fOut_2;
extern fract16        fOut_3;
extern fract16        fOut_4;
extern fract16          fKof[11];
extern fract16          fKof2[11];

//-------------------------------
extern int iTxBuffer[];         // SPORT0 DMA transmit buffer
extern int iRxBuffer[];         // SPORT0 DMA receive buffer
//-------------------------------
fract32		fBuf[4096];	//Buffer Array
//fract16		fKof[4096];	//Koeffizienten Array
int 		ic=0;		//Buffer-Load Variable
fract32		fAcc;		//Hall Akkumulator für die Rückkopplung
int 		iKoAnz;		// Anzahl an verwendeten Koeffizienten
fract16     fTemp;
fract16     fTemp2;
fract16     fTIn;
int         ip=0;

fract16     fIn;
fract32     fTmp_32;
fract32     fTmp2_32;
fract16     fTmp_16;
fract16     fTmp2_16;

//-------------------------------	//Skalierungsfaktoren
 float		flInp_1Scale = 0.5;	//Skalierungsfaktor Eingang (fInp_1Scale+fAccScale =1)
 float		flAccScale = 0.3;	//Skalierter Ausgang

 float		flKoeffAnz = 0.03448275862068965517;
 float        tmp;
//extern fract16		float_to_fr16(flKoeffAnz) =1/iKoAnz;	//bei iKoAnz (29) Koeffizienten	

//---------------------------------------------------------------------------
// Prototypes                                                              //
//---------------------------------------------------------------------------
void  Process_A(void);              // PROCESS_A Signal-Processing 
void  Process_B(void);              // PROCESS_B Signal-Processing 
void  Process_C(void);              // PROCESS_C Signal-Processing 
void  Process_D(void);              // PROCESS_D fSignal-Processing 
//------------------------
void  Process_Tim0(void);           // PROCESS_Timer_0 Signal-Processing  
//---------------------------------------------------------------------------
void  PutDAC(short,fract16);        // Data to DAC
//---------------------------------------------------------------------------



//----------------------------------------------------------------------------
// Function:    PROCESS_A for Signal-Processing                             //
// ---------------------------------------                                  //
// Description:                                                             //
//                                                                          //
//                                                                          //  
//----------------------------------------------------------------------------
void Process_A(void)                // (For Test Only - Talkthrough) 
 { //------------------------
   
 	fIn    =   fInp_1;
    	
	
	ip=ip&0x00000fff;				// Kreisindizierung mit 4096 indizes
	/*
	fTmp_16=0.6r16;					// eingangskoeffizient
	fBuf[ip]=fTmp_16*fIn;*/			// 16 bit Multiplikation mit neuem Wert, ergbnis 32 bit
	fBuf[ip]=fIn<<15;					// 1 mal schieben um korrektes ergebnis zu erhalten
	/*fTmp_16=0.7r16;					// Rückkopplungskoeffizient
	fTmp2_16=fAcc>>16;				// Akkumulator 32 bit in 16 variable laden
	fTmp_32=fTmp_16*fTmp2_16;*/		// 16 Multiplikation mit 32 bit ergebnis
	fBuf[ip]+=fAcc>>2;			// ergebnis korrigieren
	int k=0;						// koeffizienten index
	if(ic>4095){
		fAcc=0;
		
		fTmp_16=fKof[0];
		k=ip;
		k=k&0x00000fff;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32>>1;
		fAcc+=fTmp2_32;
		
		fTmp_16=fKof[1];
		k=ip+500;
		k=k&0x00000fff;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32>>1;
		fAcc+=fTmp2_32;
		
		fTmp_16=fKof[2];
		k=ip+1100;
		k=k&0x00000fff;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32>>1;
		fAcc+=fTmp2_32;
		
		fTmp_16=fKof[3];
		k=ip+1800;
		k=k&0x00000fff;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32>>1;
		fAcc+=fTmp2_32;
		
		fTmp_16=fKof[4];
		k=ip+2400;
		k=k&0x00000fff;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32>>1;
		fAcc+=fTmp2_32;
		
		fTmp_16=fKof[5];
		k=ip+2700;
		k=k&0x00000fff;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32>>1;
		fAcc+=fTmp2_32;
		
		fTmp_16=fKof[6];
		k=ip+3000;
		k=k&0x00000fff;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32>>1;
		fAcc+=fTmp2_32;
		
		fTmp_16=fKof[7];
		k=ip+3250;
		k=k&0x00000fff;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32>>1;
		fAcc+=fTmp2_32;
		
		fTmp_16=fKof[8];
		k=ip+3500;
		k=k&0x00000fff;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32>>1;
		fAcc+=fTmp2_32;
		
		fTmp_16=fKof[9];
		k=ip+3700;
		k=k&0x00000fff;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32>>1;
		fAcc+=fTmp2_32;
		
		fTmp_16=fKof[10];
		k=ip+4000;
		k=k&0x00000fff;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32>>1;
		fAcc+=fTmp2_32;
		
		/*fTmp_16=0.8r16;				// 1. koeffizient
		k=ip+1000;					// koeffizientenindex relativ zum letzten wert
		k=k&0x00000fff;				// kreisindizierung des koeffizientenindex, max 4095
		fTmp2_16=fBuf[k]>>16;		// 32 bit buffer in 16 variable 
		fTmp2_32=fTmp2_16*fTmp_16;	// 16 bit multiplikation mit 32 bit ergebniss
		fTmp2_32<<1;				// korrigieren des ergebnis'	
		fAcc+=fTmp2_32;				// Wert akkumulieren
		
		fTmp_16=0.5r16;
		k=ip+3000;
		k=k&0x00000fff;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32<<1;
		fAcc+=fTmp2_32;
		
		fTmp_16=0.25r16;
		k=ip+4000;
		k=k&0x00000fff;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32<<1;
		fAcc+=fTmp2_32;*/
		
		/*fTmp_16=0.27r16;
		k=ip+3800;
		if(k>=4095)
		    k-=4095;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32<<1;
		fAcc+=fTmp2_32;*/
	}
   
     if(ic<4096){			//c max 4095
    		ic++;			//Buffer load erhöhen, falls nicht schon voll
    	}
      ip++;                 // pointer auf ringbuffer (kreisindizierung)
	
    fOut_1=fAcc>>16;	        // akkumulator wert auf beide ausgänge legen
	fOut_2=fOut_1;
   
   PutDAC(DAC_1R, fOut_1);  // Write DAC 1R
   PutDAC(DAC_1L, fOut_2);  // Write DAC 1L
   //------------------------
 }
//--------------------------------------------------------------------------//


//--------------------------------------------------------------------------//
// Function:    PROCESS_B for Signal-Processing                             //
// ---------------------------------------                                  //
// Description:                                                             //
//                                                                          //
//                                                                          //  
//--------------------------------------------------------------------------//
void Process_B(void) 
 { 
     
    fTmp_16=0.5r16;
	fTmp2_32=fInp_1*fTmp_16;
	fOut_1=fTmp2_32>>15; 
	
	
	PutDAC(DAC_1R, fOut_1);
	PutDAC(DAC_1L, fOut_1);
 }
//--------------------------------------------------------------------------//


//--------------------------------------------------------------------------//
// Function:    PROCESS_C for Signal-Processing                             //
// ---------------------------------------                                  //
// Description:                                                             //
//                                                                          //
//                                                                          //  
//--------------------------------------------------------------------------//
void Process_C(void) 
 {
    fOut_1=fInp_1;	
	
	PutDAC(DAC_1R, fOut_1);
	PutDAC(DAC_1L, fOut_1);
 }
//--------------------------------------------------------------------------//


//--------------------------------------------------------------------------//
// Function:    PROCESS_D for Signal-Processing                             //
// ---------------------------------------                                  //
// Description:                                                             //
//                                                                          //
//                                                                          //  
//--------------------------------------------------------------------------//
void Process_D(void) 
 {

 }
//--------------------------------------------------------------------------//


//--------------------------------------------------------------------------//
// Function:    Process_Tim0 - Signal-Processing                             //
// ---------------------------------------                                  //
// Description:                                                             //
//                                                                          //
//                                                                          //  
//--------------------------------------------------------------------------//
void Process_Tim0(void) 
 { 

 }
//--------------------------------------------------------------------------//



//-------------------------------------------------------------------------//
void PutDAC(short Port, fract16 fData)  // Data to DAC
 {//-----------------------------------------------------------------------//
   int          iData;
   unsigned int LeftWord,RightWord;
   //-----------------------------------------------------------------
   iData = (int)fData;              // 16 bit signed --> 32 bit signed    
   iData = iData << 8;              // expand 16 bit to 24 bit
   //-----------------------------------------------------------------
   if(ChipVer < 0x02) 
    { //--------------------------------------------------------------
      // DMA-Input anomaly, Chip-Realize 0.1 for EZ-KIT LITE  Rev. 1.1 
      //--------------------------------------------------------------
      LeftWord  = (unsigned int)iData >> 16;
      RightWord = (unsigned int)iData << 16;
      iData = LeftWord | RightWord;
      //------------ 
    } // end if anomaly
   //-----------------------------------------------------------------
   iTxBuffer[Port]=iData;           //  Write DAC
 } // end function PutDAC
//--------------------------------------------------------------------------//
