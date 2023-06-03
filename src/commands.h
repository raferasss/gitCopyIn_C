#ifndef COMMANDS_H
#define COMMANDS_H

void iniciarVersionador();
void adicionarArquivos(int argc, char* argv[]);
void registrarSnapshot(const char* texto);
void exibirLog(int showContent);
void mostrarVersao(const char* identificador);
void mudarVersao(const char* identificador);
void reverterVersaoAtual();

#endif  // COMMANDS_H
