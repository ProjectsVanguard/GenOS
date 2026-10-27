# GenOS V3 — instalador Calamares (preparação)

> **Estado: scaffold de integração, ainda não é um instalador inicializável nem validado. Não use para particionar ou instalar em um disco real.**

## Arquitetura proposta

1. Inicializar uma mídia live Linux de manutenção/instalação.
2. Executar o Calamares nessa sessão live.
3. Mostrar etapas de idioma, teclado, particionamento, resumo e confirmação.
4. Instalar os artefatos do GenOS apenas quando houver um payload de sistema instalado definido e testado.
5. Validar o resultado em uma VM descartável, primeiro com disco virtual vazio, depois com reinicialização UEFI/BIOS.

O GenOS atual inicializa seu próprio kernel x86-64 via Limine. Ele **não é uma distribuição Linux**, não fornece um root filesystem Linux que o Calamares possa instalar e ainda não tem uma especificação completa de layout de disco, bootloader instalado, atualizações e recuperação. O Calamares não pode ser executado diretamente sobre o GenKernel atual.

## Pré-requisitos antes de habilitar uma instalação real

- Definir o layout de disco e partições GPT/EFI suportadas.
- Definir a árvore do sistema GenOS instalada e seus arquivos essenciais.
- Implementar um módulo de instalação idempotente que copie e valide somente os artefatos GenOS esperados.
- Instalar/configurar Limine no destino e criar entradas de boot testáveis em BIOS e UEFI.
- Ter estratégia de falha/rollback e validar que o destino selecionado é o disco correto.
- Criar mídia live Linux licenciada, reproduzível e com Calamares e dependências empacotados.
- Testar instalação, reinicialização, atualização e recuperação em discos virtuais descartáveis.

## Configuração

- `calamares/settings.conf`: lista inicial de módulos e fluxo visual.
- `calamares/modules.conf`: diretório de configurações dos módulos.
- A lista é um ponto de partida para uma mídia live Linux; a configuração precisa ser revisada contra a versão de Calamares empacotada na mídia antes de uso.
- Nenhum comando destrutivo de particionamento ou instalação do GenOS é implementado neste scaffold.

## Testes

O workflow `Validate GenOS V3 Calamares scaffold` verifica a estrutura e os requisitos documentados. Isso **não** prova que o Calamares abre, que a ISO live inicializa ou que o GenOS pode ser instalado. Esses testes só podem ser marcados como aprovados após criar e executar a mídia live de instalação real.
