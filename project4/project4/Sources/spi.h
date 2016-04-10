#include <stdint.h>

#ifndef SOURCES_SPI_H_
#define SOURCES_SPI_H_

void SPI1_Init();

void SPI1_TX_Byte(uint8_t data);

void SPI1_SS_LOW();
void SPI1_SS_HIGH();

#endif /* SOURCES_SPI_H_ */
