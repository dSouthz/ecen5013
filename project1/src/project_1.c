#include <stdint.h>
#include <stdio.h>
#include "project_1.h"
#include "memory.h"

#define ARRAY_SIZE     32
#define PTR_2_POS    7
#define PTR_3_POS    15

void project_1_report()
{
    uint8_t array[ARRAY_SIZE];
    uint8_t *aptr_1 = array;
    uint8_t *aptr_2 = array + PTR_2_POS;
    uint8_t *aptr_3 = array + PTR_3_POS;
    uint8_t i;

    for (i=0; i < 16; i++)
    {
        *(aptr_1 + i) = i + 1;
    }

    for (i = PTR_3_POS; i < ARRAY_SIZE; i++)
    {
        memzero(aptr_3, 17); //TODO: fix this up..
    }

    memcpy(aptr_1, aptr_3, 8);
    memmove(aptr_2, aptr_1, 16);
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

