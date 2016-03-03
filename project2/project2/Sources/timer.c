/*
 * timer.c
 *
 *  Created on: Feb 27, 2016
 *      Author: Sean
 */
#include <stdint.h>
#include "MKL25Z4.h"
#include "timer.h"
#include "ports.h"

void enable_timer_interrupts()
{

	//IRQ definitions in MKL25Z4.h
	//This function comes from core_cm0plus.h
	NVIC_EnableIRQ(TPM0_IRQn);
    NVIC_EnableIRQ(TPM1_IRQn);
}

void sys_clock_48MHz()
{
    SystemInit();
    return;

    //Was trying to do this on own. Just use SystemInit(). It's better.

    /* TODO: get rid of this.
    //Set PTA18 (EXTAL0) to ALT0 (should also be default
    //PORTA_PCR18 = PORT_PCR_MUX(PORT_MUX_ALT0);
    //Set PTA19 (XTAL0) to ALT0 (XTAL0, also should be default)
    //PORTA_PCR19 = PORT_PCR_MUX(PORT_MUX_ALT0);

    OSC0->CR |= OSC_CR_ERCLKEN(1); //external clock enabled... even if using EXTAL instead of XTAL, both are 8 MHz


    SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x0) | //Divide by 1 for system, core clocks
                    SIM_CLKDIV1_OUTDIV4(0x1); //Divide by 2 for a 24 MHz bus

    //Page 395 of manual details how to transition to phase locked loop
    //Note: have to modify, since they use a 4 MHz clock and we have an 8 MHz clock
    //Step 1 FEI -> FBE mode
    MCG->C2 =   MCG_C2_RANGE0(0x1)  | //High freq
                MCG_C2_HGO0(0x1)    | //High gain
                MCG_C2_EREFS0(0x1);   // Using a crytal

    MCG->C1 =   MCG_C1_CLKS(0x2)    | //using external referece system clock
                MCG_C1_FRDIV(0x3)   | //Divide by 256 - gives us 31.25 KHz reference for FLL
                MCG_C1_IREFS(0x0);    //Clear, sets external ref clock and enables external oscillator

    while (!(MCG->S & MCG_S_OSCINIT0_MASK)); //Loop until OSCINT0 set to 1, meaning crystal has been initalized

    //Loop until IREFST set to 0, indicating external reference is current source for ref clock
    while (MCG->S & MCG_S_IREFST_MASK);

    //Wait until CLKST = 0x2- external ref clock is selected to feed MCGOUTCLK
    while ( ((MCG->S & MCG_S_CLKST_MASK ) >> MCG_S_CLKST_SHIFT) != 0x2);

    //Step 2, generate PLL ref freq
    MCG->C5 =   MCG_C5_PRDIV0(0x3); //Divide by 4, makes 8 MHz ref 2 MHz

    //Step 3, transitioning only to PBE (not BLPE, low power mode)
    MCG->C6 =   MCG_C6_PLLS(0x1)    | //Select the PLL
                MCG_C6_VDIV0(0x0);    //Select muliply by 24. 2 MHz * 24 = 48 MHz

    while (!(MCG->S & MCG_S_LOCK0_MASK)); //Loop until LOCK0 set, indicating PLL has acquired lock

    //Step 4, PBE -> PEE mode
    MCG->C1 = MCG_C1_CLKS(0x0); //Using PLL as system clock

    //Loop until CLKST is 0x3, indicating the PLL output is selected to feed MCGOUTCLK in the current mode.
    while (((MCG->S & MCG_S_CLKST_MASK ) >> MCG_S_CLKST_SHIFT) != 0x3);
    */

    //Clock should now be 48 MHz
}


void main_clk_init()
{
    MCG->C2     |= MCG_C2_IRCS_MASK;                        //Fast internal ref clock = 4 MHz
    SIM_SCGC6   |= SIM_SOPT2_TPMSRC(SIM_TPM_SRC_MCGIRCLK);  //TPM clock source -> MCGIRCLK = 4MHz
    //Send clock to TPM0-2
	SIM_SCGC6   |= SIM_SCGC6_TPM0(TPM_ENABLED) | SIM_SCGC6_TPM1(TPM_ENABLED) | SIM_SCGC6_TPM2(TPM_ENABLED);
}

