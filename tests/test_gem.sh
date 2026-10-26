#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
make gem
python3 tools/gem/genpkg.py validate build/org.genos.hello.gem
TMP=$(mktemp -d)
python3 tools/gem/genpkg.py install build/org.genos.hello.gem --root "$TMP/Applications"
test -f "$TMP/Applications/org.genos.hello/manifest.json"
test -f "$TMP/Applications/org.genos.hello/INSTALLATION.json"
rm -rf "$TMP"
echo 'PASS: pack, validate e install'
