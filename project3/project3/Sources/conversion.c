#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static const double PRECISION = 0.00000000000001;

/**
* Double to ASCII
*
* Taken from StackOverflow:
* http://stackoverflow.com/questions/2302969/how-to-implement-char-ftoafloat-num-without-sprintf-library-function-i
*/
char *dtoa(char *s, double n) {
	// handle special cases
	if (isnan(n)) {
		strcpy(s, "nan");
	}
	else if (isinf(n)) {
		strcpy(s, "inf");
	}
	else if (n == 0.0) {
		strcpy(s, "0");
	}
	else {
		int digit, m, m1;
		char *c = s;
		int neg = (n < 0);
		if (neg)
			n = -n;
		// calculate magnitude
		m = log10(n);
		int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
		if (neg)
			*(c++) = '-';
		// set up for scientific notation
		if (useExp) {
			if (m < 0)
				m -= 1.0;
			n = n / pow(10.0, m);
			m1 = m;
			m = 0;
		}
		if (m < 1.0) {
			m = 0;
		}
		// convert the number
		while (n > PRECISION || m >= 0) {
			double weight = pow(10.0, m);
			if (weight > 0 && !isinf(weight)) {
				digit = floor(n / weight);
				n -= (digit * weight);
				*(c++) = '0' + digit;
			}
			if (m == 0 && n > 0)
				*(c++) = '.';
			m--;
		}
		if (useExp) {
			// convert the exponent
			int i, j;
			*(c++) = 'e';
			if (m1 > 0) {
				*(c++) = '+';
			}
			else {
				*(c++) = '-';
				m1 = -m1;
			}
			m = 0;
			while (m1 > 0) {
				*(c++) = '0' + m1 % 10;
				m1 /= 10;
				m++;
			}
			c -= m;
			for (i = 0, j = m - 1; i<j; i++, j--) {
				// swap without temporary
				c[i] ^= c[j];
				c[j] ^= c[i];
				c[i] ^= c[j];
			}
			c += m;
		}
		*(c) = '\0';
	}
	return s;
}


/* itoa - convert integer to ascii */
//MAX_LEN is most digits that could be output for a 32 bit integer to string conversion
#define MAX_LEN 11

char *itoa(int num)
{
	static char str[MAX_LEN];
	char *ptr = &str[MAX_LEN - 2];
	int negative = 0;
	str[MAX_LEN -1 ] = '\0';

	if (num < 0)
	{
		negative = 1;
		num *= -1;
	}

	do
	{
		*ptr = '0' + num % 10;
		ptr--;
		num /= 10;
	} while (num != 0);

	if (negative)
	{
		*ptr = '-';
	}
	else
	{
		ptr++;
	}

	return ptr;
}

char *uitoa(unsigned int num)
{
	static char str[MAX_LEN];
	char *ptr = &str[MAX_LEN - 2];

	str[MAX_LEN -1 ] = '\0';

	do
	{
		*ptr = '0' + num % 10;
		ptr--;
		num /= 10;
	} while (num != 0);

	ptr++;

	return ptr;
}
