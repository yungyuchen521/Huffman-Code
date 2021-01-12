#include <iostream>
#include "Huffman Tree.h"
#include "minHeap.h"
#include "node.h"
#include <queue>
#include <math.h>
using namespace std;

#define SEPARATOR 255

void compress(char*);

unordered_map<char, int> getFreqTable(FILE*);
unordered_map<char, char*> getCodeTable(unordered_map<char, int>&);

void fprintHeader(FILE*, char*, int, unordered_map<char, int>&);
void fprintCompressedFile(FILE*, FILE*, unordered_map<char, char*>&);

char* getFileType(char*);
char* getCompName(char*);
char getCompressedChar(queue<char>&);

void compress(char *fileName) {
    FILE *file = fopen(fileName, "r");
    
    if (file == NULL) {
        cout << "Fail to open file!" << endl;
        return;
    }
    
    unordered_map<char, int> freqTable = getFreqTable(file);
    unordered_map<char, char*> codeTable = getCodeTable(freqTable);
    
    unordered_map<char, char*>::iterator it0 = codeTable.begin();
    while (it0 != codeTable.end()) {
        printf("%c: %s\n", it0->first, it0->second);
        it0++;
    }
    
    char *type = getFileType(fileName);
    char *compressName = getCompName(fileName);
    FILE *comp = fopen(compressName, "w");
    
    int len = 0;
    unordered_map<char, int>::iterator it = freqTable.begin();
    
    for (; it != freqTable.end(); it++) {
        len += strlen(codeTable[it->first]) * it->second;
    }
    
    fprintHeader(comp, type, len, freqTable);
    fseek(file, 0, SEEK_SET);
    fprintCompressedFile(file, comp, codeTable);
    
    fclose(file);
    fclose(comp);
}

unordered_map<char, int> getFreqTable(FILE *file) {
    unordered_map<char, int> freqTable;
    char c;
    
    while (fscanf(file, "%c", &c) != EOF) {
        if (freqTable.find(c) == freqTable.end())
            freqTable[c] = 1;
        else
            freqTable[c]++;
    }
    
    return freqTable;
}

unordered_map<char, char*> getCodeTable(unordered_map<char, int> &freqTable) {
    int size = freqTable.size();
    int i = 0;
    Node **nodes = new Node*[size];
    unordered_map<char, int>::iterator it = freqTable.begin();
    
    while (it != freqTable.end()) {
        nodes[i] = new Node(it->second, it->first, NULL, NULL);
        it++;
        i++;
    }
    
    MinHeap heap(size, nodes);
    HuffmanTree tree(heap);
    
    return tree.getCodes();
}

// print the info needed for future decompression
// file type, length of the file, appearances of each character
void fprintHeader(FILE *file, char *type, int len, unordered_map<char, int> &freqTable) {
    fprintf(file, "%s%c%d%c", type, SEPARATOR, len, SEPARATOR);
    
    unordered_map<char, int>::iterator it = freqTable.begin();
    while (it != freqTable.end()) {
        fprintf(file, "%c%d%c", it->first, it->second, SEPARATOR);
        it++;
    }
    
    fprintf(file, "%c", SEPARATOR); // another SEPARATOR to indicate the end of the header
}

void fprintCompressedFile(FILE *input, FILE *output, unordered_map<char, char*> &codeTable) {
    fseek(input, 0, SEEK_SET);
    char c;
    queue<char> buffer;
    
    
    while (fscanf(input, "%c", &c) != EOF) {
        char *code = codeTable[c];
        
        for (int i = 0; i < strlen(code); i++) {
            buffer.push(code[i]);
            
            if (buffer.size() == 8) { // each character consists of 8 bits
                fprintf(output, "%c", getCompressedChar(buffer));
            }
        }
    }
    
    // 
    if (buffer.size() != 0) {
        while (buffer.size() <= 8) {
            buffer.push('0');
        }
        fprintf(output, "%c", getCompressedChar(buffer));
    }
}

// find out the type of the file to be compressed
char* getFileType(char *fileName) {
    char *type = new char[1000];
    unsigned long i = strlen(fileName) - 1;
    while (i > 0 && fileName[i] != '.') i--;
    
    if (i == 0)
        return type;
    
    i++;
    for (int j = 0; i + j < strlen(fileName); j++) {
        type[j] = fileName[i+j];
    }
    
    return type;
}

// generate the name of the compressed file
char* getCompName(char *fileName) {
    char *name = new char[1000];
    
    for (int i = 0; i < strlen(fileName); i++) {
        name[i] = fileName[i];
        
        if (fileName[i] == '.')
            break;
    }
    
    return strcat(name, "compressed");
}

// return the corresponding character to the binary number in the buffer
char getCompressedChar(queue<char> &buffer) {
    char c = 0;
    
    for (int i = 7; i >= 0; i--) {
        if (buffer.front() == '1')
            c += pow(2, i);
        
        buffer.pop();
    }
    
    return c;
}
