#pragma once
#include <stdint.h>

/* QEMU 'virt' memory map (M-mode) */
#define UART0_BASE   0x10000000UL   /* ns16550a */
#define CLINT_BASE   0x02000000UL   /* mtime/mtimecmp/msip */

static inline void write_csr_mstatus(uint64_t v){ asm volatile("csrw mstatus, %0"::"r"(v)); }
static inline uint64_t read_csr_mstatus(void){ uint64_t x; asm volatile("csrr %0, mstatus":"=r"(x)); return x; }
static inline void set_csr_bits(const char* csr, uint64_t mask) { asm volatile("csrs mstatus, %0"::"r"(mask)); }
static inline void set_mie(uint64_t mask){ asm volatile("csrs mie, %0"::"r"(mask)); }
static inline void clr_mie(uint64_t mask){ asm volatile("csrc mie, %0"::"r"(mask)); }
static inline uint64_t read_mhartid(void){ uint64_t x; asm volatile("csrr %0, mhartid":"=r"(x)); return x; }

/* mstatus MIE bit */
#define MSTATUS_MIE  (1UL << 3)
/* mie MTIE bit (machine-timer interrupt enable) */
#define MIE_MTIE     (1UL << 7)
