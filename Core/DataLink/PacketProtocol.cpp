/*
 * PacketProtocol.cpp
 *
 *  Created on: 28.03.2019
 *      Author: Tim
 */

#include "PacketProtocol.h"
#include <cstdio>

PacketProtocol::PacketProtocol()
{
	reset();
}

void PacketProtocol::reset()
{
	isComplete = false;
	state = RS_WaitingForStart;
	runningSum = 0;
	escapedSymbolFound = false;
	bytesToReceive = 0;
	receiveBuffer.clear();
}

DataType_t PacketProtocol::checkForReservedSymbol(char value)
{
	if (value == '#')
		return VT_StartSymbol;
	else if (value == '?')
		return VT_EscapeSymbol;

	return VT_DefaultSymbol;
}

void PacketProtocol::processByte(char value)
{
	// check for start and reserved symbols
	DataType_t valueType = checkForReservedSymbol(value);
	switch (valueType)
	{
	case VT_StartSymbol:
		reset();
		state = RS_ReceivingId;
		break;
	case VT_EscapeSymbol:
		escapedSymbolFound = true;
		break;
	default:
		if (escapedSymbolFound)
		{
			value = value ^ 0x20;
			escapedSymbolFound = false;
		}

		processPacketContent(value);
		break;
	}
}

void PacketProtocol::processPacketContent(char value)
{
	runningSum += value;
	switch (state)
	{
	case RS_WaitingForStart:
		break;
	case RS_ReceivingId:
		receivedPacket.id = value;
		state = RS_ReceivingSizeLSB;
		break;

	case RS_ReceivingSizeLSB:
		receivedPacket.size.raw.lsb = value;
		state = RS_ReceivingSizeMSB;
		break;

	case RS_ReceivingSizeMSB:
		receivedPacket.size.raw.msb = value;
		bytesToReceive = receivedPacket.size.value;

		if (bytesToReceive > tttConfig_MAX_RECV_PACKET_SIZE)
		{
			state = RS_WaitingForStart;
		}
		else
		{
			state = RS_ReceivingPayload;
		}
		break;

	case RS_ReceivingPayload:
		receiveBuffer.append(value);
		if (--bytesToReceive == 0)
		{
			state = RS_ReceivingChecksum;
		}
		//printf("P %u\n", value);
		break;

	case RS_ReceivingChecksum:
		if (runningSum == 0)
		{
			isComplete = true;
		}
		state = RS_WaitingForStart;
		break;
	}
}

bool PacketProtocol::isPacketComplete()
{
	return isComplete;
}

bool PacketProtocol::getPacket(packet_t& packet)
{
	isComplete = false;

	packet.id = receivedPacket.id;
	packet.size.value = receivedPacket.size.value;
	//packet.payload.add(receiveBuffer.data(),
	packet.payload.add(receiveBuffer.data(), receivedPacket.size.value);
	return true;
}
