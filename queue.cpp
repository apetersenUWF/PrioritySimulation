#include "queue.hpp"

  Queue::Queue(): front(nullptr), back(nullptr) {}
  Queue::Queue(Customer* customer): front(customer), back(customer) {}

  void Queue::pop() {
    Customer* curr = front;
    if (curr != nullptr) {//if front isnt nullptr, assign its next node as the new front
      delete front;
      front = curr->next;
    }
    if (front == nullptr) back = front;//if front is now nullptr or if it was an empty list, it was the only node, also reassign back
  }
  Customer* Queue::peek() const {return front;}

  void Queue::insert(const Customer& customerToInsert) {
    Customer* curr = new Customer(customerToInsert);
    if (isEmpty()) {
      front = back = curr;
      return;
    }
      back->setNext(curr);
      back = curr;
  }
  bool Queue::isEmpty() const {
    if (front == nullptr) return true;
    else return false;
  }

