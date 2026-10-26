# GenOS Genesis — GenKernel, GenUI e pacotes .gem

O GenOS é um sistema operacional experimental próprio para x86-64, não uma distribuição Linux. Este repositório contém o código-fonte inicial, ferramentas de desenvolvimento e protótipos de aplicativos.

## Gerar a ISO de teste

O GitHub Actions tenta construir automaticamente uma ISO híbrida BIOS/UEFI a cada alteração relevante na branch `main`.

1. Abra a aba **Actions**: https://github.com/ProjectsVanguard/GenOS/actions
2. Entre no fluxo **Build GenOS test ISO**.
3. Abra a execução mais recente com status verde.
4. Em **Artifacts**, baixe `GenOS-Genesis-x86_64-ISO` e extraia o ZIP para obter a ISO e o SHA-256.

Também é possível iniciar uma compilação manual em **Actions → Build GenOS test ISO → Run workflow**.

A imagem deve ser considerada uma **ISO de teste experimental**. O fluxo compila o GenKernel, gera a ISO com Limine e verifica o arquivo/checksum. A conclusão do build não substitui um teste visual em QEMU/VirtualBox e em hardware real. Teste primeiro em máquina virtual. A ISO não instala o GenOS nem deve ser usada para particionar o disco interno.

## Estado atual

- O kernel C freestanding para x86-64 solicita framebuffer ao Limine e desenha uma tela de demonstração.
- Existe uma proposta de aplicativos iniciais em C para framebuffer/teclado legado na [PR de runtime nativo](https://github.com/ProjectsVanguard/GenOS/pull/2). Enquanto a PR não for integrada e o build/teste não passar, não considere esses aplicativos parte da versão estável.
- A ferramenta Python cria, valida e instala pacotes `.gem` experimentais.
- A suíte GenOS Suite reúne os protótipos web em um único pacote `.gem`; esses apps usam runtime web e não são executados nativamente pelo kernel.
- O build automatizado gera imagens para teste; compatibilidade com cada computador ainda precisa ser verificada.

## Requisitos para build local

GCC, binutils, Python 3.10+, `xorriso` e os binários oficiais do Limine em `boot/limine/`.

```sh
make clean test
./build-iso.sh
```

O ELF é gerado em `build/kernel.elf`; a ISO em `build/GenOS-Genesis-x86_64.iso`.

## Ferramenta e suíte `.gem`

Criar, validar e instalar um pacote:

```sh
python3 tools/gem/genpkg.py pack apps/hello -o build/hello.gem
python3 tools/gem/genpkg.py validate build/hello.gem
python3 tools/gem/genpkg.py install build/hello.gem --root ./Applications
```

Construir a suíte unificada:

```sh
make suite-gem
```

O pacote resultante é `build/org.genos.suite.gem`. Consulte [o guia da suíte](docs/GENOS-SUITE-GEM.md) e a [documentação de atualizações da suíte](docs/SUITE-UPDATE.md).

`.gem` é um contêiner ZIP experimental com `manifest.json`, não é RubyGem e ainda não é executado pelo kernel. Não instale pacotes desconhecidos.

## Atualizações e Wine

- Atualizações da suíte web são publicadas como artefatos do GitHub Actions; isso não significa que os apps sejam nativos.
- O fluxo de atualização de segurança empacota o kernel para instalação offline e exige reinicialização; não é hot-patching nem garante que um kernel em execução aplique a atualização automaticamente. Consulte [atualizações de segurança](docs/SECURITY-UPDATES.md).
- O plano para portar Wine está documentado em [NATIVE-WINE-PORT.md](docs/NATIVE-WINE-PORT.md). Wine ainda **não está instalado nem é executável no GenOS**.

## Limitações conhecidas

Ainda faltam componentes fundamentais de um sistema operacional de uso geral, incluindo escalonador completo, gerência de memória virtual própria, syscalls e processos em modo usuário, sistema de arquivos, rede e runtime nativo de aplicativos. GenUI permanece experimental. O pacote `.gem` não tem assinatura, resolução de dependências ou sandbox. Não há suporte funcional a executáveis Windows `.exe`/Wine.

## Manutenção deste README

A seção abaixo é atualizada automaticamente pelo GitHub Actions quando alterações relevantes são enviadas para `main`. Para mudanças funcionais, atualize também manualmente as seções de estado, build e limitações, para que a documentação descreva o comportamento real — não apenas a existência de um commit.

<!-- README-AUTO-UPDATE:START -->
- Última atualização automática: 2026-10-26T20:07:00-03:00 UTC — commit [`dc1c55f`](https://github.com/ProjectsVanguard/GenOS/commit/dc1c55faea83b153804673198f7ebd3dff0d4bf4) — ci: add tested ISO v2 builds for current and legacy editions
<!-- README-AUTO-UPDATE:END -->
