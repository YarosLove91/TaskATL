.equ STACK_SIZE,   4096

.global _start

.section .text.init
_start:
	csrr    t0, mhartid
	bnez    t0, park

	/* Vector trap handler (direct mode, trap_entry is .align 2) */
	la      t0, trap_entry
	csrw    mtvec, t0

	la      sp, stacks + STACK_SIZE

	call	start

1:
	j	1b

.global pr
pr:
        lui     a1, 65552
        sw      a0, 0(a1)
        ret

.align 2
trap_entry:
	j	handle_trap

/* Secondary harts have no own stack and no work: park them */
park:
	wfi
	j	park

.section .bss
.align 4
.global stacks
stacks:
	.skip STACK_SIZE
