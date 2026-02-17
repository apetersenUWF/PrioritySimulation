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
    processStatistics();
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
  void Simulator::print() const{//for testing purposes
    std::cout << "Results of the simulation:" << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Po = " << Po << std::endl;
    std::cout << "L = " << L << std::endl;
    std::cout << "W = " << W << std::endl;
    std::cout << "Lq = " << Lq << std::endl;
    std::cout << "Wq = " << Wq << std::endl;
    std::cout << "Rho = " << Rho << std::endl;
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

  void Simulator::processStatistics() {
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
