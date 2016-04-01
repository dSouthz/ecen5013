#ifndef SOURCES_UART_H_
#define SOURCES_UART_H_

#include <stddef.h>

typedef enum _data_type
{
	INTEGER_8,
	INTEGER_16,
	INTEGER_32,
	UINTEGER_8,
	UINTEGER_16,
	UINTEGER_32,
	FLOAT
} DataType_t;

void Configure_UART();

void SendChar(char c);

char ReceiveChar();

void LOG_UART(char *str, size_t len);
void LOG_UART_PARAM(char *str, size_t len, void *param, DataType_t type);

#endif /* SOURCES_UART_H_ */
