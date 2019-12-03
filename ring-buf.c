#include <ring-buf.h>
#include <string.h>

int ringbuf_init(ring_buf_t *const rbuf, void *const addr, const unsigned bsize, const unsigned count)
{
	/* inappropriate paremeters */
	if(bsize == 0 || count < 2 || rbuf == NULL || addr == NULL)
		return -1;

	rbuf->start = addr;

	rbuf->head = rbuf->start;
	rbuf->tail = rbuf->start;

	rbuf->cnt = count;
	rbuf->bs = bsize;

	return 0;
}

void* ringbuf_head(const ring_buf_t *const rbuf)
{
	return rbuf->head;
}

void* ringbuf_tail(const ring_buf_t *const rbuf)
{
	return rbuf->tail;
}

void* ringbuf_next(const ring_buf_t *const rbuf, const void *const ptr)
{
	return (rbuf->start + ((ptr - rbuf->start) + rbuf->bs)%(rbuf->cnt * rbuf->bs));
}

/* Is full? */
int ringbuf_full(const ring_buf_t *const rbuf)
{
	return (ringbuf_next(rbuf, rbuf->tail) == rbuf->head);
}

/* Is empty? */
int ringbuf_empty(const ring_buf_t *const rbuf)
{
	return (rbuf->head == rbuf->tail);
}

/* Insert element */
int ringbuf_put(ring_buf_t* const rbuf, const void *const data)
{
	/* if buffer is full, just return */
	if(ringbuf_full(rbuf))
		return -1;

	/* buffer is not full, then we insert element */
	memcpy(rbuf->tail, data, rbuf->bs);
	rbuf->tail = ringbuf_next(rbuf, rbuf->tail);

    return 0;
}

/* Get an element */
int ringbuf_get(ring_buf_t *const rbuf, void *const data)
{
	/* if buffer is empty, just return */
	if(ringbuf_empty(rbuf))
		return -1;

	/* take an element */
	memcpy(data, rbuf->head, rbuf->bs);
	rbuf->head = ringbuf_next(rbuf, rbuf->head);

	return 0;
}

/* Get count of elements inside buffer */
size_t ringbuf_getcnt(const ring_buf_t *const rbuf)
{
	int d = (rbuf->tail - rbuf->head)/rbuf->bs;
	if (d >= 0)
		return d;
	else
		return rbuf->cnt + d;
}

/* Clear buffer */
void ringbuf_flush(ring_buf_t *const rbuf)
{
	rbuf->head = rbuf->tail;
}
