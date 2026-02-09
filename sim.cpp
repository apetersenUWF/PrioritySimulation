#include "sim.hpp"
#include "fstream"
#include <iostream>
  Simulator::Simulator() {
    lambda = 0;
    mu = 0;
    M = 0;
    EVENTS_TO_SIMULATE = 0;
    minHeap = new MinHeap();
    queue = new Queue();
  }
  Simulator::~Simulator() {
    delete minHeap;
    delete queue;
  }
  void Simulator::print() const{
    std::cout << "lambda = " << lambda << std::endl;
    std::cout << "mu = " << mu << std::endl;
    std::cout << "M = " << M << std::endl;
    std::cout << "EVENTS_TO_SIMULATE = " << EVENTS_TO_SIMULATE << std::endl;
  }
  bool Simulator::load(const std::string& filename){
    std::ifstream inFS;
    inFS.open(filename);
    if (!inFS.is_open()) return false;//failed to open the file
    std::string line;
    std::getline(inFS, line);
    lambda = stoi(line);
    std::getline(inFS, line);
    mu = stoi(line);
    std::getline(inFS, line);
    M = stoi(line);
    std::getline(inFS, line);
    EVENTS_TO_SIMULATE = stoi(line);
    inFS.close();
    return true;
  }
  void Simulator::run() {}