#include "platform.h"
#include "uart.h"

/* NS16550 registers (byte offsets) */
#define RBR 0x00 /* receive buffer (read) */
#define THR 0x00 /* transmit holding (write) */
#define IER 0x01
#define FCR 0x02
#define LCR 0x03
#define LSR 0x05

#define LSR_THRE (1<<5)
#define LSR_DR   (1<<0)

static inline volatile uint8_t* uart_reg(uintptr_t off){
    return (volatile uint8_t*)(UART0_BASE + off);
}

void uart_init(void){
    /* 8N1, FIFO enable (simple init; QEMU's default clock/baud works) */
    *uart_reg(LCR) = 0x03; /* 8-bit */
    *uart_reg(FCR) = 0x07; /* enable & clear FIFOs */
}

void uart_putc(char c){
    if (c == '\n') uart_putc('\r');
    while(((*uart_reg(LSR)) & LSR_THRE) == 0) { }
    *uart_reg(THR) = (uint8_t)c;
}

void uart_puts(const char* s){
    while (*s) uart_putc(*s++);
}

int uart_getc_nonblock(void){
    if (((*uart_reg(LSR)) & LSR_DR) == 0) return -1;
    return *uart_reg(RBR);
}
