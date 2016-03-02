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
#include <stdio.h>
#include "fsl_device_registers.h"
#include "timer.h"
#include "ports.h"

//Enable this definition to use standard library functions instead of custom
//You will also need to disable -fno-builtin options under Project->Properties->C/C++ Build->
//                          Settings -> Cross ARM C Compiler->Miscellaneous

//Instead of using the undef, you can, remove the macro definition in File->Properties->
// 	Preprocessor Include Paths-> (in middle of window) CDT User Setting Entries->PRJ_MEM_FUNC=1

#include <string.h>

#undef PRJ_MEM_FUNC
/*
#ifdef PRJ_MEM_FUNC
	#include "memory.h"
#else
	#include <string.h>
#endif
*/

// Memory test definitions
#define ALPHABET_MOD    25
#define ASCII_OFFSET    65
#define MAX_BUF_SIZE 	5000
#define TEST_SIZE_10 	10
#define TEST_SIZE_100	100
#define TEST_SIZE_1000	1000
#define TEST_SIZE_5000	5000
#define MEMSET_VALUE            0
// Array accessors for test cases
#define NUM_TEST_CASES          4
#ifdef PRJ_MEM_FUNC
    #define NUM_TEST_FUNCTIONS  3
#else
    #define NUM_TEST_FUNCTIONS  4
#endif
#define TEST_CASE_10            0
#define TEST_CASE_100           1
#define TEST_CASE_1000          2
#define TEST_CASE_5000          3
#define TEST_FUNC_MEMMOVE       0
#define TEST_FUNC_MEMZERO       1
#define TEST_FUNC_REVERSE       2
#define TEST_FUNC_MEMSET        1
#define TEST_FUNC_STRCPY        2
#define TEST_FUNC_STRNCPY       3


// RGB transition definitions
#define MAX 100
#define DIR_UP 1
#define DIR_DOWN -1

#define PI  3.14159265
#define STEPS 1000




uint32_t pos;
static uint16_t table[STEPS];
static uint16_t offsetBlue = STEPS / 3;
static uint16_t offsetGreen = STEPS * 2 / 3;

volatile uint32_t timer_count = 0;

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
/*
		TPM2_BASE_PTR->CONTROLS[TPM_CTRL_CH_RED].CnV = table[pos];
		TPM2_BASE_PTR->CONTROLS[TPM_CTRL_CH_GRN].CnV = table[green];
		TPM0_BASE_PTR->CONTROLS[TPM_CTRL_CH_BLU].CnV = table[blue];
*/
		pos++;
		if (pos >= STEPS)
		{
			pos = 0;
		}
	}

	NVIC_ClearPendingIRQ(TPM0_IRQn);
	__enable_irq();
}

uint32_t test_memmove(uint8_t *src, uint8_t *dst, uint32_t len)
{
    uint32_t clk_start;
    uint32_t clk_end;

    clk_start = timer_count;
    memmove(src, dst, len);
    clk_end = timer_count;

    //Account for 32 bit wraparound
    if (clk_end < clk_start)
    {
        return (UINT32_MAX - clk_start + clk_end);
    }

    return clk_end - clk_start;
}

#if 0
uint32_t test_memzero(uint8_t *src, uint32_t len)
{
    uint32_t clk_start;
    uint32_t clk_end;

    clk_start = timer_count;
    memzero(src, len);
    clk_end = timer_count;

    //Account for 32 bit wraparound
    if (clk_end < clk_start)
    {
        return (UINT32_MAX - clk_start + clk_end);
    }

    return clk_end - clk_start;
}

uint32_t test_reverse(uint8_t *src, uint32_t len)
{
    uint32_t clk_start;
    uint32_t clk_end;

    clk_start = timer_count;
    reverse(src, len);
    clk_end = timer_count;

    //Account for 32 bit wraparound
    if (clk_end < clk_start)
    {
        return (UINT32_MAX - clk_start + clk_end);
    }

    return clk_end - clk_start;
}
#endif

#if 1
uint32_t test_memset(uint8_t *src, uint32_t len)
{
    uint32_t clk_start;
    uint32_t clk_end;

    clk_start = timer_count;
    memset(src, MEMSET_VALUE, len);
    clk_end = timer_count;

    //Account for 32 bit wraparound
    if (clk_end < clk_start)
    {
        return (UINT32_MAX - clk_start + clk_end);
    }

    return clk_end - clk_start;
}

uint32_t test_strcpy(uint8_t *src, uint8_t *dst)
{
    uint32_t clk_start;
    uint32_t clk_end;

    clk_start = timer_count;
    strcpy((char *)dst, (char *)src);
    clk_end = timer_count;

    //Account for 32 bit wraparound
    if (clk_end < clk_start)
    {
        return (UINT32_MAX - clk_start + clk_end);
    }

    return clk_end - clk_start;
}

uint32_t test_strncpy(uint8_t *src, uint8_t *dst, size_t len)
{
    uint32_t clk_start;
    uint32_t clk_end;

    clk_start = timer_count;
    strncpy((char *)dst, (char *)src, len);
    clk_end = timer_count;

    //Account for 32 bit wraparound
    if (clk_end < clk_start)
    {
        return (UINT32_MAX - clk_start + clk_end);
    }

    return clk_end - clk_start;
}
#endif

