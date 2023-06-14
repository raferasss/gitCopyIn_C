#include "commands.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include "lista.h"




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

void getLogsContent(const char* file){
    Lista* header = lst_cria();
    lst_populateList(file, header);
    ListaNo* ptr = lst_returnNodeValid(header);
    if(ptr == NULL)
    return;

    char* info = lst_infoValid(ptr);
    while (info != NULL)
    {   
        
printInfo(concatenatePaths("getLogsContent 100", info));
        getVersionContent(info, 1);
        ptr = lst_nextNode(ptr);
        info = lst_infoValid(ptr);
    }
    
    free(info);
    free(ptr);
    lst_libera(header);
    
}


