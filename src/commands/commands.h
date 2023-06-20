#ifndef COMMANDS_H
#define COMMANDS_H

void iniciarVersionador();
void adicionarArquivos(int argc, char* argv[]);
char* registrarSnapshot(const char* texto);
void exibirLog(int showContent);
void mostrarVersao(const char* identificador);
void mudarVersao(const char* identificador);
void reverterVersaoAtual();
void  saveSnapshotFilesInContent(const char* identifier);
void getLogsContent(char* file);
void changeVersion(const char* identifier);
void restoreCurrentVersion();

#endif  // COMMANDS_H