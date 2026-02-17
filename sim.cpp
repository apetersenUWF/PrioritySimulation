#include "sim.hpp"
#include "stats.hpp"
#include "fstream"
#include <iostream>
  void Simulator::run() {
    float customerArrivalTime = 0.0;
    float interval = getNextRandomInterval(lambda);
    int i = 0;//event counter
    serversAvailable = M;//set all servers to be free
    customerArrivalTime += interval;
    while (minHeap->getSize() < DEFAULT_MINHEAP_CAPACITY && i < EVENTS_TO_SIMULATE) {
      Customer* curr = new Customer(customerArrivalTime);
      minHeap->insert(curr);
      interval = getNextRandomInterval(lambda);
      customerArrivalTime += interval;//accumulate time
      i++;
    }
    while (!minHeap->isEmpty()) {
      processNextEvent();
        if(i < EVENTS_TO_SIMULATE && minHeap->getSize() <= M+1) {
          Customer* curr = new Customer(customerArrivalTime);
          minHeap->insert(curr);
          interval = getNextRandomInterval(lambda);
          customerArrivalTime += interval;//accumulate time
          i++;
        }
    }
    totalTime = currEvent->getDT();
    processSimulationStatistics();
    print();
  }
  void Simulator::processNextEvent() {
    currEvent = minHeap->serve();//update the current event being processed
    lastEventTime = eventTime;
    eventTime = currEvent->getPQT();
    //can be either an arrival or departure of a customer
    if (currEvent->isArrival()) {
      accumulateRunningTotals();
      if (serversAvailable > 0) {
        if (serversAvailable == M) accumulateIdleTime();//if someone just arrived while all servers were available, we have lastDT and lastAT to calculate idleTime
        serversAvailable--;
        currEvent->setSOST(currEvent->getAT());
        float serviceTime = getNextRandomInterval(mu);
        float departureTime = currEvent->getSOST() + serviceTime;
        currEvent->setDT(departureTime);
        currEvent->setPQT(departureTime);
        minHeap->insert(currEvent);
        totalServiceTime += serviceTime;
      }
      else {
        queueRunningTotals[queue->getSize()] += currEvent->getAT() - lastQueueChange;
        queue->insert(currEvent);//if most recent event was an arrival but no servers available, put in line
        lastQueueChange = currEvent->getAT();
      }
    }
    else {//event is a departure
      accumulateRunningTotals();
      serversAvailable++;//free up a server
      if (!queue->isEmpty()) {//if someone is waiting in line as an arrival
        queueRunningTotals[queue->getSize()] += currEvent->getDT() - lastQueueChange;
        Customer* frontOfQueue = queue->pop();//pull them from the queue, insert their departure event
        frontOfQueue->setSOST(currEvent->getDT());
        float serviceTime = getNextRandomInterval(mu);
        float departureTime = frontOfQueue->getSOST() + serviceTime;
        frontOfQueue->setDT(departureTime);
        frontOfQueue->setPQT(departureTime);
        minHeap->insert(frontOfQueue);
        lastQueueChange = frontOfQueue->getSOST();
        totalQueueTime += frontOfQueue->getSOST() - frontOfQueue->getAT();
        totalServiceTime += serviceTime;
        serversAvailable--;
      }
    }
  }
  Simulator::Simulator() {
    lambda = 0;
    mu = 0;
    M = 0;
    EVENTS_TO_SIMULATE = 0;
    serversAvailable = 0;
    //STATISTICS
    customersInSystem = 0;
    eventTime = 0.0;
    lastEventTime = 0.0;
    lastQueueChange = 0.0;
    totalTime = 0.0;
    totalTimeInSystem = 0.0;
    totalServiceTime = 0.0;
    totalQueueTime = 0.0;
    idleTime = 0.0;
    Po = 0.0;
    L = 0.0;
    W = 0.0;
    Lq = 0.0;
    Wq = 0.0;
    Rho = 0.0;
    minHeap = new MinHeap();
    queue = new Queue();
    currEvent = nullptr;
  }
  Simulator::~Simulator() {
    delete minHeap;
    delete queue;
    for (long unsigned int i = 0; i < processedCustomers.size(); i++) {
      delete processedCustomers.at(i);
    }
  }
  void Simulator::print() const{
    StatisticsData calcVals = getStatistics(lambda, mu, M);
    float Po_error = percentError(Po, calcVals.Po);
    float L_error = percentError(L, calcVals.L);
    float W_error = percentError(W, calcVals.W);
    float Lq_error = percentError(Lq, calcVals.Lq);
    float Wq_error = percentError(Wq, calcVals.Wq);
    float Rho_error = percentError(Rho, calcVals.Rho);
    printf("Results of the simulation:\n");
    printf("%12s %12s %15s\n", "Simulated", "Calculated", "Percent Error");
    printf("Po  %0.4f       %0.4f          %0.2f\n", Po, calcVals.Po, Po_error);
    printf("L   %0.4f       %0.4f          %0.2f\n", L, calcVals.L, L_error);
    printf("W   %0.4f       %0.4f          %0.2f\n", W, calcVals.W, W_error);
    printf("Lq  %0.4f       %0.4f          %0.2f\n", Lq, calcVals.Lq, Lq_error);
    printf("Wq  %0.4f       %0.4f          %0.2f\n", Wq, calcVals.Wq, Wq_error);
    printf("Rho %0.4f       %0.4f          %0.2f\n", Rho, calcVals.Rho, Rho_error);
  }

  bool Simulator::load(const std::string& filename){
    std::ifstream inFS;
    inFS.open(filename);
    if (!inFS.is_open()) return false;//failed to open the file
    std::string line;
    std::getline(inFS, line);
    lambda = stoi(line);
    std::getline(inFS, line);
    mu = stoi(line);
    std::getline(inFS, line);
    M = stoi(line);
    std::getline(inFS, line);
    EVENTS_TO_SIMULATE = stoi(line);
    inFS.close();
    return true;
  }
  
  void Simulator::accumulateIdleTime() {
    idleTime += (eventTime - lastEventTime);//last event mustve been a departure for all servers to be free
  }
    
  void Simulator::accumulateRunningTotals() {
    float duration = eventTime - lastEventTime;//calculate duration of the previous state
    runningTotals[customersInSystem] += duration;
    if (currEvent->isArrival()) {
        customersInSystem++; 
    } else {
        customersInSystem--;
        totalTimeInSystem += (eventTime - currEvent->getAT());
    }
  }

  void Simulator::processSimulationStatistics() {
    Po = idleTime/totalTime;
    for (int i = 0; i < 50; i++) {
      float x = i * runningTotals[i];
      L += x;
    }
    L /= totalTime;
    W = totalTimeInSystem/EVENTS_TO_SIMULATE;
    for (int i = 0; i < 50; i++) {
      float x = i * queueRunningTotals[i];
      Lq += x;
    }
    Lq /= totalTime;
    Wq = totalQueueTime/EVENTS_TO_SIMULATE;
    Rho = totalServiceTime/(totalTime*M);
  }
