#include <stdio.h>
#include <stdint.h>

void __attribute((noreturn)) panic(const char *reason)
{
	printf("\nPANIC: ");
	printf(reason);
	printf("\nreboot me.....\n");

	while (1);
	__builtin_unreachable();
}

void handle_trap(void)
{
	uint32_t mcause, mepc, mtval;

	asm volatile("csrr %0, mcause" : "=r"(mcause));
	asm volatile("csrr %0, mepc" : "=r"(mepc));
	asm volatile("csrr %0, mtval" : "=r"(mtval));

	printf("TRAP: mcause: %lu mecpc %lu mtval %lu\n", mcause, mepc, mtval);

	while (1);
	__builtin_unreachable();
}
