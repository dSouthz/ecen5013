/*
 * timer.c
 *
 *  Created on: Feb 27, 2016
 *      Author: Sean
 */
#include <stdint.h>
#include "MKL25Z4.h"
#include "timer.h"

void enable_timer0_interrupts()
{

	//IRQ definitions in MKL25Z4.h
	//This function comes from core_cm0plus.h
	//NVIC_EnableIRQ(TPM2_IRQn);
	NVIC_EnableIRQ(TPM0_IRQn);
}

void main_clk_init()
{

	//Select MCG for internal reference clock and enable
	//MCG_BASE_PTR->C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK;
	MCG_BASE_PTR->C1 = MCG_C1_IRCLKEN_MASK;

	//Select fast internal reference clock - 4 MHz
	MCG_BASE_PTR->C2 = MCG_C2_IRCS_MASK;

	//Set Fast clock internal reference divider to divide by 1 (4MHz freq). This is option 0x00.
	MCG_BASE_PTR->SC &= ~(MCG_SC_FCRDIV_MASK);

	//Set TPM to use MCGIRCLK
	SIM_BASE_PTR->SOPT2 |= SIM_SOPT2_TPMSRC(SIM_TPM_SRC_MCGIRCLK);
	//enable TPM clocks
	//TODO: use this! SIM_SCGC6 |= SIM_SCGC6_TPM0(TPM_ENABLED) | SIM_SCGC6_TPM1(TPM_ENABLED) | SIM_SCGC6_TPM2(TPM_ENABLED);
	//TODO: only one clock for now....
	// PTB18 is TPM2_CH0
	// PTB19 is TPM2_CH1
	// PTD01 is TPM0_ch1
	SIM_SCGC6 |= SIM_SCGC6_TPM0(TPM_ENABLED) | SIM_SCGC6_TPM2(TPM_ENABLED);


}

//TODO: rename - this is for red LED
void timer2_init()
{
	// Increment timer on every clock and prescale the clock by 1
	//TPM2_BASE_PTR->SC = TPM_SC_CMOD(TPM_INCREMENT_EVERY_CLK) | TPM_SC_PS(TPM_PRESCALE_DIV_8);
	TPM2_BASE_PTR->SC = TPM_SC_CMOD(TPM_INCREMENT_EVERY_CLK) | TPM_SC_PS(TPM_PRESCALE_DIV_1);

	TPM2_BASE_PTR->SC &= ~(TPM_SC_CPWMS_MASK); //Set CPWMS to up counting

	// Set overflow value to max
	TPM2_BASE_PTR->MOD = UINT16_MAX;


	//Set to Edge-aligned PWM, high true pulses (clear output on match, set output on reload)
	TPM2_BASE_PTR->CONTROLS[TPM_CTRL_CH_RED].CnSC = TPM_CnSC_ELSB(TPM_ENABLED) |
													TPM_CnSC_ELSA(TPM_DISABLED) |
													TPM_CnSC_MSB(TPM_ENABLED) |
													TPM_CnSC_MSA(TPM_DISABLED);

	TPM2_BASE_PTR->CONTROLS[TPM_CTRL_CH_GRN].CnSC = TPM_CnSC_ELSB(TPM_ENABLED) |
													TPM_CnSC_ELSA(TPM_DISABLED) |
													TPM_CnSC_MSB(TPM_ENABLED) |
													TPM_CnSC_MSA(TPM_DISABLED);



	//Red and Green LEDs are off to start
	TPM2_BASE_PTR->CONTROLS[TPM_CTRL_CH_RED].CnV = START_VALUE_RED;
	TPM2_BASE_PTR->CONTROLS[TPM_CTRL_CH_GRN].CnV = START_VALUE_GREEN;
}


void timer0_init()
{
	// Increment timer on every clock and prescale the clock by 1
	//TPM2_BASE_PTR->SC = TPM_SC_CMOD(TPM_INCREMENT_EVERY_CLK) | TPM_SC_PS(TPM_PRESCALE_DIV_8);
	TPM0_BASE_PTR->SC = TPM_SC_CMOD(TPM_INCREMENT_EVERY_CLK) | TPM_SC_PS(TPM_PRESCALE_DIV_1);

	TPM0_BASE_PTR->SC &= ~(TPM_SC_CPWMS_MASK); //Set CPWMS to up counting

	// Set overflow value to max
	TPM0_BASE_PTR->MOD = UINT16_MAX;


	//Set to Edge-aligned PWM, high true pulses (clear output on match, set output on reload)
	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_BLU].CnSC = TPM_CnSC_ELSB(TPM_ENABLED) |
													TPM_CnSC_ELSA(TPM_DISABLED) |
													TPM_CnSC_MSB(TPM_ENABLED) |
													TPM_CnSC_MSA(TPM_DISABLED);

	//Red and Green LEDs are off to start
	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_BLU].CnV = START_VALUE_BLUE;



	//Set refresh rate...
	//TODO: give this a better value.
	//Set for output compare, will use this as an update clock for cycling through the colors
	//Ensure the interrupt for this channel is active.


	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_UPD].CnSC = TPM_CnSC_ELSB(TPM_ENABLED) |
													TPM_CnSC_ELSA(TPM_DISABLED) |
													TPM_CnSC_MSB(TPM_ENABLED) |
													TPM_CnSC_MSA(TPM_DISABLED) | TPM_CnSC_CHIE_MASK;

	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_UPD].CnV = 0x1; //TODO: don't hardcode...

	// Profiling timer - 100 KHz (10 usec)

	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_PRF].CnSC = TPM_CnSC_ELSB(TPM_ENABLED) |
														TPM_CnSC_ELSA(TPM_DISABLED) |
														TPM_CnSC_MSB(TPM_ENABLED) |
														TPM_CnSC_MSA(TPM_DISABLED) | TPM_CnSC_CHIE_MASK;

	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_PRF].CnV = TPM_100KHZ_CMP; //TODO: don't hardcode...

}

/*
void timer_init()
{
	MCG_BASE_PTR->C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK;
	MCG_BASE_PTR->C2 = MCG_C2_IRCS_MASK; //fast internal clock

	SIK_BASE_PTR->SCGC6 |= SIM_SCGC6_TPM2_MASK; //enable TPM2 clock
	SIK_BASE_PTR->SOPT2 |= SIM_SOPT2_TPMSRC(3); //Set clock to ???

	//enable timer and set divider
	TPM2_BASE_PTR->SC = TPM_SC_CHOD(1) | TPM_SC_PS(3);
	//set the overflow value to the max
	TPM2_BASE_PTR->NOD = 0x7FFF;

	//enable ???
	TPM2_BASE_PTR->SC |= TPM_SC_????_MASK;
	NVIC->ISER[0] = 1 << 19;
}
*/
