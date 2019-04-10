/*
 * Application.cpp
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#include <Core/Application/Application.h>
#include <Core/Exceptions/CustomException.h>

Application::Application()
{
	bRunning = true;
	link = nullptr;
	pathManager = nullptr;
}

Application::~Application()
{

}

bool Application::initialize()
{
	// initialize the path environment
	createPathManager();
	if (pathManager->initialize() == false)
		return false;

	// initialize the configuration module
	loadConfigurationFromFile(pathManager->getBinarypath() + "config.ini");

	// initialize the input provider
	std::string inputprovider(pathManager->getPluginpath());
	inputprovider += configuration.getInputProviderName();
	inputProvider = TestinputProvider::loadPlugin(inputprovider.c_str());
	if (inputProvider.initialize(pathManager->getDatapath(), configuration["measurement.provider-args"]) == false)
	{
		return false;
	}

	// initialize user interface
	ui.initialize();
	ui.setMaximumSampleCount(configuration.getIterations());

	// initialize comdriver
	auto link = ComLinkFactory::CreateComLink(configuration.getComdriverType());
	if (link == nullptr)
	{
		return false;
	}

	if (link->initialize(configuration["core.device"]) == false)
	{
		return false;
	}

	// initialize communicator
	communicator.setLink(link);

	// initialize profiler
	profiler.setProfilingTarget(configuration.getSuTName(), communicator);

	return true;
}

void Application::stop()
{
	bRunning = false;
}

void Application::run()
{
	RemainingTime remainingTime;

	int iterations = configuration.getIterations();

	for (int i = 0; i < iterations; i++)
	{
		if (bRunning == false)
			break;

		/* initialize random seed: */
		//srand(time(NULL));
		/* generate delay number between 1 and 10: */
		//int delayTime = rand() % 5 + 1;
		//std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
		remainingTime.startMeasurement();
		auto measurement = profiler.profile(communicator, inputProvider);
		remainingTime.endMeasurement();

		//if ((i % 500) == 0)
		{
			ui.showMeasurementResult(measurement);
			ui.showWCET(profiler.getWCET());
			ui.showSampleCount(i);
			ui.updateOverallIterations(i);

			auto secondsLeft = remainingTime.calculateRemainingSeconds(iterations - i);

			ui.showRemainingTime(secondsLeft);
			ui.render();
		}

	}

	ui.destroy();

	auto wcet = profiler.getWCET();
	printf("Profiling done.\n");
	printf("Measured WCET: %lu cycles [%.2f us].\n", wcet, (wcet / 72.0));
	fflush(stdout);
	auto dist = profiler.getFrequencyDistribution();

	std::sort(dist->begin(), dist->end(), [](freqdist_entry_t a, freqdist_entry_t b)
	{	return a.value < b.value;});

	std::ofstream myfile;
	myfile.open("wcet-dist.csv");
	for (auto& slot : *dist)
		myfile << slot.value << "," << slot.counts << "\n";

	myfile.close();
}

bool Application::loadConfigurationFromFile(const std::string& filepath)
{
	std::ifstream stream(filepath);
	if (!stream.is_open())
	{
		throw CustomException("Unable to locate configuration file.");
		return false;
	}
	stream >> configuration;
	return true;
}

void Application::createPathManager()
{
#ifdef DEBUG
	pathManager = new DebugPathManager();
#else
	pathManager = new PathManager();
#endif
}

