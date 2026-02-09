#include <iostream>
#include "sim.hpp"
using namespace std;
int testQueues();
void testSim();
int main() {
  //cout << testQueues();
  testSim();
  return 0;
}

int testQueues() {
  Queue* fifo = new Queue();
  MinHeap* minHeap = new MinHeap();
  const int tests_count = 100;
  for (int i = 0; i < tests_count; i++) {//tests inserting elements into minheap
    float rand = getRandomFloat();
    Customer* curr = new Customer();
    curr->setPQT(rand);
    minHeap->insert(curr);
  }
  if (minHeap->isEmpty()) return -1;//makes sure the heap is not empty
  float prev = 0.0;
  while (!minHeap->isEmpty()) {//verifies it is a valid minheap
    float x = minHeap->serve()->getPQT();
    if (x < prev) return -2;
    prev = x;
  }
  cout << "MINHEAP SUCCESS" << endl;
  for (int i = 0; i < tests_count; i++) {//tests inserting into the fifo queue
    float rand = getRandomFloat();
    Customer* curr = new Customer();
    curr->setPQT(rand);
    fifo->insert(curr);
  }
  if (fifo->isEmpty()) return -3;//verifies the fifo is not empty
  while (!fifo->isEmpty()) {//prints out the pqtime values for each customer in the fifo
    cout << fifo->pop()->getPQT() << endl;
  }
  delete minHeap;
  delete fifo;
  fifo = new Queue();
  minHeap = new MinHeap();
  Customer* test1 = new Customer();
  test1->setPQT(0.5);
  fifo->insert(test1);
  minHeap->insert(fifo->pop());//tests popping out of the fifo and into the minheap
  if (minHeap->isEmpty()) return -4;
  if (!fifo->isEmpty()) return -5;
  cout << "SUCCESS" << endl;
  return 100;
}
void testSim() {
  Simulator* sim1 = new Simulator();
  Simulator* sim2 = new Simulator();
  sim1->load(FILE1);
  sim2->load(FILE2);
  sim1->print();
  sim2->print();
  delete sim1;
  delete sim2;
}