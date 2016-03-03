//###########################################################################
// FILE:    9150.c
//
// TITLE:   �����������ʼ��
//###########################################################################
#include "DSP280x_Device.h"     // DSP280x Headerfile Include File
//#include "Driver_I2C.h"

//****************************************
// ����MPU6050�ڲ���ַ
//****************************************
#define SMPLRT_DIV      0x19    //�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define CONFIG          0x1A    //��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define GYRO_CONFIG     0x1B    //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define ACCEL_CONFIG    0x1C    //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40
#define TEMP_OUT_H      0x41
#define TEMP_OUT_L      0x42
#define GYRO_XOUT_H     0x43
#define GYRO_XOUT_L     0x44    
#define GYRO_YOUT_H     0x45
#define GYRO_YOUT_L     0x46
#define GYRO_ZOUT_H     0x47
#define GYRO_ZOUT_L     0x48
#define PWR_MGMT_1      0x6B    //��Դ��������ֵ��0x00(��������)
#define WHO_AM_I            0x75    //IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define SlaveAddress    0xD0    //IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

extern Uint16 I2CA_WriteData(Uint16 num,Uint16 RomAddress,unsigned char *str);
extern Uint16 I2CA_ReadData(unsigned char *RamAddr, Uint16  RomAddress, Uint16 number);
extern void board_i2c_init();
extern void eeprom_init();
unsigned char  MagFlag=0;

//**************************************
//��I2C�豸д��һ���ֽ�����
//**************************************
unsigned char  Single_WriteI2C(uchar REG_Address,uchar REG_data)
{
/*     I2C_Start();                  //��ʼ�ź�            */
/*     I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź� */
/*     I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��    */
/*     I2C_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�    */
/*     I2C_Stop();                   //����ֹͣ�ź�        */
    return I2CA_WriteData(1,REG_Address,&REG_data);

}

//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
unsigned char Single_ReadI2C(uchar REG_Address,uchar *dat)
{
/*  uchar REG_data;                                              */
/*  I2C_Start();                   //��ʼ�ź�                    */
/*  I2C_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�         */
/*  I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ   */
/*  I2C_Start();                   //��ʼ�ź�                    */
/*  I2C_SendByte(SlaveAddress+1);  //�����豸��ַ+���ź�         */
/*  REG_data=I2C_RecvByte();       //�����Ĵ�������              */
/*  I2C_SendACK(1);                //����Ӧ���ź�                */
/*  I2C_Stop();                    //ֹͣ�ź�                    */
/*  return REG_data;                                             */
    return I2CA_ReadData(dat,REG_Address,1);
}

void MPU9150_Delay(unsigned int i)
{
    while(--i);
}
  
//**************************************
//��ʼ��MPU6050
//**************************************
void InitMPU9150()
{
    Single_WriteI2C(0x31,0x0B); MPU9150_Delay(5000);   //������Χ,����16g��13λģʽ
    Single_WriteI2C(0x2C,0x08);  MPU9150_Delay(5000);  //�����趨Ϊ12.5 �ο�pdf13ҳ
    Single_WriteI2C(0x2D,0x08); MPU9150_Delay(5000);   //ѡ���Դģʽ   �ο�pdf24ҳ
    Single_WriteI2C(0x2E,0x80); MPU9150_Delay(5000);   //ʹ�� DATA_READY �ж�
    Single_WriteI2C(0x1E,0x00); MPU9150_Delay(5000);   //X ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
    Single_WriteI2C(0x1F,0x00); MPU9150_Delay(5000);   //Y ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
    Single_WriteI2C(0x20,0x05);  MPU9150_Delay(5000);  //Z ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
}

//**************************************
//�ϳ�����
//**************************************
unsigned char GetData(uchar REG_Address,short *dat)
{
    unsigned char  H=0,L=0;
    if(Single_ReadI2C(REG_Address+1,&H)!=0)
        return 1;
    MPU9150_Delay(50);
    if(Single_ReadI2C(REG_Address,&L)!=0)
        return 1;
    MPU9150_Delay(50);
    *dat =  (unsigned short)(H<<8)+L;   //�ϳ�����
    return 0;
}
 
