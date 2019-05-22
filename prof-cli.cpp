#include <cstdio>
#include <Core/Application/Application.h>
#include <Core/Exceptions/CustomException.h>
#include <cstring>
#include <Core/Configuration/ConfigManager.h>

Application app;

void signalHandler(int sig)
{
	if (sig != SIGINT)
	{
		return;
	}

	app.stop();
}

int main(int argc, char** argv)
{
	signal(SIGINT, signalHandler);
	try
	{
		if (app.initialize() == false)
		{
			printf("An unexpected error occured during initialization.\nTerminating application.\n");
			return -1;
		}
		app.run2();

	} catch (CustomException& ex)
	{
		printf("Error: %s\n\rErrorcode: %d\n\rSystem message: %s\r\n", ex.getMessage(), ex.getErrorCode(),
				strerror(ex.getErrorCode()));
	} catch (const char* ex)
	{
		printf("Error: %s\n", ex);
	}

	return 0;
}
