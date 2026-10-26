# Atualizações da GenOS Suite sem gerar ISO

Alterações em `apps/**` publicam um pacote independente da suite e **não** disparam os workflows de ISO. O pacote não contém kernel, bootloader nem imagem ISO.

## Obter o pacote
1. Abra [GitHub Actions](https://github.com/ProjectsVanguard/GenOS/actions/workflows/publish-suite-update.yml).
2. Selecione a execução mais recente de **Publish GenOS app-suite update (no ISO)**.
3. Na seção **Artifacts**, baixe `GenOS-Suite-Update-<número>` e extraia o ZIP do artefato. Ele contém `GenOS-Suite-Update.zip`, `SHA256SUMS.txt` e `update-manifest.json`.
4. Verifique o checksum no diretório onde está o pacote:
   `sha256sum -c SHA256SUMS.txt`
5. Para aplicar a um diretório Applications acessível no Linux, execute:
   `bash tools/apply-suite-update.sh GenOS-Suite-Update.zip /caminho/para/Applications`

O script cria uma cópia de segurança do conteúdo anterior. Teste primeiro numa pasta de demonstração e confirme que o diretório de destino é o correto.

## Limitações importantes
- A atualização é para os arquivos da suite web, não para o kernel.
- O GenOS ainda não tem um gerenciador nativo de atualizações nem um runtime que execute esses protótipos como apps nativos.
- O script deve ser executado num ambiente Linux com `bash` e `unzip`, com permissão de escrita no diretório de destino.
- O pacote é publicado como artifact de Actions; não é uma GitHub Release e não exige permissão de escrita para releases.
