# GenOS Store

Loja web experimental para navegar pelos aplicativos web do GenOS.

## Gerar e instalar o pacote .gem

```sh
make app-store-gem
python3 tools/gem/genpkg.py validate build/org.genos.app-store.gem
python3 tools/gem/genpkg.py info build/org.genos.app-store.gem
python3 tools/gem/genpkg.py install build/org.genos.app-store.gem --root ./Applications
python3 tools/gem/genpkg.py list --root ./Applications
python3 tools/gem/genpkg.py remove org.genos.app-store --root ./Applications
```

O instalador extrai os arquivos para uma pasta Applications gerenciada e impede IDs duplicados e caminhos de extração inseguros. Ele não instala no kernel bare-metal, não executa HTML nativamente e não verifica assinaturas criptográficas. Só instale pacotes confiáveis.

## Limites

- “Instalar” na interface web mantém uma lista no localStorage deste navegador.
- “Abrir” inicia os protótipos web existentes.
- Não há ainda backend de loja, catálogo remoto de pacotes, assinaturas, sandbox, dependências ou atualização automática.
- A integração com o desktop é via preview web, não uma aplicação nativa do GenKernel.
