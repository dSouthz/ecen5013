#include <string.h>
#include <stdint.h>
#include "fsl_device_registers.h"
#include "conversion.h"
#include "uart.h"
#include "log.h"
#include "circularbuffer.h"
#include "unit_test.h"

#define EXTRA_SPACE 100

//Prints a UART demo, showing that integers and floating points can be converted and sent
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
	char c;

	SystemInit();
	Configure_UART();

	PrintDemo();
	RunTestSuite();

	for (;;)
	{
		c = ReceiveChar();
		SendChar(c);
	}
    return 0;
}
