#include <stdint.h>
#include "platform.h"
#include "uart.h"
#include "clint.h"

volatile uint64_t g_ticks = 0;

/* trap vector in C: save minimal context in assembly prologue in start.S (mtvec points here) */
void trap_handler(uintptr_t mcause, uintptr_t mepc, uintptr_t mtval){
    /* Interrupt? MSB of mcause set on RV64 */
    if (mcause >> 63) {
        uint64_t cause = mcause & 0xfff;
        if (cause == 7) { /* machine timer interrupt */
            g_ticks++;
            clint_timer_ack();
        }
        return;
    } else {
        uart_puts("\n[trap] exception mcause=");
        /* very small hex print */
        for (int i=60;i>=0;i-=4){
            int d = (mcause>>i) & 0xf;
            uart_putc("0123456789abcdef"[d]);
        }
        uart_puts(" mepc=");
        for (int i=60;i>=0;i-=4){
            int d = (mepc>>i) & 0xf;
            uart_putc("0123456789abcdef"[d]);
        }
        uart_puts(" mtval=");
        for (int i=60;i>=0;i-=4){
            int d = (mtval>>i) & 0xf;
            uart_putc("0123456789abcdef"[d]);
        }
        uart_puts("\nHalting.\n");
        while(1){ asm volatile("wfi"); }
    }
}

__attribute__((naked)) void trap_vec(void){
    asm volatile(
        "addi sp, sp, -128\n"
        "sd ra, 120(sp)\n"
        "sd a0, 112(sp)\n"
        "sd a1, 104(sp)\n"
        "sd a2, 96(sp)\n"
        "csrr a0, mcause\n"
        "csrr a1, mepc\n"
        "csrr a2, mtval\n"
        "call trap_handler\n"
        "ld ra, 120(sp)\n"
        "addi sp, sp, 128\n"
        "mret\n"
    );
}
