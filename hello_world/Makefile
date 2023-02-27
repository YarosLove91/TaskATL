K = hello

all:
	@make -C src

clean:
	@make -C src clean
	@rm hello

qemu:
	 # gdb --args ~/Documents/kernel_workspace/qemu/build/qemu-system-riscv32 -M sifive_u -m 64k -kernel $K -nographic -bios none -d guest_errors -D test.txt
	 ~/qemu-7.2.0/build/qemu-system-riscv32 -M sifive_u -kernel $K -nographic -bios none

qemu-gdb:
	# ~/Documents/kernel_workspace/qemu/build/qemu-system-riscv32 -M sifive_u -m 64k -kernel $K -nographic -bios none -s -S -d guest_errors -D test.txt
	qemu-system-riscv32 -M sifive_u -kernel $K -nographic -bios none -s -S
