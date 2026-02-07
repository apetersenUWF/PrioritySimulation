#include "min-heap.hpp"
#include <iostream>
  MinHeap::MinHeap(){
    size = 1;
    capacity = DEFAULT_MINHEAP_CAPACITY;
    priorityQueue.resize(capacity);
    priorityQueue.at(0) = new Customer();
    priorityQueue.at(0)->setPQT(0);
  }

  MinHeap::MinHeap(int capacity) {
    size = 1;
    this->capacity = capacity;
    priorityQueue.resize(capacity);
    priorityQueue.at(0) = new Customer();
    priorityQueue.at(0)->setPQT(0);
  }

  void MinHeap::insert(Customer* customerToInsert) {
    if (size == capacity) return;//PQ full
    priorityQueue.at(size) = customerToInsert;
    percolateUp(size);
    size++;
  }

  void MinHeap::serve() {
    if (size == 1) return;//PQ empty
    priorityQueue.at(1) = priorityQueue.at(size-1);
    size--;
    percolateDown(1);
  }

  void MinHeap::percolateUp(int index) {
    int parentIndex = parent(index);
    Customer* curr = priorityQueue.at(index);
    Customer* currParent = priorityQueue.at(parentIndex);
    //std::cout << "**PERC UP**" << std::endl;
    //std::cout << curr->getPQT() << " compared with parent " << currParent->getPQT() << std::endl;

    while (curr->getPQT() < currParent->getPQT()){//curr can never be less than element 0, which is 0
      swap(index, parentIndex);
      index = parentIndex;
      parentIndex = parent(index);
      currParent = priorityQueue.at(parentIndex);
      //std::cout << "SWAP" << std::endl;
      //std::cout << curr->getPQT() << " compared with parent " << currParent->getPQT() << std::endl;
    }
    //std::cout << "**PERCUP OVER**" << std::endl;
  }

  void MinHeap::percolateDown(int index) {
    float curr = priorityQueue.at(index)->getPQT();

    while (index <= size/2) {//nodes below size/2 have children
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
        if (lChild < rChild) {
          swap(index, lChildIndex);
          index = lChildIndex;
        }
        else {
          swap(index, rChildIndex);
          index = rChildIndex;
        }
      }
    }
  }

  int MinHeap::parent(int index) const {return index/2;}
  int MinHeap::LChild(int index) const {return index*2;}
  int MinHeap::RChild(int index) const {return index*2 + 1;}
  void MinHeap::swap(int indexA, int indexB) {
    std::swap(priorityQueue.at(indexA), priorityQueue.at(indexB));
  }
  bool MinHeap::isEmpty() const {
    if (size <= 1) return true;//1 based 
    else return false;
  }
  float MinHeap::retrieve() const {return priorityQueue.at(1)->getPQT();}

  void MinHeap::print() const {
    for (int i = 0; i < size; i++) {
      if (priorityQueue.at(i) != nullptr)
        std::cout << "*" << i << "* " << std::fixed << priorityQueue.at(i)->getPQT() << std::endl;
      else
        std::cout << "null" << std::endl;
    }
  }
  