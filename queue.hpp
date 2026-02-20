#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "customer.hpp"
class Queue{
  Customer* front;//pointer to the front customer
  Customer* back;//pointer to the back customer
  int size;//holds the size
  public:
  Queue();
  Queue(Customer* customer);
  ~Queue();
  Customer* pop();//remove and return the front customer
  Customer* peek() const;//return the front customer
  void insert(Customer* customerToInsert);//inserts a customer to the back of the queue
  bool isEmpty() const;
  int getSize() const;
};
#endif