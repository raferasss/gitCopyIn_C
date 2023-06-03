#ifndef UTILS_H
#define UTILS_H

void printError(const char* message);
void printInfo(const char* message);
void createDirectory(const char* path);
char* concatenatePaths(const char* path1, const char* path2);
char* readTextFile(const char* filename);
void writeTextFile(const char* filename, const char* content);

#endif  // UTILS_H

