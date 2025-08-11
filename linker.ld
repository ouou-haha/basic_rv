OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY {
  RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

SECTIONS
{
  . = ORIGIN(RAM);

  .text : {
    *(.text.boot)
    *(.text .text.*)
    *(.rodata .rodata.*)
  } > RAM

  . = ALIGN(8);
  .data : {
    *(.data .data.*)
  } > RAM

  . = ALIGN(8);
  .bss (NOLOAD): {
    *(.bss .bss.*)
    *(COMMON)
  } > RAM

  . = ALIGN(16);
  PROVIDE(_stack_top = ORIGIN(RAM) + LENGTH(RAM));
}
