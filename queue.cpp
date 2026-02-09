#include "queue.hpp"

  Queue::Queue(): front(nullptr), back(nullptr) {}
  Queue::Queue(Customer* customer): front(customer), back(customer) {}
  Queue::~Queue() {
    while (front) {
      Customer* curr = front->getNext();
      delete front;
      front = curr;
    }
  }
  Customer* Queue::pop() {
    Customer* curr = front;
    Customer* exitingCustomer = new Customer(curr);//saves the current customer for returning
    if (curr != nullptr) {//if front isnt nullptr, assign its next node as the new front
      delete front;
      front = curr->getNext();
    }
    if (front == nullptr) back = front;//if front is now nullptr or if it was an empty list, it was the only node, also reassign back
    return exitingCustomer;
  }
  Customer* Queue::peek() const {return front;}

  void Queue::insert(Customer* customerToInsert) {
    if (isEmpty()) {
      front = back = customerToInsert;
      return;
    }
      back->setNext(customerToInsert);
      back = customerToInsert;
  }
  bool Queue::isEmpty() const {
    if (front == nullptr) return true;
    else return false;
  }

