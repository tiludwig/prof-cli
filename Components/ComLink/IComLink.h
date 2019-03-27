/*
 * IComLink.h
 *
 *  Created on: Mar 25, 2019
 *      Author: tim
 */

#ifndef COMPONENTS_COMLINK_ICOMLINK_H_
#define COMPONENTS_COMLINK_ICOMLINK_H_


#include <stdint.h>

/*
 * Name: 	IComLink
 *
 * Purpose:	Provides a well-defined interface for the host communication module.
 *
 * 			The 'initialize' method should be used to initialize the comm driver
 * 			being used (or the comm peripheral in bare-metal applications).
 *
 * 			The two methods 'write' and 'read' should provide a mean for sending
 * 			an arbitrary amount of bytes and receiving a single byte from the
 * 			communication interface for further processing.
 */
class IComLink
{
public:

	/*
	 * Name:		~IComLink
	 *
	 * Purpose:		Destructs the communication link object and ensures that the
	 * 				underlying driver is in a known state.
	 */
	virtual ~IComLink(){}

	/*
	 * Name: 		initialize
	 *
	 * Purpose: 	Initializes the comm driver (or the comm peripheral).
	 *
	 * Arguments: 	<none>
	 *
	 * Notes: This is called from the core app before any calls to read and write.
	 */
	virtual bool initialize() = 0;

	/*
	 * Name:		write
	 *
	 * Purpose: 	Sends an arbitrary amount of bytes over the communication channel
	 * 				to the host.
	 *
	 * Arguments: 	data - a pointer to the data buffer to be sent to the host
	 * 				count - the number of bytes to send from the data buffer
	 */
	virtual void writeData(const uint8_t* data, uint32_t count) = 0;

	/*
	 * Name:		read
	 *
	 * Purpose: 	Reads a byte from the communication interface for further processing
	 *
	 * Arguments: 	<none>
	 */
	virtual uint8_t readData() = 0;
};


#endif /* COMPONENTS_COMLINK_ICOMLINK_H_ */
