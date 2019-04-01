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
			exit(0);
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
		printf("\tRemaining: 0h0m0s\n");
		printf("\tcurrent: [####################] 0%%\n");
		printf("\ttotal:   [####################] 0%%\n");
		printf("[Result]\n");
		printf("\e[s");
		fflush(stdout);
	}

	void updateRemainingTime(int hours, int minutes, int seconds)
	{
		printf("\e[u\e[4A\e[K");
		fflush(stdout);
		printf("\tRemaining: %dh%dm%ds\n", hours, minutes, seconds);
	}

	void updateCurrentBar(int current, int total)
	{
		printf("\e[u\e[3A\e[K");
		fflush(stdout);
		printf("\tcurrent: [");

		float percentage = (float) current / total;
		int barValue = 20 * percentage;

		for (int i = 0; i < barValue; i++)
			printf("#");

		for (int i = 0; i < (20 - barValue); i++)
			printf(" ");

		printf("] %3.0f%%\n", (percentage * 100.0));
	}

	void updateTotalBar(int current, int total)
	{
		printf("\e[u\e[2A\e[K");
		fflush(stdout);
		printf("\ttotal:   [");

		float percentage = (float) current / total;
		int barValue = 20 * percentage;

		for (int i = 0; i < barValue; i++)
			printf("#");

		for (int i = 0; i < (20 - barValue); i++)
			printf(" ");

		printf("] %3.0f%%\n", (percentage * 100.0));
	}

	void printResult(uint32_t result)
	{
		float execTime = result / 72.0;

		printf("\e[u\e[K");
		printf("\tExecution time: %u [%.2f us]\n", result, execTime);
	}

	void printState(uint32_t* state)
	{
		printf("\e[u\e[1B\e[K");
		printf("\tState: [ ");

		for (int i = 0; i < 4; i++)
		{
			printf("%u ", state[i]);
		}
		printf(" ]\n");
	}

	void printCounter(uint32_t counter)
	{
		printf("\e[u\e[2B\e[K");
		printf("\tPackets received: %u\n", counter);
	}
};

void printDist(FrequencyDistribution& freqStat)
{
	auto freqDist = freqStat.getDistribution();

	for (auto& slot : *freqDist)
		printf("\t%lu: %lu\n", slot.value, slot.counts);
}

int main(int argc, char** argv)
{
	int iterations = atoi(argv[1]);
	int uiUpdateIterations = atoi(argv[2]);

	printf("[Settings]\n");
	printf("\tCore clock: 72 [MHz]\n");
	printf("\tIterations: %d\n", iterations);
	printf("[Run]\n");

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
	int uiIterations = 0;
	MinMax minMaxStats;
	Variance varStats;
	Variance timeStats;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	auto response = communicator.request(simulatedAccValues);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	for (int i = 0; i < iterations; i++)
	{
		start = std::chrono::steady_clock::now();
		response = communicator.request(profilingRequest);

		counter++;
		if (response.id == 65)
		{
			uint32_t* payload = (uint32_t*) response.payload;
			uint32_t execTime = payload[0];

			minMaxStats.update(execTime);
			varStats.update(execTime);
			freqStats.update(execTime);

			if (uiIterations == uiUpdateIterations)
			{
				uiIterations = 0;
				ui.printResult(minMaxStats.getMax());
				printf("\tMean: %f\n", varStats.getMean());
				printf("\tStd.Dev.: %f\n", sqrt(varStats.getVariance()));
				printf("\tSample Count: %u\n", varStats.getSampleCount());
				printDist(freqStats);
			}
			uiIterations++;
			ui.updateCurrentBar(uiIterations, uiUpdateIterations);
			ui.updateTotalBar(i, iterations);
		}

		end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		timeStats.update(duration);
		if ((i % 100) == 0)
		{
			double meanduration = timeStats.getMean();
			int64_t estimatedSeconds = meanduration * (iterations-i) / 1000000;
			int estimatedHours = estimatedSeconds / 3600;
			estimatedSeconds -= (estimatedHours * 3600);
			int estimatedMinutes = estimatedSeconds / 60;
			estimatedSeconds -= (estimatedMinutes * 60);
			ui.updateRemainingTime(estimatedHours, estimatedMinutes, estimatedSeconds);
		}
	}
	printf("Exiting\n");
}
