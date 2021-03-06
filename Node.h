#ifndef node_h
#define node_h

#include <iostream>
using namespace std;

struct Node {
    int freq;
    char key;
    Node *left, *right;
    
    Node(int f, char k, Node *n1, Node *n2) {
        freq = f;
        key = k;
        left = n1;
        right = n2;
    }
    
    void operator = (Node &n) {
        if (this == &n) return;
        
        freq = n.freq;
        key = n.key;
        left = n.left;
        right = n.right;
    }
    
    friend ostream& operator << (ostream &os, Node &n) {
        os << "(" << n.freq << ", " << n.key << ") ";
        return os;
    }
    
    friend ostream& operator << (ostream &os, Node *n) {
        os << "(" << n->freq << ", " << n->key << ") ";
        return os;
    }
};

// for priority_queue
class Comparison {
public:
    bool operator() (Node *n1, Node *n2) {
        return (n1->freq == n2->freq) ? (n1->key > n2->key) : (n1->freq > n2->freq);
    }
};

#endif /* node_h */
