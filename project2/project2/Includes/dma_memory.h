#ifndef INCLUDES_DMA_MEMORY_H_
#define INCLUDES_DMA_MEMORY_H_

typedef enum __dma_res
{
    DMA_SUCCESS         = 0,
    DMA_FAILURE         = 1,
    DMA_ERROR_CONFIG    = 2,
    DMA_ERROR_BUS_SRC   = 3,
    DMA_ERROR_BUS_DST   = 4
} DMA_Result_t;

void dma0_configure_8bit();
int8_t memmove(uint8_t *src, uint8_t *dst, uint32_t length);
int8_t memzero(uint8_t *src, uint32_t length);

#endif /* INCLUDES_DMA_MEMORY_H_ */
