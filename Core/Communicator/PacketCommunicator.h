/*
 * PacketCommunicator.h
 *
 *  Created on: 29.03.2019
 *      Author: Tim
 */

#ifndef CORE_COMMUNICATOR_PACKETCOMMUNICATOR_H_
#define CORE_COMMUNICATOR_PACKETCOMMUNICATOR_H_

#include <Components/ComLink/IComLink.h>
#include <Core/DataLink/PacketProtocol.h>
#include <Core/DataLink/Packet.h>

class PacketCommunicator
{
private:
	IComLink* comDriver;
	PacketProtocol protocol;
private:
	void sendStartSymbol();
	void send(char* buffer, uint16_t size);
	void sendByte(char value);
public:
	PacketCommunicator(IComLink* link);

	int8_t calculateChecksum(packet_t& packet);
	void sendPacket(packet_t& packet);
	void receivePacket(packet_t& packet);


	packet_t request(packet_t& request);
};

#endif /* CORE_COMMUNICATOR_PACKETCOMMUNICATOR_H_ */
