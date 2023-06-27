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
void createBranch(char* name);
void removeBranch(char* branchName);
void renameBranch(char* nameAtual,char* novoNome);

#endif  // COMMANDS_H