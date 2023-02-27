#ifndef __PANIC_H__
#define __PANIC_H__

#include <stdint.h>

void __attribute((noreturn)) panic(const char *reason);

#endif
