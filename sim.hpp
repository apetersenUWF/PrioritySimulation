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
  void processNextEvent();
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
  float runningTotals[50];//assumes no more than 49 customers will be in the system at once
  float queueRunningTotals[50];//assumes no more than 49 customers will be in the queue at once

  float Po;
  float L;
  float W;
  float Lq;
  float Wq;
  float Rho;

  //stats

  void processStatistics();
  void accumulateIdleTime();
  void accumulateRunningTotals();

  public:
  Simulator();
  ~Simulator();
  bool load(const std::string& filename);
  void run();
  void print() const;
};
#endif