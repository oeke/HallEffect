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
extern fract16		fAcc;		//Hall Akkumulator für die Rückkopplung
extern int 		iKoAnz;		// Anzahl an verwendeten Koeffizienten


//-------------------------------	//Skalierungsfaktoren
extern fract16		fInp_1Scale = 0.5;	//Skalierungsfaktor Eingang (fInp_1Scale+fAccScale =1)
extern fract16		fAccScale = 0.5;	//Skalierter Ausgang

extern fract16		fKoeffAnz = 1/iKoAnz;	//bei iKoAnz (29) Koeffizienten	

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
	
	fBuf[0]=fInp_1*fInp_1Scale;			//neuen Wert aus ADC laden * skalierungsfaktor
 	fBuf[0]+=fAcc*fAccScale;			// Rückkoplung vom Ausgang, skaliert
 	if(ic<4095){			//c max 4095
		ic++;				//Buffer load erhöhen, falls nicht schon voll
	}	
	
	// Koeffizienten berechnen
	if(c>4094){
		fAcc=0;
		fAcc+=fBuf[0]*fKoeffAnz;
		fAcc+=fBuf[500]*float_to_frac16(exp(-500))*fKoeffAnz;
		fAcc+=fBuf[1000]*float_to_frac16(exp(-1000))*fKoeffAnz;
		fAcc+=fBuf[1500]*float_to_frac16(exp(-1500))*fKoeffAnz;
		fAcc+=fBuf[2000]*float_to_frac16(exp(-2000))*fKoeffAnz;
		fAcc+=fBuf[2500]*float_to_frac16(exp(-2500))*fKoeffAnz;
		fAcc+=fBuf[3000]*float_to_frac16(exp(-3000))*fKoeffAnz;
		fAcc+=fBuf[3050]*float_to_frac16(exp(-3050))*fKoeffAnz;
		fAcc+=fBuf[3100]*float_to_frac16(exp(-3100))*fKoeffAnz;
		fAcc+=fBuf[3150]*float_to_frac16(exp(-3150))*fKoeffAnz;
		fAcc+=fBuf[3200]*float_to_frac16(exp(-3200))*fKoeffAnz;
		fAcc+=fBuf[3250]*float_to_frac16(exp(-3250))*fKoeffAnz;
		fAcc+=fBuf[3300]*float_to_frac16(exp(-3300))*fKoeffAnz;
		fAcc+=fBuf[3350]*float_to_frac16(exp(-3350))*fKoeffAnz;
		fAcc+=fBuf[3400]*float_to_frac16(exp(-3400))*fKoeffAnz;
		fAcc+=fBuf[3450]*float_to_frac16(exp(-3450))*fKoeffAnz;
		fAcc+=fBuf[3500]*float_to_frac16(exp(-3500))*fKoeffAnz;
		fAcc+=fBuf[3550]*float_to_frac16(exp(-3550))*fKoeffAnz;
		fAcc+=fBuf[3600]*float_to_frac16(exp(-3600))*fKoeffAnz;
		fAcc+=fBuf[3650]*float_to_frac16(exp(-3650))*fKoeffAnz;
		fAcc+=fBuf[3700]*float_to_frac16(exp(-3700))*fKoeffAnz;
		fAcc+=fBuf[3750]*float_to_frac16(exp(-3750))*fKoeffAnz;
		fAcc+=fBuf[3800]*float_to_frac16(exp(-3800))*fKoeffAnz;
		fAcc+=fBuf[3850]*float_to_frac16(exp(-3850))*fKoeffAnz;
		fAcc+=fBuf[3900]*float_to_frac16(exp(-3900))*fKoeffAnz;
		fAcc+=fBuf[3950]*float_to_frac16(exp(-3950))*fKoeffAnz;
		fAcc+=fBuf[4000]*float_to_frac16(exp(-4000))*fKoeffAnz;
		fAcc+=fBuf[4050]*float_to_frac16(exp(-4050))*fKoeffAnz;
		fAcc+=fBuf[4095]*float_to_frac16(exp(-4095))*fKoeffAnz;
	}
	fOut_1=fAcc;
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
