#ifndef Huffman_Tree_h
#define Huffman_Tree_h

#include <iostream>
#include "node.h"
#include "minHeap.h"
#include <string.h>
#include <math.h>
#include <unordered_map>
#include <queue>
using namespace std;

class HuffmanTree {
public:
    HuffmanTree();
    HuffmanTree(MinHeap&);
    
    unordered_map<char, char*> getCodes() const;
    void decode(FILE*, FILE*, int);
    void printTree() const;

private:
    Node *root;
    
    void getCodes(Node*, char*, unordered_map<char, char*>&) const;
    void printPreOrder(Node*) const;
    void printPostOrder(Node*) const;
    static char* charToBinary(char);
};

HuffmanTree::HuffmanTree():
root(NULL)
{}

// construct the tree with the priority queue (MinHeap)
HuffmanTree::HuffmanTree(MinHeap &nodes):
root(NULL)
{
    // handle the exception which the file is empty
    if (nodes.empty()) {
        root = new Node(0, '\0', NULL, NULL);
    }
    
    // handle the exception which the file consists of only 1 different character
    else if (nodes.getSize() == 1) {
        Node *node = nodes.extractMin();
        root = new Node(node->freq, '\0', node, NULL);
    }
    
    // in normal circumstances
    // the file consists of more than 1 different characters
    else {
        while (!nodes.empty()) {
            Node *n1, *n2;
            n1 = nodes.extractMin();
            n2 = nodes.extractMin();
            
            Node *newNode = new Node(n1->freq + n2->freq, '\0', n1, n2);
            
            if (nodes.empty())
                this->root = newNode;
            else
                nodes.insert(newNode);
        }
    }
}

// the pre-order and post-order traversal can determine an unique binary tree
void HuffmanTree::printTree() const {
    if (root == NULL)
        return;
    
    cout << "PreOrder: ";
    printPreOrder(root);
    cout << endl;
    cout << "PostOrder: ";
    printPostOrder(root);
    cout << endl;
}

void HuffmanTree::printPreOrder(Node *node) const {
    cout << node;
    if (node->left) printPreOrder(node->left);
    if (node->right) printPreOrder(node->right);
}

void HuffmanTree::printPostOrder(Node *node) const {
    if (node->left) printPostOrder(node->left);
    if (node->right) printPostOrder(node->right);
    cout << node;
}

// generates the Huffman codes for all characters
unordered_map<char, char*> HuffmanTree::getCodes() const {
    char *code = new char[300];
    code[0] = '\0';
    unordered_map<char, char*> table;
    
    getCodes(root, code, table);
    
    return table;
}

// travel all nodes in the tree to get Huffman code of each
void HuffmanTree::getCodes(Node *node, char *code, unordered_map<char, char*> &table) const {
    if (!node->left && !node->right)
        table[node->key] = code;
    
    else {
        unsigned long len = strlen(code);
        
        if (node->left) {
            char *c = new char[300];
            strcpy(c, code);
            c[len] = '0';
            c[len+1] = '\0';
            
            getCodes(node->left, c, table);
        }
        
        if (node->right) {
            char *c = new char[300];
            strcpy(c, code);
            c[strlen(c)] = '1';
            c[len+1] = '\0';
            
            getCodes(node->right, c, table);
        }
    }
}

void HuffmanTree::decode(FILE *file, FILE *decomp, int bitLen) {
    char c;
    char *code = new char[10];
    int bitCount = 0;
    Node *node = root;
    
    while (bitCount < bitLen) {
        fscanf(file, "%c", &c);
        code = charToBinary(c);
        
        for (int i = 0; i < 8 && bitCount < bitLen; i++, bitCount++) {
            node = (code[i] == '0') ? node->left : node->right;
            
            // we've arrived at a leaf, so a character is identified
            if (!node->left && !node->right) {
                fprintf(decomp, "%c", node->key);
                node = root;
            }
        }
    }
}

// an utility function to convert the int value of a character
// to a binary number (as an array of characters)
char* HuffmanTree::charToBinary(char c) {
    char *binary = new char[9];
    int bin = (int)c;           // the int value of char in C++ is within [-128, 127]
    if (bin < 0) bin += 256;    // those greater than 127 overflows and start from -128
                                // exmaple: (char)128 -> -128, (char)129 -> -127, (char)255 -> -1, (char)254 -> -2 ... 
                                // but we want: (char)128 -> 128, (char)129 -> 129, (char)255 -> 255, (char)254 -> 254 ... 
    
    for (int i = 0; i < 8; i++) {
        int p = pow(2, 8 - i - 1);
        
        binary[i] = (bin / p == 1) ? '1' : '0';
        bin %= p;
    }
    binary[8] = '\0';
    
    return binary;
}

#endif /* Huffman_Tree_h */
