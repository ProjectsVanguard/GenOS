# GenOS V3 — GenUI 3.0 e subsistemas nativos

Este é o plano de integração dos próximos subsistemas. **O nome GenUI 3.0 é a meta da nova interface, não uma alegação de que ela já está pronta.** Cada item só muda para concluído após integração ao kernel/ISO e teste reproduzível.

## GenUI 3.0
- [x] Núcleo independente de janelas/eventos com fila limitada e foco/mover/minimizar/fechar (`genui/genui3.c`); ainda não conectado ao framebuffer.
- [ ] Compositor framebuffer com clipping, double buffering e retângulos invalidados.
- [ ] Sistema de eventos e entrada abstraída; manter PS/2 como fallback, adicionar USB HID quando o controlador USB estiver pronto.
- [ ] Janelas com foco, movimento, minimizar/fechar e ciclo de eventos.
- [ ] Barra de tarefas, menu iniciar, terminal gráfico e painel de configurações.
- [ ] Fonte bitmap e recursos versionados; tratamento de resoluções e formatos de pixel.
- [ ] Testes automatizados para desenho, limites do framebuffer e fila de eventos.

## Sistema de arquivos
- [x] VFS inicial somente leitura em memória com caminhos limitados, diretórios implícitos, open/read/list e parser GNRD v1 (`kernel/subsystems/vfs.c`); ainda não integrado ao boot nem a um driver de bloco.
- [ ] Abstração VFS (mount/open/read/write/close/list) com validação de caminhos e limites.
- [ ] Initramfs somente leitura para os primeiros aplicativos e recursos do sistema.
- [ ] Driver de bloco e camada de armazenamento testados antes de habilitar escrita.
- [ ] FAT de leitura para mídia removível; depois, escrita transacional com testes de falha.
- [ ] Explorador de arquivos nativo integrado à VFS.

## Rede
- [x] Parsers iniciais Ethernet/IPv4 com checagem de comprimento, checksum IPv4 e rejeição de fragmentação (`kernel/subsystems/net_parse.c`); não há driver NIC nem conectividade.
- [ ] Descoberta PCI e driver NIC virtualizado (prioridade: virtio-net em QEMU).
- [ ] Buffers de pacotes com limites e validação; Ethernet e ARP.
- [ ] IPv4, ICMP, UDP e DHCP; TCP e DNS após os testes de base.
- [ ] Testes de pacotes malformados, timeouts e desconexão.
- [ ] API de sockets para aplicativos só depois de isolamento/processos.

## USB
- [x] Parser defensivo de descritores e busca de interface (`kernel/subsystems/usb_desc.c`); não controla hardware USB.
- [ ] Descoberta PCI e driver xHCI/EHCI com limites de DMA e interrupções.
- [ ] Enumeração e descritores USB validados.
- [ ] USB HID para teclado e mouse; PS/2 segue como fallback.
- [ ] Armazenamento USB somente após camada de blocos e VFS.
- [ ] Testes QEMU e documentação de controladores/dispositivos suportados.

## Aplicativos nativos
- [x] Contrato ABI proposto e protótipos host-side de Hello/calculadora (`apps/native/`); não executáveis no kernel atual.
- [ ] Definir ABI, syscalls, modo usuário e isolamento de memória.
- [ ] Carregador ELF e ciclo de vida de processos antes de executar binários de terceiros.
- [ ] APIs de UI, arquivos e rede para aplicativos.
- [ ] Primeiros apps: terminal, editor simples, calculadora, configurações e explorador.
- [ ] Loja nativa e instalação .gem somente após validação de pacotes, permissões e rollback.
- [ ] Não tratar HTML/JavaScript da demonstração web como aplicativos nativos.

## Integração e critérios de pronto
1. Um subsistema por PR/commit lógico; build freestanding com avisos tratados como erros.
2. Testes unitários host-side para estruturas puras e testes QEMU para integração.
3. Cada imagem Home/Ultimate/Enterprise-LTSC deve compilar e passar BIOS + UEFI smoke tests.
4. Nenhum recurso será anunciado como suportado apenas porque existe um arquivo ou uma interface visual.
5. Publicar checksums e logs dos testes junto das ISOs.

## Estado atual
A base atual é um kernel x86-64 experimental com framebuffer, saída serial e protótipo de menu com polling PS/2. Foram adicionados núcleos C host-testáveis para VFS/initramfs, parsing Ethernet/IPv4, validação de descritores USB e estado de janelas/eventos GenUI, além de protótipos de apps. Ainda não estão ligados ao boot nem equivalem a drivers: não há NIC funcional, controlador USB, modo usuário, syscalls, ELF loader ou compositor visual integrado. A demonstração do navegador é separada do kernel. `make subsystem-test` executa testes host-side para os componentes puros.
