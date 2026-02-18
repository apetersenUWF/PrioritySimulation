#ifndef STATS_HPP
#define STATS_HPP
const float RANDOM_FLOAT_LB = 0.0;
const float RANDOM_FLOAT_UB = 1.0;
struct StatisticsData {
  float Po;
  float L;
  float W;
  float Lq;
  float Wq;
  float Rho;
};
float getRandomFloat(const float a = RANDOM_FLOAT_LB, const float b = RANDOM_FLOAT_UB);
float getNextRandomInterval(const int avg);
float percentError(const float meas, const float expected);
long long fact(const int input);
StatisticsData getStatistics (const float lambda, const float mu, const float M);
#endif