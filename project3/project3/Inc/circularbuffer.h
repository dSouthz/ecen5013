
#ifndef SOURCES_CIRCULARBUFFER_H_
#define SOURCES_CIRCULARBUFFER_H_

/* Circular buffer */
typedef struct CircBuf
{
	void *buffer;
	void *head;
	void *tail;
	size_t size;
	size_t item_size;
	size_t num_items;
} CircBuf_t;

typedef enum CircBufStat
{
	BUFFER_OK,
	BUFFER_BAD,
	BUFFER_FULL,
	BUFFER_EMPTY
} CircBufStatus_t;

CircBufStatus_t CircBuf_Init(CircBuf_t *buf, size_t itemSize, size_t numItems);
CircBufStatus_t CircBuf_Add(CircBuf_t *buf, void *item);
CircBufStatus_t CircBuf_Remove(CircBuf_t *buf);
void CircBuf_Dump(CircBuf_t *buf);

#endif /* SOURCES_CIRCULARBUFFER_H_ */
