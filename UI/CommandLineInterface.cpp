/*
 * CommandLineInterface.cpp
 *
 *  Created on: Apr 1, 2019
 *      Author: tim
 */

#include <UI/CommandLineInterface.h>
#include <cstdio>

namespace UI
{

void CommandLineInterface::initialize(int32_t iterations, int32_t updateInterval)
{
	progressBarCurrentTotal = updateInterval;
	progressBarTotalTotal = iterations;
	printf("\e[2J\e[0;0H[Settings]\n");
	printf("\tCore clock: 72 [MHz]\n");
	printf("\tIterations: %d\n", iterations);
	printf("[Run]\n");
	printf("\tRemaining: 0h0m0s\n");
	printf("\tcurrent: [####################] 0%%\n");
	printf("\ttotal:   [####################] 0%%\n");
	printf("[Result]\n");
	printf("\e[s");
	bottomLine = 0;
	fflush(stdout);
}

void CommandLineInterface::printRemainingTime(uint64_t secondsRemaining)
{
	int hours = secondsRemaining / 3600;
	secondsRemaining -= (hours * 3600);
	int minutes = secondsRemaining / 60;
	secondsRemaining -= (minutes * 60);

	printf("\e[u\e[4A\e[K");
	fflush(stdout);
	printf("\tRemaining: %dh%dm%ds\n", hours, minutes, secondsRemaining);
}

void CommandLineInterface::updateCurrentProgressbar(int32_t value)
{
	printf("\e[u\e[3A\e[K");
	fflush(stdout);
	printf("\tcurrent: [");

	float percentage = (float) value / progressBarCurrentTotal;
	int barValue = 20 * percentage;

	for (int i = 0; i < barValue; i++)
		printf("#");

	for (int i = 0; i < (20 - barValue); i++)
		printf(" ");

	printf("] %3.0f%%\n", (percentage * 100.0));
}

void CommandLineInterface::updateTotalProgressbar(int32_t value)
{
	printf("\e[u\e[2A\e[K");
	fflush(stdout);
	printf("\ttotal:   [");

	float percentage = (float) value / progressBarTotalTotal;
	int barValue = 20 * percentage;

	for (int i = 0; i < barValue; i++)
		printf("#");

	for (int i = 0; i < (20 - barValue); i++)
		printf(" ");

	printf("] %3.0f%%\n", (percentage * 100.0));
}

void CommandLineInterface::printResult(uint32_t value)
{
	float execTime = value / 72.0;

	printf("\e[u\e[K");
	printf("\tExecution time: %u [%.2f us]\n", value, execTime);
}

void CommandLineInterface::printState(uint32_t* stateVec)
{
	printf("\e[u\e[1B\e[K");
	printf("\tState: [ ");

	for (int i = 0; i < 4; i++)
	{
		printf("%u ", stateVec[i]);
	}
	printf(" ]\n");
}

void CommandLineInterface::printCounter(uint32_t value)
{
	printf("\e[u\e[2B\e[K");
	printf("\tPackets received: %u\n", value);
}

} /* namespace UI */
