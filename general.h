/*
 * general.h
 *
 * Created: 23.05.2016 10:18:20
 *  Author: Tom_
 */ 


#ifndef GENERAL_H_
#define GENERAL_H_
/************************************************************************/
/* Debug                                                                */
/************************************************************************/
//#define	DEBUG_SPI_SEND	1
//#define	DEBUG_ADNS	1
//#define	DEBUG_ADNS_FIRMWARE	1
//#define SPI_ADNS_PERM_ENABLE	1

/************************************************************************/
/* SPI                                                                  */
/************************************************************************/
#define DDR_SPI_SS		DDRB	//PB0
#define DDR_SPI_SCLK	DDRB	//PB1
#define DDR_SPI_MOSI	DDRB	//PB2
#define DDR_SPI_MISO	DDRB	//PB3
//#define DDR_SPI_ADNS	DDRL	//PL6
#define DDR_SPI_ADNS	DDRB	//PB4

#define PIN_SPI_SS		PIN0
#define PIN_SPI_SCLK	PIN1
#define PIN_SPI_MOSI	PIN2
#define PIN_SPI_MISO	PIN3
#define PIN_SPI_ADNS	PIN4


#define PORT_SPI_ADNS	PORTB

/************************************************************************/
/* gpt                                                                  */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/*  Timer 0																*/
/*  Timer 2																*/
/************************************************************************/




#endif /* GENERAL_H_ */