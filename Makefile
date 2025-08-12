TARGET = app
CROSS ?= riscv64-unknown-elf-
CC     = $(CROSS)gcc
LD     = $(CROSS)gcc
OBJCOPY= $(CROSS)objcopy

# CFLAGS = -march=rv64imac -mabi=lp64 -mcmodel=medany -O2 -ffreestanding -fno-builtin -Wall -Wextra -Iinclude
CFLAGS = -march=rv64imac_zicsr_zifencei -mabi=lp64 -mcmodel=medany -O2 -ffreestanding -fno-builtin -Wall -Wextra -Iinclude
LDFLAGS= -nostdlib -Wl,-Bstatic,-T,linker.ld,-Map,$(TARGET).map,--build-id=none

SRCS = start.S src/trap.c src/uart.c src/clint.c src/main.c
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.S=.o)

all: $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(SRCS) linker.ld
	$(LD) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f $(TARGET).elf $(TARGET).bin $(TARGET).map $(OBJS)

run: $(TARGET).elf
	qemu-system-riscv64 -M virt -nographic -bios none -kernel $(TARGET).elf

gdb: $(TARGET).elf
	qemu-system-riscv64 -M virt -nographic -bios none -kernel $(TARGET).elf -s -S

stop:
	@echo "Stopping QEMU..."
	@pkill -f qemu-system-riscv64 || true

.PHONY: all clean run gdb
