# GenOS Genesis — GenKernel, GenUI e pacotes .gem

## GenOS Store e gerenciador de pacotes .gem

A loja de aplicativos web está em [apps/app-store](apps/app-store/) e foi adicionada à [GenOS Suite](apps/genos-suite/index.html) e ao desktop de demonstração. Para gerar e gerenciar o pacote da loja no computador de desenvolvimento:

```sh
make app-store-gem
python3 tools/gem/genpkg.py validate build/org.genos.app-store.gem
python3 tools/gem/genpkg.py info build/org.genos.app-store.gem
python3 tools/gem/genpkg.py install build/org.genos.app-store.gem --root ./Applications
python3 tools/gem/genpkg.py list --root ./Applications
python3 tools/gem/genpkg.py remove org.genos.app-store --root ./Applications
```

O gerenciador extrai pacotes para um diretório de aplicativos controlado e oferece listar/remover instalações gerenciadas. **Limites atuais:** isso ainda roda no host de desenvolvimento; não está integrado ao kernel bare-metal. O botão “Instalar” na loja web salva apenas uma seleção no navegador. Os pacotes .gem não têm assinatura criptográfica nem sandbox, e os apps HTML não são executáveis nativos. Instale apenas pacotes confiáveis. O workflow [Validate GenOS Store and .gem packages](.github/workflows/validate-app-store.yml) testa o ciclo do pacote.


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

## Compatibilidade de arquitetura e firmware

| Plataforma | Estado atual |
|---|---|
| AMD64 / x86-64 | Arquitetura do kernel atual; alvo de build das ISOs V3. |
| BIOS legado (AMD64) | Teste de inicialização automatizado no QEMU. |
| UEFI x86-64 | Smoke test de inicialização aprovado no QEMU no workflow V3 concluído com sucesso; ainda não validado em hardware físico. |
| ARM / AArch64 | **Ainda não suportado pelo kernel.** Exige port de arquitetura, linker/entrada de kernel, acesso a serial/console e uma imagem/boot chain próprios; uma ISO AMD64 não inicializa em ARM. |

 > AMD64 é o nome da arquitetura x86-64 e não significa apenas processadores AMD: CPUs Intel 64 também pertencem a essa família. BIOS e UEFI são modos de firmware, não arquiteturas de CPU. O smoke test UEFI foi aprovado no QEMU; isso não representa validação em hardware físico.

## Downloads das edições GenOS

<p align="center">
  <a href="https://github.com/ProjectsVanguard/GenOS/releases/latest"><img alt="Baixar ISO mais recente do GenOS Home" src="https://img.shields.io/badge/Download-GenOS%20Home-2563eb?style=for-the-badge&logo=github"></a>
  <a href="https://github.com/ProjectsVanguard/GenOS/releases/latest"><img alt="Baixar ISO mais recente do GenOS Ultimate" src="https://img.shields.io/badge/Download-GenOS%20Ultimate-7c3aed?style=for-the-badge&logo=github"></a>
  <a href="https://github.com/ProjectsVanguard/GenOS/releases/latest"><img alt="Baixar ISO mais recente do GenOS Enterprise LTSC" src="https://img.shields.io/badge/Download-GenOS%20Enterprise%20LTSC-0f766e?style=for-the-badge&logo=github"></a>
</p>

