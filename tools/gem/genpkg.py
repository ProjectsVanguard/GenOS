#!/usr/bin/env python3
"""GenOS .gem package tool. Format v1 is a ZIP container, not RubyGems."""
import argparse, hashlib, json, pathlib, re, shutil, sys, tempfile, zipfile
FORMAT='gem'; VERSION=1
ID_RE=re.compile(r'^[a-z0-9]+(?:[.-][a-z0-9]+)*$')

def safe_name(name):
    p=pathlib.PurePosixPath(name)
    return bool(name) and not p.is_absolute() and '..' not in p.parts and '\\\\' not in name and not name.startswith('/')

def load_manifest(z):
    try: m=json.loads(z.read('manifest.json'))
    except Exception as e: raise ValueError(f'manifest.json ausente ou inválido: {e}')
    required=('format','format_version','id','name','version','entrypoint','architecture')
    for k in required:
        if k not in m: raise ValueError(f'campo obrigatório ausente: {k}')
    if m['format'] != FORMAT or m['format_version'] != VERSION: raise ValueError('formato .gem incompatível')
    if not ID_RE.fullmatch(m['id']): raise ValueError('id inválido')
    if not isinstance(m['architecture'],list) or not m['architecture']: raise ValueError('architecture deve ser uma lista')
    if not safe_name(m['entrypoint']): raise ValueError('entrypoint inseguro')
    names=z.namelist()
    if m['entrypoint'] not in names: raise ValueError('entrypoint não existe no pacote')
    for n in names:
        if not safe_name(n): raise ValueError(f'caminho inseguro no pacote: {n}')
    return m

def pack(src,out):
    src=pathlib.Path(src).resolve(); out=pathlib.Path(out).resolve()
    manifest=src/'manifest.json'
    if not manifest.is_file(): raise ValueError('diretório precisa conter manifest.json')
    m=json.loads(manifest.read_text())
    if m.get('format')!='gem' or m.get('format_version')!=1: raise ValueError('manifesto precisa declarar format=gem e format_version=1')
    files=[]
    for p in src.rglob('*'):
        if p.is_symlink(): raise ValueError(f'links simbólicos não permitidos: {p}')
        if p.is_file():
            rel=p.relative_to(src).as_posix()
            if not safe_name(rel): raise ValueError(f'caminho inseguro: {rel}')
            files.append((p,rel))
    if not any(rel==m.get('entrypoint') for _,rel in files): raise ValueError('entrypoint não encontrado')
    out.parent.mkdir(parents=True,exist_ok=True)
    with zipfile.ZipFile(out,'w',zipfile.ZIP_DEFLATED) as z:
        for p,rel in sorted(files,key=lambda x:x[1]): z.write(p,rel)
    validate(out)
    print(f'Pacote criado: {out} ({out.stat().st_size} bytes)')

def read_package(path):
    with zipfile.ZipFile(path) as z: return load_manifest(z)

def validate(path):
    m=read_package(path)
    print(json.dumps({'valid':True,'id':m['id'],'name':m['name'],'version':m['version'],'entrypoint':m['entrypoint']},ensure_ascii=False,indent=2))

def install(path,dest):
    path=pathlib.Path(path).resolve()
    dest=pathlib.Path(dest).resolve()
    with zipfile.ZipFile(path) as z:
        m=load_manifest(z)
        target=(dest/m['id']).resolve()
        if dest not in target.parents: raise ValueError('destino fora do diretório de aplicativos')
        if target.exists(): raise ValueError(f'aplicativo já instalado: {target}')
        dest.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='.gem-install-',dir=dest) as td:
            tmp=pathlib.Path(td)/'app'; tmp.mkdir()
            for info in z.infolist():
                if not safe_name(info.filename): raise ValueError(f'caminho inseguro: {info.filename}')
                out=(tmp/info.filename).resolve()
                if tmp.resolve() not in out.parents and out != tmp.resolve(): raise ValueError('tentativa de path traversal')
                if info.is_dir(): out.mkdir(parents=True,exist_ok=True)
                else:
                    out.parent.mkdir(parents=True,exist_ok=True)
                    with z.open(info) as src, open(out,'wb') as dst: shutil.copyfileobj(src,dst)
            (tmp/'INSTALLATION.json').write_text(json.dumps({'installed_from':path.name,'sha256':hashlib.sha256(path.read_bytes()).hexdigest()},indent=2))
            tmp.rename(target)
    print(f"Instalado {m['name']} em {target}")

def list_installed(root):
    root=pathlib.Path(root).resolve()
    if not root.exists():
        print(f'Nenhum aplicativo instalado em {root}')
        return
    found=0
    for manifest in sorted(root.glob('*/manifest.json')):
        try:
            m=json.loads(manifest.read_text(encoding='utf-8'))
            if not (manifest.parent/'INSTALLATION.json').is_file(): continue
            print(f"{m.get('id','?')}\t{m.get('version','?')}\t{m.get('name','?')}")
            found+=1
        except (OSError, ValueError):
            print(f'AVISO: manifesto inválido: {manifest}',file=sys.stderr)
    if not found: print(f'Nenhum aplicativo instalado em {root}')

def remove(app_id,root):
    if not ID_RE.fullmatch(app_id): raise ValueError('id inválido')
    root=pathlib.Path(root).resolve()
    target=(root/app_id).resolve()
    if root not in target.parents: raise ValueError('destino fora do diretório de aplicativos')
    marker=target/'INSTALLATION.json'
    if not marker.is_file() or not (target/'manifest.json').is_file():
        raise ValueError(f'aplicativo instalado/gerenciado não encontrado: {app_id}')
    # Refuse to follow symlinked installation targets.
    if target.is_symlink(): raise ValueError('não é permitido remover destino que seja link simbólico')
    shutil.rmtree(target)
    print(f'Removido {app_id} de {target}')

def main():
    p=argparse.ArgumentParser(description='Gerenciador experimental de pacotes GenOS .gem')
    s=p.add_subparsers(dest='cmd',required=True)
    a=s.add_parser('pack'); a.add_argument('source'); a.add_argument('-o','--output',required=True)
    a=s.add_parser('validate'); a.add_argument('package')
    a=s.add_parser('info'); a.add_argument('package')
    a=s.add_parser('install'); a.add_argument('package'); a.add_argument('--root',default='./Applications')
    a=s.add_parser('list'); a.add_argument('--root',default='./Applications')
    a=s.add_parser('remove'); a.add_argument('app_id'); a.add_argument('--root',default='./Applications')
    x=p.parse_args()
    try:
        if x.cmd=='pack': pack(x.source,x.output)
        elif x.cmd=='validate': validate(x.package)
        elif x.cmd=='info': print(json.dumps(read_package(x.package),ensure_ascii=False,indent=2))
        elif x.cmd=='install': install(x.package,x.root)
        elif x.cmd=='list': list_installed(x.root)
        elif x.cmd=='remove': remove(x.app_id,x.root)
    except (ValueError, OSError, zipfile.BadZipFile, KeyError) as e:
        print(f'ERRO: {e}',file=sys.stderr); return 2
    return 0
if __name__=='__main__': sys.exit(main())
