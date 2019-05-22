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

void Application::run()
{
	int testCount = configuration.getTestCount();

	log("Detected %i tests\n", testCount);

	for (int i = 0; i < testCount; i++)
	{
		TestConfiguration& testConfig = configuration.getNextTest();
		log("Initializing test %i/%i: '%s'\n", (i + 1), testCount, testConfig.name.c_str());

		RemainingTime remainingTime;
		int dumpToDiskIterations = 100;
		log("Dumping distribution to disk every %d iterations\n", dumpToDiskIterations);
		log("Loading configuration for '%s'\n", testConfig.name.c_str());

		std::string outputFilename(testConfig.name);
		std::replace(outputFilename.begin(), outputFilename.end(), ' ', '_');
		outputFilename += ".csv";

		unsigned int iterations = std::stoul(testConfig["iterations"]);
		view->setMaximumIterations(iterations);
		log("Test duration: %u iterations\n", iterations);


		// initialize the input provider
		std::string inputprovider(pathManager->getPluginpath());
		inputprovider += testConfig["input-method"];
		log("Loading input plugin '%s'\n", inputprovider.c_str());
		inputProvider = TestinputProvider::loadPlugin(inputprovider.c_str());
		if (inputProvider.initialize(pathManager->getDatapath(), testConfig["input-args"]) == false)
		{
			std::string message("Failed to initialize plugin '");
			message += inputprovider;
			message += "'";
			throw CustomException(message);
		}


		profiler.reset();
		profiler.setProfilingTarget(testConfig["taskname"], communicator);


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
			log("");
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

		log("\nProfiling done.\n");
		auto wcet = profiler.getWCET();
		uint32_t baseline = 0;

		log("Measured WCET: %lu cycles [%.2f us].\n", wcet, (wcet / 72.0));
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
