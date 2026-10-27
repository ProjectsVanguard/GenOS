/* GenOS ARMv6 bring-up entry point (ARM1176 / Raspberry Pi 1 class). */
.syntax unified
.cpu arm1176jzf-s
.arm

.section .text.boot, "ax"
.global _start
.type _start, %function
_start:
    cpsid if
    ldr sp, =__stack_top__
    bl armv6_kernel_main
1:
    wfi
    b 1b
.size _start, . - _start
