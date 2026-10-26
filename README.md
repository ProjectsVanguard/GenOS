# GenOS Genesis — GenKernel, GenUI e pacotes .gem

O GenOS é um sistema operacional experimental próprio para x86-64, não uma distribuição Linux. Este repositório contém o código-fonte inicial e ferramentas de desenvolvimento.

## Gerar a ISO de teste

O GitHub Actions tenta construir automaticamente uma ISO híbrida BIOS/UEFI a cada alteração relevante na branch `main`.

1. Abra a aba **Actions**: https://github.com/ProjectsVanguard/GenOS/actions
2. Entre no fluxo **Build GenOS test ISO**.
3. Abra a execução mais recente com status verde.
4. Em **Artifacts**, baixe `GenOS-Genesis-x86_64-ISO` e extraia o ZIP para obter a ISO e o SHA-256.

Também é possível iniciar uma compilação manual em **Actions → Build GenOS test ISO → Run workflow**.

A imagem deve ser considerada uma **ISO de teste experimental**. O fluxo compila o GenKernel, gera a ISO com Limine oficial e verifica o arquivo/checksum. A conclusão do build não substitui um teste visual em QEMU/VirtualBox e em hardware real. Teste primeiro em máquina virtual. A ISO não instala o GenOS nem deve ser usada para particionar o disco interno.

## Estado atual

- O kernel C freestanding para x86-64 solicita framebuffer ao Limine e desenha uma tela estática de demonstração.
- A ferramenta Python cria, valida e instala pacotes `.gem` experimentais.
- A ISO antiga que mostra `BIOS boot path OK` é somente diagnóstico e não carrega o GenKernel.
- O build automatizado gera uma imagem para teste; compatibilidade com cada computador ainda precisa ser verificada.

## Requisitos para build local

GCC, binutils, Python 3.10+, `xorriso` e os binários oficiais do Limine em `boot/limine/`.

```sh
make clean test
./build-iso.sh
```

O ELF é gerado em `build/kernel.elf`; a ISO em `build/GenOS-Genesis-x86_64.iso`.

## Ferramenta `.gem`

```sh
python3 tools/gem/genpkg.py pack apps/hello -o build/hello.gem
python3 tools/gem/genpkg.py validate build/hello.gem
python3 tools/gem/genpkg.py install build/hello.gem --root ./Applications
```

`.gem` é um contêiner ZIP experimental com `manifest.json`, não é RubyGem e ainda não é executado pelo kernel. Não instale pacotes desconhecidos.

## Limitações conhecidas

Ainda não há escalonador, gerência de memória virtual própria, syscalls, processos em modo usuário, sistema de arquivos, rede, teclado/mouse ou compositor de janelas. GenUI é uma tela estática no framebuffer. O pacote `.gem` não tem assinatura, resolução de dependências, sandbox ou runtime nativo; não há suporte a `.exe`/Wine.
