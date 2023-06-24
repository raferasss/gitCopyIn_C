#include "commands.h"
#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../database/database.h"
#include "../utils/lista.h"



/**
 * @brief Inicia o versionador.
 *
 * Cria o banco de dados e exibe uma mensagem de sucesso.
 */
void iniciarVersionador() {
    createDatabase();
    printInfo("Versionador iniciado com sucesso.");
}

/**
 * @brief Adiciona arquivos ao snapshot.
 * @param argc Número de argumentos.
 * @param argv Vetor de argumentos contendo os nomes dos arquivos.
 *
 * Percorre os argumentos passados e verifica se cada arquivo existe.
 * Se o arquivo existir, adiciona ao snapshot e exibe uma mensagem de sucesso.
 * Caso contrário, exibe uma mensagem informando que o arquivo não foi encontrado.
 */

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

/**
 * @brief Registra um snapshot.
 * @param texto Texto do snapshot.
 * @return Retorna o identificador único do snapshot.
 *
 * Gera um identificador único para o snapshot e o registra no banco de dados.
 * Exibe uma mensagem de sucesso com o identificador gerado.
 * Retorna o identificador gerado.
 */

char* registrarSnapshot(const char* texto) {
    char* identifier = generateUniqueIdentifier();
    registerSnapshot(identifier, texto);
    printf("Snapshot registrado com sucesso. Identificador: %s", identifier);
    return identifier;
}

/**
 * @brief Exibe o log do versionador.
 * @param showContent Flag para exibir o conteúdo dos arquivos nas versões.
 *
 * Obtém o número de versões e exibe uma mensagem informando que o log será exibido.
 * Se o flag showContent for verdadeiro, exibe o conteúdo dos arquivos em cada versão.
 */

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

/**
 * @brief Mostra o conteúdo de uma versão específica.
 * @param identificador Identificador da versão.
 *
 * Obtém o conteúdo dos arquivos na versão correspondente ao identificador informado.
 * Exibe o conteúdo dos arquivos.
 */

void mostrarVersao(const char* identificador) {
    getVersionContent(identificador, 1);
}

/**
 * @brief Muda para uma versão específica.
 * @param identificador Identificador da versão.
 *
 * Verifica se a versão com o identificador informado existe.
 * Se existir, muda para essa versão.
 * Caso contrário, exibe uma mensagem informando que a versão não existe.
 */

void mudarVersao(const char* identificador) {
    if (versionExists(identificador)) {
        changeVersion(identificador);
    } else {
        printf("A versão %s não existe.", identificador);
    }
}

/**
 * @brief Reverte a versão atual.
 *
 * Restaura a versão anterior da versão atual.
 * Exibe uma mensagem de sucesso.
 */

void reverterVersaoAtual() {
    restoreCurrentVersion();
    printInfo("Versão atual revertida com sucesso.");
}

/**
 * @brief Salva os arquivos do snapshot no conteúdo.
 * @param identifier Identificador do snapshot.
 *
 * Define o caminho para o identificador do snapshot.
 * Popula uma lista com os nomes dos arquivos do próximo snapshot.
 * Percorre a lista e adiciona o conteúdo dos arquivos ao snapshot atual.
 * Remove o arquivo que lista os arquivos do próximo snapshot.
 * Libera a memória alocada.
 */

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

/**
 * @brief Obtém o conteúdo dos logs.
 * @param file Arquivo dos logs.
 *
 * Cria uma lista e a preenche com o conteúdo do arquivo dos logs.
 * Percorre a lista e exibe o conteúdo das versões correspondentes.
 * Libera a memória alocada.
 */

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

/**
 * @brief Muda para uma versão específica.
 * @param identifier Identificador da versão.
 *
 * Verifica se a versão com o identificador informado existe.
 * Se existir, muda para essa versão.
 * Caso contrário, exibe uma mensagem informando que a versão não existe.
 * Faz um backup dos arquivos atuais em uma pasta temporária.
 * Restaura os arquivos da versão especificada.
 */

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

/**
 * @brief Restaura a versão atual.
 *
 * Verifica se a pasta temporária de backup existe.
 * Se não existir, exibe uma mensagem de erro e retorna.
 * Obtém a lista de arquivos na pasta de backup.
 * Percorre a lista e restaura os arquivos para a versão atual.
 * Remove a pasta de backup.
 * Exibe uma mensagem informando que os arquivos foram restaurados.
 */

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