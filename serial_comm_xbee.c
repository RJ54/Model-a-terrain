//Call uart0_init() once in main() to use the "serial_comm_xbee.c" functions

void uart0_init(void)		//USART0 intitialisation
{
	cli();//clear global interrupts
	UCSR0B = 0x00; //disable while setting baud rate
	UCSR0A = 0x00;
	UCSR0C = 0x06;//default value
	UBRR0L = 0x5F; //set baud rate low
	UBRR0H = 0x00; //set baud rate high
	UCSR0B |=(1<<RXEN0)|(1<<TXEN0);//Enabling Receiver and transmitter  
	sei();//Set global interrupt
}

void USART_Transmit( unsigned char data )		//Function to transmit a single byte
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )		//Function for receiving a single byte
{	
	 /* Wait for data to be received */
	 while ( !(UCSR0A & (1<<RXC0)) );
	 /* Get and return received data from buffer */
	 return UDR0;
}

void send_string(char* mydata)		//Function for sending a string
{
	int i;
	int n = strlen(mydata);
	for(i=0;i<n;++i)
	{
		USART_Transmit(mydata[i]);
	}
}
