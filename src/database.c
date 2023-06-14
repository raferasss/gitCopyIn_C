#define _DEFAULT_SOURCE
#include "database.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <string.h>
#include "lista.h"


Version versions[MAX_NUM_VERSIONS];
int numVersions = 0;

void createDatabase() {
    createDirectory(".versionador");
    createDirectory(".versionador/content");
    createDirectory(".versionador/snapshots");
    writeTextFile(".versionador/versions.txt", "");
}

void addFileToSnapshot(const char* filename) {
     // Marcar o arquivo para ser adicionado ao próximo snapshot
    char* snapshotPath = concatenatePaths(".versionador/snapshots", "next_snapshot.txt");
    size_t len1 = strlen("\n");
    size_t len2 = strlen(filename);
    char* combined = (char*)malloc(len1 + len2 + 2);
    strcpy(combined, filename);
    strcat(combined, "");
    strcat(combined, "\n");
     
    FILE* snapshotFile = fopen(snapshotPath, "w");
    if (snapshotFile == NULL) {
        printError("Failed to open snapshot file for appending.");
        free(snapshotPath);
       
        free(combined);
        return;
    }

    fputs(combined, snapshotFile);
    fclose(snapshotFile);
    free(combined);
    free(snapshotPath);

}

void registerSnapshot(const char* identifier, const char* commit) {
    // Criar um snapshot dos arquivos marcados e registrar no banco de dados
    char* snapshotPath = concatenatePaths(".versionador/snapshots", identifier);
    writeTextFile(snapshotPath, commit);
   
    free(snapshotPath);
}
void  setPathToSnapshotIdentifier(const char* identifier){
    char* path = concatenatePaths(".versionador/content", identifier);
    writeTextFileNextLine(".versionador/versions.txt", identifier);
    createDirectory(path);
    printInfo("create version path");
    free(path);
}
void moveFileSnapshotToContent(const char* file, const char* identifier){

}
int getSnapshotInfo() {
    printf("Lista de snapshots:\n");

    // Abrir a pasta de snapshots
    DIR* snapshotsDir = opendir(".versionador/snapshots");
    if (snapshotsDir == NULL) {
        printError("Failed to open snapshots directory.");
        return 0;
    }

    // Ler os arquivos na pasta de snapshots
    int numVersions = 0;
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
                numVersions++;
            }

            free(snapshotPath);
        }
    }

    closedir(snapshotsDir);
    return numVersions;
}

void getVersionContent(const char* id, int showContent) {
    char identifier[100];
    int i = 0;
    int j = 0;
   
   while (id[i] != '\0') {
            identifier[j] = id[i]; // Armazena o caractere na palavra
            j++;
            i++;
    }
    identifier[j] = '\0';
    // Obter o texto registrado para a versão com o identificador especificado
    // Obter o conteúdo dos arquivos versionados naquela versão
    // Se showContent for verdadeiro, exibir o conteúdo dos arquivos no console
    printInfo(identifier);
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


int directoryExists(const char* path) {
    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        return 1;  // Diretório existe
    }
    return 0;  // Diretório não existe
}

char* getCurrentVersionIdentifier() {
    char* versionsFile = ".versionador/versions.txt";
    char* currentVersionIdentifier = NULL;

    // Ler o identificador da versão atual do arquivo versions.txt
    char* versionsText = readTextFile(versionsFile);
    if (versionsText != NULL) {
        // Obter a última linha do arquivo
        char* lastLine1 = strrchr(versionsText, '\n');
        char* lastLine;
        strcpy(lastLine, lastLine1 + 1);
        if (lastLine != NULL) {
            // Obter o identificador da versão
            size_t length = strlen(lastLine);
            currentVersionIdentifier = (char*)malloc(length + 1);
            strncpy(currentVersionIdentifier, lastLine, length);
            currentVersionIdentifier[length] = '\0';
        }
        free(versionsText);
    }

    return currentVersionIdentifier;
}

void copyDirectory(const char* sourceDir, const char* destDir) { 

    // Abrir o diretório de origem
    DIR* dir = opendir(sourceDir);
    if (dir == NULL) {
        printf("Failed to open source directory.\n");
        return;
    }

    // Ler os arquivos no diretório de origem
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construir os caminhos completos para o arquivo de origem e destino
        char* sourcePath = concatenatePaths(sourceDir, entry->d_name);
        char* destPath = concatenatePaths(destDir, entry->d_name);

        // Verificar se é um diretório
        struct stat st;
        if (stat(sourcePath, &st) == 0 && S_ISDIR(st.st_mode)) {
            // Se for um diretório, chamar recursivamente a função copyDirectory
            copyDirectory(sourcePath, destPath);
        } else {
            // Se for um arquivo, copiar o arquivo para o destino
            copyFile(sourcePath, destPath);
        }

        free(sourcePath);
        free(destPath);
    }

    closedir(dir);
}

void copyFile(const char* sourceFile, const char* destFile) {
    FILE* source = fopen(sourceFile, "rb");
    FILE* dest = fopen(destFile, "wb");

    if (source == NULL || dest == NULL) {
        printf("Failed to open source or destination file.\n");
        return;
    }

    char buffer[4096];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytesRead, dest);
    }

    fclose(source);
    fclose(dest);
}

void removeDirectory(const char* path) {
    // Abrir o diretório
    DIR* dir = opendir(path);
    if (dir == NULL) {
        printf("Failed to open directory.\n");
        return;
    }

    // Ler os arquivos no diretório
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construir o caminho completo para o arquivo/diretório
        char* filePath = concatenatePaths(path, entry->d_name);

        // Verificar se é um diretório
        struct stat st;
        if (stat(filePath, &st) == 0 && S_ISDIR(st.st_mode)) {
            // Se for um diretório, chamar recursivamente a função removeDirectory
            removeDirectory(filePath);
        } else {
            // Se for um arquivo, remover o arquivo
            if (remove(filePath) != 0) {
                printf("Failed to remove file: %s\n", filePath);
            }
        }

        free(filePath);
    }

    closedir(dir);

    // Remover o diretório
    if (rmdir(path) != 0) {
        printf("Failed to remove directory: %s\n", path);
    }
}

void renameDirectory(const char* oldPath, const char* newPath) {
    // Verificar se os caminhos são diferentes
    if (strcmp(oldPath, newPath) == 0) {
        printf("The old and new paths are the same.\n");
        return;
    }

    // Renomear o diretório usando a função rename
    if (rename(oldPath, newPath) != 0) {
        printf("Failed to rename directory.\n");
        return;
    }
}

void addContent( const char* identifier,const char* text){
    char filename[100];
    int i = 0;
    int j = 0;
   
   while (text[i] != '\0') {
            filename[j] = text[i]; // Armazena o caractere na palavra
            j++;
            i++;
    }
    filename[j] = '\0';
    char* file = concatenatePaths(".versionador/content", concatenatePaths(identifier, filename));
    printInfo(filename);
    writeTextFile(file, readTextFile(filename));
    free(file);
}