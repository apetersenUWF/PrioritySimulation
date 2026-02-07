#include <iostream>
#include "min-heap.hpp"
#include "queue.hpp"
#include "stats.cpp"
using namespace std;
int main() {
  Queue* fifo = new Queue();
  MinHeap* minHeap = new MinHeap();
  cout << "START" << endl;
  if (!fifo->isEmpty()) return -5;
  int tests = 100;
  int lambda = 5;
  for (int i = 0; i < tests; i++) {
    float rand = getRandomFloat(0, 10);
    cout << rand << endl;
    Customer* curr = new Customer();
    if (curr->getPQT() != -1) return -1;
    if (curr->getNext() != nullptr) return -2;
    curr->setPQT(rand);
    if (curr->getPQT() == -1) return -3;
    if (curr->getNext() != nullptr) return -4;
    fifo->insert(curr);
    
    minHeap->insert(curr);
  }
  cout << "Complete 1" << endl;
  if (fifo->isEmpty()) return -6;
  if (minHeap->isEmpty()) return -7;
  cout << "QUEUE*********" << endl;
  while (!fifo->isEmpty()) {
    Customer* curr = fifo->peek();
    float x = curr->getPQT();
    cout << x << endl;
    fifo->pop();
  }
  cout << "Complete 2" << endl;
  cout << "MINHEAP********" << endl;
  minHeap->print();
  float prev = 0.0;
  while (!minHeap->isEmpty()) {
    float x = minHeap->retrieve();
    if (x < prev) cout << "helpme" << endl;
    cout << x << endl;
    prev = x;
    minHeap->serve();
  }
  cout << "Complete 3";

  delete fifo;
  delete minHeap;
  return 0;
}
