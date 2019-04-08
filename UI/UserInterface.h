/*
 * UserInterface.h
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#ifndef UI_USERINTERFACE_H_
#define UI_USERINTERFACE_H_

#include <stdint.h>
#include <ncurses.h>

#include "ProgressBar.h"

class UserInterface
{
protected:
	WINDOW* logWindow;
	WINDOW* statusWindow;

	ProgressBar timeTillFinishedProgressBar;

	uint32_t wcet;
	uint32_t cycles;
	uint32_t samples;
	uint32_t uiIterations;
	uint32_t iterations;
public:
	UserInterface();
	~UserInterface();

	// text functions
	void log(const char* format, ...);
	// initialize
	void initialize();

	void setMaximumSampleCount(int maxSamples)
	{
		timeTillFinishedProgressBar.setMaximum(maxSamples);
	}

	void showWCET(uint32_t wcet);
	void showMeasurementResult(uint32_t cycles);
	void showSampleCount(uint32_t sampleCount);
	void showRemainingTime(uint64_t remainingSeconds);
	void updateUiIterations(uint32_t uiIterations);
	void updateOverallIterations(uint32_t iterations);

	void render();
};

#endif /* UI_USERINTERFACE_H_ */
