/**** Call Angle_setup() once in main function****/

#define RAD_TO_DEG 180/M_PI
#include<math.h>
#define ACC_FACTOR 16384
#define GYRO_FACTOR 131

unsigned long timer; //it's a timer, saved as a big-ass unsigned int.  We use it to save times from the "micros()" command and subtract the present time in microseconds from the time stored in timer to calculate the time for each loop.
float compAngleX, compAngleY; //These are the angles in the complementary filter

void Angle_setup()
{
	    float GYX=0,GYY=0,GYZ=0;
		int gyx,gyy,gyz,acx,acy,acz;
		GYRO_READ(&gyx,&gyy,&gyz);
		ACC_READ(&acx,&acy,&acz);
		GYX=(float)(gyx-GYRO_XOUT_OFFSET)/GYRO_FACTOR;
		GYY=(float)(gyy-GYRO_YOUT_OFFSET)/GYRO_FACTOR;
		GYZ=(float)(gyz-GYRO_ZOUT_OFFSET)/GYRO_FACTOR;
		float roll=atan2(acy,acz)*RAD_TO_DEG;
		float pitch=atan2(-acx,acz)*RAD_TO_DEG;
		compAngleX = roll;
		compAngleY = pitch;
		timer=millis();
}

void Angle_values(float *X,float *Y)
{
	float GYX=0,GYY=0,GYZ=0;
	int gyx,gyy,gyz,acx,acy,acz;
	GYRO_READ(&gyx,&gyy,&gyz);
	ACC_READ(&acx,&acy,&acz);
	GYX=(float)(gyx-GYRO_XOUT_OFFSET)/GYRO_FACTOR;
	GYY=(float)(gyy-GYRO_YOUT_OFFSET)/GYRO_FACTOR;
	GYZ=(float)(gyz-GYRO_ZOUT_OFFSET)/GYRO_FACTOR;
	float dt = (float)(millis()-timer)/1000 ;
	timer = millis();
	float roll=atan2(acy,acz)*RAD_TO_DEG;
	float pitch=atan2(-acx,acz)*RAD_TO_DEG;
	compAngleX = 0.7 * (compAngleX + GYX * dt) + 0.3 * roll; // Calculate the angle using a Complimentary filter
	compAngleY = 0.7 * (compAngleY + GYY * dt) + 0.3 * pitch;
	*X = compAngleX;
	*Y = compAngleY;
}