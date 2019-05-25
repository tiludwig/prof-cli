/*
 * Profiler.cpp
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#include <Core/Profiler/Profiler.h>

Profiler::Profiler()
{
	// TODO Auto-generated constructor stub

}

Profiler::~Profiler()
{
	// TODO Auto-generated destructor stub
}

bool Profiler::setProfilingTarget(const std::string& targetname, PacketCommunicator& communicator)
{
	packet_t profilingTarget; // = { 30, static_cast<uint16_t>(targetname.length() + 1), const_cast<char*>(targetname.c_str()) };
	profilingTarget.id = 30;
	profilingTarget.size.value = static_cast<uint16_t>(targetname.length() + 1);
	profilingTarget.payload.add(targetname);

	communicator.request(profilingTarget);
	return true;
}

packet_t Profiler::buildProfilingRequestPacket() const
{
	packet_t profilingRequest;
	profilingRequest.id = 10;
	profilingRequest.size.value = 1;
	profilingRequest.payload.add('P');
	return profilingRequest;
}

uint64_t Profiler::profile(PacketCommunicator& communicator)
{
	// start profiler
	auto profilingPacket = buildProfilingRequestPacket();
	auto response = communicator.request(profilingPacket);

	if (response.id != 65)
	{
		return INVALID_WCET;
	}

	uint32_t* payload = (uint32_t*) response.payload.data();
	uint32_t execTime = *payload;

	return execTime;
}
