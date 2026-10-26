# Native Wine port to GenOS: prerequisites and implementation plan

## Status

Wine is **not installed** by this change. GenOS currently boots a freestanding x86-64 kernel and framebuffer desktop. It does not yet provide the Unix/POSIX environment or user-mode process services expected by upstream Wine. Copying a Linux Wine binary into a `.gem` package would not make it executable.

## Why a native port cannot be a simple package

Upstream Wine is a large compatibility implementation that relies on host operating-system facilities and architecture-specific code. A GenOS port needs a working host ABI and runtime first. At minimum:

- Ring-3 user processes, context switching, exception delivery and process/thread lifecycle.
- Virtual address spaces, page-fault handling, memory mapping and protection, shared memory, and synchronization primitives.
- A stable system-call ABI and a POSIX-like layer sufficient to build and run Wine's Unix-side components.
- A filesystem with file descriptors, directories, permissions, path handling, and writable application data.
- ELF loading for GenOS-native userland and PE/COFF loading for Windows executables, including relocations and import resolution.
- Required Unix libraries, dynamic linking, TLS, signals, sockets, IPC, timers, and threading.
- A graphics/windowing path and input support; audio and networking are needed by many Windows programs.
- A reproducible cross-build of a pinned upstream Wine version and its dependencies, plus licensing and provenance records.

These are prerequisites, not claims that all Windows applications will work. Wine compatibility varies by application.

## Recommended staged plan

1. **Kernel/user boundary:** GDT/TSS and ring-3 entry, interrupt-safe context switching, page tables and per-process address spaces.
2. **Syscall and process API:** syscall dispatch, process/thread creation and exit, wait, signals/exceptions, synchronization, and memory-map operations.
3. **Native userland:** ELF loader, dynamic linker strategy, basic C library, file descriptors, filesystem and console I/O.
4. **POSIX compatibility:** implement and test the subset needed by the selected Wine version, including threads, mmap/mprotect, futex-like synchronization, signals, clocks, sockets and IPC.
5. **Windows binary foundation:** PE loader and a minimal executable that exercises imports, relocations, exceptions and system DLL loading.
6. **Wine port:** select a pinned Wine release, add a GenOS host backend, cross-compile dependencies, and maintain upstream patches.
7. **Packaging and validation:** introduce a signed/versioned package format and dependency metadata; run a QEMU test suite for loader, APIs, GUI and regression cases before publishing an artifact.

## Acceptance criteria before calling it installed

- A user-mode ELF test program runs with isolated address space and exits through the syscall ABI.
- Faults in a user process do not crash the kernel.
- File I/O, memory mapping, threads, synchronization, and process creation tests pass.
- A minimal PE program runs under the compatibility layer.
- A built Wine binary launches that PE program in QEMU and logs its actual exit status.
- CI publishes an artifact only after those tests pass.

## Current alternatives

Until these milestones exist, GenOS cannot honestly claim native Wine support. Run Wine on a Linux host or virtual machine for now; a browser-based `.gem` app or a command that merely prints “Wine” is not Wine compatibility.
