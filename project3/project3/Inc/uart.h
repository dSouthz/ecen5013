#ifndef SOURCES_UART_H_
#define SOURCES_UART_H_

#include <stddef.h>

void Configure_UART();

void SendChar(char c);

char ReceiveChar();

#endif /* SOURCES_UART_H_ */
