/*
 * conversion.h
 *
 *  Created on: Mar 30, 2016
 *      Author: Sean
 */

#ifndef SOURCES_CONVERSION_H_
#define SOURCES_CONVERSION_H_

// Converts a floating point (double) to string
// Params:
//			s - the output string
//			n - the floating point number
//
// Returns:
//			A pointer to the string or NULL on failure
char *dtoa(char *s, double n);

// Converts a signed integer to a string
// Params:
//			num - the integer to convert
//
// Returns:
//			A pointer to the string or NULL on failure
char *itoa(int num);

// Converts an unsigned integer to a string
// Params:
//			num - the integer to convert
//
// Returns:
//			A pointer to the string or NULL on failure
char *uitoa(unsigned int num);

#endif /* SOURCES_CONVERSION_H_ */
