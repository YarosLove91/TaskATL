.equ STACK_SIZE,   4096
.equ STACK_SHIFT,  30

.global _start

.section .text.init
_start:
	csrr    t0, mhartid
	slli    t0, t0, STACK_SHIFT
	la      sp, stacks + STACK_SIZE
	add     sp, sp, t0

1:
	call	start

.global pr
pr:
        lui     a1, 65552
        sw      a0, 0(a1)
        ret

.align 2
trap_entry:
	j	handle_trap

.section .bss
.align 4
.global stacks
stacks:
	.skip STACK_SIZE
