#include <string.h>
#include <stdlib.h>
#include "circularbuffer.h"
#include "uart.h"

CircBufStatus_t CircBuf_Init(CircBuf_t *buf, size_t itemSize, size_t numItems)
{
	if (buf == NULL)
	{
		return BUFFER_BAD;
	}

	buf->item_size = itemSize;
	buf->num_items = numItems + 1;
	buf->size = itemSize * buf->num_items;
	buf->buffer = malloc(buf->size);

	if (buf->buffer == NULL)
	{
		return BUFFER_BAD;
	}

	buf->head = buf->buffer;
	buf->tail = buf->buffer;

	return BUFFER_OK;
}

CircBufStatus_t CircBuf_Add(CircBuf_t *buf, void *item)
{
	if (buf == NULL || item == NULL)
	{
		return BUFFER_BAD;
	}

	if (buf->head == buf->tail)
	{
		//Empty buffer. Add and move.

		if ((void *)((char *)buf->tail + buf->item_size) > (void *)((char *)buf->buffer + buf->num_items))
		{
			buf->tail = buf->buffer;
			buf->head = buf->buffer;
		}

		memcpy(buf->tail, item, buf->item_size); //TODO: error checking
		if ((char *)buf->tail + buf->item_size > (char *)buf->buffer + buf->size - 1)
		{
			//Wraparound
			buf->tail = buf->buffer;
		}
		else
		{
			buf->tail = (void *)((char *)buf->tail + buf->item_size);
		}
	}
	else
	{
		//Not an empty buffer. Check size and continue.

		//Simple case of tail being one behind head
		if ((void *)((char *)buf->head - buf->item_size) == buf->tail)
		{
			return BUFFER_FULL;
		}
		//Otherwise, check to see if we are on the edge and the head is at the beginning
		else if ((void *)((char *)buf->buffer + buf->size - buf->item_size) == buf->tail && buf->head == buf->buffer)
		{
			return BUFFER_FULL;
		}

		memcpy(buf->tail, item, buf->item_size);

		//Check if we are at the end of the buffer. If so, move to the front.
		if ((void *)((char *)buf->tail + buf->item_size) > (void *)((char *)buf->buffer + buf->num_items))
		{
			buf->tail = buf->buffer;
		}
		else
		{
			buf->tail = (void *)((char *)buf->tail + buf->item_size);
		}


	}

	return BUFFER_OK;
}


CircBufStatus_t CircBuf_Remove(CircBuf_t *buf)
{
	if (buf == NULL)
	{
		return BUFFER_BAD;
	}

	if (buf->head == buf->tail )
	{
		return BUFFER_EMPTY;
	}

	if (buf->head == (void*)((char *)buf->buffer + buf->size))
	{
		buf->head = buf->buffer;
	}

	memset(buf->head, 'X', 1); //TODO: remove

	buf->head = (void *)((char *)buf->head + buf->item_size);


	return BUFFER_OK;
}

void CircBuf_Dump(CircBuf_t *buf)
{
	char *ch;

	if (buf == NULL)
	{
		return;
	}

	ch = (char *)buf->buffer;

	while (ch < ((char *)buf->buffer + buf->size))
	{
		SendChar(*ch);
		SendChar(' ');
		ch++;
	}

	SendChar('\n');
}
