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

#include "fsl_device_registers.h"
#include "conversion.h"
#include "uart.h"
#include <string.h>
#include <stdint.h>

#define EXTRA_SPACE 100
void PrintDemo()
{
	char testing[] = "Testing123, Serial Print Test, no params";
    char integer[] = "This is an integer";
    char floating[] = "This is a floating point number";
    char concat[EXTRA_SPACE] = "This string will be concated with a number. ";

    int 		test_concat     = -98;
    uint8_t  	test_int_200    = 200;
    uint16_t	test_int_4096   = 4096;
    uint32_t 	test_int_123456 = 123456;
    float   	test_float	    = 1543.321;

    LOG_UART(testing, strlen(testing));

    LOG_UART_PARAM(integer, strlen(integer), &test_int_200, UINTEGER_8);
    LOG_UART_PARAM(integer, strlen(integer), &test_int_4096, INTEGER_16);
    LOG_UART_PARAM(integer, strlen(integer), &test_int_123456, INTEGER_32);
    LOG_UART_PARAM(floating, strlen(floating), &test_float, FLOAT);

    LOG_UART(concat, strlen(concat));
    LOG_UART_PARAM(integer, strlen(integer), &test_concat, INTEGER_32);
    strcat(concat, itoa(test_concat));
    LOG_UART(concat, strlen(concat));
}

int main(void)
{
	SystemInit();
	Configure_UART();

	PrintDemo();

	for (;;)
	{
		c = ReceiveChar();
		SendChar(c);
	}
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
