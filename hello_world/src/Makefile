MAKEFLAGS += --silent

# Префикс кросс-тулчейна (riscv-none-elf-gcc, riscv-none-elf-ld, ...).
# Порядок выбора:
#   1) явное задание: make CROSS=/path/to/prefix  (или export CROSS=...)
#   2) riscv-none-elf из PATH
#   3) xpack-тулчейн из $(TOOLS_ROOT)/xpack-riscv-none-elf-gcc-*/bin
# Пример: make CROSS=/opt/riscv/bin/riscv32-unknown-elf
TOOLS_ROOT ?= $(HOME)/Work/TOOLS
CROSS ?= $(shell \
	if command -v riscv-none-elf-gcc >/dev/null 2>&1; then \
		echo riscv-none-elf; \
	else \
		g=$$(ls -d $(TOOLS_ROOT)/xpack-riscv-none-elf-gcc-*/bin/riscv-none-elf-gcc 2>/dev/null | sort -V | tail -n 1); \
		echo $${g%-gcc}; \
	fi)
ifeq ($(CROSS),)
CROSS := riscv-none-elf
endif

ABI_OLD = -march=rv32imac_zicsr -mabi=ilp32 -mcmodel=medany
# ABI = -march=rv32imafdc -mabi=ilp32d

CFLAGS = -O0 -ffreestanding -static -nostdlib $(ABI_OLD) -Wall -pedantic -Iinclude -g3

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

all: $(OBJ) crt.o
	@echo "LD hello"
	$(CROSS)-ld  crt.o $(OBJ) -T qemu.ld -o ../hello

$(OBJ): $(SRC)
	@echo "CC $@"
	$(CROSS)-gcc -c $(SRC) $(CFLAGS)

crt.o:
	@echo "AS $@"
	$(CROSS)-gcc -c crt.S -o crt.o $(ABI_OLD)

clean:
	@rm -f $(OBJ) crt.o
