#include "customer.hpp"
#include <iostream>
  Customer::Customer() {
    arrivalTime = -1;
    startOfServiceTime = -1;
    departureTime = -1;
    pqTime = -1;
    next = nullptr;
  }
  Customer::Customer(const Customer* rhs) {
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

  void Customer::operator=(Customer* rhs) {
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

  bool Customer::isArrival() const {
    if (departureTime == -1) return true;
    else return false;
  }

  void Customer::print() const{
    std::cout << "arrivalTime = " << arrivalTime << std::endl;
    std::cout << "startOfServiceTime = " << startOfServiceTime << std::endl;
    std::cout << "departureTime = " << departureTime << std::endl;
    std::cout << "pqTime = " << pqTime << std::endl;
  }
  