# GenOS Genesis — GenKernel, GenUI e pacotes .gem

O GenOS é um sistema operacional experimental próprio para x86-64, não uma distribuição Linux. Este repositório contém o código-fonte inicial, ferramentas de desenvolvimento e protótipos de aplicativos.

## Terminal CMD do GenOS

<p align="center">
  <table>
    <tr>
      <td bgcolor="#0c0c0c">
        <pre><font color="#00ff66">GenOS CMD [versão 3.0.0-preview]
(c) GenOS Project. Protótipo experimental.

C:\\GenOS&gt; help
Comandos demonstrativos: help, about, clear, get-updates

C:\\GenOS&gt; about
GenOS — sistema operacional experimental x86-64

C:\\GenOS&gt; _</font></pre>
      </td>
    </tr>
  </table>
</p>

> Este bloco é uma ilustração visual do terminal; os comandos acima não são executados pelo README nem representam um shell nativo completo.

## Downloads das edições GenOS

<p align="center">
  <a href="https://github.com/ProjectsVanguard/GenOS/releases/latest"><img alt="Baixar GenOS Core" src="https://img.shields.io/badge/Download-GenOS%20Core-2563eb?style=for-the-badge&logo=github"></a>
  <a href="https://github.com/ProjectsVanguard/GenOS/releases/latest"><img alt="Baixar GenOS Ultimate" src="https://img.shields.io/badge/Download-GenOS%20Ultimate-7c3aed?style=for-the-badge&logo=github"></a>
  <a href="https://github.com/ProjectsVanguard/GenOS/releases/latest"><img alt="Baixar GenOS Enterprise" src="https://img.shields.io/badge/Download-GenOS%20Enterprise-0f766e?style=for-the-badge&logo=github"></a>
</p>

> **GenOS V3:** o workflow [Build GenOS V3 edition ISOs](https://github.com/ProjectsVanguard/GenOS/actions/workflows/build-iso-v3.yml) agora prepara ISOs separadas para Home, Ultimate e Enterprise LTSC, executa testes de compilação/pacotes e um smoke test de boot BIOS no QEMU. A release só é publicada se as três variantes passarem. Até a release V3 aparecer abaixo, considere os builds ainda em validação. As edições compartilham o mesmo kernel e payload; os nomes identificam perfis experimentais, não conjuntos de recursos distintos. Enterprise LTSC não implica garantia de suporte prolongado.

[Ver todas as releases e downloads](https://github.com/ProjectsVanguard/GenOS/releases)

## Experimentar o GenOS v2 antes de baixar

<p align="center">
  <a href="https://projectsvanguard.github.io/GenOS/core-preview/"><img alt="Experimentar GenOS v2 no navegador" src="https://img.shields.io/badge/Experimentar-GenOS%20v2%20Online-16a34a?style=for-the-badge&logo=googlechrome"></a>
</p>

A [demonstração interativa do GenOS v2](https://projectsvanguard.github.io/GenOS/core-preview/) demonstra uma área de trabalho com menu, janelas, notas, calculadora, terminal simulado, arquivos, configurações, Paint, calendário, monitor ilustrativo, relógio e pacotes .gem. **É uma simulação web:** não inicia o GenKernel, não executa a ISO nem valida a compatibilidade do sistema. A publicação depende de GitHub Pages estar habilitado e do workflow [Publish GenOS Core web preview](https://github.com/ProjectsVanguard/GenOS/actions/workflows/core-preview-pages.yml) terminar com sucesso.

A demonstração inclui o comando `get-updates` (também aceita `get-update`) no terminal. Ele consulta a última release pública e mostra atalhos para os canais de atualização do site, ISOs, pacotes e kernel. **É um verificador/atalho, não um instalador universal:** atualizações do kernel/ISO não podem ser aplicadas pelo navegador e exigem os procedimentos de build/instalação correspondentes.

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
- Última atualização automática: 2026-10-26T21:56:29-03:00 UTC — commit [`869ae63`](https://github.com/ProjectsVanguard/GenOS/commit/869ae63e8ae6afc06d710381af1387f46b9035a9) — Make terminal get-update list downloadable release assets
<!-- README-AUTO-UPDATE:END -->
