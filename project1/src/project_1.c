#include <stdint.h>
#include <stdio.h>
#include "project_1.h"
#include "memory.h"

#define ARRAY_SIZE          32
#define PTR_2_POS           8
#define PTR_3_POS           16

#define INIT_AMOUNT         16
#define MEMCPY_AMOUNT       8
#define MEMMOVE_AMOUNT      16

#define BYTES_PER_LINE      8

void project_1_report()
{
    uint8_t array[ARRAY_SIZE];
    uint8_t *aptr_1 = array;
    uint8_t *aptr_2 = array + PTR_2_POS;
    uint8_t *aptr_3 = array + PTR_3_POS;
    uint8_t i;

    /* Project 1 part 2 task 2.a.iii.1 */
    for (i=0; i < INIT_AMOUNT; i++)
    {
        *(aptr_1 + i) = i + 1;
    }

    /* Project 1 part 2 task 2.a.iii.2 */
    memzero(aptr_3, ARRAY_SIZE - PTR_3_POS);

    /* Project 1 part 2 task 2.a.iv */
    memcpy(aptr_1, aptr_3, MEMCPY_AMOUNT);

    /* Project 1 part 2 task 2.a.v */
    memmove(aptr_2, aptr_1, MEMMOVE_AMOUNT);

    /* Project 1 part 2 task 2.a.vi */
    reverse(aptr_1, ARRAY_SIZE);

    /* Print results BYTES_PER_LINE bytes per line*/
    for (i=0; i < ARRAY_SIZE; i++)
    {
        if (i % BYTES_PER_LINE == 0 && i != 0)
        {
            printf("\n");
        }
        printf("%02x ", array[i]);
    }
    printf("\n");
}

