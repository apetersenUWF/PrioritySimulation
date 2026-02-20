#include <iostream>
#include "sim.hpp"
#include "stats.hpp"
using namespace std;
int testQueues();
void testSim();
int main() {
  //cout << testQueues();
  testSim();
  return 0;
}

int testQueues() {//runs some tests for the fifo and pq
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
  Simulator* sim = new Simulator();
  sim->load(FILE1);
  sim->run();
  delete sim;
  sim = new Simulator();
  sim->load(FILE2);
  sim->run();
}