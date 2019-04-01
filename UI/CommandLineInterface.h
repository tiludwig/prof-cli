/*
 * CommandLineInterface.h
 *
 *  Created on: Apr 1, 2019
 *      Author: tim
 */

#ifndef UI_COMMANDLINEINTERFACE_H_
#define UI_COMMANDLINEINTERFACE_H_

#include <stdint.h>
namespace UI
{

class CommandLineInterface
{
private:
	int32_t bottomLine;
	int32_t progressBarCurrentTotal;
	int32_t progressBarTotalTotal;
public:
	void initialize(int32_t iterations, int32_t updateInterval);

	void printRemainingTime(uint64_t secondsRemaining);
	void updateCurrentProgressbar(int32_t value);
	void updateTotalProgressbar(int32_t value);

	void printResult(uint32_t value);
	void printState(uint32_t* stateVec);
	void printCounter(uint32_t value);
};

} /* namespace UI */

#endif /* UI_COMMANDLINEINTERFACE_H_ */
