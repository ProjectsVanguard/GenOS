CC ?= gcc
.DEFAULT_GOAL := all
CFLAGS = -std=c11 -O2 -Wall -Wextra -Werror -ffreestanding -fno-stack-protector -fno-pic -fno-pie -m64 -mno-red-zone -mcmodel=kernel -fno-asynchronous-unwind-tables -fno-unwind-tables
LDFLAGS = -nostdlib -static -no-pie -Wl,-T,kernel/linker.ld -Wl,--build-id=none
.PHONY: all clean test subsystem-test gem suite-gem app-store-gem FORCE
FORCE:
all: build/kernel.elf build/org.genos.hello.gem
build:
	mkdir -p build
build/kernel.o: kernel/kernel.c kernel/limine.h | build
	$(CC) $(CFLAGS) -c $< -o $@
build/kernel.elf: build/kernel.o kernel/linker.ld
	$(CC) $(LDFLAGS) $< -o $@
	file $@
gem: build/org.genos.hello.gem
suite-gem: build/org.genos.suite.gem
app-store-gem: build/org.genos.app-store.gem
build/org.genos.hello.gem: apps/hello/manifest.json apps/hello/bin/hello.txt tools/gem/genpkg.py | build
	python3 tools/gem/genpkg.py pack apps/hello -o $@
build/org.genos.suite.gem: FORCE apps/genos-suite/index.html tools/gem/build_suite.py tools/gem/genpkg.py
	python3 tools/gem/build_suite.py $@
build/org.genos.app-store.gem: FORCE apps/app-store/index.html apps/app-store/manifest.json tools/gem/genpkg.py | build
	python3 tools/gem/genpkg.py pack apps/app-store -o $@
subsystem-test: | build
	$(CC) -std=c11 -O2 -Wall -Wextra -Werror -I. tests/test_subsystems.c kernel/subsystems/vfs.c kernel/subsystems/net_parse.c kernel/subsystems/usb_desc.c genui/genui3.c -o build/test_subsystems
	./build/test_subsystems
	$(CC) -std=c11 -O2 -Wall -Wextra -Werror -I. tests/test_fat32.c kernel/subsystems/fat32.c kernel/subsystems/vfs.c -o build/test_fat32
	./build/test_fat32
test: all suite-gem app-store-gem subsystem-test
	python3 tools/gem/genpkg.py validate build/org.genos.hello.gem
	python3 tools/gem/genpkg.py validate build/org.genos.suite.gem
	python3 tools/gem/genpkg.py validate build/org.genos.app-store.gem
	python3 -c "import zipfile; z=zipfile.ZipFile('build/org.genos.suite.gem'); required=['manifest.json','apps/genos-suite/index.html','apps/notepad/index.html','apps/terminal/index.html','apps/solaris-networking/index.html']; missing=[p for p in required if p not in z.namelist()]; assert not missing, f'missing suite files: {missing}'; print('PASS: suite completa em um único .gem')"
	python3 -m py_compile tools/gem/genpkg.py
clean:
	rm -rf build

# Experimental ARMv6 (ARM1176JZF-S) cross-build; separate from the x86_64 default.
ARM_CC ?= arm-none-eabi-gcc
ARM_OBJCOPY ?= arm-none-eabi-objcopy
ARM_CFLAGS = -std=c11 -O2 -Wall -Wextra -Werror -ffreestanding -fno-stack-protector -fno-pic -fno-pie -mcpu=arm1176jzf-s -marm
ARM_LDFLAGS = -nostdlib -Wl,-T,arch/armv6/linker.ld -Wl,--build-id=none
.PHONY: armv6
armv6: build/armv6/genos-armv6.elf build/armv6/kernel.img
build/armv6:
	mkdir -p $@
build/armv6/boot.o: arch/armv6/boot.S | build/armv6
	$(ARM_CC) $(ARM_CFLAGS) -c $< -o $@
build/armv6/kernel.o: arch/armv6/kernel.c | build/armv6
	$(ARM_CC) $(ARM_CFLAGS) -c $< -o $@
build/armv6/genos-armv6.elf: build/armv6/boot.o build/armv6/kernel.o arch/armv6/linker.ld
	$(ARM_CC) $(ARM_CFLAGS) $(ARM_LDFLAGS) build/armv6/boot.o build/armv6/kernel.o -o $@
	arm-none-eabi-readelf -h $@ | grep -q 'Machine:.*ARM'
build/armv6/kernel.img: build/armv6/genos-armv6.elf
	$(ARM_OBJCOPY) -O binary $< $@
