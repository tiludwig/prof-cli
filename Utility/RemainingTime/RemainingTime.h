/*
 * RemainingTime.h
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#ifndef UTILITY_REMAININGTIME_REMAININGTIME_H_
#define UTILITY_REMAININGTIME_REMAININGTIME_H_

#include <stdint.h>
#include <Utility/Variance.hpp>
#include <chrono>

class RemainingTime
{
private:
	Variance timeStats;
	uint64_t duration;
	std::chrono::steady_clock::time_point startTime;
public:
	RemainingTime();
	virtual ~RemainingTime();

	void startMeasurement();
	void endMeasurement();

	uint64_t calculateRemainingSeconds(int iterationsLeft);
};

#endif /* UTILITY_REMAININGTIME_REMAININGTIME_H_ */
