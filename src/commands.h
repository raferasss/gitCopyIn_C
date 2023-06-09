#ifndef COMMANDS_H
#define COMMANDS_H

void iniciarVersionador();
void adicionarArquivos(int argc, char* argv[]);
char* registrarSnapshot(const char* texto);
void exibirLog(int showContent);
void mostrarVersao(const char* identificador);
void mudarVersao(const char* identificador);
void reverterVersaoAtual();
void  saveSnapshotFilesInContent(char* identifier);

#endif  // COMMANDS_H