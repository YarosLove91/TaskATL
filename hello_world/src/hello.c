#include <UART_RISC_V.h>

void test_pattern(void);

void __attribute((noreturn)) start(void)
{
	uart_init();			// настройка UART

	test_pattern();
	printf("Program completed !\n");
	while (1);
}
