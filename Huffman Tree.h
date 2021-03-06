#ifndef Huffman_Tree_h
#define Huffman_Tree_h

#include "Node.h"
#include <queue>
#include <vector>
#include <string.h>
#include <math.h>
#include <unordered_map>
using namespace std;

class HuffmanTree {
public:
    HuffmanTree();
    HuffmanTree(priority_queue<Node*, vector<Node*>, Comparison>&);
    
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

HuffmanTree::HuffmanTree(priority_queue<Node*, vector<Node*>, Comparison> &nodes):
root(NULL)
{
    // the file is empty
    if (nodes.empty()) {
        root = new Node(0, '\0', NULL, NULL);
    }
    
    // the file consists of only 1 different character
    else if (nodes.size() == 1) {
        Node *node = nodes.top();
        nodes.pop();
        root = new Node(node->freq, '\0', node, NULL);
    }
    
    // the file consists of more than 1 different character
    else {
        while (!nodes.empty()) {
            Node *n1, *n2;
            n1 = nodes.top();
            nodes.pop();
            n2 = nodes.top();
            nodes.pop();
            
            Node *newNode = new Node(n1->freq + n2->freq, '\0', n1, n2);
            
            if (nodes.empty())
                this->root = newNode;
            else
                nodes.push(newNode);
        }
    }
}

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

unordered_map<char, char*> HuffmanTree::getCodes() const {
    char *code = new char[300];
    code[0] = '\0';
    unordered_map<char, char*> table;
    
    getCodes(root, code, table);
    
    return table;
}

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
            
            if (!node->left && !node->right) {
                fprintf(decomp, "%c", node->key);
                node = root;
            }
        }
    }
}

char* HuffmanTree::charToBinary(char c) {
    char *binary = new char[9];
    int bin = (int)c;
    if (bin < 0) bin += 256;
    
    for (int i = 0; i < 8; i++) {
        int p = pow(2, 8 - i - 1);
        
        binary[i] = (bin / p == 1) ? '1' : '0';
        bin %= p;
    }
    binary[8] = '\0';
    
    return binary;
}

#endif /* Huffman_Tree_h */
