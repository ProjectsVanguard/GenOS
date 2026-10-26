# GenOS Genesis — teste de inicialização BIOS

Esta é uma ISO ISO9660 mínima com catálogo El Torito BIOS e imagem de boot por emulação de disquete. O setor de boot imprime uma mensagem de diagnóstico via BIOS.

**Limitação importante:** confirma somente o caminho BIOS/El Torito. Não carrega `build/kernel.elf`, não oferece boot UEFI e não é a imagem completa do GenOS. O GenKernel espera os serviços do protocolo Limine.

Reconstrução (ambiente GNU binutils + Python):

```sh
as --32 boot-test/boot.S -o boot-test/boot.o
ld -m elf_i386 -Ttext 0x7c00 --oformat binary boot-test/boot.o -o boot-test/boot.bin
python3 boot-test/make_iso.py
```

A saída esperada é `build/GenOS-Genesis-BIOS-Boot-Test.iso`. O teste estrutural da ISO não substitui teste em QEMU/VirtualBox ou hardware.
