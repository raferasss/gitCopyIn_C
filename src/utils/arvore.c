#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura do nó da árvore
typedef struct TreeNode {
    char name[50];
    char version[50];
    char initVersion[50];
    char parentName[50];
    struct TreeNode* parent;
    struct TreeNode* firstChild;
    struct TreeNode* nextSibling;
} TreeNode;

// Definição da estrutura da lista dinâmica
typedef struct ListNode {
    TreeNode* node;
    struct ListNode* next;
} ListNode;

typedef enum boolean {
    FALSE = 0,
    TRUE,
}Boolean;

// Função para criar um novo nó da árvore
TreeNode* createNode(const char* name, const char* initVersion, const char* version, const char* parentName) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(newNode->name, name);
    strcpy(newNode->initVersion, initVersion);
    strcpy(newNode->version, version);
    strcpy(newNode->parentName, parentName);
    newNode->parent = NULL;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    return newNode;
}

// Função para criar um novo nó da lista dinâmica
ListNode* createListNode(TreeNode* node) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->node = node;
    newNode->next = NULL;
    return newNode;
}

// Função para adicionar um nó à lista dinâmica
void addListNode(ListNode** head, TreeNode* node) {
    ListNode* newNode = createListNode(node);
    if (*head == NULL) {
        *head = newNode;
    } else {
        ListNode* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}
TreeNode*  findNodeInListByInitVersion(ListNode* head, const char* version){
    ListNode* current = head;
    while (current != NULL) {
        if (strcmp(current->node->initVersion, version) == 0) {
            return current->node;
        }
        current = current->next;
    }
    return NULL;
}
// Função para buscar um nó na lista dinâmica
TreeNode* findNodeInList(ListNode* head, const char* name) {
    ListNode* current = head;
    while (current != NULL) {
        if (strcmp(current->node->name, name) == 0) {
            return current->node;
        }
        current = current->next;
    }
    return NULL;
}

// Função para adicionar um nó filho a um nó pai na árvore
void addChild(TreeNode* parent, TreeNode* child) {
    child->parent = parent;
    if (parent->firstChild == NULL)
        parent->firstChild = child;
    else {
        TreeNode* sibling = parent->firstChild;
        while (sibling->nextSibling != NULL)
            sibling = sibling->nextSibling;
        sibling->nextSibling = child;
    }
}

// Função para imprimir a árvore
void printTree(TreeNode* node, int depth) {
    if (node == NULL)
        return;
    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("%s (%s) = %s\n", node->name, node->initVersion, node->version);
    printTree(node->firstChild, depth + 1);
    printTree(node->nextSibling, depth);
}

// Função para liberar a memória alocada pela árvore
void freeTree(TreeNode* node) {
    if (node == NULL)
        return;
    freeTree(node->firstChild);
    freeTree(node->nextSibling);
    free(node);
}
void addVersionsToList(const char* fileName, ListNode** listHead, const char* nodeName) {
    FILE* file = fopen(fileName, "r");
    if (file != NULL) {
        TreeNode* node = findNodeInList(*listHead, nodeName);
        if (node != NULL) {
            char line[200];
            char version[50];

            while (fgets(line, sizeof(line), file) != NULL) {
                sscanf(line, "%s", version);
                TreeNode* newNode = createNode(nodeName, "", version, "");
                TreeNode* nodeExist = findNodeInListByInitVersion(*listHead, version);

                if (nodeExist != NULL && !strcmp(nodeExist->name, nodeName) == 0) {
                    addChild(node, newNode);
                    addChild(newNode, nodeExist);
                } else {
                    addChild(node, newNode);
                }
            }
        }
        fclose(file);
    }
}

void readMetadataFile(const char* fileName, ListNode** listHead, TreeNode** root) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de metadados.\n");
        return;
    }

    char line[200];
    char name[50];
    char initVersion[50];
    char parentName[50];

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strcmp(line, "<INICIO>\n") == 0) {
            fgets(line, sizeof(line), file);  // Ler o nome da branch
            sscanf(line, "%s", name);
            fgets(line, sizeof(line), file);  // Ler a versão da branch
            sscanf(line, "%s", initVersion);
            fgets(line, sizeof(line), file);  // Ler o nome do pai da branch
            sscanf(line, "%s", parentName);

            TreeNode* newNode = createNode(name, initVersion, "", parentName);
            addListNode(listHead, newNode);

            TreeNode* node = findNodeInList(*listHead, "principal");
            if (*root == NULL && node != NULL) {
                *root = newNode;
            }
        }
    }

    fclose(file);
}

