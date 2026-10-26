CC ?= gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Werror -ffreestanding -fno-stack-protector -fno-pic -fno-pie -m64 -mno-red-zone -mcmodel=kernel -fno-asynchronous-unwind-tables -fno-unwind-tables
LDFLAGS = -nostdlib -static -no-pie -Wl,-T,kernel/linker.ld -Wl,--build-id=none
.PHONY: all clean test gem suite-gem FORCE
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
build/org.genos.hello.gem: apps/hello/manifest.json apps/hello/bin/hello.txt tools/gem/genpkg.py | build
	python3 tools/gem/genpkg.py pack apps/hello -o $@
build/org.genos.suite.gem: FORCE apps/genos-suite/index.html tools/gem/build_suite.py tools/gem/genpkg.py
	python3 tools/gem/build_suite.py $@
test: all suite-gem
	python3 tools/gem/genpkg.py validate build/org.genos.hello.gem
	python3 tools/gem/genpkg.py validate build/org.genos.suite.gem
	python3 -c "import zipfile; z=zipfile.ZipFile('build/org.genos.suite.gem'); required=['manifest.json','apps/genos-suite/index.html','apps/notepad/index.html','apps/terminal/index.html','apps/solaris-networking/index.html']; missing=[p for p in required if p not in z.namelist()]; assert not missing, f'missing suite files: {missing}'; print('PASS: suite completa em um único .gem')"
	python3 -m py_compile tools/gem/genpkg.py
clean:
	rm -rf build
