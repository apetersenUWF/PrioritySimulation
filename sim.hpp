#ifndef SIM_HPP
#define SIM_HPP
#include "min-heap.hpp"
#include "queue.hpp"
#include "customer.hpp"
#include <string>
const std::string FILE1 = "test1.txt";
const std::string FILE2 = "test2.txt";
class Simulator{
  int lambda;//avergae arrivals per time unit
  int mu;//avergae services completed per time unit
  int M;//number of servers
  int EVENTS_TO_SIMULATE;//number of events to simulate
  int serversAvailable;//number of available servers
  MinHeap* minHeap;//PQ
  Queue* queue;//FIFO queue
  Customer* currEvent;//current event being processed, highest priority
  void processNextEvent();
//STATISTICS
  int customersInSystem;//keeps track of the number of customers in the system
  int customersWithWaitTime;//keeps track of the number of customers who had to wait
  float eventTime;//keeps track of the time the current event took place
  float lastEventTime;//keeps track of the time the last event took place
  float lastQueueChange;//keeps track of the last time the queue size changed
  float totalTime;//total time of the simulation
  float totalTimeInSystem;//keeps track of the total time all customers spend in the system
  float totalServiceTime;//keeps track of the total time customers spend getting service
  float totalQueueTime;//keeps track of the total time customers spend in the queue
  float idleTime;//keeps track of the amount of time nobody is in the system
  float runningTotals[50];//assumes no more than 49 customers will be in the system at once
  float queueRunningTotals[50];//assumes no more than 49 customers will be in the queue at once

  float Po;//percent idle time
  float L;//avg # in system
  float W;//avg time in the system
  float Lq;//avg # in the queue
  float Wq;//avg time in the queue
  float Rho;//utilization factor
  float chanceOfWaiting;//probability of having to enter the queue before being served

  //stats

  void processSimulationStatistics();//process the data gathered in the simulation
  void accumulateIdleTime();
  void accumulateRunningTotals();

  public:
  Simulator();
  ~Simulator();
  bool load(const std::string& filename);//load starting values from a file
  void run();//run the simulation
  void print() const;//print the results
};
#endif