/*
 * SerialLink.cpp
 *
 *  Created on: Mar 25, 2019
 *      Author: tim
 */

#include "SerialLink.h"

#include <Core/Exceptions/CustomException.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <string.h>
#include <cstdio>

SerialLink::SerialLink()
{
	hSerialPort = -1;
}

SerialLink::~SerialLink()
{
	close(hSerialPort);
	hSerialPort = -1;
	printf("Closing serial port\n");
}

/*
 * Name: 		initialize
 *
 * Purpose: 	Initializes the comm driver (or the comm peripheral).
 *
 * Arguments: 	<none>
 *
 * Notes: This is called from the core app before any calls to read and write.
 */
bool SerialLink::initialize(const std::string& initString)
{
	// Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
	hSerialPort = open(initString.c_str(), O_RDWR);

	if (hSerialPort < 0)
	{
		//printf("Error %i opening the serial port: %s\n", errno, strerror(errno));
		throw CustomException(errno, "Unable to open the serial port");
	}

	// Create new termios struc, we call it 'tty' for convention
	struct termios tty;
	memset(&tty, 0, sizeof tty);

	// Read in existing settings, and handle any error
	if (tcgetattr(hSerialPort, &tty) != 0)
	{
		//printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
		close(hSerialPort);
		hSerialPort = -1;
		throw CustomException(errno, "Unable to read settings of serial port");
	}

	tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
	tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
	tty.c_cflag |= CS8; // 8 bits per byte (most common)
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
	// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
	// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

	tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 0;

	// Set in/out baud rate to be 9600
	cfsetispeed(&tty, B19200);
	cfsetospeed(&tty, B19200);

	// Save tty settings, also checking for error
	if (tcsetattr(hSerialPort, TCSANOW, &tty) != 0)
	{
		//printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		close(hSerialPort);
		hSerialPort = -1;
		throw CustomException(errno, "Unable to apply serial port settings");
	}

	return true;
}

/*
 * Name:		write
 *
 * Purpose: 	Sends an arbitrary amount of bytes over the communication channel
 * 				to the host.
 *
 * Arguments: 	data - a pointer to the data buffer to be sent to the host
 * 				count - the number of bytes to send from the data buffer
 */
void SerialLink::writeData(const uint8_t* data, uint32_t count)
{
	while(count > 0)
	{
		count -= write(hSerialPort, data, count);
	}
}

/*
 * Name:		read
 *
 * Purpose: 	Reads a byte from the communication interface for further processing
 *
 * Arguments: 	<none>
 */
uint8_t SerialLink::readData()
{
	char recvdByte;

	int num_bytes = 0;
	while (num_bytes == 0)
	{
		// Read bytes. The behaviour of read() (e.g. does it block?,
		// how long does it block for?) depends on the configuration
		// settings above, specifically VMIN and VTIME
		num_bytes = read(hSerialPort, &recvdByte, 1);
		if (num_bytes < 0)
		{
			//printf("Error %i from readData: %s\n", errno, strerror(errno));
			close(hSerialPort);
			hSerialPort = -1;
			throw CustomException(errno, "Failed to read data from serial port");
			//return 0;
		}
	}

	return recvdByte;
}
