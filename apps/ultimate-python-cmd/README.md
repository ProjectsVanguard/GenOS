# Python CMD — GenOS Ultimate

Aplicativo experimental de terminal Python para a edição Ultimate do GenOS V3.

- Interface de terminal no navegador.
- Usa Pyodide/WebAssembly para executar código Python de verdade no navegador, quando o runtime externo carrega.
- Inclui comandos auxiliares `help()`, `about()`, `clear()` e `get_updates()`.
- Requer conexão para baixar Pyodide na primeira inicialização; não é um interpretador nativo do kernel.
- O código roda isolado no runtime WebAssembly do navegador e não tem acesso direto ao sistema de arquivos do host.

A ISO Ultimate inclui este app em `/Applications/ultimate-python-cmd/`. Home e Enterprise LTSC não o incluem por padrão.