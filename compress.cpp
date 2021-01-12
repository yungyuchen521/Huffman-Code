#include <iostream>
#include "Huffman Tree.h"
#include "minHeap.h"
#include "node.h"
#include <queue>
#include <math.h>
using namespace std;

#define SEPARATOR 255

void compress(char*);
char* getFileType(char*);
char* getCompName(char*);
unordered_map<char, int> getFreqTable(FILE*);
unordered_map<char, char*> getCodeTable(unordered_map<char, int>&);
void fprintHeader(FILE*, char*, int, unordered_map<char, int>&);
void fprintCompressedFile(FILE*, FILE*, unordered_map<char, char*>&);
char getCompressedChar(queue<char>&);
