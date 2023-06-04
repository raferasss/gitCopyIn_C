#include "commands.h"
#include "utils.h"
#include "database.h"

void iniciarVersionador() {
    createDatabase();
    printInfo("Versionador iniciado com sucesso.");
}

void adicionarArquivos(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        // Verificar se o arquivo existe e adicionar à lista de arquivos a serem registrados
        // Utilizar função do database.h para adicionar o arquivo
    }
}

void registrarSnapshot(const char* texto) {
    // Registrar os arquivos marcados com a função correspondente do database.h
    // Atribuir um número único (identificador) para o snapshot dos arquivos
}

void exibirLog(int showContent) {
    // Obter informações sobre as versões do banco de dados com a função correspondente do database.h
    // Exibir as informações no console
    // Se showContent for verdadeiro, exibir também o conteúdo dos arquivos registrados em cada versão
}

void mostrarVersao(const char* identificador) {
    // Obter o texto registrado para a versão com o identificador especificado
    // Obter o conteúdo dos arquivos versionados naquela versão e exibi-los no console
}

void mudarVersao(const char* identificador) {
    // Verificar se o identificador é válido e existe no banco de dados
    // Salvar um backup dos arquivos atuais em uma pasta temporária
    // Restaurar os arquivos da versão com o identificador especificado
}

void reverterVersaoAtual() {
    // Reverter os arquivos de uma versão específica para a versão atual dos arquivos
    // Estejam eles registrados em uma última versão ou não
}
