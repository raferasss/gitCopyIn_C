#include "database.h"
#include "utils.h"
#define _DEFAULT_SOURCE
#include <dirent.h>
#include <stddef.h> 

#define _DEFAULT_SOURCE = DT_R

void createDatabase() {
    createDirectory(".versionador");
    createDirectory(".versionador/content");
}

void addFileToSnapshot(const char* filename) {
    // Marcar o arquivo para ser adicionado ao próximo snapshot
}

void registerSnapshot(const char* identifier, const char* text) {
    // Criar um snapshot dos arquivos marcados e registrar no banco de dados
    char* snapshotPath = concatenatePaths(".versionador/snapshots", identifier);
    writeTextFile(snapshotPath, text);
    free(snapshotPath);
}

void getSnapshotInfo() {
    // Obter informações sobre os snapshots registrados no banco de dados
    printf("Lista de snapshots:\n");

    // Abrir a pasta de snapshots
    DIR* snapshotsDir = opendir(".versionador/snapshots");
    if (snapshotsDir == NULL) {
        printError("Failed to open snapshots directory.");
        return;
    }

    // Ler os arquivos na pasta de snapshots
    struct dirent* entry;
    while ((entry = readdir(snapshotsDir)) != NULL) {
        if (entry->d_type == DT_REG) {  // Verificar se é um arquivo regular
            char* identifier = entry->d_name;

            // Construir o caminho completo para o arquivo do snapshot
            char* snapshotPath = concatenatePaths(".versionador/snapshots", identifier);

            // Ler o texto do snapshot
            char* snapshotText = readTextFile(snapshotPath);
            if (snapshotText != NULL) {
                printf("Identificador: %s\n", identifier);
                printf("Texto: %s\n\n", snapshotText);
                free(snapshotText);
            }

            free(snapshotPath);
        }
    }

    closedir(snapshotsDir);
}

void getVersionContent(const char* identifier, int showContent) {
    // Obter o texto registrado para a versão com o identificador especificado
    // Obter o conteúdo dos arquivos versionados naquela versão
    // Se showContent for verdadeiro, exibir o conteúdo dos arquivos no console

    // Construir o caminho completo para o arquivo do snapshot
    char* snapshotPath = concatenatePaths(".versionador/snapshots", identifier);

    // Ler o texto do snapshot
    char* snapshotText = readTextFile(snapshotPath);
    if (snapshotText != NULL) {
        printf("Texto registrado para a versão %s:\n", identifier);
        printf("%s\n\n", snapshotText);
        free(snapshotText);
    }

    if (showContent) {
        // Construir o caminho completo para a pasta de conteúdo
        char* contentPath = concatenatePaths(".versionador/content", identifier);

        // Abrir a pasta de conteúdo
        DIR* contentDir = opendir(contentPath);
        if (contentDir == NULL) {
            printError("Failed to open content directory.");
            free(contentPath);
            return;
        }

        printf("Conteúdo dos arquivos versionados:\n");

        // Ler os arquivos na pasta de conteúdo
        struct dirent* entry;
        while ((entry = readdir(contentDir)) != NULL) {
            if (entry->d_type == DT_REG) {  // Verificar se é um arquivo regular
                // Construir o caminho completo para o arquivo de conteúdo
                char* filePath = concatenatePaths(contentPath, entry->d_name);

                // Ler o conteúdo do arquivo
                char* fileContent = readTextFile(filePath);
                if (fileContent != NULL) {
                    printf("Arquivo: %s\n", entry->d_name);
                    printf("Conteúdo:\n%s\n\n", fileContent);
                    free(fileContent);
                }

                free(filePath);
            }
        }

        closedir(contentDir);
        free(contentPath);
    }

    free(snapshotPath);
}

void changeVersion(const char* identifier) {
    // Mudar para a versão com o identificador especificado
    // Salvar um backup dos arquivos atuais em uma pasta temporária
    // Restaurar os arquivos da versão com o identificador especificado

    // Verificar se a versão com o identificador especificado existe
    if (!versionExists(identifier)) {
        printf("A versão %s não existe.\n", identifier);
        return;
    }

    // Construir o caminho completo para a pasta de backup
    char* backupPath = concatenatePaths(".versionador/backup", identifier);

    // Verificar se a pasta de backup já existe
    if (directoryExists(backupPath)) {
        printf("Já existe um backup para a versão %s.\n", identifier);
        free(backupPath);
        return;
    }

    // Criar a pasta de backup
    createDirectory(backupPath);

    // Salvar um backup dos arquivos atuais
    backupCurrentFiles(backupPath);

    // Restaurar os arquivos da versão com o identificador especificado
    restoreVersionFiles(identifier, backupPath);

    printf("Mudança para a versão %s concluída com sucesso.\n", identifier);

    free(backupPath);
}

void restoreCurrentVersion() {
    // Obter o identificador da versão atual do arquivo versions.txt
    char* currentVersionIdentifier = getCurrentVersionIdentifier();

    // Verificar se a versão atual existe no banco de dados
    if (!versionExists(currentVersionIdentifier)) {
        printError("A versão atual não existe.");
        return;
    }

    // Verificar se a pasta temporária de backup existe
    if (!directoryExists(".versionador/backup")) {
        printError("A pasta de backup não existe.");
        return;
    }

    // Restaurar os arquivos da versão atual
    restoreVersionFiles(currentVersionIdentifier, ".versionador/backup");

    printInfo("Os arquivos foram restaurados para a versão atual.");
}

int versionExists(const char* identifier) {
    // Verificar se a versão com o identificador especificado existe
    char* snapshotPath = concatenatePaths(".versionador/snapshots", identifier);
    int exists = fileExists(snapshotPath);
    free(snapshotPath);
    return exists;
}

void backupCurrentFiles(const char* backupPath) {
    // Salvar um backup dos arquivos atuais em uma pasta temporária

    // Construir o caminho completo para a pasta de conteúdo
    char* contentPath = concatenatePaths(".versionador/content", getCurrentVersionIdentifier());

    // Criar a pasta de backup
    createDirectory(backupPath);

    // Copiar todos os arquivos da pasta de conteúdo para a pasta de backup
    copyDirectory(contentPath, backupPath);

    free(contentPath);
}

void restoreVersionFiles(const char* identifier, const char* backupPath) {
    // Restaurar os arquivos da versão com o identificador especificado

    // Construir o caminho completo para a pasta de conteúdo da versão
    char* versionContentPath = concatenatePaths(".versionador/content", identifier);

    // Remover a pasta de conteúdo atual
    removeDirectory(".versionador/content");

    // Copiar todos os arquivos da pasta de backup para a pasta de conteúdo
    copyDirectory(backupPath, ".versionador/content");

    // Renomear a pasta de conteúdo para a versão desejada
    renameDirectory(".versionador/content", versionContentPath);

    free(versionContentPath);
}