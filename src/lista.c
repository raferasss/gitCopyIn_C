#include"lista.h"
#include<stdlib.h>
#include<stdio.h>
#include "utils.h"
#include "database.h"

struct lista_no {
    char* info;
    ListaNo* proximo;
};

struct lista {
    ListaNo* primeiro;
};


Lista* lst_cria(void)
{
    Lista *l = (Lista*) malloc(sizeof(Lista));
    l->primeiro = NULL;
    return l;
}

void lst_insere(Lista *header, char* text)
{
    ListaNo *novo = (ListaNo*) malloc(sizeof(ListaNo));
    novo->info = text;
    novo->proximo = header->primeiro;
    header->primeiro = novo;
}

void lst_populateList(char* path, Lista* header){
    char* content = readTextFile(path);


    int i = 0;
    char palavra[100]; // Tamanho máximo da palavra definido como 100
    int j = 0;
    
    while (content[i] != '\0') {
        if (content[i] == '\n') {
            palavra[j] = '\0'; // Adiciona o caractere nulo para indicar o fim da palavra
            printInfo(palavra);
            ListaNo *novo = (ListaNo*) malloc(sizeof(ListaNo));
            
            novo->info = palavra;
            
            novo->proximo = header->primeiro;
            header->primeiro = novo; // Imprime a palavra completa
            j = 0; // Reseta o índice para a próxima palavra
        } else {
            palavra[j] = content[i]; // Armazena o caractere na palavra
            j++;
        }
        i++;
    }
    
}

void lst_imprime(Lista *header)
{
    ListaNo *ptr = header->primeiro;
    while(ptr != NULL)
    {
        printf("%s\n", ptr->info);
        ptr = ptr->proximo;
    }
}

int pertence(Lista *header, char* text)
{
    ListaNo *ptr = header->primeiro;
    while(ptr != NULL)
    {
        if (ptr->info == text)
            return 1;
        ptr = ptr->proximo;
    }

    return 0;
}

void lst_retira(Lista *header, char* text)
{
    ListaNo *ant = NULL;
    ListaNo *p = header->primeiro;

    while (p != NULL && p->info != text)
    {
        ant = p;
        p = p->proximo;
    }

    if (p != NULL) {
        if (ant == NULL)
            header->primeiro = p->proximo;
        else 
            ant->proximo = p->proximo;

        free(p);
    }
}

int lst_vazia(Lista *header)
{
    return (header->primeiro == NULL);
}

void lst_libera(Lista *header)
{
    ListaNo *p = header->primeiro;
    while (p != NULL)
    {
        ListaNo *t  = p->proximo;
        free(p);
        p = t;
    }

    free(header);
}
ListaNo* lst_returnNodeValid(Lista *header){
    if(header->primeiro != NULL)
    return header->primeiro;
    
    printError("lista vazia");

    return NULL;
}
ListaNo* lst_nextNode(ListaNo *ptr){
    if(ptr != NULL)
    return ptr->proximo;

    printError("no invalido");

    return NULL;
}
char* lst_infoValid(ListaNo *ptr)
{
        if(ptr != NULL)
        return ptr->info;
        
        return NULL;
         
}