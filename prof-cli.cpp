#include <cstdio>
#include "Components/ComLink/SerialLink.h"
#include "Core/CommandReceiver/CommandReceiver.h"

#include <chrono>
#include <thread>

#include <iostream>

void waitForPacket(SerialLink& link, CommandReceiver& receiver)
{
	auto data = link.readData();
	while (receiver.process(data) == false)
	{
		data = link.readData();
	}
}

int main()
{
	SerialLink link;
	if (link.initialize() == false)
	{
		printf("Error initializing serial port\n");
		return 0;
	}

	CommandReceiver receiver;

	uint8_t msg[] = { '#', 10, 2, 0, '?', 3, 'i', 104 };
	uint8_t msg_2[] = { '#', 20, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, (uint8_t) -28 };

	int* acc_x = (int*) &msg_2[3];
	int* acc_y = (int*) &msg_2[7];

	while (true)
	{
		std::cout << "\nEnter acc_x: ";
		std::cin >> *acc_x;
		std::cout << "\nEnter acc_y: ";
		std::cin >> *acc_y;
		link.writeData(msg_2, 13);
		waitForPacket(link, receiver);

		link.writeData(msg, 8);
		waitForPacket(link, receiver);
		std::cin.get();
	}
	printf("Exiting\n");
}
