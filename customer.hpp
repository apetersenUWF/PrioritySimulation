#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP
class Customer{
  float arrivalTime;
  float startOfServiceTime;
  float departureTime;
  float pqTime;//stores the value to be referenced as the priority in the minHeap, can be the arrival time or departure time
  Customer* next;//stores the next customer used for the FIFO, ignored for the minHeap
  public:
  Customer();
  Customer(const Customer* rhs);//makes a new customer from a provided one
  Customer(float arrivalTime);//makes a customer from an arrival time
  void operator=(Customer* rhs);//copies one customer to another

  void setAT(float AT);
  void setSOST(float SOST);
  void setDT(float DT);
  void setPQT(float PQT);
  void setNext(Customer* next);
  float getAT() const;
  float getSOST() const;
  float getDT() const;
  float getPQT() const;
  Customer* getNext() const;

  bool isArrival() const;//returns true if customer is an arrival, false otherwise
};
#endif