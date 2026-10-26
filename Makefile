CC ?= gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Werror -ffreestanding -fno-stack-protector -fno-pic -fno-pie -m64 -mno-red-zone -mcmodel=kernel -fno-asynchronous-unwind-tables -fno-unwind-tables
LDFLAGS = -nostdlib -static -no-pie -Wl,-T,kernel/linker.ld -Wl,--build-id=none
.PHONY: all clean test gem
all: build/kernel.elf build/org.genos.hello.gem
build:
	mkdir -p build
build/kernel.o: kernel/kernel.c kernel/limine.h | build
	$(CC) $(CFLAGS) -c $< -o $@
build/kernel.elf: build/kernel.o kernel/linker.ld
	$(CC) $(LDFLAGS) $< -o $@
	file $@
gem: build/org.genos.hello.gem
build/org.genos.hello.gem: apps/hello/manifest.json apps/hello/bin/hello.txt tools/gem/genpkg.py | build
	python3 tools/gem/genpkg.py pack apps/hello -o $@
test: all
	python3 tools/gem/genpkg.py validate build/org.genos.hello.gem
	python3 -m py_compile tools/gem/genpkg.py
clean:
	rm -rf build
