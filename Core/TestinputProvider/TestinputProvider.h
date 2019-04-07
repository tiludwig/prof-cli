/*
 * TestinputProvider.h
 *
 *  Created on: Apr 6, 2019
 *      Author: tim
 */

#ifndef CORE_TESTINPUTPROVIDER_TESTINPUTPROVIDER_H_
#define CORE_TESTINPUTPROVIDER_TESTINPUTPROVIDER_H_

#include <datatypes.h>

typedef bool (*InitFunc)();
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
	bool initialize();

	buffer_t getNextDataset();

	void feedbackMeasurementResult(result_t result);

	static TestinputProvider loadPlugin(const char* path);
};

#endif /* CORE_TESTINPUTPROVIDER_TESTINPUTPROVIDER_H_ */
