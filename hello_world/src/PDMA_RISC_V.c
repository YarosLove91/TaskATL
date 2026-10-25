#include <PDMA_RISC_V.h>

static void dma_start(void)
{
	writel((PDMA_CONTROL_RUN_Set |  PDMA_CONTROL_CLAIM_Set), &PDMA_CH0->PDMA_CONTROL);

	/* from qemu emulator:
	 *
	 * If channel was not claimed before run bit is set,
	 * or if the channel is disclaimed when run was low,
	 * DMA won't run.
	 */
}

static void dma_claim_channel(void)
{
	writel(PDMA_CONTROL_CLAIM_Set, &PDMA_CH0->PDMA_CONTROL);
}

void dma_wait_transaction(void)
{
	/* Wait untill previous one is completed */
	while (1) {
		pdmareg32_t cntr = readl(&PDMA_CH0->PDMA_CONTROL);
		if (cntr & PDMA_CONTROL_fDONE)
			break;
		else if (cntr & PDMA_CONTROL_fERROR)
			panic("DMA transfer error");
	}
}

/* @size_code is a PDMA_NCR_WSIZE_x code: transaction size = 1 << size_code bytes.
 * rsize == wsize is a MUST based on qemu emulator */
void dma_memcpy(void *dst, const volatile void *src, size_t size_code, int wait_ready)
{
	if (wait_ready)
		dma_wait_transaction();

	dma_claim_channel();

	writel((PDMA_NEXT_CONFIG_RSIZE(size_code) | PDMA_NEXT_CONFIG_WSIZE(size_code)), &PDMA_CH0->PDMA_NEXT_CONFIG);
	writel(U(dst), &PDMA_CH0->PDMA_NEXT_DST);
	writel(U(src), &PDMA_CH0->PDMA_NEXT_SOURCE);
	writel((1 << size_code), &PDMA_CH0->PDMA_NEXT_BYTES);

	dma_start();
}
