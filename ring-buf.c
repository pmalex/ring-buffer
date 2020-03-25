#include "ring-buf.h"

static int ringbuf_npeek(const ring_buf_t *rbuf, const void *ptr, void *data,
					     unsigned n);
static void* ringbuf_next(const ring_buf_t *rbuf, const void *ptr);

void ringbuf_init(ring_buf_t *rbuf, const void *addr, unsigned bs, unsigned cnt)
{
	rbuf->start = rbuf->head = rbuf->tail = (void*)addr;

	rbuf->cnt = cnt;
	rbuf->bs = bs;
}

static void* ringbuf_next(const ring_buf_t *rbuf, const void *ptr)
{
	return (rbuf->start + ((ptr - rbuf->start) + rbuf->bs)%
				(rbuf->cnt * rbuf->bs));
}

/* Is full? */
unsigned ringbuf_isfull(const ring_buf_t *rbuf)
{
	return (ringbuf_next(rbuf, rbuf->tail) == rbuf->head);
}

/* Is empty? */
unsigned ringbuf_isempty(const ring_buf_t *rbuf)
{
	return (rbuf->head == rbuf->tail);
}

/* Peek n bytes from buffer element pointed by ptr */
static int ringbuf_npeek(const ring_buf_t *rbuf, const void *ptr, void *data,
						 unsigned n)
{
	/* If buffer is empty, just return error code */
	if (ringbuf_isempty(rbuf))
	{
		return -1;
	}

	/* Check if n is exceeded buffer element size */
	n = (n > rbuf->bs)?rbuf->bs:n;

	unsigned i;

	/* Just copy elements */
	for (i = 0; i < n; i++)
	{
		((unsigned char*)data)[i] = ((unsigned char*)ptr)[i];
	}

	return i; /* Return number of inserted bytes */
}

/* Insert n bytes in buffer element */
int ringbuf_nput(ring_buf_t *rbuf, const void *data, unsigned n)
{
	/* If buffer is full, just return error code */
	if (ringbuf_isfull(rbuf))
	{
		return -1;
	}

	/* Check if n is exceeded buffer element size */
	n = (n > rbuf->bs)?rbuf->bs:n;

	unsigned i;

	/* Buffer is not full, therefore we can insert the element */
	for (i = 0; i < n; i++)
	{
		((unsigned char*)(rbuf->tail))[i] = ((unsigned char*)data)[i];
	}

	rbuf->tail = ringbuf_next(rbuf, rbuf->tail);

    return i;
}

/* Get n bytes from buffer element */
int ringbuf_nget(ring_buf_t *rbuf, void *data, unsigned n)
{
	int rc;

	/* Peek head */
	if ((rc = ringbuf_npeek(rbuf, rbuf->head, data, n)) >= 0)
	{
		rbuf->head = ringbuf_next(rbuf, rbuf->head);
	}

	return rc;
}

int ringbuf_peek_until(const ring_buf_t *rbuf, void *data, unsigned char byte)
{
	if (ringbuf_isempty(rbuf))
	{
		return -1;
	}

	unsigned i;

	for (i = 0; i < rbuf->bs; i++)
	{
		/* Let's assign until the byte not match with some array element */
		if ((((unsigned char*)data)[i] = ((unsigned char*)(rbuf->head))[i])
				== byte)
		{
			break;
		}
	}

	return i;
}

int ringbuf_get_until(ring_buf_t *rbuf, void *data, unsigned char byte)
{
	int rc;

	if ((rc = ringbuf_peek_until(rbuf, data, byte)) >= 0)
	{
		rbuf->head = ringbuf_next(rbuf, rbuf->head);
	}

	return rc;
}

/* Get count of elements inside buffer */
unsigned ringbuf_get_cnt(const ring_buf_t *rbuf)
{
	if (rbuf->tail > rbuf->head)
	{
		return (unsigned)((rbuf->tail - rbuf->head)/rbuf->bs);
	}
	else
	{
		return (unsigned)((rbuf->head - rbuf->tail)/rbuf->bs);
	}
}

/* Clear buffer */
void ringbuf_flush(ring_buf_t *rbuf)
{
	rbuf->head = rbuf->tail;
}