void freeList(ListNode* listHead) {
    ListNode* current = listHead;
    while (current != NULL) {
        ListNode* next = current->next;
        free(current);
        current = next;
    }
}


void addVersionsToListAndConcatenate(ListNode** head, const char* name) {
    char filename[200];
    sprintf(filename, "versoes%s.txt", name);
    addVersionsToList(filename, head, name);
}


TreeNode* createTree(const char* fileName){
    ListNode* listHead = NULL;
    TreeNode* root = NULL;

    readMetadataFile(fileName, &listHead, &root);

    ListNode* current = listHead;
    while (current != NULL) {
        addVersionsToListAndConcatenate(&listHead, current->node->name);
        current = current->next;
    }

    freeList(listHead);
    return root;
}

void searchVersion(TreeNode* node, const char* version) {
    if (node == NULL)
        return;

    if (strcmp(node->version, version) == 0) {
        printf("Versão encontrada!\n");
        printf("Nome do nó: %s\n", node->name);
        printf("Versão: %s\n", node->version);
        return;
    }


    // Recursivamente buscar na subárvore do primeiro filho
    searchVersion(node->firstChild, version);

    // Recursivamente buscar na subárvore do próximo irmão
    searchVersion(node->nextSibling, version);
}

void printBranchByName(TreeNode* node, const char* name, const char* versionStop) {

    if (node == NULL)
        return;
    
    if (strcmp(node->name, name) == 0) {
        // Imprimir o nó atual e seus descendentes
        if(versionStop != NULL){
        if((strcmp(node->version, versionStop ) == 0)){
            printf("%s %s\n", node->name, node->version);
            return;
        }
        }

        if (!(strcmp(node->version, "") == 0))
        printf("%s %s\n", node->name, node->version);
        // printTree(node->firstChild, depth + 1);
    }

    // Buscar no próximo irmão
    printBranchByName(node->firstChild, name, versionStop);
    printBranchByName(node->nextSibling, name, versionStop);
}
void treeBranchRecursiv(char* branch, TreeNode* root, char* proxBranch, ListNode* listnodeRoot, char* versionStop){
    ListNode* listnodeRootCopy = listnodeRoot;
        
        if(proxBranch == NULL){
            printf("branch inexistente");
            return;
        }

        

        if(strcmp(proxBranch, "Nenhum") == 0){
            return;
        }

        while(!(strcmp(listnodeRootCopy->node->name, proxBranch) == 0)){
            listnodeRootCopy = listnodeRootCopy->next;
        }

        treeBranchRecursiv(branch, root, listnodeRootCopy->node->parentName, listnodeRoot, listnodeRootCopy->node->initVersion);

        if(strcmp(proxBranch, branch) == 0){
            printBranchByName(root, branch , NULL);
        }else{
            printBranchByName(root, proxBranch , versionStop);
        }
}
void treeBranch(char* branch, TreeNode* root){
    ListNode* listnodeRoot =NULL;
    TreeNode* nodeRoot = NULL;
    readMetadataFile("dados.txt",&listnodeRoot, &nodeRoot);
    char* proxBranch = (char*)malloc(sizeof(char));
    strcpy(proxBranch, branch);

    char* versionStop = (char*)malloc(sizeof(char));
    treeBranchRecursiv(branch, root, proxBranch, listnodeRoot, versionStop);
    
    freeList(listnodeRoot);
    freeTree(nodeRoot);
    free(proxBranch);
    free(versionStop);
        
}

