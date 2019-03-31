/*
 * MinMax.hpp
 *
 *  Created on: Mar 11, 2019
 *      Author: timl
 */

#ifndef UTILS_STATISTICS_MINMAX_HPP_
#define UTILS_STATISTICS_MINMAX_HPP_

#include <linux/types.h>

class MinMax {
private:
	__u64 _min;
	__u64 _max;
	bool _validMinMax;

public:
	MinMax();

	void update(__u64 newValue);

	__u64 getMin();
	__u64 getMax();
};

#endif /* UTILS_STATISTICS_MINMAX_HPP_ */
