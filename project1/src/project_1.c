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

void project_1_report()
{
    uint8_t array[ARRAY_SIZE];
    uint8_t *aptr_1 = array;
    uint8_t *aptr_2 = array + PTR_2_POS;
    uint8_t *aptr_3 = array + PTR_3_POS;
    uint8_t i;

    for (i=0; i < INIT_AMOUNT; i++)
    {
        *(aptr_1 + i) = i + 1;
    }

    memzero(aptr_3, (ARRAY_SIZE - 1) - PTR_3_POS);
    memcpy(aptr_1, aptr_3, MEMCPY_AMOUNT);
    memmove(aptr_2, aptr_1, MEMMOVE_AMOUNT);
    reverse(aptr_1, ARRAY_SIZE);

    for (i=0; i < ARRAY_SIZE; i++)
    {
        if (i % 8 == 0 && i != 0)
        {
            printf("\n");
        }
        printf("%02x ", array[i]);
    }
    printf("\n");
}

