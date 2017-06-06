#define F_CPU 14745600
#define RAD_TO_DEG 180/M_PI
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<math.h>

#define ACC_FACTOR 16384
#define GYRO_FACTOR 131
#define SS_THRESHOLD 90
#define VERY_FAST 100
#define MED 150
#define FAST 200
#define VERY_FAST 250
#define LINE_THRESHOLD 10
#include "lcd.c"
#include "serial_comm_xbee.c"
#include "MPU6050.c"
#include "millis.c"
#include "ADC.c"
#include "Pos_control.c"
#include "velocity.c"
////////////////////////////#include "color.c"
#include "Servo1.c"
#include "Angle_calc.c"

void setup()
{
	uart0_init();
	lcd_init();
	MPU6050_init();
	millis_init();
	adc_init();
	motor_control_init();
	timer5_init();
////////////////////////	TCS3200_init();
	init_servo();
	Angle_setup();
}

void Right_90()
{
	velocity(FAST,FAST);
	forward_mm(65);
	right_degrees(90);
}

void left_90()
{
	velocity(FAST,FAST);
	forward_mm(65);
	left_degrees(90);
}

void U_turn()
{
	velocity(FAST,FAST);
	forward_mm(65);
	right();
	angle_rotate(180);
}

void loop()
{
	while(1)
	{
		char NOD='0',COL1='0',COL2='0',INST;
		int SSM=0,SSF=0,ADC1,ADC2,ADC3,L_S,M_S,R_S,IR2,IR3,IR4;
		float AX,AY;
		
		forward();								//Initiating the robot
		velocity(VERY_FAST,VERY_FAST);
		
		Angle_values(&AX,&AY);					//Getting Euler angles about X and Y axis
		ADC1=ADC_conversion(3);					//White line sensor 1 readings	
		ADC2=ADC_conversion(2);					//White line sensor 2 readings
		ADC3=ADC_conversion(1);					//White line sensor 3 readings
		SSM = ADC_conversion(9);				//Sharp sensor movable readings
		SSF = ADC_conversion(13);				//Sharp sensor fixed readings
		IR2 = ADC_conversion(5);				//IR sensor 2 readings
		IR3 = ADC_conversion(6);				//IR sensor 3 readings
		IR4 = ADC_conversion(7);				//IR sensor 4 readings

		
		if(ADC1>LINE_THRESHOLD)
			L_S=0;
		else
			L_S=1;
		
		if(ADC2>LINE_THRESHOLD)
			M_S=0;
		else
			M_S=1;
		
		if(ADC3>LINE_THRESHOLD)
			R_S=0;
		else
			R_S=1;
		
		if(SSM>SS_THRESHOLD)
			SSM=1;
		else 
			SSM=0;
			
		if(SSF>SS_THRESHOLD)
			SSF=1;
		else
			SSF=0;		
/********************SERVO AND COLOUR SENSOR PART********************/

		if(SSM==1 || SSF==1)				//checking for objects either sides
		{
			char dataC[20];
			_delay_ms(50);
			Stop();
			buzzer_on();
			_delay_ms(200);
			buzzer_off();
			if(SSM==0 && SSF==1)		
			{
				servo_1(180);
				_delay_ms(2000);
	//			COL2 = colorDetect();
				servo_1(0);
				_delay_ms(100);
			}
			
			else if(SSM==1 && SSF==0)
			{
				_delay_ms(2000);
	//			COL1 = colorDetect();
			}
			
			else 
			{
				_delay_ms(2000);
	//			COL1 = colorDetect();
				servo_1(180);
				_delay_ms(2000);
	//			COL2 = colorDetect();
				servo_1(0);
				_delay_ms(100);
			}
			snprintf(dataC,sizeof(dataC),"C:%03d,%03d,%c,%c",SSM,SSF,COL1,COL2);
			send_string(dataC);
			USART_Transmit(13);
			forward();
			velocity(VERY_FAST,VERY_FAST);
			_delay_ms(100);
		}

		if(L_S==1 && M_S==1 && R_S==1)
		{
			forward();
			velocity(VERY_FAST,VERY_FAST);
		}

		if(L_S==1 && M_S==0 && R_S==1)
		{
			forward();
			velocity(VERY_FAST,VERY_FAST);
		}

		if(L_S==1 && M_S==1 && R_S==0)
		{
			right();
			velocity(120,50);
			_delay_ms(60);
		}

		if(L_S==0 && M_S==1 && R_S==1)
		{
			left();
			velocity(50,120);
			_delay_ms(60);
		}
		
		if((L_S==0 && M_S==0 && R_S==1) || (L_S==1 && M_S==0 && R_S==0)|| (L_S==0 && M_S==0 && R_S==0))
		{
			char dataN[20];
			NOD = '1';
			Stop();
			buzzer_on();
			_delay_ms(10);
			buzzer_off();
/******* SENDING SECTION WHEN NODE DETECTED***********/	

			snprintf(dataN,sizeof(dataN),"N:%03d,%03d,%03d",IR2,IR3,IR4);
			send_string(dataN);
			USART_Transmit(13);
			INST = USART_Receive();
			if(INST=='F')
			{
				velocity(VERY_FAST,VERY_FAST);
				forward();
				_delay_ms(700);
			}
			
			if(INST=='R')
			{
				Right_90();
			}
			
			if(INST=='L')
			{
				left_90();
			}
			
			if(INST=='U')
			{
				U_turn();
			}
			
			if(INST=='S')
			{
				Stop();
				buzzer_on();
				_delay_ms(5000);
				buzzer_off();
				break;
			}
		}
		
		else
		{
			/**********Sending Section ***********/
			char dataD[30];
			snprintf(dataD,sizeof(dataD),"D:%5.2f,%5.2f,%03d,%03d,%03d",AX,AY,IR2,IR3,IR4);
			send_string(dataD);
			USART_Transmit(13);
		}
	}
	
}

void main()
{
	setup();
	DDRE&=~(1<<PINE7);					//Initialization for Interrupt switch
	while(bit_is_set(PINE,7))			//waiting for interrupt switch to be pressed
	;
	loop();
}