#include <cstdio>
#include "Components/ComLink/SerialLink.h"
#include "Core/Communicator/PacketCommunicator.h"
#include "Core/DataLink/Packet.h"
#include "Utility/MinMax.hpp"
#include "Utility/Variance.hpp"
#include "Utility/FrequencyDistribution.h"
#include "UI/CommandLineInterface.h"
#include <iostream>
#include <chrono>
#include <signal.h>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <thread>

using namespace UI;

volatile bool bRunning = true;

volatile uint32_t wcet;
void sigfunc(int sig)
{
	if (sig != SIGINT)
		return;
	else
	{
		fflush(stdout);
		printf("\e[2J\e[0;0H");
		fflush(stdout);
		printf("Profiling done.\n");
		printf("Measured WCET: %u cycles [%.2f us].\n", wcet, (wcet / 72.0));
		exit(0);
	}
}

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

	std::string taskname(argv[3]);

	FrequencyDistribution freqStats;

	signal(SIGINT, sigfunc);

	CommandLineInterface ui;
	ui.initialize(iterations, uiUpdateIterations);

	SerialLink link;

	if (link.initialize() == false)
	{
		printf("Error initializing serial port\n");
		return 0;
	}

	PacketCommunicator communicator(&link);

	char profRequestBuffer = 'P';
	int32_t simAccValues[3] = { 1, 20, -1 };

	packet_t simulatedAccValues = { 20, 3*sizeof(int32_t), (char*) simAccValues };
	packet_t profilingRequest = { 10, 1, &profRequestBuffer };
	packet_t profilingTarget = {30, static_cast<uint16_t>(taskname.length() + 1), const_cast<char*>(taskname.c_str())};

	communicator.request(profilingTarget);
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
		/* initialize random seed: */
		srand(time(NULL));

		/* generate delay number between 1 and 10: */
		int delayTime = rand() % 5 + 1;
		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

		start = std::chrono::steady_clock::now();
		response = communicator.request(profilingRequest);

		counter++;
		if (response.id == 65)
		{
			uint32_t* payload = (uint32_t*) response.payload;
			uint32_t execTime = payload[0];

			minMaxStats.update(execTime);
			wcet = minMaxStats.getMax();
			varStats.update(execTime);
			freqStats.update(execTime);

			if (uiIterations == uiUpdateIterations)
			{
				uiIterations = 0;
				ui.printResult(minMaxStats.getMax());
				printf("\tMean: %f\n", varStats.getMean());
				printf("\tStd.Dev.: %f\n", sqrt(varStats.getVariance()));
				printf("\tSample Count: %u\n", varStats.getSampleCount());
				//printDist(freqStats);
			}
			uiIterations++;
			ui.updateCurrentProgressbar(uiIterations);
			ui.updateTotalProgressbar(i);
		}

		end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		timeStats.update(duration);
		if ((i % 100) == 0)
		{
			double meanduration = timeStats.getMean();
			int64_t estimatedSeconds = meanduration * (iterations - i) / 1000000;
			ui.printRemainingTime(estimatedSeconds);
		}
	}

	fflush(stdout);
	printf("\e[2J\e[0;0H");
	fflush(stdout);
	printf("Profiling done.\n");
	printf("Measured WCET: %u cycles [%.2f us].\n", wcet, (wcet / 72.0));

	auto dist = freqStats.getDistribution();

	std::sort(dist->begin(), dist->end(), [](freqdist_entry_t a, freqdist_entry_t b){return a.value < b.value;});

	std::ofstream myfile;
	myfile.open("wcet-dist.csv");
	for(auto& slot : *dist)
		myfile << slot.value << "," << slot.counts << "\n";

	myfile.close();
}
