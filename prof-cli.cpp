#include <cstdio>
#include "Components/ComLink/SerialLink.h"
#include "Core/Communicator/PacketCommunicator.h"
#include "Core/DataLink/Packet.h"

#include <iostream>

#include <chrono>
#include <thread>

volatile uint32_t timesWritten;

int main()
{
	SerialLink link;
	if (link.initialize() == false)
	{
		printf("Error initializing serial port\n");
		return 0;
	}

	PacketCommunicator communicator(&link);

	char profRequestBuffer ='P';
	int32_t simAccValues[2] = {0,0};
	packet_t profilingRequest = {10, 1, &profRequestBuffer};
	packet_t simulatedAccValues = {20, 8, (char*)simAccValues};
	unsigned int counter = 0;
	while (true)
	{
		printf("Sending simulated sensor values ...");
		auto response = communicator.request(simulatedAccValues);
		if(response.id == 11)
		{
			printf("ok\n");
		}
		else
		{
			printf("failed\n");
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
		printf("Sending profiling request ...");
		fflush(stdout);
		response = communicator.request(profilingRequest);
		printf("ok (%u)\n", counter++);
		if(response.id == 65)
		{

			uint32_t* payload = (uint32_t*)response.payload;
			uint32_t execTime = payload[0];
			uint32_t state_x = payload[1];
			uint32_t state_y = payload[2];
			uint32_t state_v = payload[3];
			uint32_t state_w = payload[4];

			printf("Measured execution time: %u cycles\n", execTime);
			printf("New state is: [%u %u %u %u]\n", state_x, state_y, state_v, state_w);
		}
	}
	printf("Exiting\n");
}
