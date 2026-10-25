# Changelog

All notable changes to this project are documented in this file.

## [Unreleased] - 2026-10-25

### Fixed

#### PDMA driver (`hello_world/src/PDMA_RISC_V.c`, `include/PDMA_RISC_V.h`)

- `dma_wait_transaction()` tested `fERROR` against the mask constant instead
  of the control register value, so a failed DMA transfer spun forever instead
  of panicking. Now checks `cntr & PDMA_CONTROL_fERROR`.
- `DMAx_UNIT_BASE` addresses were referenced by the `PDMA_CHx` macros before
  being defined; definitions reordered.
- `wsize`/`rsize` field masks widened from 1 bit to 3 bits (`0x07UL`) per the
  FU540 manual.
- `dma_memcpy()` parameter `size` renamed to `size_code`: it is a log2 size
  code (`PDMA_NCR_WSIZE_x`), not a byte count. Comments updated, including the
  `PDMA_MAX_TRANSACTION` note (64-byte transaction).

#### Boot code (`hello_world/src/crt.S`)

- `mtvec` now points at `trap_entry`, so machine-mode traps reach
  `handle_trap()` instead of vectoring to address 0.
- Secondary harts previously got stacks at `mhartid << 30` bytes offset while
  only one 4 KiB stack was allocated in `.bss` — out of bounds for any
  hart > 0. Now hart 0 runs the program and secondary harts park in a `wfi`
  loop.

#### Ring buffer (`hello_world/src/ring_buffer.c`, `include/ring_buffer.h`)

- `ring_buffer_is_full()` was identically zero: in `(w - r) & T_INDEX & ~MASK`
  the `T_INDEX` bit (0x40) is a subset of `MASK` (0x7F), so the expression can
  never be non-zero. A full buffer was never detected; writes silently
  overwrote unread data and the `overflow` flag was never raised. Replaced
  with `(w_count - r_count) == SIZE`.
- `ring_buffer_count()` masked the occupancy with `SIZE - 1`, which returned 0
  for a full buffer and made `ring_buffer_read_many()` refuse valid reads.
  Now returns the raw counter difference.
- `ring_buffer_init()` sets the `empty` status flag, consistent with
  `ring_buffer_clear()`.
- `ring_buffer_is_empty()` was defined but missing from the header (an error
  under C23 / GCC 15 implicit-declaration rules); declaration added.
- Dropped the ignored `const` qualifier on the `get_status()` return type
  (`-Wignored-qualifiers`).

#### Misc

- `config.h`: removed the unused `val` parameter from `readlLong()`; fixed
  comment typos.
- `UART_RISC_V.c`: annotated the intentional `%p` -> `%x` fallthrough in
  `vsnprintf()`.

### Changed

#### Build

- `src/Makefile`: the toolchain prefix `CROSS` is now resolved automatically —
  explicit `make CROSS=...` first, else `riscv-none-elf` from `PATH`, else the
  newest `xpack-riscv-none-elf-gcc-*` under `TOOLS_ROOT` (default
  `$(HOME)/Work/TOOLS`). Removes the hard-coded absolute path
  `/home/m.yaroslove/Work/...`.
- `src/Makefile`: `-march=rv32imac_zicsr` — binutils >= 2.38 requires the
  explicit `zicsr` extension for the CSR instructions used by `crt.S` and
  `panic.c`.
- `Makefile`: `qemu`/`qemu-gdb` targets run `$(QEMU)` (default
  `qemu-system-riscv32` from `PATH`) instead of a hard-coded `~/qemu-7.2.0`
  build path.
- `clean` targets use `rm -f` so they do not fail on missing artifacts.

### Verified

- Warning-free build with xpack riscv-none-elf-gcc 15.2.0
  (`-O0 -Wall -pedantic`).
- QEMU `sifive_u` run: 1 MiB PDMA random-pattern fill completes with
  `Patterns do match` / `Program completed !`.
- Ring buffer changes additionally covered by a host-side unit test:
  init state, fill to `SIZE`, write refusal with `full`/`overflow` flags,
  `read_many` from a full buffer, FIFO order across index wraparound,
  counter wraparound at 2^32, `half_full` watermark.
