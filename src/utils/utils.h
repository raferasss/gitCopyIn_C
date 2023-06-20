#ifndef UTILS_H
#define UTILS_H
#include "lista.h"

void printError(const char* message);
void printInfo(const char* message);
void createDirectory(const char* path);
char* concatenatePaths(const char* path1, const char* path2);
char* readTextFile(const char* name);
void writeTextFile(const char* filename, const char* content);
int fileExists(const char* filename);
char* generateUniqueIdentifier();
int contLines(char* path);
void removeFile(char* file);
void writeTextFileNextLine(const char* name, const char* text);
void searchDirectoryFiles(char* path, Lista* header);
#endif  // UTILS_H
