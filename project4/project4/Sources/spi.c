#include <stdint.h>
#include "MKL25Z4.h"
#include "spi.h"

#define ALT1 0b001
#define ALT2 0b010

#define PIN_SPI1_MOSI 1
#define PIN_SPI1_SCK  2
#define PIN_SPI1_MISO 3
#define PIN_SPI1_PCS0 4

#define SPI_PRESCALE_DIV	0b011		// Divide by 4
#define SPI_BAUD_DIV		0b0001		// Divide by 4

void SPI1_TX_Byte(uint8_t data)
{
	while (!(SPI1->S & SPI_S_SPTEF_MASK))
	{
		//Do nothing! Wait for flag to clear so we can send
	}
	SPI1->D = data;
}

void SPI1_Init()
{
	SIM->SCGC4 |= SIM_SCGC4_SPI1_MASK; 	//Enable SPI1 clock gate
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; //Have to enable the clock to the port

	SPI1->C1 = 	SPI_C1_SPE_MASK		| 	// Enable SPI system
				SPI_C1_MSTR_MASK	| 	// Obey your MASTER mode
				SPI_C1_SSOE_MASK;		// This + MODFEN 1 = SS pin is automatic

	SPI1->C2 =	SPI_C2_MODFEN_MASK;

	SPI1->BR =  SPI_BR_SPPR(SPI_PRESCALE_DIV) | SPI_BR_SPR(SPI_BAUD_DIV);

	//Configure SPI pins
	PORTE->PCR[PIN_SPI1_MOSI] |= PORT_PCR_MUX(ALT2);
	PORTE->PCR[PIN_SPI1_SCK]  |= PORT_PCR_MUX(ALT2);
	PORTE->PCR[PIN_SPI1_MISO] |= PORT_PCR_MUX(ALT2);
	PORTE->PCR[PIN_SPI1_PCS0] |= PORT_PCR_MUX(ALT2);
}
