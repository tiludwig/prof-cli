/*
 * PrintView.cpp
 *
 *  Created on: May 20, 2019
 *      Author: tim
 */

#include <View/PrintView.h>
#include <iostream>
#include <iomanip>

using namespace std;

void PrintView::setWCET(uint64_t wcet)
{
	this->wcet = wcet;
}

void PrintView::setCurrentExecutionTime(uint64_t current)
{
	this->current = current;
}

void PrintView::setRemainingTime(uint64_t seconds)
{
	this->remainingSeconds = seconds;
}

void PrintView::setMaximumIterations(uint64_t iterations)
{
	this->maxIterations = iterations;
}

void PrintView::setCurrentIteration(uint64_t iteration)
{
	this->currentIteration = iteration;
}

void PrintView::update()
{
	float percent = (float) currentIteration / maxIterations;
	int hours = remainingSeconds / 3600;
	int minutes = (remainingSeconds % 3600) / 60;
	int seconds = remainingSeconds % 60;

	cout << "\33[2K\rMeasuring: samples=" << fixed << currentIteration << ' ';
	cout << setw(3) << setprecision(0) << right << (100 * percent) << "% ";

	cout << '[' << setw(2) << hours << 'h' << setw(2) << minutes << 'm' << setw(2) << seconds << "s]";
	cout << " wcet=" << fixed << wcet;
	cout << ", current=" << fixed << current;
	fflush(stdout);
}

