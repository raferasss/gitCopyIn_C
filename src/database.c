#include "database.h"
#include "utils.h"

void createDatabase() {
    createDirectory(".versionador");
    createDirectory(".versionador/content");
}

void addFileToSnapshot(const char* filename) {
    // Marcar o arquivo para ser adicionado ao próximo snapshot
}

void registerSnapshot(const char* identifier, const char* text) {
    // Criar um snapshot dos arquivos marcados e registrar no banco de dados
}

void getSnapshotInfo() {
    // Obter informações sobre os snapshots registrados no banco de dados
}

void getVersionContent(const char* identifier, int showContent) {
    // Obter o texto registrado para a versão com o identificador especificado
    // Obter o conteúdo dos arquivos versionados naquela versão
    // Se showContent for verdadeiro, exibir o conteúdo dos arquivos no console
}

void changeVersion(const char* identifier) {
    // Mudar para a versão com o identificador especificado
    // Salvar um backup dos arquivos atuais em uma pasta temporária
    // Restaurar os arquivos da versão com o identificador especificado
}

void restoreCurrentVersion() {
    // Reverter os arquivos para a versão atual
    // Estejam eles registrados em uma última versão ou não
}
