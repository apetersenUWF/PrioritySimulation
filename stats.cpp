#include <cmath>
#include <random>
const float RANDOM_FLOAT_LB = 0.0;
const float RANDOM_FLOAT_UB = 1.0;
float getRandomFloat(const float a = RANDOM_FLOAT_LB, const float b = RANDOM_FLOAT_UB) {
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