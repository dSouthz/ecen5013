#include <stdio.h>
#include <stdint.h>
#include "memory.h"


int8_t memmove(uint8_t *src, uint8_t *dst, uint32_t length)
{
    uint32_t i;

    /* Check error conditions */

    if (src == NULL)
    {
        return 1;
    }
    else if (dst == NULL)
    {
        return 2;
    }

    if (src > dst)
    {
        /*  If the source is greater than the destination, always do a left to
            right move. This prevents us from overwriting the source if the
            two regions overlap
        */
        for (i = 0; i < length; i++)
        {
            dst[i] = src[i];
        }
    }
    else
    {
        /*  The source occurs before the dest. Move memory from end of src to
            beginning. This keeps us from overwriting the source.
        */
        i = length;
        while (i != 0)
        {
            dst[i-1] = src[i-1];
            i--;
        }
    }


    return 0;
}


int8_t memcpy(uint8_t *src, uint8_t *dst, uint32_t length)
{
    uint32_t i;

    /* Check error conditions */

    if (src == NULL)
    {
        return 1;
    }
    else if (dst == NULL)
    {
        return 2;
    }
    else if ((src + length) > dst && (src + length) < (dst + length))
    {
        return 3;
    }
    else if ((dst + length) > src && (dst + length) < (src + length))
    {
        return 4;
    }

    /* Copy memory */

    for (i = 0; i < length; i++)
    {
        dst[i] = src[i];
    }

    return 0;
}


int8_t memzero(uint8_t *src, uint32_t length)
{
    uint32_t i;

    /* Check error conditions */

    if (src == NULL)
    {
        return 1;
    }

    /* Perform zero operation */

    for (i=0; i < length; i++)
    {
        src[i] = 0x00;
    }

    return 0;
}


int8_t reverse(uint8_t *src, uint32_t length)
{
    uint32_t i;

    if (src == NULL)
    {
        return 1;
    }

    if (length == 0)
    {
        return 0;
    }
    /*  We will use an in-place xor algorithm. This will avoid having to
        do any division instructions to calculate when to stop
     */

    length--; /* Set to 0-based length */

    for (i = 0; i < length; i++, length--)
    {
        src[i] ^= src[length];
        src[length] ^= src[i];
        src[i] ^= src[length];
    }

    return 0;
}

