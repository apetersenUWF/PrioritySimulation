/***************************************************************
  Ayden Petersen
  min-heap.cpp
  Project 2

  This file contains implementations of the MinHeap class methods
  These methods implement the Heap ADT, AKA Priority Queue which maintains 
  a structure that always keeps the minimum value (highest priority) on 
  the top and allows for quick O(logn) insertions and deletions
***************************************************************/
#include "min-heap.hpp"
  MinHeap::MinHeap(){
    size = 1;//size starts as 1 because 0th node is a sentinel
    capacity = DEFAULT_MINHEAP_CAPACITY;
    priorityQueue.assign(capacity, nullptr);
    priorityQueue.at(0) = new Customer();//sentinel
    priorityQueue.at(0)->setPQT(0);//sentinel always equal to 0, negative values wont be allowed
  }

  MinHeap::MinHeap(int capacity) {
    size = 1;
    this->capacity = capacity;
    priorityQueue.assign(capacity, nullptr);
    priorityQueue.at(0) = new Customer();
    priorityQueue.at(0)->setPQT(0);
  }
  MinHeap::~MinHeap() {
    for (int i = 0; i < size; i++) delete priorityQueue.at(i);
  }
  bool MinHeap::insert(Customer* customerToInsert) {//inserts a customer into the minheap, returns true if successful
    if (size >= capacity) return false;//PQ full
    priorityQueue.at(size) = customerToInsert;//insert at the bottom, percolate up
    percolateUp(size);
    size++;
    return true;
  }

  Customer* MinHeap::serve() {//removes the front customer from the PQ, fixes the structure, and returns that customer
    if (size == 1) return nullptr;//PQ empty
    Customer* servedCustomer = new Customer(priorityQueue.at(1));//saves the customer pointer for returning
    delete priorityQueue.at(1);
    priorityQueue.at(1) = priorityQueue.at(size-1);//move last element to the top and percolate down
    size--;
    percolateDown(1);
    return servedCustomer;
  }

  void MinHeap::percolateUp(int index) {
    int parentIndex = parent(index);
    Customer* curr = priorityQueue.at(index);
    Customer* currParent = priorityQueue.at(parentIndex);

    while (curr->getPQT() < currParent->getPQT()){//curr can never be less than element 0, which is 0
      swap(index, parentIndex);//keep swapping while parents have lower priority
      index = parentIndex;
      parentIndex = parent(index);
      currParent = priorityQueue.at(parentIndex);
    }
  }

  void MinHeap::percolateDown(int index) {
    float curr = priorityQueue.at(index)->getPQT();

    while (index <= size/2) {//nodes below size/2 have children
      //keep swapping while a child has higher priority
      int lChildIndex = LChild(index);
      float lChild = priorityQueue.at(lChildIndex)->getPQT();
      if (index * 2 == size) {//node only has left child
        if (lChild < curr) swap(index, lChildIndex);
        return;
      }
      else {//node has 2 children
        int rChildIndex = RChild(index);
        float rChild = priorityQueue.at(rChildIndex)->getPQT();
        if (curr < lChild && curr < rChild) return;//curr has higher priority than its children
        if (lChild < rChild) {//left child has highest priority among the 3
          swap(index, lChildIndex);
          index = lChildIndex;
        }
        else {//right child must have highest priority
          swap(index, rChildIndex);
          index = rChildIndex;
        }
      }
    }
  }

  int MinHeap::parent(int index) const {return index/2;}
  int MinHeap::LChild(int index) const {return index*2;}
  int MinHeap::RChild(int index) const {return index*2 + 1;}
  void MinHeap::swap(int indexA, int indexB) {//swaps 2 customers positions
    std::swap(priorityQueue.at(indexA), priorityQueue.at(indexB));
  }
  bool MinHeap::isEmpty() const {
    if (size <= 1) return true;//1 based 
    else return false;
  }
  float MinHeap::retrieve() const {return priorityQueue.at(1)->getPQT();}//returns the top customer without modifying

  int MinHeap::getSize() const {return size;}
  