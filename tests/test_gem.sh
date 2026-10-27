#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
make gem suite-gem app-store-gem
python3 tools/gem/genpkg.py validate build/org.genos.hello.gem
python3 tools/gem/genpkg.py validate build/org.genos.suite.gem
python3 tools/gem/genpkg.py validate build/org.genos.app-store.gem
python3 - <<'PY'
import zipfile
required = [
    'manifest.json', 'apps/genos-suite/index.html', 'apps/app-store/index.html',
    'apps/notepad/index.html', 'apps/calculator/index.html', 'apps/paint/index.html',
    'apps/calendar/index.html', 'apps/terminal/index.html', 'apps/settings/index.html',
    'apps/system-monitor/index.html', 'apps/clock/index.html',
    'apps/markdown-studio/index.html', 'apps/unit-converter/index.html',
    'apps/solaris-networking/index.html'
]
with zipfile.ZipFile('build/org.genos.suite.gem') as z:
    missing = [p for p in required if p not in z.namelist()]
    assert not missing, f'missing suite files: {missing}'
print('PASS: suite contém a GenOS Store e os aplicativos previstos')
PY
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
python3 tools/gem/genpkg.py install build/org.genos.hello.gem --root "$TMP/Applications"
python3 tools/gem/genpkg.py install build/org.genos.suite.gem --root "$TMP/Applications"
python3 tools/gem/genpkg.py install build/org.genos.app-store.gem --root "$TMP/Applications"
test -f "$TMP/Applications/org.genos.hello/manifest.json"
test -f "$TMP/Applications/org.genos.hello/INSTALLATION.json"
test -f "$TMP/Applications/org.genos.suite/manifest.json"
test -f "$TMP/Applications/org.genos.suite/apps/genos-suite/index.html"
test -f "$TMP/Applications/org.genos.suite/apps/app-store/index.html"
test -f "$TMP/Applications/org.genos.app-store/index.html"
python3 tools/gem/genpkg.py list --root "$TMP/Applications" | grep -q 'org.genos.app-store'
python3 tools/gem/genpkg.py info build/org.genos.app-store.gem | grep -q 'GenOS Store'
python3 tools/gem/genpkg.py remove org.genos.app-store --root "$TMP/Applications"
test ! -e "$TMP/Applications/org.genos.app-store"
echo 'PASS: pack, validate, info, install, list e remove'
