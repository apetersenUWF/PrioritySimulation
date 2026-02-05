#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP
class Customer{
  float arrivalTime;
  float startOfServiceTime;
  float departureTime;
  float pqTime;
  public:
  Customer();
  void setAT(float AT);
  void setSOST(float SOST);
  void setDT(float DT);
  void setPQT(float PQT);
  float getAT() const;
  float getSOST() const;
  float getDT() const;
  float getPQT() const;
};
#endif