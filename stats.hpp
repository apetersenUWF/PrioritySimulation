#ifndef STATS_HPP
#define STATS_HPP
const float RANDOM_FLOAT_LB = 0.0;//defines the default range of floats we want to generate
const float RANDOM_FLOAT_UB = 1.0;
struct StatisticsData {//structure used to return analytical model values to the simulator
  float Po;
  float L;
  float W;
  float Lq;
  float Wq;
  float Rho;
};
float getRandomFloat(const float a = RANDOM_FLOAT_LB, const float b = RANDOM_FLOAT_UB);//generates a random float on (a,b]
float getNextRandomInterval(const int avg);//calculates a random interval given the average number of events per time unit
float percentError(const float meas, const float expected);//calculates the percent error
long long fact(const int input);//calculates the factorial of a positive integer
StatisticsData getStatistics (const float lambda, const float mu, const float M);//calculates values predicted by the analytical model
#endif