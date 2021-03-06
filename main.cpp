#include "Huffman Tree.h"
#include "compress.h"
#include "decompress.h"
using namespace std;

#define SEPARATOR 255

bool compareFiles(char*, char*);
/*
void compress(char*);
char* getFileType(char*);
char* getCompName(char*);
unordered_map<char, int> getFreqTable(FILE*);
unordered_map<char, char*> getCodeTable(unordered_map<char, int>&);
void fprintHeader(FILE*, char*, int, unordered_map<char, int>&);
void fprintCompressedFile(FILE*, FILE*, unordered_map<char, char*>&);
char getCompressedChar(queue<char>&);

void decompress(char*);
char* getDecompName(char*, char*);
char *readStr(FILE*);
Node* readNode(FILE*);
HuffmanTree& buildTree(FILE*);
 */

int main(int argc, const char * argv[]) {
    
    char *fileName = new char[10000];
    strcpy(fileName, argv[1]);
    
    cout << "original file: " << fileName << endl;
    compress(fileName);
    
    char *compName = getCompName(fileName);
    cout << "compressed file: " << compName << endl;
    decompress(compName);
    
    char *decompName = getDecompName(compName, getFileType(fileName));
    cout << "decompressed file: " << decompName << endl;
    
    /*
    char fileName[] = "test4.txt";
    
    compress(fileName);
    
    char *compName = getCompName(fileName);
    decompress(compName);
    
    char *decompName = getDecompName(compName, getFileType(fileName));
    */
    compareFiles(fileName, decompName);
    
    return 0;
}

bool compareFiles(char *fileName1, char *fileName2) {
    bool EOF1, EOF2;
    
    FILE *file1 = fopen(fileName1, "r");
    FILE *file2 = fopen(fileName2, "r");
    
    if (file1 == NULL || file2 == NULL) {
        cout << "Fail to open files!" << endl;
        return false;
    }
    
    char c1, c2;
    
    while (true) {
        EOF1 = (fscanf(file1, "%c", &c1) == EOF);
        EOF2 = (fscanf(file2, "%c", &c2) == EOF);
        
        if ((EOF1 || EOF2) || (c1 != c2))
            break;
    }

    fclose(file1);
    fclose(file2);
    
    if (EOF1 && EOF2) {
        cout << "100% match." << endl;
        return true;
    }
    
    else {
        cout << "The files don't match." << endl;
        return false;
    }
}
/*
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

// =============================================
// =============================================
// =============================================

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
*/
