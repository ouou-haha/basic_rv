#include <stdint.h>
#include "platform.h"
#include "uart.h"
#include "clint.h"

extern volatile uint64_t g_ticks;

static void enable_interrupts(void){
    set_mie(MIE_MTIE);          /* enable machine timer interrupt */
    asm volatile("csrs mstatus, %0"::"r"(MSTATUS_MIE)); /* global MIE */
}

int main(void){
    uart_init();
    uart_puts("\n[RISC-V bare-metal] hart0 booting...\n");

    /* init timer: 10ms tick assuming mtime ~ 10MHz in QEMU (default 10_000_000) */
    clint_timer_init(100000);  /* 10ms */
    enable_interrupts();

    uint64_t last = 0;
    uart_puts("Echo (polling). Every 100 ticks prints a dot.\n");
    while (1) {
        int ch = uart_getc_nonblock();
        if (ch >= 0) {
            uart_putc((char)ch);
        }
        if (g_ticks - last >= 100) {
            last = g_ticks;
            uart_putc('.');
        }
    }
    return 0;
}
