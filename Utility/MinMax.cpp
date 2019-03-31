/*
 * MinMax.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: timl
 */

#include "MinMax.hpp"

MinMax::MinMax()
{
	_min = 0;
	_max = 0;
	_validMinMax = false;
}

void MinMax::update(__u64 newValue)
{
	if(_validMinMax == false)
	{
		_min = newValue;
		_max = newValue;
		_validMinMax = true;
		return;
	}

	if(newValue < _min)
	{
		_min = newValue;
	}

	if(newValue > _max)
	{
		_max = newValue;
	}
}

__u64 MinMax::getMin()
{
	return _min;
}

__u64 MinMax::getMax()
{
	return _max;
}
