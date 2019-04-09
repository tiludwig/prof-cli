/*
 * RemainingTime.cpp
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#include <Utility/RemainingTime/RemainingTime.h>

using namespace std::chrono;

RemainingTime::RemainingTime()
{
	// TODO Auto-generated constructor stub

}

RemainingTime::~RemainingTime()
{
	// TODO Auto-generated destructor stub
}

void RemainingTime::startMeasurement()
{
	startTime = steady_clock::now();
}
void RemainingTime::endMeasurement()
{
	auto end = steady_clock::now();
	auto duration = duration_cast<microseconds>(end - startTime).count();
	timeStats.update(duration);
}

uint64_t RemainingTime::calculateRemainingSeconds(int iterationsLeft)
{
	double meanduration = timeStats.getMean();
	uint64_t estimatedSeconds = (meanduration * iterationsLeft) / 1000000;
	return estimatedSeconds;
}
