/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <math.h>
#include <stdint.h>
#include "fsl_device_registers.h"
#include "timer.h"
#include "ports.h"

#define MAX 100
#define DIR_UP 1
#define DIR_DOWN -1

#define PI  3.14159265

#define STEPS 1000

uint32_t pos;
static uint16_t table[STEPS];
static uint16_t offsetBlue = STEPS / 3;
static uint16_t offsetGreen = STEPS * 2 / 3;

uint32_t timer_count = 0;

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

extern void TPM0_IRQHandler()
{
	uint16_t blue;
	uint16_t green;

	__disable_irq();

	if (TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_PRF].CnSC & TPM_CnSC_CHF_MASK)
	{
		//Writing to the CHF bit clears the interrupt
		TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_PRF].CnSC |= TPM_CnSC_CHF_MASK;
		timer_count++;
	}
	else if (TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_UPD].CnSC & TPM_CnSC_CHF_MASK)
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
	__enable_irq();
}


int main(void)
{
	calculate_table();
	main_clk_init();
	timer0_init();
	timer2_init();
	portB_setup();
	portD_setup();

	enable_timer0_interrupts();
    while(1)
    {
    }

    return 0;
}
