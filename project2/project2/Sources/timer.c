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

void enable_timer0_interrupts()
{

	//IRQ definitions in MKL25Z4.h
	//This function comes from core_cm0plus.h
	NVIC_EnableIRQ(TPM0_IRQn);
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
    while (MCG->S & MCG_S_IREFST_MASK); //Loop until IREFST set to 0, indicating external reference is current source for ref clock
    while ( ((MCG->S & MCG_S_CLKST_MASK ) >> MCG_S_CLKST_SHIFT) != 0x2); //Wait until CLKST = 0x2- external ref clock is selected to feed MCGOUTCLK

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




/*
 * Using system init() for now...
 *
	//Select MCG for internal reference clock and enable
	//MCG_BASE_PTR->C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK; //TODO: Don't use this...
    MCG_BASE_PTR->C1 |= MCG_C1_IRCLKEN(MCG_C1_IRCLKEN_MASK);

	//Select fast internal reference clock - 4 MHz
	MCG_BASE_PTR->C2 |= MCG_C2_IRCS(MCG_C2_IRCS_MASK);

	//Set Fast clock internal reference divider to divide by 1 (4MHz freq). This is option 0x00.
	MCG_BASE_PTR->SC &= MCG_SC_FCRDIV(~(MCG_SC_FCRDIV_MASK));

	//Set TPM to use MCGIRCLK
	SIM_BASE_PTR->SOPT2 |= SIM_SOPT2_TPMSRC(SIM_TPM_SRC_MCGIRCLK);
*/
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
	//TPM2_BASE_PTR->SC = TPM_SC_CMOD(TPM_INCREMENT_EVERY_CLK) | TPM_SC_PS(TPM_PRESCALE_DIV_1);
    TPM2_BASE_PTR->SC = TPM_SC_CMOD(TPM_INCREMENT_EVERY_CLK) | TPM_SC_PS(TPM_PRESCALE_DIV_1);

	TPM2_BASE_PTR->SC &= ~(TPM_SC_CPWMS(TPM_SC_CPWMS_MASK)); //Set CPWMS to up counting

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

    MCG->C2 |= MCG_C2_IRCS_MASK;//Fast internal reference clock (4MHz)



    //test code...
    MCG->C2 |= MCG_C2_IRCS_MASK; //fast internal ref clock

      SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;//TPM0 clock activé
      SIM->SOPT2 |= SIM_SOPT2_TPMSRC(0x03);//TPM clock source -> MCGIRCLK = 4MHz
      TPM0->SC = TPM_SC_CMOD(0x00);//LPTPM desactivé
      TPM0->SC &= ~TPM_SC_PS(0x00);//Prescaler /4 => 4MHz/4 = 1MHz => T = 1µs
      //TPM0->SC &= !TPM_SC_CPWMS_MASK;//Compte de zéro à MOD reg
     TPM0->MOD = TPM_MOD_MOD(4000);//1000*1µs = 1ms
      TPM0->SC |= TPM_SC_TOF_MASK;//Reset TOF bit

      TPM0->SC |= TPM_SC_CMOD(0x01);//LPTPM activé compteur interne

	// Increment timer on every clock and prescale the clock by 1
	//TPM2_BASE_PTR->SC = TPM_SC_CMOD(TPM_INCREMENT_EVERY_CLK) | TPM_SC_PS(TPM_PRESCALE_DIV_8);
	//TPM0_BASE_PTR->SC = TPM_SC_CMOD(TPM_INCREMENT_EVERY_CLK) | TPM_SC_PS(TPM_PRESCALE_DIV_1);
    //TPM0_BASE_PTR->SC =  TPM_SC_PS(TPM_PRESCALE_DIV_1);
	//TPM0_BASE_PTR->SC &= ~(TPM_SC_CPWMS(TPM_SC_CPWMS_MASK)); //Set CPWMS to up counting

	// Set overflow value to max
	//TPM0_BASE_PTR->MOD = UINT16_MAX;
    //TPM0_BASE_PTR->MOD = UINT16_MAX - 1;


	//Set to Edge-aligned PWM, high true pulses (clear output on match, set output on reload)
	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_BLU].CnSC = TPM_CnSC_ELSB(TPM_ENABLED) |
													TPM_CnSC_ELSA(TPM_DISABLED) |
													TPM_CnSC_MSB(TPM_ENABLED) |
													TPM_CnSC_MSA(TPM_DISABLED);

	//Blue LED is off to start
	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_BLU].CnV = START_VALUE_BLUE;



	//Set refresh rate...
	//TODO: give this a better value.
	//Set for output compare, will use this as an update clock for cycling through the colors
	//Ensure the interrupt for this channel is active.


	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_UPD].CnSC = TPM_CnSC_ELSB(TPM_ENABLED) |
													TPM_CnSC_ELSA(TPM_DISABLED) |
													TPM_CnSC_MSB(TPM_ENABLED) |
													TPM_CnSC_MSA(TPM_DISABLED) | TPM_CnSC_CHIE(TPM_ENABLED);

	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_UPD].CnV = 4000; //TODO: don't hardcode...

	// Profiling timer - 100 KHz (10 usec)

	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_PRF].CnSC = TPM_CnSC_ELSB(TPM_ENABLED) |
														TPM_CnSC_ELSA(TPM_DISABLED) |
														TPM_CnSC_MSB(TPM_ENABLED) |
														TPM_CnSC_MSA(TPM_DISABLED) | TPM_CnSC_CHIE(TPM_ENABLED);

	//TODO: must be based on -DTIMER_INTERVAL
	//		the 100KHz timer should be = 10
	//TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_PRF].CnV = TPM_100KHZ_CMP;
	TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_PRF].CnV = 40;


}
