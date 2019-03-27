/*
 * CommandReceiver.cpp
 *
 *  Created on: Mar 25, 2019
 *      Author: tim
 */

#include "CommandReceiver.h"
#include <cstdio>
#include <stdlib.h>

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

		auto ptrPayload = &lastReceivedPacket.payload[0];
		if (lastReceivedPacket.id == 65)
		{
			lastReceivedPacket.payload.push_back('\0');
			uint32_t value = static_cast<uint32_t>(atoi(ptrPayload));

			float wcet = value / 72.0;
			uint32_t corr_value = value - 1073;
			float corr_wcet = corr_value / 72.0;

			printf("Time: %d\n", value);//, ~%.2f us (%d, ~%.2f us)\n", value, wcet, corr_value, corr_wcet);

		}
		else if(lastReceivedPacket.id == 10)
		{
			int32_t* state_vector = (int32_t*)&lastReceivedPacket.payload[0];
			printf("state = [ ");
			for(int i = 0; i < 4; i++)
			{
				printf("%d ", state_vector[i]);
			}
			printf("]\n");
		}
		printf("dropped %u packets\n", protocol.badPackets);
		return true;
	}

	return false;
}

void CommandReceiver::waitForResponse()
{

}
