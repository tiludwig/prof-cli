/*
 * IView.h
 *
 *  Created on: May 20, 2019
 *      Author: tim
 */

#ifndef VIEW_IVIEW_H_
#define VIEW_IVIEW_H_

#include <stdint.h>

class IView
{
public:

	virtual ~IView()
	{
	}

	virtual void setWCET(uint64_t wcet) = 0;
	virtual void setCurrentExecutionTime(uint64_t current) = 0;
	virtual void setRemainingTime(uint64_t seconds) = 0;
	virtual void setMaximumIterations(uint64_t iterations) = 0;
	virtual void setCurrentIteration(uint64_t iteration) = 0;
	virtual void update() = 0;
};

#endif /* VIEW_IVIEW_H_ */
