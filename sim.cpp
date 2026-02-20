/***************************************************************
  Ayden Petersen
  sim.cpp
  Project 2

  This file contains implementations of the Simulator class methods
  These methods make use of the Priority Queue and FIFO to drive the simulation and gather statistics
***************************************************************/

#include "sim.hpp"
#include "stats.hpp"
#include "fstream"
#include <iostream>

  void Simulator::run() {//runs the simulation
    float customerArrivalTime = 0.0;
    float interval = getNextRandomInterval(lambda);
    int i = 0;//event counter
    serversAvailable = M;//set all servers to be free
    customerArrivalTime += interval;//add the random interval to find the arrival time
    while (minHeap->getSize() < DEFAULT_MINHEAP_CAPACITY && i < EVENTS_TO_SIMULATE) {//if pq becomes full or all events have been generated, stop looping
      Customer* curr = new Customer(customerArrivalTime);//create new customer, accumulate time and insert into the pq for processing
      minHeap->insert(curr);
      interval = getNextRandomInterval(lambda);
      customerArrivalTime += interval;//accumulate time
      i++;
    }
    while (!minHeap->isEmpty()) {//keep looping until the pq is empty
      processNextEvent();//event processing logic
        if (i < EVENTS_TO_SIMULATE && minHeap->getSize() <= M+1) {//if there are more events to simulate and the pq is <= M+1 keep looping
          Customer* curr = new Customer(customerArrivalTime);//create new customer, accumulate time and insert into the pq for processing
          minHeap->insert(curr);
          interval = getNextRandomInterval(lambda);
          customerArrivalTime += interval;//accumulate time
          i++;
        }
    }
    totalTime = currEvent->getDT();//total sim time is equal to the time of the last departure
    processSimulationStatistics();//process the data gathered during the simulation
    print();//print results
  }
  void Simulator::processNextEvent() {
    currEvent = minHeap->serve();//update the current event being processed
    lastEventTime = eventTime;//save the time of the previous event
    eventTime = currEvent->getPQT();//current event time updates
    //event can be either an arrival or departure of a customer
    if (currEvent->isArrival()) {//event is an arrival
      accumulateRunningTotals();//keeps track of how long this many customers have been in the system
      if (serversAvailable > 0) {//if theres a server available
        if (serversAvailable == M) accumulateIdleTime();//if someone just arrived while all servers were available, we have lastDT and lastAT to calculate idleTime
        serversAvailable--;//take away a server for service
        currEvent->setSOST(currEvent->getAT());//arrived when server was available, sost is same as AT
        float serviceTime = getNextRandomInterval(mu);//calculate service time and departure time
        float departureTime = currEvent->getSOST() + serviceTime;
        currEvent->setDT(departureTime);
        currEvent->setPQT(departureTime);
        minHeap->insert(currEvent);//insert future departure event
        totalServiceTime += serviceTime;//accumulate time conducting service
      }
      else {//someone arrived when all servers were busy
        queueRunningTotals[queue->getSize()] += currEvent->getAT() - lastQueueChange;//keeps track of how long the queue has been this current size
        queue->insert(currEvent);//if most recent event was an arrival but no servers available, put in line
        lastQueueChange = currEvent->getAT();//save the time that the queue changed sizes
        customersWithWaitTime++;//count how many customers had to wait for service
      }
    }
    else {//event is a departure
      accumulateRunningTotals();//keeps track of how long this many customers have been in the system
      totalQueueTime += (currEvent->getSOST() - currEvent->getAT());//keep running total of queue time waited
      serversAvailable++;//free up a server
      if (!queue->isEmpty()) {//if someone is waiting in line as an arrival
        queueRunningTotals[queue->getSize()] += currEvent->getDT() - lastQueueChange;//keeps track of how long the queue has been this current size
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
  }
  void Simulator::print() const{
    StatisticsData calcVals = getStatistics(static_cast<float>(lambda), static_cast<float>(mu), static_cast<float>(M));//find the values predicted by the analytical model
    float Po_error = percentError(Po, calcVals.Po);//calculate percent errors from analytical model values
    float W_error = percentError(W, calcVals.W);
    float Wq_error = percentError(Wq, calcVals.Wq);
    float Rho_error = percentError(Rho, calcVals.Rho);
    printf("Results of the simulation:\n");//prints table of data
    printf("%12s %12s %15s\n", "Simulated", "Calculated", "Percent Error");
    printf("Po  %-5.4f       %-5.4f        %-5.2f%%\n", Po, calcVals.Po, Po_error);
    printf("L     N/A        %-5.4f         N/A\n", calcVals.L);
    printf("W   %-5.4f       %-5.4f        %-5.2f%%\n", W, calcVals.W, W_error);
    printf("Lq    N/A        %-5.4f         N/A\n", calcVals.Lq);
    printf("Wq  %-5.4f       %-5.4f        %-5.2f%%\n", Wq, calcVals.Wq, Wq_error);
    printf("Rho %-5.4f       %-5.4f        %-5.2f%%\n", Rho, calcVals.Rho, Rho_error);
    printf("Overall chance of having to wait -> %-5.2f%%\n", chanceOfWaiting);
  }

  bool Simulator::load(const std::string& filename){//loads starting conditions from a file
    std::ifstream inFS;//data format lambda \n mu \n M \n #ofEvents
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
    runningTotals[customersInSystem] += duration;//this number of customers in the system existed for duration time
    if (currEvent->isArrival()) {//update the number of customers in the system
        customersInSystem++; 
    } else {
        customersInSystem--;
        totalTimeInSystem += (eventTime - currEvent->getAT());//customers total time in the system is their DT - AT
    }
  }

  void Simulator::processSimulationStatistics() {//calculates simulated statistics
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
    chanceOfWaiting = (static_cast<float>(customersWithWaitTime)/EVENTS_TO_SIMULATE)*100;
  }
