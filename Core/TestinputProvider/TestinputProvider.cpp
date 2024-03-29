/*
 * TestinputProvider.cpp
 *
 *  Created on: Apr 6, 2019
 *      Author: tim
 */

#include <Core/TestinputProvider/TestinputProvider.h>
#include <Core/Exceptions/CustomException.h>
#include <dlfcn.h>

bool TestinputProvider::initialize(const std::string& pluginpath, const std::string& pluginArgs)
{
	return initFunc(pluginpath.c_str(), pluginArgs.c_str());
}

buffer_t TestinputProvider::getNextDataset()
{
	return getNextDatasetFunc();
}

void TestinputProvider::feedbackMeasurementResult(result_t result)
{
	feedbackResultFunc(result);
}

TestinputProvider TestinputProvider::loadPlugin(const char* path)
{
	TestinputProvider provider;
	provider.handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
	if(provider.handle == nullptr)
	{
		printf("%s\n", dlerror());
		throw CustomException("Unable to open datagenerator plugin");
	}
	provider.initFunc = reinterpret_cast<InitFunc>(dlsym(provider.handle, "initialize"));
	provider.getNextDatasetFunc = reinterpret_cast<GetNextSetFunc>(dlsym(provider.handle, "getNextDataset"));
	provider.feedbackResultFunc = reinterpret_cast<FeedbackResultFunc>(dlsym(provider.handle, "feedbackMeasurementResult"));
	return provider;
}
