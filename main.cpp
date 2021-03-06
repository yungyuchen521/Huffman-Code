#include "Huffman Tree.h"
#include "compress.h"
#include "decompress.h"
using namespace std;

#define SEPARATOR 255

bool compareFiles(char*, char*); // test if the original and the decompressed file match

int main(int argc, const char * argv[]) {
    char fileName[] = "test.txt"; // set the name of file to be compressed
    
    // compress.h
    compress(fileName);
    char *compName = getCompName(fileName);
    
    // decompress.h
    decompress(compName);
    char *decompName = getDecompName(compName, getFileType(fileName));
    
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
