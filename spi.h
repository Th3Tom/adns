/*
 * spi.h
 *
 * Created: 17.03.2016 14:36:09
 *  Author: Tom
 */ 


#ifndef SPI_H_
#define SPI_H_

void spi_init();
void spi_sendData(uint8_t c);
void spi_sendData_16Bit(uint16_t c);
uint8_t spi_receiveData();
uint16_t spi_receiveData_16Bit();

#endif /* SPI_H_ */