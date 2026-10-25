#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>

#define DRAM_BASE			0x80000000UL			// Взято из даташита на МК
#define DRAM_SIZE			(0x01UL << 20)			// Емкость памяти - 1 МиБ (1024 байта x 1024 = 1024^2 байт)
#define TO_KiB				1024					// Конвертация в нормальные килобайты (кратные 1024)
#define DRAM_KiB_SIZE(_data)(TO_KiB*_data)

extern char _end_of_image[];

// Пересылка данных в регистр размером 32 бита
static inline void writel(uint32_t val, volatile void *addr){
	*(volatile uint32_t *) addr = val;
}
// Чтение данных из регистра размером 32 бита
static inline uint32_t readl(const volatile void *addr){
	return *(volatile uint32_t *) addr;
}

// Пересылка данных в регистр размером 64 бита
static inline void writelLong(uint64_t val, volatile void *addr){
	*(volatile uint64_t *) addr = val;
}
// Чтение данных из регистра размером 64 бита
static inline uint64_t readlLong(const volatile void *addr){
	return *(volatile uint64_t *) addr;
}


#endif
