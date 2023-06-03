#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printError(const char* message) {
    fprintf(stderr, "[ERROR] %s\n", message);
}

void printInfo(const char* message) {
    printf("[INFO] %s\n", message);
}

void createDirectory(const char* path) {
    int result = mkdir(path, 0777);
    if (result != 0) {
        printError("Failed to create directory.");
        exit(1);
    }
}

char* concatenatePaths(const char* path1, const char* path2) {
    size_t len1 = strlen(path1);
    size_t len2 = strlen(path2);
    char* combined = (char*)malloc(len1 + len2 + 2);
    strcpy(combined, path1);
    strcat(combined, "/");
    strcat(combined, path2);
    return combined;
}

char* readTextFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printError("Failed to open file for reading.");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileLength = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(fileLength + 1);
    fread(content, 1, fileLength, file);
    content[fileLength] = '\0';

    fclose(file);
    return content;
}

void writeTextFile(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printError("Failed to open file for writing.");
        return;
    }

    fputs(content, file);
    fclose(file);
}

