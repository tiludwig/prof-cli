/*
 * Profiler.h
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#ifndef CORE_PROFILER_PROFILER_H_
#define CORE_PROFILER_PROFILER_H_

#include <stdint.h>
#include <string>

#include <Utility/MinMax.hpp>
#include <Utility/Variance.hpp>
#include <Utility/FrequencyDistribution.h>
#include <Core/Communicator/PacketCommunicator.h>
#include <Core/TestinputProvider/TestinputProvider.h>

class Profiler
{
private:
	MinMax minMaxStats;
	Variance varStats;
	FrequencyDistribution freqDistribution;
private:
	packet_t buildProfilingRequestPacket() const;
public:
	Profiler();
	~Profiler();

	void reset();

	bool setProfilingTarget(const std::string& targetname, PacketCommunicator& communicator);
	uint64_t profile(PacketCommunicator& communicator, TestinputProvider& dataprovider);
	uint64_t getWCET();

	std::vector<freqdist_entry_t>* getFrequencyDistribution();
};

#endif /* CORE_PROFILER_PROFILER_H_ */