//**************************************
//��ʼ��MPU6050
//**************************************
unsigned char MPU9150_GetAcc(unsigned char type,    short *val)
{
    unsigned char ret;
    switch(type)
    {
        case 0:
            ret =  GetData(0x32,val);
        break;
        case 1:
            ret =  GetData(0x34,val);
        break;
        case 2:
            ret =  GetData(0x36,val);
        break;
    }
    return ret;
}

//**************************************
//��ʼ��MPU6050
//**************************************
unsigned char MPU9150_GetGyo(unsigned char type,    short *val)
{
    unsigned char ret;
    switch(type)
    {
        case 0:
            ret =  GetData(0x43,val);
        break;
        case 1:
            ret =  GetData(0x45,val);
        break;
        case 2:
            ret =  GetData(0x47,val);
        break;
    }
    return ret;
}

//**************************************
//��ʼ��MPU6050
//**************************************
unsigned char MPU9150_GetMag(unsigned char type,    short *val)
{
    unsigned char ret,temp;

    MagFlag =0;
    Single_ReadI2C(0x37, &temp);
    MPU9150_Delay(50);
    Single_WriteI2C(0x37, temp | 0x02);
    MPU9150_Delay(500);
    MagFlag =1;
    Single_WriteI2C(0x0A, 0x01);
    MPU9150_Delay(50);
    temp = 0;
    while((temp&0x0001)!=0x0001)
    {
        Single_ReadI2C(0x02, &temp);
        MPU9150_Delay(50);
    }
    switch(type)
    {
        case 0:
            ret =  GetData(0x03,val);
        break;
        case 1:
            ret =  GetData(0x05,val);
        break;
        case 2:
            ret =  GetData(0x07,val);
        break;
    }
    MagFlag =0;
    *val = (*val) &0x8FFF;
    *val = ((((*val)>>8)&0x00FF) | (((*val)<<8)&0xFF00))&0x8FFF;
    MPU9150_Delay(50);
//  Single_ReadI2C(0x37, &temp);
//  MPU9150_Delay(50);
//  Single_WriteI2C(0x37, temp & 0xFD);
//  MPU9150_Delay(50);
    return ret;
}





#define BUFFSIZE   16
volatile short gaMPUData1_1;
short gaMPUData0[BUFFSIZE];
//short gaMPUData1[BUFFSIZE];
//short gaMPUData2[BUFFSIZE];

int   reading0 = 0;
int   reading1 = 0;
int   reading2 = 0;
int   max_reading = 4;

volatile int thr_h = 132;    ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//#define ControlInterval 64
#define CycleTimes  10

#define IntervalHex ((1 << CycleTimes) - 1)



volatile long sum=0;
volatile int li=0;
//**************************************
//��ʼ��MPU6050
//**************************************
int MPU9150Test()
{
    short m=0,n=0;
   // long sum=0, sum1, sum2;
    short va1, va2, va3;
    int lj= 0; 
    int lk = 0;
    uchar a[3];
    int result = 0;
    int zvalue = 0;

    //Single_ReadI2C(0x00,a);
	li=0;
    sum = 0;
    
    for(;;)
    {
        if(MPU9150_GetAcc(0,&va1)==0)  
        {
     //       if((va1 >=0) && (va1 <= 265))    //�ų�����ֵ
		//	{
            	sum += va1;
            	li++;
		//	}
        }
        if(li >= 2000)   //֮ǰ��1000�����ڸĳ�2000
        {
            sum=sum/2000;
           // gaMPUData0[reading0&(BUFFSIZE - 1)] = sum;
            //gaMPUData1_1 = gaMPUData0[reading0&(BUFFSIZE - 1)];
            gaMPUData1_1 = sum;
            sum = 0;
            li = 0;
			ServiceDog();
            return 0;
        }
		ServiceDog();
    }


}



