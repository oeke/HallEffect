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
extern fract16        fKof[11];  // Koeffizienten Array, wird in der Main initialisiert

//-------------------------------
extern int iTxBuffer[];         // SPORT0 DMA transmit buffer
extern int iRxBuffer[];         // SPORT0 DMA receive buffer
//-------------------------------
fract32		fBuf[4096];	//Buffer Array
int 		ic=0;		//Buffer-Load Variable
fract32		fAcc;		//Hall Akkumulator f√ºr die R√ºckkopplung
int         ip=0;       //Pointer auf Ringbuffer, aktueller Wert

fract16     fIn;        //Variable zum Speichern des neuen Werts
fract32     fTmp_32;
fract32     fTmp2_32;
fract16     fTmp_16;
fract16     fTmp2_16;

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
void Process_A(void)                
 { //------------------------
   
 	fIn    =   fInp_1;          // neuen Wert speichern
    	
	
	ip=ip&0x00000fff;			// Kreisindizierung mit 4096 indizes
	fBuf[ip]=fIn<<15;			// 16 bit in 32 bit fract also 16 mal nach links schieben
	                            // und multiplizieren mit Faktor 0.5, also 1 mal nach rechts schieben
	
	fBuf[ip]+=fAcc>>2;			// R¸ckkoplung mit faktor 0.25
	int k=0;					// Koeffizienten-Index
	
	if(ic>4095){                // wenn Buffer gef¸llt ist (buffer load > 4095
		fAcc=0;
		
		fTmp_16=fKof[0];            // Koeffizient laden
		k=ip;                       // Bufferindex relativ zum letzten Wert (hier 0)
		k=k&0x00000fff;             // Kreisindizierung des buffers, max 4095
		fTmp2_16=fBuf[k]>>16;       // fract32 Buffer-Wert in fract16 Variable
		fTmp2_32=fTmp2_16*fTmp_16;  // 16 bit Multiplikation mit 32 bit Ergebniss
		
		fTmp2_32>>1;                /* Ergebnis m¸sste 1 mal links geschoben werden
		                             * damit das fract32 format stimmt, aber multiplikation
		                             * mit Faktor 0.25 (2mal rechts schieben) ergibt
		                             * 1 mal rechts schieben
		                             */
		                             
		fAcc+=fTmp2_32;             // Ergebnis akkumulieren
		
		
		
		fTmp_16=fKof[1];
		k=ip+500;               // Bufferindex relativ zum letzten Wert (hier 500)
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
	}
   
	if(ic<4096){			// ic max 4095
        ic++;			    // Buffer load erh√∂hen, falls nicht schon voll
    }
    ip++;                   // pointer auf ringbuffer erhˆhen (kreisindizierung)
	
    fOut_1=fAcc>>16;	    // Akkumulator Wert auf beide Ausg√§nge legen
                            // 16 mal rechtsschieben, da fract32 zu fract16 konvertierung
	fOut_2=fOut_1;
   
    PutDAC(DAC_1R, fOut_1); // Write DAC 1R
    PutDAC(DAC_1L, fOut_2); // Write DAC 1L
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
