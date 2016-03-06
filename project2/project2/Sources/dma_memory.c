#include <stdint.h>
#include <stddef.h>
#include "MKL25Z4.h"
#include "dma_memory.h"

#define XFER_SIZE_8BIT      0x3
#define XFER_SIZE_16BIT     0x2
#define XFER_SIZE_32BIT     0x0

#define DMA_INCREMENT       0x1
#define DMA_REG             0x0

void dma0_configure_8bit()
{
    DMA0->DMA[DMA_REG].DCR = DMA_DCR_SSIZE(XFER_SIZE_8BIT)   |
                DMA_DCR_DSIZE(XFER_SIZE_8BIT);
}

void dma0_src_inc()
{
    DMA0->DMA[DMA_REG].DCR |= DMA_DCR_SINC(DMA_INCREMENT);
}

void dma0_clear_src_inc()
{
    DMA0->DMA[DMA_REG].DCR &= ~DMA_DCR_SINC_MASK;
}

void dma0_dst_inc()
{
    DMA0->DMA[DMA_REG].DCR |= DMA_DCR_DINC(DMA_INCREMENT);
}

void dma0_clear_dst_inc()
{
    DMA0->DMA[DMA_REG].DCR &= DMA_DCR_DINC_MASK;
}

//Sets up a DMA transfer
//Returns DMA_SUCCESS on success, non-zero on failure
DMA_Result_t dma0_xfer_setup(uint32_t *src, uint32_t *dst, uint32_t size)
{
    if (size > DMA_DSR_BCR_BCR_MASK )
    {
        return DMA_FAILURE;
    }

    DMA0->DMA[DMA_REG].SAR = (uint32_t)src; //Treating the pointer values as numbes
    DMA0->DMA[DMA_REG].DAR = (uint32_t)dst;
    DMA0->DMA[DMA_REG].DSR_BCR =  DMA_DSR_BCR_BCR_MASK & size;

    return DMA_SUCCESS;
}

void dma0_start_xfer()
{
    DMA0->DMA[DMA_REG].DCR |= DMA_DCR_START_MASK;
}

/* Blocks until the write is done
 * Reports DMA_SUCCESS on success, nonzero on failure
 */
DMA_Result_t dma0_wait_until_done()
{
    DMA_Result_t res = DMA_SUCCESS;

    // Busy waiting...
    // This could be done more efficiently by using the DMA done interrupt
    // or even a POSIX mutex, but we will keep it simple for now.
    while (!(DMA0->DMA[DMA_REG].DSR_BCR & DMA_DSR_BCR_DONE_MASK))
    {};

    // Once Xfer is "done", check to see if there was either
    // a setup error, a bus error, or a successful transfer
    // Multiple errors will be reported as the first encountered
    if (DMA0->DMA[DMA_REG].DSR_BCR & DMA_DSR_BCR_CE_MASK)
    {
        // Configuration error
        res = DMA_ERROR_CONFIG;
    }
    else if (DMA0->DMA[DMA_REG].DSR_BCR & DMA_DSR_BCR_BES_MASK)
    {
        // Source bus error
        res = DMA_ERROR_BUS_SRC;
    }
    else if (DMA0->DMA[DMA_REG].DSR_BCR & DMA_DSR_BCR_BED_MASK)
    {
        // Destination bus error
        res = DMA_ERROR_BUS_DST;
    }

    // Clear the "done" bit to clear errors and make DMA ready again
    DMA0->DMA[DMA_REG].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;

    return res;
}

#ifdef PRJ_DMA_FUNC
int8_t memmove(uint8_t *src, uint8_t *dst, uint32_t length)
{
    uint32_t i;

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
            right move. This can be done with DMA
        */
        dma0_src_inc();
        dma0_dst_inc();

        dma0_start_xfer();
        if (dma0_xfer_setup((uint32_t *)src, (uint32_t *)dst, length) != DMA_SUCCESS)
        {
            return 3;
        }

        switch (dma0_wait_until_done())
        {
            case DMA_ERROR_CONFIG:
                return 4;
            case DMA_ERROR_BUS_SRC:
                return 5;
            case DMA_ERROR_BUS_DST:
                return 6;
            case DMA_SUCCESS:
            default:
                return 0;
        }
    }
    else
    {
        /*  The source occurs before the dest. Move memory from end of src to
            beginning. This keeps us from overwriting the source.

            This cannot be done with DMA, since DMA can only increment
            automatically.
        */
        i = length;
        while (i != 0)
        {
            dst[i-1] = src[i-1];
            i--;
        }
    }
}

int8_t memzero(uint8_t *src, uint32_t length)
{
    uint32_t zero = 0x0;

    if (src == 0)
    {
        return 1;
    }

    dma0_clear_src_inc();
    dma0_dst_inc();
    dma0_start_xfer();

    if (dma0_xfer_setup(&zero, (uint32_t *)src, length) != DMA_SUCCESS)
    {
        return 2;
    }

    switch (dma0_wait_until_done())
    {
        case DMA_ERROR_CONFIG:
            return 3;
        case DMA_ERROR_BUS_SRC:
            return 4;
        case DMA_ERROR_BUS_DST:
            return 5;
        case DMA_SUCCESS:
        default:
            return 0;
    }
}
#endif
