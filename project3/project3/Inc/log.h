#ifndef SOURCES_LOG_H_
#define SOURCES_LOG_H_

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

static const int MAX_FLOAT_STRING_SIZE = 32;

// Logs a string to the UART
// Params: 	str - string to log
// 			len - string length
//
void LOG_UART(char *str, size_t len);

// Logs a string and parameter to the UART
// Params: 	str   - string to log
// 			len   - string length
//			param - the parameter to log
//			type  - the type of parameter to log
void LOG_UART_PARAM(char *str, size_t len, void *param, DataType_t type);

#endif /* SOURCES_LOG_H_ */
