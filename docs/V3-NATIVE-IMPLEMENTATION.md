# GenOS V3 — implementação nativa por etapas

Este documento registra o trabalho necessário para transformar a ISO experimental em um sistema operacional utilizável. **Planejado não significa implementado.** A demonstração HTML e os pacotes .gem gerenciados no host não equivalem a aplicativos executados pelo GenKernel.

## 0. Base de boot e diagnóstico
- [x] Entrada Limine, framebuffer e console serial básico no kernel x86-64.
- [x] Smoke tests BIOS e UEFI no QEMU no workflow V3 (não são testes em hardware real).
- [ ] Validar mapa de memória Limine, configurar GDT/IDT e tratamento de exceções.
- [ ] Inicializar alocador de páginas físicas e memória virtual com testes.
- [ ] Criar CI para testes unitários de componentes do kernel e boot regressivo.

## 1. GenUI nativa
- [x] Protótipo inicial: leitura por polling do teclado PS/2 e menu framebuffer alternável com Enter/Esc (sem suporte a USB, mouse ou janelas reais; validação de build/boot pendente).
- [ ] Separar primitivas de framebuffer (pixel, retângulo, fonte, clipping) do kernel principal.
- [ ] Implementar framebuffer com stride e formatos de pixel verificados, backbuffer e atualização parcial.
- [ ] Inicializar teclado e mouse via drivers apropriados e fila de eventos.
- [ ] Implementar foco, janelas móveis, eventos, desktop, menu e terminal gráfico.
- [ ] Carregar fontes e ícones da imagem de sistema em vez de depender só de recursos embutidos.
- [ ] Testar resoluções e modos gráficos distintos no QEMU.

## 2. Armazenamento e sistema de arquivos
- [ ] Descoberta PCI e barramentos de dispositivos; abstração de blocos.
- [ ] Driver inicial de armazenamento virtualizado (por exemplo virtio-blk ou AHCI) com testes.
- [ ] Camada VFS e montagem de initramfs somente leitura.
- [ ] Leitura de FAT para mídia de boot; escrita persistente apenas depois de testes de integridade.
- [ ] Explorador de arquivos nativo, após existir VFS e interface de aplicativos.

## 3. USB, áudio e rede
- [ ] Controlador USB, enumeração e HID teclado/mouse.
- [ ] Subsistema de áudio e driver inicial para dispositivo emulado.
- [ ] Interface de rede, driver virtualizado, pilha Ethernet/IP, DHCP, TCP/UDP e DNS.
- [ ] Testes de isolamento, limites de buffers e falhas de dispositivos.

## 4. Processos e aplicativos
- [ ] Temporizador, interrupções e escalonador.
- [ ] Modo usuário, syscalls, isolamento de memória e carregador ELF.
- [ ] Runtime nativo de aplicativos e IPC.
- [ ] Terminal funcional, configurações, calculadora e editor de texto nativos.
- [ ] Gerenciador de pacotes .gem com formato, permissões, assinatura/verificação, dependências e atualização segura.
- [ ] Loja nativa integrada ao gerenciador de pacotes, com rede funcional.

## 5. Edições e publicação
- [ ] Definir recursos reais e manifestos de instalação específicos para Home, Ultimate e Enterprise LTSC.
- [ ] Incluir apenas binários e recursos construídos e testados nas ISOs correspondentes.
- [ ] Testar instalação, atualização, persistência e recuperação em VMs.
- [ ] Documentar requisitos, segurança, limitações e diferenças verificadas entre edições.

## Critérios de aceitação
Cada recurso só será marcado como pronto quando: (1) compilar para o alvo x86-64 freestanding; (2) estiver conectado ao build da ISO; (3) possuir teste automatizado ou roteiro reproduzível; (4) funcionar no QEMU sem quebrar BIOS/UEFI; (5) tiver documentação que distinga protótipo de funcionalidade real.

**Ordem recomendada:** interrupções e memória → eventos de entrada → GenUI → armazenamento/VFS → processos/syscalls → apps → rede/USB/áudio → loja e edições. Não adicionar arquivos de preenchimento para aumentar artificialmente a ISO.
