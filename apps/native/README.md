# Aplicativos nativos GenOS (estágio inicial)

Estes arquivos definem o contrato e os primeiros programas de referência; ainda não são executáveis pelo kernel atual. O kernel não possui modo usuário, tabela de syscalls nem carregador ELF integrado.

## ABI proposta v0
- Binários ELF64 x86-64, entrada _start, sem acesso direto a dispositivos.
- Syscalls futuras: write(fd, buf, len), read(fd, buf, len), open(path, flags), close(fd), exit(status).
- Argumentos e buffers serão validados no limite kernel/usuário; cada processo terá espaço de endereçamento isolado.
- Apps devem tratar erro de syscall e não presumir que rede/USB/armazenamento existem.
- Pacotes .gem só poderão ser instalados após verificação de integridade, permissões e rollback.

Os programas C em apps/native/ são protótipos host-side para validar lógica pura, não aplicativos já executáveis dentro do GenOS.
