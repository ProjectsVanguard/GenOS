#!/usr/bin/env bash
set -Eeuo pipefail
cd "$(dirname "$0")"
for c in xorriso; do command -v "$c" >/dev/null || { echo "Falta ferramenta: $c" >&2; exit 2; }; done
if [[ ! -x boot/limine/limine ]] || [[ ! -f boot/limine/limine-bios.sys ]] || [[ ! -f boot/limine/BOOTX64.EFI ]]; then
  echo 'Falta o bootloader Limine oficial em boot/limine/. Coloque limine, limine-bios.sys e BOOTX64.EFI nessa pasta.' >&2
  exit 2
fi
make all
mkdir -p build/iso-root/boot/limine build/iso-root/EFI/BOOT
cp build/kernel.elf build/iso-root/boot/kernel.elf
cp boot/limine.conf build/iso-root/boot/limine/limine.conf
cp boot/limine/limine-bios.sys build/iso-root/boot/limine/
cp boot/limine/BOOTX64.EFI build/iso-root/EFI/BOOT/
xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios.sys -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot EFI/BOOT/BOOTX64.EFI -efi-boot-part --efi-boot-image --protective-msdos-label build/iso-root -o build/GenOS-Genesis-x86_64.iso
boot/limine/limine bios-install build/GenOS-Genesis-x86_64.iso
sha256sum build/GenOS-Genesis-x86_64.iso > build/SHA256SUMS.txt
echo 'ISO gerada; teste BIOS e UEFI em VM antes de distribuir.'
