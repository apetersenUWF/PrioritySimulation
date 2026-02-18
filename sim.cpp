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
        customersWithWaitTime++;
      }
    }
    else {//event is a departure
      accumulateRunningTotals();
      totalQueueTime += (currEvent->getSOST() - currEvent->getAT());//keep running total of queue time waited
      serversAvailable++;//free up a server
      if (!queue->isEmpty()) {//if someone is waiting in line as an arrival
        queueRunningTotals[queue->getSize()] += currEvent->getDT() - lastQueueChange;
        Customer* frontOfQueue = queue->pop();//pull them from the queue, insert their departure event
        frontOfQueue->setSOST(currEvent->getDT());//set start of service as the last customers departure
        float serviceTime = getNextRandomInterval(mu);
        float departureTime = frontOfQueue->getSOST() + serviceTime;//calculate departure time
        frontOfQueue->setDT(departureTime);
        frontOfQueue->setPQT(departureTime);//set this as a departure event
        minHeap->insert(frontOfQueue);//place into the PQ
        lastQueueChange = frontOfQueue->getSOST();//save the last time that the queue size changed
        totalServiceTime += serviceTime;//keep running total of service time
        serversAvailable--;//remove a server since service started
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
    customersWithWaitTime = 0;
    eventTime = 0.0;
    lastEventTime = 0.0;
    lastQueueChange = 0.0;
    totalTime = 0.0;
    totalTimeInSystem = 0.0;
    totalServiceTime = 0.0;
    totalQueueTime = 0.0;
    idleTime = 0.0;
    chanceOfWaiting = 0.0;
    Po = 0.0;
    W = 0.0;
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
    StatisticsData calcVals = getStatistics(static_cast<float>(lambda), static_cast<float>(mu), static_cast<float>(M));
    float Po_error = percentError(Po, calcVals.Po);
    float W_error = percentError(W, calcVals.W);
    float Wq_error = percentError(Wq, calcVals.Wq);
    float Rho_error = percentError(Rho, calcVals.Rho);
    printf("Results of the simulation:\n");
    printf("%12s %12s %15s\n", "Simulated", "Calculated", "Percent Error");
    printf("Po  %-5.4f       %-5.4f        %-5.2f%%\n", Po, calcVals.Po, Po_error);
    printf("L     N/A        %-5.4f         N/A\n", calcVals.L);
    printf("W   %-5.4f       %-5.4f        %-5.2f%%\n", W, calcVals.W, W_error);
    printf("Lq    N/A        %-5.4f         N/A\n", calcVals.Lq);
    printf("Wq  %-5.4f       %-5.4f        %-5.2f%%\n", Wq, calcVals.Wq, Wq_error);
    printf("Rho %-5.4f       %-5.4f        %-5.2f%%\n", Rho, calcVals.Rho, Rho_error);
    printf("Overall chance of having to wait -> %-5.2f%%\n", chanceOfWaiting);
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
    /*for (int i = 0; i < 50; i++) {
      float x = i * runningTotals[i];
      L += x;
    }
    L /= totalTime;*/
    W = totalTimeInSystem/EVENTS_TO_SIMULATE;
    /*for (int i = 0; i < 50; i++) {
      float x = i * queueRunningTotals[i];
      Lq += x;
    }
    Lq /= totalTime;*/
    Wq = totalQueueTime/EVENTS_TO_SIMULATE;
    Rho = totalServiceTime/(totalTime*M);
    chanceOfWaiting = (static_cast<float>(customersWithWaitTime)/EVENTS_TO_SIMULATE)*100;
  }
