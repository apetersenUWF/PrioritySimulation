#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP
class Customer{
  float arrivalTime;
  float startOfServiceTime;
  float departureTime;
  float pqTime;
  Customer* next;
  public:
  Customer();
  Customer(const Customer* rhs);
  void operator=(Customer* rhs);

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
};
#endif