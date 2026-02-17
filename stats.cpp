#include <cmath>
#include <random>
#include "stats.hpp"
float getRandomFloat(const float a, const float b) {
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());//uses the mersene twister engine for fast random numbers
  std::uniform_real_distribution<> distr(a, b);//defines the range from (a,b])
  float randomFloat = distr(generator);
  return randomFloat;
}

float getNextRandomInterval(const int avg) {
  float randomFloat = getRandomFloat();
  float interval = -1 * (1.0/avg) * log(randomFloat); // natural logarithm of the random float
  return interval;
}
float percentError(const float meas, const float expected) {return ((meas-expected)/expected)*100;}
StatisticsData getStatistics(const int lambda, const int mu, const int M) {
  StatisticsData data;
  data.Po = 0;
  data.L = 0;
  data.W = 0;
  data.Lq = 0;
  data.Wq = 0;
  data.Rho = 0;
  return data;
}
