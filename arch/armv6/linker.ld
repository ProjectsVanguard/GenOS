OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
    . = 0x00008000;
    .text : ALIGN(4) {
        KEEP(*(.text.boot))
        *(.text*)
    }
    .rodata : ALIGN(4) { *(.rodata*) }
    .data : ALIGN(4) { *(.data*) }
    .bss (NOLOAD) : ALIGN(8) {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    }
    . = ALIGN(8);
    . += 0x4000;
    __stack_top__ = .;
    /DISCARD/ : { *(.comment*) *(.note*) *(.eh_frame*) }
}
