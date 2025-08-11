#pragma once
#include <stdint.h>

void clint_timer_init(uint64_t tick_cycles);
void clint_timer_ack(void);
uint64_t clint_mtime(void);
