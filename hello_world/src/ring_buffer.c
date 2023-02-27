#include <ring_buffer.h>
#include <string.h>

#define ASSERT(cond) typedef int foo[(cond) ? 1 : -1]

#define MASK		(SIZE - 1)
#define T_INDEX		(SIZE >> 1)

void ring_buffer_init(struct RingBuffer *b)
{
	memset(b, 0, sizeof(*b));
}

int ring_buffer_is_full(const struct RingBuffer *b)
{
	return ((b->w_count - b->r_count) & (T_INDEX) & ~(MASK)) != 0;
}

int ring_buffer_is_empty(const struct RingBuffer *b)
{
	return b->r_count == b->w_count;
}

unsigned ring_buffer_count(const struct RingBuffer *b)
{
	return (b->w_count - b->r_count) & MASK;
}

int ring_buffer_write(struct RingBuffer *b, data_t value)
{
	if (ring_buffer_is_full(b) && b->fifo_status.status.full) {
        	b->fifo_status.status.overflow = 1;              // Попытка записать в полный буфер
		return 0;
	} else if (ring_buffer_is_full(b)){
		b->fifo_status.status.full = 1;              // Буфер заполнен
		return 0;
	}

	b->data[b->w_count++ & MASK] = value;

	b->fifo_status.status.empty = b->fifo_status.status.overflow =   0;

	if (ring_buffer_count(b) >= T_INDEX)
		b->fifo_status.status.half_full    =   1;

	return 1;
}

int ring_buffer_read(struct RingBuffer *b, data_t *val)
{
	if (ring_buffer_is_empty(b) && b->fifo_status.status.full) {
        	b->fifo_status.status.underflow = 1;              // Попытка записать в полный буфер
		return 0;
	} else if (ring_buffer_is_empty(b)){
		b->fifo_status.status.full = 0;
		b->fifo_status.status.empty = 1;
		return 0;
	}

	*val = b->data[b->r_count++ & MASK];

	b->fifo_status.status.full = b->fifo_status.status.overflow =  0;              // Если был флаг полного буфера и переполнения, сбрасываем

	if (ring_buffer_count(b) < T_INDEX)
		b->fifo_status.status.half_full = 0;

	return 1;
}

int ring_buffer_read_many(struct RingBuffer *b, data_t *val, unsigned int count)
{
	unsigned i;

	if (ring_buffer_is_empty(b) && b->fifo_status.status.full) {
        	b->fifo_status.status.underflow = 1;              // Попытка записать в полный буфер
		return 0;
	} else if (ring_buffer_is_empty(b)){
		b->fifo_status.status.full = 0;
		b->fifo_status.status.empty = 1;
		return 0;
	} else if (ring_buffer_count(b) < count) {				// Можем вычитать count элементов ?
		return 0;
	}

	for (i = 0; i < count; ++i)								// Вычитываем элементы
		val[i] = b->data[b->r_count++ & MASK];

	b->fifo_status.status.full = b->fifo_status.status.overflow =  0;              // Если был флаг полного буфера и переполнения, сбрасываем

	if (ring_buffer_count(b) < T_INDEX)
		b->fifo_status.status.half_full = 0;

	return 1;
}

void ring_buffer_clear(struct RingBuffer *b)
{
	b->r_count = 0;
	b->w_count = 0;
	b->fifo_status.reg_status = 0;
	b->fifo_status.status.empty	=	1;
}

const uint8_t get_status (const struct RingBuffer *b){
	return b->fifo_status.reg_status;
}
