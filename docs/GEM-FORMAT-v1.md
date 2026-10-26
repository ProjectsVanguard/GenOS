# Especificação experimental GenOS Executable Module (.gem), v1

`.gem` é um arquivo ZIP com caminhos POSIX e `manifest.json` na raiz. Não é RubyGem. A extensão não torna o conteúdo confiável nem significa que o pacote possa ser executado no kernel atual.

Campos obrigatórios: `format`=`gem`, `format_version`=1, `id`, `name`, `version`, `entrypoint`, `architecture` (lista). Campos recomendados: `description`, `author`, `min_genos_version`, `permissions`.

Regras implementadas pela ferramenta: rejeitar caminhos absolutos, `..`, barras invertidas e links simbólicos no empacotamento; exigir manifesto e entrypoint existente; extrair para pasta temporária e mover ao destino após validar caminhos.

Limitações: sem assinatura, checksum autenticado, dependências, rollback, sandbox ou ABI/runtime estáveis. Não instale pacotes de origem desconhecida.
