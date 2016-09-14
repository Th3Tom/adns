/*
 * adns_9800.c
 *
 * Created: 15.06.2016 10:13:27
 *  Author: Tom_
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "general.h"
#include "spi.h"
#include "gpt.h"
#include "adns_9800.h"
#include "adns_9800_sram.h"
#include "uart0.h"

void adns9800_com_beginn();
void adns9800_com_end();
void adns9800_firmeware();

void adns9800_init()
{
	uint8_t temp;
	
	spi_init();
	//gpt_init();
	
	adns9800_com_end();
	adns9800_com_beginn();
	adns9800_com_end();
	
	adns9800_write(REG_Power_Up_Reset, 0x5a);
	
	delay(50);
	
	#ifdef DEBUG_ADNS
		uart0_putsln("befor read");
	#endif
	
	adns9800_read(REG_Motion);		//0x02
	adns9800_read(REG_Delta_X_L);	//0x03
	adns9800_read(REG_Delta_X_H);	//0x04
	adns9800_read(REG_Delta_Y_L);	//0x05
	adns9800_read(REG_Delta_Y_H);	//0x06
	
	#ifdef DEBUG_ADNS
		uart0_putsln("befor firmware");
	#endif
	
	adns9800_firmeware();
	delay(10);
	
	#ifdef DEBUG_ADNS
		uart0_putsln("after firmeware");
	#endif
	
	temp = adns9800_read(REG_LASER_CTRL0);
	temp &= 0xf0;
	adns9800_write(REG_LASER_CTRL0, temp);
	
	temp = adns9800_read(REG_LASER_CTRL0);
	temp |= 0x04;
	adns9800_write(REG_LASER_CTRL0, temp);
	
	delay(1);
}

void adns9800_com_beginn()
{
	PORT_SPI_ADNS &= ~(1 << PIN_SPI_ADNS);
}

void adns9800_com_end()
{
	PORT_SPI_ADNS |=  (1 << PIN_SPI_ADNS);
}

void adns9800_write(uint8_t reg, uint8_t data)
{
	adns9800_com_beginn();
	
	delay(1);
	
	spi_sendData(reg | 0x80);
	spi_sendData(data);
	
	//delay(1);
	delay_us(20);
	adns9800_com_end();
	//delay(1);
	delay_us(100);
}

uint8_t adns9800_read(uint8_t reg)
{
	uint8_t data;
	
	adns9800_com_beginn();
	
	//delay(1);

	spi_sendData(reg & 0x7f);
	//delay(1);
	delay_us(100);
	data = spi_receiveData();
	
	//delay(1);
	delay_us(1);
	adns9800_com_end();
	//delay(1);
	delay_us(19);

	return data;
}

void adns9800_firmeware()
{
	uint8_t c;
	
	adns9800_write(REG_Configuration_IV, 0x02);
	adns9800_write(REG_SROM_Enable, 0x1d);
	delay(10);
	adns9800_write(REG_SROM_Enable, 0x18);
	
	adns9800_com_beginn();
	spi_sendData(REG_SROM_Load_Burst | 0x80);
	//delay(1);
	delay_us(15);
	
	for(uint16_t i = 0; i < firmware_length; i++){
		c = pgm_read_byte(firmware_data + i);
		spi_sendData(c);
		
		//delay(1);
		delay_us(15);
	}
	adns9800_com_end();
}

void dispRegisters(void)
{
	
	uint8_t reg[7] = { 0x00, 0x3F, 0x2A, 0x02};
	char* regname[] = {"Product_ID", "Inverse_Product_ID", "SROM_Version", "Motion"};
	uint8_t data;
	uint8_t i;

	adns9800_com_beginn();

	for(i = 0; i < 4; i++){
		spi_sendData(reg[i]);
		delay(1);
		
		uart0_putsln("---");
		uart0_putsln(regname[i]);
		
		spi_sendData(0);
		data = spi_receiveData();
		
		uart0_putCharAsDigits(data);
		uart0_newLine();
		
		delay(1);
	}
	adns9800_com_end();
}

int8_t convTwosComp(uint8_t b){
	//Convert from 2's complement
	if(b & 0x80){
		b = -1 * ((b ^ 0xff) + 1);
	}
	return b;
}

int16_t convTwosComp_16Bit(uint16_t b){
	//Convert from 2's complement
	if(b & 0x8000){
		b = -1 * ((b ^ 0xffff) + 1);
	}
	return b;
}

void getXYvalue(int16_t* x_value, int16_t* y_value)
{
	uint8_t x_l, x_h, y_l, y_h;
	uint16_t x = 0, y = 0;
	
	x_l = adns9800_read(REG_Delta_X_L);
	x_h = adns9800_read(REG_Delta_X_H);
	y_l = adns9800_read(REG_Delta_Y_L);
	y_h = adns9800_read(REG_Delta_Y_H);
	
	x = x_h;
	x = x << 8;
	x |= x_l;
	
	y = y_h;
	y = y << 8;
	y |= y_l;
	
	*x_value = convTwosComp_16Bit(x);
	*y_value = convTwosComp_16Bit(y);
}

void getXYvalue_8Bit(int8_t* x_value, int8_t* y_value)
{
	uint8_t x, y;
	
	x = adns9800_read(REG_Delta_X_L);
	y = adns9800_read(REG_Delta_Y_L);
	
	*x_value = convTwosComp(x);
	*y_value = convTwosComp(y);
}

