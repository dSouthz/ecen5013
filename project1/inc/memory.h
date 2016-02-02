#ifndef _MEM_H
#define _MEM_H

#include <stdint.h>

/*
 * Reverses a string based on length
 *
 * returns 0 on success, nonzero on failure
 */
char reverse_string(char *str, int length);

/*
 * Moves length memory from src to dst.
 *
 * src and dst may overlap
 *
 * returns 0 on success, nonzero on failure
 */
int8_t memmove(uint8_t *src, uint8_t *dst, uint32_t length);

/*
 * Copies length memory from src to dst.
 *
 * src and dest may not overlap. Use memmove for overlapping regions of memory
 *
 * returns 0 on success, nonzero on failure
 */
int8_t memcpy(uint8_t *src, uint8_t *dst, uint32_t length);

/*
 * Sets length bytes of src with the value 0x00
 *
 * returns 0 on succes, nonzero on failure
 */
int8_t memzero(uint8_t *src, uint32_t length);

/*
 * Reverses a region of memory
 *
 * returns 0 on success, nonzero on failure
 */
int8_t reverse(uint8_t *src, uint32_t length);

#endif // _MEM_H
