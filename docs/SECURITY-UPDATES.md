# Atualizações obrigatórias de segurança do kernel sem ISO

Este canal compila o `build/kernel.elf` e publica apenas o kernel e um manifesto. Não gera nem substitui uma ISO.

## Correção incluída

O kernel valida os dados do framebuffer fornecidos pelo bootloader antes de desenhar: endereço não nulo, 32 bits por pixel, dimensões não nulas e limitadas a 8192×8192, pitch suficiente e alinhado a 4 bytes. Se os dados forem inválidos, interrompe a inicialização em vez de renderizar usando parâmetros inconsistentes.

Isto é uma correção defensiva específica; **não representa uma auditoria completa nem garante que o kernel esteja seguro**.

## Baixar e verificar

1. Abra [as execuções do workflow de segurança](https://github.com/ProjectsVanguard/GenOS/actions/workflows/publish-security-update.yml).
2. Baixe o artifact mais recente `GenOS-Kernel-Security-Update-<número>).
3. Extraia o artifact e o ZIP interno.
4. No diretório com `GenOS-Kernel-Security-Update.zip`, valide o checksum do pacote usando o arquivo `.sha256` correspondente: `sha256sum -c GenOS-Kernel-Security-Update.zip.sha256`.
5. Extraia o pacote e valide os arquivos de payload conforme `SHA256SUMS.txt` (execute `sha256sum -c SHA256SUMS.txt` dentro da pasta de payload extraída).

## Instalar sem reconstruir a ISO

Monte a partição de boot do GenOS em um ambiente Linux e confirme que ela contém `kernel.elf`. Em seguida, execute como administrador:

```bash
sudo bash tools/apply-kernel-update.sh GenOS-Kernel-Security-Update.zip /mnt/genos/boot
```

O script guarda uma cópia do kernel anterior, substitui somente `kernel.elf` e exige um manifesto com `mandatory: true`. Reinicie para carregar o novo kernel. Mantenha um meio de recuperação disponível.

## Limitações importantes

- **Obrigatoriedade declarada, não aplicada automaticamente:** o manifesto e o instalador identificam a atualização como obrigatória, mas o GenOS ainda não tem serviço de atualização em execução, verificação automática de política, assinatura criptográfica do pacote ou mecanismo de rollback no boot.
- O workflow gera um ELF para instalação offline; ele não atualiza um sistema em execução e não modifica o kernel já carregado na memória.
- SHA-256 detecta divergência de conteúdo, mas não prova a identidade do publicador. Use apenas artifacts do repositório oficial e, para implantação real, implemente assinatura e verificação de confiança.
- Não é gerada outra ISO; o artefato é separado e exige reinicialização.
