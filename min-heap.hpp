#ifndef MINHEAP_HPP
#define MINHEAP_HPP
#include "customer.hpp"
#include <vector>
const int DEFAULT_MINHEAP_CAPACITY = 200;
class MinHeap{
  int size;
  int capacity;
  std::vector<Customer*> priorityQueue;
  void percolateUp(int index);
  void percolateDown(int index);
  int parent(int index) const;
  int LChild(int index) const;
  int RChild(int index) const;
  void swap(int indexA, int indexB);
  public:
  MinHeap();
  MinHeap(int capacity);
  ~MinHeap();
  void insert(Customer* customerToInsert);
  void serve();
  bool isEmpty() const;
  float retrieve() const;
  void print() const; //for testing
};
#endif