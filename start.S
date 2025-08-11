    .section .text.boot, "ax"
    .globl _start
_start:
    /* set stack (per hart same top; non-0 harts will park) */
    la   sp, _stack_top

    /* read mhartid */
    csrr a0, mhartid
    beqz a0, 1f
0:
    /* non-zero hart: park in WFI loop */
    wfi
    j 0b

1:  /* hart0 continues */

    /* set mtvec to trap_vec (direct mode) */
    la   t0, trap_vec
    csrw mtvec, t0

    /* clear bss */
    la   t0, __bss_start
    la   t1, __bss_end
2:
    bgeu t0, t1, 3f
    sd   zero, 0(t0)
    addi t0, t0, 8
    j 2b
3:

    /* jump to main */
    la   t0, main
    jr   t0

    .section .bss
    .globl __bss_start
__bss_start:
    .globl __bss_end
__bss_end:
