#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "uart.h"
#include "unit_test.h"
#include "circularbuffer.h"

#define ITEM_SIZE 		6
#define BUFFER_SIZE 	5
#define TOTAL_TESTS		3

int Test_fill_and_empty()
{
		char items[ITEM_SIZE] = { 'a', 'b', 'c', 'd', 'e', 'f' };
		CircBuf_t buffer;

		CircBuf_Init(&buffer, sizeof(char), BUFFER_SIZE);

		//Fill buffer with 5 items
		CircBuf_Add(&buffer, &items[0]);
		CircBuf_Add(&buffer, &items[1]);
		CircBuf_Add(&buffer, &items[2]);
		CircBuf_Add(&buffer, &items[3]);
		CircBuf_Add(&buffer, &items[4]);

		//Attempt to fill one more
		if (CircBuf_Add(&buffer, &items[5]) != BUFFER_FULL)
		{
			return TEST_FAILURE;
		}

		//Remove the 5 items
		CircBuf_Remove(&buffer);
		CircBuf_Remove(&buffer);
		CircBuf_Remove(&buffer);
		CircBuf_Remove(&buffer);
		CircBuf_Remove(&buffer);
		if (CircBuf_Remove(&buffer) != BUFFER_EMPTY)
		{
			return TEST_FAILURE;
		}

		return TEST_SUCCESS;
}

int Test_boundary()
{
	char items[ITEM_SIZE] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	CircBuf_t buffer;
	int i;

	CircBuf_Init(&buffer, sizeof(char), BUFFER_SIZE);

	//Fill buffer with 5 items
	CircBuf_Add(&buffer, &items[0]);
	CircBuf_Add(&buffer, &items[1]);
	CircBuf_Add(&buffer, &items[2]);
	CircBuf_Add(&buffer, &items[3]);
	CircBuf_Add(&buffer, &items[4]);

	//Attempt to fill one more
	if (CircBuf_Add(&buffer, &items[5]) != BUFFER_FULL)
	{
		return TEST_FAILURE;
	}

	//Remove all of the items
	for (i=0; i < BUFFER_SIZE; i++)
	{
		CircBuf_Remove(&buffer);
	}

	//Attempt to remove one more
	if (CircBuf_Remove(&buffer) != BUFFER_EMPTY)
	{
		return TEST_FAILURE;
	}

	return TEST_SUCCESS;
}

int Test_pointer_length()
{
	char items[ITEM_SIZE] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	CircBuf_t buffer;

	CircBuf_Init(&buffer, sizeof(char), BUFFER_SIZE);

	CircBuf_Add(&buffer, &items[0]);

	if ((char *)buffer.tail != (char *)buffer.head + sizeof(char))
	{
		return TEST_FAILURE;
	}

	return TEST_SUCCESS;
}

void RunTestSuite()
{
	int pass = 0;
	int fail = 0;
	char result_msg_fail[] = "CIRCBUF UNIT TEST SUITE: FAIL. NUMBER OF FAILURES: ";
	char result_msg_pass[] = "CIRCBUF UNIT TEST SUITE: PASS";

	/*
	 * If the unit tests were expanded, it would be a good idea to use
	 * string buffers and snprintf to create the messages, which can
	 * then be sent via UART.
	 *
	 * However, since our tests are so simple, we are just hardcoding.
	 */

	if (Test_fill_and_empty() == TEST_SUCCESS)
	{
		char msg[] = "CB UNIT TEST: 1/3 - test_fill_and_empty() .... PASS";
		pass++;
		LOG_UART(msg, strlen(msg));
	}
	else
	{
		char msg[] = "CB UNIT TEST: 1/3 - test_fill_and_empty() .... FAIL";
		fail++;
		LOG_UART(msg, strlen(msg));
	}

	if (Test_boundary() == TEST_SUCCESS)
	{
		char msg[] = "CB UNIT TEST: 2/3 - test_boundary() .... PASS";
		pass++;
		LOG_UART(msg, strlen(msg));
	}
	else
	{
		char msg[] = "CB UNIT TEST: 2/3 - test_boundary() .... FAIL";
		fail++;
		LOG_UART(msg, strlen(msg));
	}

	if (Test_pointer_length() == TEST_SUCCESS)
	{
		char msg[] = "CB UNIT TEST: 3/3 - test_pointer_length() .... PASS";
		pass++;
		LOG_UART(msg, strlen(msg));
	}
	else
	{
		char msg[] = "CB UNIT TEST: 3/3 - test_pointer_length() .... FAIL";
		fail++;
		LOG_UART(msg, strlen(msg));
	}

	if (fail > 0)
	{
		LOG_UART_PARAM(result_msg_fail, strlen(result_msg_fail), &fail, INTEGER_32);
	}
	else
	{
		LOG_UART(result_msg_pass, strlen(result_msg_pass));
	}
}
