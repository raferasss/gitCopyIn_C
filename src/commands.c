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

void  saveSnapshotFilesInContent(char* identifier){
    char* text = readTextFile("jerico.txt");
    setPathToSnapshotIdentifier(identifier);
    Lista* paths = lst_cria();
    printInfo(text);
    free(text);
    text = readTextFile("jerico.txt");
    lst_populateList(".versionador/snapshots/next_snapshot.txt", paths);
    printInfo(text);
    free(text);
    text = readTextFile("jerico.txt");
    lst_infoSetAddContent(paths, identifier);
    lst_libera(paths);
    printInfo(text);
    free(text);

}



