/***** Call adc_init() and lcd_init() once in main() to use "ADC.c" functions ******/ 


void adc_port_init()			//ADC port initialization
{
	DDRF=0x00;
	PORTF=0x00;
	DDRK=0x00;
	PORTK=0x00;
}

void adc_reg_init()			//ADC registers initialization
{
	ADCSRA=0x00;
	ADCSRB=0x00;
	ADMUX=0x20;			//Enabling Left adjust result
	ACSR=0x80;			//Disabling analog comparator
	ADCSRA=0x86;			//Enabling ADC and setting pre-scalar of 64
}

void adc_init()
{
	adc_port_init();
	adc_reg_init();
}
int ADC_conversion(int Ch)		//function for ADC conversion
{
	int a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for ADC conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; 		//clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;	
}

void print_sensor(int row, int coloumn,int channel)		//function to print ADC values
{
	int ADC_Value = ADC_conversion(channel);
	lcd_print(row, coloumn, ADC_Value, 3);
}

