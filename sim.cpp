#include "sim.hpp"
#include "stats.hpp"
#include "fstream"
#include <iostream>
  Simulator::Simulator() {
    lambda = 0;
    mu = 0;
    M = 0;
    EVENTS_TO_SIMULATE = 0;
    currTime = 0.0;
    lastDepartureTime = 0.0;
    serversAvailable = 0;
    minHeap = new MinHeap();
    queue = new Queue();
  }
  Simulator::~Simulator() {
    delete minHeap;
    delete queue;
    for (int i = 0; i < processedCustomers.size(); i++) {
      delete processedCustomers.at(i);
    }
  }
  void Simulator::print() const{//for testing purposes
    std::cout << "lambda = " << lambda << std::endl;
    std::cout << "mu = " << mu << std::endl;
    std::cout << "M = " << M << std::endl;
    std::cout << "EVENTS_TO_SIMULATE = " << EVENTS_TO_SIMULATE << std::endl;
    std::cout << "currTime = " << currTime << std::endl;
    std::cout << "lastDepartureTime = " << lastDepartureTime << std::endl;
    std::cout << "serversAvailable = " << serversAvailable << std::endl;
    std::cout << "processedCustomers = " << std::endl;
    for (int i = 0; i < processedCustomers.size(); i++) {
      processedCustomers.at(i)->print();
    }
  }
  Customer* Simulator::getNextCustomer(const float currTime) const {
    float interval = getNextRandomInterval(lambda);
    Customer* arrival = new Customer();
    arrival->setAT(currTime+interval);
    arrival->setPQT(currTime+interval);
    return arrival;
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
  void Simulator::processNextEvent() {
    Customer* lastServed = minHeap->serve();
    if (lastServed->isArrival()) {
      float interval = getNextRandomInterval(mu);
      float departureTime = (lastServed->getSOST())+interval;
      lastServed->setDT(departureTime);
      lastServed->setPQT(departureTime);
      minHeap->insert(lastServed);
    }
    else {//event is a departure
      serversAvailable++;
      lastDepartureTime = lastServed->getDT();
      processedCustomers.push_back(lastServed);
    }
  }
  void Simulator::run() {
  serversAvailable = M;
    float interval = getNextRandomInterval(lambda);
    currTime += interval;
    lastDepartureTime = currTime;
    while (serversAvailable > 0) {//insert the first customers into the pq
      Customer* curr = new Customer(currTime);
      curr->setSOST(currTime);
      minHeap->insert(curr);
      interval = getNextRandomInterval(lambda);
      currTime += interval;
      serversAvailable--;
      EVENTS_TO_SIMULATE--;
    }
    while (EVENTS_TO_SIMULATE > 0) {
      Customer* curr = new Customer(currTime);
      queue->insert(curr);
      interval = getNextRandomInterval(lambda);
      currTime += interval;
      EVENTS_TO_SIMULATE--;
    }
    while (!queue->isEmpty() && !minHeap->isEmpty()) {
      processNextEvent();
      if (serversAvailable > 0) {
        Customer* firstInLine = queue->pop();
        if (firstInLine->getAT() > lastDepartureTime) firstInLine->setSOST(firstInLine->getAT());
        else firstInLine->setSOST(lastDepartureTime);
        minHeap->insert(firstInLine);
        serversAvailable--;
      }
    }
  }