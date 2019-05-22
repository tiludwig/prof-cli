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

#include <Core/Communicator/PacketCommunicator.h>
#include <Core/TestinputProvider/TestinputProvider.h>

#define INVALID_WCET 0xFFFFFFFFFFFFFFFF

class Profiler
{
private:
	packet_t buildProfilingRequestPacket() const;
public:
	Profiler();
	~Profiler();

	bool setProfilingTarget(const std::string& targetname, PacketCommunicator& communicator);
	uint64_t profile(PacketCommunicator& communicator, TestinputProvider& dataprovider);
};

#endif /* CORE_PROFILER_PROFILER_H_ */
