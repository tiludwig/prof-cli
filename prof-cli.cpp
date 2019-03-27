
#include <cstdio>
#include "Components/ComLink/SerialLink.h"
#include "Core/CommandReceiver/CommandReceiver.h"

int main()
{
	SerialLink link;
	if(link.initialize() == false)
	{
		printf("Error initializing serial port\n");
		return 0;
	}

	CommandReceiver receiver;

	while(true)
	{
		auto data = link.readData();
		receiver.process(data);
	}
	printf("Exiting\n");
}