void memory_test()
{
	uint8_t buffer_src[MAX_BUF_SIZE];
	uint8_t buffer_dst[MAX_BUF_SIZE];

	uint32_t i;

	uint32_t results[NUM_TEST_FUNCTIONS][NUM_TEST_CASES];

	//Initialize the array with junk data
	for (i=0; i < MAX_BUF_SIZE; i++)
	{
		buffer_src[i] = i % 8 + 1;          //Arbitrary
		buffer_dst[i] = (i * 3) % 8 + 1;    //Arbitrary
	}

	// Test cases: memmove() is tested for both library and custom.
	//             Others are dependent on compilation options

    //Test memmove
    results[TEST_FUNC_MEMMOVE][TEST_CASE_10] = test_memmove(buffer_src, buffer_dst, TEST_SIZE_10);
    results[TEST_FUNC_MEMMOVE][TEST_CASE_100] = test_memmove(buffer_src, buffer_dst, TEST_SIZE_100);
    results[TEST_FUNC_MEMMOVE][TEST_CASE_1000] = test_memmove(buffer_src, buffer_dst, TEST_SIZE_1000);
    results[TEST_FUNC_MEMMOVE][TEST_CASE_5000] = test_memmove(buffer_src, buffer_dst, TEST_SIZE_5000);

#ifdef PRJ_MEM_FUNC
	//Test memzero
    results[TEST_FUNC_MEMZERO][TEST_CASE_10] = test_memzero(buffer_src, TEST_SIZE_10);
    results[TEST_FUNC_MEMZERO][TEST_CASE_100] = test_memzero(buffer_src, TEST_SIZE_100);
    results[TEST_FUNC_MEMZERO][TEST_CASE_1000] = test_memzero(buffer_src, TEST_SIZE_1000);
    results[TEST_FUNC_MEMZERO][TEST_CASE_5000] = test_memzero(buffer_src, TEST_SIZE_5000);

    //Test reverse
    results[TEST_FUNC_REVERSE][TEST_CASE_10] = test_reverse(buffer_src, TEST_SIZE_10);
    results[TEST_FUNC_REVERSE][TEST_CASE_100] = test_reverse(buffer_src, TEST_SIZE_100);
    results[TEST_FUNC_REVERSE][TEST_CASE_1000] = test_reverse(buffer_src, TEST_SIZE_1000);
    results[TEST_FUNC_REVERSE][TEST_CASE_5000] = test_reverse(buffer_src, TEST_SIZE_5000);
#else
    //Test memset
    results[TEST_FUNC_MEMSET][TEST_CASE_10] = test_memset(buffer_src, TEST_SIZE_10);
    results[TEST_FUNC_MEMSET][TEST_CASE_100] = test_memset(buffer_src, TEST_SIZE_100);
    results[TEST_FUNC_MEMSET][TEST_CASE_1000] = test_memset(buffer_src, TEST_SIZE_1000);
    results[TEST_FUNC_MEMSET][TEST_CASE_5000] = test_memset(buffer_src, TEST_SIZE_5000);

    //Test strcpy
    //strcpy will stop on a '\0' char. Ensure everything but the last character is non-null
    //Initialize the source buffer all non-null characters
    for (i=0; i < MAX_BUF_SIZE; i++)
    {
        buffer_src[i] = i % ALPHABET_MOD + ASCII_OFFSET; //Letters A - Z
    }

    //Set the last char to null, test, and restore to non-null after each test
    buffer_src[TEST_SIZE_10 -1] = '\0';
    results[TEST_FUNC_STRCPY][TEST_CASE_10] = test_strcpy(buffer_src, buffer_dst);
    buffer_src[TEST_SIZE_10 -1] = ASCII_OFFSET;

    buffer_src[TEST_SIZE_100 -1] = '\0';
    results[TEST_FUNC_STRCPY][TEST_CASE_100] = test_strcpy(buffer_src, buffer_dst);
    buffer_src[TEST_SIZE_100 -1] = ASCII_OFFSET;

    buffer_src[TEST_SIZE_1000 -1] = '\0';
    results[TEST_FUNC_STRCPY][TEST_CASE_1000] = test_strcpy(buffer_src, buffer_dst);
    buffer_src[TEST_SIZE_1000 -1] = ASCII_OFFSET;

    buffer_src[TEST_SIZE_5000 -1] = '\0';
    results[TEST_FUNC_STRCPY][TEST_CASE_5000] = test_strcpy(buffer_src, buffer_dst);
    buffer_src[TEST_SIZE_5000 -1] = ASCII_OFFSET;

    //Test strncpy
    results[TEST_FUNC_STRNCPY][TEST_CASE_10] = test_strncpy(buffer_src, buffer_dst, TEST_SIZE_10);
    results[TEST_FUNC_STRNCPY][TEST_CASE_100] = test_strncpy(buffer_src, buffer_dst, TEST_SIZE_100);
    results[TEST_FUNC_STRNCPY][TEST_CASE_1000] = test_strncpy(buffer_src, buffer_dst, TEST_SIZE_1000);
    results[TEST_FUNC_STRNCPY][TEST_CASE_5000] = test_strncpy(buffer_src, buffer_dst, TEST_SIZE_5000);

#endif

    //Add a breakpoint here and view the contents of the array to get times
    //In theory, we could printf the results, but we haven't initialized the UART. So nothing comes out.

}


int main(void)
{
    sys_clock_48MHz();
    SystemCoreClockUpdate(); //Can add expression for global variable SystemCoreClock to see core clk freq
	calculate_table();
	//main_clk_init();
	timer0_init();
	//timer2_init();
	portB_setup();
	portD_setup();
	enable_timer0_interrupts();

	memory_test();

	printf("this is a test\n");
    while(1)
    {
    }

    return 0;
}
