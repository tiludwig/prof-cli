/*
 * Application.cpp
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#include <Core/Application/Application.h>
#include <Core/Exceptions/CustomException.h>
#include "View/PrintView.h"
#include <algorithm>

Application::Application()
{
	bRunning = true;
	link = nullptr;
	pathManager = nullptr;
	view = nullptr;
}

Application::~Application()
{
	delete view;
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
	inputprovider += configuration["test.input-method"];//.getInputProviderName();
	inputProvider = TestinputProvider::loadPlugin(inputprovider.c_str());
	if (inputProvider.initialize(pathManager->getDatapath(), configuration["test.input-args"]) == false)
	{
		return false;
	}

	// initialize user interface
	view = new PrintView();

	// initialize comdriver
	auto link = ComLinkFactory::CreateComLink(configuration["core.com-driver"]);//.getComdriverType());
	if (link == nullptr)
	{
		return false;
	}

	if (link->initialize(configuration["core.driver-args"]) == false)
	{
		return false;
	}

	// initialize communicator
	communicator.setLink(link);

	// initialize profiler
	profiler.setProfilingTarget(configuration["test.taskname"], communicator);

	return true;
}

void Application::stop()
{
	bRunning = false;
}

void Application::run()
{
	printf("[App] Entering run-loop\n");
	RemainingTime remainingTime;
	int dumpToDiskIterations = 100;
	printf("[App] Dumping distribution to disk every %d iterations\n", dumpToDiskIterations);
	printf("[App] Loading configuration for '%s'\n", configuration["test.display-name"].c_str());

	std::string outputFilename(configuration["test.display-name"]);
	std::replace(outputFilename.begin(), outputFilename.end(), ' ', '_');
	outputFilename += ".csv";

	unsigned int iterations = std::stoul(configuration["test.iterations"]);//.getIterations();
	view->setMaximumIterations(iterations);
	printf("[App] Test duration: %u iterations\n", iterations);
	for (unsigned int i = 0; i < iterations; i++)
	{
		if (bRunning == false)
			break;

		remainingTime.startMeasurement();
		auto measurement = profiler.profile(communicator, inputProvider);
		remainingTime.endMeasurement();

		result_t result = { (uint32_t) measurement };
		inputProvider.feedbackMeasurementResult(result);

		auto secondsLeft = remainingTime.calculateRemainingSeconds(iterations - i);

		view->setWCET(profiler.getWCET());
		view->setCurrentExecutionTime(measurement);
		view->setCurrentIteration(i);
		view->setRemainingTime(secondsLeft);
		view->update();

		if (dumpToDiskIterations-- <= 0)
		{
			auto dist = profiler.getFrequencyDistribution();

			std::ofstream myfile;

			myfile.open(outputFilename, std::ofstream::out | std::ofstream::trunc);
			for (auto& slot : *dist)
				myfile << slot.value << "," << slot.counts << "\n";

			myfile.close();
		}

	}

	printf("\nProfiling done.\n");
	auto wcet = profiler.getWCET();
	uint32_t baseline = 0;

	// maybe subtract the baseline measurement
	std::string strWcet;
	if (configuration.getValueIfExists("test.baseline-wcet", strWcet) == true)
	{
		baseline = std::stoul(strWcet);
	}

	printf("Measured WCET: %lu cycles [%.2f us].\n", wcet, (wcet / 72.0));
	printf("Adjusted WCET: %lu cycles [%.2f us].\n", (wcet - baseline), ((wcet - baseline) / 72.0));
	fflush(stdout);
	auto dist = profiler.getFrequencyDistribution();

	std::sort(dist->begin(), dist->end(), [](freqdist_entry_t a, freqdist_entry_t b)
	{	return a.value < b.value;});

	std::ofstream myfile;
	myfile.open(outputFilename, std::ofstream::out | std::ofstream::trunc);
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

