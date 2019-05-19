/*
 * PrintView.h
 *
 *  Created on: May 20, 2019
 *      Author: tim
 */

#ifndef VIEW_PRINTVIEW_H_
#define VIEW_PRINTVIEW_H_

#include "IView.h"

class PrintView: public IView
{
	uint64_t wcet;
	uint64_t current;
	uint64_t remainingSeconds;
	uint64_t maxIterations;
	uint64_t currentIteration;
public:
	virtual void setWCET(uint64_t wcet);
	virtual void setCurrentExecutionTime(uint64_t current);
	virtual void setRemainingTime(uint64_t seconds);
	virtual void setMaximumIterations(uint64_t iterations);
	virtual void setCurrentIteration(uint64_t iteration);
	virtual void update();
};

#endif /* VIEW_PRINTVIEW_H_ */
