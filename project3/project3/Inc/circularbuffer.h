
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

// Initializes a circular buffer
// Params:	buf 		- the buffer to initialize
//			itemSize 	- size of the items buffer contains
//			numItems	- number of items the buffer holds
//
// Returns:
//			BUFFER_OK on success
//			BUFFER_BAD on failure
CircBufStatus_t CircBuf_Init(CircBuf_t *buf, size_t itemSize, size_t numItems);

// Adds an item to the buffer
// Params:	buf  - the buffer where the item will be added
//			item - the item to add to the buffer. Item is copied into the buffer
//
// Returns:
//			BUFFER_OK on successful add
//			BUFFER_FULL on add when the buffer is full or when the buffer is filled up
//			BUFFER_BAD if the buf is a null pointer
CircBufStatus_t CircBuf_Add(CircBuf_t *buf, void *item);

// Removes an item from the buffer
// Params:	buf  - the buffer where the item will be removed
//
// Returns:
//			BUFFER_OK on successful remove
//			BUFFER_EMPTY when trying to remove from an empty buffer or when the buffer has become empty
//			BUFFER_BAD if the buf is a null pointer
CircBufStatus_t CircBuf_Remove(CircBuf_t *buf);

// Dumps the entire contents of the buffer to UART
// Buffer has one extra item than what it is initialized with. This is for pointer counting.
//
// Params: buf - the buffer to dump
void CircBuf_Dump(CircBuf_t *buf);

#endif /* SOURCES_CIRCULARBUFFER_H_ */
