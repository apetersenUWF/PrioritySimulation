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
  Customer* Queue::pop() {
    if (front == nullptr) return nullptr;
    size--;
    Customer* curr = new Customer(front);//saves the current customer for returning
    delete front;
    front = curr->getNext();
    if (front == nullptr) back = front;//if front is now nullptr, it was the only node, also reassign back
    return curr;
  }
  Customer* Queue::peek() const {return front;}

  void Queue::insert(Customer* customerToInsert) {
    size++;
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
  int Queue::getSize() const {return size;}
