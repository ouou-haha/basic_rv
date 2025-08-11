#include "platform.h"
#include "clint.h"

#define CLINT_MSIP(hart)      (CLINT_BASE + 0x0000 + 4*(hart))
#define CLINT_MTIMECMP(hart)  (CLINT_BASE + 0x4000 + 8*(hart))
#define CLINT_MTIME           (CLINT_BASE + 0xBFF8)

static uint64_t g_tick_delta = 0;

static inline void write64(uintptr_t addr, uint64_t v){
    *(volatile uint64_t*)addr = v;
}
static inline uint64_t read64(uintptr_t addr){
    return *(volatile uint64_t*)addr;
}

uint64_t clint_mtime(void){
    return read64(CLINT_MTIME);
}

void clint_timer_init(uint64_t tick_cycles){
    g_tick_delta = tick_cycles;
    uint64_t now = clint_mtime();
    uint64_t hart = read_mhartid();
    write64(CLINT_MTIMECMP(hart), now + g_tick_delta);
}

void clint_timer_ack(void){
    uint64_t now = clint_mtime();
    uint64_t hart = read_mhartid();
    write64(CLINT_MTIMECMP(hart), now + g_tick_delta);
}
