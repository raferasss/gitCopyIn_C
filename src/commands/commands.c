#include "commands.h"
#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../database/database.h"
#include "../utils/lista.h"




void iniciarVersionador() {
    createDatabase();
    printInfo("Versionador iniciado com sucesso.");
}

void adicionarArquivos(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        if (fileExists(argv[i])) {
            addFileToSnapshot(argv[i]);
            printInfo("Arquivo adicionado com sucesso.");
        } else {
            printf("Arquivo não encontrado: %s", argv[i]);
        }
    }
}

char* registrarSnapshot(const char* texto) {
    char* identifier = generateUniqueIdentifier();
    registerSnapshot(identifier, texto);
    printf("Snapshot registrado com sucesso. Identificador: %s", identifier);
    return identifier;
}

void exibirLog(int showContent) {
    int numVersions = getSnapshotInfo();
    if (showContent) {
        printInfo("Exibindo conteúdo dos arquivos nas versões:");

        // Exibir o conteúdo dos arquivos registrados em cada versão
        for (int i = 0; i < numVersions; i++) {
            const char* versionIdentifier = getCurrentVersionIdentifier(i);

            // Obter o conteúdo dos arquivos versionados na versão atual
            getVersionContent(versionIdentifier, 1);
        }
    }
}

void mostrarVersao(const char* identificador) {
    getVersionContent(identificador, 1);
}

void mudarVersao(const char* identificador) {
    if (versionExists(identificador)) {
        changeVersion(identificador);
    } else {
        printf("A versão %s não existe.", identificador);
    }
}

void reverterVersaoAtual() {
    restoreCurrentVersion();
    printInfo("Versão atual revertida com sucesso.");
}

void  saveSnapshotFilesInContent(const char* identifier){
    setPathToSnapshotIdentifier(identifier);
    Lista* header = lst_cria();
    lst_populateList(".versionador/snapshots/next_snapshot.txt", header);
    ListaNo* ptr = lst_returnNodeValid(header);
    if(ptr == NULL)
    return;

    char* info = lst_infoValid(ptr);
    while (info != NULL)
    {   
        
        addContent(identifier, info);
        ptr = lst_nextNode(ptr);
        info = lst_infoValid(ptr);
    }
    removeFile(".versionador/snapshots/next_snapshot.txt");
    
    free(info);
    free(ptr);
    lst_libera(header);

}

void getLogsContent(char* file){
    Lista* header = lst_cria();
    lst_populateList(file, header);
    ListaNo* ptr = lst_returnNodeValid(header);
    if(ptr == NULL)
    return;

    char* info = lst_infoValid(ptr);
    while (info != NULL)
    {   
    //printInfo(concatenatePaths("getLogsContent 100", info));
        getVersionContent(info, 1);
        ptr = lst_nextNode(ptr);
        info = lst_infoValid(ptr);
    }
    
    free(info);
    free(ptr);
    lst_libera(header);
    
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

    if (directoryExists(".versionador/backup")) {
        removeDirectory(".versionador/backup");
    }

    createDirectory(".versionador/backup");    
    char path[100];
    strcpy(path,concatenatePaths(".versionador/content/", identifier));
    Lista *header = lst_cria();
    searchDirectoryFiles(path, header);
    ListaNo* ptr = lst_returnNodeValid(header);

    
    if(ptr == NULL){
    return;
    }else{
    

    char* fileOrigin = lst_infoValid(ptr);
    char copySec[100];
    while (fileOrigin != NULL)
    {   
        strcpy(copySec, fileOrigin);
        char *fileDest = concatenatePaths(".versionador/backup", copySec);
        writeTextFile(fileDest, "");
        copyFile(copySec, fileDest);
        ptr = lst_nextNode(ptr);
        fileOrigin = lst_infoValid(ptr);
        free(fileDest);
    }
    free(fileOrigin);
    
    }

    searchDirectoryFiles(path, header); 
    ptr = lst_returnNodeValid(header);

    if(ptr == NULL){
    return;
    }else{
    

    char* fileOrigin = lst_infoValid(ptr);
    
    while (fileOrigin != NULL)
    {   
        char *fileContentOrigin = concatenatePaths(path, fileOrigin);
        removeFile(fileOrigin);
        writeTextFile(fileOrigin, "");
        copyFile(fileContentOrigin, fileOrigin);
        ptr = lst_nextNode(ptr);
        fileOrigin = lst_infoValid(ptr);
        free(fileContentOrigin);
    }
    free(fileOrigin);
    
    }

    lst_libera(header);
}


void restoreCurrentVersion() {

    // Verificar se a pasta temporária de backup existe
    if (!directoryExists(".versionador/backup")) {
        printError("A pasta de backup não existe.");
        return;
    }

    char path[] = ".versionador/backup";
    Lista *header = lst_cria();
    searchDirectoryFiles(path, header);
    ListaNo* ptr = lst_returnNodeValid(header);

    
    char copySec[100];
    if(ptr == NULL){
    return;
    }else{
    

    char* fileOrigin = lst_infoValid(ptr);
    
    while (fileOrigin != NULL)
    {   
        strcpy(copySec, fileOrigin);
        char *fileBackupOrigin = concatenatePaths(path, copySec);
        removeFile(copySec);
        copyFile(fileBackupOrigin, copySec);
        ptr = lst_nextNode(ptr);
        fileOrigin = lst_infoValid(ptr);
        free(fileBackupOrigin);
    }
    free(fileOrigin);
    
    }

    removeDirectory(path);
    printInfo("Os arquivos foram restaurados para a versão atual.");

    lst_libera(header);
}