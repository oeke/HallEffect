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
extern fract16          fKof[29];
//-------------------------------
extern int iTxBuffer[];         // SPORT0 DMA transmit buffer
extern int iRxBuffer[];         // SPORT0 DMA receive buffer
//-------------------------------
fract32		fBuf[4097];	//Buffer Array, letzter index ist trash
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
    
   
   
   // ------------ folgender Teil dauert anscheinen zu Lange !!!!!!! -------
   /*int i=0;
	while(i<ic){
		fBuf[i+1]=fBuf[i];	//Buffer um 1 nach rechts schieben
		i++;	
	}*/	
	
	if(ic<4095){			//c max 4095
		ic++;				//Buffer load erhöhen, falls nicht schon voll
	}
	if(ip>=4095)
	    ip-=4095;
	
	fTmp_16=0.6r16;
	fBuf[ip]=fTmp_16*fIn;
	fBuf[ip]<<1;
	fTmp_16=0.3r16;
	fTmp2_16=fAcc>>16;
	fTmp_32=fTmp_16*fTmp2_16;
	fBuf[ip]+=fTmp_32<<1;
	int k=0;
	if(ic>4094){
		fAcc=0;
		
		fTmp_16=0.8r16;
		k=ip+1000;
		if(k>=4095)
		    k-=4095;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32<<1;
		fAcc+=fTmp2_32;
		
		fTmp_16=0.5r16;
		k=ip+3000;
		if(k>=4095)
		    k-=4095;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32<<1;
		fAcc+=fTmp2_32;
		
		fTmp_16=0.25r16;
		k=ip+4000;
		if(k>=4095)
		    k-=4095;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32<<1;
		fAcc+=fTmp2_32;
		
		/*fTmp_16=0.27r16;
		k=ip+3800;
		if(k>=4095)
		    k-=4095;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32<<1;
		fAcc+=fTmp2_32;*/
	}
	
	/*fTemp=-0.6r16;	
    fAcc=mult_fr1x16(fAcc,fTemp);
	*/
	
	fTmp_16=0.3r16;
	fTmp2_16=fAcc>>16;
	fTmp_32=fTmp_16*fTmp2_16;
	fOut_1=fTmp_32>>15;
	fOut_2=fOut_1;
   //fOut_1   =   fBuf[ip]>>16;              // Input Buffer
   //fOut_2   =   fBuf[ip]>>16; 
   ip++;             // Input Buffer
    /*
    fTemp=-0.5r16;
    fOut_1=mult_fr1x16(fTemp2,fTemp);
    fOut_2=-fTemp2;*/
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
	
	//tmp=fr16_to_float(fInp_1);
	fTIn=fInp_1;
	fTemp=-0.5r16;
	fBuf[0]=mult_fr1x16(fTemp,fTIn); //*float_to_fr16(flInp_1Scale);			//neuen Wert aus ADC laden * skalierungsfaktor
 	fTemp=-0.25r16;
	//fBuf[0]=+mult_fr1x16(fAcc,fTemp);			// Rückkoplung vom Ausgang, skaliert
 	if(ic<4095){			//c max 4095
		ic++;				//Buffer load erhöhen, falls nicht schon voll
	}	
	
	// Koeffizienten berechnen
	if(ic>4094){
		fAcc=0;
		fAcc+=fBuf[1000];
		fTemp=-0.5r16;
		fAcc+=mult_fr1x16(fTemp,fBuf[2000]);
		fTemp=-0.25r16;
		fAcc+=mult_fr1x16(fTemp,fBuf[3000]);
		fTemp=-0.125r16;
		fAcc+=mult_fr1x16(fTemp,fBuf[4000]);
	/*	fAcc+=mult_fr1x16(fBuf[0],fKof[0]);
		fAcc+=mult_fr1x16(fBuf[500],fKof[1]);
		fAcc+=mult_fr1x16(fBuf[1000],fKof[2]);
		fAcc+=mult_fr1x16(fBuf[1500],fKof[3]);
		fAcc+=mult_fr1x16(fBuf[2000],fKof[4]);
		fAcc+=mult_fr1x16(fBuf[2500],fKof[5]);
		fAcc+=mult_fr1x16(fBuf[3000],fKof[6]);
		fAcc+=mult_fr1x16(fBuf[3050],fKof[7]);
		fAcc+=mult_fr1x16(fBuf[3100],fKof[8]);
		fAcc+=mult_fr1x16(fBuf[3150],fKof[9]);
        fAcc+=mult_fr1x16(fBuf[3200],fKof[10]);
		fAcc+=mult_fr1x16(fBuf[3250],fKof[11]);
		fAcc+=mult_fr1x16(fBuf[3300],fKof[12]);
		fAcc+=mult_fr1x16(fBuf[3350],fKof[13]);
		fAcc+=mult_fr1x16(fBuf[3400],fKof[14]);
		fAcc+=mult_fr1x16(fBuf[3450],fKof[15]);
		fAcc+=mult_fr1x16(fBuf[3500],fKof[16]);
		fAcc+=mult_fr1x16(fBuf[3550],fKof[17]);
		fAcc+=mult_fr1x16(fBuf[3600],fKof[18]);
		fAcc+=mult_fr1x16(fBuf[3650],fKof[19]);
		fAcc+=mult_fr1x16(fBuf[3700],fKof[20]);
		fAcc+=mult_fr1x16(fBuf[3750],fKof[21]);
		fAcc+=mult_fr1x16(fBuf[3800],fKof[22]);
		fAcc+=mult_fr1x16(fBuf[3850],fKof[23]);
		fAcc+=mult_fr1x16(fBuf[3900],fKof[24]);
		fAcc+=mult_fr1x16(fBuf[3950],fKof[25]);
		fAcc+=mult_fr1x16(fBuf[4000],fKof[26]);
		fAcc+=mult_fr1x16(fBuf[4050],fKof[27]);
		fAcc+=mult_fr1x16(fBuf[4095],fKof[28]);*/
		
				/*fAcc+=fBuf[500]*float_to_fr16(exp(-500))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[1000]*float_to_fr16(exp(-1000))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[1500]*float_to_fr16(exp(-1500))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[2000]*float_to_fr16(exp(-2000))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[2500]*float_to_fr16(exp(-2500))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3000]*float_to_fr16(exp(-3000))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3050]*float_to_fr16(exp(-3050))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3100]*float_to_fr16(exp(-3100))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3150]*float_to_fr16(exp(-3150))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3200]*float_to_fr16(exp(-3200))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3250]*float_to_fr16(exp(-3250))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3300]*float_to_fr16(exp(-3300))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3350]*float_to_fr16(exp(-3350))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3400]*float_to_fr16(exp(-3400))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3450]*float_to_fr16(exp(-3450))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3500]*float_to_fr16(exp(-3500))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3550]*float_to_fr16(exp(-3550))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3600]*float_to_fr16(exp(-3600))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3650]*float_to_fr16(exp(-3650))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3700]*float_to_fr16(exp(-3700))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3750]*float_to_fr16(exp(-3750))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3800]*float_to_fr16(exp(-3800))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3850]*float_to_fr16(exp(-3850))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3900]*float_to_fr16(exp(-3900))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[3950]*float_to_fr16(exp(-3950))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[4000]*float_to_fr16(exp(-4000))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[4050]*float_to_fr16(exp(-4050))*float_to_fr16(flKoeffAnz);
		fAcc+=fBuf[4095]*float_to_fr16(exp(-4095))*float_to_fr16(flKoeffAnz);*/
	}
