#ifndef SOURCES_UART_H_
#define SOURCES_UART_H_

#include <stddef.h>

// Configures UART0 for both write and read
// UART0 is configured to 115200 baud, 1 stop bit, no parity
void Configure_UART();

// Sends a single character over UART
// This is a blocking call
void SendChar(char c);

// Receives a character from UART
// This is a blocking call
//
// Returns: character received from UART
char ReceiveChar();

#endif /* SOURCES_UART_H_ */
