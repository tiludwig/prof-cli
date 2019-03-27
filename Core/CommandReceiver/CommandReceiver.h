/*
 * CommandReceiver.h
 *
 *  Created on: Mar 25, 2019
 *      Author: tim
 */

#ifndef CORE_COMMANDRECEIVER_H_
#define CORE_COMMANDRECEIVER_H_

#include "../CommandProtocol/PacketProtocol.h"


class CommandReceiver
{
private:
	PacketProtocol protocol;
	packet_t lastReceivedPacket;
public:
	CommandReceiver();
	virtual ~CommandReceiver();

	bool process(char value);
	void waitForResponse();
};

#endif /* CORE_COMMANDRECEIVER_H_ */
