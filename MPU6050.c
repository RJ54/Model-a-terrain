/*** Call MPU6050_init() once in main() to use "MPU6050.c" functions ****/


/*** MPU6050 connections ***/
//VCC_IN -> 22
//GND -> 24
//SDA -> 19
//SCL -> 20


#define DEV_ID 0x68<<1					//MPU6050 Address
#define ACCEL_XOUT_H 0x3B				//ACCELEROMETER-X OUTPUT HIGHER BYTE ADDRESS
#define ACCEL_YOUT_H 0x3D				//ACCELEROMETER-Y OUTPUT HIGHER BYTE ADDRESS
#define ACCEL_ZOUT_H 0x3F				//ACCELEROMETER-Z OUTPUT HIGHER BYTE ADDRESS
#define GYRO_XOUT_H 0x43				//GYROSCOPE-X OUTPUT HIGHER BYTE ADDRESS
#define GYRO_YOUT_H 0x45				//GYROSCOPE-Y OUTPUT HIGHER BYTE ADDRESS
#define GYRO_ZOUT_H 0x47				//GYROSCOPE-Z OUTPUT HIGHER BYTE ADDRESS
#define GYRO_CONFIG 0x1B				//GYROSCOPE CONFIG REGISER
#define ACCEL_CONFIG 0x1C				//ACCELEROMETER CONFIG REGISTER
#include "i2c_lib.c"
float GYRO_XOUT_OFFSET=0,GYRO_YOUT_OFFSET=0,GYRO_ZOUT_OFFSET=0;
float ACC_XOUT_OFFSET=0,ACC_YOUT_OFFSET=0,ACC_ZOUT_OFFSET=0;

void MPU6050_init()
{
	i2c_init();
	i2c_sendbyte(DEV_ID,0x6B,0x00);								//Setting MPU6050 out of sleep mode
	_delay_ms(100);
	
	int GYX,GYY,GYZ;
	int ACX,ACY,ACZ;
//calculating gyroscope offset
	for(int i=0;i<1000;i++)
	{
		GYRO_READ(&GYX,&GYY,&GYZ);
		GYRO_XOUT_OFFSET+=GYX;
		GYRO_YOUT_OFFSET+=GYY;
		GYRO_ZOUT_OFFSET+=GYZ;
	}
	GYRO_XOUT_OFFSET/=1000;
	GYRO_YOUT_OFFSET/=1000;
	GYRO_ZOUT_OFFSET/=1000;
	
//calculating accelerometer offset
	for(int i=0;i<1000;i++)
	{
		ACC_READ(&ACX,&ACY,&ACZ);
		ACC_XOUT_OFFSET+=ACX;
		ACC_YOUT_OFFSET+=ACY;
		ACC_ZOUT_OFFSET+=ACZ;
	}
	ACC_XOUT_OFFSET/=1000;
	ACC_YOUT_OFFSET/=1000;
	ACC_ZOUT_OFFSET/=1000;
}

void ACC_READ(int *ACX ,int *ACY,int *ACZ)				//Function to read 3-axis ACCELEROMETER readings
{
	UINT8 ACC_X[2];
	UINT8 ACC_Y[2];
	UINT8 ACC_Z[2];
	INT16 acc_x =0 ,acc_y=0,acc_z=0;
	i2c_read_multi_byte(DEV_ID,ACCEL_XOUT_H,2,ACC_X);			//Getting 2 Bytes reading of ACCELEROMETER-X
	i2c_read_multi_byte(DEV_ID,ACCEL_YOUT_H,2,ACC_Y);			//Getting 2 Bytes reading of ACCELEROMETER-Y
	i2c_read_multi_byte(DEV_ID,ACCEL_ZOUT_H,2,ACC_Z);			//Getting 2 Bytes reading of ACCELEROMETER-Z
	
/************** Accerometer readings calculations ************/
		acc_x = ACC_X[0]<<8;
		acc_x = acc_x | ACC_X[1];
		acc_y = ACC_Y[0]<<8;
		acc_y = acc_y | ACC_Y[1];
		acc_z = ACC_Z[0]<<8;
		acc_z = acc_z | ACC_Z[1];
		*ACX=acc_x;
		*ACY=acc_y;
		*ACZ=acc_z;
}

void GYRO_READ(int *GCX, int *GCY ,int *GCZ)				//Function to read 3-axis GYROSCOPE readings
{
	UINT8 GYRO_X[2];
	UINT8 GYRO_Y[2];
	UINT8 GYRO_Z[2];
	INT16 gyro_x =0 ,gyro_y=0,gyro_z=0;
	i2c_read_multi_byte(DEV_ID,GYRO_XOUT_H,2,GYRO_X);			//Getting 2 Bytes reading of GYROSCOPE-X
	i2c_read_multi_byte(DEV_ID,GYRO_YOUT_H,2,GYRO_Y);			//Getting 2 Bytes reading of GYROSCOPE-Y
	i2c_read_multi_byte(DEV_ID,GYRO_ZOUT_H,2,GYRO_Z);			//Getting 2 Bytes reading of GYROSCOPE-Z
	
/**************** Gyroscope readings calculation ****************/
	gyro_x = GYRO_X[0]<<8;
	gyro_x = gyro_x | GYRO_X[1];
	gyro_y = GYRO_Y[0]<<8;
	gyro_y = gyro_y | GYRO_Y[1];
	gyro_z =GYRO_Z[0]<<8;
	gyro_z = gyro_z | GYRO_Z[1];
	
	*GCX =gyro_x;
	*GCY =gyro_y;
	*GCZ =gyro_z;
}

