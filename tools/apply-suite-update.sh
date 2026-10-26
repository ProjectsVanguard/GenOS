#!/usr/bin/env bash
# Apply a GenOS app-suite ZIP to a mounted/test Applications directory.
# This does NOT update the kernel or make web prototypes native applications.
set -Eeuo pipefail

usage() {
  echo "Uso: $0 <GenOS-Suite-Update.zip> <diretorio-Applications>" >&2
  echo "Exemplo: $0 GenOS-Suite-Update.zip /mnt/genos/Applications" >&2
}
[[ $# -eq 2 ]] || { usage; exit 2; }
archive="$(realpath "$1")"
target="$2"
command -v unzip >/dev/null || { echo "Falta unzip." >&2; exit 2; }
[[ -f "$archive" ]] || { echo "Pacote não encontrado: $archive" >&2; exit 2; }
mkdir -p "$target"
target="$(realpath "$target")"
tmp="$(mktemp -d)"
backup=""
cleanup() { rm -rf "$tmp"; }
trap cleanup EXIT

# Reject archive paths that could write outside the temporary extraction directory.
if unzip -Z1 "$archive" | grep -Eq '(^/|(^|/)\.\.(/|$))'; then
  echo "Pacote rejeitado: caminho inseguro no ZIP." >&2
  exit 1
fi
unzip -q "$archive" -d "$tmp"
[[ -d "$tmp/apps" ]] || { echo "Pacote inválido: diretório apps/ ausente." >&2; exit 1; }

stamp="$(date -u +%Y%m%dT%H%M%SZ)"
backup="${target}.backup-${stamp}"
mkdir -p "$backup"
if [[ -n "$(find "$target" -mindepth 1 -maxdepth 1 -print -quit)" ]]; then
  cp -a "$target/." "$backup/"
fi
# The bundle's apps/* contents become Applications/*.
cp -a "$tmp/apps/." "$target/"
printf 'Suite atualizada em: %s\nBackup anterior: %s\n' "$target" "$backup"
printf 'Nota: atualização somente dos arquivos de aplicativos; kernel/ISO não foram alterados.\n'
