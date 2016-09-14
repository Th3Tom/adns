/*
 * uart0.h
 *
 * Created: 20.05.2015 12:07:33
 *  Author: Tom
 */ 


#ifndef UART0_H_
#define UART0_H_

void uart0_init(uint16_t baudrate, uint8_t send, uint8_t receive);
void uart0_putc(char c);
void uart0_puts(char* pS);
void uart0_newLine();
void uart0_putCharAsDigits(uint8_t value);
uint8_t uart0_getc();
//uint8_t uart0_getc_x(uint8_t ninthBit, uint8_t* pError, uint16_t* pData);
uint8_t uart0_testAndGetc(uint8_t* pC);
void uart0_putf(float v);
void uart0_putsln(char* pS);

/****************************************************************************/
/* Function: uart_putIntAsDigit												*/
/* Parameter:																*/
/*		value:16Bit number which will be send to the UART					*/
/* Return value:															*/
/*		none																*/
/****************************************************************************/
void uart0_putIntAsDigits(uint16_t value);

void uart0_putSignedCharAsDigits(int8_t number);
void uart0_putSignedIntAsDigits(uint16_t value);

#endif /* UART0_H_ */