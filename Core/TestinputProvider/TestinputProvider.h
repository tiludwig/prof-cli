/*
 * TestinputProvider.h
 *
 *  Created on: Apr 6, 2019
 *      Author: tim
 */

#ifndef CORE_TESTINPUTPROVIDER_TESTINPUTPROVIDER_H_
#define CORE_TESTINPUTPROVIDER_TESTINPUTPROVIDER_H_

#include <datatypes.h>
#include <string>

typedef bool (*InitFunc)(std::string&);
typedef buffer_t (*GetNextSetFunc)();
typedef void (*FeedbackResultFunc)(result_t);

class TestinputProvider
{
private:
	void* handle;
	InitFunc initFunc;
	GetNextSetFunc getNextDatasetFunc;
	FeedbackResultFunc feedbackResultFunc;
public:
	bool initialize(std::string& pluginpath);

	buffer_t getNextDataset();

	void feedbackMeasurementResult(result_t result);

	static TestinputProvider loadPlugin(const char* path);
};

#endif /* CORE_TESTINPUTPROVIDER_TESTINPUTPROVIDER_H_ */
