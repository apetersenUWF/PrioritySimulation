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
    if (!queue->isEmpty()) std::cout << "-1" << std::endl;
    if (!minHeap->isEmpty()) std::cout << "-2" << std::endl;
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
    Customer* lastServed = minHeap->serve();//save the result of the most recent service
    //can be either an arrival or departure of a customer
    if (lastServed->isArrival()) {//if the last service was an arrival event
      float interval = getNextRandomInterval(mu);//generate the time length of the service
      float departureTime = (lastServed->getSOST())+interval;//generate the time of the departure based on the interval
      lastServed->setDT(departureTime);
      lastServed->setPQT(departureTime);//set the pqtime to prioritize based upon the departure time, because this customer arrival has already been processed
      minHeap->insert(lastServed);//reinsert this customer as a future departure event, with the generated departure time
    }
    else {//event is a departure
      serversAvailable++;//free up a server
      lastDepartureTime = lastServed->getDT();//save the time at which the server became free
      processedCustomers.push_back(lastServed);//save this customer data for analysis
    }
  }
  void Simulator::run() {
  serversAvailable = M;//set all servers to be free
    float interval = getNextRandomInterval(lambda);//find time for first customer
    currTime += interval;
    lastDepartureTime = currTime;//initialize the last departure time for later use
    while (serversAvailable > 0) {//insert the first customer arrivals into the pq
      Customer* curr = new Customer(currTime);//generate a new customer with arrival and pqtime equal to what was generated
      curr->setSOST(currTime);//since servers are free these customers start immediately
      minHeap->insert(curr);//insert into the pq as an arrival event to be processed
      interval = getNextRandomInterval(lambda);
      currTime += interval;//accumulate time
      serversAvailable--;
      EVENTS_TO_SIMULATE--;
    }
    while (EVENTS_TO_SIMULATE > 0) {
      Customer* curr = new Customer(currTime);//generate all the remaining customer arrivals from the arrival distribution times
      queue->insert(curr);//insert these into the fifo queue, guaranteed to be in order from how time is accumulated
      interval = getNextRandomInterval(lambda);
      currTime += interval;
      EVENTS_TO_SIMULATE--;
    }
    while (!queue->isEmpty() || !minHeap->isEmpty()) {//while some customers still exist in either the pq or fifo, keep looping
      processNextEvent();
      if (serversAvailable > 0 && !queue->isEmpty()) {//if a server was just freed
        Customer* firstInLine = queue->pop();//pop from fifo and save the result
        if (firstInLine->getAT() > lastDepartureTime) firstInLine->setSOST(firstInLine->getAT());//if this customer arrived after the last departure, start service at arrival time
        else firstInLine->setSOST(lastDepartureTime);//else, the customer arrived before the server was free, so set start of service to be the time of the last persons departure
        minHeap->insert(firstInLine);//insert this customer arrival event into the pq
        serversAvailable--;
      }
    }
  }