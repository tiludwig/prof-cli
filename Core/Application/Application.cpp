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
#include <cstdarg>

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

	// initialize user interface
	view = new PrintView();

	// initialize comdriver
	auto link = ComLinkFactory::CreateComLink(configuration.getCoreSetting("com-driver")); //.getComdriverType());
	if (link == nullptr)
	{
		return false;
	}

	if (link->initialize(configuration.getCoreSetting("driver-args")) == false)
	{
		return false;
	}

	// initialize communicator
	communicator.setLink(link);

	// initialize profiler
	//profiler.setProfilingTarget(configuration["test.taskname"], communicator);

	return true;
}

void Application::stop()
{
	bRunning = false;
}

void Application::initializeTest(TestConfiguration& config, std::string& outputFilename, unsigned int& iterations)
{

	RemainingTime remainingTime;
	log("Loading configuration for '%s'\n", config.name.c_str());

	outputFilename = config.name;
	std::replace(outputFilename.begin(), outputFilename.end(), ' ', '_');
	outputFilename += ".csv";

	iterations = std::stoul(config["iterations"]);
	view->setMaximumIterations(iterations);
	log("Test duration: %u iterations\n", iterations);

	// initialize the input provider
	std::string inputprovider(pathManager->getPluginpath());
	inputprovider += config["input-method"];
	log("Loading input plugin '%s'\n", inputprovider.c_str());
	inputProvider = TestinputProvider::loadPlugin(inputprovider.c_str());
	if (inputProvider.initialize(pathManager->getDatapath(), config["input-args"]) == false)
	{
		std::string message("Failed to initialize plugin '");
		message += inputprovider;
		message += "'";
		throw CustomException(message);
	}
	// assign the target task
	profiler.setProfilingTarget(config["taskname"], communicator);

	// reset the statistics for the next run
	statistics.reset();
}

uint32_t Application::runTest(RemainingTime& remainingTime)
{
	remainingTime.startMeasurement();
	auto measurement = profiler.profile(communicator, inputProvider);
	remainingTime.endMeasurement();

	if (measurement == INVALID_WCET)
	{
		log("Invalid WCET received. Ignoring for now.\n");
		return 0xFFFFFFFF;
	}
	else
	{
		statistics.update(measurement);

		result_t result = { (uint32_t) measurement };
		inputProvider.feedbackMeasurementResult(result);

		return measurement;
	}
}

void Application::run()
{
	int testCount = configuration.getTestCount();

	log("Detected %i tests\n", testCount);

	for (int i = 0; i < testCount; i++)
	{
		TestConfiguration& testConfig = configuration.getNextTest();
		log("Initializing test %i/%i: '%s'\n", (i + 1), testCount, testConfig.name.c_str());
		unsigned int iterations = 0;
		std::string outputFilename;
		int dumpToDiskIterations = 100;
		log("Dumping distribution to disk every %d iterations\n", dumpToDiskIterations);

		initializeTest(testConfig, outputFilename, iterations);

		RemainingTime remainingTime;

		for (unsigned int i = 0; i < iterations; i++)
		{
			if (bRunning == false)
				break;

			auto measurement = runTest(remainingTime);

			auto secondsLeft = remainingTime.calculateRemainingSeconds(iterations - i);

			view->setWCET(statistics.getMaximumExecutionTime());
			view->setCurrentExecutionTime(measurement);
			view->setCurrentIteration(i);
			view->setRemainingTime(secondsLeft);
			log("");
			view->update();

			if (dumpToDiskIterations-- <= 0)
			{
				writeToDisk(outputFilename, statistics.getFrequencyDistribution());
			}

		}

		log("\nProfiling done.\n");
		auto wcet = statistics.getMaximumExecutionTime();
		log("Measured WCET: %lu cycles [%.2f us].\n", wcet, (wcet / 72.0));

		auto dist = statistics.getFrequencyDistribution();
		std::sort(dist->begin(), dist->end(), [](freqdist_entry_t a, freqdist_entry_t b)
		{	return a.value < b.value;});

		writeToDisk(outputFilename, dist);
	}

}

void Application::writeToDisk(const std::string& outputFilename, std::vector<freqdist_entry_t>* dist)
{
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

void Application::log(const char* fmt, ...)
{
	va_list args;

	printf("[App]: ");
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}
