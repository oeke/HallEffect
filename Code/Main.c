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

fract16 fKof[11]={0.99999r16,0.693r16,0.447r16,0.268r16,0.172r16,0.138r16,0.111r16,0.093r16,0.077r16,0.066r16,0.053r16};//Koeffizienten Array 
 
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

