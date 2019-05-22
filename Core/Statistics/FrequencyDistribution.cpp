/*
 * FrequencyDistribution.cpp
 *
 *  Created on: Mar 31, 2019
 *      Author: tim
 */

#include <Core/Statistics/FrequencyDistribution.h>
#include <algorithm>

FrequencyDistribution::FrequencyDistribution()
{
	sampleCount = 0;
}

void FrequencyDistribution::reset()
{
	entries.clear();
}

void FrequencyDistribution::update(uint64_t value)
{
	auto slot = findFrequencySlot(value);
	if (slot == entries.end())
	{
		// new entry
		entries.push_back( { value, 1 });
	}
	else
	{
		// modify entry
		slot->counts++;
	}
	sampleCount++;
}

std::vector<freqdist_entry_t>* FrequencyDistribution::getDistribution()
{
	return &entries;
}

std::vector<freqdist_entry_t>::iterator FrequencyDistribution::findFrequencySlot(uint64_t value)
{
	auto iterator = std::find_if(entries.begin(), entries.end(), [&](freqdist_entry_t const & slot)
	{
		return slot.value == value;
	});

	return iterator;
}
