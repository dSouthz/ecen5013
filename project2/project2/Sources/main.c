/*
 * Main entry point of Project 2
 *
 * Note: Please see test_cases.h for instructions on enabling / using
 *       the standard library memmove (etc) functions
 */

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "fsl_device_registers.h"
#include "timer.h"
#include "ports.h"
#include "test_cases.h"
#ifdef PRJ_DMA_FUNC
    #include "dma_memory.h"
#endif

#define PRJ_DMA_FUNC //TODO: make this a project wide define

// RGB transition definitions
#define MAX                     100
#define DIR_UP                  1
#define DIR_DOWN                -1

#define PI                      3.14159265
#define STEPS                   1000
#define OFFSET_BLUE             (STEPS / 3)
#define OFFSET_GREEN            (STEPS * 2 / 3)

static uint16_t table[STEPS];                   //Table of sine wave values
static uint32_t pos;                            //Position in the RGB lookup table
static uint32_t offsetBlue = OFFSET_BLUE;       //Storing as a variable so we don't have to recalculate
static uint32_t offsetGreen = OFFSET_GREEN;     //Storing as a variable so we don't have to recalculate

volatile uint32_t profile_timer_count = 0; // 100KHz timer values

/* Calculates a lookup table of floating values for a sine wave
 *
 */
void calculate_table()
{
	uint32_t i;
	float twoPi = 2 * 3.14159265;
	float inc = twoPi / STEPS;
	float cur = 0;

	for (i=0; i < STEPS; i++)
	{
		float val = sin(cur);
		if (val < 0) { val *= -1; }
		val *= (float)0xFFFF;
		table[i] = val;
		cur += inc;
	}
}

extern void TPM1_IRQHandler()
{
    __disable_irq();

    if (TPM1_BASE_PTR->CONTROLS[TPM_CTRL_CH_PRF].CnSC & TPM_CnSC_CHF_MASK)
    {
        //Writing to the CHF bit clears the interrupt
        TPM1_BASE_PTR->CONTROLS[TPM_CTRL_CH_PRF].CnSC |= TPM_CnSC_CHF_MASK;
        profile_timer_count++;
    }

    NVIC_ClearPendingIRQ(TPM1_IRQn);
    __enable_irq();
}

extern void TPM0_IRQHandler()
{
	uint16_t blue;
	uint16_t green;

	__disable_irq();

	if (TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_UPD].CnSC & TPM_CnSC_CHF_MASK)
	{
		//Got an interrupt from the update channel. Acknowledge and move the count

		//Writing to the CHF bit clears the interrupt
		TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_UPD].CnSC |= TPM_CnSC_CHF_MASK;
		green = pos + offsetGreen;
		blue = pos + offsetBlue;
		if (blue >= STEPS)
		{
			blue -= STEPS;
		}
		if (green >= STEPS)
		{
			green -= STEPS;
		}

		TPM2_BASE_PTR->CONTROLS[TPM_CTRL_CH_RED].CnV = table[pos];
		TPM2_BASE_PTR->CONTROLS[TPM_CTRL_CH_GRN].CnV = table[green];
		TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_BLU].CnV = table[blue];

		pos++;
		if (pos >= STEPS)
		{
			pos = 0;
		}
	}

	NVIC_ClearPendingIRQ(TPM0_IRQn);
	__enable_irq();
}


int main(void)
{
    sys_clock_48MHz();          //Sets core clock to 48 MHz
    SystemCoreClockUpdate();    //Stores clock freq in global var SystemCoreClock. Used for debugging
	calculate_table();          //Calculates sine wave lookup table
	main_clk_init();            //Enable the MCG clock
	timer0_init();              //Enable PWM timer 0 - Blue LED
	timer1_init();              //Enable PWM timer 1 - 100 KHz profiling timer
	timer2_init();              //Enable PWM timer 2 - Red, Green LEDs
	portB_setup();              //Setup port B - red and green LEDs
	portD_setup();              //Setup port D - blue LED
#ifdef PRJ_DMA_FUNC
	dma0_configure_8bit();
#endif
	enable_timer_interrupts();  //Enable interrupts for timers 0, 1

	test_memory();
	test_printf();

	printf("this is a test\n");
    while(1)
    {
    }

    return 0;
}
