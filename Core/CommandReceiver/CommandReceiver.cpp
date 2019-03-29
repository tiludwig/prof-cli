/*
 * CommandReceiver.cpp
 *
 *  Created on: Mar 25, 2019
 *      Author: tim
 */

#include "CommandReceiver.h"
#include <cstdio>
#include <stdlib.h>

void clearScreen()
{
	printf("\033[2J"); // clear screen
	fflush(stdout);
}

CommandReceiver::CommandReceiver()
{

}

CommandReceiver::~CommandReceiver()
{

}

bool CommandReceiver::process(char value)
{
	protocol.process(value);

	if (protocol.isPacketComplete(lastReceivedPacket) == true)
	{
		//printf("Got packet [id=%d]\n", lastReceivedPacket.id);

		if (lastReceivedPacket.id == 65)
		{
			int32_t* ptrPayload = (int32_t*)&lastReceivedPacket.payload[0];
			uint32_t value = static_cast<uint32_t>(ptrPayload[0]);

			float wcet = value / 72.0;
			uint32_t corr_value = value - 1073;
			float corr_wcet = corr_value / 72.0;

			printf("\033[1;0H\033[K\033[1;0H");
			printf("Time: %u\n", value); //, ~%.2f us (%d, ~%.2f us)\n", value, wcet, corr_value, corr_wcet);

			printf("\033[2;0H\033[K\033[2;0H");

			printf("state = [ ");
			for (int i = 1; i < 5; i++)
			{
				printf("%d ", ptrPayload[i]);
			}
			printf("]\n");

		}
		else if (lastReceivedPacket.id == 11)
		{
			printf("\033[3;0H\033[K\033[3;0H");

			lastReceivedPacket.payload.push_back('\0');
			printf("Result: %s\n", &lastReceivedPacket.payload[0]);
		}
		printf("\033[4;0H\033[K\033[4;0H");
		printf("dropped %u packets\n", protocol.badPackets);
		return true;
	}

	return false;
}

void CommandReceiver::waitForResponse()
{

}
