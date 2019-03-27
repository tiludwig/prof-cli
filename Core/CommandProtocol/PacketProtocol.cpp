/*
 * PacketProtocol.cpp
 *
 *  Created on: Mar 25, 2019
 *      Author: tim
 */

#include "PacketProtocol.h"
#include <cstdio>

static const char* named_states[6] = { "WaitingForStart", "ReceivingId", "ReceivingSizeMSB", "ReceivingSizeLSB",
		"ReceivingPayload", "ReceivingChecksum" };

PacketProtocol::PacketProtocol()
{
	reset();
	badPackets = 0;
}

PacketProtocol::~PacketProtocol()
{

}

void PacketProtocol::process(char value)
{
	auto valueType = checkForReservedSymbol(value);
	if (valueType == StartSymbol)
	{
		reset();
		state = ReceivingId;
		return;
	}
	if (valueType == StuffSymbol)
	{
		needsDestuffing = true;
		// subtract from checksum again
		return;
	}
	else
	{
		if (needsDestuffing == true)
		{
			needsDestuffing = false;
			value = value ^ 0x20;
		}
	}

	packet.checksum += value;

	switch (state)
	{
	case WaitingForStart:
		//printf("NO SYNC '%c' (0x%u)\n", value, static_cast<uint8_t>(value));
		printf("Dbg: Received %u ", (uint8_t)value);
		printf("while in '%s'\n", named_states[state]);
		break;
	case ReceivingId:
		packet.id = static_cast<uint8_t>(value);
		state = ReceivingSizeLSB;
		break;
	case ReceivingSizeLSB:
		packet.size = static_cast<uint8_t>(value);
		state = ReceivingSizeMSB;
		break;
	case ReceivingSizeMSB:
		packet.size |= (static_cast<uint8_t>(value) << 8);
		bytesToReceive = packet.size;
		state = ReceivingPayload;
		break;
	case ReceivingPayload:
		packet.payload.push_back(value);
		if (--bytesToReceive <= 0)
		{
			state = ReceivingChecksum;
		}
		break;
	case ReceivingChecksum:
		if (packet.checksum == 0)
		{
			packetReceived = true;
		}
		else
		{
			badPackets++;
		}
		state = WaitingForStart;
		break;
	}
}

bool PacketProtocol::isPacketComplete(packet_t& packet)
{
	if (packetReceived == false)
		return false;

	packet = this->packet;
	return true;
}

void PacketProtocol::reset()
{
	// reset packet
	packet.id = 0;
	packet.size = 0;
	packet.checksum = 0;
	packet.payload.clear();

	// reset receiver
	bytesToReceive = 0;
	packetReceived = false;
	state = WaitingForStart;
	needsDestuffing = false;
}

bool PacketProtocol::isStartSymbol(char value)
{
	return (value == '#');
}

ReservedSymbol PacketProtocol::checkForReservedSymbol(char value)
{
	//printf("Checking symbol '%c'\n", value);
	switch (value)
	{
	case '#':
		return StartSymbol;
	case '?':
		return StuffSymbol;
	default:
		return NotReserved;
	}
}

