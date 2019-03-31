/*
 * Variance.hpp
 *
 *  Created on: Mar 11, 2019
 *      Author: timl
 */

#ifndef UTILS_STATISTICS_VARIANCE_HPP_
#define UTILS_STATISTICS_VARIANCE_HPP_

#include <linux/types.h>

class Variance {
private:
	__u32 _count;
	double _M2;
	double _mean;
public:
	Variance();

	void update(__u64 newValue);

	double getMean();
	double getVariance();
	double getSampleVariance();
	__u32 getSampleCount();
};

#endif /* UTILS_STATISTICS_VARIANCE_HPP_ */
