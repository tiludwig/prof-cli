/*
 * Application.h
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#ifndef CORE_APPLICATION_APPLICATION_H_
#define CORE_APPLICATION_APPLICATION_H_

#include <cstdio>
#include <Components/ComLink/SerialLink.h>
#include <Core/Communicator/PacketCommunicator.h>
#include <Core/DataLink/Packet.h>
#include <Utility/MinMax.hpp>
#include <Utility/Variance.hpp>
#include <Utility/FrequencyDistribution.h>
#include <UI/CommandLineInterface.h>
#include <iostream>
#include <chrono>
#include <signal.h>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <thread>
#include <sstream>
#include <unistd.h>
#include <limits.h>

#include <Core/Configuration/Configuration.h>
#include <Core/Configuration/ConfigManager.h>
#include <Core/TestinputProvider/TestinputProvider.h>

#include <Factories/ComLinkFactory.h>
#include <Utility/PathManager/PathManager.h>
#include <Utility/PathManager/DebugPathManager.h>
#include <Core/Profiler/Profiler.h>
#include <Utility/RemainingTime/RemainingTime.h>

#include "View/IView.h"

class Application
{
private:
	PathManager* pathManager;
	ConfigManager configuration;
	TestinputProvider inputProvider;
	IView* view;
	IComLink* link;
	PacketCommunicator communicator;
	Profiler profiler;

	bool bRunning;
private:
	void createPathManager();
	bool loadConfigurationFromFile(const std::string& filepath);
public:
	Application();
	~Application();

	bool initialize();
	void run();
	void run2();

	void stop();
};

#endif /* CORE_APPLICATION_APPLICATION_H_ */
