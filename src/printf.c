#include <stdarg.h>
#include <stdint.h>
#include "uart.h"

static void uart_put_uint(unsigned int v) {
    char buf[12];
    int i = 0;
    if (v == 0) { uart_putc('0'); return; }
    while (v > 0) {
        buf[i++] = '0' + (v % 10);
        v /= 10;
    }
    while (i--) uart_putc(buf[i]);
}

static void uart_put_hex(unsigned int v) {
    const char *hex = "0123456789abcdef";
    for (int i = 28; i >= 0; i -= 4) {
        uart_putc(hex[(v >> i) & 0xf]);
    }
}

void printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'd') {
                int v = va_arg(ap, int);
                if (v < 0) { uart_putc('-'); v = -v; }
                uart_put_uint((unsigned int)v);
            } else if (*fmt == 'x') {
                uart_put_hex(va_arg(ap, unsigned int));
            } else if (*fmt == 's') {
                uart_puts(va_arg(ap, const char *));
            } else if (*fmt == 'c') {
                uart_putc((char)va_arg(ap, int));
            } else {
                uart_putc('%');
                uart_putc(*fmt);
            }
        } else {
            uart_putc(*fmt);
        }
        fmt++;
    }

    va_end(ap);
}
