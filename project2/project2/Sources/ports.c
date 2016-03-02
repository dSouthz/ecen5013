/*
 * ports.c
 *
 *  Created on: Feb 27, 2016
 *      Author: Sean
 */
#include "MKL25Z4.h"
#include "ports.h"

void portB_setup()
{
	// Note: 	PTB18 is TMP2_CH0
	//			PTB19 is TMP2_CH1

	//Enable clock on PORTB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

	//Configure PORTB, pins 18, 19 to be PWM
	PORTB_PCR18 = PORT_PCR_MUX(PORT_MUX_ALT3);
	PORTB_PCR19 = PORT_PCR_MUX(PORT_MUX_ALT3);
}

void portD_setup()
{
	//Note:		PTD1 is TMP0_CH1

	//Enable clock on PORTB
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;

	//Configure PORTD, pins 1 to be PWM
	PORTD_PCR1 = PORT_PCR_MUX(PORT_MUX_ALT4);
}
