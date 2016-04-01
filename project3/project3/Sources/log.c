#include <stdint.h>
#include "conversion.h"
#include "uart.h"
#include "log.h"

#define VERBOSE

#ifdef VERBOSE
void LOG_UART(char *str, size_t len)
{
	size_t i;

	for(i=0; i < len; i++)
	{
		SendChar(str[i]);
		if (str[i] == '\0')
		{
			break;
		}
	}
	SendChar('\n');
}

void LOG_UART_PARAM(char *str, size_t len, void *param, DataType_t type)
{
	size_t i;

	for (i=0; i < len; i++)
	{
		SendChar(str[i]);
	}

	SendChar(':');
	SendChar(' ');

	if (type != FLOAT)
	{
		int temp;
		unsigned int utemp;
		char *string;

		switch (type)
		{
			case INTEGER_8:
				temp = *((int8_t *)param);
				string = itoa(temp);
				break;
			case INTEGER_16:
				temp = *((int16_t *)param);
				string = itoa(temp);
				break;
			case INTEGER_32:
				temp = *((int32_t *)param);
				string = itoa(temp);
				break;
			case UINTEGER_8:
				utemp = *((uint8_t *)param);
				string = uitoa(utemp);
				break;
			case UINTEGER_16:
				utemp = *((uint16_t *)param);
				string = uitoa(utemp);
				break;
			case UINTEGER_32:
				utemp = *((uint32_t *)param);
				string = uitoa(utemp);
				break;
			default:
				return;
		}

		i = 0;
		while (1)
		{
			SendChar(string[i]);
			if (string[i] == '\0')
			{
				break;
			}
			i++;
		}
	}
	else
	{
		char conversion[MAX_FLOAT_STRING_SIZE];
		char *result = dtoa(conversion, *((float *)param));
		i = 0;
		while (result[i] != '\0')
		{
			SendChar(result[i]);
			i++;
		}
	}

	SendChar('\n');
}
#else
void LOG_UART(char *str, size_t len)
{
	//Do nothing!
}

void LOG_UART_PARAM(char *str, size_t len, void *param, DataType_t type)
{
	//Do nothing!
}
#endif //VERBOSE
