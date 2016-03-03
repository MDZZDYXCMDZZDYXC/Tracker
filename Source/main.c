//###########################################################################
// FILE:	main.c
// version:   G://Yapha/OldOneMotorControlPro
// TITLE:	�����������ʼ��
//###########################################################################

#include "DSP280x_Device.h"     // DSP280x Headerfile Include File
#include "DC_Servo.h"
#include "pid_reg.h"

#define ADC_usDELAY  5000L
typedef unsigned char Uint8;

extern void InitialSemaphore();
extern void InitialData();
extern void EnableDog();

/////////////////////////PID������ر���//////////////////////////////////
PIDREG pidX_Current = PIDREG_DEFAULTS;
PIDREG pidY_Current = PIDREG_DEFAULTS;
//void RXdata(void);

#define CHECKNO 16
#define PAGENO 64
   
void Scib_init(void); 	 
void Scib_xmit(int a);
void delay(void);   
void I2C_init(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_readmode(void);
void I2C_writemode(void);
void I2C_writeOne(Uint16 data);
Uint8 I2C_readOne(void);
void I2C_checkack(void);
void I2C_returnnoack(void);
void I2C_returnack(void);
void I2C_write(Uint16 addr,Uint8 data);
void I2C_read(Uint16 addr,Uint8 *data);
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16*DestAddr);
void LEDPlay();


Uint16 Count;
Uint16 errbit;

extern int MPU9150Test();
extern void InitMPU9150();
//extern int  MPU9150_GetAcc(unsigned char type );
extern int GPS_Test();
extern  void maintest(void);
extern volatile int gGpsOK;
extern int32 GPSIndex;
int  gSystemInitialFlag = 0;
extern int32 gawPsdV[128][4];
Uint16 ShutDownflag=0;
main()
{
    int i=0;
    InitSysCtrl();

    DisableDog();//�ؿ��Ź�
    InitGpio();

    DINT;//��ֹȫ���ж�
    EALLOW;
    InitPieCtrl();//��ʼ��PIE������

    IER = 0x0000;//��ֹ���������CPU���ж�
    IFR = 0x0000;  
    EDIS;
    InitPieVectTable();//��ʼ���ж�������

#ifdef FLASH   //ѡ��Flash����ѡ��ʱ��Ҫע�͵�������"#endif"
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();
#endif
    EALLOW;
 
    InitCpuTimers();

    InitAdc();    
    InitEQep();
//gx -23  gy=24
    // replace 50 with 1000, change 20k to 1k, please check the frequency 
    ConfigCpuTimer(&CpuTimer0, 100, 50);//1k  ????????????
    StartCpuTimer0();
    
    GpioDataRegs.GPADAT.bit.GPIO5 = BIT_DISABLE;     // disable port
    asm(" RPT #14 || NOP");    
    GpioDataRegs.GPADAT.bit.GPIO2 = BIT_DISABLE; // disable port  

    PieVectTable.TINT0 = &TINT0_ISR;    //ӳ�䶨ʱ��0�ж�����
    PieVectTable.ADCINT = &ADCINT_ISR;
    PieVectTable.SCIRXINTA = &SCIRXINTA_ISR;
    EDIS;

    board_i2c_init();
    InitMPU9150();

    GPS_InitSci();
//	SCIB_Init_For485();    //485оƬ
    
    InitEPwm(); 
    EALLOW;
    
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;//ʹ�ܶ�ʱ��0�ж�
    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;//ADC
	//PieCtrlRegs.PIEIER9.bit.INTx1 = 1;
    IER |=( M_INT1 | M_INT9);//ʹ��CPU�ж�            
    EDIS;

	ERTM;//��ʵʱ�����ж�
    EINT;//��ȫ���ж�
    
//	DRTM;  
//	IntializeStepRecord();    //��ǰ����������
    DataInit();
//	gSystemInitialFlag = 0xFF;   
    if (gSystemInitialFlag == 0xFF)
    {
        ParaConfig();
    }

    if (gSystemInitialFlag == 0x55)
        IntializeStepRecord();
    ParaRead();
    
    GPSIndex= 0;

    if (gSystemInitialFlag == 0xaa)
    {
        RecordRead((Uint16*)gawPsdV[0], 128, 2048, 0, 8);
        RecordRead((Uint16*)gawPsdV[0], 128, 2048, 128, 8);
    }

	EnableDog();   //�����Ź�
    for(;;)
    {
        MainCtrl();
		ServiceDog();
    }
}

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16*DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    { 
       *DestAddr++ = *SourceAddr++;
    }
    return;
}

void LEDPlay()
{
	unsigned long i;
	GpioDataRegs.GPADAT.bit.GPIO28 = 1;
	for(i=0;i<0x2ffff;i++);
	GpioDataRegs.GPADAT.bit.GPIO28 = 0;
	for(i=0;i<0x2ffff;i++);
}
