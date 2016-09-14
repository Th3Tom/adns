/*
 * uart0.c
 *
 * Created: 20.05.2015 12:06:34
 *  Author: Tom
 */ 

#include <avr/io.h>
#include <stdio.h>

#define FOSZ	16000000

void uart0_init(uint16_t baudrate, uint8_t send, uint8_t receive)
{
	UBRR0 = FOSZ/16/baudrate - 1;
	
	if(send)
		UCSR0B |= 1 << TXEN0;		//activate sender
	
	if(receive)
		UCSR0B |= 1 << RXEN0;		//activate receiver
	
	/*
	//7 data bits
	UCSR0C &= ~(1 << UCSZ00);
	UCSR0C |=  (1 << UCSZ01);
	UCSR0B &= ~(1 << UCSZ02);
	
	//2 stop bits
	UCSR0C |= (1 << USBS0);
	
	//odd parity
	UCSR0C |=  ((1 << UPM00) | (1 << UPM01));
	//*/
}

void uart0_putc(char c)
{
	while( !(UCSR0A & (1 << UDRE0))); //wait until transmit buffer is empty
	UDR0 = c;
	/*
	while( !(UCSR0A & (1 << TXC0))); //wait until UART is ready
	UCSR0A |= (1 << TXC0);
	*/
}

void uart0_newLine()
{
	//send CR (carriage return) and LF (line feed)
	uart0_putc(0x0A);
	uart0_putc(0x0D);
}

void uart0_puts(char* pS)
{
	while(*pS != '\0')
	{
		uart0_putc(*pS);
		pS++;
	}
}

void uart0_putsln(char* pS)
{
	while(*pS != '\0')
	{
		uart0_putc(*pS);
		pS++;
	}
	
	uart0_newLine();
}

void uart0_putCharAsDigits(uint8_t value)
{
	uint8_t h = 0, t = 0, u = 0, help;
	
	h = value / 100;
	help = value % 100;
	t = help / 10;
	u = help % 10;
	
	uart0_putc(h + '0');
	uart0_putc(t + '0');
	uart0_putc(u + '0');
}

void uart0_putSignedCharAsDigits(int8_t number)
{
	uint8_t h = 0, t = 0, u = 0, help;
	
	if(number & 0x80){
		uart0_putc('-');
		number = number & ~0x80;
	}
	
	h = number / 100;
	help = number % 100;
	t = help / 10;
	u = help % 10;
	
	uart0_putc(h + '0');
	uart0_putc(t + '0');
	uart0_putc(u + '0');
}

uint8_t uart0_getc()
{
	uint8_t data;
	
	//wait until data are received
	while(!(UCSR0A & (1 << RXC0)));
	data = UDR0;	//get data (bit RXC0 will be cleared automatically)
	
	return data;
}

/*
uint8_t uart0_getc_x(uint8_t ninthBit, uint8_t* pError, uint16_t* pData)
{
	uint8_t error = 0, nBit = 0, result = 0;
	
	//wait until data are received
	while(!(UCSR0A & (1 << RXC0)));
	
	//get status of error bits
	if((UCSR0A & (1 << FE0)))
		error = 0x01;	//set bit 0
		
	if((UCSR0A & (1 << DOR0)))
		error = 0x02;	//set bit 1
		
	if((UCSR0A & (1 << UPE0)))
		error = 0x04;	//set bit 2
		
	//if ninth data bit is available, get it from UCSR0B
	if(ninthBit)
		nBit = (UCSR0B >> 1) & 0x01;
	
	result = UDR0;
	
	*pError = error;
	*pData = result;
	
	if(error)
		return 0; //if any error
	else
		return 1; // if no error
}
//*/

uint8_t uart0_testAndGetc(uint8_t* pC)
{
	if( UCSR0A & (1 << RXC0))
	{
		*pC = UDR0;
		return 1;
	}
	else
	{
		return 0;
	}
}

//send a float value
void uart0_putf(float value)
{
	uint16_t  v;
	
	v = (uint16_t)(value*100);
	
	//uart0_putc( v/100 + '0');
	uart0_putCharAsDigits(v/100);
	uart0_putc('.');
	uart0_putc( (v%100)/10 + '0');
	uart0_putc( v%10 + '0');
}
/*
void uart0_putf(float v)
{
	char buffer[20] = {0};
	
	sprintf(buffer, "%.3f", v);
	uart0_puts(buffer);
}
//*/

void uart0_putIntAsDigits(uint16_t value)
{
	uint8_t h, t, u, v, w;
	uint16_t help;
	
	h = value / 10000;
	help = value % 10000;
	
	t = help / 1000;
	help = help % 1000;
	
	u = help / 100;
	help = help % 100;
	
	v = help / 10;
	
	w = help % 10;
	
	uart0_putc(h + '0');
	uart0_putc(t + '0');
	uart0_putc(u + '0');
	uart0_putc(v + '0');
	uart0_putc(w + '0');
}

void uart0_putSignedIntAsDigits(int16_t value)
{
	//uint8_t h, t, u, v, w;
	//uint16_t number;
	
	if(value & 0x8000){
		uart0_putc('-');
		value = ~value;
		value++;
	}
	
	uart0_putIntAsDigits(value);
}