#include <stddef.h>

/* Symbol is needed to C runtime */
void *memcpy(void *restrict dst, const void *restrict src, size_t size)
{
	size_t i;
	unsigned char *restrict d = dst;
	const unsigned char *restrict s = src;

	for (i = 0; i < size; ++i)
		d[i] = s[i];

	return d;
}

/* Symbol is needed to C runtime */
void *memset(void *dst, int c, size_t size)
{
	size_t i;
	unsigned char *restrict d = dst;

	for (i = 0; i < size; ++i)
		d[i] = c;

	return d;
}
