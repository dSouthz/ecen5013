#include "MKL25Z4.h"
#include "uart.h"
#include "conversion.h"

#define UART_115200_MOD 5
#define UART_115200_OSC	14
#define UART_OSC_CLOCK 0x2

void Configure_UART()
{
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(UART_OSC_CLOCK); // UART use OSC Clock (24 MHz? - using SystemInit to give us this)
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; // Enable clock gate for UART0

	//This is 38400 Baud - Works! :)
	//UART0->C4 = 12; // Over sample rate (OSC)
	//UART0->BDL = 16; // Modulo - 24MHz / (115200 * (OSC + 1)) where OSC = 12

	//C4 = 14 and BDL = 5 displayed 'A' on the terminal! :)
	//WORKS WITH 115200!!
	//UART0->BDL = 5;
	//UART0->C4 = 14;

	UART0->BDL = UART_115200_MOD;
	UART0->C4  = UART_115200_OSC;
	UART0->C2 = UART_C2_TE_MASK | UART_C2_RE_MASK; //Enable TX and RX




	//Need to configure PTA1 & PTA2 as alt2 (UART0_RX, UART0_TX)
	PORTA->PCR[1] |= PORT_PCR_MUX(2);
	PORTA->PCR[2] |= PORT_PCR_MUX(2);
}

void SendChar(char c)
{
	while(!(UART0->S1 & UART_S1_TC_MASK)); //Wait until free.
	UART0->D = c;
}

char ReceiveChar()
{
	while(!(UART0->S1 & UART_S1_RDRF_MASK)); //Wait until full
	return UART0->D;
}
