#ifndef SIM_HPP
#define SIM_HPP
#include "min-heap.hpp"
#include "queue.hpp"
#include "customer.hpp"
#include <string>
const std::string FILE1 = "test1.txt";
const std::string FILE2 = "test2.txt";
class Simulator{
  int lambda;
  int mu;
  int M;
  int EVENTS_TO_SIMULATE;
  float currTime;
  float lastDepartureTime;
  int serversAvailable;
  MinHeap* minHeap;
  Queue* queue;
  std::vector<Customer*> processedCustomers;
  Customer* getNextCustomer(const float currTime) const;
  void processNextEvent();
  public:
  Simulator();
  ~Simulator();
  bool load(const std::string& filename);
  void run();
  void print() const; //for testing purposes
};
#endif