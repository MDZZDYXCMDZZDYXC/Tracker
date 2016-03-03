// TI File $Revision: /main/2 $
// Checkin $Date: December 2, 2004   11:50:58 $
//###########################################################################
//
// FILE:    DSP280x_Gpio.c
//
// TITLE:   DSP280x General Purpose I/O Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP280x Header Files V1.60 $
// $Release Date: December 3, 2007 $
//###########################################################################

#include "DSP280x_Device.h"     // DSP280x Headerfile Include File
#include "DC_Servo.h"
//---------------------------------------------------------------------------
// InitGpio: 
//---------------------------------------------------------------------------
// This function initializes the Gpio to a known (default) state.
//
// For more details on configuring GPIO's as peripheral functions,
// refer to the individual peripheral examples and/or GPIO setup example. 
void InitGpio(void)
{
    EALLOW;
   
    // Each GPIO pin can be: 
    // a) a GPIO input/output
    // b) peripheral function 1
    // c) peripheral function 2
    // d) peripheral function 3
    // By default, all are GPIO Inputs 
    GpioCtrlRegs.GPAMUX1.all = 0x0000;     // GPIO functionality GPIO0-GPIO15
    GpioCtrlRegs.GPAMUX2.all = 0x0000;     // GPIO functionality GPIO16-GPIO31
    GpioCtrlRegs.GPBMUX1.all = 0x0000;     // GPIO functionality GPIO32-GPIO34

    GpioCtrlRegs.GPADIR.all = 0x0000;      // GPIO0-GPIO31 are GP inputs
    GpioCtrlRegs.GPBDIR.all = 0x0000;      // GPIO32-GPIO34 are inputs   
   	
    // Each input can have different qualification
    // a) input synchronized to SYSCLKOUT
    // b) input qualified by a sampling window
    // c) input sent asynchronously (valid for peripheral inputs only)
    GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15 Synch to SYSCLKOUT 
    GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
    GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO34 Synch to SYSCLKOUT 

    // Pull-ups can be enabled or disabled. 
    GpioCtrlRegs.GPAPUD.all = 0x0000;      // Pullup's enabled GPIO0-GPIO31
    GpioCtrlRegs.GPBPUD.all = 0x0000;      // Pullup's enabled GPIO32-GPIO34
    //GpioCtrlRegs.GPAPUD.all = 0xFFFF;    // Pullup's disabled GPIO0-GPIO31
    //GpioCtrlRegs.GPBPUD.all = 0xFFFF;    // Pullup's disabled GPIO32-GPIO34
    
    // Added by txs@20131228
	

    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0; // Set GPIO16 -> Normal I/O Func   //ָʾ���пص�LED���ź�
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;  // Set GPIO16 -> Input Interface
	GpioDataRegs.GPADAT.bit.GPIO16 = 1;
	
	GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;
	GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0; // Set GPIO16 -> Normal I/O Func      ���룺�����пذ��PD5�ź�
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 0;  // Set GPIO16 -> Input Interface
	
	GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0; 
	GpioCtrlRegs.GPADIR.bit.GPIO6 = 0; /////////////��������λ���ź�
	GpioCtrlRegs.GPACTRL.bit.QUALPRD0 = 0x01;
	GpioCtrlRegs.GPAQSEL1.bit.GPIO6 = 2;
    // End of add

#if 1
    // Added by selfree@20140214
    // for motor manual control
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0; // Set GPIO16 -> Normal I/O Func
    GpioCtrlRegs.GPADIR.bit.GPIO18 = 0;  // Set GPIO16 -> Input Interface

    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0; // Set GPIO16 -> Normal I/O Func
    GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;  // Set GPIO16 -> Input Interface

    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0; // Set GPIO16 -> Normal I/O Func
    GpioCtrlRegs.GPADIR.bit.GPIO29 = 0;  // Set GPIO16 -> Input Interface

    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0; // Set GPIO16 -> Normal I/O Func
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 0;  // Set GPIO16 -> Input Interface

    // End of add
#endif

    EDIS;

}   
//unsigned char io6=0;
// return value -> user turn on or off Motor
unsigned char GetSwitchValue(){
    return GpioDataRegs.GPADAT.bit.GPIO6;  ////////////////////////��������λ���ź�

}   


unsigned char GetEastCtrlValue(){

    return GpioDataRegs.GPADAT.bit.GPIO18;
}   

unsigned char GetWestCtrlValue(){
    return 1;
    return GpioDataRegs.GPADAT.bit.GPIO28;
}   

unsigned char GetNorthCtrlValue(){

    return GpioDataRegs.GPADAT.bit.GPIO29;
}   

unsigned char GetSouthCtrlValue(){


    return GpioDataRegs.GPBDAT.bit.GPIO34;
}   


//===========================================================================
// End of file.
//===========================================================================
