# GenOS Genesis — GenKernel, GenUI e pacotes .gem

O GenOS é um projeto experimental de sistema operacional próprio para x86-64, não uma distribuição Linux. Este repositório contém o código-fonte inicial e ferramentas de desenvolvimento.

## Estado atual — leia antes de testar

- O kernel C freestanding compila para ELF x86-64.
- O código solicita framebuffer ao bootloader Limine e desenha uma tela estática de demonstração.
- A ferramenta Python cria, valida e instala pacotes `.gem` experimentais.
- Existe um teste de inicialização BIOS separado que mostra `BIOS boot path OK`. **Esse teste não carrega o GenKernel.**
- A ISO completa com Limine ainda não foi construída ou validada neste projeto. Não trate a ISO de diagnóstico como uma ISO funcional do sistema.

## Requisitos

GCC ou Clang x86-64, binutils, Python 3.10+; para gerar a ISO completa, também são necessários os binários oficiais do Limine e `xorriso`.

```sh
make
make test
```

O ELF é gerado em `build/kernel.elf`; o pacote de exemplo em `build/org.genos.hello.gem`.

## Ferramenta `.gem`

```sh
python3 tools/gem/genpkg.py pack apps/hello -o build/hello.gem
python3 tools/gem/genpkg.py validate build/hello.gem
python3 tools/gem/genpkg.py install build/hello.gem --root ./Applications
```

`.gem` é um contêiner ZIP experimental com `manifest.json`, não é RubyGem e ainda não é executado pelo kernel. Não instale pacotes desconhecidos.

## Integração do Limine

Veja [`INTEGRACAO-LIMINE-PENDENTE.md`](INTEGRACAO-LIMINE-PENDENTE.md) e `build-iso.sh`. O script requer `boot/limine/limine`, `boot/limine/limine-bios.sys`, `boot/limine/BOOTX64.EFI` e `xorriso`.

## Limitações

Ainda não há escalonador, memória virtual própria, syscalls, processos em modo usuário, sistema de arquivos, rede, teclado/mouse ou compositor de janelas. GenUI é somente uma tela estática no framebuffer. O pacote `.gem` não tem assinatura, resolução de dependências, sandbox ou runtime nativo; não há suporte a `.exe`/Wine.

## Próximas etapas

1. Integrar e validar Limine em BIOS e UEFI.
2. Testar boot do kernel em máquina virtual antes de hardware real.
3. Implementar exceções, temporizador, teclado e gerência de memória.
4. Criar sistema de arquivos, ABI e carregador de programas de usuário.
5. Evoluir GenUI para compositor e eventos.
6. Definir segurança, assinatura e runtime dos pacotes `.gem`.
