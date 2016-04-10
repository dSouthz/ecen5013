/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdint.h>
#include "fsl_device_registers.h"
#include "spi.h"
#include "nrf.h"



int main(void)
{
	uint8_t buffer[256]; //TODO: get rid of this. Only testing with it..
	int i;

	for (i=0; i < 256;i++)
	{
		buffer[i] = 0;
	}

	buffer[10] = 0xDE;
	buffer[11] = 0xAD;
	buffer[12] = 0xBE;
	buffer[13] = 0xEF;
	buffer[14] = 0xAB;

	SystemInit();
	SPI1_Init();

	NRF_Write_Register(REG_RX_ADDR_P0, REG_RX_ADDR_P0_LEN, &buffer[10]);

	NRF_Read_Register(REG_RX_ADDR_P0, REG_RX_ADDR_P0_LEN, buffer);
	//Send power on command
	//SPI1_TX_Byte(0x20); //Write to register 00
	//SPI1_TX_Byte(0x02); //Write power on. Everything else 0 for now.

	//for (i=0; i < 10000; i++){}  //Wait a while to go to Standby state... > 1.5 ms

	//SPI1_TX_Byte(0x0A);	// Query register A

	//ReadSPIData(buffer, 5); // Read 5 bytes of data from SPI

    for (;;) {

    }
    /* Never leave main */
    return 0;
}
