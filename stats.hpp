#ifndef STATS_HPP
#define STATS_HPP
const float RANDOM_FLOAT_LB = 0.0;
const float RANDOM_FLOAT_UB = 1.0;
float getRandomFloat(const float a = RANDOM_FLOAT_LB, const float b = RANDOM_FLOAT_UB);
float getNextRandomInterval(const int avg);
#endif