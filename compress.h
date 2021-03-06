#ifndef compress_h
#define compress_h

#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include "Huffman Tree.h"
using namespace std;

#define SEPARATOR 255 // a special character (ˇ) rarely used to serve as the separator in the header of the compressed file ( for better readabilty )

void compress(char*);

char* getFileType(char*); // get the file type from the given file name
char* getCompName(char*); // generate the file name for the compressed file ( {file name}.compressed )

unordered_map<char, int> getFreqTable(FILE*); // read the original file and count the occurrence of each unique character
unordered_map<char, char*> getCodeTable(unordered_map<char, int>&); // get the Huffman code of each character

void fprintHeader(FILE*, char*, int, unordered_map<char, int>&); 
// {file type}ˇ{ file length in bit (excluding the header) }ˇ{ char1 }ˇ{ occurrence1 }ˇ{ char2 }ˇ{ occurrence2 }ˇ{ char3 }ˇ{ occurrence3 }ˇ...ˇˇ

void fprintCompressedFile(FILE*, FILE*, unordered_map<char, char*>&);
char getCompressedChar(queue<char>&);

void compress(char *fileName) {
    FILE *file = fopen(fileName, "r");
    
    if (file == NULL) {
        cout << "Fail to open file!" << endl;
        return;
    }
    
    unordered_map<char, int> freqTable = getFreqTable(file);
    unordered_map<char, char*> codeTable = getCodeTable(freqTable);
    
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

char* getCompName(char *fileName) {
    char *name = new char[1000];
    
    for (int i = 0; i < strlen(fileName); i++) {
        name[i] = fileName[i];
        
        if (fileName[i] == '.')
            break;
    }
    
    return strcat(name, "compressed");
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
    
    priority_queue<Node*, vector<Node*>, Comparison> heap (nodes, nodes+size);
    HuffmanTree tree(heap);
    
    return tree.getCodes();
}

void fprintHeader(FILE *file, char *type, int len, unordered_map<char, int> &freqTable) {
    fprintf(file, "%s%c%d%c", type, SEPARATOR, len, SEPARATOR);
    
    unordered_map<char, int>::iterator it = freqTable.begin();
    while (it != freqTable.end()) {
        fprintf(file, "%c%d%c", it->first, it->second, SEPARATOR);
        it++;
    }
    
    fprintf(file, "%c", SEPARATOR);
}

void fprintCompressedFile(FILE *input, FILE *output, unordered_map<char, char*> &codeTable) {
    
    fseek(input, 0, SEEK_SET);
    char c;
    queue<char> buffer;
    
    while (fscanf(input, "%c", &c) != EOF) {
        char *code = codeTable[c];
        
        for (int i = 0; i < strlen(code); i++) {
            buffer.push(code[i]);
            
            if (buffer.size() == 8) {
                fprintf(output, "%c", getCompressedChar(buffer));
            }
        }
    }
    
    if (buffer.size() != 0) {
        while (buffer.size() <= 8) {
            buffer.push('0');
        }
        fprintf(output, "%c", getCompressedChar(buffer));
    }
}

char getCompressedChar(queue<char> &buffer) {
    char c = 0;
    
    for (int i = 7; i >= 0; i--) {
        if (buffer.front() == '1')
            c += pow(2, i);
        
        buffer.pop();
    }
    
    return c;
}

#endif /* compress_h */
