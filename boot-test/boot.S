.code16
.global _start
.section .text
_start:
    cli
    xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    movw $0x7c00, %sp
    sti
    movw $0x0003, %ax
    int $0x10
    movw $msg, %si
print:
    lodsb
    testb %al, %al
    jz done
    movb $0x0e, %ah
    movb $0x07, %bl
    int $0x10
    jmp print
done:
    cli
hang:
    hlt
    jmp hang
msg:
    .asciz "GenOS Genesis ISO boot test\r\nBIOS boot path OK.\r\nThis is only a bootloader smoke test; GenKernel is not loaded yet.\r\n"
.org 510
.word 0xaa55
