#ifndef MINHEAP_HPP
#define MINHEAP_HPP
#include "customer.hpp"
#include <vector>
const int DEFAULT_MINHEAP_CAPACITY = 200;//defines the default size for the min heap
class MinHeap{
  int size;//holds the effective size
  int capacity;//holds the maximum capacity
  std::vector<Customer*> priorityQueue;////holds all the customers in a minHeap structure
  void percolateUp(int index);//used internally to maintain structure
  void percolateDown(int index);//used internally to maintain structure
  int parent(int index) const;//returns parent given an index
  int LChild(int index) const;//returns lchild given an index
  int RChild(int index) const;//returns rchild given an index
  void swap(int indexA, int indexB);//swaps two customers positions, used for maintaining structure
  public:
  MinHeap();
  MinHeap(int capacity);
  ~MinHeap();
  bool insert(Customer* customerToInsert);//inserts a customer into the minHeap
  Customer* serve();//pops the top customer and returns it
  bool isEmpty() const;//checks if the heap is empty
  int getSize() const;//returns effective size of the heap
  float retrieve() const;//returns the value of the highest priority event
};
#endif