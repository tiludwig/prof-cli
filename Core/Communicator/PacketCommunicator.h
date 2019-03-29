/*
 * PacketCommunicator.h
 *
 *  Created on: Mar 29, 2019
 *      Author: tim
 */

#ifndef CORE_COMMUNICATOR_PACKETCOMMUNICATOR_H_
#define CORE_COMMUNICATOR_PACKETCOMMUNICATOR_H_

#include <stdint.h>
#include <vector>

#include "../CommandReceiver/CommandReceiver.h"
#include "../../Components/ComLink/SerialLink.h"
#include "../CommandProtocol/PacketProtocol.h"

class PacketCommunicator
{
public:
	SerialLink* link;

	PacketCommunicator();
	virtual ~PacketCommunicator();

	void putc(char value);
	void generateStartSymbol();

	void sendPacket(packet_t& packet);
	void receivePacket(packet_t& packet, SerialLink& link, CommandReceiver& receiver);

	packet_t request(packet_t& request, SerialLink& link, CommandReceiver& receiver);
};

#endif /* CORE_COMMUNICATOR_PACKETCOMMUNICATOR_H_ */
