/*
 * PacketProtocol.h
 *
 *  Created on: Mar 25, 2019
 *      Author: tim
 */

#ifndef CORE_COMMANDPROTOCOL_PACKETPROTOCOL_H_
#define CORE_COMMANDPROTOCOL_PACKETPROTOCOL_H_

#include <stdint.h>
#include <vector>

enum ReceiverState
{
	WaitingForStart,
	ReceivingId,
	ReceivingSizeMSB,
	ReceivingSizeLSB,
	ReceivingPayload,
	ReceivingChecksum,
};

enum ReservedSymbol
{
	StartSymbol,
	StuffSymbol,
	NotReserved
};


struct packet_t
{
	uint8_t id;
	uint16_t size;
	std::vector<char> payload;
	int8_t checksum;
};

class PacketProtocol
{
private:
	ReceiverState state;
	packet_t packet;
	uint16_t bytesToReceive;
	bool packetReceived;
	bool needsDestuffing;
public:
	uint32_t badPackets;
private:
	bool isStartSymbol(char value);
	ReservedSymbol checkForReservedSymbol(char value);
public:
	PacketProtocol();
	virtual ~PacketProtocol();

	void process(char value);
	bool isPacketComplete(packet_t& packet);
	void reset();
};

#endif /* CORE_COMMANDPROTOCOL_PACKETPROTOCOL_H_ */
