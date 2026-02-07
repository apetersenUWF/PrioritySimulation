#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "customer.hpp"
class Queue{
  Customer* front;
  Customer* back;
  public:
  Queue();
  Queue(Customer* customer);
  void pop();
  Customer* peek() const;
  void insert(Customer* customerToInsert);
  bool isEmpty() const;
};
#endif