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
	packet_t profilingTarget = { 30, static_cast<uint16_t>(targetname.length() + 1), const_cast<char*>(targetname.c_str()) };
	communicator.request(profilingTarget);
	return true;
}

constexpr packet_t Profiler::buildProfilingRequestPacket() const
{
	packet_t profilingRequest = { 10, 1, const_cast<char*>("P") };
	return profilingRequest;
}

uint64_t Profiler::profile(PacketCommunicator& communicator, TestinputProvider& dataprovider)
{
	// send input values
	auto dataset = dataprovider.getNextDataset();
	packet_t simulatedAccValues = { 20, static_cast<uint16_t>(dataset.size), dataset.data };
	auto response = communicator.request(simulatedAccValues);

	// start profiler
	auto profilingPacket = buildProfilingRequestPacket();
	response = communicator.request(profilingPacket);

	if (response.id == 65)
	{
		uint32_t* payload = (uint32_t*) response.payload;
		uint32_t execTime = payload[0];

		minMaxStats.update(execTime);
		freqDistribution.update(execTime);
		varStats.update(execTime);

		return execTime;
	}

	return minMaxStats.getMax();
}

uint64_t Profiler::getWCET()
{
	return minMaxStats.getMax();
}

std::vector<freqdist_entry_t>* Profiler::getFrequencyDistribution()
{
	return freqDistribution.getDistribution();
}
