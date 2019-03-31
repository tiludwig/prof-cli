/*
 * Variance.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: timl
 */

#include "Variance.hpp"

Variance::Variance()
{
	_count = 0;
	_M2 = 0.0;
	_mean = 0.0;
}

void Variance::update(__u64 newValue) {
	_count++;
	double delta = newValue - _mean;
	_mean += delta / _count;
	double delta2 = newValue - _mean;
	_M2 += delta * delta2;
}

double Variance::getMean()
{
	return _mean;
}

double Variance::getVariance()
{
	return _M2 / (_count - 1);
}

double Variance::getSampleVariance()
{
	return _M2 / _count;
}

__u32 Variance::getSampleCount()
{
	return _count;
}
