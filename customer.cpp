#include "customer.hpp"
  Customer() {
    arrivalTime = -1;
    startOfServiceTime = -1;
    deparatureTime = -1;
    pqTime = -1;
  }
  void setAT(float AT) {arrivalTime = AT;}
  void setSOST(float SOST); {startOfServiceTime = SOST;}
  void setDT(float DT) {departureTime = DT;}
  void setPQT(float PQT); {pqTime = PQT;}
  float getAT() const {return arrivalTime;}
  float getSOST() const {return startOfServiceTime;}
  float getDT() const {return departureTime;}
  float getPQT() const {return pqTime;}