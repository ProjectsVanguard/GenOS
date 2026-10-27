#!/usr/bin/env bash
set -Eeuo pipefail
cd "$(dirname "$0")"
for c in xorriso; do command -v "$c" >/dev/null || { echo "Falta ferramenta: $c" >&2; exit 2; }; done
for f in boot/limine/limine boot/limine/limine-bios.sys boot/limine/limine-bios-cd.bin boot/limine/limine-uefi-cd.bin; do
  [[ -f "$f" ]] || { echo "Falta arquivo oficial do Limine: $f" >&2; exit 2; }
done
[[ -x boot/limine/limine ]] || chmod +x boot/limine/limine
make all
mkdir -p build/iso-root/boot/limine build/iso-root/Applications
cp build/kernel.elf build/iso-root/boot/kernel.elf
cp -a apps/. build/iso-root/Applications/
cp boot/limine.conf build/iso-root/boot/limine/limine.conf
cp boot/limine/limine-bios.sys boot/limine/limine-bios-cd.bin boot/limine/limine-uefi-cd.bin build/iso-root/boot/limine/
edition="$(printenv GENOS_EDITION 2>/dev/null || true)"
version="$(printenv GENOS_VERSION 2>/dev/null || true)"
edition="${edition:-Genesis}"
version="${version:-3.0.0-preview}"
cat > build/iso-root/GENOS-EDITION.txt <<EOF
GenOS V3 — ${edition}
Version: ${version}
Kernel: experimental x86-64 GenKernel
Edition profiles are metadata-only; edition-specific features are not implemented.
Enterprise LTSC is a preview label, not a long-term-support guarantee.
EOF
xorriso -as mkisofs -R -r -J \
  -b boot/limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table \
  --efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image \
  --protective-msdos-label build/iso-root -o build/GenOS-Genesis-x86_64.iso
boot/limine/limine bios-install build/GenOS-Genesis-x86_64.iso
sha256sum build/GenOS-Genesis-x86_64.iso > build/SHA256SUMS.txt
file build/GenOS-Genesis-x86_64.iso
echo 'ISO gerada. O build não substitui teste visual em VM e hardware.'
