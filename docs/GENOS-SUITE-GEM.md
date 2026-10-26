# GenOS Suite em um único pacote .gem

O pacote `org.genos.suite.gem` reúne o launcher e todos os diretórios presentes em `apps/` em um único arquivo. A instalação cria uma única pasta de aplicativo (`Applications/org.genos.suite/`), com o manifesto na raiz e os aplicativos incluídos em `apps/`.

## Gerar e validar

```sh
make suite-gem
python3 tools/gem/genpkg.py validate build/org.genos.suite.gem
```

Instalação offline em uma pasta de aplicativos:

```sh
python3 tools/gem/genpkg.py install build/org.genos.suite.gem --root ./Applications
```

O entrypoint declarado é `apps/genos-suite/index.html`. O launcher usa caminhos relativos para os demais aplicativos, mantidos dentro do mesmo pacote.

**Limitação importante:** o formato `.gem` do GenOS é atualmente um contêiner ZIP experimental. O kernel não executa HTML como aplicativo nativo; esta entrega empacota toda a suite como uma única unidade instalável para uso em um runtime compatível, sem adicionar esse runtime ao kernel.
