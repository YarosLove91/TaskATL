#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>
// Функции для работы со строками
void *memcpy(void *restrict dst, const void *restrict src, size_t size);
void *memset(void *dst, int c, size_t size);

#endif
