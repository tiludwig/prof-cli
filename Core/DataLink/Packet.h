/*
 * Packet.h
 *
 *  Created on: 28.03.2019
 *      Author: Tim
 */

#ifndef CORE_DATALINK_PACKET_H_
#define CORE_DATALINK_PACKET_H_

#include <stdint.h>

union packet_size_t
{
	uint16_t value;
	struct
	{
		uint16_t lsb : 8;
		uint16_t msb : 8;
	} raw;
};

struct packet_t
{
	uint8_t id;
	packet_size_t size;
	char* payload;
};
#endif /* CORE_DATALINK_PACKET_H_ */
