/*
 * Statistics.cpp
 *
 *  Created on: May 22, 2019
 *      Author: tim
 */

#include <Core/Statistics/Statistics.h>

void Statistics::reset()
{
	_minMax.reset();
	_freqDistribution.reset();
	_variance.reset();
}

void Statistics::update(uint64_t executionTime)
{
	_minMax.update(executionTime);
	_freqDistribution.update(executionTime);
	_variance.update(executionTime);
}

std::vector<freqdist_entry_t>* Statistics::getFrequencyDistribution()
{
	return _freqDistribution.getDistribution();
}

uint64_t Statistics::getMinimumExecutionTime()
{
	return _minMax.getMin();
}

uint64_t Statistics::getMaximumExecutionTime()
{
	return _minMax.getMax();
}

double Statistics::getVariance()
{
	return _variance.getVariance();
}

double Statistics::getMeanExecutionTime()
{
	return _variance.getMean();
}

