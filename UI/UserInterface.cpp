/*
 * UserInterface.cpp
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#include "UserInterface.h"

UserInterface::UserInterface()
{
	statusWindow = nullptr;
	logWindow = nullptr;
	wcet = 0;
	cycles = 0;
	uiIterations = 0;
	iterations = 0;
	samples = 0;
	destroyed = false;
}

UserInterface::~UserInterface()
{
	destroy();
}

void UserInterface::log(const char* format, ...)
{
}

void UserInterface::initialize()
{
	initscr();
	logWindow = newwin(14, 80, 9, 0);
	statusWindow = newwin(8, 80, 0, 0);

	box(logWindow, 0, 0);
	box(statusWindow, 0, 0);
	mvwprintw(logWindow, 0, 2, "Log output");
	mvwprintw(statusWindow, 0, 2, "Status");

	mvwprintw(statusWindow, 1, 1, "Overall progress");
	timeTillFinishedProgressBar.setMaximum(1000);
	timeTillFinishedProgressBar.setParent(statusWindow);
	timeTillFinishedProgressBar.setLocation(18, 1);
	timeTillFinishedProgressBar.setSize(58, 1);

}

void UserInterface::showWCET(uint32_t wcet)
{
	this->wcet = wcet;
}

void UserInterface::showMeasurementResult(uint32_t cycles)
{
	this->cycles = cycles;
}

void UserInterface::showSampleCount(uint32_t sampleCount)
{
	this->samples = sampleCount;
}

void UserInterface::updateUiIterations(uint32_t uiIterations)
{
}

void UserInterface::updateOverallIterations(uint32_t iterations)
{
	timeTillFinishedProgressBar.setValue(iterations);
}

void UserInterface::showRemainingTime(uint64_t secondsRemaining)
{
	int hours = secondsRemaining / 3600;
	secondsRemaining -= (hours * 3600);
	int minutes = secondsRemaining / 60;
	secondsRemaining -= (minutes * 60);

	mvwhline(statusWindow, 3, 1, ' ', 80);
	mvwprintw(statusWindow, 3, 1, "Remaining time: %dh%dm%ds", hours, minutes, secondsRemaining);
	refresh();
}

void UserInterface::destroy()
{
	if (!destroyed)
	{
		destroyed = true;
		delwin(logWindow);
		delwin(statusWindow);
		endwin();
	}
}

void UserInterface::render()
{
	timeTillFinishedProgressBar.draw();

	mvwhline(statusWindow, 4, 1, ' ', 80);
	mvwhline(statusWindow, 5, 1, ' ', 80);
	mvwhline(statusWindow, 6, 1, ' ', 80);
	mvwprintw(statusWindow, 4, 1, "Measured execution time: %u", cycles);
	mvwprintw(statusWindow, 5, 1, "Measured WCET: %u", wcet);
	mvwprintw(statusWindow, 6, 1, "Sample count: %u", samples);

	box(logWindow, 0, 0);
	box(statusWindow, 0, 0);
	mvwprintw(logWindow, 0, 2, "Log output");
	mvwprintw(statusWindow, 0, 2, "Status");

	refresh();
	wrefresh(logWindow);
	wrefresh(statusWindow);
}
