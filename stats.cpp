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
long long fact(const int input) {
  if (input == 0) return 1;
  long long fact = input;
  for (int i = input-1; i > 0; i--) fact *= i;
  return fact;
}
StatisticsData getStatistics(const float lambda, const float mu, const float M) {
  StatisticsData data;
  float c1 = 0.0;
  float c2 = 0.0;
  for (int i = 0; i < M; i++) {
    c1 += (1.0/fact(i))*pow((lambda/mu), i);
  }
  c2 += (1.0/fact(static_cast<int>(M)))*pow((lambda/mu), M)*((M*mu)/((M*mu)-lambda));
  data.Po = 1/(c1+c2);
  c1 = (lambda*mu*pow((lambda/mu), M))/(fact(M-1.0)*(M*mu-lambda)*(M*mu-lambda));
  c2 = lambda/mu;
  data.L = c1*data.Po + c2;
  data.W = data.L/lambda;
  data.Lq = data.L - (lambda/mu);
  data.Wq = data.Lq/lambda;
  data.Rho = lambda/(M*mu);
  return data;
}
