/*
 * GenOS ARMv6 architecture bring-up.
 * This is deliberately a minimal compile/link smoke-test kernel, not the
 * x86_64 desktop kernel or a claim of full ARM hardware support.
 */
typedef unsigned int u32;

volatile u32 genos_armv6_boot_marker;

void armv6_kernel_main(void)
{
    /* A stable marker for future board-specific UART/GPIO diagnostics. */
    genos_armv6_boot_marker = 0x47454E36u; /* "GEN6" */
    for (;;) {
        __asm__ volatile ("wfi");
    }
}
