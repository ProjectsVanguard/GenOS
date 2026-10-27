# GenOS ARMv6 bring-up

This directory is an **experimental ARMv6 bare-metal bring-up scaffold** for
ARM1176JZF-S-class systems (for example, Raspberry Pi 1 / Zero v1.2 CPU class).
It is separate from the current AMD64/x86-64 GenOS kernel.

## Build

Install an ARM bare-metal GNU toolchain (commands below assume
`arm-none-eabi-gcc` and `arm-none-eabi-objcopy` are available):

```sh
make armv6
```

The output is `build/armv6/genos-armv6.elf` and a raw
`build/armv6/kernel.img` suitable as a starting point for board-specific
boot integration.

## Current limitations

- This only checks that an ARMv6 entry point can be compiled and linked.
- No Raspberry Pi firmware/SD-card image, board UART, GPIO, timer, interrupt
  controller, MMU, memory allocator, framebuffer, drivers, or desktop exists.
- The x86 Limine boot flow does not boot this ARMv6 image.
- CI compilation is not hardware boot validation. Do not describe ARMv6 as
  fully supported until an emulator/board boot test and hardware checks pass.

Next milestones: define a supported board and memory map; add early UART logging;
validate boot in QEMU `raspi0` or on supported hardware; then port memory,
interrupt, timer, framebuffer and input subsystems.
