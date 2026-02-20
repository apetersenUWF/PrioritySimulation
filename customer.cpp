/***************************************************************
  Ayden Petersen
  customer.cpp
  Project 2

  This file contains implementations of the Customer class methods
  The customers will act as the nodes of the linked FIFO queue
  and as the members of the minHeap vector to be used in the simulation
  Each customer holds a few values about their service event and a pointer
  to the next customer, used for the FIFO
***************************************************************/
#include "customer.hpp"
  Customer::Customer() {//initialize all values as -1 to easily determine if the event is a departure
    arrivalTime = -1;
    startOfServiceTime = -1;
    departureTime = -1;
    pqTime = -1;
    next = nullptr;
  }
  Customer::Customer(const Customer* rhs) {//copy constructor
    arrivalTime = rhs->getAT();
    startOfServiceTime = rhs->getSOST();
    departureTime = rhs->getDT();
    pqTime = rhs->getPQT();
    next = rhs->getNext();
  }
  Customer::Customer(float arrivalTime) {
    this->arrivalTime = arrivalTime;
    startOfServiceTime = -1;
    departureTime = -1;
    pqTime = arrivalTime;
    next = nullptr;
  }

  void Customer::operator=(Customer* rhs) {//= operator overloading
    this->arrivalTime = rhs->getAT();
    this->startOfServiceTime = rhs->getSOST();
    this->departureTime = rhs->getDT();
    this->pqTime = rhs->getPQT();
    this->next = rhs->getNext();
  }
  void Customer::setAT(float AT) {arrivalTime = AT;}
  void Customer::setSOST(float SOST) {startOfServiceTime = SOST;}
  void Customer::setDT(float DT) {departureTime = DT;}
  void Customer::setPQT(float PQT) {pqTime = PQT;}
  void Customer::setNext(Customer* next) {this->next = next;}
  float Customer::getAT() const {return arrivalTime;}
  float Customer::getSOST() const {return startOfServiceTime;}
  float Customer::getDT() const {return departureTime;}
  float Customer::getPQT() const {return pqTime;}
  Customer* Customer::getNext() const {return next;}

  bool Customer::isArrival() const {//customer is arrival by default
    if (departureTime == -1) return true;// if departure time hasnt been set, event is an arrival
    else return false;
  }

  