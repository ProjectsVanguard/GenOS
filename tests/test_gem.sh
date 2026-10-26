#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
make gem suite-gem
python3 tools/gem/genpkg.py validate build/org.genos.hello.gem
python3 tools/gem/genpkg.py validate build/org.genos.suite.gem
python3 -c "import zipfile; z=zipfile.ZipFile('build/org.genos.suite.gem'); required=['manifest.json','apps/genos-suite/index.html','apps/notepad/index.html','apps/calculator/index.html','apps/paint/index.html','apps/calendar/index.html','apps/terminal/index.html','apps/settings/index.html','apps/system-monitor/index.html','apps/clock/index.html','apps/markdown-studio/index.html','apps/unit-converter/index.html','apps/solaris-networking/index.html']; missing=[p for p in required if p not in z.namelist()]; assert not missing, f'missing suite files: {missing}'; print('PASS: todos os aplicativos estão no único .gem')"
TMP=$(mktemp -d)
python3 tools/gem/genpkg.py install build/org.genos.hello.gem --root "$TMP/Applications"
test -f "$TMP/Applications/org.genos.hello/manifest.json"
test -f "$TMP/Applications/org.genos.hello/INSTALLATION.json"
rm -rf "$TMP"
echo 'PASS: pack, validate e install'
