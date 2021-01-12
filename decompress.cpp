#include <iostream>
#include "Huffman Tree.h"
#include <queue>
#include <math.h>

void decompress(char*);
char* getDecompName(char*, char*);
char *readStr(FILE*);
Node* readNode(FILE*);
HuffmanTree& buildTree(FILE*);

void decompress(char *fileName) {
    FILE *file = fopen(fileName, "r");
    
    if (file == NULL) {
        cout << "Fail to open file!" << endl;
        return;
    }
    
    char *type = readStr(file);
    int bitLen = atoi(readStr(file));
    HuffmanTree tree = buildTree(file);
    
    char *decompName = getDecompName(fileName, type);
    FILE *decomp = fopen(decompName, "w");
    
    tree.decode(file, decomp, bitLen);
    
    fclose(file);
    fclose(decomp);
}

char* getDecompName(char *fileName, char *type) {
    char *name = new char[1000];
    strcpy(name, fileName);
    
    name[strlen(name) - strlen("compressed") - 1] = '\0';
    strcat(name, "_decompressed.");
    return strcat(name, type);
}

HuffmanTree& buildTree(FILE *file) {
    MinHeap heap;
    Node *node;
    
    while (true) {
        node = readNode(file);
        if (node == NULL) break;
        heap.insert(node);
    }
    heap.printHeap();
    HuffmanTree *tree = new HuffmanTree(heap);
    
    return *tree;
}

char* readStr(FILE *file) {
    char *str = new char[1000];
    int i = 0;
    
    while (fscanf(file, "%c", &(str[i]))) {
        if (str[i] == (char)SEPARATOR) break;
        i++;
    }
    str[i] = '\0';
    
    return str;
}

Node* readNode(FILE *file) {
    char c;
    
    fscanf(file, "%c", &c);
    if (c == char(SEPARATOR))
        return NULL;
    
    int freq = atoi(readStr(file));
    
    Node *node = new Node(freq, c, NULL, NULL);
    
    return node;
}
