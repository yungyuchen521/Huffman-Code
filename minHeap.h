#ifndef minHeao_h
#define minHeao_h

#include <iostream>
#include "node.h"
using namespace std;

class MinHeap {
public:
    MinHeap();
    MinHeap(int, Node**);
    
    int getSize() const;
    int getCapacity() const;
    bool empty() const;
    
    void insert(Node*);
    Node* extractMin();
    void heapify();
    
    void printHeap() const;
    
private:
    int size, capacity;
    Node **heap;
    
    void heapify(int);
    void expand();
    void shrink();
};

MinHeap::MinHeap():
size(0), capacity(8)
{
    heap = new Node*[capacity];
}

MinHeap::MinHeap(int n, Node **nodes):
size(0), capacity(8)
{
    while (capacity < n * 2) capacity *= 2;
    heap = new Node*[capacity];
    
    for (int i = 0; i < n; i++)
        insert(nodes[i]);
}

int MinHeap::getSize() const { return size; }

int MinHeap::getCapacity() const { return capacity; }

bool MinHeap::empty() const { return size == 0; }

void MinHeap::insert(Node *node) {
    if (size == capacity) expand();
    
    heap[size] = node;
    size++;
    heapify();
}

Node* MinHeap::extractMin() {
    if (size == 0) return NULL;
    
    if (size * 4 < capacity) shrink();
    
    swap(heap[0], heap[size-1]);
    size--;
    heapify();
    
    return heap[size];
}

void MinHeap::heapify() {
    for (int i = (size-1)/2; i >= 0; i--)
        heapify(i);
}

void MinHeap::printHeap() const {
    for (int i = 0; i < size; i++) cout << heap[i];
    cout << endl;
}

// =================== private ===================

void MinHeap::heapify(int i) {
    int least = i;
    int left  = i * 2 + 1;
    int right = i * 2 + 2;
    
    if (left < size && heap[least]->freq >= heap[left]->freq)
        least = left;
    
    if (right < size && heap[least]->freq >= heap[right]->freq)
        least = right;
    
    if (least != i) {
        swap(heap[i], heap[least]);
        heapify(least);
    }
}

void MinHeap::expand() {
    capacity *= 2;
    Node **newHeap = new Node*[capacity];
    
    for (int i = 0; i < size; i++)
        newHeap[i] = heap[i];
    
    free(heap);
    heap = newHeap;
}

void MinHeap::shrink() {
    capacity /= 2;
    Node **newHeap = new Node*[capacity];
    
    for (int i = 0; i < size; i++)
        newHeap[i] = heap[i];
    
    free(heap);
    heap = newHeap;
}


#endif /* minHeao_h */