//	if(fInp_1 > 0x8000)
//	 fInp_1=fInp_1+1;
//	fTemp=0x4000;
    fTemp=-0.6r16;	
    //fAcc=mult_fr1x16(fAcc,fTemp);// Demo: rauschen bei multiplikation mit faktor
	fBuf[0]=mult_fr1x16(fTemp,fTIn);
	fOut_1=fBuf[0];
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
    fIn    =   fInp_1;
    
   
   
   // ------------ folgender Teil dauert anscheinen zu Lange !!!!!!! -------
   /*int i=0;
	while(i<ic){
		fBuf[i+1]=fBuf[i];	//Buffer um 1 nach rechts schieben
		i++;	
	}*/	
	
	if(ic<4095){			//c max 4095
		ic++;				//Buffer load erhöhen, falls nicht schon voll
	}
	if(ip>=4095)
	    ip-=4095;
	
	fTmp_16=-0.5r16;
	fBuf[ip]=fTmp_16*fIn;
	fBuf[ip]<<1;
	fTmp_16=0.125r16;
	fTmp2_16=fAcc>>16;
	fTmp_32=fTmp_16*fTmp2_16;
	//fBuf[ip]+=fTmp_32<<1;
	int k=0;
	if(ic>4094){
		fAcc=0;
		
		fTmp_16=0.35r16;
		k=ip+3000;
		if(k>=4095)
		    k-=4095;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32<<1;
		fAcc+=fTmp2_32;
		
		fTmp_16=0.3r16;
		k=ip+3500;
		if(k>=4095)
		    k-=4095;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32<<1;
		fAcc+=fTmp2_32;
		
		fTmp_16=0.25r16;
		k=ip+4000;
		if(k>=4095)
		    k-=4095;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32<<1;
		fAcc+=fTmp2_32;
		
		fTmp_16=0.27r16;
		k=ip+3800;
		if(k>=4095)
		    k-=4095;
		fTmp2_16=fBuf[k]>>16;
		fTmp2_32=fTmp2_16*fTmp_16;
		fTmp2_32<<1;
		fAcc+=fTmp2_32;
	}
	
	/*fTemp=-0.6r16;	
    fAcc=mult_fr1x16(fAcc,fTemp);
	*/
   fOut_1   =   fBuf[ip]>>16;              // Input Buffer
   fOut_2   =   fBuf[ip]>>16; 
   ip++;             // Input Buffer
    /*
    fTemp=-0.5r16;
    fOut_1=mult_fr1x16(fTemp2,fTemp);
    fOut_2=-fTemp2;*/
   PutDAC(DAC_1R, fOut_1);  // Write DAC 1R
   PutDAC(DAC_1L, fOut_2);  // Write DAC 1L
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
