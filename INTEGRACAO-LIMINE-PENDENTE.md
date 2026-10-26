# Integração do Limine — estado real

O GenKernel foi compilado para ELF x86-64. A configuração `boot/limine.conf` aponta para `boot():/boot/kernel.elf`.

## Dependências

- Binário oficial `boot/limine/limine`
- `boot/limine/limine-bios.sys`
- `boot/limine/BOOTX64.EFI`
- `xorriso`

Essas dependências não estão incluídas no repositório. Portanto, a ISO completa com Limine ainda não foi gerada nem validada aqui. A ISO BIOS de diagnóstico é um teste separado e não executa o GenKernel.

## Quando as dependências estiverem disponíveis

1. Obtenha uma versão oficial compatível do Limine e coloque os três arquivos nos caminhos acima.
2. Instale `xorriso`.
3. Execute `./build-iso.sh`.
4. Confira o SHA-256 e teste primeiro em uma VM com BIOS e UEFI, antes de tentar hardware real.

Revise o script contra a versão exata do Limine antes de distribuir uma imagem.
