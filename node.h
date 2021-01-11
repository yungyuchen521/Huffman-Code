#ifndef node_h
#define node_h

#include <iostream>
using namespace std;

struct Node {
    int freq; // the number of appearances of the key
    char key; // the character
    Node *left, *right; // points to the left and right children of the node (for the HuffmanTree structure)
    
    // ==============================================================================================
    // it is a good habit to define a constrcutor and overload the assignment operator for the struct 
    // ==============================================================================================
    
    // a constructor for Node
    Node(int f, char k, Node *n1, Node *n2) {
        freq = f;
        key = k;
        left = n1;
        right = n2;
    }
    
    // assignment operator overloading
    void operator = (Node &n) {
        if (this == &n) return;
        
        freq = n.freq;
        key = n.key;
        left = n.left;
        right = n.right;
    }
    
    // =====================================================
    // the 2 below are not necessary 
    // simply makes printing Node to the console a lot easier
    // ======================================================
    
    // overload 'cout <<' operator
    friend ostream& operator << (ostream &os, Node &n) {
        os << "(" << n.freq << ", " << n.key << ") ";
        return os;
    }
    
    // overload 'cout <<' operator for pointers to Node
    friend ostream& operator << (ostream &os, Node *n) {
        os << "(" << n->freq << ", " << n->key << ") ";
        return os;
    }
};


#endif /* node_h */
