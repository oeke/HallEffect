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
//-------------------------------
extern int iTxBuffer[];         // SPORT0 DMA transmit buffer
extern int iRxBuffer[];         // SPORT0 DMA receive buffer
//-------------------------------
extern fract16		fBuf[4097];	//Buffer Array, letzter index ist trash
extern fract16		fKof[4096];	//Koeffizienten Array
extern int 		ic=0;		//Buffer-Load Variable
extern float 		flAcc;		//Hall Akkumulator für die Rückkopplung
extern int 		iKoAnz;		// Anzahl an verwendeten Koeffizienten

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
   fOut_1   =  fInp_1;              // Input Buffer
   fOut_2   =  -fInp_1;              // Input Buffer
   //------------------------
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
	int i=0;
	while(i<ic){
		fBuf[i+1]=fBuf[i];	//Buffer um 1 nach rechts schieben
		i++;	
	}
	
	fBuf[0]=fInp_1;			//neuen Wert aus ADC laden
	if(ic<4095){			//c max 4095
		ic++;			//Buffer load erhöhen, falls nicht schon voll
	}	
	
	// Koeffizienten berechnen
	if(c>4094){
		flAcc=0;
		flAcc+=fr16_to_float(fBuf[0]);
		flAcc+=fr16_to_float(fBuf[500])*exp(500);
		flAcc+=fr16_to_float(fBuf[1000])*exp(1000);
		flAcc+=fr16_to_float(fBuf[1500])*exp(1500);
		flAcc+=fr16_to_float(fBuf[2000])*exp(2000);
		flAcc+=fr16_to_float(fBuf[2500])*exp(2500);
		flAcc+=fr16_to_float(fBuf[3000])*exp(3000);
		flAcc+=fr16_to_float(fBuf[3050])*exp(3050);
		flAcc+=fr16_to_float(fBuf[3100])*exp(3100);
		flAcc+=fr16_to_float(fBuf[3150])*exp(3150);
		flAcc+=fr16_to_float(fBuf[3200])*exp(3200);
		flAcc+=fr16_to_float(fBuf[3250])*exp(3250);
		flAcc+=fr16_to_float(fBuf[3300])*exp(3300);
		flAcc+=fr16_to_float(fBuf[3350])*exp(3350);
		flAcc+=fr16_to_float(fBuf[3400])*exp(3400);
		flAcc+=fr16_to_float(fBuf[3450])*exp(3450);
		flAcc+=fr16_to_float(fBuf[3500])*exp(3500);
		flAcc+=fr16_to_float(fBuf[3550])*exp(3550);
		flAcc+=fr16_to_float(fBuf[3600])*exp(3600);
		flAcc+=fr16_to_float(fBuf[3650])*exp(3650);
		flAcc+=fr16_to_float(fBuf[3700])*exp(3700);
		flAcc+=fr16_to_float(fBuf[3750])*exp(3750);
		flAcc+=fr16_to_float(fBuf[3800])*exp(3800);
		flAcc+=fr16_to_float(fBuf[3850])*exp(3850);
		flAcc+=fr16_to_float(fBuf[3900])*exp(3900);
		flAcc+=fr16_to_float(fBuf[3950])*exp(3950);
		flAcc+=fr16_to_float(fBuf[4000])*exp(4000);
		flAcc+=fr16_to_float(fBuf[4050])*exp(4050);
		flAcc+=fr16_to_float(fBuf[4095])*exp(4095);
	}
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
