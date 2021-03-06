#ifndef decompress_h
#define decompress_h

#include <iostream>
#include <queue>
#include <vector>
#include "Huffman Tree.h"
using namespace std;

void decompress(char*);
char* getDecompName(char*, char*); // generate the file name of the decompressed file from the given file name and type
char *readStr(FILE*); // read the header of the compressed file
Node* readNode(FILE*); // read the header of the compressed file
HuffmanTree& buildTree(FILE*); // rebuild the tree with the info provided in the header of the compressed file

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
    priority_queue<Node*, vector<Node*>, Comparison> heap;
    
    while (true) {
        Node *node;
        node = readNode(file);
        if (node == NULL) break;
        heap.push(node);
    }
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

#endif /* decompress_h */
