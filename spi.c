/*
 * spi.c
 *
 * Created: 17.03.2016 14:36:27
 *  Author: Tom
 */ 

#include <avr/io.h>
#include "spi.h"
#include "general.h"
#include "uart0.h"

void spi_init()
{
	//SS, SCLK, MOSI
	//MISO
	//DDRB |= ((1 << PIN0) | (1 << PIN1) | (1 << PIN2));
	//DDRB &= ~(1 << PIN3);
	
	//SS
	DDR_SPI_SS |= (1 << PIN_SPI_SS);
	
	//SCLK
	DDR_SPI_SCLK |= (1 << PIN_SPI_SCLK);
	
	//MOSI
	DDR_SPI_MOSI |= (1 << PIN_SPI_MOSI);
	
	//MISO
	DDR_SPI_MISO &= ~(1 << PIN_SPI_MISO);

	//CS - ADNS
	DDR_SPI_ADNS |= (1 << PIN_SPI_ADNS);
	
	//Master mode
	SPCR |= (1 << MSTR);
	
	
	//clk - 2MHz
	/*
	SPSR |=  (1 << SPI2X);
	SPCR &= ~(1 << SPR1);
	SPCR |=  (1 << SPR0);
	//*/
	
	//clk - 500kHz
	/*
	SPSR |=  (1 << SPI2X);
	SPCR |=  (1 << SPR1);
	SPCR &= ~(1 << SPR0);
	//*/
	
	//clk - 125kHz
	//*
	SPSR &= ~(1 << SPI2X);
	SPCR |=  (1 << SPR1);
	SPCR |=  (1 << SPR0);
	//*/
	
	//falling setup, rising sample | SPI-Mode 3
	SPCR |= ((1 << CPOL) | (1 << CPHA));
	
	//MSB first
	SPCR &= ~(1 << DORD);
	
	//SPI enable
	SPCR |= (1 << SPE);
}

void spi_sendData(uint8_t c)
{
	//uint8_t dummy;
	//uint8_t temp;
	
	#ifdef DEBUG_SPI_SEND
		uart0_putc('1');
		uart0_newLine();
	#endif
	
	SPDR = c;
	
	#ifdef DEBUG_SPI_SEND
		uart0_putc('2');
		uart0_newLine();
	#endif
	
	while(!(SPSR & (1 << SPIF))){
		/*
		//temp = SPSR & (1 << SPIF);
		temp = TCNT5;
		uart0_putCharAsDigits(temp);
		uart0_newLine();
		//*/
	}
	
	#ifdef DEBUG_SPI_SEND
		uart0_putc('3');
		uart0_newLine();
	#endif
	
	//dummy = SPDR;
}

void spi_sendData_16Bit(uint16_t c)
{
	uint8_t temp;
	
	temp = c >> 8;
	spi_sendData(temp);
	temp = c;
	spi_sendData(temp);
}

uint8_t spi_receiveData()
{
	//uint8_t data;
	
	SPDR = 0x00;
	while(!(SPSR & (1 << SPIF)));
	
	return SPDR;
}

uint16_t spi_receiveData_16Bit()
{
	uint8_t temp1, temp2;
	uint16_t temp;
	
	temp1 = spi_receiveData();
	temp2 = spi_receiveData();
	
	temp = temp1 << 8;
	temp |= temp2;
	
	return temp;
}