void timer2_init()
{
    TPM2_BASE_PTR->SC = TPM_SC_CMOD(TPM_DISABLED);             //Disable TPM
    TPM2_BASE_PTR->SC &= ~TPM_SC_PS(TPM_PRESCALE_DIV_1);       //Div by 1 = 4 MHz
    TPM2_BASE_PTR->MOD = TPM_MOD_MOD(TPM_MOD_LEDS );           //Set PWM max freq
    TPM2_BASE_PTR->SC |= TPM_SC_TOF_MASK;                      //Reset TOF bit
    TPM2_BASE_PTR->SC |= TPM_SC_CMOD(TPM_INCREMENT_EVERY_CLK); //Active TPM

	//Set to Edge-aligned PWM, high true pulses (clear output on match, set output on reload)
	TPM2_BASE_PTR->CONTROLS[TPM_CTRL_CH_RED].CnSC = TPM_CnSC_ELSB(TPM_ENABLED) |
													TPM_CnSC_ELSA(TPM_DISABLED) |
													TPM_CnSC_MSB(TPM_ENABLED) |
													TPM_CnSC_MSA(TPM_DISABLED);

	TPM2_BASE_PTR->CONTROLS[TPM_CTRL_CH_GRN].CnSC = TPM_CnSC_ELSB(TPM_ENABLED) |
													TPM_CnSC_ELSA(TPM_DISABLED) |
													TPM_CnSC_MSB(TPM_ENABLED) |
													TPM_CnSC_MSA(TPM_DISABLED);


	//Set red and green LEDs default intensity
	TPM2_BASE_PTR->CONTROLS[TPM_CTRL_CH_RED].CnV = START_VALUE_RED;
	TPM2_BASE_PTR->CONTROLS[TPM_CTRL_CH_GRN].CnV = START_VALUE_GREEN;
}

#ifdef TIMER_INTERVAL
#undef TPM_MOD_COUNTER
#define TPM_MOD_COUNTER (4 * TIMER_INTERVAL)
#endif
//TODO: base this mod on value passed in from compile line
void timer1_init()
{
    TPM1_BASE_PTR->SC = TPM_SC_CMOD(TPM_DISABLED);             //Disable TPM
    TPM1_BASE_PTR->SC &= ~TPM_SC_PS(TPM_PRESCALE_DIV_1);       //Div by 1 = 4 MHz
    TPM1_BASE_PTR->MOD = TPM_MOD_MOD(TPM_MOD_COUNTER);         //Set PWM max freq
    TPM1_BASE_PTR->SC |= TPM_SC_TOF_MASK;                      //Reset TOF bit
    TPM1_BASE_PTR->SC |= TPM_SC_CMOD(TPM_INCREMENT_EVERY_CLK); //Active TPM

    TPM1_BASE_PTR->CONTROLS[TPM_CTRL_CH_PRF].CnSC = TPM_CnSC_ELSB(TPM_ENABLED) |
                                                    TPM_CnSC_ELSA(TPM_DISABLED) |
                                                    TPM_CnSC_MSB(TPM_ENABLED)   |
                                                    TPM_CnSC_MSA(TPM_DISABLED)  |
                                                    TPM_CnSC_CHIE(TPM_ENABLED);

    //TODO: must be based on -DTIMER_INTERVAL
    //      the 100KHz timer should be = 40
    TPM1_BASE_PTR->CONTROLS[TPM_CTRL_CH_PRF].CnV =  TPM_PRESCALE_DIV_1;

}


void timer0_init()
{
    TPM0_BASE_PTR->SC = TPM_SC_CMOD(TPM_DISABLED);             //Disable TPM
    TPM0_BASE_PTR->SC &= ~TPM_SC_PS(TPM_PRESCALE_DIV_1);       //Div by 1 = 4 MHz
    TPM0_BASE_PTR->MOD = TPM_MOD_MOD(TPM_MOD_LEDS );           //Set PWM max freq
    TPM0_BASE_PTR->SC |= TPM_SC_TOF_MASK;                      //Reset TOF bit
    TPM0_BASE_PTR->SC |= TPM_SC_CMOD(TPM_INCREMENT_EVERY_CLK); //Active TPM


	//Set to Edge-aligned PWM, high true pulses (clear output on match, set output on reload)
	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_BLU].CnSC = TPM_CnSC_ELSB(TPM_ENABLED)  |
													TPM_CnSC_ELSA(TPM_DISABLED) |
													TPM_CnSC_MSB(TPM_ENABLED)   |
													TPM_CnSC_MSA(TPM_DISABLED);

	//Blue LED is off to start
	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_BLU].CnV = START_VALUE_BLUE;



	//Set refresh rate - LED values change based on this channel
	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_UPD].CnSC = TPM_CnSC_ELSB(TPM_ENABLED)  |
													TPM_CnSC_ELSA(TPM_DISABLED) |
													TPM_CnSC_MSB(TPM_ENABLED)   |
													TPM_CnSC_MSA(TPM_DISABLED)  |
													TPM_CnSC_CHIE(TPM_ENABLED);

	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_UPD].CnV = TPM_MOD_LEDS;

}
