/*
 * SerialLink.h
 *
 *  Created on: Mar 25, 2019
 *      Author: tim
 */

#ifndef COMPONENTS_COMLINK_SERIALLINK_H_
#define COMPONENTS_COMLINK_SERIALLINK_H_

#include "IComLink.h"
#include <string>

class SerialLink: public IComLink
{
private:
	int hSerialPort;

public:
	SerialLink();
	virtual ~SerialLink();

	/*
	 * Name: 		initialize
	 *
	 * Purpose: 	Initializes the comm driver (or the comm peripheral).
	 *
	 * Arguments: 	<none>
	 *
	 * Notes: This is called from the core app before any calls to read and write.
	 */
	virtual bool initialize(const std::string& initString);

	/*
	 * Name:		write
	 *
	 * Purpose: 	Sends an arbitrary amount of bytes over the communication channel
	 * 				to the host.
	 *
	 * Arguments: 	data - a pointer to the data buffer to be sent to the host
	 * 				count - the number of bytes to send from the data buffer
	 */
	virtual void writeData(const uint8_t* data, uint32_t count);

	/*
	 * Name:		read
	 *
	 * Purpose: 	Reads a byte from the communication interface for further processing
	 *
	 * Arguments: 	<none>
	 */
	virtual uint8_t readData();
};

#endif /* COMPONENTS_COMLINK_SERIALLINK_H_ */
