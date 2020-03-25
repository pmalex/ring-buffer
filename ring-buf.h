#ifndef RING_BUF_H
#define RING_BUF_H

typedef struct ring_buf {
	void*	start;		// Start address of area

	void*	head;
	void*	tail;

	unsigned     cnt;		// Count of elements
	unsigned     bs;		// Size of one elements (in bytes)
} ring_buf_t;

void ringbuf_init(ring_buf_t *rbuf, const void *addr, unsigned bs, unsigned cnt);

unsigned ringbuf_isfull(const ring_buf_t *rbuf);
unsigned ringbuf_isempty(const ring_buf_t *rbuf);

int ringbuf_nput(ring_buf_t *rbuf, const void *data, unsigned n);
int ringbuf_nget(ring_buf_t *rbuf, void *data, unsigned n);

int ringbuf_peek_until(const ring_buf_t *rbuf, void *data, unsigned char byte);
int ringbuf_get_until(ring_buf_t *rbuf, void *data, unsigned char byte);

unsigned ringbuf_get_cnt(const ring_buf_t *rbuf);
void ringbuf_flush(ring_buf_t *rbuf);


static inline
int ringbuf_put(ring_buf_t *rbuf, const void *data)
{
	return ringbuf_nput(rbuf, data, rbuf->bs);
}

static inline
int ringbuf_get(ring_buf_t *rbuf, void *data)
{
	return ringbuf_nget(rbuf, data, rbuf->bs);
}

#endif
