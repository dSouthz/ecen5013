/*
 * timer.h
 *
 *  Created on: Feb 27, 2016
 *      Author: Sean
 */

#ifndef SOURCES_TIMER_H_
#define SOURCES_TIMER_H_

#define SIM_TPM_SRC_MCGIRCLK 		0x3u
#define TPM_ENABLED 				0x1u
#define TPM_DISABLED 				0x0u
#define TPM_INCREMENT_EVERY_CLK		0x1u
#define TPM_MOD_LEDS                0xFFFE
#define TPM_MOD_COUNTER             0x28

#define TPM_PRESCALE_DIV_1			0x0u
#define TPM_PRESCALE_DIV_2			0x1u
#define TPM_PRESCALE_DIV_4			0x2u
#define TPM_PRESCALE_DIV_8			0x3u

#define TPM_CTRL_CH_RED				0x0u
#define TPM_CTRL_CH_GRN				0x1u
#define TPM_CTRL_CH_BLU				0x1u
#define TPM_CTRL_CH_UPD				0x2u   // LED update channel
#define TPM_CTRL_CH_PRF				0x1u   // 100KHz profiler

//Start values for each LED
//Red starts in an "on" state as a debugging aid.
#define START_VALUE_RED				0x5555u
#define START_VALUE_BLUE			0xFFFFu
#define START_VALUE_GREEN			0xFFFFu

#define TPM_100KHZ_CMP				0x28u // 4 MHz / 100 kHz = 40 steps

extern volatile uint32_t profile_timer_count; // 100KHz timer values

void sys_clock_48MHz();
void main_clk_init();
void timer2_init();
void timer1_init();
void timer0_init();

void enable_timer_interrupts();

#endif /* SOURCES_TIMER_H_ */
