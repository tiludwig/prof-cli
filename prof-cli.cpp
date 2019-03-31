#include <cstdio>
#include "Components/ComLink/SerialLink.h"
#include "Core/Communicator/PacketCommunicator.h"
#include "Core/DataLink/Packet.h"
#include "Utility/MinMax.hpp"
#include "Utility/Variance.hpp"
#include "Utility/FrequencyDistribution.h"

#include <iostream>

#include <chrono>
#include <thread>
#include <cmath>
#include <signal.h>

volatile bool bRunning = true;

void sigfunc(int sig)
{
	int c;

	if (sig != SIGINT)
		return;
	else
	{
		printf("\nWollen Sie das Programm beenden (j/n) : ");
		c = getchar();
		if (c == 'j')
			bRunning = false;
		else
			return;
	}
}

volatile uint32_t timesWritten;

class UI
{
public:
	void initialize()
	{
		printf("\e[s");
		fflush(stdout);
	}

	void printResult(uint32_t result)
	{
		float execTime = result / 72.0;

		printf("\e[u\e[K");
		printf("Execution time: %u [%.2f us]\n", result, execTime);
	}

	void printState(uint32_t* state)
	{
		printf("\e[u\e[1B\e[K");
		printf("State: [ ");

		for (int i = 0; i < 4; i++)
		{
			printf("%u ", state[i]);
		}
		printf(" ]\n");
	}

	void printCounter(uint32_t counter)
	{
		printf("\e[u\e[2B\e[K");
		printf("Packets received: %u\n", counter);
	}
};

void printDist(FrequencyDistribution& freqStat)
{
	auto freqDist = freqStat.getDistribution();

	for (auto& slot : *freqDist)
		printf("%lu: %lu\n", slot.value, slot.counts);
}

int main()
{
	FrequencyDistribution freqStats;

	signal(SIGINT, sigfunc);

	UI ui;
	ui.initialize();

	SerialLink link;

	if (link.initialize() == false)
	{
		printf("Error initializing serial port\n");
		return 0;
	}

	PacketCommunicator communicator(&link);

	char profRequestBuffer = 'P';
	int32_t simAccValues[2] = { 0, 0 };

	packet_t simulatedAccValues = { 10, 8, (char*) simAccValues };
	packet_t profilingRequest = { 20, 1, &profRequestBuffer };
	unsigned int counter = 0;
	MinMax minMaxStats;
	Variance varStats;
	auto response = communicator.request(simulatedAccValues);
	while (bRunning)
	{
		response = communicator.request(profilingRequest);
		counter++;
		if (response.id == 65)
		{
			uint32_t* payload = (uint32_t*) response.payload;
			uint32_t execTime = payload[0];

			minMaxStats.update(execTime);
			varStats.update(execTime);
			freqStats.update(execTime);

			ui.printResult(minMaxStats.getMax());
			printf("Mean: %f\n", varStats.getMean());
			printf("Std.Dev.: %f\n", sqrt(varStats.getVariance()));
			printf("Sample Count: %u\n", varStats.getSampleCount());

			if (((counter + 1) % 1000) == 0)
			{
				printDist(freqStats);
			}
		}
		//ui.printCounter(counter);
	}
	printf("Exiting\n");
}
