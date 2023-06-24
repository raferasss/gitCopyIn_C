#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "commands/commands.h"
#include "utils/utils.h"
#include "database/database.h"
#include "utils/lista.h"

// Protótipos das funções
void iniciar();
void adiciona(int argc, char *argv[]);
void registra(char *texto);
void exibirlog();
void logComConteudo();
void mostrar(char *identificador);
void mudar(char *identificador);
void mudarAtual();

int main(int argc, char *argv[]) {
    // Verifica o número de argumentos
   
    if (argc < 2) {
        printf("Uso: versionador.exe <comando> [argumentos]\n");
        return 1;
    }

    // Obtém o comando a ser executado
    char *comando = argv[1];

    // Chama a função correspondente ao comando
    if (strcmp(comando, "iniciar") == 0) {
        iniciar();
        
    } else if (strcmp(comando, "adiciona") == 0) {
        adiciona(argc, argv);
    } else if (strcmp(comando, "registra") == 0) {
        // Verifica se foi fornecido o texto do snapshot
        if (argc < 3) {
            printf("Uso: versionador.exe registra \"Texto do snapshot\"\n");
            return 1;
        }
        char *texto = argv[2];
        registra(texto);
    } else if (strcmp(comando, "log") == 0) {
        for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--conteudo") == 0) {
            logComConteudo();
        }else{
            exibirlog();
        }
        }
        
    } else if (strcmp(comando, "mostrar") == 0) {
        // Verifica se foi fornecido o identificador da versão
        if (argc < 3) {
            printf("Uso: versionador.exe mostrar <identificador>\n");
            return 1;
        }
        char *identificador = argv[2];
        mostrar(identificador);
    } else if (strcmp(comando, "mudar") == 0) {
        // Verifica se foi fornecido o identificador da versão
        if (argc < 3) {
            printf("Uso: versionador.exe mudar <identificador>\n");
            return 1;
        }else{for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--atual") == 0) {
            mudarAtual();
        }else{
            char *identificador = argv[2];
            mudar(identificador);
        }
        }
        }
        
     }else {
        printf("Comando inválido.\n");
        return 1;
    }

    return 0;
}

// Implementação das funções
/**
 * @brief Inicia o versionador chamando a função iniciarVersionador().
 */
void iniciar() {
    iniciarVersionador();
}

/**
 * @brief Adiciona arquivos ao versionador.
 * 
 * @param argc O número de argumentos.
 * @param argv Os argumentos.
 */

void adiciona(int argc, char* argv[]) {
    for (int i = 2; i < argc; i++) {
        if (fileExists(argv[i])) {
            addFileToSnapshot(argv[i]);
            printInfo("Arquivo adicionado com sucesso.");
        } else {
            printf("Arquivo não encontrado: %s", argv[i]);
        }
    }
}

/**
 * @brief Registra um snapshot com o texto especificado.
 * 
 * @param texto O texto do snapshot.
 */

void registra(char* texto) {
    char* id = registrarSnapshot(texto);
    saveSnapshotFilesInContent(id);
    free(id);
}

/**
 * @brief Exibe o log das versões.
 */

void exibirlog() {
    int numVersions = getSnapshotInfo();
    printInfo("Exibindo log das versões:");

    for (int i = 0; i < numVersions; i++) {
        const char* versionIdentifier = getCurrentVersionIdentifier(i);
        printf("Versão %d: %s\n", i, versionIdentifier);
    }
}

/**
 * @brief Exibe o log das versões com o conteúdo dos arquivos.
 */

void logComConteudo() {
    int numVersions = getSnapshotInfo();
    printInfo("Exibindo conteúdo dos arquivos nas versões:");
    getLogsContent(".versionador/versions.txt");
    
    
}

/**
 * @brief Mostra o conteúdo de uma versão específica.
 * 
 * @param identificador O identificador da versão.
 */

void mostrar(char* identificador) {
    getVersionContent(identificador, 1);
}

/**
 * @brief Muda para uma versão específica.
 * 
 * @param identificador O identificador da versão.
 */

void mudar(char* identificador) {
    if (versionExists(identificador)) {
        changeVersion(identificador);
    } else {
        printf("A versão %s não existe.", identificador);
    }
}

/**
 * @brief Muda para a versão atual.
 */

void mudarAtual() {
    restoreCurrentVersion();
    printInfo("Versão atual revertida com sucesso.");
}

