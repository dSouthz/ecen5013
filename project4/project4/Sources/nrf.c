#include <stdint.h>
#include "nrf.h"
#include "spi.h"
#include "MKL25Z4.h"

#define DELAY 30

uint8_t *NRF_Read_Register(uint8_t reg, size_t len, uint8_t* buffer)
{
	uint8_t* dataptr = buffer;
	int i;

	if (buffer == NULL)
	{
		return NULL;
	}

	SPI1_SS_LOW();
	SPI1_TX_Byte(reg);

	//This is just to clear the data register after the command is sent
	//The data in this register isn't actually of any use.
	while(!(SPI1->S & SPI_S_SPRF_MASK))
	{
		//Wait for data
	}
	*dataptr = SPI1->D;

	while (len > 0)
	{
		SPI1_TX_Byte(NOP);  //Send NOP (0xFF) to get data
		while(!(SPI1->S & SPI_S_SPRF_MASK))
		{
			//Wait for data
		}
		*dataptr = SPI1->D;

		dataptr++;
		len--;
	}

	for (i=0; i < DELAY; i++) {}
	SPI1_SS_HIGH();
	return buffer;
}

void NRF_Write_Register(uint8_t reg, size_t len, uint8_t *buffer)
{
	uint8_t* dataptr = buffer;
	int i;

	if (len > 0 & buffer == NULL)
	{
		return;
	}

	if (reg > MAX_REG)
	{
		return;
	}

	SPI1_SS_LOW();
	SPI1_TX_Byte(W_REGISTER | reg);

	while (len > 0)
	{
		SPI1_TX_Byte(*dataptr);
		dataptr++;
		len--;
	}

	while (!(SPI1->S & SPI_S_SPTEF_MASK))
	{
		//Do nothing! Wait for flag to clear so we can send
	}
	for (i=0; i < DELAY; i++) {}
	SPI1_SS_HIGH();
}
