/*****Call timer5_init() once in main() to use the "velocity.c" functions*****/

void timer5_init()					//Timer 5 initialization
{
	//Fast PWM 8-bit mode (Non-inverting)
	//Pre-scalar of 256
	TCCR5A=0b10101001;
	TCCR5B=0b00001100;
	TCNT5=0;		//Setting timer value to zero
	OCR5A=255;		//Output Compare register A initialization
	OCR5B=255;		//Output Compare register B initialization
}

void velocity(int left_motor,int right_motor)			//Function for velocity variation using PWM
{
	OCR5A=left_motor;
	OCR5B=right_motor;
	
}
