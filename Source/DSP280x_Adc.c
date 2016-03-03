// TI File $Revision: /main/1 $
// Checkin $Date: December 1, 2004   11:11:30 $
//###########################################################################
//
// FILE:    DSP280x_Adc.c
//
// TITLE:   DSP280x ADC Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP280x Header Files V1.60 $
// $Release Date: December 3, 2007 $
//###########################################################################

#include "DSP280x_Device.h"     // DSP280x Headerfile Include File
#include "DC_Servo.h"
#define ADC_usDELAY  5000L

//---------------------------------------------------------------------------
// InitAdc: 
//---------------------------------------------------------------------------
// This function initializes ADC to a known state.
//
// PLEASE NOTE, ADC INIT IS DIFFERENT ON 281x vs 280x DEVICES!!!
//

#define ADC_usDELAY  5000L
void InitAdc(void)
{
/*    extern void DSP28x_usDelay(Uint32 Count);
    AdcRegs.ADCTRL1.bit.RESET = 1;          //�����λ
    asm(" RPT #14 || NOP");                 //��Ҫ����ʱ
    AdcRegs.ADCREFSEL.bit.REF_SEL = 0x01;   //�ⲿ2.048V�ο���ѹ

    // Please note that for the delay function below to operate correctly the 
    // CPU_CLOCK_SPEED define statement in the DSP280x_Examples.h file must 
    // contain the correct CPU clock period in nanoseconds. 

    AdcRegs.ADCTRL3.all = 0x00E0;           //Power up the reference, bandgap, and analog circuits together
    DELAY_US(ADC_usDELAY);                  // Delay before converting ADC channels

    AdcRegs.ADCTRL1.bit.CPS = 1;
    AdcRegs.ADCTRL3.bit.ADCCLKPS = 2;       //AD�ں�ʱ��ѡΪADCLK=6.25MHz
    AdcRegs.ADCTRL1.bit.ACQ_PS = 1;         //��������Ϊ2��ADCLK����
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 0;       //˫����ģʽ

    AdcRegs.ADCMAXCONV.bit.MAX_CONV2 = 0;
    AdcRegs.ADCCHSELSEQ4.bit.CONV15 = 0xF;
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 0xF;
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0;       //����/ֹͣģʽ
    AdcRegs.ADCOFFTRIM.bit.OFFSET_TRIM = 0;
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clears the SEQ1 interrupt flag bit
    AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;     //clears the SEQ2 interrupt flag bit
    //AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
//  AdcRegs.ADCREFSEL.bit.REF_SEL = 0;

    AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1;
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 1;   //Interrupt request by INT_SEQ1 is enabled
    AdcRegs.ADCTRL2.bit.INT_MOD_SEQ2 = 0;   //INT_SEQ1 is set at the end of every SEQ1 sequence.*/

//������ADCʱ��Ƶ��Ϊ500k��HSPCLK=1��sysOUTCLK=30M��CPS=0����Ƶ��ADCCLKPS=15��
//����Ƶ��Ϊ250k����ΪACQ_PS=1

    extern void DSP28x_usDelay(Uint32 Count);
    
    AdcRegs.ADCTRL1.bit.RESET=1;
    asm(" RPT #14 || NOP");                 //��Ҫ����ʱ
    AdcRegs.ADCREFSEL.bit.REF_SEL = 0x00;   //�ⲿ2.048V�ο���ѹ
    AdcRegs.ADCTRL1.bit.RESET=0;
    AdcRegs.ADCTRL1.bit.SUSMOD=3;           //�������ʱ���������������߼�����ֹͣ
    AdcRegs.ADCTRL1.bit.ACQ_PS=1;           //�ɼ����ڵĴ�С
    AdcRegs.ADCTRL1.bit.CPS=0;              //�ں�ʱ��Ԥ���������ʱ��HSPCLK��ƵֵΪ0
    AdcRegs.ADCTRL1.bit.CONT_RUN=0;         //��ʼ--ֹͣģʽ
    AdcRegs.ADCTRL1.bit.SEQ_CASC=1;         //����ģʽ
    
    AdcRegs.ADCTRL3.bit.ADCBGRFDN=3;        //��϶�Ͳο���·�ϵ�
    DELAY_US(ADC_usDELAY);
    ServiceDog();                  // Delay before powering up rest of ADC
    AdcRegs.ADCTRL3.bit.ADCPWDN=1;          //����϶�Ͳο���·���ADC����ģ���·�ϵ�
    DELAY_US(ADC_usDELAY);                  // Delay after powering up ADC
	ServiceDog();
    AdcRegs.ADCTRL3.bit.ADCCLKPS=15;
    AdcRegs.ADCTRL3.bit.SMODE_SEL=0;        //ѡ��ͬ������ģʽ
    //configure ADC
	AdcRegs.ADCMAXCONV.all = 9;

	// AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 8;
    
    AdcRegs.ADCCHSELSEQ1.bit.CONV00=0;
    AdcRegs.ADCCHSELSEQ1.bit.CONV01=1;
    AdcRegs.ADCCHSELSEQ1.bit.CONV02=2;
    AdcRegs.ADCCHSELSEQ1.bit.CONV03=3;
        
    AdcRegs.ADCCHSELSEQ2.bit.CONV04=4;
    AdcRegs.ADCCHSELSEQ2.bit.CONV05=5;
    AdcRegs.ADCCHSELSEQ2.bit.CONV06=6;
    AdcRegs.ADCCHSELSEQ2.bit.CONV07=7;

    AdcRegs.ADCCHSELSEQ3.bit.CONV08=8;
    AdcRegs.ADCCHSELSEQ3.bit.CONV09=9;
/*    AdcRegs.ADCCHSELSEQ3.bit.CONV10=10;
    AdcRegs.ADCCHSELSEQ3.bit.CONV11=11;
    
    AdcRegs.ADCCHSELSEQ4.bit.CONV12=12;
    AdcRegs.ADCCHSELSEQ4.bit.CONV13=13;
    AdcRegs.ADCCHSELSEQ4.bit.CONV14=14;
    AdcRegs.ADCCHSELSEQ4.bit.CONV15=15;
*/
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //���SEQ1���жϱ�־λINT_SEQ1
    AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;     //���SEQ2���жϱ�־λINT_SEQ2
        
    AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ=0;    //��������
    AdcRegs.ADCTRL2.bit.RST_SEQ1=0;         //��λ������1��������
    AdcRegs.ADCTRL2.bit.SOC_SEQ1=0;
        
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1=1;     //ʹ��INT SEQ1�������ж�����
    AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1=0;     //ÿ��SEQ1���н���ʱ��INT SEQ1��λ
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1=0;   //EVA�Ĵ����źŲ�������SEQ1
    AdcRegs.ADCTRL2.bit.EXT_SOC_SEQ1=0;     //�ⲿADCDOC�����źŲ�����ADC�Զ�ת��
    AdcRegs.ADCTRL2.bit.RST_SEQ2=0;
    AdcRegs.ADCTRL2.bit.SOC_SEQ2=0;
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2=0;
    AdcRegs.ADCTRL2.bit.INT_MOD_SEQ2=0;
    AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2=0;

}   

//===========================================================================
// End of file.
//===========================================================================

