/***************************************************************
  Ayden Petersen
  queue.cpp
  Project 2

  This file contains implementations of the Queue class methods
  These methods implement the FIFO Queue ADT
***************************************************************/
#include "queue.hpp"

  Queue::Queue(): front(nullptr), back(nullptr), size(0) {}
  Queue::Queue(Customer* customer): front(customer), back(customer), size(0) {}
  Queue::~Queue() {
    while (front) {
      Customer* curr = front->getNext();
      delete front;
      front = curr;
    }
  }
  Customer* Queue::pop() {//removes the item at the front of the queue and returns it
    if (front == nullptr) return nullptr;//queue is empty already
    size--;
    Customer* curr = new Customer(front);//saves the current customer for returning
    delete front;
    front = curr->getNext();
    if (front == nullptr) back = front;//if front is now nullptr, it was the only node, also reassign back
    return curr;
  }
  Customer* Queue::peek() const {return front;}//returns the item at the front of queue without modifying anything

  void Queue::insert(Customer* customerToInsert) {//inserts a customer into the back of the queue
    size++;
    if (isEmpty()) {//queue is empty, assign new customer as the front and back
      front = back = customerToInsert;
      return;
    }
      back->setNext(customerToInsert);//queue wasnt empty, only assign the back
      back = customerToInsert;
  }
  bool Queue::isEmpty() const {
    if (front == nullptr) return true;
    else return false;
  }
  int Queue::getSize() const {return size;}
