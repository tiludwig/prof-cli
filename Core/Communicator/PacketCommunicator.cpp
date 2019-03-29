/*
 * PacketCommunicator.cpp
 *
 *  Created on: Mar 29, 2019
 *      Author: tim
 */

#include "PacketCommunicator.h"

void PacketCommunicator::putc(char value)
{
	if (value == '?' || value == '#')
	{
		link->writeData((const uint8_t*) "?", 1);
		value = value ^ 0x20;
	}

	link->writeData((const uint8_t*) &value, 1);
}

void PacketCommunicator::generateStartSymbol()
{
	link->writeData((const uint8_t*) "#", 1);
}

PacketCommunicator::PacketCommunicator()
{
	link = nullptr;

}

PacketCommunicator::~PacketCommunicator()
{
	// TODO Auto-generated destructor stub
}

void PacketCommunicator::sendPacket(packet_t& packet)
{
	uint8_t lenLSB = packet.size & 0xFF;
	uint8_t lenMSB = (packet.size >> 8) & 0xFF;

	packet.checksum = packet.id;
	packet.checksum += lenLSB;
	packet.checksum += lenMSB;

	generateStartSymbol();
	putc(packet.id);
	putc(lenLSB);
	putc(lenMSB);

	for (uint16_t i = 0; i < packet.size; i++)
	{
		char value = packet.payload[i];
		packet.checksum += value;
		putc(value);
	}

	packet.checksum = -packet.checksum;
	putc(packet.checksum);
}

void PacketCommunicator::receivePacket(packet_t& packet, SerialLink& link, CommandReceiver& receiver)
{
	auto data = link.readData();
	while (receiver.process(data) == false)
	{
		data = link.readData();
	}
}

packet_t PacketCommunicator::request(packet_t& request, SerialLink& link, CommandReceiver& receiver)
{
	sendPacket(request);

	packet_t response;

	return response;
}
