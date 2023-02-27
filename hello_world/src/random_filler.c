#include <stdio.h>
#include <string.h>

#include <panic.h>
#include <ring_buffer.h>
#include <PDMA_RISC_V.h>
#include <random.h>
#include <config.h>

#define DEBUG
// 
#ifdef DEBUG
#define LOG(...)printf(__VA_ARGS__)
#else
#define LOG(...)do { } while (0)
#endif


#define FULL_MEMORY				// Емкость ОЗУ - 1 МБайт, иначе - 16 KiB

static inline void fill_transaction(struct RingBuffer *b){
	unsigned int i;
	for (i = 0; i < PDMA_MAX_TRANSACTION >> 2; ++i) {
		uint32_t rand = LFSR(0);
		ring_buffer_write(b, rand);
	}
}

static int LFSR_check_pattern(void *start, size_t size){
	unsigned i;
	uint32_t *s = start;

	LFSR(1);	/* do restart */

	for (i = 0; i < size / sizeof(uint32_t); ++i) {
		uint32_t rand = LFSR(0);
		uint32_t actual = s[i];

		//printf("r = %lx a = %lx [%d]\n", rand, actual, i);
		LOG ("r = %lx a = %lx [%d]\n", rand, actual, i);
		
		if (actual != rand)
			return -1;
	}

	return 0;
}

void test_pattern(void)
{
#ifdef FULL_MEMORY
// Емкость памяти - 1 МиБ
	size_t end = DRAM_BASE + DRAM_SIZE;
	size_t start = (size_t)&_end_of_image;
	size_t size =  end - start;
#endif
#ifndef FULL_MEMORY
	size_t size = DRAM_KiB_SIZE(16);					// Емкость ОЗУ - 16 Кибибайт (16384 байта)
#endif
	size_t i;
	struct RingBuffer buf;
	ring_buffer_init(&buf);

	/* Image is linked at DRAM_BASE, so
	 * _end points to the end of the image data.
	 *
	 * _end _should_ be aligned to 64 which is implied by linker script
	 */

	LFSR(1);	/* do restart */
	fill_transaction(&buf);

	for (i = 0; i < size / PDMA_MAX_TRANSACTION; i++) {
		uint32_t transaction[PDMA_MAX_TRANSACTION >> 2];

		ring_buffer_read_many(&buf, transaction, PDMA_MAX_TRANSACTION >> 2);

		dma_memcpy(&_end_of_image[i * PDMA_MAX_TRANSACTION],
		    transaction,
		    PDMA_NCR_WSIZE_64BYTE,
		    i != 0);

		fill_transaction(&buf);
		LOG("Transaction completed\n");
	}

	dma_wait_transaction();

	printf("RANDOM FILLER DONE\n");

	if (!LFSR_check_pattern(&_end_of_image, size))
		printf("Patterns do match\n");
	else
		printf("Patterns DO NOT match\n");
}
