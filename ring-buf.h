#ifndef _RING_BUF_H
#define _RING_BUF_H

#include <stdlib.h>

typedef struct ring_buf {
	void*	start;		// start address of area

	void*	head;
	void*	tail;

	unsigned	cnt;		// count of elements
	unsigned	bs;			// size of one elements (in bytes)
} ring_buf_t;

int ringbuf_init(ring_buf_t *const rbuf, void *const addr, const unsigned size, const unsigned count);
void* ringbuf_head(const ring_buf_t *const rbuf);
void* ringbuf_tail(const ring_buf_t *const rbuf);
void* ringbuf_next(const ring_buf_t *const rbuf, const void *const ptr);
int ringbuf_full(const ring_buf_t *const rbuf);
int ringbuf_empty(const ring_buf_t *const rbuf);
int ringbuf_put(ring_buf_t* const rbuf, const void *const data);
int ringbuf_get(ring_buf_t *const rbuf, void *const data);
size_t ringbuf_getcnt(const ring_buf_t *const rbuf);
void ringbuf_flush(ring_buf_t *const rbuf);

#endif
