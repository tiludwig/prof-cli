/*
 * Statistics.h
 *
 *  Created on: May 22, 2019
 *      Author: tim
 */

#ifndef CORE_STATISTICS_STATISTICS_H_
#define CORE_STATISTICS_STATISTICS_H_

#include "MinMax.hpp"
#include "Variance.hpp"
#include "FrequencyDistribution.h"

class Statistics
{
private:
	Variance _variance;
	MinMax _minMax;
	FrequencyDistribution _freqDistribution;

public:
	void reset();
	void update(uint64_t executionTime);

	std::vector<freqdist_entry_t>* getFrequencyDistribution();
	uint64_t getMinimumExecutionTime();
	uint64_t getMaximumExecutionTime();
	double getVariance();
	double getMeanExecutionTime();
};

#endif /* CORE_STATISTICS_STATISTICS_H_ */