Os botões abrem a página da **release mais recente**, onde você pode baixar os arquivos que já foram publicados. Os links diretos para arquivos V3 retornavam “página não encontrada” porque esses nomes ainda não estão disponíveis em uma release. Depois que o build V3 publicar as três ISOs, os links diretos poderão ser ativados com os nomes confirmados dos arquivos.

 > **GenOS V3:** o workflow [Build GenOS V3 edition ISOs](https://github.com/ProjectsVanguard/GenOS/actions/workflows/build-iso-v3.yml) foi concluído com sucesso na [execução 40709805588](https://github.com/ProjectsVanguard/GenOS/actions/runs/40709805588). As três variantes — Home, Ultimate e Enterprise LTSC — passaram pela compilação, testes de pacotes, verificações de integração da GenUI e smoke tests BIOS/UEFI no QEMU. A publicação depende de todas as variantes passarem. ARM/AArch64 continua sem suporte. As edições compartilham o mesmo kernel e payload; os nomes identificam perfis experimentais, não conjuntos de recursos distintos. Enterprise LTSC não implica garantia de suporte prolongado.

[Ver todas as releases e downloads](https://github.com/ProjectsVanguard/GenOS/releases)

## Experimentar o GenOS V3 antes de baixar

<p align="center">
  <a href="https://projectsvanguard.github.io/GenOS/core-preview/"><img alt="Experimentar GenOS V3 no navegador" src="https://img.shields.io/badge/Experimentar-GenOS%20V3%20Online-16a34a?style=for-the-badge&logo=googlechrome"></a>
</p>

A [demonstração interativa do GenOS V3](https://projectsvanguard.github.io/GenOS/core-preview/) demonstra uma área de trabalho com menu, janelas, notas, calculadora, terminal simulado, arquivos, configurações, Paint, calendário, monitor ilustrativo, relógio e pacotes .gem. **É uma simulação web:** não inicia o GenKernel, não executa a ISO nem valida a compatibilidade do sistema. A publicação depende de GitHub Pages estar habilitado e do workflow [Publish GenOS Core web preview](https://github.com/ProjectsVanguard/GenOS/actions/workflows/core-preview-pages.yml) terminar com sucesso.

A demonstração inclui o comando `get-updates` (também aceita `get-update`) no terminal. Ele consulta a última release pública e mostra atalhos para os canais de atualização do site, ISOs, pacotes e kernel. **É um verificador/atalho, não um instalador universal:** atualizações do kernel/ISO não podem ser aplicadas pelo navegador e exigem os procedimentos de build/instalação correspondentes.

## Gerar a ISO de teste

O GitHub Actions tenta construir automaticamente uma ISO híbrida BIOS/UEFI a cada alteração relevante na branch `main`.

1. Abra a aba **Actions**: https://github.com/ProjectsVanguard/GenOS/actions
2. Entre no fluxo **Build GenOS test ISO**.
3. Abra a execução mais recente com status verde.
4. Em **Artifacts**, baixe `GenOS-Genesis-x86_64-ISO` e extraia o ZIP para obter a ISO e o SHA-256.

Também é possível iniciar uma compilação manual em **Actions → Build GenOS test ISO → Run workflow**.

A imagem deve ser considerada uma **ISO de teste experimental**. O fluxo compila o GenKernel, gera a ISO com Limine e verifica o arquivo/checksum. A conclusão do build não substitui um teste visual completo em QEMU/VirtualBox e em hardware real. Teste primeiro em máquina virtual. A ISO não instala o GenOS nem deve ser usada para particionar o disco interno.

## GenUI 3.0, sistema de arquivos, rede, USB e aplicativos nativos

O roteiro de evolução está em [GenUI 3.0 e subsistemas nativos](docs/GENUI-3.0-NATIVE-SUBSYSTEMS.md). Ele organiza o trabalho para a nova interface nativa, VFS/sistema de arquivos, rede, USB e runtime de aplicativos nativos.

**Estado real:** GenUI 3.0 continua em desenvolvimento. Foram adicionados núcleos C host-testáveis: VFS em memória somente leitura e parser de initramfs, parser Ethernet/IPv4, validação de descritores USB, modelo de janelas/eventos e protótipos de aplicativos. Eles ainda não estão conectados ao boot nem são drivers de hardware: não há conectividade de rede, controlador USB funcional, compositor GenUI integrado, syscalls, modo usuário ou carregador ELF. A demonstração web e os pacotes .gem no host não substituem esses componentes. Os testes host-side estão disponíveis em `make subsystem-test`; suporte real só será declarado após integração e testes QEMU/hardware.

## Estado atual

- O kernel C freestanding para x86-64 solicita framebuffer ao Limine e desenha uma tela experimental. O protótipo de GenUI inclui menu com navegação por teclado PS/2 em polling; a interação ainda precisa de validação visual/runtime dedicada no QEMU e não é uma interface completa.
- Existe uma proposta de aplicativos iniciais em C para framebuffer/teclado legado na [PR de runtime nativo](https://github.com/ProjectsVanguard/GenOS/pull/2). Enquanto a PR não for integrada e o build/teste não passar, não considere esses aplicativos parte da versão estável.
- A ferramenta Python cria, valida e instala pacotes `.gem` experimentais.
- A suíte GenOS Suite reúne os protótipos web em um único pacote `.gem`; esses apps usam runtime web e não são executados nativamente pelo kernel.
- O workflow V3 [passou na execução 40709805588](https://github.com/ProjectsVanguard/GenOS/actions/runs/40709805588), incluindo smoke tests de inicialização BIOS e UEFI no QEMU para as três edições. Compatibilidade com computadores físicos ainda precisa ser verificada.

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

Ainda faltam componentes fundamentais de um sistema operacional de uso geral, incluindo escalonador completo, gerência de memória virtual própria, syscalls e processos em modo usuário, integração da VFS com armazenamento, drivers de rede/USB e runtime nativo de aplicativos. Os núcleos host-testáveis atuais não significam suporte a dispositivos físicos. GenUI permanece experimental. O pacote `.gem` não tem assinatura, resolução de dependências ou sandbox. Não há suporte funcional a executáveis Windows `.exe`/Wine.

## Manutenção deste README

A seção abaixo é atualizada automaticamente pelo GitHub Actions quando alterações relevantes são enviadas para `main`. Para mudanças funcionais, atualize também manualmente as seções de estado, build e limitações, para que a documentação descreva o comportamento real — não apenas a existência de um commit.

<!-- README-AUTO-UPDATE:START -->
- Última atualização automática: 2026-10-28T12:02:50-03:00 UTC — commit [`6cad0ae`](https://github.com/ProjectsVanguard/GenOS/commit/6cad0aec9683d888b4acc0e8f3aad9e4d9e89722) — Install Limine UEFI loader required by ISO builder
<!-- README-AUTO-UPDATE:END -->
