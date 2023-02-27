MAKEFLAGS += --silent

#CROSS ?= ~/random_apps/riscv/bin/riscv32-unknown-elf
#CROSS ?=  /opt/riscv/bin/riscv64-unknown-linux-gnu
CROSS ?=  /opt/riscv/bin/riscv32-unknown-elf

ABI_OLD = -march=rv32imac -mabi=ilp32 -mcmodel=medany
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
	@rm $(OBJ) crt.o
