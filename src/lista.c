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
    FILE  *arquivo = fopen(path, "r");
    if(arquivo == NULL){
    printError("nulo");
        return;
    }
        while(!feof(arquivo)){
            ListaNo *novo = (ListaNo*) malloc(sizeof(ListaNo));
            fgets(novo->info,50, arquivo);
            novo->proximo = header->primeiro;
            header->primeiro = novo;
        }

    lst_imprime(header);
    free(arquivo);
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

void lst_infoSetAddContent (Lista *header, char* identifier)
{
    ListaNo *ptr = header->primeiro;
    while(ptr != NULL)
    {   
        if(ptr ->proximo!= NULL){
            
            addContent(ptr->info, identifier);
          
        }
        ptr = ptr -> proximo;
         
    }
}