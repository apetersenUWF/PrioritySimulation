#include "sim.hpp"
#include "stats.hpp"
#include "fstream"
#include <iostream>
  Simulator::Simulator() {
    lambda = 0;
    mu = 0;
    M = 0;
    EVENTS_TO_SIMULATE = 0;
    serversAvailable = 0;
    eventTime = 0.0;
    lastEventTime = 0.0;
    customersInSystem = 0.0;
    totalTime = 0.0;
    idleTime = 0.0;
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
    std::cout << "lambda = " << lambda << std::endl;
    std::cout << "mu = " << mu << std::endl;
    std::cout << "M = " << M << std::endl;
    std::cout << "EVENTS_TO_SIMULATE = " << EVENTS_TO_SIMULATE << std::endl;
    std::cout << "serversAvailable = " << serversAvailable << std::endl;
    std::cout << "Po " << (idleTime/totalTime)*100 << "%" << std::endl;
    float avgInSystem = 0.0;
    for (int i = 0; i < 20; i++) {
      float x = i * runningTotals[i];
      avgInSystem += x;
    }
    float avgInQueue = 0.0;
    for (int i = 0; i < 20; i++) {
      float x = i * queueRunningTotals[i];
      avgInQueue += x;
    }
    avgInSystem /= totalTime;
    avgInQueue /= totalTime;
    std::cout << "L = " << avgInSystem << std::endl;
    std::cout << "W = " << totalTimeInSystem/EVENTS_TO_SIMULATE << std::endl;
    std::cout << "Lq = " << avgInQueue << std::endl;
    std::cout << "Wq = " << totalQueueTime/EVENTS_TO_SIMULATE << std::endl;
    std::cout << "Rho = " << totalServiceTime/(totalTime*M) << std::endl;
    if (!queue->isEmpty()) std::cout << "-1" << std::endl;
    if (!minHeap->isEmpty()) std::cout << "-2" << std::endl;
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
  }

  void Simulator::accumulateIdleTime() {
    idleTime += (eventTime - lastEventTime);//last event mustve been a departure for all servers to be free
  }
  //void processStatistics() {}
    
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
        totalQueueTime += frontOfQueue->getSOST() - frontOfQueue->getAT();
        float serviceTime = getNextRandomInterval(mu);
        float departureTime = frontOfQueue->getSOST() + serviceTime;
        frontOfQueue->setDT(departureTime);
        frontOfQueue->setPQT(departureTime);
        minHeap->insert(frontOfQueue);
        lastQueueChange = frontOfQueue->getSOST();
        serversAvailable--;
        totalServiceTime += serviceTime;
      }
    }
  }