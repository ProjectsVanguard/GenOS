#!/usr/bin/env bash
# Install a previously verified GenOS kernel update into a mounted boot directory.
# This is an offline installer, not an in-OS updater. A reboot is required.
set -Eeuo pipefail

usage() {
  echo "Uso: $0 <GenOS-Kernel-Security-Update.zip> <diretorio-boot>" >&2
  echo "Exemplo: sudo bash $0 GenOS-Kernel-Security-Update.zip /mnt/genos/boot" >&2
}
[[ $# -eq 2 ]] || { usage; exit 2; }
archive="$(realpath "$1")"
boot_dir="$2"
command -v unzip >/dev/null || { echo "Falta unzip." >&2; exit 2; }
[[ -f "$archive" ]] || { echo "Pacote não encontrado: $archive" >&2; exit 2; }
[[ -d "$boot_dir" ]] || { echo "O diretório boot deve existir e estar montado: $boot_dir" >&2; exit 2; }
boot_dir="$(realpath "$boot_dir")"
[[ -f "$boot_dir/kernel.elf" ]] || { echo "kernel.elf atual não encontrado em $boot_dir; recusando instalar." >&2; exit 1; }

tmp="$(mktemp -d)"
cleanup() { rm -rf "$tmp"; }
trap cleanup EXIT
if unzip -Z1 "$archive" | grep -Eq '(^/|(^|/)\.\.(/|$))'; then
  echo "Pacote rejeitado: caminho inseguro no ZIP." >&2
  exit 1
fi
unzip -q "$archive" -d "$tmp"
candidate="$tmp/boot/kernel.elf"
manifest="$tmp/update-manifest.json"
[[ -s "$candidate" && -s "$manifest" ]] || { echo "Pacote incompleto." >&2; exit 1; }
grep -q '"mandatory"[[:space:]]*:[[:space:]]*true' "$manifest" || { echo "Atualização não marcada como obrigatória; recusando." >&2; exit 1; }
command -v file >/dev/null && file "$candidate" | grep -q 'ELF 64-bit' || { echo "Kernel candidato não é um ELF 64-bit reconhecido." >&2; exit 1; }

stamp="$(date -u +%Y%m%dT%H%M%SZ)"
backup="$boot_dir/kernel.elf.backup-$stamp"
install -m 0644 "$boot_dir/kernel.elf" "$backup"
install -m 0644 "$candidate" "$boot_dir/kernel.elf.new"
mv -f "$boot_dir/kernel.elf.new" "$boot_dir/kernel.elf"
echo "Kernel atualizado em: $boot_dir/kernel.elf"
echo "Backup anterior: $backup"
echo "Reinicie GenOS para carregar o kernel novo. Se houver falha de boot, restaure o backup."
