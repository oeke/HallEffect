//--------------------------------------------------------------------
//  Name:   Signal processing with ADSP-BF533 EZ-KIT Lite           //
//--------------------------------------------------------------------
//  Project Name:   DSP-Framework                                   //
//-------------------------------                                   //
//  Date Modified:  Juni 2012                                       //
//--------------------------------------------------------------------
//  Software:       VisualDSP++5.0 - Update 10                      //
//  Hardware:       ADSP-BF533 EZ-KIT Board Rev 1.1 or 1.5          //
//--------------------------------------------------------------------
//  Purpose:        This program sets up the SPI port on            //
//                  the ADSP-BF533 to configure the                 //
//                  AD1836 codec.  The SPI port is disabled         //
//                  after initialization.                           //  
//                  The data to/from the codec are                  //
//                  transfered over SPORT0 in I2S mode              //
//--------------------------------------------------------------------
#include "Main.h"  
#include "math.h"  

fract16		fKof[29];	//Koeffizienten Array      
//--------------------------------------------------------------------
// Function:    main                                                //
// -------------------------------------------------------------------
// Description: After calling a few initalization routines,         //
//              main just waits in a loop while forever,            //
//              for Interrupt-Events to realize Processing          //
// -------------------------------------------------------------------
void main(void)
 { //-----------------------------------------------------------------
   Init_EBIU();        // Init 16-bit External Memory
                       // access via external bus interface
   Init_Flash_Ports(); // 
   //------------------
   Init_1836();         // Init AD1836-codec  
   Init_Sport0();       // Init Serial Port0:
                        // Clock & Frame
                        
   fKof[0]=1;
   fKof[1]=float_to_fr16(exp(-0.5)/29);
   fKof[2]=float_to_fr16(exp(-1)/29);
   fKof[3]=float_to_fr16(exp(-1.5)/29);
   fKof[4]=float_to_fr16(exp(-2.0)/29);
   fKof[5]=float_to_fr16(exp(-2.5)/29);
   fKof[6]=float_to_fr16(exp(-3.5)/29);
   fKof[7]=float_to_fr16(exp(-3.05)/29);
   fKof[8]=float_to_fr16(exp(-3.1)/29);
   fKof[9]=float_to_fr16(exp(-3.15)/29);
   fKof[10]=float_to_fr16(exp(-3.2)/29);
   fKof[11]=float_to_fr16(exp(-3.25)/29);
   fKof[12]=float_to_fr16(exp(-3.3)/29);
   fKof[13]=float_to_fr16(exp(-3.35)/29);
   fKof[14]=float_to_fr16(exp(-3.4)/29);
   fKof[15]=float_to_fr16(exp(-3.45)/29);
   fKof[16]=float_to_fr16(exp(-3.5)/29);
   fKof[17]=float_to_fr16(exp(-3.55)/29);
   fKof[18]=float_to_fr16(exp(-3.6)/29);
   fKof[19]=float_to_fr16(exp(-3.65)/29);
   fKof[20]=float_to_fr16(exp(-3.7)/29);
   fKof[21]=float_to_fr16(exp(-3.75)/29);
   fKof[22]=float_to_fr16(exp(-3.8)/29);
   fKof[23]=float_to_fr16(exp(-3.85)/29);
   fKof[24]=float_to_fr16(exp(-3.9)/29);
   fKof[25]=float_to_fr16(exp(-3.95)/29);
   fKof[26]=float_to_fr16(exp(-4.0)/29);
   fKof[27]=float_to_fr16(exp(-4.05)/29);
   fKof[28]=float_to_fr16(exp(-4.095)/29);
                        
   Init_DMA();          // Direct Memory Access
   Init_Interrupts();   // Init Interrupts
   Enable_DMA_Sport0(); 
   //------------------
   Init_Push_Button();
   Init_Timer0();       // Peripheral Clock
   
   
      //-----------------------------------------------------------------
   while(1)
   { //--------------------------------------------------------
     if(NewAdcData != 0)                // if new data from ADC
      { //-----------------------------------------------------
        NewAdcData = 0;                 // set flag back   
        //-------------------------------------------------
        //  Choose Type of Signal processing                 
        //-------------------------------------------------
        switch(PrMode) 
         {
           case 0: Process_A();break;   // Signal-Process-A
           case 1: Process_B();break;   // Signal-Process-B
           case 2: Process_C();break;   // Signal-Process-C
           case 3: Process_D();break;   // Signal-Process-D
          }
        //-------------------------------------------------
      } // end if NewAdcData
     //----------------------------------------------------
     if(NewTimerEvent != 0)             // if new TimerEvent
      { //-------------------------------------------------
        NewTimerEvent = 0;              // set flag back    
        //-------------------------------------------------
        Process_Tim0();
        //-------------------------------------------------
      }  // end if NewTimerEvent
        //-------------------------------------------------
   }  // end endless while-loop
   //-----------------------------------------------------------------
 } // end main 
//--------------------------------------------------------------------

