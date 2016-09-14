/*
 * adns_9800_test2.c
 *
 * Created: 15.06.2016 10:10:05
 * Author : Tom_
 */ 

#include <avr/io.h>
#include "adns_9800.h"
#include "uart0.h"
#include "gpt.h"

int main(void)
{
	uint8_t data;
	int16_t x, y;
	int8_t x_value, y_value;
	
	uart0_init(9600, 1, 1);
	gpt_init();
    adns9800_init();
	
	//dispRegisters();
	
    while (1) 
    {
		/*
		data = adns9800_read(REG_Product_ID);
		uart0_putCharAsDigits(data);
		uart0_newLine();
		delay(500);
		//*/
		
		//*
		uart0_getc();

		getXYvalue(&x, &y);
		uart0_putSignedIntAsDigits(x);
		uart0_puts(" ");
		uart0_putSignedIntAsDigits(y);
		uart0_newLine();
		//delay(500);
		//*/
		
		/*
		getXYvalue_8Bit(&x_value, &y_value);

		uart0_putSignedIntAsDigits(x_value);
		uart0_puts(" ");
		uart0_putSignedIntAsDigits(y_value);
		uart0_newLine();

		delay(500);
		//*/

		/*
		uart0_getc();
		//delay(1000);

		//if (solo == 1){
		//solo = 0;
		data = adns9800_read(REG_Product_ID);
		//data = adns9800_read(REG_SROM_ID);
		uart0_putCharAsDigits(data);
		uart0_newLine();
		//*/
    }
}

