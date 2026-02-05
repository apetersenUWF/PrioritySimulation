#include <cmath>
#include <random>
float getRandomFloat(const float a = 0.0, const float b = 1.0) {
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