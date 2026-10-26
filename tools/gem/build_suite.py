#!/usr/bin/env python3
"""Build the complete web app suite as one experimental GenOS .gem package."""
import json
import pathlib
import shutil
import sys
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "gem"))
import genpkg  # noqa: E402

MANIFEST = {
    "format": "gem",
    "format_version": 1,
    "id": "org.genos.suite",
    "name": "GenOS Suite",
    "version": "1.0.0",
    "description": "Pacote único com o launcher e todos os aplicativos web da suite GenOS.",
    "author": "Projeto GenOS",
    "entrypoint": "apps/genos-suite/index.html",
    "architecture": ["x86_64"],
    "min_genos_version": "0.1.0",
    "permissions": [],
    "runtime": "web-prototype",
    "notes": "Contém protótipos HTML; o GenKernel atual não os executa nativamente."
}


def main():
    if len(sys.argv) != 2:
        raise SystemExit("Uso: build_suite.py <arquivo-saida.gem>")
    output = pathlib.Path(sys.argv[1]).resolve()
    apps = ROOT / "apps"
    if not (apps / "genos-suite" / "index.html").is_file():
        raise SystemExit("Launcher apps/genos-suite/index.html não encontrado")
    with tempfile.TemporaryDirectory(prefix="genos-suite-gem-") as tmp:
        stage = pathlib.Path(tmp)
        shutil.copytree(apps, stage / "apps")
        (stage / "manifest.json").write_text(
            json.dumps(MANIFEST, ensure_ascii=False, indent=2) + "\\n",
            encoding="utf-8",
        )
        genpkg.pack(stage, output)


if __name__ == "__main__":
    main()
