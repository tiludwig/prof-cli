/*
 * PacketProtocol.h
 *
 *  Created on: 28.03.2019
 *      Author: Tim
 */

#ifndef CORE_DATALINK_PACKETPROTOCOL_H_
#define CORE_DATALINK_PACKETPROTOCOL_H_

#include <TTTProfConfig.h>
#include <Core/Buffer/Buffer.h>
#include <Core/DataLink/Packet.h>


enum DataType_t
{
	VT_StartSymbol,
	VT_EscapeSymbol,
	VT_DefaultSymbol,
};

enum ReceiverState_t
{
	RS_WaitingForStart,
	RS_ReceivingId,
	RS_ReceivingSizeLSB,
	RS_ReceivingSizeMSB,
	RS_ReceivingPayload,
	RS_ReceivingChecksum
};

class PacketProtocol
{
private:
	Buffer<char, tttConfig_MAX_RECV_PACKET_SIZE> receiveBuffer;
	packet_t receivedPacket;
	uint32_t bytesToReceive;
	bool isComplete;
	int8_t runningSum;
	bool escapedSymbolFound;

	ReceiverState_t state;
private:
	bool isValueStartSymbol(char value);
	bool isValueEscaped(char value);
	void processPacketContent(char value);
	DataType_t checkForReservedSymbol(char value);
	void reset();
public:
	PacketProtocol();

	void processByte(char value);
	bool isPacketComplete();

	bool getPacket(packet_t& packet);
};

#endif /* CORE_DATALINK_PACKETPROTOCOL_H_ */
