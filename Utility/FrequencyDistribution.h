/*
 * FrequencyDistribution.h
 *
 *  Created on: Mar 31, 2019
 *      Author: tim
 */

#ifndef UTILITY_FREQUENCYDISTRIBUTION_H_
#define UTILITY_FREQUENCYDISTRIBUTION_H_

#include <stdint.h>
#include <vector>

struct freqdist_entry_t
{
	uint64_t value;
	uint64_t counts;
};

class FrequencyDistribution
{
private:
	std::vector<freqdist_entry_t> entries;
	uint64_t sampleCount;

private:
	std::vector<freqdist_entry_t>::iterator findFrequencySlot(uint64_t value);
public:
	FrequencyDistribution();

	void update(uint64_t value);

	std::vector<freqdist_entry_t>* getDistribution();
};

#endif /* UTILITY_FREQUENCYDISTRIBUTION_H_ */
