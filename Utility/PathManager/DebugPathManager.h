/*
 * DebugPathManager.h
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#ifndef UTILITY_PATHMANAGER_DEBUGPATHMANAGER_H_
#define UTILITY_PATHMANAGER_DEBUGPATHMANAGER_H_

#include "PathManager.h"

class DebugPathManager: public PathManager
{
public:
	virtual std::string getPluginpath() const;
	virtual std::string getBinarypath() const;
	virtual std::string getDatapath() const;
};

#endif /* UTILITY_PATHMANAGER_DEBUGPATHMANAGER_H_ */
