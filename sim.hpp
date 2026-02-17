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
  int serversAvailable;
  MinHeap* minHeap;
  Queue* queue;
  Customer* currEvent;
  std::vector<Customer*> processedCustomers;
//STATISTICS
  float eventTime;
  float lastEventTime;
  float lastQueueChange;
  int customersInSystem;
  float totalTime;
  float totalTimeInSystem;
  float totalServiceTime;
  float totalQueueTime;
  float idleTime;
  float runningTotals[20];
  float queueRunningTotals[20];

  //stats

  void processNextEvent();
  void processStatistics();
  void accumulateIdleTime();
  void accumulateRunningTotals();
  public:
  Simulator();
  ~Simulator();
  bool load(const std::string& filename);
  void run();
  void print() const; //for testing purposes
};
#endif